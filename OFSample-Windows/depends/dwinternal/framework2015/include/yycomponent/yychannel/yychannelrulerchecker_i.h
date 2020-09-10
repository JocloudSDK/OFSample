#pragma once
#include "dwbase/dwcomex.h"

/* use for getObject() */
DWDEFINE_INTERFACE( IYYChannelRulerChecker ) : public IUnk
{
/**
   * @brief .权限初始化，对某个频道的某个人的操作有没有权限。如果user为0，则是对频道的操作。当操作的对象变动时，需要重新调用这个接口进行初始化，否则权限是上次的操作对象的权限表
   * @param .room，频道或子频道id；user，用户Uid。
   * @return .无返回值
   */
	virtual void initChecker(UINT32 room, UINT32 user) = 0;
/**
   * @brief .判断能不能进入某个子频道，
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canJoinChannel() = 0;
/**
   * @brief .判断能不能进入顶级频道（大厅）
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canJoinTopChannel() = 0;
/**
   * @brief .进入子频道是否需要密码
   * @param .无
   * @return .不需要返回true，否则返回false
   */
	virtual bool canJoinWithoutPasswd() = 0;

/**
   * @brief .有没有权限控麦
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canMuteQueue() = 0;	
/**
   * @brief .有没有权限放麦
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canUnmuteQueue() = 0;
/**
   * @brief .有没有权限进行连麦操作
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canPush2MutiMai() = 0;	
/**
   * @brief .有没有权限进行取消连麦操作
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canKickFromMutiMai() = 0;
/**
   * @brief .有没有权限改变频道模式
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canChangeStyle() = 0;


/**
   * @brief .有没有权限对于一个用户更改角色，用户Uid在调用initChecker传入。
   * @param .role，角色；addOrRemove，加上或移除该角色
   * @return .有权限返回true，否则返回false
   */
	virtual bool canMakeUserAs(ROLE_IN_CHANNEL role, bool addOrRemove) = 0;


/**
	* @brief .有没有权限禁止抢麦。
	* @param .无
	* @return .有权限返回true，否则返回false
	*/
	virtual bool canForbidJoiningMaixu() = 0;

/**
	* @brief .有没有权限允许抢麦。
	* @param .无
	* @return .有权限返回true，否则返回false
	*/
	virtual bool canAllowJoiningMaixu() = 0;

	/**
	* @brief 有没有权限邀请别人和自己连麦，自己需要在首麦，被邀请者必须在麦序。该接口无需要先调initChecker.
	* @param uid，被邀请的用户.
	* @return 有权限返回true，否则返回false.
	*/
	virtual bool canInvite2MultiMai(quint32 uid) = 0;

	/**
	* @brief 有没有权限取消别人和自己连麦，自己需要在首麦。该接口无需要先调initChecker.
	* @param id，被邀请的用户.
	* @return 有权限返回true，否则返回false.
	*/
	virtual bool canKickFromInvitedMultiMaixu(quint32 uid) = 0;
/**
   * @brief .有没有权限加倍麦序时间
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canDoubleTimeMaixu() = 0;

/**
   * @brief .有没有权限清空麦序
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canClearMaixu() = 0;

	virtual bool canEditBulletin() = 0;

	virtual bool canEditChannelMessage() = 0;

	virtual bool canDisableQueue() = 0;

	virtual bool canAllowQueue() = 0;
	
	virtual bool canShowAsManager() = 0;

	virtual bool canJoinQueue() = 0;

	virtual bool canDropUser() = 0;

	virtual bool canDragUser()  = 0;
	virtual bool canMoveUsers() = 0; // 批量调度
	virtual bool canDragUsers() = 0;
	virtual bool canDropUsers() =0;
	virtual bool canSendChatMsg() =0;
	virtual bool canJoinHisChannel() =0;
	virtual bool canAddFriend() =0;
	virtual bool canMoveUser() =0;
	virtual bool canDragToMaixu() =0;
	virtual bool canSetKeyActive() =0;
	virtual bool canKickUser(bool) =0;
	virtual bool canEnableText(bool) =0;
	virtual bool canEnableVoice(bool) =0;
	virtual bool canBanUser(bool) =0;
	virtual bool canQuitGuild() =0;
	virtual bool canBroadcastToSubChannel() =0;
	virtual bool canSetAllKeyVoice() =0;
	virtual bool canManageText() =0;
	virtual bool canCreateSubChannel() =0;
	virtual bool canDestroySubChannel() =0;
	virtual bool canSetJiedaiChannel() =0;
	virtual bool canClearJiedaiChannel() =0;
	virtual bool canShowGuildManagement() =0;
	virtual bool canShowApplyGuild() =0;
	virtual bool canRemoveFromMaixu() =0;
	virtual bool canMoveUpMaixu() =0;
	virtual bool canMoveDownMaixu() =0;
	virtual bool canMove2TopMaixu() =0;
	virtual bool canILaunchInvitation() =0;
    virtual bool canVoice() = 0;
    virtual bool canEditChannelLimit() = 0;
    virtual bool canDebanIPID() = 0;
    virtual bool canText() = 0;
	virtual bool canSetChannelPassword() = 0;//设置频道密码
	virtual bool canSetPeopleNumberLimit() = 0;//设置频道人数限制
	virtual bool canSetIsLimitedToVisitor() = 0;//设置是否禁止游客进入
	virtual bool isLimitSeeNAddFriend() = 0; // 设置是否游客不能查看管理员以下成员的信息及加好友 //8.45弃用
	virtual bool canSetIsLimitSeeNAddFriends() = 0;//是否有编辑私聊，好友的权限
	virtual void asynGetRulerChecker(quint32 room, quint32 user, quint32 hostId, const std::wstring &hostParam) = 0;//配合opKey2WString生成最后一个参数
	virtual void initAsynChecker(UINT32 room, UINT32 user) = 0;

	typedef _def_boost_signals2_signal_type<void (quint32 hostId, const std::wstring &hostParam)>::type	_SIG_RULERCHECKER_ARRIVED;
	virtual _SIG_RULERCHECKER_ARRIVED *getRulerCheckerArrivedSignal() = 0;

    virtual bool canFreeAudioChat() = 0;    // 频道自由说话及播放伴奏

	/*
	* @ brief 虎牙模板定制RQ-15463
	*/
	// Fun1 是否开启了业务定制（开关），restoreBusinessMadeSwitch默认为false
	virtual void setBusinessMadeSwitch(bool onOff) = 0;
	virtual bool getBusinessMadeSwitch() = 0;
	virtual void restoreBusinessMadeSwitch() = 0;
	/*
	  Fun2 设置 自由说话/播放伴奏
	  setBusinessMadeSwitch 设置true才生效, restoreFreeAudioEnable默认为true
	*/
	virtual void setFreeAudioEnable(bool enable) = 0;
	virtual bool getFreeAudioEnable() = 0;
	virtual void restoreFreeAudioEnable() = 0;
	/* 
	  Fun3 设置人员右键菜单/Folder右键菜单-- 按键说话子菜单（显示/隐藏）
	  不受setBusinessMadeSwitch 设置值限制, restoreKeyVoiceEnable默认为true
    */
	virtual void setKeyVoiceEnable(bool enable) = 0;
	virtual bool getKeyVoiceEnable() = 0;
	virtual void restoreKeyVoiceEnable() = 0;

	typedef _def_boost_signals2_signal_type<void (bool enable)>::type	_SIG_FREEAUDIO_ENABLECHANGED;
	virtual _SIG_FREEAUDIO_ENABLECHANGED *getFreeAudioEnableChangedSignal() = 0;

	/*
	* @ brief 管理员（黄马及以上）自行设置接受/不接受游客私聊的值
	*      0=不接受 1=接受 3=用户未设置，取频道OW统一设置的值
	*/
	virtual void setGuestPrivateChatPermission(UINT32 permission) = 0;
	virtual UINT32 getGuestPrivateChatPermission() = 0;

	/*
	* @brief 是否限制查看用户信息和加用户为好友。旧的isLimitSeeNAddFriend弃用，由于initChecker时传入的参数是sid和topsid，
	* 而且hover频道树用户就会调用，可能会引起频道树右键用户菜单的查权有问题，现改为单独接口。该接口无需要先调initChecker.
	* @param userCurSid，被邀请的用户当前所在子频道.
	* @param topSid，被邀请的用户的顶级频道id.
	* @return 限制返回true，否则返回false.
	*/
	virtual bool isLimitSeeNAddFriend2(quint32 userCurSid, quint32 topSid) = 0;

	virtual std::wstring OPKeys2WString(const QSet<quint32> &) = 0; //配合asynGetRulerChecker使用，生成最后一个参数
};
