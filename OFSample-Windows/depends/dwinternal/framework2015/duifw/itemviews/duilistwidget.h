
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"
#include "duiscrollbar.h"
#include "duiscrollarea.h"
#include "duiabstractitemview.h"
#include "duiabstractviewitem.h"

class DuiListWidgetPrivate;
class QStandardItem;
struct DuiDragDropDataFormater;

class DW_DUIFW_EXPORT DuiListItem : public DuiAbstractViewItem
{
	Q_OBJECT;
public:
	DuiListItem();
	~DuiListItem();

signals:
	void	itemSelected();

protected:
	DuiListItem(QStandardItem *item, QGraphicsItem *parent);
	virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

public:
	virtual QSizeF calcPreferredSize(qreal width) const;
	virtual DuiListItem *createItem(QStandardItem *item, QGraphicsItem *parent);
};
//////////////////////////////////////////////////////////////////////////

class DuiDefaultListItemPrivate;

class DW_DUIFW_EXPORT DuiDefaultListItem : public DuiAbstractViewItem
{
	Q_DECLARE_PRIVATE(DuiDefaultListItem)
public:
	DuiDefaultListItem();
	~DuiDefaultListItem();

	virtual DuiDefaultListItem *createItem(QStandardItem *item, QGraphicsItem *parent);
	virtual QSizeF calcPreferredSize(qreal width) const;

protected:
	DuiDefaultListItem(QStandardItem *item, QGraphicsItem *parent);

	void init();

	virtual void resizeEvent( QGraphicsSceneResizeEvent *event );
	virtual void activated();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
};

//////////////////////////////////////////////////////////////////////////

class DW_DUIFW_EXPORT DuiListColorItem : public DuiListItem
{
	Q_DISABLE_COPY( DuiListColorItem )
public:
	DuiListColorItem();
	~DuiListColorItem();

	virtual DuiListColorItem *createItem(QStandardItem *item, QGraphicsItem *parent);
	virtual QSizeF calcPreferredSize(qreal width) const;

protected:
	DuiListColorItem(QStandardItem *item, QGraphicsItem *parent);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

//////////////////////////////////////////////////////////////////////////

class DW_DUIFW_EXPORT DuiListWidget : public DuiAbstractItemView
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiListWidget )
	Q_DECLARE_PRIVATE( DuiListWidget )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiListWidget )
public:
	enum DisplayMode{NormalMode,FishEyeMode};

	enum SelectMode{SingleSelection, MultiSelection};

public:
	explicit DuiListWidget(QGraphicsItem *parent = NULL);
	DuiListWidget(DuiListWidgetPrivate &dd, QGraphicsItem *parent = NULL);
	~DuiListWidget();

	void addItemPrototype( DuiAbstractViewItem *prototype );
	void setSelectIndex( int index , bool isSelected );
	void selectIndex(int index);
	int indexSelected() const;
	DuiAbstractViewItem* viewItemAt(int index) const;
	DuiAbstractItemContainer *container() const;
	void setFixItemHeight(int fixItemHeight);
	void beginInsertRows();
	void endInsertRows(int start, int end);

	void setDisplayMode( DisplayMode mode);
	DisplayMode displayMode() const;

	//鱼眼模式数据.
	//selectedHeight	:选中时的高度.
	//preSelectHeight	:当前选中的上下两个item高度
	void setFishEyeData(qreal selectedHeight, qreal preSelectHeight );

	void setSelectionMode(SelectMode mode);
	void setDrapDropDataFormater(DuiDragDropDataFormater* formater);

signals:
	void leftButtonClicked(int index);
	void rightButtonClicked(int index);
	void rightButtonMultiClicked(const QList<int>& selectedIndexs);
	void leftButtonDoubleClicked(int index);
	void selectItemChange(QObject* obj, int child);
	void leftButtonPressed(int index);

protected slots:
	void on_selectPosChanged(int pos);
	void onItemSelected();

protected:
	virtual void modelReset();
	virtual void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end );
	virtual void rowsRemoved ( const QModelIndex & parent, int start, int end );
	virtual void rowsInserted ( const QModelIndex & parent, int start, int end );
	virtual void itemChanged( QStandardItem *item );

	virtual void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
	virtual void keyPressEvent(QKeyEvent *event);

protected:
	void appendItem( QStandardItem* item );
	void insertItem( int index, QStandardItem* item );

	virtual DuiAbstractViewItem *createViewItem( QStandardItem *item );
	void reset();
};
