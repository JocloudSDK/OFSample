#include "stable.h"

#include "core/duiframeitem.h"
#include "duiwidget_p.h"
#include "duitheme.h"
#include "duiprogressbar.h"

class DuiProgressBarStripeItem : public DuiWidget
{
public:
	explicit DuiProgressBarStripeItem( QGraphicsItem* parent = NULL );
	~DuiProgressBarStripeItem(){};
	void	setItemImage(const QString& image);
	QPixmap	itemImage();

protected:
	void	paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );

private:
	QPixmap		m_processStripeImage;
};

DuiProgressBarStripeItem::DuiProgressBarStripeItem( QGraphicsItem* parent /*= NULL */ ):
DuiWidget(parent)
{

}

void DuiProgressBarStripeItem::setItemImage( const QString& image )
{
	m_processStripeImage = DuiTheme::getImage(image);
}

void DuiProgressBarStripeItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_UNUSED( widget );
	Q_UNUSED( option );
	if ( !m_processStripeImage.isNull() && width() != 0 )
	{
		painter->drawPixmap(QPointF(0.0, 0.0), m_processStripeImage, rect());	
	}
}

QPixmap DuiProgressBarStripeItem::itemImage()
{
	return m_processStripeImage;
}

class DuiProgressBarPrivate : public DuiWidgetPrivate
{
public:
	Q_DECLARE_PUBLIC(DuiProgressBar);

	DuiProgressBarPrivate(void);
	~DuiProgressBarPrivate(void);

	void	setSmoothAnimation( bool isSmooth, int perDuration );

private:
	DuiFrameItem					*m_backgroundImage;	// 背景
	DuiFrameItem					*m_processItem;
	DuiProgressBarStripeItem		*m_processStripeItem;
	QGraphicsSimpleTextItem			*m_progressText;

	int								m_value;			// 当前进度位置
	int								m_minimum;
	int								m_maximum;
	bool							m_isTextVisible;
	Qt::Orientation					m_orientation;

	QPropertyAnimation				*m_smoothAnimation;

	void update();
};

DuiProgressBarPrivate::DuiProgressBarPrivate( void )
: m_backgroundImage( NULL )
, m_processItem( NULL )
, m_progressText( NULL )
, m_value( 0 )
, m_minimum( 0 )
, m_maximum( 100 )
, m_isTextVisible( true ) 
, m_orientation(Qt::Horizontal)
, m_processStripeItem(NULL)
, m_smoothAnimation(NULL)
{

}

DuiProgressBarPrivate::~DuiProgressBarPrivate( void )
{

}

void DuiProgressBarPrivate::update()
{
	int width = 0;
	if ( ( m_maximum - m_minimum ) > 0 )
	{
		int Horizontal = ( m_value * m_backgroundImage->width() ) / ( m_maximum - m_minimum );
		int Vertical = ( m_value * m_backgroundImage->height() ) / ( m_maximum - m_minimum );
		width = m_orientation == Qt::Horizontal ? Horizontal : Vertical;
	}
	else
	{
		DW_ASSERT_X(false, __FUNCTION__, QString("can not divide zero.").toLatin1().constData());
	}
	
	QRectF processItemRect(0, 0, 0, 0);
	if (m_orientation == Qt::Horizontal)
	{
		processItemRect.setWidth(width);
		processItemRect.setHeight(m_backgroundImage->height());
	}
	else
	{
		processItemRect.setWidth(m_backgroundImage->width());
		processItemRect.setHeight(width);
	}
	
 	if ( m_smoothAnimation )
 	{
		QRectF rc;
		if ( m_processStripeItem->itemImage().isNull() )
		{
			rc = m_processItem->geometry();
		}
		else
		{
			rc = m_processStripeItem->geometry();
		}
 		m_smoothAnimation->stop();
 		m_smoothAnimation->setStartValue(rc);
 		m_smoothAnimation->setEndValue(processItemRect);
 		m_smoothAnimation->start();
 	}
 	else
 	{
 		m_processItem->setGeometry( processItemRect );
 		m_processStripeItem->setGeometry( processItemRect );
	}
	
	if (m_isTextVisible)
	{
		QString progressText = QString::number( m_value * 100 / ( m_maximum - m_minimum ) );
		progressText += "%";
		m_progressText->setText(progressText);
		QFontMetrics qfmText(m_progressText->font()); 

		qreal textXPos = 0, textYPos = 0;
		if (m_orientation == Qt::Horizontal)
		{
			textXPos = width;
			if ( textXPos + m_progressText->boundingRect().width() > m_backgroundImage->width())
			{
				textXPos = m_backgroundImage->width() - m_progressText->boundingRect().width();
			}

			textYPos = m_backgroundImage->height() - qfmText.height();
			textYPos /= 2;
		}
		else
		{
			textXPos = m_backgroundImage->width() - qfmText.width(progressText);
			textXPos /= 2;

			textYPos = width;
			if ( textYPos + m_progressText->boundingRect().height() > m_backgroundImage->height())
			{
				textYPos = m_backgroundImage->height() - m_progressText->boundingRect().height();
			}
		}

		m_progressText->setPos(textXPos, textYPos);
		m_progressText->show();
	}
	else
	{
		m_progressText->hide();
	}
}

