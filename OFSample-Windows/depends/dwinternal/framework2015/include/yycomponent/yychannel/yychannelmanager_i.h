#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/**
* @brief Ƶ������.
*/
DWDEFINE_INTERFACE( IYYChannelManager ) : public IUnk
{
/**
   * @brief ���ýӴ�Ƶ��.
   * @param sid����Ƶ��id.
   * @param enable���Ӵ��Ƿ���Ч.
   */
	virtual void asyncSetReceive(UINT32 sid, bool enable ) = 0;

	/**
	* @brief ���ýӴ�Ƶ���ı��ź�.
	* @param sid����Ƶ��id.
	* @param enable���Ӵ��Ƿ���Ч.
	* @param uid��˭�ı�Ӵ�Ƶ��.
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 sid, bool)>::type	_SIG_ECHO_RECEPTION_ALTER;
	virtual _SIG_ECHO_RECEPTION_ALTER* sigEchoReception() = 0;

/**
   * @brief �ı�Ƶ���û������.
   * @param uid,���ı���û�uid.
   * @param role����Ҫ�ı��Ϊ�Ľ�ɫ.
   * @param sid��ĳ������Ƶ��id�������Ƶ������Ա�Ľ�ɫʱʹ��.
   */
	virtual void asyncChangeUserRole( UINT32 uid, ROLE_IN_CHANNEL role, UINT32 sid ) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32/* admin*/, UINT32/* who*/, ROLE_IN_CHANNEL/* oldRole*/, ROLE_IN_CHANNEL/* newRole*/, UINT32 /*specSid*/)>::type	_SIG_ECHO_ROLE_CHANGED;

	/**
	* @brief ��ȡƵ����ɫ�ı��ź�.
	* @return Ƶ����ɫ�ı��ź�.
	*/
	virtual _SIG_ECHO_ROLE_CHANGED* sigEchoRoleChanged() = 0;

/**
   * @brief �ж���û�����˵�Ȩ�� .
   * @param sid����Ҫ���������˵���Ƶ��id.
   * @param uid����Ҫ���ߵ��û�uid.
   * @return ��Ȩ�޷���true�����򷵻�false.
   */
	virtual bool	canKickOutUser(UINT32 sid, UINT32 uid) = 0;
/**
   * @brief ��һ���û��߳�Ƶ������Ƶ��.
   * @param sid����Ҫ���������˵���Ƶ��id.
   * @param uid����Ҫ���ߵ��û�uid.
   * @param minute���߳�����ʱ�䣬��Ԫ����.
   * @param reason���߳�����.
   * @param kickOff��true���߳�Ƶ����false���߳��û�������Ƶ����������.
  */
	virtual void asyncKickOutUser(UINT32 sid, UINT32 uid, UINT32 minute, LPCWSTR reason, bool kickOff) = 0;

/**
   * @brief �ı�Ƶ���û������.
   * @param uid,���ı���û�uid.
   * @param role����Ҫ�ı��Ϊ�Ľ�ɫ.
   */
	virtual void  makeUserAs(UINT32 uid, ROLE_IN_CHANNEL role) = 0;

	virtual void asyncChangeChannelInfo(UINT32 sid, IPropBagWrapperPtr spInfo) = 0;
	virtual void asyncChangeChannelStyle(UINT32 sid, CHANNEL_STYLE channelStyle) = 0;

	virtual void asyncCreateSubChannel(UINT32 id, IPropBagWrapperPtr data) = 0;
	virtual void asyncDestroySubChannel(UINT32 id) = 0;
	virtual bool isSubChannelCountOverFlow(UINT32* maxCount) = 0;

	virtual void putMgmtPasswd(LPCSTR pwd) = 0;
	virtual bool hasMgmtPassword() const = 0;
	virtual void cancelMgmtPassword() = 0;

	// sig: echoIsValidMgmtPasswd
	typedef _def_boost_signals2_signal_type<void (bool isValid)>::type	_SIG_IS_VALID_MGMT_PWD;
	virtual _SIG_IS_VALID_MGMT_PWD *getSigIsValidMgmtPwd() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_CANCEL_MGMT_PWD;
	virtual _SIG_CANCEL_MGMT_PWD *getSigCancelMgmtPwd() = 0;
	// sig: chgChangeChannelInfo
	typedef _def_boost_signals2_signal_type<void (UINT32, IPropBagWrapperPtr, void*)>::type	_SIG_CHANGE_INFO;
	virtual _SIG_CHANGE_INFO  *getSigChgInfo() = 0;

	virtual void asyncChangeBulletin( UINT32 sid, LPCWSTR newBulletin ) = 0;

	virtual void asyncBanUser( UINT32 uid, LPCWSTR reason, bool banID) = 0;

	virtual void asyncSetAllKeyVoice(UINT32 sid) = 0;
	virtual void asyncEnableAllText(UINT32 sid, bool b) = 0;
	virtual void asyncEnableGuestText(UINT32 sid, bool b) = 0;
	virtual void asyncMoveUsers(UINT32 sid, UINT32 subSid) = 0;
	virtual void asyncChangeJiedaiSid(UINT32 sid, bool b) = 0;
	virtual void asyncMoveUser(UINT32 sid, UINT32 uid, UINT32 subSid) = 0;
	virtual void asyncMaixuJoin(UINT32 uid) = 0;
	virtual void asyncSetKeyVoice(UINT32 uid) = 0;
	virtual void asyncEnableVoice(UINT32 sid, bool b) = 0;
	virtual void asyncEnableText(UINT32 sid, bool b) = 0;
	virtual void asynQuitGuild() = 0;
	
	virtual bool switchChannelStyle() = 0;
	virtual void asyncChangeCurSubSid(UINT32 newSubSid, LPCSTR szPassword) = 0;
	virtual void allocateChannel( UINT32 creatorUid, std::wstring& channelTag) = 0;
	virtual void destroyChannel( UINT32 tmpChannelId ) = 0;
	virtual void refreshChannel( UINT32 tmpChannelId ) = 0;

	virtual void asyncMoveAnonyUsers(UINT32 nCount, UINT32 topSid, UINT32 fromSid, UINT32 toSid, const std::set<UINT32> &uids)=0;
	virtual void asyncKickOutAnonyUsers(UINT32 nCount, UINT32 topSid, UINT32 fromSid, LPCWSTR reason)=0;

	// ����Ա�Ҽ��˵��Զ����Ƿ�����ο�˽��
	virtual void asyncChangeGuestPrivateChatPermission(UINT32 topsid, UINT32 subsid, UINT32 uid, bool permission) = 0;

	/*
	  @brief ���û������󣬲���������������ʱ�䣨�������Ϲ���ԱȨ�ޣ�
	  @param uid �����������û�uid
	  @param time ��������ʱ������λ����
	*/
	virtual void asyncAddToTopFirstMaixu(UINT32 uid, UINT32 time)=0;

	/*
	@brief  ����ҵ�����󻥶��ĺ�һ��Ͻӿڣ��Ͽ������������󣬽��û������󣬶�������������������������ʱ�����������Ϲ���ԱȨ�ޣ�
	@param uid ���������û�uid
	@param time ��������ʱ������λ����
	*/
	virtual void asyncAdd2ndQueueChorus(UINT32 uid, UINT32 time)=0;
};
