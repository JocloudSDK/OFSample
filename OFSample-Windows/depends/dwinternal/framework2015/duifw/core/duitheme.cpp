#include "stable.h"

#include "duitheme.h"
#include "duiresourceloader.h"
#include "duicolorthemedef.h"
#include "utility/averagecolor.h"
#include "utility/duicolorformat.h"
#include "dwbase/perflog.h"
#include "duidrawutil.h"

int DuiColorizeEvent::EventType = QEvent::registerEventType();

void broacastThemeChanged()
{
	PerfLogF(F_DUI);
	QWidgetList topWidgets = qApp->topLevelWidgets();
	foreach (QWidget* widget, topWidgets)
	{
		qApp->postEvent( widget, new DuiColorizeEvent(DuiTheme::getColor(DuiTheme::BaseColor)) );
	}
}

DuiColorizeEvent::DuiColorizeEvent( QRgb color )
: QEvent( QEvent::Type(EventType) )
, m_color(color)
{
}

QRgb DuiColorizeEvent::color() const
{
	return m_color;
}


struct DuiThemeData
{
	DuiThemeData()
		: m_currentColorTheme(0)
		, decorateFadeoutMask(":/theme/duifw/decorate_fadeout.png")
		, LargeAreaDecorateFadeoutMask(":/theme/duifw/largearea_decorate_fadeout.png")
		, m_currentPerceptualColor(_colorThemeData[0].colors[DuiTheme::BaseColor])
		, themeStyle(DuiTheme::DarkTheme)
	{
	}

	QPixmap currentDecorateImage;
	QPixmap largeAreaDecorateImage;//大面积球形皮肤，提供给im与群使用
	DuiTheme::FillMode fillMode;
	DuiTheme::ThemeStyle themeStyle;
	QPixmap decorateFadeoutMask;
	QPixmap LargeAreaDecorateFadeoutMask;

	int currentColorTheme() const 
	{
		return m_currentColorTheme;
	}

	QColor currentPerceptualColor() const
	{
		return m_currentPerceptualColor;
	}

	void setPerceptualColor(const QColor &c)
	{
		m_currentPerceptualColor = c;
	}

	void setRecommendColor(const QColor &c)
	{
		m_currentColorTheme = calcNearestColorTheme(c);
	}

	// 此函数只供游戏大厅对应窗口锁定蓝色皮肤用
	bool setColorTheme(int colorTheme)
	{
		DW_ASSERT(colorTheme >= 0 && colorTheme < 16);

		if(colorTheme < 0 || colorTheme >= 16)
		{
			colorTheme = 0;
		}

		if(m_currentColorTheme != colorTheme)
		{
			m_currentColorTheme = colorTheme;
			return true;
		}
		return false;
	}

private:
	int calcNearestColorTheme(const QColor &c)
	{
		PerfLogF(F_DUI);
		double cie_l, cie_a, cie_b;
		Dui::rgb2lab(&cie_l, &cie_a, &cie_b, c.red(), c.green(), c.blue());

		int nearestIndex = -1;
		double labDelta = DBL_MAX;

		for(int i = 0; i < DW_ARRAY_SIZE(_colorThemeData); i++)
		{
			QColor baseColor = _colorThemeData[i].colors[DuiTheme::BaseColor];

			double cie_l2, cie_a2, cie_b2;
			Dui::rgb2lab(&cie_l2, &cie_a2, &cie_b2, baseColor.red(), baseColor.green(), baseColor.blue());
			double delta = (cie_a2 - cie_a) * (cie_a2 - cie_a) + (cie_b2 - cie_b)  * (cie_b2 - cie_b);

			if(delta < labDelta)
			{
				labDelta = delta;
				nearestIndex = i;
			}
		}
		return nearestIndex;
	}

private:
	QColor m_currentPerceptualColor;
	int m_currentColorTheme;
};

Q_GLOBAL_STATIC(DuiThemeData, _themeData);

