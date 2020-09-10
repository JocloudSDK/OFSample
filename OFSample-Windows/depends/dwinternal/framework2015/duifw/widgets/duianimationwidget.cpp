#include "stable.h"

#include <QMovie>

#include "duiwidget_p.h"
#include "duianimationwidget.h"
#include "duifw/duiresourceloader.h"


class DuiAnimationWidgetPrivate : public DuiWidgetPrivate
{
public:
	Q_DECLARE_PUBLIC( DuiAnimationWidget );

	DuiAnimationWidgetPrivate(void)
		: m_displayGif( NULL )
		, m_movieShow( NULL )
		, m_isPlayLoop( true )
	{
	}

	~DuiAnimationWidgetPrivate(void)
	{

	}

	QPoint getCenterPos( QSize& imageItemSize, QSize& widgetSize );
	void resetPixmapItemDefaultPos( );

private:
	bool				m_isPlayLoop;
	QGraphicsPixmapItem	*m_displayGif;		//ÏÔÊ¾gifÍ¼Æ¬
	QMovie				*m_movieShow;		//²¥·ÅÆ÷
	QColor				m_borderColor;
	QByteArray			m_data;
	QBuffer				m_buf;
	QSizeF				m_imageScaleSize;
};

QPoint DuiAnimationWidgetPrivate::getCenterPos( QSize& imageItemSize, QSize& widgetSize )
{
	int x = ( widgetSize.width() - imageItemSize.width() ) / 2 ;
	int y = ( widgetSize.height() - imageItemSize.height() ) / 2 ;

	return QPoint( x, y );
}

void DuiAnimationWidgetPrivate::resetPixmapItemDefaultPos()
{
	DW_ASSERT( m_displayGif );
	if (m_displayGif == NULL )
	{
		return;
	}

	m_displayGif->setPos(1, 1);
}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiAnimationWidget)

DuiAnimationWidget::DuiAnimationWidget( QGraphicsItem* parent ) :
DuiWidget(*new DuiAnimationWidgetPrivate(), parent)
{
	init();
}

void DuiAnimationWidget::init()
{
	Q_D( DuiAnimationWidget );

	d->m_displayGif = new QGraphicsPixmapItem(this);
	d->m_displayGif->setTransformationMode( Qt::SmoothTransformation );

	d->m_movieShow = new QMovie(this);
	//d->m_movieShow->setCacheMode( QMovie::CacheAll );
	connect(d->m_movieShow, SIGNAL(frameChanged(int)), this, SLOT(on_frameChanged(int)));
}

void DuiAnimationWidget::on_frameChanged ( int frameNumber )
{
	Q_UNUSED(frameNumber);
	Q_D(DuiAnimationWidget);

	if (!d->m_isPlayLoop && frameNumber == d->m_movieShow->frameCount() - 1 )
	{
		hide();
		d->m_movieShow->stop();
	}
	else
	{
		if ( d->m_imageScaleSize.isValid() )
		{
			if ( QRect(0, 0, d->m_imageScaleSize.width(), d->m_imageScaleSize.height() ).contains(d->m_movieShow->currentPixmap().rect()) )
			{
				//ÉèÖÃÍ¼¾ÓÖÐ
				d->m_displayGif->setPos( d->getCenterPos(d->m_movieShow->currentPixmap().size(), d->m_imageScaleSize.toSize() ) ); 
				d->m_displayGif->setPixmap( d->m_movieShow->currentPixmap() );
			}
			else
			{
				d->resetPixmapItemDefaultPos();
				d->m_displayGif->setPixmap( d->m_movieShow->currentPixmap().scaled( d->m_imageScaleSize.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
			}
		}
		else
		{
			d->m_displayGif->setPixmap( d->m_movieShow->currentPixmap() );
		}	
	}
}

QString DuiAnimationWidget::imagePath() const
{
	return d_func()->m_movieShow->fileName();
}

void DuiAnimationWidget::setImage( const QString &imageName )
{
	Q_D( DuiAnimationWidget );
	
	d->m_movieShow->stop();

	if ( imageName.isEmpty() )
	{
		d->m_displayGif->setPixmap( QPixmap() );
		d->m_data.clear();
		d->m_buf.close();
		d->m_movieShow->setDevice( NULL );
	}
	else
	{
		d->m_data = getResourceLoader()->loadRawData(imageName);

		d->m_buf.close();
		d->m_buf.setData( d->m_data );
		d->m_movieShow->setDevice( &d->m_buf );
	}
}

void DuiAnimationWidget::start()
{
	Q_D( DuiAnimationWidget );
	d->m_movieShow->start();
}

void DuiAnimationWidget::stop()
{
	Q_D( DuiAnimationWidget );
	d->m_movieShow->stop();
}

void DuiAnimationWidget::setPaused(bool b)
{
    Q_D( DuiAnimationWidget );
    d->m_movieShow->setPaused(b);
}

void DuiAnimationWidget::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
	DuiWidget::resizeEvent(event);
}

void DuiAnimationWidget::on_finished()
{
	Q_D(DuiAnimationWidget);

	if ( !d->m_isPlayLoop )
	{
		hide();
		d->m_movieShow->stop();
	}
}

void DuiAnimationWidget::setPlayLoop( bool isLoop )
{
	Q_D( DuiAnimationWidget );
	d->m_isPlayLoop = isLoop;
}

bool DuiAnimationWidget::playLoop() const
{
	Q_D( const DuiAnimationWidget );
	return d->m_isPlayLoop;
}

void DuiAnimationWidget::setImageScaleSize( const QSizeF& size )
{
	d_func()->m_imageScaleSize = size;
}

QSizeF DuiAnimationWidget::imageScaleSize() const
{
	return d_func()->m_imageScaleSize;
}

QColor DuiAnimationWidget::borderColor() const
{
	return d_func()->m_borderColor;	
}

void DuiAnimationWidget::setBorderColor( const QColor &color )
{
	Q_D( DuiAnimationWidget );

	d->m_borderColor = color;
	d->resetPixmapItemDefaultPos();
}

void DuiAnimationWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_UNUSED( widget );
	Q_UNUSED( option );

	if ( d_func()->m_borderColor.isValid() )
	{
		QPainterPath outerPath;
		QRect outerRect = rect().toRect().adjusted( 0, 0, -1, -1 );
		outerPath.addRoundedRect(outerRect, 2, 2);
		painter->setPen( d_func()->m_borderColor );
		painter->drawPath(outerPath);
	}
}
