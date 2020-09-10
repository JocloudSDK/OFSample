
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QMargins>

#include "dwglobal.h"

//------------------------------------------------------------------------------
class DuiFrameItemDrawerPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiFrameItemDrawer
{
	Q_DISABLE_COPY(DuiFrameItemDrawer)
	Q_DECLARE_PRIVATE(DuiFrameItemDrawer)

public:
	enum FrameType
	{
		Undefined = 0,
		OnePiece,
		ThreePiecesHorizontal,		// 伸展两端
		ThreePiecesHorizontalMid,	// 伸张中间
		ThreePiecesVertical,
		ThreePiecesVerticalMid,
		NinePieces
	};
	DuiFrameItemDrawer(const QString &frameGraphicName, FrameType type);
	DuiFrameItemDrawer(const QPixmap &frameGraphic, FrameType type);
	~DuiFrameItemDrawer();

	void paint(QPainter *painter, const QRect &frameRect, const QMargins &frameMargin, int idx = 0) const;

public:
	static int string2Enum(const QString& str);

private:
	void easyDraw( QPainter * painter, const QRect &frameRect, const QMargins & frameMargin ) const;
	void drawOnePiece(QPainter* painter, const QRect &frameRect, const QMargins &frameMargin ) const;
	void drawThreePiecesVertical(QPainter* painter , const QRect &frameRect, const QMargins &frameMargin ) const;
	void drawThreePiecesVerMid(QPainter* painter, const QRect &frameRect, const QMargins &frameMargin ) const;
	void drawThreePiecesHorizontal( QPainter * painter, const QRect &frameRect, const QMargins & frameMargin ) const;
    void setClipMargins(QPainter * painter, const QRect &frameRect, const QMargins & frameMargin) const;

private:
	DuiFrameItemDrawerPrivate* d_ptr;
};
