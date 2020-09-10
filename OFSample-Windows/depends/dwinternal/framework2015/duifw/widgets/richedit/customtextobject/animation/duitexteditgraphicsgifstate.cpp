#include "stable.h"

#include "duifw/duiitemdrawers.h"
#include "duifw/duitexteditgraphicsitem.h"
#include "duitexteditgraphicsabstractstate_p.h"
#include "duitexteditgraphicsgifstate.h"
#include "duitexteditgraphicsanimation.h"

class DuiTextEditGraphicsGifStatePrivate : DuiTextEditGraphicsAbstractStatePrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsGifState );

public:
	DuiTextEditGraphicsGifStatePrivate();
	virtual ~DuiTextEditGraphicsGifStatePrivate();

protected:
	QPixmap		m_pixmap;
};

DuiTextEditGraphicsGifStatePrivate::DuiTextEditGraphicsGifStatePrivate()
: DuiTextEditGraphicsAbstractStatePrivate()
{
}

DuiTextEditGraphicsGifStatePrivate::~DuiTextEditGraphicsGifStatePrivate()
{

}
//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsGifState::DuiTextEditGraphicsGifState( )
: DuiTextEditGraphicsAbstractState( *(new DuiTextEditGraphicsGifStatePrivate) )
{

}

DuiTextEditGraphicsGifState::DuiTextEditGraphicsGifState( DuiTextEditGraphicsGifStatePrivate &dd )
: DuiTextEditGraphicsAbstractState( dd )
{
}

DuiTextEditGraphicsGifState::~DuiTextEditGraphicsGifState()
{
	
}

QPixmap DuiTextEditGraphicsGifState::changedPixmap() const
{
	return d_func()->m_pixmap;
}


void DuiTextEditGraphicsGifState::stateChanged( DuiTextEditGraphicsAnimation* animation )
{
	DuiTextEditGraphicsAbstractState::stateChanged(animation);
}

void DuiTextEditGraphicsGifState::stateFinished( DuiTextEditGraphicsAnimation* animation )
{
	DuiTextEditGraphicsAbstractState::stateFinished(animation);
	stateChanged( animation );
}

void DuiTextEditGraphicsGifState::stateStart( DuiTextEditGraphicsAnimation* animation )
{
	DuiTextEditGraphicsAbstractState::stateStart(animation);
	stateChanged( animation );
}

void DuiTextEditGraphicsGifState::setChangedPixmap( const QPixmap& pixmap )
{
	d_func()->m_pixmap = pixmap;
}
