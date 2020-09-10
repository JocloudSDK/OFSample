#include "stable.h"

#include "duitexteditgraphicspixmapitem_p.h"

DuiTextEditGraphicsPixmapItemPrivate::DuiTextEditGraphicsPixmapItemPrivate()
: DuiTextEditGraphicsItemPrivate()
, m_frameType( DuiFrameItemDrawer::OnePiece )
, m_imageStatus(IS_NormalStatus)
, m_imageShowMode(ISM_ImagePath)
{

}

DuiTextEditGraphicsPixmapItemPrivate::~DuiTextEditGraphicsPixmapItemPrivate()
{
}

void DuiTextEditGraphicsPixmapItemPrivate::setTextSize()
{
    QFontMetrics fm( m_font );
    m_textSize = fm.boundingRect( m_text ).size();
}