#include "stable.h"
#include "duitooltipstyle2.h"

class DuiToolTipStyle2Private
{
public:
	DuiToolTipStyle2Private();
public:
	QBrush			m_brush;
	QPen			m_borderPen;
	quint32			m_triangleValue;
	quint32			m_triangleOffset;
	quint32			m_triangleOffsetPercent;
	quint32			m_roundValue;
	int				m_referenceSpacing;
	int				m_referenceOffset;
	int				m_referencePercent;
	Qt::ArrowType	m_arrowType;
	DuiToolTipStyle2::ShowStyle		m_showStyle;
	DuiToolTipStyle2::ReferenceType	m_referenceType;
};

DuiToolTipStyle2Private::DuiToolTipStyle2Private()
: m_arrowType( Qt::UpArrow )
, m_triangleValue(0)
, m_roundValue(2)
, m_triangleOffset(0)
, m_referenceType(DuiToolTipStyle2::ReferenceMouse)
, m_referenceSpacing(0)
, m_referenceOffset(0)
, m_triangleOffsetPercent(0)
, m_referencePercent(0)
, m_showStyle(DuiToolTipStyle2::AnimationShow)
{
	QLinearGradient liner;
	liner.setColorAt(0,QColor(255, 255, 255));
	liner.setColorAt(1,QColor(255, 255, 255));

	m_brush = liner;
	m_borderPen = QPen(QColor(168,192,203));
}

DuiToolTipStyle2::DuiToolTipStyle2()
: d_ptr( new DuiToolTipStyle2Private() )
{

}

DuiToolTipStyle2::DuiToolTipStyle2( const DuiToolTipStyle2& style )
: d_ptr( new DuiToolTipStyle2Private(*(style.d_ptr)) )
{

}

DuiToolTipStyle2& DuiToolTipStyle2::operator =( const DuiToolTipStyle2& style )
{
	if ( this != &style )
	{
		*d_ptr = *style.d_ptr;
	}
	return *this;
}

DuiToolTipStyle2::~DuiToolTipStyle2()
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiToolTipStyle2::setBackground( const QBrush& backGroundBrush )
{
	Q_D(DuiToolTipStyle2);
	d->m_brush = backGroundBrush;
}

void DuiToolTipStyle2::setBorder( const QPen& border )
{
	Q_D(DuiToolTipStyle2);
	d->m_borderPen = border;
}

void DuiToolTipStyle2::setTriangleHeight( uint value )
{
	Q_D(DuiToolTipStyle2);
	d->m_triangleValue = value;
}

void DuiToolTipStyle2::setRoundedRect( quint32 round )
{
	Q_D(DuiToolTipStyle2);
	d->m_roundValue = round;
}

void DuiToolTipStyle2::setArrowType( Qt::ArrowType type )
{
	Q_D(DuiToolTipStyle2);
	d->m_arrowType = type;
}

QPen DuiToolTipStyle2::border() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_borderPen;
}

quint32 DuiToolTipStyle2::roundedRect() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_roundValue;
}

QBrush DuiToolTipStyle2::background() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_brush;
}

quint32 DuiToolTipStyle2::triangleHeight() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_triangleValue;
}

quint32 DuiToolTipStyle2::triangleOffset() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_triangleOffset;
}

Qt::ArrowType DuiToolTipStyle2::arrowType() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_arrowType;
}

void DuiToolTipStyle2::setReferenceType( ReferenceType type )
{
	Q_D(DuiToolTipStyle2);
	d->m_referenceType = type;
}

DuiToolTipStyle2::ReferenceType DuiToolTipStyle2::referenceType() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_referenceType;
}

void DuiToolTipStyle2::setReferenceSpacing( int spacing )
{
	Q_D(DuiToolTipStyle2);
	d->m_referenceSpacing = spacing;
}

int DuiToolTipStyle2::referenceSpacing() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_referenceSpacing;
}

void DuiToolTipStyle2::setReferenceOffset( int offset )
{
	Q_D(DuiToolTipStyle2);
	d->m_referenceOffset = offset;
}

int DuiToolTipStyle2::referenceOffset() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_referenceOffset;
}

void DuiToolTipStyle2::setTriangleOffset( quint32 offset )
{
	Q_D(DuiToolTipStyle2);
	d->m_triangleOffset = offset;
}

void DuiToolTipStyle2::setTriangleOffsetPercent( quint32 percentage )
{
	Q_D(DuiToolTipStyle2);
	d->m_triangleOffsetPercent = percentage;
}

quint32 DuiToolTipStyle2::triangleOffsetPercent() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_triangleOffsetPercent;
}

int DuiToolTipStyle2::referencePercent() const
{	
	Q_D(const DuiToolTipStyle2);
	return d->m_referencePercent;
}

void DuiToolTipStyle2::setReferenceOffsetPercent( int percent )
{
	Q_D(DuiToolTipStyle2);
	d->m_referencePercent = percent;
}

DuiToolTipStyle2::ShowStyle DuiToolTipStyle2::showStyle() const
{
	Q_D(const DuiToolTipStyle2);
	return d->m_showStyle;
}

void DuiToolTipStyle2::setShowStyle( ShowStyle showStyle )
{
	Q_D(DuiToolTipStyle2);
	d->m_showStyle = showStyle;
}
