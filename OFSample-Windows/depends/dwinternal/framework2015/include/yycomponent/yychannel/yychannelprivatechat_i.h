#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"

DWDEFINE_INTERFACE(IChannelPrivateChat) : public IUnk
{
/**
   * @brief .invoke
   * @param .method，要调用的函数；inParam，输入参数；outParam，输出参数
   * @return .调用返回码，见InvokeCode
   */
	virtual int invoke( const QString& method, InvokeParam* inParam, InvokeParam* outParam ) = 0;
};

namespace ApiChannelPrivateChat
{
	typedef _def_boost_signals2_signal_type<void (UINT32 uid)>::type	_SIG_START_PRIVATE_CHAT;
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_CLOSE_PRIVATE_CHAT;

	const char KStartPrivateChat[]	= "startPrivateChat";
	const char KClosePrivateChat[]	= "closePrivateChat";
	const char KStartChatSignal[]	= "startChatSig";
	const char KCloseChatSignal[]	= "closeChatSig";

	class Invoker
	{
	public:
/**
   * @brief .辅助类
   * @param .私聊指针
   * @return .无
   */
		explicit Invoker( IChannelPrivateChatPtr invokePtr ):m_invokePtr(invokePtr)
		{

		}

/**
   * @brief .开始私聊
   * @param .uid，私聊对象uid
   * @return .调用返回码，见InvokeCode
   */
		int startPrivateChat( UINT32 uid )
		{
			PL_1<UINT32> inParam; inParam.size = sizeof( inParam);
			inParam.p1 = uid;

			return m_invokePtr->invoke(KStartPrivateChat, &inParam, NULL);
		}

/**
   * @brief .关闭私聊
   * @param .无
   * @return .调用返回码，见InvokeCode
   */
		int closePrivateChat()
		{
			return m_invokePtr->invoke(KClosePrivateChat, NULL, NULL );
		}
/**
	* @brief .获取开始私聊信号
	* @param .无
	* @return .私聊信号
	*/
		_SIG_START_PRIVATE_CHAT*	sigStartPrivateChat()
		{
			PL_1< _SIG_START_PRIVATE_CHAT* > outParam; outParam.size = sizeof(outParam);
			outParam.p1 = NULL;

			m_invokePtr->invoke(KStartChatSignal, NULL, &outParam);

			return outParam.p1;
		}
/**
	* @brief .获取关闭私聊信号
	* @param .无
	* @return .关闭私聊信号
	*/
		_SIG_CLOSE_PRIVATE_CHAT*	sigClosePrivateChat()
		{
			PL_1< _SIG_CLOSE_PRIVATE_CHAT* > outParam; outParam.size = sizeof(outParam);
			outParam.p1 = NULL;

			m_invokePtr->invoke(KCloseChatSignal, NULL, &outParam);

			return outParam.p1;
		}

	private:
		IChannelPrivateChatPtr		m_invokePtr;
	};
}