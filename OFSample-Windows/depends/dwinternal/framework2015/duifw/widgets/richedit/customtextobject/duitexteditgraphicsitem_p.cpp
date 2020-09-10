#include "stable.h"

#include "duitexteditgraphicsitem_p.h"

DuiTextEditGraphicsItemPrivate::DuiTextEditGraphicsItemPrivate()
: q_ptr( NULL )
, m_pos( 0.0, 0.0 )
, m_itemSize(30.0, 30.0)
, m_hoverStatus( DuiTextEditGraphicsItemPrivate::Leave )
, m_containerWidget( NULL )
, m_enableHoverEvent( false )
, m_isVisible( true )
, m_isPressed( false )
, m_textObjectIndex( -1 )
, m_acceptedMouseButtons( Qt::NoButton )
, m_animation(NULL)
{
	
}

DuiTextEditGraphicsItemPrivate::~DuiTextEditGraphicsItemPrivate()
{
	if (m_animation)
	{
		m_animation->deleteLater();
		m_animation = NULL;
	}
}

void DuiTextEditGraphicsItemPrivate::init()
{
	static int i = 0;
	m_objectName = QString("TextEditGraphicsItem_%1").arg( i );
    m_hoverCursorShape = Qt::ArrowCursor;
	i++;
}

void DuiTextEditGraphicsItemPrivate::setPressStatus( bool isPressed )
{
	m_isPressed = isPressed;
}

bool DuiTextEditGraphicsItemPrivate::pressStatus() const
{
	return m_isPressed;
}
