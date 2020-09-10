#include "stable.h"

#include "duifw/duitheme.h"
#include "duifw/duitreewidget.h"
#include "duifw/duiframeitem.h"
#include "duifw/duiseparateline.h"
#include "itemviews/duitreewidget_p.h"

#include "duisettingpagebuttontree.h"

extern QFont qStringToFont(const QString& val);

namespace
{
    enum
    {
        ExpandDefault = 0,
        ExpandNoCollapse,
    };

    enum 
    {
        KPageName = Qt::UserRole + 1,
        KFolderExpandMode,
        KTextFont,
        KItemHeightRole,
    };

    static const qreal KItemHeight = 36.0;
    static const qreal KItemHeightMouse = 40.0;
}

class DuiSettingPageButtonTreePrivate : public DuiTreeWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiSettingPageButtonTree)
	Q_DISABLE_COPY(DuiSettingPageButtonTreePrivate)

public:
	DuiSettingPageButtonTreePrivate();
	~DuiSettingPageButtonTreePrivate();

public:
	QModelIndex m_folderClickedIndex;
};

DuiSettingPageButtonTreePrivate::DuiSettingPageButtonTreePrivate()
{

}
DuiSettingPageButtonTreePrivate::~DuiSettingPageButtonTreePrivate()
{

}

class DuiButtonTreeItem : public DuiTreeViewItem
{
public:
	explicit DuiButtonTreeItem(QStandardItem *item, QGraphicsItem *parent, bool isChild);
	virtual DuiTreeViewItem *createItem( QStandardItem *modelItem, QGraphicsItem *parent );

	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint ) const;

	virtual bool isFolder() const;
	
protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void expandedChanged();
	void selectedChanged();
	void doColorize( QRgb color );

	bool isTailItem() const;
	bool isParentIsTailItem() const;

private:
	bool					m_isChild;
	QGraphicsPixmapItem*	m_treeIcon;
	QGraphicsTextItem*		m_textItem;
	DuiFrameItem*			m_frameItem;
	DuiFrameItem*			m_selectFrameItem;
	DuiSeparateLine*		m_separateLine;
	DuiSeparateLine*		m_separateLineWhite;//白色虚线
};

DuiButtonTreeItem::DuiButtonTreeItem( QStandardItem *item, QGraphicsItem *parent, bool isChild )
: DuiTreeViewItem( item, parent) 
, m_isChild( isChild )
, m_treeIcon( NULL )
, m_textItem( NULL )
, m_frameItem( NULL )
, m_selectFrameItem( NULL )
, m_separateLine( NULL )
, m_separateLineWhite( NULL )
{
	setColorizePolicy( ColorableFlag );

	if ( !m_isChild )
	{
		m_frameItem = new DuiFrameItem( ":/theme/mainframe/button_setting_normal.png", DuiFrameItemDrawer::NinePieces, this );
		m_frameItem->setThemeColorType( DuiTheme::MidDarkColor );
	}
	else
	{
		m_selectFrameItem = new DuiFrameItem( ":/theme/mainframe/button_setting_down.png", DuiFrameItemDrawer::NinePieces, this );
		m_selectFrameItem->setThemeColorType( DuiTheme::DarkColor );
		m_selectFrameItem->hide();
	}
	m_separateLine	=	new DuiSeparateLine( this );
	m_separateLineWhite	=	new DuiSeparateLine( this );
	m_treeIcon = new QGraphicsPixmapItem( this );
	m_textItem = new QGraphicsTextItem( this );

	m_separateLine->setThemeColorType( DuiTheme::LightColor );

	if ( m_isChild )
	{
		if ( !modelItem()->icon().isNull() )
		{
			m_treeIcon->setPixmap( modelItem()->icon().pixmap(20, 17) );
		}

		m_treeIcon->setPos( 32.0, ( KItemHeight / 2 ) - (  m_treeIcon->pixmap().size().height() / 2 ) );

		m_textItem->setPlainText( modelItem()->text() );
		m_textItem->setPos( 51.0, 6.0 );
		
		m_separateLine->setColor( DuiTheme::getColor( DuiTheme::LightColor) );
		m_separateLineWhite->setColor(QColor(255,255,255));

		if ( isTailItem() )
		{
			if ( isParentIsTailItem() )
			{
				m_separateLine->setPenStyle( Qt::CustomDashLine );
				m_separateLineWhite->setPenStyle( Qt::CustomDashLine );
			}
		}
		else
		{
			m_separateLine->setPenStyle( Qt::CustomDashLine );
			m_separateLineWhite->setPenStyle( Qt::CustomDashLine );
		}
	}
	else
	{
		m_treeIcon->setPixmap( DuiTheme::getImage(":/theme/duifw/tree_collapse_icon.png") );
		m_treeIcon->setPos( 12.0, 14.0 );
		QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect( this );
		colorizeEffect->setColor( DuiTheme::getColor( DuiTheme::DarkColor ) );
		m_treeIcon->setGraphicsEffect( colorizeEffect );

		m_textItem->setDefaultTextColor( DuiTheme::getColor( DuiTheme::MoreDarkColor ) );
		if ( modelItem() )
		{
			m_textItem->setPlainText( modelItem()->text() );
		}

		m_textItem->setPos( 30.0, 6.0 );
	}	
}

