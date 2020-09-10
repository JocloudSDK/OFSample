#include "stable.h"

//---
//calculatePerceptualBodyColor can extract perceptual color of a image
//it utilize a pattern recognition technique named color segmentation
//---
//Reference:
//1. A Novel Vector-Based Approach to Color Image Retrieval
//Using a Vector Angular-Based Distance Measure
//D. Androutos, K. N. Plataniotis, and A. N. Venetsanopoulos
//Computer Vision and Image Understanding Vol. 75, Nos. 1/2, July/August. pp 46-58 1999
//Article ID cviu.1999.0767 available online at http://www.idealibrary.com
//---
//2. Color Image Segmentation Using Histogram Thresholding ¨C Fuzzy C-means Hybrid Approach
//Khang Siang Tan, Nor Ashidi Mat Isa
//Pattern Recognition 44 (2011) 1¨C15

//from Qt.Network.q_static_assert
namespace {
    template<bool> struct StaticAssertFailed;
    template<> struct StaticAssertFailed<true> { enum { Value = 1 }; };
}

static inline void qt_noop_with_arg(int) {}
#define q_static_assert(expr)   qt_noop_with_arg(sizeof(StaticAssertFailed< expr >::Value))
//end of q_static_assert

//WARNING!:caller of calculateXXX function listed below must notice:
//image shall contain alpha channel, or these function WILL FAIL!
QRgb calculateAverageColor(const QImage &image)
{
	int height = image.height();
	int width = image.width();
	int red = 0;
	int blue = 0;
	int green =0;
	for(int i = 0; i < height; ++i) {
		const QRgb* line = (const QRgb*)image.constScanLine(i);
		for(int j = 0; j < width; ++j) {
			if(qAlpha(line[j]) > 200) {
				red += qRed(line[j]);
				green += qGreen(line[j]);
				blue += qBlue(line[j]);
			}
		}
	}
	red /= height * width;
	green /= height * width;
	blue /= height * width;
	return qRgb(red, green, blue);
}

QRgb calculateRightBottomAverageColor(const QImage &image)
{
	int height = image.height();
	int width = image.width();
	if(height < 100 || width < 100) {
		if(image.depth() != 32) {
			return calculateAverageColor(image.convertToFormat(QImage::Format_ARGB32_Premultiplied));
		} else {
			return calculateAverageColor(image);
		}
	} else {
		QImage v(100, height, QImage::Format_ARGB32_Premultiplied);
		QImage h(width, 100, QImage::Format_ARGB32_Premultiplied);
		QPixmap pix = QPixmap::fromImage(image);

		QPainter pv(&v);
		QPainter ph(&h);

		qDrawBorderPixmap(&pv, v.rect(), QMargins(0, 0, 100,0), pix);
		qDrawBorderPixmap(&ph, h.rect(), QMargins(0, 0, 0,100), pix);

		QRgb cv = calculateAverageColor(v);
		QRgb ch = calculateAverageColor(h);

		return qRgb((qRed(cv) + qRed(ch)) / 2, (qGreen(cv) + qGreen(ch)) / 2, (qBlue(cv) + qBlue(ch)) / 2);
	}
}

static qreal KPi = 3.14159265;
//experimental value from [1]
static int KBrightValueThreshold = 255 * 0.75;
static int KDarkValueThreshold = 255 * 0.25;
//not 0.2 here, we find 20/255 make it look much better
static int KSaturationThreshold = 20;
//static int KSaturationThreshold = 255 * 0.2;
static int KTorelantThreshold = 180 * 0.1 / KPi; //0.1 rad
//experimental value from [2]
static int KDominatingPeakThreshold = 20;

struct AnalyticPixel
{
	AnalyticPixel()
		: color(), next(NULL)
	{}

	QColor color;
	AnalyticPixel *next;
};

//simple single link list
//NOTICE:AnalyticPixelSet do NOT own AnalyticPixel
//it's just a helper class for sorting them out
struct AnalyticPixelSet {
	AnalyticPixelSet()
		: count(0), m_head(NULL), m_tail(NULL)
	{}

