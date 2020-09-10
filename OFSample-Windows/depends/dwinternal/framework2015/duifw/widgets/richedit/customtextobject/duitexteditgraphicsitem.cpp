#include "stable.h"

#include "duitexteditgraphicsitem.h"
#include "duitexteditgraphicsitem_p.h"
#include "animation/duitexteditgraphicsanimation.h"
#include "animation/duitexteditgraphicsabstractstate.h"
#include "widgets/richedit/richtextbrowser.h"

DuiTextEditGraphicsItem::DuiTextEditGraphicsItem( ) 
: d_ptr( new DuiTextEditGraphicsItemPrivate )
{
	d_ptr->q_ptr = this;

	d_func()->init();
}

DuiTextEditGraphicsItem::DuiTextEditGraphicsItem( DuiTextEditGraphicsItemPrivate &dd )
: d_ptr( &dd )
{
	d_ptr->q_ptr = this;
	d_func()->init();
}

DuiTextEditGraphicsItem::~DuiTextEditGraphicsItem()
{
	DW_ASSERT_X(d_ptr, __FUNCTION__, "private data is null.");
	delete d_ptr;
	d_ptr = NULL;
}

void DuiTextEditGraphicsItem::setPos( const QPointF & pos )
{
	d_func()->m_pos = pos;
}

void DuiTextEditGraphicsItem::setPos( qreal x, qreal y )
{
	d_func()->m_pos = QPointF( x, y );
}

QPointF DuiTextEditGraphicsItem::absolutePos( const QRectF& rect ) const
{
	return QPointF( rect.left() + pos().x(), rect.top() + pos().y() );	
}

QPointF DuiTextEditGraphicsItem::pos() const
{
	return d_func()->m_pos;
}

void DuiTextEditGraphicsItem::mousePressEvent( QMouseEvent *ev, quint32 textObjectIndex )
{
	Q_UNUSED( ev );
	Q_UNUSED( textObjectIndex );

	qDebug() << "TextObjectIndex=" << textObjectIndex << "DuiTextEditGraphicsItem::mousePressEvent"  << ev->pos();
}

void DuiTextEditGraphicsItem::mouseMoveEvent( QMouseEvent *ev, quint32 textObjectIndex )
{
	Q_UNUSED( ev );
	Q_UNUSED( textObjectIndex );
}

void DuiTextEditGraphicsItem::mouseReleaseEvent( QMouseEvent *ev, quint32 textObjectIndex )
{
	Q_UNUSED( ev );
	Q_UNUSED( textObjectIndex );
}

void DuiTextEditGraphicsItem::hoverEnterEvent( QMouseEvent * ev, quint32 textObjectIndex )
{
	Q_UNUSED( ev );
	Q_UNUSED( textObjectIndex );
	qDebug() << "TextObjectIndex=" << textObjectIndex << "DuiTextEditGraphicsItem::hoverEnterEvent"  << ev->pos();
    QCursor cur = cursor();
    if(cur.shape() != d_func()->m_hoverCursorShape)
    {
        d_func()->m_hoverOldCursor = cur.shape();
        setCursor(d_func()->m_hoverCursorShape);
    }
}

void DuiTextEditGraphicsItem::hoverLeaveEvent( QMouseEvent * ev, quint32 textObjectIndex )
{
	Q_UNUSED( ev );
	Q_UNUSED( textObjectIndex );
	qDebug() << "TextObjectIndex=" << textObjectIndex << "DuiTextEditGraphicsItem::hoverLeaveEvent"  << ev->pos();
    QCursor cur = cursor();
    if(cur.shape() != d_func()->m_hoverOldCursor)
    {
        setCursor(d_func()->m_hoverOldCursor);
    }
}