void DuiButtonTreeItem::doColorize( QRgb color )
{
	Q_UNUSED( color );

	QGraphicsColorizeEffect* colorizeEffect  = qobject_cast<QGraphicsColorizeEffect*> (m_treeIcon->graphicsEffect( ));
	if ( colorizeEffect )
	{
		colorizeEffect->setColor( DuiTheme::getColor( DuiTheme::DarkColor ) );
	}

	if ( !m_isChild )
	{
		m_textItem->setDefaultTextColor( DuiTheme::getColor( DuiTheme::MoreDarkColor ) );
	}
}

DuiTreeViewItem* DuiButtonTreeItem::createItem( QStandardItem *modelItem, QGraphicsItem *parent )
{
	return new DuiButtonTreeItem( modelItem, parent, !modelItem->hasChildren() );
}

QSizeF DuiButtonTreeItem::sizeHint( Qt::SizeHint which, const QSizeF &constraint ) const
{
	if(which == Qt::PreferredSize)
	{
		return QSizeF(parentItem()->boundingRect().width(), KItemHeight );
	}

	return DuiTreeViewItem::sizeHint(which, constraint);
}

void DuiButtonTreeItem::expandedChanged()
{
	if ( isExpanded() )
	{
		m_treeIcon->setPixmap( DuiTheme::getImage(":/theme/duifw/tree_expand_icon.png") );
	}
	else
	{
		m_treeIcon->setPixmap( DuiTheme::getImage(":/theme/duifw/tree_collapse_icon.png") );
	}	
}

void DuiButtonTreeItem::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	if ( m_frameItem )
	{
		m_frameItem->setGeometry( QRectF(QPointF(0, 0), event->newSize()) );
	}

	if ( m_selectFrameItem )
	{
		m_selectFrameItem->setGeometry( QRectF(QPointF(0, 0), event->newSize()) );
	}	

	if ( m_separateLine )
	{
		m_separateLine->setGeometry( 0, event->newSize().height() - 1, event->newSize().width(), 1 );
	}

	if(m_separateLineWhite)
	{
		if (m_isChild)
		{
			m_separateLineWhite->setGeometry( 0, event->newSize().height() , event->newSize().width(), 1 );
		}
		else
		{
			m_separateLineWhite->hide();
		}
	}
}

