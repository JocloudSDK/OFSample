#include "stable.h"

#include "duifw/duitooltip2.h"
#include "duitab_p.h"
#include "duifw/duininepatchpixmapitem.h"

const int KIconTextSpace = 4;
struct TabItemInfo
{
	DuiFrameItem			*frameItem;
	QGraphicsSimpleTextItem *textItem;
	DuiNinePatchPixmapItem	*iconItem;
	QString					iconNormal;
	QString					iconSelected;
	QString					toolTip;
	TabItemInfo() : frameItem(NULL), textItem(NULL), iconItem(NULL)
	{
	}

	void clearItems()
	{
		DW_ASSERT(frameItem);
		DW_ASSERT(textItem);
		DW_ASSERT(iconItem);

		DW_SAFE_DELETE(frameItem);

		//the iconItem is a child of frameItem, will be delete by frameItem.
		iconItem = NULL;
		textItem = NULL;
	}

	~TabItemInfo()
	{
	}
};

DuiTabPrivate::DuiTabPrivate()
: m_selectIndex(-1)
, m_hoverIndex(-1)
, m_isTooltipEnabled(false)
, m_drawStyle( DuiFrameItem::PixmapDrawStyle )
, m_colorWidgetFlag( DuiWidget::ColorableFlag )
, m_colorIconFlag (DuiTab::ColorizeAll)
,m_imagePath(":/theme/duifw/tab.png")
{
}

DuiTabPrivate::~DuiTabPrivate()
{
	qDeleteAll(m_tabItems);
	m_tabItems.clear();
}

int DuiTabPrivate::insertItem( int index, const QString &text, const QString &icoPath, const QString & toolTip, const QString &tabImage)
{
	Q_Q(DuiTab);
	if(index == -1)
	{
		index = tabCount();
	}

	TabItemInfo *ti = new TabItemInfo;
	ti->frameItem = new DuiFrameItem(m_imagePath, DuiFrameItemDrawer::NinePieces, q);
	ti->frameItem->setParent(q);
	ti->frameItem->setObjectName(q->objectName() + QString::number(index));	//这里是为了自动化测试而做的修改
	ti->frameItem->setColorizePolicy( m_colorWidgetFlag );
	ti->frameItem->setAccessibleName(text);

	ti->textItem = new QGraphicsSimpleTextItem( text, ti->frameItem );
	ti->toolTip = toolTip;

	setEnableItemToolTip( ti, m_isTooltipEnabled );

	QStringList imageList = icoPath.split("|");
	if (imageList.size() > 0)
	{
		ti->iconNormal = imageList[0];
	}
	if (imageList.size() > 1)
	{
		ti->iconSelected = imageList[1];
	}

	ti->iconItem = new DuiNinePatchPixmapItem(QPixmap(ti->iconNormal), ti->frameItem);

	if (!tabImage.isEmpty())
	{
		ti->frameItem->setImage(tabImage);
	}
	else
	{
		ti->frameItem->setImage( m_imagePath );
	}

	m_tabItems.insert(index, ti);

	if (index <= m_selectIndex)
	{
		m_selectIndex++;
	}

	if (index <= m_hoverIndex)
	{
		m_hoverIndex++;
	}
	 
	relayout(q->size());
	return m_tabItems.count();
}

void DuiTabPrivate::removeTab( int index )
{
	Q_Q(DuiTab);

	if ( index < 0 || index >= tabCount() )
	{
		DW_ASSERT(false);
		return;
	}

	if (index == m_selectIndex)
	{
		setCurrentIndex(0);
	}

	if (index < m_hoverIndex)
	{
		m_hoverIndex--;
	}

	if (index < m_selectIndex)
	{
		m_selectIndex--;
	}

	TabItemInfo *item = m_tabItems[index];
	m_tabItems.removeAt(index);
	item->clearItems();
	DW_SAFE_DELETE(item);

	//全被删除的情况下要处理,否则tab为0的时候在插入会有bug
	if (m_tabItems.isEmpty())
	{
		m_selectIndex = -1;
		m_hoverIndex = -1;
	}

	relayout(q->size());
}

