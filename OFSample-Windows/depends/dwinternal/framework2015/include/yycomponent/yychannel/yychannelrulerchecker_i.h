#pragma once
#include "dwbase/dwcomex.h"

/* use for getObject() */
DWDEFINE_INTERFACE( IYYChannelRulerChecker ) : public IUnk
{
/**
   * @brief .Ȩ�޳�ʼ������ĳ��Ƶ����ĳ���˵Ĳ�����û��Ȩ�ޡ����userΪ0�����Ƕ�Ƶ���Ĳ������������Ķ���䶯ʱ����Ҫ���µ�������ӿڽ��г�ʼ��������Ȩ�����ϴεĲ��������Ȩ�ޱ�
   * @param .room��Ƶ������Ƶ��id��user���û�Uid��
   * @return .�޷���ֵ
   */
	virtual void initChecker(UINT32 room, UINT32 user) = 0;
/**
   * @brief .�ж��ܲ��ܽ���ĳ����Ƶ����
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canJoinChannel() = 0;
/**
   * @brief .�ж��ܲ��ܽ��붥��Ƶ����������
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canJoinTopChannel() = 0;
/**
   * @brief .������Ƶ���Ƿ���Ҫ����
   * @param .��
   * @return .����Ҫ����true�����򷵻�false
   */
	virtual bool canJoinWithoutPasswd() = 0;

/**
   * @brief .��û��Ȩ�޿���
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canMuteQueue() = 0;	
/**
   * @brief .��û��Ȩ�޷���
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canUnmuteQueue() = 0;
/**
   * @brief .��û��Ȩ�޽����������
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canPush2MutiMai() = 0;	
/**
   * @brief .��û��Ȩ�޽���ȡ���������
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canKickFromMutiMai() = 0;
/**
   * @brief .��û��Ȩ�޸ı�Ƶ��ģʽ
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canChangeStyle() = 0;


/**
   * @brief .��û��Ȩ�޶���һ���û����Ľ�ɫ���û�Uid�ڵ���initChecker���롣
   * @param .role����ɫ��addOrRemove�����ϻ��Ƴ��ý�ɫ
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canMakeUserAs(ROLE_IN_CHANNEL role, bool addOrRemove) = 0;


/**
	* @brief .��û��Ȩ�޽�ֹ����
	* @param .��
	* @return .��Ȩ�޷���true�����򷵻�false
	*/
	virtual bool canForbidJoiningMaixu() = 0;

/**
	* @brief .��û��Ȩ����������
	* @param .��
	* @return .��Ȩ�޷���true�����򷵻�false
	*/
	virtual bool canAllowJoiningMaixu() = 0;

	/**
	* @brief ��û��Ȩ��������˺��Լ������Լ���Ҫ�����󣬱������߱��������򡣸ýӿ�����Ҫ�ȵ�initChecker.
	* @param uid����������û�.
	* @return ��Ȩ�޷���true�����򷵻�false.
	*/
	virtual bool canInvite2MultiMai(quint32 uid) = 0;

	/**
	* @brief ��û��Ȩ��ȡ�����˺��Լ������Լ���Ҫ�����󡣸ýӿ�����Ҫ�ȵ�initChecker.
	* @param id����������û�.
	* @return ��Ȩ�޷���true�����򷵻�false.
	*/
	virtual bool canKickFromInvitedMultiMaixu(quint32 uid) = 0;
/**
   * @brief .��û��Ȩ�޼ӱ�����ʱ��
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canDoubleTimeMaixu() = 0;

/**
   * @brief .��û��Ȩ���������
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
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
	virtual bool canMoveUsers() = 0; // ��������
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
	virtual bool canSetChannelPassword() = 0;//����Ƶ������
	virtual bool canSetPeopleNumberLimit() = 0;//����Ƶ����������
	virtual bool canSetIsLimitedToVisitor() = 0;//�����Ƿ��ֹ�οͽ���
	virtual bool isLimitSeeNAddFriend() = 0; // �����Ƿ��οͲ��ܲ鿴����Ա���³�Ա����Ϣ���Ӻ��� //8.45����
	virtual bool canSetIsLimitSeeNAddFriends() = 0;//�Ƿ��б༭˽�ģ����ѵ�Ȩ��
	virtual void asynGetRulerChecker(quint32 room, quint32 user, quint32 hostId, const std::wstring &hostParam) = 0;//���opKey2WString�������һ������
	virtual void initAsynChecker(UINT32 room, UINT32 user) = 0;

	typedef _def_boost_signals2_signal_type<void (quint32 hostId, const std::wstring &hostParam)>::type	_SIG_RULERCHECKER_ARRIVED;
	virtual _SIG_RULERCHECKER_ARRIVED *getRulerCheckerArrivedSignal() = 0;

    virtual bool canFreeAudioChat() = 0;    // Ƶ������˵�������Ű���

	/*
	* @ brief ����ģ�嶨��RQ-15463
	*/
	// Fun1 �Ƿ�����ҵ���ƣ����أ���restoreBusinessMadeSwitchĬ��Ϊfalse
	virtual void setBusinessMadeSwitch(bool onOff) = 0;
	virtual bool getBusinessMadeSwitch() = 0;
	virtual void restoreBusinessMadeSwitch() = 0;
	/*
	  Fun2 ���� ����˵��/���Ű���
	  setBusinessMadeSwitch ����true����Ч, restoreFreeAudioEnableĬ��Ϊtrue
	*/
	virtual void setFreeAudioEnable(bool enable) = 0;
	virtual bool getFreeAudioEnable() = 0;
	virtual void restoreFreeAudioEnable() = 0;
	/* 
	  Fun3 ������Ա�Ҽ��˵�/Folder�Ҽ��˵�-- ����˵���Ӳ˵�����ʾ/���أ�
	  ����setBusinessMadeSwitch ����ֵ����, restoreKeyVoiceEnableĬ��Ϊtrue
    */
	virtual void setKeyVoiceEnable(bool enable) = 0;
	virtual bool getKeyVoiceEnable() = 0;
	virtual void restoreKeyVoiceEnable() = 0;

	typedef _def_boost_signals2_signal_type<void (bool enable)>::type	_SIG_FREEAUDIO_ENABLECHANGED;
	virtual _SIG_FREEAUDIO_ENABLECHANGED *getFreeAudioEnableChangedSignal() = 0;

	/*
	* @ brief ����Ա���������ϣ��������ý���/�������ο�˽�ĵ�ֵ
	*      0=������ 1=���� 3=�û�δ���ã�ȡƵ��OWͳһ���õ�ֵ
	*/
	virtual void setGuestPrivateChatPermission(UINT32 permission) = 0;
	virtual UINT32 getGuestPrivateChatPermission() = 0;

	/*
	* @brief �Ƿ����Ʋ鿴�û���Ϣ�ͼ��û�Ϊ���ѡ��ɵ�isLimitSeeNAddFriend���ã�����initCheckerʱ����Ĳ�����sid��topsid��
	* ����hoverƵ�����û��ͻ���ã����ܻ�����Ƶ�����Ҽ��û��˵��Ĳ�Ȩ�����⣬�ָ�Ϊ�����ӿڡ��ýӿ�����Ҫ�ȵ�initChecker.
	* @param userCurSid����������û���ǰ������Ƶ��.
	* @param topSid����������û��Ķ���Ƶ��id.
	* @return ���Ʒ���true�����򷵻�false.
	*/
	virtual bool isLimitSeeNAddFriend2(quint32 userCurSid, quint32 topSid) = 0;

	virtual std::wstring OPKeys2WString(const QSet<quint32> &) = 0; //���asynGetRulerCheckerʹ�ã��������һ������
};
