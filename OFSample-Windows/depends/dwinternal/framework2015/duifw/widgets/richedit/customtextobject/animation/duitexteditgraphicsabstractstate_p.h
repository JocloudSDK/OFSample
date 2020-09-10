#pragma once

#include "duitexteditgraphicsabstractstate.h"

class DuiTextEditGraphicsAbstractStatePrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsAbstractState );

public:
	DuiTextEditGraphicsAbstractStatePrivate();
	virtual ~DuiTextEditGraphicsAbstractStatePrivate();

protected:
	DuiTextEditGraphicsAbstractState *q_ptr;

	DuiTextEditGraphicsAbstractState::StateStatus		m_status;
};
