#pragma once
#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/yycomponent.h"

class DuiWidget;
DWDEFINE_INTERFACE(IChannelChatQuickReply) : public IUnk 
{
	/**
	* @brief �����Զ��ظ�����.
	* @return ʧ�ܻ�ɹ�.
	*/
	virtual bool create(DuiWidget* parent) = 0;

	/**
   * @brief ��ʾ����.
   * @param relateWidget���Զ��ظ����ڻ�����������ָ��Ĵ����Զ������ں��ʵ�λ������ʾ����.
   */
	virtual void show( DuiWidget* relateWidget ) = 0;

	/**
   * @brief ��Enter������Ϣѡ���ź�.
    * @param ����1���Ƿ�ѡ��.
   */
	typedef _def_boost_signals2_signal_type<void (bool)>::type _SIG_ENTER_SEND_ACTION;
	virtual _SIG_ENTER_SEND_ACTION* getEnterSendActionSignal() = 0;

	/**
   * @brief ��Ctrl+Enter������Ϣѡ���ź�.
   * @param ����1���Ƿ�ѡ��.
   */
	typedef _def_boost_signals2_signal_type<void (bool)>::type _SIG_CTR_ENTER_SEND_ACTION;
	virtual _SIG_CTR_ENTER_SEND_ACTION* getCtrlEnterSendActionSignal() = 0;

	/**
   * @brief ������Ϣѡ���ź�.
   * @param ����1��ѡ��Ҫ���͵��ı�.
   */
	typedef _def_boost_signals2_signal_type<void (const QString&)>::type _SIG_CHAT_QUICK_REPLY;
	virtual _SIG_CHAT_QUICK_REPLY* getChatQuickReplySignal() = 0;
};