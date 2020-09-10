#include "stable.h"

#include "duispliterwidget_p.h"

DuiSpliterWidgetPrivate::DuiSpliterWidgetPrivate()
: m_orintation(Qt::Horizontal)
, m_index(0)
{

}

DuiSpliterWidgetPrivate::~DuiSpliterWidgetPrivate()
{

}

void DuiSpliterWidgetPrivate::setOrientation( Qt::Orientation orientation )
{
	m_orintation = orientation;
}

Qt::Orientation DuiSpliterWidgetPrivate::orientation() const
{
	return m_orintation;
}

int DuiSpliterWidgetPrivate::insertItem( DuiWidget* item, int index /*= -1 */ )
{
	Q_Q(DuiSpliterWidget);
	if (NULL == item 
		|| q == item 
		|| index > m_itemList.size() / 2 
		|| -1 != m_itemList.indexOf(item))
	{
		DW_ASSERT(false);
		return -1;
	}

	if (!m_itemList.empty())
	{
		DuiSpliter *spliter = DuiSpliter::createObject(q, m_orintation, m_index++);
		if (-1 == index)
		{
			m_itemList.push_back(spliter);
		}
		else if (0 == index)
		{
			m_itemList.push_front(spliter);
		}
		else
		{
			m_itemList.insert(index * 2 - 1, spliter);
		}
	}

	if (-1 == index)
	{
		m_itemList.push_back(item); 
		index = m_itemList.size() / 2;
	}
	else if (0 == index)
	{
		m_itemList.push_front(item);
	}
	else
	{
		m_itemList.insert(index * 2, item);
	}

	item->setGeometry( QRectF( 0
		, 0
		, qAbs(item->preferredWidth())
		, qAbs(item->preferredHeight() ) ) );

	if (-1 != m_currentSpliterIndex) 
	    m_currentSpliterIndex=-1;

	if (q->isVisible())
	{
		relayout(q->size());
	}
	else
	{
		m_needRelayout = true;
	}

	return index;
}

void DuiSpliterWidgetPrivate::removeItem( DuiWidget* item )
{
	int index = m_itemList.indexOf(item);

	if (NULL == item || -1 == index)
	{
		DW_ASSERT(false);
		return;
	}

	DuiWidget* spliter = NULL;

	if (-1 != m_itemList.indexOf(m_itemList.value(index - 1)))
	{
		spliter = m_itemList[index - 1];
	}
	else if (-1 != m_itemList.indexOf(m_itemList.value(index + 1)))
	{
		spliter = m_itemList[index + 1];
	}

	m_itemList.removeOne(spliter);
	m_itemList.removeOne(item);
	DW_SAFE_DELETE(spliter);

	Q_Q(DuiSpliterWidget);
	relayout(q->size());
	
    if (-1 != m_currentSpliterIndex) 
        m_currentSpliterIndex=-1;

}

void DuiSpliterWidgetPrivate::removeItem( int index )
{
	removeItem(m_itemList.value(index * 2));
}

void DuiSpliterWidgetPrivate::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	relayout(event->newSize());
}

void DuiSpliterWidgetPrivate::showEvent( QShowEvent *event )
{
	Q_UNUSED(event);

	if (m_needRelayout)
	{
		relayout(q_func()->size());
		m_needRelayout = false;
	}
}

void DuiSpliterWidgetPrivate::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	for ( int index = 1; index < m_itemList.size(); index += 2 )
	{
		if( m_itemList[index]->geometry().contains( event->pos() ) )
		{
			m_isMouseDown = true;
			m_preMousePos = event->pos();
			m_currentSpliterIndex = index;
			((DuiSpliter*)m_itemList[index])->setIsDragging(true);
			break;
		}
	}
}

void DuiSpliterWidgetPrivate::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED(event);

	m_isMouseDown = false;

	if (-1 != m_currentSpliterIndex)
	{
		((DuiSpliter*)m_itemList[m_currentSpliterIndex])->setIsDragging(false);
	}

	m_currentSpliterIndex = -1;
}

