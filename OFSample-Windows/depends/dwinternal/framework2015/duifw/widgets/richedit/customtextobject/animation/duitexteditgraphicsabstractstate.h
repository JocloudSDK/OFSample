#pragma once

#include "dwglobal.h"

class DuiTextEditGraphicsAbstractStatePrivate;
class DuiTextEditGraphicsAnimation;

class DW_DUIFW_EXPORT DuiTextEditGraphicsAbstractState
{
	Q_DISABLE_COPY( DuiTextEditGraphicsAbstractState )
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsAbstractState )
	
public:
	friend DuiTextEditGraphicsAnimation;

	enum StateStatus
	{
		enumStateNone		=	0,
		enumStateStart,
		enumStateChanged,
		enumStateFinished
	};

	DuiTextEditGraphicsAbstractState( );
	virtual ~DuiTextEditGraphicsAbstractState();

	StateStatus status() const;
	
	virtual QPixmap	changedPixmap() const;
	virtual void	setChangedPixmap( const QPixmap& pixmap );

protected:
	explicit DuiTextEditGraphicsAbstractState( DuiTextEditGraphicsAbstractStatePrivate &dd );

	//signal
	virtual void stateStart( DuiTextEditGraphicsAnimation* animation );
	virtual void stateChanged( DuiTextEditGraphicsAnimation* animation );
	virtual void stateFinished( DuiTextEditGraphicsAnimation* animation );

protected:
	DuiTextEditGraphicsAbstractStatePrivate * d_ptr;
};