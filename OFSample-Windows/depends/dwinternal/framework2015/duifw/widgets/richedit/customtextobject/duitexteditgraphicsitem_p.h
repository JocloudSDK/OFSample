#pragma once

#include "duitexteditgraphicsitem.h"
#include "animation/duitexteditgraphicsanimation.h"

class DuiTextEditGraphicsItemPrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsItem );

public:
	DuiTextEditGraphicsItemPrivate();
	virtual ~DuiTextEditGraphicsItemPrivate();

	enum HoverStatus
	{
		Hover		= 0,
		Leave		= Hover + 1
	};

	void init();
	void setPressStatus(bool isPressed);
	bool pressStatus() const;

protected:
	DuiTextEditGraphicsItem *q_ptr;

	QPointF m_pos;
	QSizeF	m_itemSize;
	QString m_objectName;
	HoverStatus m_hoverStatus;
	QPointer<QWidget> m_containerWidget;
    Qt::CursorShape m_hoverCursorShape;
    Qt::CursorShape m_hoverOldCursor;
	bool	m_enableHoverEvent;
	bool	m_isVisible;
	bool	m_isPressed;
	int		m_textObjectIndex;
	quint32 m_acceptedMouseButtons;
	QHash<QString, QVariant > m_propertyData;
	QPointer<DuiTextEditGraphicsAnimation>	m_animation;
};
