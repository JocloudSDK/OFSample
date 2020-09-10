
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duifw/duitreewidget.h"

class DuiSettingPageButtonTreePrivate;

class DW_DUIFW_EXPORT DuiSettingPageButtonTree : public DuiTreeWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiSettingPageButtonTree )
	Q_DECLARE_PRIVATE( DuiSettingPageButtonTree )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSettingPageButtonTree )

	Q_PROPERTY( QList<QVariant> data READ data WRITE setData )

public:
	explicit DuiSettingPageButtonTree(QGraphicsItem *parent = NULL);

	virtual ~DuiSettingPageButtonTree();

	void setData( const QList<QVariant> &dataList );

	void addCategory( const QString& categoryText, const QString& icon );
	void addCategoryItem( const QString& categoryText, const QString& itemText, const QString& itemIcon, const QString& pageName );

	void removeCategory( const QString& categoryText );
	void removeCategoryItem( const QString& categoryText, const QString& itemText );

	// expandItem is slow when data is large.
	void expandItem( const QString &pageName );
    void expandAllItem();
	QString pageName( const QModelIndex& index ) const;
    QString categoryText( const QString &pageName ) const;

protected slots:
	void on_folderLeftButtonClicked( const QModelIndex &index );
	void on_scroll();

private:
	QStandardItem* findItem( const QString& categoryText );
	QStandardItem* findItem( QStandardItem* item, const QString& categoryText );
	
	void			_selectItem( QStandardItem *item );
	void			_expandItem( QStandardItem *item, bool recursive = false );
	QStandardItem*	_findItemByName( QStandardItem *item , const QString &pageName ) const;

	QList<QVariant> data() const;
	QStandardItem* processNode( const QVariant& elem );
	void init();

//private:
//	QModelIndex			m_folderClickedIndex;

};

class DW_DUIFW_EXPORT DuiSettingPageButtonTreeMouse: public DuiSettingPageButtonTree
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiSettingPageButtonTreeMouse )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSettingPageButtonTreeMouse )

public:
	explicit DuiSettingPageButtonTreeMouse(QGraphicsItem *parent = NULL);
};