QPixmap DuiTheme::getImage(const QString &fileName)
{
	if(fileName.isEmpty())
	{
		//sometimes we are using empty image file name.
		return QPixmap();
	}

	return getResourceLoader()->loadImage(fileName);
}

QRgb DuiTheme::getColor(ColorType type)
{
	int theme = _themeData()->currentColorTheme();
	return _colorThemeData[theme].colors[type];
}

QRgb DuiTheme::getCurrentPerceptualColor()
{
	return _themeData()->currentPerceptualColor().rgba();
}

void DuiTheme::setCurrentPerceptualColor(const QColor &c)
{
	return _themeData()->setPerceptualColor(c);
}

QPixmap DuiTheme::getColorizeImage( const QString &fileName )
{
	if(fileName.isEmpty())
	{
		return QPixmap();
	}

	QPixmap p = getResourceLoader()->loadImage(fileName);
	QImage image = p.toImage();

	int wd = image.width();
	int ht = image.height();

	int h = _colorThemeData[_themeData()->currentColorTheme()].baseHue;
	int s = _colorThemeData[_themeData()->currentColorTheme()].baseSatuations;
	for(int y = 0; y < ht; y++)
	{
		for(int x = 0; x < wd; x++)
		{
			QRgb c = image.pixel(x, y);
			QColor color(c);
			color.setHsl(h, s, qGray(c), qAlpha(c));
			image.setPixel(x, y, color.rgba());
		}
	}

	return QPixmap::fromImage(image);
}

int DuiTheme::getColorTheme()
{
	return _themeData()->currentColorTheme();
}

bool DuiTheme::setColorTheme( int colorTheme )
{
	bool res = setColorThemeOnly(colorTheme);
	if(res)
	{
		broacastThemeChanged();
	}
	return res;
}

bool DuiTheme::setColorThemeOnly(int colorTheme)
{
	return _themeData()->setColorTheme(colorTheme);
}

QImage DuiTheme::colorizeImage(const QImage &image, QColor color)
{
	QImage destImage;
	QRect destRect(QPoint(0, 0), image.size());
	QImage srcImage = image.convertToFormat(image.hasAlphaChannel() ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32);
	destImage = QImage(srcImage.size(), srcImage.format());

	// do colorizing
	QPainter destPainter(&destImage);
	destPainter.drawImage(destRect, srcImage);
	destPainter.setCompositionMode(QPainter::CompositionMode_Screen);
	destPainter.fillRect(destRect, color);
	destPainter.end();

	if (srcImage.hasAlphaChannel())
	{
		destImage.setAlphaChannel(srcImage.alphaChannel());
	}
	return destImage;
}

//static
QPixmap DuiTheme::decorationImage()
{
	return _themeData()->currentDecorateImage;
}

QPixmap DuiTheme::largeAreaDecorationImage()
{
	return _themeData()->largeAreaDecorateImage;
}

//static
QColor DuiTheme::decorationBackgroundColor()
{
	return _themeData()->currentPerceptualColor();
}

//static
DuiTheme::FillMode DuiTheme::decorationFillMode()
{
	return _themeData()->fillMode;
}

DuiTheme::ThemeStyle DuiTheme::themeStyle()
{
	return _themeData()->themeStyle;
}

