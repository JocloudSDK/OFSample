#pragma once

#include "dwbase/dwcomex.h"
#include "yychannelmessage_i.h"
#include "yycomponent/yychannel/yyproperty.h"

class DWXmlElement;

//!! ע�⣬�ӿ����ڲ⣬��ʱ�Ȳ�Ҫ�á�

DWDEFINE_INTERFACE(IYYChannelMessageLogic):public IUnk
{
	enum SwitchState 
	{ 
		Off = 0,
		On = 1,
	}; 

	typedef yyproperty::property<SwitchState> SwitchPro; 

	virtual YYMessageData parseMessage( const QString& message ) = 0;

	virtual bool sendMessage( const QString& message ) = 0;
	virtual bool sendPrivateMessage(quint32 uid, const QString& message ) = 0;

	typedef _def_boost_signals2_signal_type<void(const channelmsg::SendError&)>::type	 SIG_SEND_RESULT;
	virtual SIG_SEND_RESULT* sigSendResult();

	virtual channelmsg::SendError getLastSendError() = 0;

	virtual bool appendMessage( const YYMessageData& message ) = 0;
	virtual bool appendPrivateMessage( const YYMessageData& message ) = 0 ;

	typedef _def_boost_signals2_signal_type<void(const YYMessageData&)>::type	SIG_MESSAGE_ARRIVED;

	virtual SIG_MESSAGE_ARRIVED* sigMessageArrived() = 0;
	virtual SIG_MESSAGE_ARRIVED* sigPrivateMessageArrived() = 0;

	virtual SIG_MESSAGE_ARRIVED* sigMessageAppended() = 0;
	virtual SIG_MESSAGE_ARRIVED* sigPrivateMessageAppended() = 0;
	
	virtual SwitchPro& proImageable() = 0;
	virtual SwitchPro& proTextable() = 0;


	virtual void sendBroadCastMessage(const QString &txt, const TEXT_FONT_INFO& fontInfo , UINT32 channelId, UINT32 recieverMask, std::deque<UINT32> sids) = 0;

    typedef _def_boost_signals2_signal_type<void(DWXmlElement*)>::type	SIG_MESSAGE_ADDITION;
    /**
	* @brief �����xml��ʽ��������Ϣ����Զ�����Ϣ����ʹ��YYChannelMessageExtraInfoHelper���
	* @return widget.
	*/
    virtual SIG_MESSAGE_ADDITION* sigMessageAboutToAddInfo() = 0;

	typedef _def_boost_signals2_signal_type<void(const QString&)>::type SIG_EXTRAINFO_TO_COMPONENT;
	/**
	* @brief ���������Ϣ���е�tips�Ȳ���
	* @return .
	*/
	virtual SIG_EXTRAINFO_TO_COMPONENT* sigExtraInfoToComponent() = 0;

	virtual void handleLink(const QString &linkKey, const QString &linkValue) = 0;

	virtual void xmlToCompositeMsg(const std::wstring &xml, QList<YYMessageItem>& compositeMsg) = 0;

	DWDECLARE_SIGNAL(SigUserClicked, void(quint32 uid));
	DWDECLARE_SIGNAL(SigRemoveLink, void(const QString &linkKey, const QString &linkValue, const QString &linkName));

	/**
	* @brief ��ȡƵ�����������֪ͨ��Ϣ����.
	* @param QList<YYMessageData>
	*/
	virtual QList<YYMessageData> getCacheEchoMessage(void) = 0;

	typedef _def_boost_signals2_signal_type<void(DWXmlElement*,const QString &)>::type	SIG_MESSAGE_ADDITION_EX;
    /**
	* @brief sigMessageAboutToAddInfo�ӿڵ���չ�ӿ�,����������Ҳ֪ͨ�����.
	* @return widget.
	*/
    virtual SIG_MESSAGE_ADDITION_EX* sigMessageAboutToAddInfoEx() = 0;

    /**
    * @brief ��err_codeת���ַ���
    * @param err_code �����룬����yychannelmessage_i
    */
    virtual QString formatSendError(const channelmsg::SendErrorCode err_code) = 0;

    /**
    * @brief ��err_codeת���ַ���
    * @param err_code �����룬����yychannelmessage_i
    * @param argument �������ᰴ��Ҫ��ʽ�����ַ����С�
    */
    virtual QString formatSendErrorArg1(const channelmsg::SendErrorCode err_code, const QString& argument) = 0;


	/*
	@brief ���ͽ���������˵�Ƶ���㲥
	*/
	virtual void sendBroadCastMessageWithReview(const QString &txt, const TEXT_FONT_INFO& fontInfo,UINT32 opSid, UINT32 recieverMask, std::deque<UINT32> sids)=0;

	typedef _def_boost_signals2_signal_type<void(UINT32 respCode, bool bReviewFailed)>::type	 SIG_BROADCAST_REVIEW_RESULT;
	virtual SIG_BROADCAST_REVIEW_RESULT* sigChannelBroadcastReviewResult() = 0;

	/*
	@brief ���ĺ��յ�������Ƶ������ת�����û���Ϣ������ת����Ϣ��8.40��
	*/
	virtual SIG_MESSAGE_ARRIVED* sigTransferMessageArrived() = 0;

	/*
	@brief ҵ���Ƿ�ӹ�sigSendResult�д������ڿͻ��˵���ʾ�����ǣ�pcyy��߲���������ʾ��������ҵ������ʾui
	Ĭ��ֵΪfalse
	*/
	virtual void setIsBusinessTakeOverErrPrompt(bool v) = 0; 
	virtual bool getIsBusinessTakeOverErrPrompt() = 0;
};

DWDEFINE_SIGNAL(IYYChannelMessageLogic::SigUserClicked);
DWDEFINE_SIGNAL(IYYChannelMessageLogic::SigRemoveLink);
