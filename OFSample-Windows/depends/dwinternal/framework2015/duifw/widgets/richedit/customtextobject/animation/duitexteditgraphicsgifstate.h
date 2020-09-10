#pragma once

#include "dwglobal.h"
#include "duitexteditgraphicsabstractstate.h"

class DuiTextEditGraphicsGifStatePrivate;

class DW_DUIFW_EXPORT DuiTextEditGraphicsGifState : public DuiTextEditGraphicsAbstractState
{
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsGifState )
	Q_DISABLE_COPY( DuiTextEditGraphicsGifState )

public:
	DuiTextEditGraphicsGifState( );
	~DuiTextEditGraphicsGifState();

	QPixmap changedPixmap() const;
	void	setChangedPixmap( const QPixmap& pixmap );

protected:
	explicit DuiTextEditGraphicsGifState( DuiTextEditGraphicsGifStatePrivate &dd );

	virtual void stateStart( DuiTextEditGraphicsAnimation* animation );
	virtual void stateChanged( DuiTextEditGraphicsAnimation* animation );
	virtual void stateFinished( DuiTextEditGraphicsAnimation* animation );
};