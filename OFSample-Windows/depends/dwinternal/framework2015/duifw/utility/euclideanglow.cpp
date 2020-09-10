#include "euclideanglow.h"

//Reference:
//[1] An Efficient Euclidean Distance Transform
//Donal G Bailey, Institute of Information Science and Technology
//Massey University, Palmerston North, New Zealand
//D.G.Bailey@massey.ac.nz

static const size_t kStockedSquaredTableSizeLimit = 4096; //4 * 4k page
static const quint32 *g_stockedSquaredTable = NULL;

//generate square look up table without multiplication
//x^2 = 0 if x == 0
//x^2 = (x - 1)^2 + 2x - 1 if x > 0
const quint32* generateSquaredTable(size_t size)
{
	Q_ASSERT(size);
	size++;
	quint32 *table = new quint32[size];
	table[0] = 0;
	for(size_t i = 1; i < size; ++i) {
		table[i] = table[i - 1] + (i << 1) - 1;
	}
	return table;
}

const quint32* getStockedSquaredTable()
{
	if(!g_stockedSquaredTable) {
		g_stockedSquaredTable = generateSquaredTable(kStockedSquaredTableSizeLimit);
	}
	return g_stockedSquaredTable;
}

const quint32* getSquaredTable(size_t size)
{
	if(size < kStockedSquaredTableSizeLimit) {
		return getStockedSquaredTable();
	} else {
		return generateSquaredTable(size);
	}
}

static void releaseSquaredTable(const quint32 *table)
{
	if(g_stockedSquaredTable != table) {
		delete[] table;
	}
}

//Bresenham's Line Drawing Algorithm
template<typename plot_type>
void lineDown(int x0, int x1, int y0, int y1, plot_type &plot)
{
	//caller promise x0 < x1 && y0 > y1
	Q_ASSERT(x0 < x1 && y0 > y1);
	if(x1 - x0 > y0 - y1) {
		//non steep version, draw from top left
		int deltaX = x1 - x0;
		int deltaY = y0 - y1;
		int error = deltaX >> 1;
		for(int x = x0, y = y0; x <= x1; ++x) {
			plot(x, y);
			error -= deltaY;
			if(error < 0) {
				error += deltaX;
				--y;
			}
		}
	} else {
		//flipped steep version, draw from bottom right
		int deltaX = x1 - x0;
		int deltaY = y0 - y1;
		int error = deltaY >> 1;
		int count = 0;
		for(int y = y1, x = x1; y <= y0; ++y) {
			error -= deltaX;
			++count;
			if(error < 0) {
				error += deltaY;
				//we need value mapping rather than line drawing
				//plot once per x value is good enough
				plot(x, y - (count >> 1));
				--x;
				count = 0;
			}
		}
	}
}

struct SquaredPlot {
	quint8 *buffer;
	const quint32 *squareTable;

	void operator() (int x, int y) {
		buffer[squareTable[x]] = y;
	}
};

struct Plot {
	quint8 *buffer;

	void operator() (int x, int y) {
		buffer[x] = y;
	}
};

//generate color table correspond to squared euclidean distance,
//which is more accurate and much prettier
//double Bresenham's Line Drawing Algorithm
//1st pass is used to generate alpha value corresponding to distance
//2nd pass is used to generate parabola inscribe sections
static inline quint8* createSquaredColorTable(int size, int spread, int alpha, const quint32 *squareTable)
{
	int squaredSize = squareTable[size];
	QScopedArrayPointer<quint8> fastLookupTable(new quint8[squaredSize + 1]);
	int turningPoint = size * spread / 100;

	fastLookupTable[0] = alpha;
	fastLookupTable[squaredSize] = 0;
	for(int i = 1, end = squareTable[turningPoint]; i < end; ++i) {
		fastLookupTable[i] = alpha;
	}

	//1st pass, calculate section points on parabola
	SquaredPlot sqp;
	sqp.buffer = fastLookupTable.data();
	sqp.squareTable = squareTable;
	lineDown(turningPoint, size, alpha, 0, sqp);
	//2nd pass, draw sections
	Plot plot;
	plot.buffer = fastLookupTable.data();
	for(int i = turningPoint; i < size; ++i) {
		int begin = squareTable[i];
		int end = squareTable[i + 1];
		lineDown(begin, end, fastLookupTable[begin], fastLookupTable[end], plot);
	}
	return fastLookupTable.take();
}

