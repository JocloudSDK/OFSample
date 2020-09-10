#pragma once
#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/yycomponent.h"

class DuiWidget;
DWDEFINE_INTERFACE(IChannelChatQuickReply) : public IUnk 
{
	/**
	* @brief 创建自动回复窗口.
	* @return 失败或成功.
	*/
	virtual bool create(DuiWidget* parent) = 0;

	/**
   * @brief 显示窗口.
   * @param relateWidget，自动回复窗口会根据这个参数指针的窗口自动调整在合适的位置上显示出来.
   */
	virtual void show( DuiWidget* relateWidget ) = 0;

	/**
   * @brief 按Enter发送消息选中信号.
    * @param 参数1，是否选中.
   */
	typedef _def_boost_signals2_signal_type<void (bool)>::type _SIG_ENTER_SEND_ACTION;
	virtual _SIG_ENTER_SEND_ACTION* getEnterSendActionSignal() = 0;

	/**
   * @brief 按Ctrl+Enter发送消息选中信号.
   * @param 参数1，是否选中.
   */
	typedef _def_boost_signals2_signal_type<void (bool)>::type _SIG_CTR_ENTER_SEND_ACTION;
	virtual _SIG_CTR_ENTER_SEND_ACTION* getCtrlEnterSendActionSignal() = 0;

	/**
   * @brief 发送消息选中信号.
   * @param 参数1，选中要发送的文本.
   */
	typedef _def_boost_signals2_signal_type<void (const QString&)>::type _SIG_CHAT_QUICK_REPLY;
	virtual _SIG_CHAT_QUICK_REPLY* getChatQuickReplySignal() = 0;
};