//static
void DuiTheme::setDecorationImage(const QImage &newDecorate, const QColor decorateColor /* = QColor()*/,
								  const QColor avgColor/* = QColor() */, DuiTheme::FillMode mode /* = ExtendFill */, DuiTheme::ThemeStyle themeStyle/* = DuiTheme::DarkTheme*/)
{
	PERFLOG;
	//decorate image may contain user input, we must validate it first
	DW_ASSERT(!newDecorate.isNull());

	_themeData()->themeStyle = themeStyle;

	QImage decorateImage = newDecorate;

	//add alpha channel, or we can't draw decorate mask on it with alpha blend
	if(!decorateImage.hasAlphaChannel() || decorateImage.depth() != 32)
	{
		decorateImage = decorateImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	}

	QColor perceptualColor;
	if (decorateColor.isValid())
	{
		perceptualColor = decorateColor;
	}
	else
	{
		perceptualColor = calculatePerceptualBodyColor(decorateImage);
	}

	QColor recommendColor = perceptualColor;
	if( avgColor.isValid() )
	{
		recommendColor = avgColor;
	}

	_themeData()->setPerceptualColor(perceptualColor);
	_themeData()->setRecommendColor(recommendColor);

	//blur edge with decoration image
	QImage largeAreaDecorateImage = decorateImage;
	if(mode == DuiTheme::ExtendFill)
	{
		QPainter px(&largeAreaDecorateImage);
		px.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
		DuiDrawUtil::drawPixmap(&px, largeAreaDecorateImage.rect(), _themeData()->LargeAreaDecorateFadeoutMask);
		px.end();

		QPainter p(&decorateImage);
		p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
        DuiDrawUtil::drawPixmap(&p, decorateImage.rect(), _themeData()->decorateFadeoutMask);
		p.end();
	}
	_themeData()->fillMode = mode;
	_themeData()->currentDecorateImage = QPixmap::fromImage(decorateImage);
	_themeData()->largeAreaDecorateImage = QPixmap::fromImage(largeAreaDecorateImage);
	//decorateImage.save("d:/xxx_decorateImage.png");
	//largeAreaDecorateImage.save("d:/xxx_largeAreaDecorateImage.png");
	broacastThemeChanged();
}

QRegion DuiTheme::calcFrameClipRegion( const QRect &rect )
{
	QRect rects[5] = 
	{
        QRect(2, 0, rect.width() - 4, 1),
		QRect(1, 1, rect.width() - 2, 1),
        QRect(0, 2, rect.width(), rect.height() - 4),

		QRect(1, rect.bottom()-1, rect.width() - 2, 1),
        QRect(2,rect.bottom()-0, rect.width() - 4, 1)
	};

	int x = rect.left();
	int y = rect.top();
	for(int i = 0; i < sizeof(rects) / sizeof(QRect); i++)
	{
		rects[i].adjust(x, y, x, y);
	}

	QRegion rgn;
	rgn.setRects(rects, DW_ARRAY_SIZE(rects));

	return rgn;
}

void DuiTheme::drawSelectedRect(QPainter *painter, const QRect &rect)
{
	painter->fillRect(rect, DuiTheme::getCurrentPerceptualColor());
	if (DuiTheme::themeStyle() != DuiTheme::LightTheme)
	{
		QPixmap pix = DuiTheme::getImage(":/theme/duifw/item_selected_mask.png");
		drawNinePiecesBoarder(painter, rect, pix);
	}
}

void DuiTheme::drawHoveredRect(QPainter *painter, const QRect &rect)
{
	painter->fillRect(rect, DuiTheme::getCurrentPerceptualColor());
	if (DuiTheme::themeStyle() != DuiTheme::LightTheme)
	{
		QPixmap pix = DuiTheme::getImage(":/theme/duifw/item_hovered_mask.png");
		drawNinePiecesBoarder(painter, rect, pix);
	}
}

void DuiTheme::drawNinePiecesBoarder(QPainter *p, const QRect &destRect, const QPixmap &image)
{
	PerfLogF(F_DUI);
	int sourceWidth = image.width();
	int sourceHeight = image.height();
	int destWidth = destRect.width() > sourceWidth ? sourceWidth : destRect.width();
	int destHeight = destRect.height() > sourceHeight ? sourceHeight : destRect.height();
	QMargins sourceMargin(sourceWidth / 3, sourceHeight / 3, sourceWidth / 3, sourceHeight / 3);
	QMargins targetMargin(destWidth / 3, destHeight / 3, destWidth / 3, destHeight / 3);
	qDrawBorderPixmap(p, destRect, targetMargin, image, image.rect(), sourceMargin, QTileRules(Qt::StretchTile));
}
