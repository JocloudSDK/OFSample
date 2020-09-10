#pragma once

#include "duiwidget_p.h"
#include "duispliterwidget.h"


struct ItemSegment
{
	qreal minimumSize;
	qreal maximumSize;
	qreal currentSize;
	int adjustSize;
	QSizePolicy::Policy splitPolicy;
	ItemSegment()
	{
		minimumSize = 0;
		maximumSize = 0;
		currentSize = 0;
		adjustSize = 0;
		splitPolicy = QSizePolicy::Expanding;
	}
};

class DuiSpliter : public DuiWidget
{
    Q_OBJECT;
	Q_DISABLE_COPY(DuiSpliter)
public:
	~DuiSpliter();
	void setIsDragging(bool isDragging);
	void setHorizontal(bool isHorizontal);

	static DuiSpliter* createObject(QGraphicsItem *parent, Qt::Orientation orientation, int idx);
	static void setSpliterPolicy(ISpliterPolicy *policy);

protected:
	DuiSpliter( QGraphicsItem *parent, int idx);
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:
	bool m_isDragging;
	bool m_isHorizontal;
	int  m_index; // 按顺序分配的索引号
	static ISpliterPolicy *m_policy;
};

class DuiSpliterWidgetPrivate : public DuiWidgetPrivate
{
	Q_DISABLE_COPY(DuiSpliterWidgetPrivate)
	Q_DECLARE_PUBLIC(DuiSpliterWidget)

	explicit DuiSpliterWidgetPrivate();
	~DuiSpliterWidgetPrivate();

	void setOrientation( Qt::Orientation orientation );
	Qt::Orientation orientation() const;
	int insertItem( DuiWidget* item, int index = -1 );
	void removeItem( DuiWidget* item );
	void removeItem( int index );
	DuiWidget *itemByIndex( int index ) const;
	
	int itemIndex( DuiWidget* item );
	bool setItemPrefferredSize( DuiWidget* item, qreal prefferredSize );
    bool getSpliterPreferredSize(int index , int& w, int& h);

	void resizeEvent( QGraphicsSceneResizeEvent *event );
	void showEvent( QShowEvent *event );
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

	typedef QMap<int,ItemSegment> ItemSegmentMap;
	typedef QList<int> ItemSequenceList;
	void relayout( const QSizeF &size );

	void relayoutHorizontal( const QSizeF &size );
	void totalWidth( qreal &minimumWidth, qreal &maximumWidth, qreal &currentWidth );
	void horizontalAdjustBySegment(ItemSegmentMap &segmentMap, qreal top, qreal actualHeight, qreal left);


	void realyoutVertical( const QSizeF &size );
	void totalHeight( qreal &minimumHeight, qreal &maximumHeight, qreal &currentHeight );
	void verticalAdjustBySegment(ItemSegmentMap &segmentMap, qreal left, qreal actualWidth, qreal top);

	int getAdjustedSegment( ItemSegmentMap &segmentMap, int remainSize );
	qreal getMinimumDelta( qreal preferredDelta, ItemSegment &segmentPreItem, ItemSegment &segmentNextItem );
	void segmentFromItem( const DuiWidget* item, ItemSegment &segment, bool isHorizontal );
	qreal getSequenceAdjustedSegment( ItemSegmentMap &segmentMap, int remainSize, ItemSequenceList &sequenceList );
	qreal getAcceptableAdjust( ItemSegmentMap &segmentMap, qreal preferredAdjust, ItemSequenceList &sequenceList );

	void init();
	void getSegmentBeforIndex( ItemSegmentMap &segmentMap, ItemSequenceList &sequenceList, int index );
	void getSegmentAfterIndex( ItemSegmentMap &segmentMap, ItemSequenceList &sequenceList, int index );
	void getAllSegment( ItemSegmentMap &segmentAnytimeMap, ItemSegmentMap &segmentUserSplitMap );
	void split( qreal delta, bool horizontal );

	Qt::Orientation m_orintation;
	int m_index;

	typedef QList<DuiWidget*> ItemList;
	ItemList m_itemList;
	bool m_needRelayout;
	bool m_isMouseDown;
	QPointF m_preMousePos;
	int m_currentSpliterIndex;
};