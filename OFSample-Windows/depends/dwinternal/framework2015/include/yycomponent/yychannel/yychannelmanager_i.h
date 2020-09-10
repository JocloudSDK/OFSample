#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/**
* @brief 频道管理.
*/
DWDEFINE_INTERFACE( IYYChannelManager ) : public IUnk
{
/**
   * @brief 设置接待频道.
   * @param sid，子频道id.
   * @param enable，接待是否生效.
   */
	virtual void asyncSetReceive(UINT32 sid, bool enable ) = 0;

	/**
	* @brief 设置接待频道改变信号.
	* @param sid，子频道id.
	* @param enable，接待是否生效.
	* @param uid，谁改变接待频道.
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 sid, bool)>::type	_SIG_ECHO_RECEPTION_ALTER;
	virtual _SIG_ECHO_RECEPTION_ALTER* sigEchoReception() = 0;

/**
   * @brief 改变频道用户的马甲.
   * @param uid,被改变的用户uid.
   * @param role，将要改变成为的角色.
   * @param sid，某个特殊频道id，针对子频道管理员的角色时使用.
   */
	virtual void asyncChangeUserRole( UINT32 uid, ROLE_IN_CHANNEL role, UINT32 sid ) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32/* admin*/, UINT32/* who*/, ROLE_IN_CHANNEL/* oldRole*/, ROLE_IN_CHANNEL/* newRole*/, UINT32 /*specSid*/)>::type	_SIG_ECHO_ROLE_CHANGED;

	/**
	* @brief 获取频道角色改变信号.
	* @return 频道角色改变信号.
	*/
	virtual _SIG_ECHO_ROLE_CHANGED* sigEchoRoleChanged() = 0;

/**
   * @brief 判断有没有踢人的权限 .
   * @param sid，将要从这里踢人的子频道id.
   * @param uid，将要被踢的用户uid.
   * @return 有权限返回true，否则返回false.
   */
	virtual bool	canKickOutUser(UINT32 sid, UINT32 uid) = 0;
/**
   * @brief 将一个用户踢出频道或子频道.
   * @param sid，将要从这里踢人的子频道id.
   * @param uid，将要被踢的用户uid.
   * @param minute，踢出多少时间，单元分钟.
   * @param reason，踢出理由.
   * @param kickOff，true，踢出频道，false，踢出用户所在子频道到大厅，.
  */
	virtual void asyncKickOutUser(UINT32 sid, UINT32 uid, UINT32 minute, LPCWSTR reason, bool kickOff) = 0;

/**
   * @brief 改变频道用户的马甲.
   * @param uid,被改变的用户uid.
   * @param role，将要改变成为的角色.
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

	// 管理员右键菜单自定义是否接受游客私聊
	virtual void asyncChangeGuestPrivateChatPermission(UINT32 topsid, UINT32 subsid, UINT32 uid, bool permission) = 0;

	/*
	  @brief 将用户抱首麦，并设置其首麦麦序时间（橙马及以上管理员权限）
	  @param uid 被抱上首麦用户uid
	  @param time 设置首麦时长，单位：秒
	*/
	virtual void asyncAddToTopFirstMaixu(UINT32 uid, UINT32 time)=0;

	/*
	@brief  陪我业务连麦互动四合一组合接口：断开首麦已有连麦，将用户抱二麦，二麦与首麦连麦，设置首麦麦序时长（橙马及以上管理员权限）
	@param uid 被抱二麦用户uid
	@param time 设置首麦时长，单位：秒
	*/
	virtual void asyncAdd2ndQueueChorus(UINT32 uid, UINT32 time)=0;
};
