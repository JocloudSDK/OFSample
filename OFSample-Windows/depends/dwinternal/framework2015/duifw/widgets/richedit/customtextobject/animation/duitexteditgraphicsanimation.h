#pragma once

#include "dwglobal.h"
#include "duitexteditgraphicsabstractstate.h"

class DuiTextEditGraphicsAnimationPrivate;
class DuiTextEditGraphicsItem;

class DW_DUIFW_EXPORT DuiTextEditGraphicsAnimation : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiTextEditGraphicsAnimation )
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsAnimation )
	
public:
	explicit DuiTextEditGraphicsAnimation( QObject* parent = NULL );
	~DuiTextEditGraphicsAnimation();

	void	setOwer( DuiTextEditGraphicsItem* owerItem );
	DuiTextEditGraphicsItem* ower() const;

	int		speed () const;
	void	setSpeed ( int msec );

	int		playTime() const;
	void	setPlayTime( int time );

	void	start( int msec = -1 );
	void	stop();

	void	startMoive( const QString& imagePath );
	void	stopMoive();

	int		currentFrameIndex() const;

	void	setState( DuiTextEditGraphicsAbstractState* state );
	DuiTextEditGraphicsAbstractState* state() const;

protected slots:
	void on_playMovieTimeOut( );
	void on_frameChanged ( int frameNumber );

protected:
	DuiTextEditGraphicsAnimation( QObject* parent, DuiTextEditGraphicsAnimationPrivate &dd );

protected:
	DuiTextEditGraphicsAnimationPrivate * d_ptr;
};