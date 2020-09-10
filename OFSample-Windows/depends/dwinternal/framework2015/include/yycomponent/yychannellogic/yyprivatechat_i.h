#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE( IYYPrivateChat): public IUnk
{
	enum StopReason
	{
		KVisitorLimit = 1,
		KChatUserBeenLeft = 2, //����˽��ʱ����
		KWantChatUserBeenLeft = 3, // �����˽��ʱ����
		KApplyPrivateUserClosed = 4, // ����˽���û��ر�˽�ģ���ͨģ���йرգ�

		/* 
		    ���������Ĵ����룬���������ź�getSigStopPrivateChat ��IPropBagPtr ��ȡ������ʾ����
		    ��ȡ��ʽ��propbagptr->get(L"formated_error_string") 
		*/
		KGuestPrivateChatTooFast = 5, //����˽��̫������
		KMgrRejectGuestPrivateChat = 6,  //OW ����Ƶ������Ա�������ο�Ƶ��˽��
		KMgrSelfRejectGuestPrivateChat = 7, //����Ա�Զ��岻�����ο�˽��
	};

	virtual void startPrivateChatTo(quint32 uid) = 0;

	typedef xsignals::signal<void (IPropBagPtr)> SIG_START_PRIVATE_CHAT;
	virtual SIG_START_PRIVATE_CHAT* getSigStartPrivateChat() = 0;

	typedef xsignals::signal<void (IPropBagPtr, int )> SIG_STOP_PRIVATE_CHAT;
	virtual SIG_STOP_PRIVATE_CHAT* getSigStopPrivateChat() = 0;

	/*
	@brief �ṩ��ҵ��ģ�壨����ģ�壩�رջ���ģ���˽�ģ�bug id YYXMII-50515��
	*/
	virtual void privateChatClosedByBusiness() = 0;
};