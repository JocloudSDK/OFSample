#pragma once

#include "dwbase/dwcomex.h"

/**
* @brief 鲜花接口.
*/
DWDEFINE_INTERFACE(IYYChannelFlower) : public IDWQComponent
{
/**
   * @brief 花篮中的鲜花数量.
   * @return 花篮中的鲜花数量.
   */
	virtual quint32 flowersInBasket() = 0;
/**
   * @brief 一束花的鲜花数量.
   * @return 一束花的鲜花数量，0为全部.
   */
	virtual quint32 countOfBouquet() = 0;
/**
   * @brief 请求送花给指定用户.
   * @param uid，用户uid.
   * @param count，鲜花数量.
   * @return 0为成功，1为频道不允许送花，2为不允许送0朵鲜花，3为花篮中的鲜花数量不足
   */
	virtual quint32 giftFlowers(quint32 uid, quint32 count) = 0;
/**
   * @brief 请求指定用户的鲜花总数.
   * @param uid，用户uid.
   */
	virtual void getTotalFlower(quint32 uid) = 0;

signals:
	void flowersInBasketChanged(quint32 newCount);
	void giftFlowerBroadcast(quint32 fromId, quint32 toId, quint32 count, bool isThemeFlower);
	void totalFlowerChanged(quint32 uid, quint32 total);
};