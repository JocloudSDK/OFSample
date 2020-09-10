#pragma once
#include "dwbase/dwcomex.h"
/**
* @brief .����༭���ӿ�
*/
DWDEFINE_INTERFACE(IYYChannelBulletinWidget) : public IUnk 
{
/**
   * @brief �򿪹���༭����.
   */
	virtual void show() = 0;
/**
   * @brief ���ع���༭����.
   */
	virtual void hide() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_BULLETIN_CHANGED;

	/**
	* @brief �������ݸı��ź�,ͨ��ChannelData::strBulletin֪���ɻ�ȡ��������.
	* @return �������ݸı��ź�.
	*/
	virtual _SIG_BULLETIN_CHANGED* getSigBulletinChanged() = 0;

	virtual QString getBulletin() = 0;
};