void DuiProgressBarPrivate::setSmoothAnimation( bool isSmooth, int perDuration )
{
	if ( !isSmooth )
	{
		if ( m_smoothAnimation )
		{
			m_smoothAnimation->stop();
			delete m_smoothAnimation;
			m_smoothAnimation = NULL;
		}
		return;
	}

	if ( !m_smoothAnimation )
	{
		QObject *tarObj = m_processItem;
		if ( !m_processStripeItem->itemImage().isNull() )
		{
			tarObj = m_processStripeItem;
		}
		m_smoothAnimation = new QPropertyAnimation(tarObj, "geometry", q_func());
		m_smoothAnimation->setDuration(perDuration);
	}
}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiProgressBar)

DuiProgressBar::DuiProgressBar(QGraphicsItem* parent) : 
DuiWidget(*(new DuiProgressBarPrivate()), parent )
{
	//setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	init();
}

void DuiProgressBar::setValue(int value)
{
	Q_D(DuiProgressBar);

	if(value > d->m_maximum || value < d->m_minimum)
	{
		return;
	}
	d->m_value = value;

	d->update();

	emit valueChanged( value );
}

void DuiProgressBar::init()
{
	Q_D(DuiProgressBar);
	d->m_backgroundImage = new DuiFrameItem(":/theme/duifw/progressbar_background_slider.png", DuiFrameItemDrawer::NinePieces, this);	
	d->m_backgroundImage->setThemeColorType(DuiTheme::MoreDarkColor);

	d->m_processItem = new DuiFrameItem( ":/theme/duifw/progressbar_selected_part_slider.png", DuiFrameItemDrawer::NinePieces, this);
	d->m_processItem->setThemeColorType(DuiTheme::DarkColor);

	d->m_processStripeItem = new DuiProgressBarStripeItem(this);

	d->m_progressText = new QGraphicsSimpleTextItem(this);

	d->m_backgroundImage->setGeometry( 0, 0, 210, 9 );
	d->m_processItem->setGeometry( d->m_backgroundImage->rect().adjusted(1, 1, -1, -1) );
}

void DuiProgressBar::setProgressImage(const QString& backgroundImage, const QString& progressItemImage)
{
	Q_D(DuiProgressBar);
	
	d->m_backgroundImage->setImage(backgroundImage);	
	d->m_processItem->setImage(progressItemImage);
}

void DuiProgressBar::setProgressImage( const QString& imageList )
{
	QStringList strValues = imageList.split("|");
	if (strValues.size() < 2)
	{
		return;
	}

	setProgressImage( strValues.at(0), strValues.at(1) );
}


void DuiProgressBar::setProgressStripeImage( const QString& imageList )
{
	Q_D(DuiProgressBar);
	
	QStringList strValues = imageList.split("|");
	if (strValues.size() < 2)
	{
		DW_ASSERT(false && "Must set empty by ||");
		return;
	}

	d->m_backgroundImage->setImage(strValues[0]);	
	d->m_processStripeItem->setItemImage(strValues[1]);
}

void DuiProgressBar::setRange( int minimum, int maximum )
{
	if ( maximum <= minimum || maximum == 0 )
	{
		DW_ASSERT(false);
		return;
	}

	d_func()->m_minimum = minimum;
	d_func()->m_maximum = maximum;
	d_func()->update();
}

void DuiProgressBar::reset()
{
	d_func()->m_value = d_func()->m_minimum;
}

void DuiProgressBar::setMaximum( int maximum )
{
	d_func()->m_maximum = maximum;
}

void DuiProgressBar::setMinimum( int minimum )
{
	d_func()->m_minimum = minimum;
}

int DuiProgressBar::value() const
{
	return d_func()->m_value;
}

int DuiProgressBar::maximum() const
{
	return d_func()->m_maximum;
}

int DuiProgressBar::minimum() const
{
	return d_func()->m_minimum;
}

bool DuiProgressBar::isTextVisible() const
{
	return d_func()->m_isTextVisible;
}

void DuiProgressBar::setTextVisible( bool visible )
{
	d_func()->m_isTextVisible = visible;
	d_func()->update();
}

QString DuiProgressBar::progressImage() const
{
	return QString();
}

Qt::Orientation	DuiProgressBar::orientation() const
{
	return d_func()->m_orientation;
}

void DuiProgressBar::setOrientation(Qt::Orientation orientation)
{
	d_func()->m_orientation = orientation;
	d_func()->update();
}


void DuiProgressBar::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D( DuiProgressBar );

	QRectF newSize(0, 0, event->newSize().width(), event->newSize().height());
	d->m_backgroundImage->setGeometry(newSize);
	d->update();
}

void DuiProgressBar::disableThemeColor()
{
	Q_D(DuiProgressBar);
	d->m_processItem->setColorizePolicy(UnColorableFlag);
}

void DuiProgressBar::setBackgroundColorizePolicy( ColorizeWidgetFlag flag )
{
	Q_D(DuiProgressBar);
	d->m_backgroundImage->setColorizePolicy(flag);
}

DuiWidget::ColorizeWidgetFlag DuiProgressBar::backgroundColorizePolicy() const
 {
 	return d_func()->m_backgroundImage->colorizePolicy();
 }

void DuiProgressBar::setSmoothAnimation( bool isSmooth, int perDuration )
{
	d_func()->setSmoothAnimation(isSmooth, perDuration);
}