	AnalyticPixelSet(const AnalyticPixelSet& rhs)
		: count(rhs.count), m_head(rhs.m_head), m_tail(rhs.m_tail)
	{}

	void add(AnalyticPixel* new_m_head) {
		new_m_head->next = m_head;
		m_head = new_m_head;
		if(m_tail == NULL) {
			m_tail = new_m_head;
			m_tail->next = NULL;
		}
		++count;
	}

	void assign(const AnalyticPixelSet& rhs) {
		count = rhs.count;
		m_head = rhs.m_head;
		m_tail = rhs.m_tail;
	}

	void merge(const AnalyticPixelSet& rhs) {
		if(m_tail == NULL) {
			assign(rhs); //in case this is empty
		} else {
			count += rhs.count;
			m_tail->next = rhs.m_head;
			m_tail = rhs.m_tail ? rhs.m_tail : m_tail;
		}
	}

	QColor centroid() const {
		if(count == 0) {
			return QColor();
		}
		AnalyticPixel *i = m_head;
		int red = 0;
		int blue = 0;
		int green =0;
		while(i) {
			int r, g, b;
			i->color.getRgb(&r, &g, &b);
			red += r;
			green += g;
			blue += b;
			i = i->next;
		}
		red /= count;
		green /= count;
		blue /= count;
		return QColor(red, green, blue);
	}

	int count;
	AnalyticPixel *m_head;
	AnalyticPixel *m_tail;
};

typedef int(*HistogramKeyExtractorType)(AnalyticPixel *pix);
typedef QList<AnalyticPixelSet> segments_type;

template<HistogramKeyExtractorType extractor, int range, int half_window_size>
class Histogram {
	public:

		Histogram()
			: m_regulatedBins(new int[range]),
				m_pixelSets(new AnalyticPixelSet[range]) {
			//range must contain at least window size + 1 room
			//or windowing produre will fail with buffer overrun
			q_static_assert(half_window_size * 2 + 2 <= range);
			q_static_assert(3 <= range);
		}

		void add(AnalyticPixel *pix) {
			int key = extractor(pix);
			DW_ASSERT(key < range);

			m_pixelSets[key].add(pix);
		}

		void clear() {
			//leave m_regulatedBins as is, fuzzySegmentation will over write it completely
			//AnalyticPixelSet is memset safe
			::qMemSet(m_pixelSets.data(), 0, range * sizeof(AnalyticPixelSet));
		}

		//smooth-windowing with half_window_size, and then fuzzy local peak removing
		//then perform segmentation with domniating peaks
		//WARNING:call this function ONCE AND ONLY ONCE after construction/cleared
		//pixels inside histogram are gone with return regions
		segments_type fuzzySegmentation()
		{
			windowing();
			QList<int> peaks;
			QList<int> dominatingPeak;
			QList<int> valleys;
			segments_type regions;
			int cur = 1;
			int end = range - 1;
			do {
				int val = m_regulatedBins[cur];
				int left = m_regulatedBins[cur - 1];
				int right = m_regulatedBins[cur + 1];
				if(left < val && val > right) {
					peaks.push_back(cur); //f'(x) = 0 and f''(x) < 0
				} else if(left > val && val < right) {
					valleys.push_back(cur); //f'(x) = 0 and f''(x) > 0
				}
				++cur;
			} while(cur < end);

			//fuzzy smoothen peak
			foreach(int i, peaks) {
				if(m_regulatedBins[i - 1] < m_regulatedBins[i + 1]) {
					m_regulatedBins[i] = m_regulatedBins[i + 1];
				} else {
					m_regulatedBins[i] = m_regulatedBins[i - 1];
				}
			}

			//fuzzy smoothen valley
			foreach(int i, valleys) {
				if(m_regulatedBins[i - 1] < m_regulatedBins[i + 1]) {
					m_regulatedBins[i] = m_regulatedBins[i - 1];
				} else {
					m_regulatedBins[i] = m_regulatedBins[i + 1];
				}
			}

			//examine in-window average derivation to tell dominating peaks from local peaks
			foreach(int i, peaks) {
				int left = i - half_window_size;
				left = left >= 0 ? left : 0;
				int right = i + half_window_size;
				right = right < range ? right : range - 1;

				if(m_regulatedBins[i] > m_regulatedBins[left]
						&& m_regulatedBins[right] < m_regulatedBins[i]
						&& m_regulatedBins[i] > KDominatingPeakThreshold) {
					dominatingPeak.push_back(i);
				}
			}

			int start = 0;
			end = dominatingPeak.size();
			//divide regions by mean point
			for(int i = 0; i < end; ++i) {
				int mean = i + 1 == end ? range - 1 : (dominatingPeak[i] + dominatingPeak[i + 1]) / 2;
				regions.push_back(merge(start, mean));
				start = mean + 1;
			}
			return regions;
		}

