#pragma once

#include <QStandardItem>
#include "duiwidget.h"

class DW_DUIFW_EXPORT DuiTreeModelItem : public QStandardItem
{
public:
	enum ModelItemType
	{ 
		FolderType = UserType + 1,
		ItemType = UserType + 2,
        PlainType = UserType + 3,
	};

	explicit DuiTreeModelItem(const QString &text);
	DuiTreeModelItem(const QString &icon, const QString &text);
	~DuiTreeModelItem();

	int type() const;
	void setType(int type);

	virtual DuiTreeModelItem *clone() const;  

private:
	int m_type;
};

class DuiTreeViewItemPrivate;

class DW_DUIFW_EXPORT DuiTreeViewItem : public DuiWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiTreeViewItem)

	friend class DuiTreeItemContainer;

public:
	DuiTreeViewItem(QStandardItem *item, QGraphicsItem *parent);
	~DuiTreeViewItem();

	virtual DuiTreeViewItem *createItem( QStandardItem *modelItem, QGraphicsItem *parent );

	QStandardItem *modelItem() const;

	bool isExpanded() const;
	void setExpanded(bool expanded);
	void setSelected( bool selected );
	bool isSelected() const;
	int	indent() const;
	void setImagePath( const QString &imagePath );
	QString imagePath() const;

	virtual QString toolTip() const;
	virtual bool isFolder() const;

	virtual void setAcceptDoubleClick(bool isAcceptDoubleClick);
	virtual bool isAcceptDoubleClick() const;

protected:
	DuiTreeViewItem(DuiTreeViewItemPrivate &dd, QStandardItem *item, QGraphicsItem *parent);

	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

	virtual void dataChanged();
	virtual void expandedChanged();
	virtual void selectedChanged();
	virtual QSizeF calcPreferredSize( qreal width ) const;
    virtual QSizeF preferredSize() const;
	virtual bool isChildrenCollapsible() const;
};

//////////////////////////////////////////////////////////////////////////

class DuiCategoryHeaderItemPrivate;

class DW_DUIFW_EXPORT DuiCategoryHeaderItem : public DuiTreeViewItem
{
	Q_DECLARE_PRIVATE(DuiCategoryHeaderItem)
public:
	DuiCategoryHeaderItem( QStandardItem *item, QGraphicsItem *parent );
	~DuiCategoryHeaderItem();

	virtual DuiTreeViewItem *createItem( QStandardItem *modelItem, QGraphicsItem *parent );

protected:
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF */) const;
	virtual void doColorize( QRgb color );
	virtual void expandedChanged();
	virtual bool isFolder() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
};

class DW_DUIFW_EXPORT DuiCategoryItem : public DuiTreeViewItem
{
public:
	DuiCategoryItem(QStandardItem *item, QGraphicsItem *parent);

	virtual DuiTreeViewItem *createItem( QStandardItem *modelItem, QGraphicsItem *parent );

protected:
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF */) const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
};
