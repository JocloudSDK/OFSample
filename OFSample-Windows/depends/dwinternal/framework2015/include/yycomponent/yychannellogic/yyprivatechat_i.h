#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE( IYYPrivateChat): public IUnk
{
	enum StopReason
	{
		KVisitorLimit = 1,
		KChatUserBeenLeft = 2, //正在私聊时走了
		KWantChatUserBeenLeft = 3, // 想和它私聊时走了
		KApplyPrivateUserClosed = 4, // 发起私聊用户关闭私聊（普通模板中关闭）

		/* 
		    以下新增的错误码，可以连接信号getSigStopPrivateChat 从IPropBagPtr 获取错误提示内容
		    获取方式：propbagptr->get(L"formated_error_string") 
		*/
		KGuestPrivateChatTooFast = 5, //发起私聊太快提醒
		KMgrRejectGuestPrivateChat = 6,  //OW 设置频道管理员不接受游客频道私聊
		KMgrSelfRejectGuestPrivateChat = 7, //管理员自定义不接受游客私聊
	};

	virtual void startPrivateChatTo(quint32 uid) = 0;

	typedef xsignals::signal<void (IPropBagPtr)> SIG_START_PRIVATE_CHAT;
	virtual SIG_START_PRIVATE_CHAT* getSigStartPrivateChat() = 0;

	typedef xsignals::signal<void (IPropBagPtr, int )> SIG_STOP_PRIVATE_CHAT;
	virtual SIG_STOP_PRIVATE_CHAT* getSigStopPrivateChat() = 0;

	/*
	@brief 提供给业务模板（娱乐模板）关闭基础模板的私聊（bug id YYXMII-50515）
	*/
	virtual void privateChatClosedByBusiness() = 0;
};