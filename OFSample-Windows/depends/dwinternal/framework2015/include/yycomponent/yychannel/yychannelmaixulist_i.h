#pragma once
#include "dwbase/dwcomex.h"

/**
*@brief Ƶ������������źŽӿ�.\n
*signals:
*	void maixuStyleChanged(int style);//Ƶ��ģʽ�ı�.\n
*	void maixuAdd( const QList<quint32> &uids );//�������������ź�.\n
*	void maixuRemove( const QList<quint32> &uids );.\n
*	void maixuMoved( quint32 uid, quint32 toAfterUid );//����ĳ��uidλ�õ����ź�.\n
*	void maixuRemoveAll();//��������ź�.\n
*	void maixuMaiTurn(quint32 uid, quint32 sec);//�����ֵ�˭�ˣ��ж���ʱ��.\n
*	void maixuResetMaiTime(quint32 admin, quint32 uid, quint32 sec);//����Աadmin�������û�uid������ʱ���Ϊsec.\n
*	void maixuTimeChanged( quint32 admin, quint32 newTime );//Ƶ����Ϣ�����������ʱ��ı�.\n
*	void maixuOverMax();//���򵽴���������ź�.\n
*	\n
*	void multiMaiChanged( quint32 adminUid, quint32 userId, bool beAdded);//����仯�źţ�adminUid��ӵ�й���ԱȨ�޵Ĳ����ţ�userId�����������ˣ�beAdded���Ǽ���������Ǵ��������쳣.\n
*	void multiMaiFinished(quint32 firstUser);//�������.\n
*	void multiMaiInvited(quint32 invitor);//��������֪ͨ.\n
*	void multiMaiInviteReplyed(quint32 uid, bool accept);//��������ظ�.\n
*	void multiMaiNumberOvered();//���������ﵽ����.\n

*	\n
*	void maixuControlled(quint32 adminUid);//�����ź�.\n
*	void maixuReleased(quint32 adminUid);//�����ź�.\n
*	void maixuForbade(quint32 adminUid);//��ֹ�����ź�.\n
*	void maixuAllowed(quint32 adminUid);//���������ź�.\n

*/

DWDEFINE_INTERFACE(IYYChannelMaiXuList) : public IDWQComponent
{
/**
   * @brief �������һ���û�.
   * @return �����һ���û���Uid.
   */
	virtual quint32 currentSpeaker() = 0;

/**
	* @brief ��������û��б�.
	* @return �����һ���û���Uid�б�.
	*/
	virtual QList<quint32> maiXuList()  = 0;
/**
   * @brief ��ȡƵ��ģʽ�����������壩.
   * @return CHANNEL_STYLE_FREE = 0(����ģʽ), CHANNEL_STYLE_CHAIR = 1(��ϯģʽ), CHANNEL_STYLE_MAIXU = 2(����ģʽ).
   */
	virtual int maiXuStyle() = 0;
/**
   * @brief �ж��Ƿ��а�һ���˱��������Ȩ��.
   * @param id�������˵�uid.u
   * @return ����Ȩ�ޣ�����true�����򷵻�false.
   */
	virtual bool canPushToMaixu(quint32 uid) = 0 ;
/**
   * @brief ��һ���˱�������.
   * @param uid����Ҫ����������˵�uid.
   */
	virtual void pushToMaixu(quint32 uid) = 0;
/**
   * @brief �ж��Ƿ��а�һ���˱���2�������Ȩ��.
   * @param uid,��Ҫ������2���������uid.
   * @return ����Ȩ�ޣ�����true�����򷵻�false.
   */
	virtual bool canMove2TopMaixu(quint32 uid) = 0;
/**
   * @brief ��һ���˱���2������.
   * @param ��Ҫ������2���������uid.
   */
	virtual void move2TopMaixu(quint32 uid) = 0 ;
/**
   * @brief �ж��Լ��Ƿ��������Ȩ��
   * @return ����Ȩ�ޣ�����true�����򷵻�false.
   */
	virtual bool canJoinMaixu() = 0;
/**
   * @brief �Լ�����.
   */
	virtual void joinMaixu() = 0;
/**
   * @brief ��ȡ����ʱ��.
   * @return ��������ʱ��.
   */
	virtual quint32 getMaixuTime() = 0;

/**
	* @brief ��ȡ�����ϵ������û��б�.
	* @return ���������ϵ������û��б�.
	*/
	virtual QList<quint32> multiMaiList() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 uid, bool bAdd)>::type	_SIG_PUSH_TO_MUTIMAI;
	virtual _SIG_PUSH_TO_MUTIMAI* getSigPushToMutiMai() = 0;

/**
   * @brief ����Ȩ�޲�ѯ��IYYChannelRulerChecker::canMuteQueue��.
   */
	virtual void kongMai() = 0;