void DuiSpliterWidgetPrivate::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if (m_isMouseDown && -1 != m_currentSpliterIndex)
	{
		QRectF rectSpliter = m_itemList[m_currentSpliterIndex]->geometry();
		qreal delta = 0;

		if (Qt::Horizontal == m_orintation)
		{
			delta = event->pos().x() - m_preMousePos.x();

			if ((delta > 0 && event->pos().x() > rectSpliter.left())
				|| (delta < 0 && event->pos().x() < rectSpliter.right()))
			{
				split(delta, true);
			}
		}
		else
		{
			delta = event->pos().y() - m_preMousePos.y();

			if ((delta > 0 && event->pos().y() > rectSpliter.top())
				|| (delta < 0 && event->pos().y() < rectSpliter.bottom()))
			{
				split(delta, false);
			}
		}
		emit q_func()->splited( m_currentSpliterIndex, event->pos() );

		m_preMousePos = event->pos();
	}
}

void DuiSpliterWidgetPrivate::relayout( const QSizeF &size )
{
	if (size.isEmpty() || m_itemList.empty())
	{
		return;
	}

	/*Q_Q(DuiSpliterWidget);	

	if (!q->isVisible())
	{
		m_needRelayout = true;	should record size here
		return;
	}*/

	if (Qt::Horizontal == m_orintation)
	{
		relayoutHorizontal(size);
	}
	else
	{
		realyoutVertical(size);
	}
}

void DuiSpliterWidgetPrivate::relayoutHorizontal( const QSizeF &size )
{
	Q_Q(DuiSpliterWidget);
	qreal left = 0, right = 0, top  = 0, bottom = 0;
	q->getContentsMargins(&left, &right, &top, &bottom);

	qreal actualHeight = size.height() - (top + bottom);

	qreal minimumWidth = 0, maximumWidth = 0, currentWidth = 0;
	totalWidth( minimumWidth, maximumWidth, currentWidth );

	int remainWidth = size.width() - (left + right + currentWidth);

	ItemSegmentMap anytimeMap;
	ItemSegmentMap userSplitMap;

	getAllSegment(anytimeMap, userSplitMap);

	remainWidth = getAdjustedSegment(anytimeMap, remainWidth);
	if (remainWidth != 0)
	{
		getAdjustedSegment(userSplitMap, remainWidth);
		anytimeMap.unite(userSplitMap);
	}

	horizontalAdjustBySegment(anytimeMap, top, actualHeight, left);

#ifdef _DEBUG
	QGraphicsLayoutItem *item = m_itemList[m_itemList.size() - 1];
	QRectF rectItem = item->geometry();
	if (rectItem.right() < size.width() - right - 1 )
	{
		qDebug("Horizontal relayout problem, last item's right is: %d, right border is: %d. Please check your layout.", rectItem.right(), size.width() - right - 1);
	}
#endif //_DEBUG
}

void DuiSpliterWidgetPrivate::totalWidth( qreal &minimumWidth, qreal &maximumWidth, qreal &currentWidth )
{
	for (ItemList::const_iterator iter = m_itemList.begin();
		iter != m_itemList.end();
		++iter)
	{
		minimumWidth += (*iter)->minimumWidth() > 0 ? (*iter)->minimumWidth() : 0;
		maximumWidth += (*iter)->maximumWidth() > 0 ? (*iter)->maximumWidth() : 0x00007ffff;
		currentWidth += (*iter)->geometry().width();
	}
}

void DuiSpliterWidgetPrivate::horizontalAdjustBySegment( ItemSegmentMap &segmentMap, qreal top, qreal actualHeight, qreal left )
{
	qreal lastItemRight = left;
	qreal offset = 0;
	QRectF rectItem;
	ItemSegmentMap::iterator iter;
	qreal adjustSize = 0;
	QGraphicsLayoutItem *item = NULL;

	for ( int index = 0; index < m_itemList.size(); ++index )
	{
		item = m_itemList[index];
		iter = segmentMap.find(index);

		if (segmentMap.end() == iter)
		{
			adjustSize = 0;
		}
		else
		{
			adjustSize = iter.value().adjustSize;
		}

		rectItem = item->geometry();
		offset = lastItemRight - rectItem.left();

		rectItem.adjust( offset, 0, offset + adjustSize, 0 );
		rectItem.setTop(top);
		rectItem.setHeight(actualHeight);

		item->setGeometry(rectItem);
		lastItemRight += rectItem.width();
	}
}

