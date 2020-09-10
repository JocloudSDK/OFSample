
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"
class QStandardItem;

class DuiAbstractViewItemPrivate;

class DW_DUIFW_EXPORT DuiAbstractViewItem : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiAbstractViewItem)
	Q_DECLARE_PRIVATE(DuiAbstractViewItem)

public:
	DuiAbstractViewItem( QStandardItem *item, QGraphicsItem *parent );

	QStandardItem *modelItem() const;

	bool selected() const;
	void setSelected(bool selected);
	bool isHover() const;
	void setIsHover( bool isHover );
	virtual void selectChanged();
	virtual void activated();

	virtual DuiAbstractViewItem *createItem( QStandardItem *item, QGraphicsItem *parent ) = NULL;
	virtual QSizeF calcPreferredSize(qreal width) const;
	virtual void dataChanged();

protected:
	DuiAbstractViewItem( DuiAbstractViewItemPrivate &dd, QStandardItem *item, QGraphicsItem *parent );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );
};