void DuiButtonTreeItem::selectedChanged()
{
	if ( m_isChild && m_selectFrameItem )
	{
		if ( isSelected() )
		{
			m_selectFrameItem->show();

			m_textItem->setDefaultTextColor( QColor(255,255,255) );

			//这里存在反复创建与删除内存，存在隐患
			QGraphicsDropShadowEffect* textShadowEffect = new QGraphicsDropShadowEffect( m_textItem );
			textShadowEffect->setColor( QColor(0,0,0, 104) );
			textShadowEffect->setOffset(1, 1);
			textShadowEffect->setBlurRadius( 2 );
			m_textItem->setGraphicsEffect( textShadowEffect );
		}
		else
		{
			m_selectFrameItem->hide();
			m_textItem->setDefaultTextColor( Qt::black );
			m_textItem->setGraphicsEffect( NULL );
		}	
	}
}

bool DuiButtonTreeItem::isTailItem() const
{
	QStandardItem* parentItem = modelItem()->parent();
	if (  parentItem == NULL || parentItem->rowCount() == 0 )
		return false;

	QStandardItem* tailChildItem = parentItem->child( parentItem->rowCount() - 1 );
	if ( tailChildItem && tailChildItem == modelItem() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DuiButtonTreeItem::isParentIsTailItem() const
{
	QStandardItem* parentItem = modelItem()->parent();
	if ( parentItem == NULL )
		return false;

	QStandardItem* gParentItem = parentItem->parent();
	if ( gParentItem == NULL || gParentItem->rowCount() == 0 )
		return false;

	QStandardItem* tailChildItem = gParentItem->child( gParentItem->rowCount() - 1 );
	if ( tailChildItem && tailChildItem == parentItem )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DuiButtonTreeItem::isFolder() const
{
	return !m_isChild;
}

class DuiButtonTreeItemMouse : public DuiTreeViewItem
{
public:
	explicit DuiButtonTreeItemMouse(QStandardItem *item, QGraphicsItem *parent, bool isChild);
	virtual DuiButtonTreeItemMouse *createItem( QStandardItem *modelItem, QGraphicsItem *parent );

	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint ) const;

	virtual bool isFolder() const;
    virtual bool isChildrenCollapsible() const;

protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void expandedChanged();
	void selectedChanged();
	void doColorize( QRgb color );

	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
	QGraphicsPixmapItem*	m_treeIcon;
	QGraphicsTextItem*		m_textItem;
	DuiFrameItem*			m_selectFrameItem;
	DuiFrameItem*			m_frameBackground;
};

DuiButtonTreeItemMouse::DuiButtonTreeItemMouse( QStandardItem *item, QGraphicsItem *parent, bool isChild )
: DuiTreeViewItem( item, parent) 
, m_treeIcon( NULL )
, m_textItem( NULL )
, m_selectFrameItem( NULL )
{
	if (NULL == item)
	{
		return;
	}

	setColorizePolicy( ColorableFlag );

	m_frameBackground = new DuiFrameItem( ":/theme/duifw/item_hovered_mask.png", DuiFrameItemDrawer::NinePieces, this );
	m_frameBackground->setBaseColor(DuiTheme::getColor(DuiTheme::BaseColor));
	m_frameBackground->hide();

    m_selectFrameItem = new DuiFrameItem( ":/theme/duifw/settingitem_selected_mask.png", DuiFrameItemDrawer::NinePieces, this );
    //m_selectFrameItem->setBaseColor(DuiTheme::getColor(DuiTheme::DarkColor));
    m_selectFrameItem->setColorizePolicy(DuiWidget::UnColorableFlag);
	m_selectFrameItem->hide();

	m_treeIcon = new QGraphicsPixmapItem( this );
	m_textItem = new QGraphicsTextItem( this );

    QString fontString = item ? item->data(KTextFont).toString() : "";
    if (fontString.isEmpty() == false)
    {
        m_textItem->setFont(qStringToFont(fontString));
    }

    int item_height = KItemHeightMouse;
    if (item && item->data(KItemHeightRole).isValid())
        item_height = qvariant_cast<qreal>(item->data(KItemHeightRole));
	m_textItem->setPlainText( item->text() );
	int nTextHeight = m_textItem->document()->size().height();
	int nTextTop = ( item_height -  nTextHeight)>>1;
    if (item && item->hasChildren())
    {
        int expandType = qvariant_cast<int>(item->data(KFolderExpandMode));
        if (expandType == ExpandDefault)
        {
            m_treeIcon->setPixmap( DuiTheme::getImage(":/theme/duifw/tree_collapse_icon.png") );
            m_treeIcon->setPos( 12.0, ( item_height -  m_treeIcon->pixmap().size().height())>>1 );
            m_textItem->setPos( 30.0, nTextTop);
        }
        else
        {
            if (!item->icon().isNull())
            {
                QList<QSize> asizes = modelItem()->icon().availableSizes();
                if (!asizes.isEmpty())
                {
                    m_treeIcon->setPixmap( modelItem()->icon().pixmap(asizes.last()) );
                }
                m_treeIcon->setPos( 12.0, ( item_height -  m_treeIcon->pixmap().size().height())>>1 );
                m_textItem->setPos( 12.0 + m_treeIcon->boundingRect().width() + 1.0, nTextTop );
            }
        }
    }
    else if (item)
	{
        m_treeIcon->setVisible(false); // no child not show the expand icon
		if ( !item->icon().isNull() )
		{
            QList<QSize> asizes = modelItem()->icon().availableSizes();
            if (!asizes.isEmpty())
            {
                m_treeIcon->setPixmap( modelItem()->icon().pixmap(asizes.last()) );
            }
		}
		m_treeIcon->setPos( 32.0, ( item_height / 2 ) - (  m_treeIcon->pixmap().size().height() / 2 ) );
		m_textItem->setPos( 32.0 + m_treeIcon->boundingRect().width() + 1.0, nTextTop );
	}

    QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect( this );
    colorizeEffect->setColor( DuiTheme::getColor( DuiTheme::MoreDarkColor ) );
    m_treeIcon->setGraphicsEffect( colorizeEffect );

    m_textItem->setDefaultTextColor(Qt::black);
}

void DuiButtonTreeItemMouse::doColorize( QRgb color )
{
	Q_UNUSED( color )

	QGraphicsColorizeEffect* colorizeEffect  = new QGraphicsColorizeEffect;
	if ( colorizeEffect )
	{
		colorizeEffect->setColor( DuiTheme::getColor( DuiTheme::MoreDarkColor ) );
        m_treeIcon->setGraphicsEffect(colorizeEffect);
	}
}

DuiButtonTreeItemMouse* DuiButtonTreeItemMouse::createItem( QStandardItem *modelItem, QGraphicsItem *parent )
{
	return new DuiButtonTreeItemMouse( modelItem, parent, !modelItem->hasChildren() );
}

QSizeF DuiButtonTreeItemMouse::sizeHint( Qt::SizeHint which, const QSizeF &constraint ) const
{
	if(which == Qt::PreferredSize)
	{
        qreal item_height = KItemHeightMouse;
        if (modelItem() && modelItem()->data(KItemHeightRole).isValid())
            item_height = qvariant_cast<qreal>(modelItem()->data(KItemHeightRole));

		return QSizeF(parentItem()->boundingRect().width(), item_height);
	}

	return DuiTreeViewItem::sizeHint(which, constraint);
}

void DuiButtonTreeItemMouse::expandedChanged()
{
    if (QStandardItem* st_item = modelItem())
    {
        if (qvariant_cast<int>(st_item->data(KFolderExpandMode)) != ExpandDefault)
            return;
    }

	if ( isExpanded() )
	{
		m_treeIcon->setPixmap( DuiTheme::getImage(":/theme/duifw/tree_expand_icon.png") );
	}
	else
	{
		m_treeIcon->setPixmap( DuiTheme::getImage(":/theme/duifw/tree_collapse_icon.png") );
	}
}

void DuiButtonTreeItemMouse::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	if ( m_selectFrameItem )
	{
		m_selectFrameItem->setGeometry( QRectF(QPointF(0, 0), event->newSize()) );
	}	

	m_frameBackground->setGeometry( QRectF(QPointF(0, 0), event->newSize()));
}

void DuiButtonTreeItemMouse::selectedChanged()
{
    if (QStandardItem* st_item = modelItem())
    {
        if (qvariant_cast<int>(st_item->data(KFolderExpandMode)) != ExpandDefault && st_item->hasChildren())
            return;
    }

	if ( isSelected() )
	{
		m_selectFrameItem->show();
        m_textItem->setDefaultTextColor(QColor(38,143,214));
	}
	else
	{
        m_textItem->setDefaultTextColor(Qt::black);
		m_selectFrameItem->hide();
	}
}

bool DuiButtonTreeItemMouse::isFolder() const
{
    if (QStandardItem* st_item = modelItem())
    {
        return st_item->hasChildren();
    }

    return false;
}


void DuiButtonTreeItemMouse::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);
	m_frameBackground->hide();
    if (!isSelected())
    {
        m_textItem->setDefaultTextColor(Qt::black);
    }
}