void DuiSpliterWidgetPrivate::realyoutVertical( const QSizeF &size )
{
	Q_Q(DuiSpliterWidget);
	qreal left = 0, right = 0, top  = 0, bottom = 0;
	q->getContentsMargins(&left, &right, &top, &bottom);

	qreal actualWidth = size.width() - (left + right);

	qreal minimumHeight = 0, maximumHeight = 0, currentHeight = 0;
	totalHeight( minimumHeight, maximumHeight, currentHeight );

	int remainHeight = size.height() - (top + left + currentHeight);

	ItemSegmentMap anytimeMap;
	ItemSegmentMap userSplitMap;

	getAllSegment(anytimeMap, userSplitMap);

	remainHeight = getAdjustedSegment(anytimeMap, remainHeight);
	if (remainHeight != 0)
	{
		getAdjustedSegment(userSplitMap, remainHeight);
		anytimeMap.unite(userSplitMap);
	}

	verticalAdjustBySegment(anytimeMap, left, actualWidth, top);

#ifdef _DEBUG
	QGraphicsLayoutItem *item = m_itemList[m_itemList.size() - 1];
	QRectF rectItem = item->geometry();
	if (rectItem.bottom() < size.height() - bottom - 1 )
	{
		qDebug("Vertical relayout problem, last item's bottom is: %d, bottom border is: %d. Please check your layout.", rectItem.bottom(), size.height() - bottom - 1);
	}
#endif //_DEBUG
}

void DuiSpliterWidgetPrivate::totalHeight( qreal &minimumHeight, qreal &maximumHeight, qreal &currentHeight )
{
	for (ItemList::const_iterator iter = m_itemList.begin();
		iter != m_itemList.end();
		++iter)
	{
		minimumHeight += (*iter)->minimumHeight() > 0 ? (*iter)->minimumHeight() : 0;
		maximumHeight += (*iter)->maximumHeight() > 0 ? (*iter)->maximumHeight() : 0x00007fff;
		currentHeight += (*iter)->geometry().height();
	}
}

void DuiSpliterWidgetPrivate::verticalAdjustBySegment( ItemSegmentMap &segmentMap, qreal left, qreal actualWidth, qreal top )
{
	qreal lastItemTop = top;
	qreal offset = 0;
	QRectF rectItem;
	ItemSegmentMap::iterator iter;
	qreal adjustSize = 0;
	QGraphicsLayoutItem *item = NULL;

	for ( int index = 0; index < m_itemList.size(); ++index )
	{
		item = m_itemList[index];
		iter = segmentMap.find(index);

		if (segmentMap.end() == iter)
		{
			adjustSize = 0;
		}
		else
		{
			adjustSize = iter.value().adjustSize;
		}

		rectItem = item->geometry();
		offset = lastItemTop - rectItem.top();

		rectItem.adjust( 0, offset, 0, offset + adjustSize );
		rectItem.setLeft(left);
		rectItem.setWidth(actualWidth);

		item->setGeometry(rectItem);
		lastItemTop += rectItem.height();
	}
}

