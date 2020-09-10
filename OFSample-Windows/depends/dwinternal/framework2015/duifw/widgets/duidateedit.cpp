#include "stable.h"

#include "duiwidget_p.h"
#include "duidateedit.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiDateEdit)

class DuiDateEditPrivate: public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiDateEdit)

public:
	DuiDateEditPrivate();
	~DuiDateEditPrivate();

	QDateEdit	*m_dateEdit;
};

DuiDateEditPrivate::DuiDateEditPrivate()
: m_dateEdit( NULL )
{

}

DuiDateEditPrivate::~DuiDateEditPrivate()
{
}

//////////////////////////////////////////////////////////////////////////
DuiDateEdit::DuiDateEdit( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget(*(new DuiDateEditPrivate()), parent)
{
	_init();
}

void DuiDateEdit::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_UNUSED( event );
	_adjustNativeGeometry( rect() );
}

void DuiDateEdit::_init()
{
	Q_D(DuiDateEdit);

	setDragPolicy(NotDragWindow);

	d->m_dateEdit = new QDateEdit( topQWidget() );
	d->m_dateEdit->setEnabled(true);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
}
void DuiDateEdit::setMinimumWidth(qreal width)
{
	Q_D(DuiDateEdit);
	d->m_dateEdit->setMinimumWidth( width );
}

QVariant DuiDateEdit::itemChange( GraphicsItemChange change, const QVariant & value )
{
	Q_UNUSED( value );

	if (change == ItemScenePositionHasChanged)
	{
		_adjustNativeGeometry(rect());
	}
	else if ( change == ItemVisibleHasChanged )
	{
		d_func()->m_dateEdit->setVisible( isVisible() );
	}

	return DuiWidget::itemChange( change, value);
}

void DuiDateEdit::_adjustNativeGeometry( const QRectF &r )
{
	Q_D(DuiDateEdit);

	QRectF sceneRect = mapRectToScene(r);
	d->m_dateEdit->setGeometry( sceneRect.toRect()  );

	QPainterPath path = clipPath();
	if (!path.isEmpty())
	{
		QPolygon poly = path.toFillPolygon().toPolygon();
		QRegion region(poly);

		if (!region.isEmpty())
		{
			d->m_dateEdit->setVisible(isVisible());
			d->m_dateEdit->setMask(region);
		}
	}
}

void DuiDateEdit::moveEvent( QGraphicsSceneMoveEvent * event )
{
	DuiWidget::moveEvent( event );
	_adjustNativeGeometry( rect() );
}

void DuiDateEdit::setMinimumDate( const QDate & min )
{
	d_func()->m_dateEdit->setMinimumDate( min );
}

void DuiDateEdit::setCalendarPopup( bool isPopup )
{
	d_func()->m_dateEdit->setCalendarPopup( isPopup );
}

void DuiDateEdit::setDisplayFormat( const QString& format )
{
	d_func()->m_dateEdit->setDisplayFormat( format );
}

QDate DuiDateEdit::date() const 
{
	return d_func()->m_dateEdit->date();
}

QString DuiDateEdit::text() const
{
	return d_func()->m_dateEdit->text();
}