void DuiButtonTreeItemMouse::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);

    QStandardItem* st_item = modelItem();
    if (st_item && qvariant_cast<int>(st_item->data(KFolderExpandMode)) != ExpandDefault && st_item->hasChildren())
    {
    }
	else
    {
        //m_frameBackground->show();
	    m_frameBackground->hide();
    	m_textItem->setDefaultTextColor(QColor(38,143,214));
    }
}

bool DuiButtonTreeItemMouse::isChildrenCollapsible() const
{
    if (QStandardItem* st_item = modelItem())
    {
        return qvariant_cast<int>(st_item->data(KFolderExpandMode)) == ExpandDefault && st_item->hasChildren();
    }
    return DuiTreeViewItem::isChildrenCollapsible();
}
//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiSettingPageButtonTree )

DuiSettingPageButtonTree::DuiSettingPageButtonTree( QGraphicsItem* parent )
: DuiTreeWidget( *new DuiSettingPageButtonTreePrivate(), parent )
{
	init();
}

DuiSettingPageButtonTree::~DuiSettingPageButtonTree()
{
}

QStandardItem* DuiSettingPageButtonTree::processNode( const QVariant& elem )
{
	const QMap<QString, QVariant> &m = elem.toMap();

	QString text = m.value( "text" ).toString();
	QString icon = m.value( "icon" ).toString();
	QString pageName = m.value( "pagename" ).toString();
    int expandMode = m.value( "expandMode" ).toInt();
    QString fontString = m.value("font").toString();
    QString heightString = m.value("height").toString();

	QStandardItem* item = NULL;
	item = new QStandardItem( text );

	if ( !icon.isEmpty() )
	{
		item->setIcon(QIcon( icon ));
	}

	if ( !pageName.isEmpty() )
	{
		item->setData( pageName );
	}

    if ( !heightString.isEmpty() )
    {
        item->setData(heightString.toFloat(), KItemHeightRole);
    }

    item->setData( expandMode, KFolderExpandMode );
    item->setData( fontString, KTextFont );

	if (m.contains("children"))	//还有下一级节点
	{
		const QList<QVariant> &sub = m.value("children").toList();
		for (int i = 0; i < sub.count(); ++i)
		{
			item->appendRow(processNode(sub.at(i)));
		}
	}

	return item;
}