int DuiSpliterWidgetPrivate::getAdjustedSegment( ItemSegmentMap &segmentMap, int remainSize )
{
	if (segmentMap.empty())
	{
		return remainSize;
	}

	qreal currentSize = 0;

	foreach(const ItemSegment &segment, segmentMap)
	{
		currentSize += segment.currentSize + segment.adjustSize;
	}

	DW_ASSERT(currentSize != 0);
	if (qFuzzyCompare(0,currentSize))
	{
		return remainSize;
	}

	qreal scaleFactor = remainSize / currentSize;
	int preferredScaleSize = 0;
	int actualScaleSize = 0;
	ItemSegmentMap remainMap = segmentMap;

	for( ItemSegmentMap::iterator iter = segmentMap.begin();
		segmentMap.end() != iter;
		++iter )
	{
		ItemSegment &segment = *iter;
		actualScaleSize = preferredScaleSize = (segment.currentSize + segment.adjustSize) * scaleFactor + ((remainSize > 0) ? 1 : -1);

		if (preferredScaleSize > 0)
		{
			if (segment.maximumSize < (segment.currentSize + segment.adjustSize) + preferredScaleSize)
			{
				actualScaleSize = segment.maximumSize - (segment.currentSize + segment.adjustSize);
				remainMap.remove(iter.key());
			}
		}
		else
		{
			if (segment.minimumSize > (segment.currentSize + segment.adjustSize) + preferredScaleSize)
			{
				actualScaleSize = segment.minimumSize - (segment.currentSize + segment.adjustSize);
				remainMap.remove(iter.key());
			}
		}

		if (actualScaleSize > 0)
		{
			actualScaleSize = (actualScaleSize > remainSize) ? remainSize : actualScaleSize;
		}
		else if(actualScaleSize < 0)
		{
			actualScaleSize = (actualScaleSize < remainSize) ? remainSize : actualScaleSize;
		}

		segment.adjustSize += actualScaleSize;
		remainSize -= actualScaleSize;
	}

	if (0 != remainSize)
	{
		for( ItemSegmentMap::iterator iter = remainMap.begin();
			remainMap.end() != iter;
			++iter )
		{
			ItemSegmentMap::iterator temp = segmentMap.find(iter.key());
			if (segmentMap.end() != temp)
			{
				*iter = *temp;
			}
		}

		remainSize = getAdjustedSegment(remainMap, remainSize);

		for( ItemSegmentMap::iterator iter = remainMap.begin();
			remainMap.end() != iter;
			++iter )
		{
			ItemSegmentMap::iterator temp = segmentMap.find(iter.key());
			if (segmentMap.end() != temp)
			{
				*temp = *iter;
			}
		}
	}

	return remainSize;
}

qreal DuiSpliterWidgetPrivate::getMinimumDelta( qreal preferredDelta, ItemSegment &segmentPreItem, ItemSegment &segmentNextItem )
{
	qreal minDelta = preferredDelta;

	if (preferredDelta > 0)
	{
		minDelta = qMin( segmentPreItem.maximumSize - segmentPreItem.currentSize, segmentNextItem.currentSize - segmentNextItem.minimumSize);
		minDelta = qMin( minDelta, preferredDelta );
	}
	else
	{
		minDelta = qMax( segmentPreItem.minimumSize - segmentPreItem.currentSize, segmentNextItem.currentSize - segmentNextItem.maximumSize );
		minDelta = qMax( minDelta, preferredDelta );
	}

	return minDelta;
}

void DuiSpliterWidgetPrivate::segmentFromItem( const DuiWidget* item, ItemSegment &segment, bool isHorizontal )
{
	segment.minimumSize = isHorizontal ? (item->minimumWidth() > 0 ? item->minimumWidth() : 0) : (item->minimumHeight() > 0 ? item->minimumHeight() : 0);
	segment.maximumSize = isHorizontal ? qAbs(item->maximumWidth()) : qAbs(item->maximumHeight());
	segment.currentSize = isHorizontal ? item->geometry().width() : item->geometry().height();
	segment.splitPolicy = item->splitPolicy();
}

qreal DuiSpliterWidgetPrivate::getSequenceAdjustedSegment( ItemSegmentMap &segmentMap, int remainSize, ItemSequenceList &sequenceList )
{
	int index;
	bool isAdd = remainSize > 0;

	foreach(index, sequenceList)
	{
		DW_ASSERT(segmentMap.end() != segmentMap.find(index));
		ItemSegmentMap::iterator iter = segmentMap.find(index);
		if (segmentMap.end() == iter)
		{
			continue;
		}
		ItemSegment &segment = *iter;

		if (isAdd)
		{
			if (segment.maximumSize - segment.currentSize > remainSize)
			{
				segment.adjustSize = remainSize;
				remainSize -= remainSize;
			}
			else
			{
				segment.adjustSize = segment.maximumSize - segment.currentSize;
				remainSize -= segment.adjustSize;
			}
		}
		else
		{
			if (segment.minimumSize - segment.currentSize < remainSize)
			{
				segment.adjustSize = remainSize;
				remainSize -= remainSize;
			}
			else
			{
				segment.adjustSize = segment.minimumSize - segment.currentSize;
				remainSize -= segment.adjustSize;
			}
		}

		if (0 == remainSize)
		{
			break;
		}
	}

	return remainSize;
}

