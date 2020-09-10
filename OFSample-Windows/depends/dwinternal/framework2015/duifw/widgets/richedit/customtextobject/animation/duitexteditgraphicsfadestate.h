#pragma once

#include "dwglobal.h"
#include "duitexteditgraphicsabstractstate.h"

class DuiTextEditGraphicsFadeStatePrivate;

class DW_DUIFW_EXPORT DuiTextEditGraphicsFadeState : public DuiTextEditGraphicsAbstractState
{
	Q_DISABLE_COPY( DuiTextEditGraphicsFadeState )
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsFadeState )

public:
	DuiTextEditGraphicsFadeState( );
	~DuiTextEditGraphicsFadeState();

	QPixmap changedPixmap() const;

	void	setSourceImagePath( const QString& imagePath );
	QString sourceImagePath()const;

protected:
	explicit DuiTextEditGraphicsFadeState( DuiTextEditGraphicsFadeStatePrivate &dd );

	virtual void stateStart( DuiTextEditGraphicsAnimation* animation );
	virtual void stateChanged( DuiTextEditGraphicsAnimation* animation );
	virtual void stateFinished( DuiTextEditGraphicsAnimation* animation );
};