void DuiSettingPageButtonTree::setData( const QList<QVariant> &dataList )
{
	if ( dataList.count() == 0 )
	{
		return;
	}

	QStandardItemModel *model = new QStandardItemModel();
	QStandardItem *rootItem = new QStandardItem( "root" );
	model->appendRow(rootItem);
	for ( int i = 0; i < dataList.count(); i++ )
	{
		rootItem->appendRow( processNode(dataList.at(i)) );
	}

	setModel( model );
}

QList<QVariant> DuiSettingPageButtonTree::data() const
{
	DW_ASSERT_X(KDuiIsEditorMode, __FUNCTION__, "this method should not be invoke.");
	return QList<QVariant>();
}

void DuiSettingPageButtonTree::init()
{
	addItemPrototype( new DuiButtonTreeItem( NULL, NULL, NULL ) );

	setSelectionMode( DuiTreeWidget::SingleSelection );
	
	connect( this, SIGNAL(folderLeftButtonClicked(const QModelIndex&)), SLOT(on_folderLeftButtonClicked( const QModelIndex &)) );
}

void DuiSettingPageButtonTree::addCategory( const QString& categoryText, const QString& icon )
{
	if ( model() == NULL || model()->rowCount() == 0 )
	{
		return;
	}

	QStandardItem* item = new QStandardItem( categoryText );
	item->setIcon( QIcon( icon ) );

	QStandardItem* rootItem = model()->item( 0 );
	rootItem->appendRow( item );
}