qreal DuiSpliterWidgetPrivate::getAcceptableAdjust( ItemSegmentMap &segmentMap, qreal preferredAdjust, ItemSequenceList &sequenceList )
{
	bool isPositive = (preferredAdjust > 0);
	qreal acceptableAdjust = 0, adjustSize = 0;;

	foreach(int index, sequenceList)
	{
		DW_ASSERT(segmentMap.end() != segmentMap.find(index));
		ItemSegmentMap::iterator iter = segmentMap.find(index);
		if (segmentMap.end() == iter)
		{
			continue;
		}
		ItemSegment &segment = *iter;
		if (isPositive)
		{
			adjustSize = segment.maximumSize - segment.currentSize;

			if (preferredAdjust - adjustSize > 0)
			{
				acceptableAdjust += adjustSize;
				preferredAdjust -= adjustSize;
			}
			else
			{
				acceptableAdjust += preferredAdjust;
				preferredAdjust -= preferredAdjust;
			}

			if (preferredAdjust <= 0)
			{
				break;
			}
		}
		else
		{
			adjustSize = segment.minimumSize - segment.currentSize;

			if (preferredAdjust - adjustSize < 0)
			{
				acceptableAdjust += adjustSize;
				preferredAdjust += adjustSize;
			}
			else
			{
				acceptableAdjust += preferredAdjust;
				preferredAdjust -= preferredAdjust;
			}

			if (preferredAdjust >= 0)
			{
				break;
			}
		}
	}

	return acceptableAdjust;
}

void DuiSpliterWidgetPrivate::init()
{
	m_needRelayout = false;
	m_isMouseDown = false;
	m_preMousePos.setX(0);
	m_preMousePos.setY(0);
	m_currentSpliterIndex = -1;
}

void DuiSpliterWidgetPrivate::getSegmentBeforIndex( ItemSegmentMap &segmentMap, ItemSequenceList &sequenceList, int index )
{
	for ( index -= 1; index >= 0; index -= 2 )
	{
		ItemSegment segment;
		segmentFromItem(m_itemList[index], segment, Qt::Horizontal == m_orintation);
		segmentMap.insert(index, segment);
		sequenceList.push_back(index);
	}
}

void DuiSpliterWidgetPrivate::getSegmentAfterIndex( ItemSegmentMap &segmentMap, ItemSequenceList &sequenceList, int index )
{
	for ( index += 1; index < m_itemList.size(); index += 2 )
	{
		ItemSegment segment;
		segmentFromItem(m_itemList[index], segment, Qt::Horizontal == m_orintation);
		segmentMap.insert(index, segment);
		sequenceList.push_back(index);
	}
}

void DuiSpliterWidgetPrivate::getAllSegment( ItemSegmentMap &segmentAnytimeMap, ItemSegmentMap &segmentUserSplitMap )
{
	for ( int index = 0; index < m_itemList.size(); index += 2 )
	{
		ItemSegment segment;
		segmentFromItem(m_itemList[index], segment, Qt::Horizontal == m_orintation);
		if (QSizePolicy::Preferred == segment.splitPolicy)
		{
			segmentUserSplitMap.insert(index, segment);
		}
		else
		{
			segmentAnytimeMap.insert(index, segment);
		}
	}
}