//very very special purpose data structure
//can only be push in back end, but pop from both end
//use fix buffer to store element, store only fix number of elements a round
//NO element COPY CONSTRUCT/DESTRUCT is handle, use POD only
//-- naming myth --
//the paper[1] say it uses a stack, but remove bottom element like a queue
//so, we name it staque, for combination of stack and queue, pronouce the same as stack
//written as staque, read as stack!
template<typename Ty_>
struct Staque {
	Staque(int size) : m_buf(new Ty_[size]) {
		//do not use initializer, assign order must be specific
		m_allocEnd = m_buf.data() + size;
		m_front = m_buf.data();
		m_back = m_buf.data();
	}

	bool isEmpty() {
		Q_ASSERT(m_front <= m_back);
		return m_front == m_back;
	}

	Ty_* bottom() {
		return m_front;
	}

	Ty_* top() {
		return m_back - 1;
	}

	void pop_front() {
		++m_front;
		Q_ASSERT(m_front <= m_back);
	}

	void push_back() {
		++m_back;
		Q_ASSERT(m_back < m_allocEnd);
	}

	void pop_back() {
		--m_back;
		Q_ASSERT(m_front <= m_back);
	}

	void reset() {
		m_front = m_buf.data();
		m_back = m_buf.data();
	}

	size_t size() {
		return m_back - m_front;
	}
	private:
		QScopedArrayPointer<Ty_> m_buf;
		Ty_ *m_allocEnd;
		Ty_ *m_front;
		Ty_ *m_back;
};

struct VoronoiBoundary {
	QRgb point;
	int y;
	int boundary;

	void assign(QRgb p, int ny, int b) {
		point = p;
		y = ny;
		boundary = b;
	}

	QRgb Isqr() {
		return point;
	}
};

static inline bool condition(QRgb c)
{
	return 0xff000000 & c;
}

//XXX:optimize out one multiplication by understanding QImage internal buffer structure
//if Qt change QImage buffer layout, these code WILL FAIL
static inline QRgb* incr_scanline(QRgb *r, int bpl) {
	return (QRgb*)(((uchar*)r) + bpl);
}

static inline QRgb* decr_scanline(QRgb *r, int bpl) {
	return (QRgb*)(((uchar*)r) - bpl);
}

QRgb kInfinite = 0xffffffff;

void HorizontalScan(QImage &src, const quint32 *squareTable)
{
	int width = src.width();
	int height = src.height();

	for(int y = 0; y < height; ++y) {
		QRgb *scanline= (QRgb*)src.scanLine(y);
		quint32 runLength = 0;
		//indicate current run length is not from image edge
		bool engaged = false;
		for(int x = 0; x < width; ++x) {
			if(condition(scanline[x])) {
				scanline[x] = 0;
				if(engaged) {
					runLength >>= 1;
				} else {
					engaged = true;
				}
				//scan reach edge, look back and correct distance from the this side
				if(runLength != 0) {
					for(quint32 i = 1; i <= runLength; ++i) {
						scanline[x - i] = squareTable[i];
					}
					runLength = 0;
				}
			} else {
				++runLength;
				if(engaged) {
					scanline[x] = squareTable[runLength];
				} else {
					scanline[x] = kInfinite;
				}
			}
		}
	}
}