void DuiSettingPageButtonTree::addCategoryItem( const QString& categoryText, const QString& itemText, const QString& itemIcon, const QString& pageName )
{
	if ( model() == NULL )
	{
		return;
	}

	QStandardItem* categoryItemParent  = findItem( categoryText );
	if ( categoryItemParent == NULL )
		return ;

	QStandardItem* item = new QStandardItem( itemText );
	item->setIcon( QIcon( itemIcon ) );
	item->setData( pageName, KPageName );

	categoryItemParent->appendRow( item );	
}

void DuiSettingPageButtonTree::removeCategory( const QString& categoryText )
{
	if ( model() == NULL )
	{
		return;
	}

	QStandardItem* categoryItem  = findItem( categoryText );
	if ( NULL == categoryItem)
		return ;
	QStandardItem* categoryParentItem = categoryItem->parent();
	if ( NULL == categoryParentItem)
	{
		return;
	}
	categoryParentItem->removeRow(categoryItem->row());
	//被删除的同级目录为空，则删除parent , 些逻辑只支持二级目录（不包含root item）以下结构.
	QStandardItem* rootItem = model()->item(0);
	if (NULL == rootItem ||
		categoryParentItem == rootItem ||
		categoryParentItem->hasChildren() ||
		NULL == categoryParentItem->parent())
	{
		return;
	}
	categoryParentItem->parent()->removeRow(categoryParentItem->row());
}

void DuiSettingPageButtonTree::removeCategoryItem( const QString& categoryText, const QString& itemText )
{
	if ( model() == NULL )
	{
		return;
	}

	QStandardItem* categoryItemParent  = findItem( categoryText );
	if ( categoryItemParent == NULL )
		return ;

	for ( int i = 0; i < categoryItemParent->rowCount(); i++ )
	{
		QStandardItem* item = categoryItemParent->child( i );
		if ( item == NULL )
		{
			continue;
		}

		qDebug() << item->text();
		
		if ( item->text() == itemText )
		{
			categoryItemParent->removeRow( i );
			break;
		}
	}
}

QStandardItem* DuiSettingPageButtonTree::findItem( QStandardItem* item, const QString& categoryText )
{
	QStandardItem* foundItem = NULL; 

	for ( int i = 0; i < item->rowCount(); i++ )
	{
		QStandardItem* childItem = item->child( i );
		QString itemText = childItem->text();
		if ( itemText == categoryText )
		{
			foundItem = childItem;
		}
		else
		{
			foundItem = findItem( childItem, categoryText );
		}

		if ( foundItem )
		{
			break ;
		}
	}

	return foundItem;
}

QStandardItem* DuiSettingPageButtonTree::findItem( const QString& categoryText )
{
	if ( model() == NULL || model()->rowCount( ) == 0 )
	{
		return NULL;
	}


	QStandardItem* rootItem = model()->item( 0 );

	if ( categoryText == "root")
	{
		return rootItem;
	}

	return findItem( rootItem, categoryText );
}

