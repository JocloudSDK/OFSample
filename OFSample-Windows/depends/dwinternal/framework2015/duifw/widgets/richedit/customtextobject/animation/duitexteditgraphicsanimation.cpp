#include "stable.h"

#include "duifw/duiresourceloader.h"

#include "duifw/duitexteditgraphicsitem.h"
#include "duitexteditgraphicsanimation.h"

class DuiTextEditGraphicsAnimationPrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsAnimation );

public:
	DuiTextEditGraphicsAnimationPrivate();
	virtual ~DuiTextEditGraphicsAnimationPrivate();

protected:
	DuiTextEditGraphicsAnimation *q_ptr;

	DuiTextEditGraphicsItem*			m_owerItem;
	DuiTextEditGraphicsAbstractState*	m_state;

	int			m_speed;			//动画的速度（帖/毫秒）
	int			m_playTime;			//播放时间
	int			m_frameIndex;		//当前的帧数
	QTimer*		m_playTimer;		
	QMovie*		m_movieShow;
	QBuffer		m_buf;
};

DuiTextEditGraphicsAnimationPrivate::DuiTextEditGraphicsAnimationPrivate()
: q_ptr( NULL )
, m_speed( 100 )
, m_playTime( 500 )
, m_frameIndex( 0 )
, m_playTimer(NULL)
, m_owerItem( NULL )
, m_state( NULL )
, m_movieShow( NULL )
{

}

DuiTextEditGraphicsAnimationPrivate::~DuiTextEditGraphicsAnimationPrivate()
{
	if (m_state)
	{
		delete m_state;
		m_state = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsAnimation::DuiTextEditGraphicsAnimation( QObject* parent )
: QObject(parent)
, d_ptr( new DuiTextEditGraphicsAnimationPrivate )
{
	d_ptr->q_ptr = this;
}

DuiTextEditGraphicsAnimation::DuiTextEditGraphicsAnimation( QObject* parent, DuiTextEditGraphicsAnimationPrivate &dd )
: QObject( parent )
, d_ptr( &dd )
{
	d_ptr->q_ptr = this;
}

DuiTextEditGraphicsAnimation::~DuiTextEditGraphicsAnimation()
{
	if (d_ptr)
	{
		delete d_ptr;
		d_ptr = NULL;
	}
}

void DuiTextEditGraphicsAnimation::start( int msec /*= -1*/ )
{
	Q_D(DuiTextEditGraphicsAnimation);

	if ( msec != -1 )
	{
		setSpeed( msec );
	}

	if ( d->m_playTimer == NULL )
	{
		d->m_playTimer = new QTimer( this );
		connect(d->m_playTimer, SIGNAL(timeout()), this, SLOT(on_playMovieTimeOut()));
	}
	
	stop();

	d->m_playTimer->start( speed() );
}

void DuiTextEditGraphicsAnimation::stop()
{
	Q_D(DuiTextEditGraphicsAnimation);

	if ( d->m_playTimer == NULL )
	{
		return;
	}
	
	if ( d->m_playTimer->isActive() )
	{
		d->m_playTimer->stop();
	}

	d->m_frameIndex = 0;
}

int DuiTextEditGraphicsAnimation::speed() const
{
	return d_func()->m_speed;
}

void DuiTextEditGraphicsAnimation::setSpeed( int msec )
{
	d_func()->m_speed = msec;
}

void DuiTextEditGraphicsAnimation::on_playMovieTimeOut()
{
	Q_D(DuiTextEditGraphicsAnimation);

	if (state() == NULL || ower() == NULL)
	{
		return;
	}

	//停止动画
	if ( (speed() * currentFrameIndex()) >= playTime() )
	{
		stop();
		state()->stateFinished( this );
		ower()->animationChanged( state() );
		return;
	}

	//state开始动画
	if ( 0 == d->m_frameIndex )
	{
		d->m_frameIndex++;
		state()->stateStart( this );
		ower()->animationChanged( state() );
	}
	else
	{
		d->m_frameIndex++;
		state()->stateChanged( this );
		ower()->animationChanged( state() );
	}
}

void DuiTextEditGraphicsAnimation::setOwer( DuiTextEditGraphicsItem* owerItem )
{
	d_func()->m_owerItem = owerItem;
}

DuiTextEditGraphicsItem* DuiTextEditGraphicsAnimation::ower() const
{
	return d_func()->m_owerItem;
}

int DuiTextEditGraphicsAnimation::currentFrameIndex() const
{
	return d_func()->m_frameIndex;
}

int DuiTextEditGraphicsAnimation::playTime() const
{
	return d_func()->m_playTime;
}

void DuiTextEditGraphicsAnimation::setState( DuiTextEditGraphicsAbstractState* state )
{
	Q_D(DuiTextEditGraphicsAnimation);
	if (d->m_state)
	{
		delete d->m_state;
		d->m_state = NULL;
	}
	d->m_state = state;
}

DuiTextEditGraphicsAbstractState* DuiTextEditGraphicsAnimation::state() const
{
	return d_func()->m_state;
}

void DuiTextEditGraphicsAnimation::startMoive( const QString& imagePath )
{
	Q_D(DuiTextEditGraphicsAnimation);
	if ( d->m_movieShow == NULL )
	{
		d->m_movieShow = new QMovie(this);
		connect(d->m_movieShow, SIGNAL(frameChanged(int)), this, SLOT(on_frameChanged(int)));
	}

	QByteArray			data;
	data = getResourceLoader()->loadRawData(imagePath);
	d->m_buf.setData( data );
	d->m_movieShow->setDevice( &d->m_buf );
	d->m_movieShow->start();

}

void DuiTextEditGraphicsAnimation::on_frameChanged( int frameNumber )
{
	Q_UNUSED(frameNumber);
	Q_D(DuiTextEditGraphicsAnimation);
	if ( d->m_state == NULL )
	{
		return;
	}
	
	d->m_state->setChangedPixmap( d->m_movieShow->currentPixmap() );
	state()->stateChanged( this );
	ower()->animationChanged( state() );
}

void DuiTextEditGraphicsAnimation::stopMoive()
{
	Q_D(DuiTextEditGraphicsAnimation);
	if (d->m_movieShow)
	{
		d->m_movieShow->stop();
	}
}