QImage outerGlow(const QPixmap &pixmap, QRgb color, int size, int spread, int alpha)
{
	Q_ASSERT(alpha >= 0 && alpha <= 255);
	if(alpha <= 0 || alpha > 255 || size == 0) {
		//invalid or invisible
		QImage inv(pixmap.size(), QImage::Format_ARGB32);
		inv.fill(0);
		return inv;
	}

	QImage src = pixmap.toImage();
	//we use not premultiplied version, cause we've got to manipulate alpha channel by hand
	if(src.format() != QImage::Format_ARGB32) {
		src = src.convertToFormat(QImage::Format_ARGB32);
	}
	int width = src.width();
	int height = src.height();

	QRgb *imageBuffer = (QRgb*)src.bits();
	color &= 0x00ffffff;
	QRgb objectBackground = color | alpha << 24;

	const quint32 *squareTable = getSquaredTable(qMax(size, qMax(width, height)));
	QScopedArrayPointer<quint8> colorTable(createSquaredColorTable(size, spread, alpha, squareTable));
	QScopedArrayPointer<QRgb> verticalLine(new QRgb[height]);
	QRgb squaredSize = squareTable[size];

	//round 1: HorizontalScan, generate distance map on x axis
	HorizontalScan(src, squareTable);

	int bpl = src.bytesPerLine();
	Staque<VoronoiBoundary> staque(height);
	//round 2: column scan, generate final result
	for(int x = 0; x < width; ++x) {
		//pass 1: from top to bottom
		QRgb *row = imageBuffer + x;
		QRgb *dst = verticalLine.data();
		//set the destination buffer to infinite
		qMemSet(dst, 0xff, height * sizeof(QRgb));
		int y = 0;
		//scan trapped in object boundary
		bool trapped = false;
		staque.reset();
		while(y < height) {
			if(*row == 0) {
				trapped = true;
			} else {
				if(trapped) {
					trapped = false;
					staque.reset();
					staque.push_back();
					//refer [1] 3.2.1
					staque.top()->assign(0, y - 1, height);
				}
				bool has_furthur_influnce = true;
				//3.2.2 is not true, don't skip
				if(*row != kInfinite) {
					//3.2.3
					if(!staque.isEmpty()) {
						VoronoiBoundary *bot = staque.bottom();
						QRgb Isqr_new = bot->Isqr() + squareTable[y - bot->y]; //Eq.(16)
						//XXX(the paper is wrong, > shall be replace with <):3.2.4
						if(*row < Isqr_new) {
							//3.2.5 will put current row into staque, and then break to 3.2.10
							staque.reset();
							//and then skip 3.2.10, so, we reach 3.2.11
							has_furthur_influnce = false;
						}
					}
					//use a while loop to manage flow control, avoid notorious goto
					//continue go back to 3.2.5, and break go straigh down to 3.2.10
					while(true) {
						//3.2.5
						if(staque.isEmpty()) {
							staque.push_back();
							staque.top()->assign(*row, y, height);
							break;	//goto 3.2.10
						} else if(*row < staque.top()->Isqr()) {
							//3.2.6
							staque.pop_back();
							continue; //back to 3.2.5
						} else {
							//3.2.7
							int deltaY = y - staque.top()->y;
							//Eq. (14)
							int boundary = y + ((int(*row - staque.top()->Isqr() - squareTable[deltaY]) / deltaY) >> 1);
							if(boundary >= height) {
								//skip this row, set Isqr to unlimited
								*row = kInfinite;
								break; //goto 3.2.10
								//notice: staque.top() - 1 is protected by "or short circuit"
							} else if(staque.size() <= 1 || (staque.top() - 1)->boundary < boundary) {
								//3.2.8, keep and update top of staque
								staque.top()->boundary = boundary;
								staque.push_back();
								staque.top()->assign(*row, y, height);
								break;
							} else {
								//3.2.9, pop and continue
								staque.pop_back();
								continue;
							}
						}
					}
				}
				//3.2.10
				if(has_furthur_influnce && !staque.isEmpty()) {
					VoronoiBoundary *bot = staque.bottom();
					QRgb Isqr_new = bot->Isqr() + squareTable[y - bot->y];
					if(Isqr_new < *row) {
						*dst = Isqr_new;
						*row = kInfinite;
					} else {
						*dst = *row;
					}
					if(bot->boundary < y + 1) {
						staque.pop_front();
					}
				} else {
					//unite me!
					*dst = *row;
				}
			}
			//3.2.11
			++y;
			++dst;
			row = incr_scanline(row, bpl);
		}

		//XXX:too coward to copy code snippet, review me
		//pass 2: from bottom to top
		//reset aux variables
		y = height - 1;
		trapped = false;
		staque.reset();
		row = decr_scanline(row, bpl);
		dst--;

		while(y >= 0) {
			if(*row == 0) {
				trapped = true;
			} else {
				if(trapped) {
					trapped = false;
					staque.reset();
					staque.push_back();
					//refer [1] 3.3.1, but we don't build skip table at 3.2.1
					staque.top()->assign(0, y + 1, -1);
				}
				bool has_furthur_influnce = true;
				//3.3.2 is not true, don't skip
				if(*row != kInfinite) {
					//3.3.3
					if(!staque.isEmpty()) {
						VoronoiBoundary *bot = staque.bottom();
						//reversed Eq.(16), coz bot->y > y
						QRgb Isqr_new = bot->Isqr() + squareTable[bot->y - y];
						//XXX(the paper is wrong, > shall be replace with <):3.3.4, one more condiction checked
						if(*row < Isqr_new || *dst < Isqr_new) {
							//3.3.5 will put current row into staque, and then break to 3.3.10
							staque.reset();
							//and then skip 3.3.10, so, we reach 3.3.11
							has_furthur_influnce = false;
						}
					}
					//use a while loop to manage flow control, avoid notorious goto
					//continue go back to 3.3.5, and break go straigh down to 3.3.10
					while(true) {
						//3.3.5
						if(staque.isEmpty()) {
							staque.push_back();
							staque.top()->assign(*row, y, -1);
							break;	//goto 3.3.10
						} else if(*row < staque.top()->Isqr()) {
							//3.3.6
							staque.pop_back();
							continue; //back to 3.3.5
						} else {
							//3.3.7
							int deltaY = y - staque.top()->y;
							//Eq. (14)
							int boundary = y + ((int(*row - staque.top()->Isqr() - squareTable[-deltaY]) / deltaY) >> 1);
							if(boundary < 0) {
								//skip this row, but updating Isqr is no more needed
								break; //goto 3.3.10
								//notice: staque.top() - 1 is protected by "or short circuit"
								//NOTICE:direction changed!
							} else if(staque.size() <= 1 || (staque.top() - 1)->boundary > boundary) {
								//3.3.8, keep and update top of staque
								staque.top()->boundary = boundary;
								staque.push_back();
								staque.top()->assign(*row, y, -1);
								break;
							} else {
								//3.3.9, pop and continue
								staque.pop_back();
								continue;
							}
						}
					}
				}
				//3.3.10
				if(has_furthur_influnce && !staque.isEmpty()) {
					VoronoiBoundary *bot = staque.bottom();
					//reverse order!
					QRgb Isqr_new = bot->Isqr() + squareTable[bot->y - y];
					if(Isqr_new < *dst) {
						*dst = Isqr_new;
					}
					if(bot->boundary > y - 1) {
						staque.pop_front();
					}
				}
			}
			//do pixel manipulation
			if(*row) {
				*row = *dst > squaredSize ? 0 : (colorTable[*dst] << 24 | color);
			} else {
				*row = objectBackground;
			}
			//3.2.11
			--y;
			--dst;
			row = decr_scanline(row, bpl);
		}
	}

	releaseSquaredTable(squareTable);
	return src;
}