QString DuiSettingPageButtonTree::pageName( const QModelIndex& index ) const
{
	QStandardItem *item = model()->itemFromIndex(index);

	if ( item )
	{
		return item->data(KPageName).toString();
	}

	return QString();
}

QString DuiSettingPageButtonTree::categoryText( const QString &pageName ) const
{
    QStandardItem *rootItem = model()->item(0);
    QStandardItem *item = _findItemByName( rootItem, pageName );
    return item ? item->text() : QString();
}

void DuiSettingPageButtonTree::expandItem( const QString &pageName )
{
	if (NULL == model())
	{
		return;
	}
	QStandardItem *rootItem = model()->item(0);

	QStandardItem *item = _findItemByName( rootItem, pageName );
	if ( item )
	{
		_expandItem( item->parent() );
		_selectItem(item);
	}
}

void DuiSettingPageButtonTree::expandAllItem()
{
    if (QStandardItem* root_item = model()->item(0))
        _expandItem(root_item, true);
}

QStandardItem* DuiSettingPageButtonTree::_findItemByName( QStandardItem *item , const QString &pageName ) const
{
	QStandardItem *resitem = NULL;
	for ( int i = 0 ; i < item->rowCount(); ++i )
	{
		resitem = _findItemByName( item->child(i), pageName);
		if ( resitem != NULL )
		{
			return resitem;
		}
		if ( item->child(i)->data(KPageName).toString() == pageName )
		{
			return item->child(i);
		}
	}

	return NULL;
}

void DuiSettingPageButtonTree::_expandItem( QStandardItem *item, bool recursive/* = false*/ )
{
	Q_D(DuiSettingPageButtonTree);

    if (recursive)
    {
        const int row_count = item->rowCount();
        for (int i = 0; i < row_count; ++i)
        {
            QStandardItem* st_item = item->child(i);
            _expandItem(st_item, recursive);
        }
    }

	int count  = viewItemCount();
	for(int i = 0; i < count; i++)
	{
		DuiTreeViewItem *viewItem = treeItemAt(i);
		if(viewItem->modelItem() == item)
		{
			DuiTreeWidget::expandItem(i);
			d->m_folderClickedIndex = item->index();
            if (!recursive)
            {
			    on_scroll();
			    break;
            }
		}
	}
}

void DuiSettingPageButtonTree::_selectItem( QStandardItem *item )
{
	for ( int i= 0 ; i < viewItemCount(); ++i )
	{
		if ( treeItemAt(i)->modelItem() == item )
		{
			selectItem(i);
			break;
		}
	}
}

void DuiSettingPageButtonTree::on_folderLeftButtonClicked( const QModelIndex &index )
{
	Q_D(DuiSettingPageButtonTree);
	d->m_folderClickedIndex = index;	
	QTimer::singleShot(0, this, SLOT(on_scroll()));
}

void DuiSettingPageButtonTree::on_scroll()
{
	Q_D(DuiSettingPageButtonTree);
	if( !d->m_folderClickedIndex.isValid() )
	{
		return;
	}

	int folderRow = -1;
	for ( int i = 0; i < viewItemCount(); i++ )
	{
		DuiTreeViewItem* item =  treeItemAt( i );
		if ( item->isFolder() )
		{
			folderRow ++;
		}

		if ( folderRow == d->m_folderClickedIndex.row() )
		{
			setScrollPos( item->geometry().y() );
			return;
		}
	}
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiSettingPageButtonTreeMouse )
DuiSettingPageButtonTreeMouse::DuiSettingPageButtonTreeMouse(QGraphicsItem *parent)
: DuiSettingPageButtonTree(parent)
{
    addItemPrototype(new DuiButtonTreeItemMouse(NULL, NULL, NULL));
}
