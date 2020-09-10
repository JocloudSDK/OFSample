
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QPixmap>
#include <QEvent>
#include "dwglobal.h"

class DW_DUIFW_EXPORT DuiColorizeEvent : public QEvent
{
public:
	DuiColorizeEvent(QRgb color);

	QRgb color() const;

	static int EventType;

private:
	QRgb m_color;
};

class DW_DUIFW_EXPORT DuiTheme
{
	Q_DISABLE_COPY(DuiTheme);

public:
	enum ColorType
	{
		InvalidColor = 0,
		MoreLightColor = 1,
		MidLightColor = 2,
		LightColor = 3, //多用于画分隔线
		BaseColor = 5,
		DarkColor = 7,//多用于控件着色
		MidDarkColor = 8,
		MoreDarkColor = 9
	};

	enum LinearGradientType
	{
		NoGradient = -1,
		MainFrameGradient = 0,
		ThickTitleGradient = 1, 
		DialogGradient = 2,
		PopupGradient = 3,		
		SettingframeGradient = 4,
		ChannelGradient = 5,
		ImGroupGradient = 6,
		LargeMainFrameGradient =7,
	};

	enum FillMode
	{
		ExtendFill = 0,
		TiledFill = 1,

		//Alias of Extend
		DefaultFill = 0,
	};

	enum ThemeStyle
	{
		DarkTheme = 0,
		LightTheme,
	};

	static QPixmap getImage(const QString &fileName);

	static QRgb getColor(ColorType type = BaseColor);
	static int getColorTheme();
	static QRgb getCurrentPerceptualColor();
	static void setCurrentPerceptualColor(const QColor &c);

	static QImage colorizeImage(const QImage &image, QColor color);
	static QPixmap decorationImage();
	static QPixmap largeAreaDecorationImage();
	static void setDecorationImage(const QImage &newDecorate, const QColor decorateColor = QColor(),
		const QColor avgColor = QColor(), FillMode mode = ExtendFill, ThemeStyle themeStyle = DuiTheme::DarkTheme);

	static QColor decorationBackgroundColor();
	static FillMode decorationFillMode();
	static ThemeStyle themeStyle();

	static QRegion calcFrameClipRegion(const QRect &rect);
	static void drawSelectedRect(QPainter *painter, const QRect &rect);
	static void drawHoveredRect(QPainter *painter, const QRect &rect);
	static void drawNinePiecesBoarder(QPainter *p, const QRect &destRect, const QPixmap &image);
	static bool setColorThemeOnly(int colorTheme);

private:
	//deprecated functions
	static bool setColorTheme(int colorTheme);
	static QPixmap getColorizeImage(const QString &fileName);
	DuiTheme(); //unconstructable
};
