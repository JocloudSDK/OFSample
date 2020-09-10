#include "stable.h"


#include "yynotificationboxitem.h"

YYNotificationBoxItem::YYNotificationBoxItem( QObject *parent /*= NULL*/ )
: QObject(parent)
, m_type(Unknown)
{

}

YYNotificationBoxItem::~YYNotificationBoxItem()
{

}

void YYNotificationBoxItem::update()
{
	emit dataChanged();
}

void YYNotificationBoxItem::setType(NotificaitonItemType type)
{
	m_type = type;
}

YYNotificationBoxItem::NotificaitonItemType YYNotificationBoxItem::getType(void) const
{
	return m_type;
}