/**
   * @brief ����Ȩ�޲�ѯ��IYYChannelRulerChecker::canUnmuteQueue��.
   */
	virtual void fangMai() = 0;

/**
   * @brief �ı�Ƶ��ģʽ��Ȩ�޲�ѯ��IYYChannelRulerChecker::canChangeStyle��.
   * @param stype��Ƶ��ģʽ��0,����ģʽ��1,��ϯģʽ��2������ģʽ.
   */
	virtual void channgeMaixuStype(int stype) = 0;


/**
   * @brief ��һ���˼��������У�Ȩ�޲�ѯ��IYYChannelRulerChecker::canPush2MutiMai��.
   * @param uid����Ҫ�����������uid.
   */
	virtual void push2MutiMai(quint32 uid) = 0;
/**
   * @brief ��һ���˴��������Ƴ���Ȩ�޲�ѯ��IYYChannelRulerChecker::canKickFromMutiMai��.
   * @param uid����Ҫ�Ƴ��������uid.
   */
	virtual void kickFromMutiMai(quint32 uid) = 0;

/**
   * @brief ����.
   */
	virtual void leaveMaixu() = 0;

/**
	* @brief ��ֹ����Ȩ�޲�ѯ��IYYChannelRulerChecker::canForbidJoiningMaixu��.
	*/
	virtual void forbidMaixu() = 0;

/**
	* @brief ��������Ȩ�޲�ѯ��IYYChannelRulerChecker::canAllowJoiningMaixu��.
	*/
	virtual void allowMaixu() = 0;

	/**
	* @brief ��ȡ�����һ���û���ǰ�Ļ��ж���ʱ��.
	* @return �����һ���û���ǰ�Ļ��ж���ʱ��.
	*/
	virtual quint32 getFirstUserTime() = 0;

	/**
	* @brief �����Ƿ����ڿ�����.
	* @return ��������У�����true������false.
	*/
	virtual bool isKongMai() = 0;

	/**
	* @brief ��������.
	* @param uid���������.��Ȩ�޲�ѯ��IYYChannelRulerChecker::canInvite2MultiMai��
	*/	
	virtual void invite2MultiMai(quint32 uid) = 0; 

	/**
	* @brief ȡ����������.
	* @param uid��ȡ���������.��Ȩ�޲�ѯ��IYYChannelRulerChecker::canKickFromInvitedMultiMaixu��
	*/	
	virtual void kickFromInvitedMultiMaixu(quint32 uid) = 0;

	/**
	* @brief ������.
	* @param invitor��������.
	* @param accept���Ƿ��������.
	*/	
	virtual void replyMultiMaiInvite(quint32 invitor, bool accept) = 0;

/**
   * @brief �ӱ�����ʱ�䣨Ȩ�޲�ѯ��IYYChannelRulerChecker::canDoubleTimeMaixu��.
   */
	virtual void doubleTimeMaixu() = 0;
	
/**
   * @brief �������Ȩ�޲�ѯ��IYYChannelRulerChecker::canClearMaixu��.
   */
	virtual void clearMaixu() = 0;
	
/**
   * @brief ����һλ��Ȩ�޲�ѯ��IYYChannelRulerChecker::canMoveUpMaixu��.
   */
	virtual void maixuMoveUp(UINT32 uid) = 0;

/**
   * @brief ����һλ��Ȩ�޲�ѯ��IYYChannelRulerChecker::canMoveDownMaixu��.
   */
	virtual void maixuMoveDown(UINT32 uid) = 0;

/**
	* @brief �����Ƿ����ڿ�����.
	* @return �������У�����true������false.
	*/
	virtual bool isAllowMaixu() = 0;

/**
   * @brief .��û��Ȩ�޷��Ͷ�����������
   * @param .��
   * @return .��Ȩ�޷���true�����򷵻�false
   */
	virtual bool canNotifySecondMaixu() = 0;