void DuiTextEditGraphicsItem::_dispatchHoverEvent( QMouseEvent* event, bool isMouseMove, quint32 textObjectIndex )
{
	Q_D( DuiTextEditGraphicsItem );
	
	if ( !acceptHoverEvents() )
	{
		return;
	}

	if ( isMouseMove && d->m_hoverStatus == DuiTextEditGraphicsItemPrivate::Leave )
	{
		hoverEnterEvent( event, textObjectIndex );
	}

	if ( !isMouseMove && d->m_hoverStatus == DuiTextEditGraphicsItemPrivate::Hover )
	{
		hoverLeaveEvent( event, textObjectIndex );
	}

	d->m_hoverStatus = ( isMouseMove ? DuiTextEditGraphicsItemPrivate::Hover : DuiTextEditGraphicsItemPrivate::Leave );
}

void DuiTextEditGraphicsItem::_setContainerWidget( QWidget* widget )
{
	d_func()->m_containerWidget = widget;
}

void DuiTextEditGraphicsItem::update()
{
	DW_ASSERT( d_func()->m_containerWidget );

	if ( d_func()->m_containerWidget == NULL )
	{
		return;
	}

	d_func()->m_containerWidget->update();
}

void DuiTextEditGraphicsItem::setAcceptHoverEvents( bool enabled )
{
	d_func()->m_enableHoverEvent = enabled;
}

bool DuiTextEditGraphicsItem::acceptHoverEvents() const
{
	return d_func()->m_enableHoverEvent;
}

void DuiTextEditGraphicsItem::setTextObjectIndex( int index )
{
	d_func()->m_textObjectIndex = index;
}

int DuiTextEditGraphicsItem::textObjectIndex() const
{
	return d_func()->m_textObjectIndex;
}

void DuiTextEditGraphicsItem::setAcceptedMouseButtons( quint32 buttons )
{
	d_func()->m_acceptedMouseButtons = buttons;
}

quint32 DuiTextEditGraphicsItem::acceptedMouseButtons() const
{
	return d_func()->m_acceptedMouseButtons;
}

QString DuiTextEditGraphicsItem::objectName() const
{
	return d_func()->m_objectName;
}

void DuiTextEditGraphicsItem::setObjectName( const QString& objectName )
{
	d_func()->m_objectName = objectName;
}

QVariant DuiTextEditGraphicsItem::property( const QString& propertyKey ) const
{
	return d_func()->m_propertyData.value( propertyKey, "" );
}

void DuiTextEditGraphicsItem::setProperty( const QString& propertyKey, const QVariant & value )
{
	d_func()->m_propertyData.insert( propertyKey, value );
}

QSizeF DuiTextEditGraphicsItem::size() const
{
	return d_func()->m_itemSize;
}

void DuiTextEditGraphicsItem::setSize( const QSizeF& size )
{
	d_func()->m_itemSize = size;
}

bool DuiTextEditGraphicsItem::isVisible() const
{
	return d_func()->m_isVisible;
}

void DuiTextEditGraphicsItem::setVisible( bool visible )
{
	d_func()->m_isVisible = visible;
	update();
}

void DuiTextEditGraphicsItem::animationChanged( DuiTextEditGraphicsAbstractState* state )
{
	Q_UNUSED(state);	
}

void DuiTextEditGraphicsItem::setAnimationObject( DuiTextEditGraphicsAnimation* animation )
{
	d_func()->m_animation = animation;
}

DuiTextEditGraphicsAnimation* DuiTextEditGraphicsItem::animationObject() const
{
	return d_func()->m_animation;
}

void DuiTextEditGraphicsItem::setCursor( const QCursor& cursor )
{
	RichTextEdit* pedit = (RichTextEdit*)(d_func()->m_containerWidget.data());
    pedit->viewport()->setCursor( cursor );
}

QCursor DuiTextEditGraphicsItem::cursor() const
{
    RichTextEdit* pedit = (RichTextEdit*)(d_func()->m_containerWidget.data());
    return pedit->viewport()->cursor();
}

void DuiTextEditGraphicsItem::setHoverCursorShape( Qt::CursorShape shape )
{
    d_func()->m_hoverCursorShape = shape;
}

QString DuiTextEditGraphicsItem::gifImagePath() const
{
	return QString();
}

QByteArray DuiTextEditGraphicsItem::gifImageData() const
{
    return QByteArray();
}
