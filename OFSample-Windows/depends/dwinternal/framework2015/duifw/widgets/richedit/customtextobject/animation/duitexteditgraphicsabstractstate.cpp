#include "stable.h"

#include "duitexteditgraphicsabstractstate_p.h"
#include "duitexteditgraphicsabstractstate.h"
#include "duitexteditgraphicsanimation.h"

DuiTextEditGraphicsAbstractStatePrivate::DuiTextEditGraphicsAbstractStatePrivate()
: q_ptr( NULL )
, m_status( DuiTextEditGraphicsAbstractState::enumStateNone )
{

}

DuiTextEditGraphicsAbstractStatePrivate::~DuiTextEditGraphicsAbstractStatePrivate()
{

}

//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsAbstractState::DuiTextEditGraphicsAbstractState( )
: d_ptr( new DuiTextEditGraphicsAbstractStatePrivate )
{
	d_ptr->q_ptr = this;
}

DuiTextEditGraphicsAbstractState::DuiTextEditGraphicsAbstractState( DuiTextEditGraphicsAbstractStatePrivate &dd )
: d_ptr( &dd )
{
	d_ptr->q_ptr = this;
}

DuiTextEditGraphicsAbstractState::~DuiTextEditGraphicsAbstractState()
{
	delete d_ptr;
	d_ptr = NULL;
}

void DuiTextEditGraphicsAbstractState::stateStart( DuiTextEditGraphicsAnimation* animation )
{
	Q_UNUSED(animation);
	d_func()->m_status = enumStateStart;
}

void DuiTextEditGraphicsAbstractState::stateChanged( DuiTextEditGraphicsAnimation* animation )
{
	Q_UNUSED(animation);
	d_func()->m_status = enumStateChanged;
}

void DuiTextEditGraphicsAbstractState::stateFinished( DuiTextEditGraphicsAnimation* animation )
{
	Q_UNUSED(animation);
	d_func()->m_status = enumStateFinished;
}

DuiTextEditGraphicsAbstractState::StateStatus DuiTextEditGraphicsAbstractState::status() const
{
	return d_func()->m_status;
}

QPixmap DuiTextEditGraphicsAbstractState::changedPixmap() const
{
	return QPixmap();
}

void DuiTextEditGraphicsAbstractState::setChangedPixmap( const QPixmap& pixmap )
{
	Q_UNUSED( pixmap );
	return;
}