void DuiSpliterWidgetPrivate::split( qreal delta, bool horizontal )
{
	DW_ASSERT(-1 != m_currentSpliterIndex);
	DW_ASSERT( m_currentSpliterIndex >= 1 );
	DW_ASSERT( m_currentSpliterIndex < m_itemList.size() - 1 );

	ItemSegmentMap segmentPreMap;
	ItemSequenceList sequencePreList;

	getSegmentBeforIndex(segmentPreMap, sequencePreList, m_currentSpliterIndex);

	ItemSegmentMap segmentNextMap;
	ItemSequenceList sequenceNextList;

	getSegmentAfterIndex(segmentNextMap, sequenceNextList, m_currentSpliterIndex);

	qreal minDelta = qMin(qAbs(getAcceptableAdjust(segmentPreMap, delta, sequencePreList)), qAbs(getAcceptableAdjust(segmentNextMap, -delta, sequenceNextList)));

	if (delta < 0)
	{
		minDelta = -minDelta;
	}

	getSequenceAdjustedSegment(segmentPreMap, minDelta, sequencePreList);

	getSequenceAdjustedSegment(segmentNextMap, -minDelta, sequenceNextList);

	segmentPreMap.unite(segmentNextMap);
	qreal left, top, right, bottom;

	Q_Q(DuiSpliterWidget);
	q->getContentsMargins(&left, &right, &top, &bottom);

	if (horizontal)
	{
		horizontalAdjustBySegment(segmentPreMap, top, q->size().height() - (top + bottom), left);
	}
	else
	{
		verticalAdjustBySegment(segmentPreMap, left, q->size().width() - (left + right), top);
	}
}

DuiWidget* DuiSpliterWidgetPrivate::itemByIndex( int index ) const
{
	return m_itemList.value(index * 2);
}

int DuiSpliterWidgetPrivate::itemIndex( DuiWidget* item )
{
	int index =  m_itemList.indexOf( item );
	if ( index != -1 )
	{
		index = index / 2;
	}
	return index;
}

bool DuiSpliterWidgetPrivate::setItemPrefferredSize( DuiWidget* item, qreal prefferredSize )
{
	int index = m_itemList.indexOf(item);
	if (-1 == index)
	{
		DW_ASSERT(false);
		return false;
	}
	qreal adjustment = 0;
	if (Qt::Horizontal == m_orintation)
	{
		qreal direction = prefferredSize - item->geometry().width();
		if(direction > 0)
		{
			adjustment = qMin(prefferredSize, item->maximumWidth()) - item->geometry().width();
		}
		else
		{
			adjustment = qMax(prefferredSize, item->minimumWidth()) - item->geometry().width();
		}
	}
	else
	{
		qreal direction = prefferredSize - item->geometry().height();
		if(direction > 0)
		{
			adjustment = qMin(prefferredSize, item->maximumHeight()) - item->geometry().height();
		}
		else
		{
			adjustment = qMax(prefferredSize, item->minimumHeight()) - item->geometry().height();
		}
	}

	if (qFuzzyCompare(0, adjustment))
	{
		return false;
	}

	ItemSegmentMap nextSegmentMap;
	ItemSequenceList nextSegmentList;
	getSegmentAfterIndex(nextSegmentMap, nextSegmentList, index + 1);

	qreal remainSize = getSequenceAdjustedSegment(nextSegmentMap, -adjustment, nextSegmentList);

	ItemSegment segment;
	segmentFromItem(item, segment, Qt::Horizontal == m_orintation);
	segment.adjustSize = adjustment -(- remainSize);
	nextSegmentMap.insert(index, segment);

	qreal left, top, right, bottom;

	Q_Q(DuiSpliterWidget);
	q->getContentsMargins(&left, &right, &top, &bottom);

	if (Qt::Horizontal == m_orintation)
	{
		horizontalAdjustBySegment(nextSegmentMap, top, q->size().height() - (top + bottom), left);
	}
	else
	{
		verticalAdjustBySegment(nextSegmentMap, left, q->size().width() - (left + right), top);
	}

	if (qFuzzyCompare(0, remainSize))
	{
		return false;
	}

	ItemSegmentMap preSegmentMap;
	ItemSequenceList preSegmentList;
	getSegmentBeforIndex(preSegmentMap, preSegmentList, index - 1);

	adjustment = -remainSize;
	remainSize = getSequenceAdjustedSegment(preSegmentMap, -adjustment, preSegmentList);

	segmentFromItem(item, segment, Qt::Horizontal == m_orintation);
	segment.adjustSize = adjustment -(- remainSize);
	preSegmentMap.insert(index, segment);

	if (Qt::Horizontal == m_orintation)
	{
		horizontalAdjustBySegment(preSegmentMap, top, q->size().height() - (top + bottom), left);
	}
	else
	{
		verticalAdjustBySegment(preSegmentMap, left, q->size().width() - (left + right), top);
	}
    return true;
}

