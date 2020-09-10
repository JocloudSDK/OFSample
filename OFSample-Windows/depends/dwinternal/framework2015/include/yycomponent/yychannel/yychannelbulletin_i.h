#pragma once
#include "dwbase/dwcomex.h"
/**
* @brief .公告编辑窗接口
*/
DWDEFINE_INTERFACE(IYYChannelBulletinWidget) : public IUnk 
{
/**
   * @brief 打开公告编辑窗口.
   */
	virtual void show() = 0;
/**
   * @brief 隐藏公告编辑窗口.
   */
	virtual void hide() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_BULLETIN_CHANGED;

	/**
	* @brief 公告内容改变信号,通过ChannelData::strBulletin知道可获取公告内容.
	* @return 公告内容改变信号.
	*/
	virtual _SIG_BULLETIN_CHANGED* getSigBulletinChanged() = 0;

	virtual QString getBulletin() = 0;
};