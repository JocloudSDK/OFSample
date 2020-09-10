#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/yyspeaksetting_i.h"
#include "biz/biz_types.h"

class QString;
DWDEFINE_INTERFACE(IYYCommandTalk): public IUnk
{
	virtual void invite(const std::vector<UINT32>& invites) = 0;
	virtual void dismissCommandCommunication(const std::vector<UINT32>& members) = 0;
	virtual void closeTalk() = 0;
	virtual bool isInCommandTalk() const = 0;
	virtual void channelSpeakKeyPress( quint32 keyCode ) = 0;
	virtual void changeSpeakStyle() = 0;
	virtual bool getSoftMute( AUDIODEV_TYPE devType ) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32, LPCWSTR)>::type	_SIG__ALLOCATE_CHANNEL;
	virtual _SIG__ALLOCATE_CHANNEL *getSigAllocateChannel() = 0;

	//�յ�ָ��ͨ������
	typedef _def_boost_signals2_signal_type<void (UINT32, UINT32)>::type _SIG__BE_INVITED;
	virtual _SIG__BE_INVITED *getSigBeInvited() = 0;

	//ָ��ͨ����ɢ
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG__DISMISS;
	virtual _SIG__DISMISS *getSigDismiss() = 0;

	typedef _def_boost_signals2_signal_type<void ( UINT32 )>::type	_SIG__INANOTHERTALK;
	virtual _SIG__INANOTHERTALK *getSigInAnotherTalk() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_JOINED;
	virtual _SIG_JOINED *getSigJoined() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_LEAVED;
	virtual _SIG_LEAVED *getSigLeaved() = 0;
	typedef _def_boost_signals2_signal_type<void (CHANNEL_ERROR_CODE, IPropBagPtrCR spErrInfo)>::type	_SIG_ERROR;
	virtual _SIG_ERROR *getSigError() = 0;
	typedef _def_boost_signals2_signal_type<void (DATA_NOTIFY_TYPE dataNotifyType, Data::IRowArrayPtr spNewRowArray, Data::IRowArrayPtr spOldRowArray)>::type	_SIG_DATA_NOTIFY;
	virtual _SIG_DATA_NOTIFY *getUserTableNotifySignal() = 0;

	typedef _def_boost_signals2_signal_type<void (const QString&)>::type _SIG_ECHOTIPS;
	virtual _SIG_ECHOTIPS *getEchoTipsSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (SpeakSettingConst::SpeakType, quint32)>::type _SIG_SPEAKSETTINGCHANGED;
	virtual _SIG_SPEAKSETTINGCHANGED *getSigSpeakKeySettingChanged() = 0;
	
	virtual bool isUserTalking(UINT32 uid) = 0;
	virtual QList<quint32> getTalkingUsers() = 0;
	typedef _def_boost_signals2_signal_type<void (const QList<quint32>&)>::type _SIG_SPEAKING_USER_CHANGED;
	virtual _SIG_SPEAKING_USER_CHANGED *getSigSpeakingUsersChanged() = 0;
	virtual  void setCmdChannelTag(const QString& tag) = 0;

    /**
    * @brief .��ȡ��Ƶ���﷢�����������û��б���������˵�����û���
    * @param .��
    * @return .�û�uid�б�
    */	
    virtual const QList<quint32>& getSpeakedUsers() const = 0;

    /**
	* @brief .��ȡƵ���������������û��б�ı���ź�
	* @param .��
	* @return .�б�ı��ź�
	*/
	typedef _def_boost_signals2_signal_type<void (const QList<quint32>&)>::type _SIG_SPEAKED_USERS_CHANGED;
	virtual _SIG_SPEAKED_USERS_CHANGED* getSpeakedUsersSignal() = 0;

	/**
	* @brief.  ��ȡָ��ͨ��ֱ���뿪YY����ֻ���뿪Ƶ�������ź�                                                                 
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 uid)>::type	_SIG_LEAVEDYY;
	virtual _SIG_LEAVEDYY *getSignalLeavedYY() = 0;

	/*
	*@ brief. �������û���������ָ��ͨ����
	*@ param ��������uid
	*@ param  ������������ָ��ͨ��ID
	*/
	typedef _def_boost_signals2_signal_type<void ( UINT32, UINT32 )>::type	_SIG__INANOTHERTALK2;
	virtual _SIG__INANOTHERTALK2 *getSigInAnotherTalk2() = 0;
};