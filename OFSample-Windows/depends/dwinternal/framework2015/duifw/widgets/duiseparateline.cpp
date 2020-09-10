#include "stable.h"

#include "duiwidget_p.h"

#include "duiseparateline.h"

class DuiSeparateLinePrivate : public DuiWidgetPrivate
{
public:
	Q_DECLARE_PUBLIC( DuiSeparateLine );

	DuiSeparateLinePrivate( void );
	
	void init();
private:
	QPen m_pen;
	Qt::Orientation	m_lineOrientation;
	QString m_imagePath;

};

DuiSeparateLinePrivate::DuiSeparateLinePrivate() 
: m_pen( Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin )
, m_lineOrientation( Qt::Horizontal )
{
}

void DuiSeparateLinePrivate::init()
{
}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiSeparateLine )

DuiSeparateLine::DuiSeparateLine( QGraphicsItem *parent )
	: DuiWidget(*(new DuiSeparateLinePrivate()), parent)
{
	d_func()->init();
}


DuiSeparateLine::~DuiSeparateLine()
{

}

void DuiSeparateLine::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */ )
{
	Q_UNUSED( widget );
	Q_UNUSED( option );

	Q_D( DuiSeparateLine );
	if (!d->m_imagePath.isEmpty())
	{
		QImage image(d->m_imagePath);
        if (colorizePolicy() == ColorableFlag)
        {
            int type = themeColorType();
            painter->drawImage(rect(), DuiTheme::colorizeImage(image, DuiTheme::getColor((DuiTheme::ColorType)type)));
        }
        else
        {
            painter->drawImage(rect(), image);
        }
		
		return;
	}
	painter->setPen( d->m_pen);

	QPointF startPoint, endPoint;

	//lHorizontal
	switch ( d_func()->m_lineOrientation )
	{
	case Qt::Horizontal:
		{
			startPoint.setX( 0 );
			startPoint.setY( size().height() / 2 );
			endPoint.setX( size().width() );
			endPoint.setY( size().height() / 2 );
		}
		break;
	case Qt::Vertical:
	default:
		{
			startPoint.setX( size().width() / 2 );
			startPoint.setY( 0 );
			endPoint.setX( size().width() / 2  );
			endPoint.setY( size().height() );
		}
	}

	painter->drawLine( startPoint, endPoint );
}

int DuiSeparateLine::lineWidth() const
{
	return d_func()->m_pen.width();
}

void DuiSeparateLine::setLineWidth( const int lineWidth )
{
	d_func()->m_pen.setWidth( lineWidth );
}

QColor DuiSeparateLine::color() const
{
	return d_func()->m_pen.color();
}

void DuiSeparateLine::setColor( const QColor& color )
{
	d_func()->m_pen.setColor( color );
}

Qt::PenStyle DuiSeparateLine::penStyle() const
{
	return d_func()->m_pen.style();
}

void DuiSeparateLine::setPenStyle( const Qt::PenStyle penStyle )
{
	if(penStyle == Qt::CustomDashLine)	//如果是自定义虚线样式（在系统设置中的虚线用到）
	{
		QVector<qreal> dashes;
		qreal solid = 2;				//两个像素的实点
		qreal space = 2;				//两个像素的虚点

		dashes << solid << space ;

        setDashPattern(dashes);
	}
	d_func()->m_pen.setStyle( penStyle );	
}

void DuiSeparateLine::setDashPattern(const QVector<qreal> &dashPattern)
{
    d_func()->m_pen.setDashPattern(dashPattern);	
}

QVector<qreal> DuiSeparateLine::dashPattern() const
{
    return d_func()->m_pen.dashPattern();
}

Qt::Orientation DuiSeparateLine::lineOrientation() const
{
	return d_func()->m_lineOrientation;
}

void DuiSeparateLine::setLineOrientation( Qt::Orientation orientation )
{
	d_func()->m_lineOrientation = orientation;
}

void DuiSeparateLine::doColorize( QRgb color )
{
	Q_UNUSED( color );
	setColor( DuiTheme::getColor( (DuiTheme::ColorType)themeColorType() ) );
}

void DuiSeparateLine::setThemeColorType( int colorType )
{
	DuiWidget::setThemeColorType( colorType );
	setColorizePolicy( ColorableFlag );

	setColor( DuiTheme::getColor( (DuiTheme::ColorType)colorType ) );

	//TODO: refactor, create pen object when painting.
}

QString DuiSeparateLine::imagePath()
{
	return d_func()->m_imagePath;
}


void DuiSeparateLine::setImagePath(const QString& path)
{
	d_func()->m_imagePath = path;
}
