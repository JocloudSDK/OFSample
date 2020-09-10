#pragma once

#include "dwbase/dwcomex.h"

/**
* @brief �ʻ��ӿ�.
*/
DWDEFINE_INTERFACE(IYYChannelFlower) : public IDWQComponent
{
/**
   * @brief �����е��ʻ�����.
   * @return �����е��ʻ�����.
   */
	virtual quint32 flowersInBasket() = 0;
/**
   * @brief һ�������ʻ�����.
   * @return һ�������ʻ�������0Ϊȫ��.
   */
	virtual quint32 countOfBouquet() = 0;
/**
   * @brief �����ͻ���ָ���û�.
   * @param uid���û�uid.
   * @param count���ʻ�����.
   * @return 0Ϊ�ɹ���1ΪƵ���������ͻ���2Ϊ��������0���ʻ���3Ϊ�����е��ʻ���������
   */
	virtual quint32 giftFlowers(quint32 uid, quint32 count) = 0;
/**
   * @brief ����ָ���û����ʻ�����.
   * @param uid���û�uid.
   */
	virtual void getTotalFlower(quint32 uid) = 0;

signals:
	void flowersInBasketChanged(quint32 newCount);
	void giftFlowerBroadcast(quint32 fromId, quint32 toId, quint32 count, bool isThemeFlower);
	void totalFlowerChanged(quint32 uid, quint32 total);
};