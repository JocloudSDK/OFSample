#pragma once

#include "dwbase/dwcomex.h"
#include "yychannelmessage_i.h"
#include "yycomponent/yychannel/yyproperty.h"

class DWXmlElement;

//!! 注意，接口在内测，暂时先不要用。

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
	* @brief 组件以xml格式给聊天信息添加自定义信息，请使用YYChannelMessageExtraInfoHelper添加
	* @return widget.
	*/
    virtual SIG_MESSAGE_ADDITION* sigMessageAboutToAddInfo() = 0;

	typedef _def_boost_signals2_signal_type<void(const QString&)>::type SIG_EXTRAINFO_TO_COMPONENT;
	/**
	* @brief 组件根据信息进行弹tips等操作
	* @return .
	*/
	virtual SIG_EXTRAINFO_TO_COMPONENT* sigExtraInfoToComponent() = 0;

	virtual void handleLink(const QString &linkKey, const QString &linkValue) = 0;

	virtual void xmlToCompositeMsg(const std::wstring &xml, QList<YYMessageItem>& compositeMsg) = 0;

	DWDECLARE_SIGNAL(SigUserClicked, void(quint32 uid));
	DWDECLARE_SIGNAL(SigRemoveLink, void(const QString &linkKey, const QString &linkValue, const QString &linkName));

	/**
	* @brief 获取频道公屏缓存的通知消息数据.
	* @param QList<YYMessageData>
	*/
	virtual QList<YYMessageData> getCacheEchoMessage(void) = 0;

	typedef _def_boost_signals2_signal_type<void(DWXmlElement*,const QString &)>::type	SIG_MESSAGE_ADDITION_EX;
    /**
	* @brief sigMessageAboutToAddInfo接口的扩展接口,把聊天内容也通知给组件.
	* @return widget.
	*/
    virtual SIG_MESSAGE_ADDITION_EX* sigMessageAboutToAddInfoEx() = 0;

    /**
    * @brief 从err_code转成字符串
    * @param err_code 错误码，参照yychannelmessage_i
    */
    virtual QString formatSendError(const channelmsg::SendErrorCode err_code) = 0;

    /**
    * @brief 从err_code转成字符串
    * @param err_code 错误码，参照yychannelmessage_i
    * @param argument 参数，会按需要格式化到字符串中。
    */
    virtual QString formatSendErrorArg1(const channelmsg::SendErrorCode err_code, const QString& argument) = 0;


	/*
	@brief 发送接入秩序审核的频道广播
	*/
	virtual void sendBroadCastMessageWithReview(const QString &txt, const TEXT_FONT_INFO& fontInfo,UINT32 opSid, UINT32 recieverMask, std::deque<UINT32> sids)=0;

	typedef _def_boost_signals2_signal_type<void(UINT32 respCode, bool bReviewFailed)>::type	 SIG_BROADCAST_REVIEW_RESULT;
	virtual SIG_BROADCAST_REVIEW_RESULT* sigChannelBroadcastReviewResult() = 0;

	/*
	@brief 订阅后收到的其他频道公屏转发的用户消息（公屏转发消息，8.40）
	*/
	virtual SIG_MESSAGE_ARRIVED* sigTransferMessageArrived() = 0;

	/*
	@brief 业务是否接管sigSendResult中错误码在客户端的提示，若是，pcyy这边不会另外提示，若否，由业务处理提示ui
	默认值为false
	*/
	virtual void setIsBusinessTakeOverErrPrompt(bool v) = 0; 
	virtual bool getIsBusinessTakeOverErrPrompt() = 0;
};

DWDEFINE_SIGNAL(IYYChannelMessageLogic::SigUserClicked);
DWDEFINE_SIGNAL(IYYChannelMessageLogic::SigRemoveLink);
