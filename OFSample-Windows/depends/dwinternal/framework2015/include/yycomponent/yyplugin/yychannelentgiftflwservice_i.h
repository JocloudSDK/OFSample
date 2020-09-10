#pragma once
#include "dwbase/dwcomex.h"
#include <dwbase/dwcominterface_i.h>

/**
  * @brief 频道鲜花接口，请在收到IYYCustomDataProviderNotify::serviceLoaded之后，comRoot->queryInterface<IYYChannelEntGiftFlw>(KSERVICEEntGiftFlw)使用，收到IYYCustomDataProviderNotify::serviceAboutToUnload时必须释放
*/
const char KSERVICEChannelEntGiftFlw[] = "com.yy.entgiftflwservice";
DWDEFINE_INTERFACE(IYYChannelEntGiftFlw): public IDWQComponent
{
/**
   * @brief 设置当前组件别名，用于接受飞机票。请容许我心中千万草泥马奔腾而过.
   * @param alias 当前组件别名.
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