		AnalyticPixelSet merge(int start, int end) {
			DW_ASSERT(start <= end);
			AnalyticPixelSet res;
			while(start <= end) {
				res.merge(m_pixelSets[start]);
				++start;
			}
			return res;
		}

	private:
		void windowing() {
			AnalyticPixelSet *left = m_pixelSets.data();
			int *cur = m_regulatedBins.data() + half_window_size;
			AnalyticPixelSet *right = left + half_window_size * 2;
			AnalyticPixelSet *end = left + range;

			//clean up unused bins
			for(int i = 0; i < half_window_size; ++i) {
				m_regulatedBins[i] = 0;
				m_regulatedBins[range - i - 1] = 0;
			}

			int value = 0;
			//calculate initial current point value
			for(AnalyticPixelSet *p = left; p <= right; ++p) {
				value += p->count;
			}

			//fill m_regulatedBins with average value inside window
			while(right < end - 1) {
				*cur = value;
				//slide window by one
				++cur;
				value -= left->count;
				++left;
				++right;
				value += right->count;
			}
			//last value
			*cur = value;
		}

	private:
		QScopedArrayPointer<int> m_regulatedBins;
		QScopedArrayPointer<AnalyticPixelSet> m_pixelSets;
};


int extractSaturation(AnalyticPixel *pix) {
	DW_ASSERT(pix->color.isValid() && pix->color.spec() == QColor::Hsv);
	return pix->color.hsvSaturation();
}

int extractHue(AnalyticPixel *pix) {
	DW_ASSERT(pix->color.isValid() && pix->color.spec() == QColor::Hsv);
	return ((ushort)pix->color.hsvHue()) % 360; //make sure hue value is in range [0, 360)
}

typedef Histogram<extractSaturation, 256, 2> SaturationHistogram;
typedef Histogram<extractHue, 360, 2> HueHistogram;


#ifdef IMAGE_PROCESSING_DEBUG
void divideByChromatic(const QImage &image) {
	QImage whitePillar(image.width(), image.height(), image.format());
	QImage blackCone(image.width(), image.height(), image.format());
	QImage brightChromaticDonut(image.width(), image.height(), image.format());
	QImage chromaticPudding(image.width(), image.height(), image.format());

	int height = image.height();
	int width = image.width();
	for(int i = 0; i < height; ++i) {
		const QRgb* line = (const QRgb*)image.constScanLine(i);
		for(int j = 0; j < width; ++j) {
			qreal h, s, v;
			QColor tmp(line[j]);
			tmp.getHsvF(&h, &s, &v);
			tmp.setAlpha(v > 0.75 && s >= 0.2 ? 255: 0);
			brightChromaticDonut.setPixel(j, i, tmp.rgba());
			tmp.setAlpha(v > 0.75 && s < 0.2 ? 255: 0);
			whitePillar.setPixel(j, i, tmp.rgba());
			tmp.setAlpha(v < 0.25 ? 255 : 0);
			blackCone.setPixel(j, i, tmp.rgba());
			tmp.setAlpha(v >= 0.25 && v < 0.75 ? 255 : 0);
			chromaticPudding.setPixel(j, i, tmp.rgba());
		}
	}
	whitePillar.save("d:\\whitePillar.png", "png");
	blackCone.save("d:\\blackCone.png", "png");
	brightChromaticDonut.save("d:\\brightChromaticDonut.png", "png");
	chromaticPudding.save("d:\\chromaticPudding.png", "png");
}