void DuiTabPrivate::setCurrentIndex( int index )
{
	//这里有个问题，如果删除的是第0个，删除之前选择是第0个会有个没高亮的BUG
	if( index == m_selectIndex && index != 0)
	{
		return;
	}

	if( index < 0 || index >= tabCount() )
	{
		DW_ASSERT(false);
		return;
	}

	if (-1 != m_selectIndex)
	{
		//恢复旧的
		m_tabItems[m_selectIndex]->frameItem->setChecked(false);
		//m_tabItems[m_selectIndex]->frameItem->setHovered(false);旧的实现方式,由于frameitem的加入不需要
		m_tabItems[m_selectIndex]->iconItem->setPixmap(QPixmap(m_tabItems[m_selectIndex]->iconNormal));
		m_tabItems[m_selectIndex]->iconItem->setSubIdx(DuiFrameItem::StateNormal);
	}

	m_selectIndex = index;
	m_tabItems[m_selectIndex]->frameItem->setChecked(true);
	m_tabItems[m_selectIndex]->iconItem->setPixmap(QPixmap(m_tabItems[m_selectIndex]->iconSelected));
	m_tabItems[m_selectIndex]->iconItem->setSubIdx(DuiFrameItem::StatePressed);

	if (m_hoverIndex == m_selectIndex)
	{
		m_hoverIndex = -1;
	}
}

int DuiTabPrivate::currentIndex() const
{
	return m_selectIndex;
}

int DuiTabPrivate::tabCount() const
{
	return m_tabItems.count();
}

void DuiTabPrivate::relayout( const QSizeF &size )
{
	int count = tabCount();
	if (count == 0)
	{
		return;
	}

	int tabWidth = size.width() / count;
	int i = 0;
	for(; i < count - 1; i++)
	{
		moveItem(m_tabItems[i], QRectF(tabWidth * i, 0, tabWidth, size.height()));
	}
	//the last item would be longer than others to cover all of tab area.
	moveItem(m_tabItems[i], QRectF(tabWidth * i, 0, size.width() - tabWidth * i, size.height()));
}

int DuiTabPrivate::indexFromPoint( const QPointF &point )
{
	int count = tabCount();
	if (0 == count)
	{
		return -1;
	}

	Q_Q(DuiTab);

	qreal tabwidth = q->size().width() / count;
	if (qFuzzyCompare(tabwidth, 0))
	{
		return -1;
	}
	if( tabwidth * (count - 1) < point.x())
	{
		return count - 1;
	}
	return point.x() / tabwidth;
}

void DuiTabPrivate::moveItem( TabItemInfo *item, const QRectF &rect )
{
	int textWidth = item->textItem->boundingRect().width();
	int textHeight = item->textItem->boundingRect().height();
	int iconWidth = item->iconItem->boundingRect().width();
	int iconHeight = item->iconItem->boundingRect().height();

	item->frameItem->setGeometry(rect);

	if(iconWidth + textWidth + KIconTextSpace * 2 < rect.width())
	{
		item->iconItem->setPos( (rect.width() - iconWidth - textWidth) / 2, (rect.height() - iconHeight) / 2);

		int space = iconWidth != 0 ? KIconTextSpace : 0;
		int x = (rect.width() - iconWidth - textWidth) / 2 + space + iconWidth;
		int y = (rect.height() - textHeight) / 2;

		item->textItem->setPos( x, rect.top() + y );
		item->textItem->setVisible( true );
	}
	else
	{
		item->iconItem->setPos( (rect.width() - iconWidth) / 2, (rect.height() - iconHeight) / 2);
		item->textItem->setVisible( false );
	}
}

void DuiTabPrivate::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_Q(DuiTab);
	QPointF p = event->scenePos();
	p = q->mapFromScene(p);
	int index = indexFromPoint(p);

	if(index == m_hoverIndex)
	{
		return;
	}
	//注释部分为：旧的实现方式,由于frameitem的加入不需要
	if(m_hoverIndex != -1 /*&& m_hoverIndex != m_selectIndex*/ && m_tabItems.count() > m_hoverIndex)
	{
		m_tabItems[m_hoverIndex]->frameItem->setHovered(false);
	}

	m_hoverIndex = index;

	if(m_hoverIndex != -1 /*&& m_hoverIndex != m_selectIndex*/ && m_tabItems.count() > m_hoverIndex)
	{
		m_tabItems[m_hoverIndex]->frameItem->setHovered(true);
	}
}

