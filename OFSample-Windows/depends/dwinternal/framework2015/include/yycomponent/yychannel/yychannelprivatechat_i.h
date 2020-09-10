#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"

DWDEFINE_INTERFACE(IChannelPrivateChat) : public IUnk
{
/**
   * @brief .invoke
   * @param .method��Ҫ���õĺ�����inParam�����������outParam���������
   * @return .���÷����룬��InvokeCode
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
   * @brief .������
   * @param .˽��ָ��
   * @return .��
   */
		explicit Invoker( IChannelPrivateChatPtr invokePtr ):m_invokePtr(invokePtr)
		{

		}

/**
   * @brief .��ʼ˽��
   * @param .uid��˽�Ķ���uid
   * @return .���÷����룬��InvokeCode
   */
		int startPrivateChat( UINT32 uid )
		{
			PL_1<UINT32> inParam; inParam.size = sizeof( inParam);
			inParam.p1 = uid;

			return m_invokePtr->invoke(KStartPrivateChat, &inParam, NULL);
		}

/**
   * @brief .�ر�˽��
   * @param .��
   * @return .���÷����룬��InvokeCode
   */
		int closePrivateChat()
		{
			return m_invokePtr->invoke(KClosePrivateChat, NULL, NULL );
		}
/**
	* @brief .��ȡ��ʼ˽���ź�
	* @param .��
	* @return .˽���ź�
	*/
		_SIG_START_PRIVATE_CHAT*	sigStartPrivateChat()
		{
			PL_1< _SIG_START_PRIVATE_CHAT* > outParam; outParam.size = sizeof(outParam);
			outParam.p1 = NULL;

			m_invokePtr->invoke(KStartChatSignal, NULL, &outParam);

			return outParam.p1;
		}
/**
	* @brief .��ȡ�ر�˽���ź�
	* @param .��
	* @return .�ر�˽���ź�
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