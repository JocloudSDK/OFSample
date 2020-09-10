#pragma once
#include "dwbase/dwcomex.h"
#include <dwbase/dwcominterface_i.h>

/**
  * @brief Ƶ���ʻ��ӿڣ������յ�IYYCustomDataProviderNotify::serviceLoaded֮��comRoot->queryInterface<IYYChannelEntGiftFlw>(KSERVICEEntGiftFlw)ʹ�ã��յ�IYYCustomDataProviderNotify::serviceAboutToUnloadʱ�����ͷ�
*/
const char KSERVICEChannelEntGiftFlw[] = "com.yy.entgiftflwservice";
DWDEFINE_INTERFACE(IYYChannelEntGiftFlw): public IDWQComponent
{
/**
   * @brief ���õ�ǰ������������ڽ��ܷɻ�Ʊ��������������ǧ��������ڶ���.
   * @param alias ��ǰ�������.
   */
	virtual void setAlias(const std::string &alias) = 0;
	virtual void queryFlowersInBasket() = 0;
	virtual void queryFlowerNumber(const QList<quint32> &uids, const QString &password) = 0;
	virtual void giftFlower(const quint32 uid) = 0;

//signals:
//	void flowersInBasketChanged(quint32 newCount);
//	void giftFlowerBroadcast(quint32 fromId, quint32 toId, quint32 count, bool isThemeFlower);
//	void flowerNumberChanged(const QList<QPair<quint32, quint32>> &data);
};
