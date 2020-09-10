\
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiframeitem.h"

class DuiFrameItem;
enum DuiFrameItem::DrawStyle;

class DW_DUIFW_EXPORT DuiFrameItemEx : public DuiWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY(DuiFrameItemEx);
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiFrameItemEx);

	Q_PROPERTY( QString imagePath READ imagePath WRITE setImage);
	Q_PROPERTY(bool acceptClick READ acceptClick WRITE setAcceptClick)
	Q_PROPERTY(bool acceptHover READ acceptHover WRITE setAcceptHover)
	Q_PROPERTY(bool acceptDoubleClick READ acceptDoubleClick WRITE setAcceptDoubleClick)
	Q_PROPERTY(bool acceptRightClick READ acceptRightClick WRITE setAcceptRightClick)
	Q_PROPERTY( QString frameType READ frameTypeStr WRITE setFrameType )

public:
	explicit DuiFrameItemEx(QGraphicsItem *parent = NULL);
	DuiFrameItemEx(const QString &frameImageName, DuiFrameItemDrawer::FrameType type, QGraphicsItem *parent);

	void setImage(const QString &image);
	void setImage(const QString &imageNormal, const QString &imageHover, const QString &imageDown);
	void setAcceptClick(bool b);
	void setAcceptHover(bool b);
	void setAcceptDoubleClick(bool b);
	void setAcceptRightClick(bool b);

	void setColorizePolicy( ColorizeWidgetFlag flag);	// 只是设置背景图片是否换肤，子元素一定不换肤

	void setPressed(bool pressed);

	void setFrameType( const QString& frameType );
	int frameType() const;
	void setFrameType(int type);

	QString	 frameTypeStr() const;

	void setDrawStyle( DuiFrameItem::DrawStyle style );
	DuiFrameItem::DrawStyle drawStyle() const;

	virtual void colorize(QRgb color);
	virtual void setThemeColorType( int colorType );

signals:
	void clicked();
	void doubleClicked();
	void rightClicked();
	void entered();
	void leaved();

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent * event);

	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	virtual void resizeEvent(QGraphicsSceneResizeEvent * event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

private:
	QString imagePath() const;
	bool acceptClick() const;
	bool acceptHover() const;
	bool acceptDoubleClick() const;
	bool acceptRightClick() const;

private:
	DuiFrameItem *m_background;
	bool m_acceptClick;
	bool m_acceptHover;
	bool m_acceptDoubleClick;
	bool m_acceptRightClick;
};