/**
	* @brief ���Ͷ�������
		Ȩ�����⣺
		1������������ģʽ��
		2�������ϱ��볬��������
		3���Լ�����Ϊ����
		4���Լ�Ҫ���㹻Ȩ�� (��Ȩ�޲�ѯ��canNotifySecondMaixu)
		����4���⣬����3����Ҫ�Լ������ж�
	*/
	virtual void notifySecondMaixu() = 0;

	virtual bool isInMutiMai(UINT32 uid) = 0;
	virtual bool canInviteToMutiMaiByTopMai(UINT32 uid) = 0;
	virtual bool canKickFromMutiMaiByTopMai(UINT32 uid) = 0;
	virtual void asyncMutiMaiInvite(UINT32 uid, bool add) = 0;
	virtual void asyncReplyMutiMaiInvite(UINT32 uid, bool accept) = 0;

	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type	_SIG_ADD_SOME_MAIXU;
	virtual _SIG_ADD_SOME_MAIXU *getSigAddSomeMaixu() = 0;
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type	_SIG_REMOVE_SOME_MAIXU;
	virtual _SIG_REMOVE_SOME_MAIXU *getSigRemoveSomeMaixu() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 toAfterUid)>::type	_SIG_MOVE_MAIXU;
	virtual _SIG_MOVE_MAIXU *getSigMoveMaixu() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_REMOVE_ALL;
	virtual _SIG_REMOVE_ALL *getSigRemoveAll() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 flags)>::type	_SIG_SOMEONE_INFO_CHANGED;
	virtual _SIG_SOMEONE_INFO_CHANGED *getSigSomeoneInfoChanged() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 admin)>::type	_SIG_KONG_MAI;
	virtual _SIG_KONG_MAI *getSigKongMai() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 admin)>::type	_SIG_FANG_MAI;
	virtual _SIG_KONG_MAI *getSigFangMai() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 admin)>::type	_SIG_ALLOW_JOINING;
	virtual _SIG_ALLOW_JOINING *getSigAllowJoining() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 admin)>::type	_SIG_FORBID_JOINING;
	virtual _SIG_FORBID_JOINING *getSigForbidJoining() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_OVER_MICUSER_MAX;
	virtual _SIG_REMOVE_ALL *getSigOverMicUserMax() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 admin, UINT32 uid, UINT32 sec)>::type	_SIG_RESET_MAI_TIME;
	virtual _SIG_RESET_MAI_TIME *getSigResetMaiTime() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 sec)>::type	_SIG_MAI_TURN;
	virtual _SIG_MAI_TURN *getSigMaiTurn() = 0;

	virtual void refresh() = 0;
	virtual void removeFromMaixu(UINT32 uid) = 0;
	virtual void doubleTimeMaixu2(UINT32 uid) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type	_SIG_MAIXU_TIME_TICK;
	virtual _SIG_MAIXU_TIME_TICK *getSigMaixuTimeTick() = 0;
    typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_REFRESH_MAIXU;
    virtual _SIG_REFRESH_MAIXU *getSigRefreshMaixu() = 0;

	/**
   * @brief ��һ���˼���������,��token.(΢��ҵ���õ�)
   * @param uid����Ҫ�����������uid.
   * @param appKey�������ҵ��id.
   * @param token��Ȩ����֤�����Ƹ�ʽ.
   * @param context��͸������.
   */
	virtual void push2MutiMai2(quint32 uid,quint32 appKey,const std::string& token,const QString& context) = 0;

	/**
   * @brief ��һ���˴��������Ƴ�,��token.(΢��ҵ���õ�)
   * @param uid����Ҫ�Ƴ��������uid.
   * @param appKey�������ҵ��id.
   * @param token��Ȩ����֤�����Ƹ�ʽ.
   * @param context��͸������.
   */
	virtual void kickFromMutiMai2(quint32 uid,quint32 appKey,const std::string& token,const QString& context) = 0;

	/**
   * @brief ��������¼�.
   * @param uri�����������ʶ��.
   * @param errorCode��������.
   * @param uiConext��͸������.
   * @param props�������ֶ�.
   */
	typedef _def_boost_signals2_signal_type<void (INT32 errorCode, const std::string& uiConext, const std::map<UINT32, std::string>& props)>::type	_SIG_MAIXU_OPERATE_RESULT;
	virtual _SIG_MAIXU_OPERATE_RESULT* getSigMaixuOperateResult() = 0;

	/**
	* @brief ������������ʱ����Ȩ���ж�Ϊ����Ա��.
	* @ param time, ����ʱ��
	*/
	virtual void setTopMaixuTime(UINT32 time) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 admin, UINT32 uid, UINT32 sec)>::type	_SIG_TOP_MAI_OCCUPIED;
	//virtual _SIG_TOP_MAI_OCCUPIED *getSigTopMaiOccupied() = 0;


	/**
	* @brief ��������¼�.
	* @param uri�����������ʶ��.
	* @param errorCode��������.
	* @param uiConext��͸������.
	* @param props �����ֶ�.
	* @param reqOperType  ��ӦSessRequest�е�reqType
	* @param subOperType  ��reqOperTpe�Ĳ���
 	*/
	typedef _def_boost_signals2_signal_type<void (INT32 errorCode, const std::string& uiConext, const std::map<UINT32, std::string>& props, UINT32 reqOperType, UINT32 subOperType)>::type	_SIG_MAIXU_OPERATE_RESULT_EX;
	virtual _SIG_MAIXU_OPERATE_RESULT_EX* getSigMaixuOperateResultEx() = 0;
};
