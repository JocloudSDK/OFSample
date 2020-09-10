#pragma once

#include "duitexteditgraphicspixmapitem.h"
#include "duitexteditgraphicsitem_p.h"

class DuiTextEditGraphicsPixmapItemPrivate : public DuiTextEditGraphicsItemPrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsPixmapItem )

public:
	DuiTextEditGraphicsPixmapItemPrivate();
	virtual ~DuiTextEditGraphicsPixmapItemPrivate();

    void setTextSize();

	enum ImageStatus
	{
		IS_NormalStatus = 0,
		IS_HoverStatus,
		IS_PressStatus
	};

	enum ImageShowMode
	{
		ISM_ImagePath = 0,
		ISM_Pixmap,
	};

	DuiFrameItemDrawer::FrameType	m_frameType;
	QString		m_normalPath;
	QString		m_hoverPath;
	QString		m_pressPath;
	QString		m_currentImagePath;
	
	QPixmap		m_pixmapList[3];
	ImageStatus m_imageStatus;

	ImageShowMode m_imageShowMode;
	

    QString     m_text;
    QFont	    m_font;
    QPen	    m_pen;
    QSizeF      m_textSize;
};