#include "stable.h"

#include <float.h>
#include "duispaceritem.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpacerItem)
DuiSpacerItem::DuiSpacerItem(QGraphicsItem *parent)
: DuiWidget(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents);
    setAcceptedMouseButtons(Qt::NoButton);
    setAcceptHoverEvents(false);
    setPreferredSize(DBL_MAX, DBL_MAX);
}

DuiSpacerItem::~DuiSpacerItem(void)
{
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpacerItemEx)
DuiSpacerItemEx::DuiSpacerItemEx(QGraphicsItem *parent)
: DuiSpacerItem(parent)
{
    // set to the right layout policy for spacer item
    setPreferredSize(0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}