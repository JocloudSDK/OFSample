#include "stable.h"

#include "duifw/duiitemdrawers.h"
#include "duifw/duitexteditgraphicsitem.h"
#include "duitexteditgraphicsabstractstate_p.h"
#include "duitexteditgraphicsfadestate.h"
#include "duitexteditgraphicsanimation.h"

class DuiTextEditGraphicsFadeStatePrivate : DuiTextEditGraphicsAbstractStatePrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsFadeState );

public:
	DuiTextEditGraphicsFadeStatePrivate();
	virtual ~DuiTextEditGraphicsFadeStatePrivate();

	QPixmap getAnimationPixmap( const QSize& imageSize, qreal opacity = 1.0 );

protected:
	QString		m_sourceImagePath;
	QPixmap		m_pixmap;
};

DuiTextEditGraphicsFadeStatePrivate::DuiTextEditGraphicsFadeStatePrivate()
: DuiTextEditGraphicsAbstractStatePrivate()
{
}

DuiTextEditGraphicsFadeStatePrivate::~DuiTextEditGraphicsFadeStatePrivate()
{

}

QPixmap DuiTextEditGraphicsFadeStatePrivate::getAnimationPixmap( const QSize& imageSize, qreal opacity /*= 1.0 */ )
{
	QPixmap image( imageSize );
	image.fill( QColor(0, 0, 0, 0) );
	QPainter painter( &image );

	if ( opacity != 1.0 )
	{
		painter.setOpacity( opacity );
	}

	DuiFrameItemDrawer drawer( m_sourceImagePath, DuiFrameItemDrawer::ThreePiecesHorizontal );
	drawer.paint( &painter, QRectF(QPointF(0.0, 0.0), imageSize).toRect(), QMargins() );

	return image;
}

//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsFadeState::DuiTextEditGraphicsFadeState( )
: DuiTextEditGraphicsAbstractState( *(new DuiTextEditGraphicsFadeStatePrivate) )
{
}

DuiTextEditGraphicsFadeState::DuiTextEditGraphicsFadeState( DuiTextEditGraphicsFadeStatePrivate &dd )
: DuiTextEditGraphicsAbstractState( dd )
{
}

DuiTextEditGraphicsFadeState::~DuiTextEditGraphicsFadeState()
{
	
}

QPixmap DuiTextEditGraphicsFadeState::changedPixmap() const
{
	return d_func()->m_pixmap;
}


void DuiTextEditGraphicsFadeState::stateChanged( DuiTextEditGraphicsAnimation* animation )
{
	DuiTextEditGraphicsAbstractState::stateChanged(animation);

	if (animation == NULL || animation->ower() == NULL)
	{
		DW_ASSERT(false);
		return;
	}

	qreal opacity = 1.0 - ( animation->currentFrameIndex() * 0.2 );
	if ( opacity > 1.0 )
	{
		opacity = 1.0;
	}else if ( opacity < 0.0 )
	{
		opacity = 0.0;
	}

	d_func()->m_pixmap = d_func()->getAnimationPixmap( animation->ower()->size().toSize(), opacity );
}

void DuiTextEditGraphicsFadeState::stateFinished( DuiTextEditGraphicsAnimation* animation )
{
	DuiTextEditGraphicsAbstractState::stateFinished(animation);

	if (animation == NULL || animation->ower() == NULL)
	{
		DW_ASSERT(false);
		return;
	}

	d_func()->m_pixmap = d_func()->m_pixmap = d_func()->getAnimationPixmap( animation->ower()->size().toSize(), 1.0 );;
}

void DuiTextEditGraphicsFadeState::stateStart( DuiTextEditGraphicsAnimation* animation )
{
	DuiTextEditGraphicsAbstractState::stateStart(animation);
	stateChanged( animation );
}

void DuiTextEditGraphicsFadeState::setSourceImagePath( const QString& imagePath )
{
	d_func()->m_sourceImagePath = imagePath;
}

QString DuiTextEditGraphicsFadeState::sourceImagePath() const
{
	return d_func()->m_sourceImagePath;
}