void DuiTabPrivate::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);
	//注释部分为：旧的实现方式,由于frameitem的加入不需要
	if (-1 != m_hoverIndex && m_tabItems.count() > m_hoverIndex)
	{
		m_tabItems[m_hoverIndex]->frameItem->setHovered(false);
	}

	m_hoverIndex = -1;
}

void DuiTabPrivate::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	itemClickedByIndex( indexFromPoint(event->pos()) );	
}

void DuiTabPrivate::itemClicked(bool next)
{
	int index = m_selectIndex;
	if (next)
	{
		++index;
		if (index >= m_tabItems.count())
		{
			index = m_tabItems.count() - 1;
		}
	}
	else
	{
		--index;
		if (index < 0)
		{
			index = 0;
		}
	}
	itemClickedByIndex(index);
}

QString DuiTabPrivate::tabImage() const
{
	return m_imagePath;
}

void DuiTabPrivate::setTabImage( const QString &tabImage )
{
	m_imagePath = tabImage;
	foreach(TabItemInfo *item, m_tabItems)
	{
		item->frameItem->setImage(m_imagePath);
	}
}

QString DuiTabPrivate::itemText( int index ) const
{
	if ( index < 0 || index >= tabCount() )
	{
		DW_ASSERT(false);
		return QString();
	}

	return m_tabItems[index]->textItem->text();
}

void DuiTabPrivate::setItemText( int index, const QString &text )
{
	if (index < 0 || index >= tabCount() )
	{
		DW_ASSERT(false);
		return;
	}

	return m_tabItems[index]->textItem->setText(text);
}

void DuiTabPrivate::setEnableToolTip( bool enable )
{
	foreach( TabItemInfo* item, m_tabItems )
	{
		if ( item )
		{
			setEnableItemToolTip( item, enable );
		}
	}
	m_isTooltipEnabled = enable;
}

void DuiTabPrivate::setEnableItemToolTip( TabItemInfo* item, bool enable )
{
	if ( enable )
	{
		item->frameItem->setToolTip( item->toolTip );
		duiToolTip()->attatchWidget( item->frameItem );
	}
	else
	{
		duiToolTip()->detatchWidget( item->frameItem );
	}
}

void DuiTabPrivate::setTextColor( QColor color )
{
	foreach( TabItemInfo* item, m_tabItems )
	{
		if ( item )
		{
			QBrush brush = item->textItem->brush();
			brush.setColor(color);
			item->textItem->setBrush(brush);
		}
	}
}

void DuiTabPrivate::setColorizeWidgetFlag( const DuiWidget::ColorizeWidgetFlag& colorFlag )
{
	m_colorWidgetFlag = colorFlag;
}

void DuiTabPrivate::doColorize( QRgb color )
{
	Q_Q( DuiTab );

	setTextColor(DuiTheme::getColor(DuiTheme::MoreDarkColor));

    if(m_colorIconFlag == DuiTab::ColorizeAll)
    {
        foreach( TabItemInfo* item, m_tabItems )
        {
            if ( item  && item->iconItem)
            {
                QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect(q);
                effect->setColor(DuiTheme::getColor(DuiTheme::MoreDarkColor));
                item->iconItem->setGraphicsEffect(effect);
            }
        }
    }else
    {
        for(int i = 0; i< m_tabItems.size() ; i++)
        {
            if(i == currentIndex())
            {
                QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect(q);
                effect->setColor(DuiTheme::getColor(DuiTheme::MoreDarkColor));
                m_tabItems[i]->iconItem->setGraphicsEffect(effect);
            }else
            {
                m_tabItems[i]->iconItem->setGraphicsEffect(NULL);
            }
        }
    }
}

void DuiTabPrivate::itemClickedByIndex(int index)
{
	Q_Q(DuiTab);
	if (0 > index || m_tabItems.count() <= index)
	{
		return;
	}
	DuiWidget::duiUpdateAccessibility(m_tabItems.at(index)->frameItem, DuiWidget::AccStateChanged);
	if (index == m_selectIndex)
	{
		emit q->clickedOnSelectedItem(m_selectIndex);
		return;
	}

	setCurrentIndex(index);
	
	emit q->selectChanged(m_selectIndex);
}
