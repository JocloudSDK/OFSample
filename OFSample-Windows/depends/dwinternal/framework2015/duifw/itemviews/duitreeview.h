
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

class DuiTreeViewPrivate;
class QTreeView;
class QHeaderView;
class QAbstractItemModel;
class QModelIndex;

class DW_DUIFW_EXPORT DuiTreeView : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiTreeView )
	Q_DECLARE_PRIVATE( DuiTreeView )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiTreeView )

	Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet )

public:
	explicit DuiTreeView( QGraphicsItem *parent = NULL );
	virtual ~DuiTreeView();

	void setModel( QAbstractItemModel *model );
	QAbstractItemModel* model( ) const;

	void setStyleSheet(const QString& styleSheet);
	QString styleSheet() const;

	QHeaderView* header() const;
	QTreeView*	 realTreeView() const;
	QModelIndex currentIndex() const ;
	void setHeaderHidden(bool hide);
	
	QAbstractItemView::SelectionMode	selectionMode () const;
	void	setSelectionMode ( QAbstractItemView::SelectionMode mode );

	QItemSelectionModel * selectionModel () const;

signals:
	void contextMenu( const QModelIndex & index );
	void clicked ( const QModelIndex & index );
	void clicked( );
	void doubleClicked ( const QModelIndex & index );
	void pressed ( const QModelIndex & index );
	void activated ( const QModelIndex & index );
	void keyPress(QKeyEvent *event);

protected:
	explicit DuiTreeView( DuiTreeViewPrivate &dd, QGraphicsItem *parent = NULL );

	virtual void resizeEvent( QGraphicsSceneResizeEvent * event ); 
	virtual void moveEvent( QGraphicsSceneMoveEvent * event );
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

	virtual void doColorize( QRgb color );
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void focusInEvent(QFocusEvent *event);

private slots:
	void on_currentIndexChanged(const QModelIndex& current);
};