bool DuiSpliterWidgetPrivate::getSpliterPreferredSize(int index , int& w, int& h)
{
    int spliterIndex = 2*index + 1;
    if (spliterIndex < m_itemList.size())
    {
        DuiSpliter * spliter = (DuiSpliter*)(m_itemList.at(spliterIndex));
        if (spliter)
        {
            QSizeF size = spliter->preferredSize();
            w = size.width();
            h = size.height();
            return true;
        }
    }

    return false;
}

ISpliterPolicy* DuiSpliter::m_policy = NULL;

DuiSpliter::DuiSpliter(QGraphicsItem *parent, int idx)
: DuiWidget(parent)
, m_index(idx)
, m_isDragging(false)
, m_isHorizontal(true)
{
	//setColorizePolicy(ColorableFlag);
}

DuiSpliter::~DuiSpliter()
{

}

void DuiSpliter::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if (m_policy != NULL)
	{
		DuiSpliterWidget *spliter = (DuiSpliterWidget*)parentItem();
		if (spliter != NULL)
		{
			if (m_policy->doPaint(spliter->objectName(), m_index, painter, rect().toRect()))
			{
				return;
			}
		}		
	}
    
    painter->fillRect(rect(),QColor(0xeb,0xee,0xf1));//yy7.0ÐÞ¸ÄÎª#ebeef1
    
    /*
	if (m_isHorizontal)
	{
		QPointF topLeft = rect().topLeft();
		QPointF bottomLeft = rect().bottomLeft();
		painter->setPen( qRgb(255, 255, 255) );
		painter->drawLine( topLeft, bottomLeft );
		topLeft.setX(topLeft.x() + 1);
		bottomLeft.setX(bottomLeft.x() + 1);

		painter->setPen( DuiTheme::getColor(DuiTheme::LightColor) );
		painter->drawLine( topLeft, bottomLeft );
		topLeft.setX(topLeft.x() + 1);
		bottomLeft.setX(bottomLeft.x() + 1);

		painter->setPen( DuiTheme::getColor(DuiTheme::MoreLightColor) );
		painter->drawLine( topLeft, bottomLeft );
	}
	else
	{
		QPointF topLeft = rect().topLeft();
		QPointF topRight = rect().topRight();

		painter->setPen( DuiTheme::getColor(DuiTheme::MoreLightColor) );
		painter->drawLine( topLeft, topRight );
		topLeft.setY(topLeft.y() + 1);
		topRight.setY(topRight.y() + 1);

		painter->setPen( DuiTheme::getColor(DuiTheme::LightColor) );
		painter->drawLine( topLeft, topRight );
		topLeft.setY(topLeft.y() + 1);
		topRight.setY(topRight.y() + 1);

		painter->setPen( qRgb(255, 255, 255) );
		painter->drawLine( topLeft, topRight );
	}
	*/
}

void DuiSpliter::setIsDragging( bool isDragging )
{
	if (m_isDragging == isDragging)
	{
		return;
	}

	m_isDragging = isDragging;

	update();
}

void DuiSpliter::setHorizontal( bool isHorizontal )
{
	m_isHorizontal = isHorizontal;
}

DuiSpliter* DuiSpliter::createObject(QGraphicsItem *parent, Qt::Orientation orientation, int idx)
{
	DuiSpliter* spliter = new DuiSpliter(parent, idx);

	int w = 2;
	int h = 2;
	if (m_policy != NULL)
	{
		DuiSpliterWidget *parent = (DuiSpliterWidget*)spliter->parentItem();
		if (parent != NULL)
		{
			m_policy->getPreferredSize(parent->objectName(), idx, w, h);
		}
	}
	spliter->setPreferredSize(w, h);
	spliter->setGeometry(0, 0, w, h);

	if (orientation == Qt::Horizontal)
	{
		spliter->setCursor(Qt::SplitHCursor);
		spliter->setHorizontal(true);
	}
	else
	{
		spliter->setCursor(Qt::SplitVCursor);
		spliter->setHorizontal(false);
	}

	return spliter;
}

void DuiSpliter::setSpliterPolicy(ISpliterPolicy *policy)
{
	m_policy = policy;
}