#define storeSegmentationResult() \
do { \
	QImage res(normalizedImage.width(), normalizedImage.height(), normalizedImage.format()); \
	foreach(const AnalyticPixelSet& i, regions) { \
		QColor centroid = i.centroid(); \
		QRgb rgb = centroid.rgb(); \
		AnalyticPixel* pix = i.m_head; \
		while(pix) { \
			AnalyticPixel* tmp = pix->next; \
			int off = pix - pixels.data(); \
			res.setPixel(off%width, off/width, rgb); \
			pix = tmp; \
		} \
	} \
	\
	res.save(QString("d:\\results\\%1.png").arg(regions.size()), "png"); \
} while(0)
#else
#define storeSegmentationResult() do{}while(0)
#endif

inline QImage scaleDown(const QImage &image) {
	int height = image.height();
	int width = image.width();
	if(height * width > 512 * 512) {
		return image.scaledToWidth(512 / sqrtf(qreal(height) / width));
	}
	return image;
}

//we use multimodal-saturation feature extraction algorithm from [1]
//with refined fuzzy histogram thresholding method from [2]
//to find dominating segments, and then calculate perceptual color
QRgb calculatePerceptualBodyColor(const QImage &image)
{
	//make sure image is in ARGB32Premultiple format!
	QImage normalizedImage = scaleDown(image);
	if(!normalizedImage.hasAlphaChannel()) {
		normalizedImage = normalizedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	}
	int height = normalizedImage.height();
	int width = normalizedImage.width();

	//AnalyticPixelSet whitePillar;
	AnalyticPixelSet blackCone;
	HueHistogram chromaticHueHistogram;
	SaturationHistogram chromaticPudding;
	segments_type regions;
	int ignoredPixels = 0;

	
	QScopedArrayPointer<AnalyticPixel> pixels(new AnalyticPixel[width * height]);
	AnalyticPixel *p = pixels.data();
	for(int i = 0; i < height; ++i) {
		const QRgb* line = (const QRgb*)normalizedImage.constScanLine(i);
		for(int j = 0; j < width; ++j) {
			if(qAlpha(line[j]) < 200) {
				++ignoredPixels;
				continue;
			}
			QColor &color = p->color;
			color.setRgb(line[j]);
			color = color.toHsv();

			if(color.value() > KBrightValueThreshold) {
#if 0
				if(color.hsvSaturation() >= KSaturationThreshold) {
					//bright chromatics, gone!
					chromaticHueHistogram.add(p);
				} else {
					//white, gone!
					whitePillar.add(p);
				}
#endif
			} else if(color.value() < KDarkValueThreshold) {
				//black
				blackCone.add(p);
			} else {
				chromaticPudding.add(p);
			}
			++p;
		}
	}

	regions.append(blackCone);
	//regions.append(whitePillar);
	//regions.append(chromaticHueHistogram.fuzzySegmentation());
	const segments_type &chromaticRegions = chromaticPudding.fuzzySegmentation();

	//we re-use the chromaticHueHistogram histogram, for the sake of saving a couple of heap alloc
	foreach(const AnalyticPixelSet& satModal, chromaticRegions) {
		chromaticHueHistogram.clear();
		AnalyticPixel* pix = satModal.m_head;
		while(pix) {
			AnalyticPixel* tmp = pix->next;
			chromaticHueHistogram.add(pix);
			pix = tmp;
		}
		regions.append(chromaticHueHistogram.fuzzySegmentation());
	}

	//debug code
	storeSegmentationResult();

	int weight = 0;
	const AnalyticPixelSet *result = 0;
	foreach(const AnalyticPixelSet& i, regions) {
		if(i.count > weight) {
			weight = i.count;
			result = &i;
		}
	}

	//if biggest segment failed to cover less than 20%
	//we fall back to edge average method
	if(!result || qreal(weight) / (height * width - ignoredPixels) < 0.2) {
		return calculateRightBottomAverageColor(normalizedImage);
	}
	return result->centroid().rgb();
}
