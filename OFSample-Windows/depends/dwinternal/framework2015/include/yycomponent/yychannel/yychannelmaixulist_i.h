#pragma once
#include "dwbase/dwcomex.h"

/**
*@brief 频道麦序操作和信号接口.\n
*signals:
*	void maixuStyleChanged(int style);//频道模式改变.\n
*	void maixuAdd( const QList<quint32> &uids );//麦序有人上麦信号.\n
*	void maixuRemove( const QList<quint32> &uids );.\n
*	void maixuMoved( quint32 uid, quint32 toAfterUid );//麦序某人uid位置调整信号.\n
*	void maixuRemoveAll();//麦序清空信号.\n
*	void maixuMaiTurn(quint32 uid, quint32 sec);//麦首轮到谁了，有多少时间.\n
*	void maixuResetMaiTime(quint32 admin, quint32 uid, quint32 sec);//管理员admin对麦首用户uid的麦序时间改为sec.\n
*	void maixuTimeChanged( quint32 admin, quint32 newTime );//频道信息里的麦序设置时间改变.\n
*	void maixuOverMax();//麦序到达最大人数信号.\n
*	\n
*	void multiMaiChanged( quint32 adminUid, quint32 userId, bool beAdded);//连麦变化信号，adminUid，拥有管理员权限的操作着；userId，被操作的人；beAdded，是加入连麦或是从连麦中异常.\n
*	void multiMaiFinished(quint32 firstUser);//连麦结束.\n
*	void multiMaiInvited(quint32 invitor);//连麦邀请通知.\n
*	void multiMaiInviteReplyed(quint32 uid, bool accept);//连麦邀请回复.\n
*	void multiMaiNumberOvered();//连麦人数达到上限.\n

*	\n
*	void maixuControlled(quint32 adminUid);//控麦信号.\n
*	void maixuReleased(quint32 adminUid);//放麦信号.\n
*	void maixuForbade(quint32 adminUid);//禁止抢麦信号.\n
*	void maixuAllowed(quint32 adminUid);//允许抢麦信号.\n

*/

DWDEFINE_INTERFACE(IYYChannelMaiXuList) : public IDWQComponent
{
/**
   * @brief 获得麦序一个用户.
   * @return 麦序第一个用户的Uid.
   */
	virtual quint32 currentSpeaker() = 0;

/**
	* @brief 获得麦序用户列表.
	* @return 麦序第一个用户的Uid列表.
	*/
	virtual QList<quint32> maiXuList()  = 0;
/**
   * @brief 获取频道模式（名字有歧义）.
   * @return CHANNEL_STYLE_FREE = 0(自由模式), CHANNEL_STYLE_CHAIR = 1(主席模式), CHANNEL_STYLE_MAIXU = 2(麦序模式).
   */
	virtual int maiXuStyle() = 0;
/**
   * @brief 判断是否有把一个人抱上麦序的权限.
   * @param id，被抱人的uid.u
   * @return 若有权限，返回true，否则返回false.
   */
	virtual bool canPushToMaixu(quint32 uid) = 0 ;
/**
   * @brief 把一个人抱上麦序.
   * @param uid，将要被抱麦序的人的uid.
   */
	virtual void pushToMaixu(quint32 uid) = 0;
/**
   * @brief 判断是否有把一个人抱上2号麦序的权限.
   * @param uid,将要被抱上2号麦序的人uid.
   * @return 若有权限，返回true，否则返回false.
   */
	virtual bool canMove2TopMaixu(quint32 uid) = 0;
/**
   * @brief 把一个人抱上2号麦序.
   * @param 将要被抱上2号麦序的人uid.
   */
	virtual void move2TopMaixu(quint32 uid) = 0 ;
/**
   * @brief 判断自己是否有上麦的权限
   * @return 若有权限，返回true，否则返回false.
   */
	virtual bool canJoinMaixu() = 0;
/**
   * @brief 自己上麦.
   */
	virtual void joinMaixu() = 0;
/**
   * @brief 获取麦序时间.
   * @return 返回麦序时间.
   */
	virtual quint32 getMaixuTime() = 0;

/**
	* @brief 获取麦序上的连麦用户列表.
	* @return 返回麦序上的连麦用户列表.
	*/
	virtual QList<quint32> multiMaiList() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 uid, bool bAdd)>::type	_SIG_PUSH_TO_MUTIMAI;
	virtual _SIG_PUSH_TO_MUTIMAI* getSigPushToMutiMai() = 0;

/**
   * @brief 控麦（权限查询见IYYChannelRulerChecker::canMuteQueue）.
   */
	virtual void kongMai() = 0;
/**
   * @brief 放麦（权限查询见IYYChannelRulerChecker::canUnmuteQueue）.
   */
	virtual void fangMai() = 0;

/**
   * @brief 改变频道模式（权限查询见IYYChannelRulerChecker::canChangeStyle）.
   * @param stype，频道模式，0,自由模式；1,主席模式，2，麦序模式.
   */
	virtual void channgeMaixuStype(int stype) = 0;


/**
   * @brief 把一个人加入连麦中（权限查询见IYYChannelRulerChecker::canPush2MutiMai）.
   * @param uid，将要加入连麦的人uid.
   */
	virtual void push2MutiMai(quint32 uid) = 0;
/**
   * @brief 把一个人从连麦中移除（权限查询见IYYChannelRulerChecker::canKickFromMutiMai）.
   * @param uid，将要移除连麦的人uid.
   */
	virtual void kickFromMutiMai(quint32 uid) = 0;

/**
   * @brief 下麦.
   */
	virtual void leaveMaixu() = 0;

/**
	* @brief 禁止抢麦（权限查询见IYYChannelRulerChecker::canForbidJoiningMaixu）.
	*/
	virtual void forbidMaixu() = 0;

/**
	* @brief 允许抢麦（权限查询见IYYChannelRulerChecker::canAllowJoiningMaixu）.
	*/
	virtual void allowMaixu() = 0;

	/**
	* @brief 获取麦序第一个用户当前的还有多少时间.
	* @return 麦序第一个用户当前的还有多少时间.
	*/
	virtual quint32 getFirstUserTime() = 0;

	/**
	* @brief 麦序是否正在控麦中.
	* @return 麦序控麦中，返回true，否则false.
	*/
	virtual bool isKongMai() = 0;

	/**
	* @brief 邀请连麦.
	* @param uid，邀请对象.（权限查询见IYYChannelRulerChecker::canInvite2MultiMai）
	*/	
	virtual void invite2MultiMai(quint32 uid) = 0; 

	/**
	* @brief 取消邀请连麦.
	* @param uid，取消邀请对象.（权限查询见IYYChannelRulerChecker::canKickFromInvitedMultiMaixu）
	*/	
	virtual void kickFromInvitedMultiMaixu(quint32 uid) = 0;

	/**
	* @brief 答复邀请.
	* @param invitor，邀请者.
	* @param accept，是否接受邀请.
	*/	
	virtual void replyMultiMaiInvite(quint32 invitor, bool accept) = 0;

/**
   * @brief 加倍麦序时间（权限查询见IYYChannelRulerChecker::canDoubleTimeMaixu）.
   */
	virtual void doubleTimeMaixu() = 0;
	
/**
   * @brief 清空麦序（权限查询见IYYChannelRulerChecker::canClearMaixu）.
   */
	virtual void clearMaixu() = 0;
	
/**
   * @brief 上移一位（权限查询见IYYChannelRulerChecker::canMoveUpMaixu）.
   */
	virtual void maixuMoveUp(UINT32 uid) = 0;

/**
   * @brief 下移一位（权限查询见IYYChannelRulerChecker::canMoveDownMaixu）.
   */
	virtual void maixuMoveDown(UINT32 uid) = 0;

/**
	* @brief 麦序是否正在开麦中.
	* @return 麦序开麦中，返回true，否则false.
	*/
	virtual bool isAllowMaixu() = 0;

/**
   * @brief .有没有权限发送二麦麦序提醒
   * @param .无
   * @return .有权限返回true，否则返回false
   */
	virtual bool canNotifySecondMaixu() = 0;

/**
	* @brief 发送二麦提醒
		权限问题：
		1、必须在麦序模式中
		2、麦序上必须超过两个人
		3、自己不能为二麦
		4、自己要有足够权限 (该权限查询见canNotifySecondMaixu)
		除第4项外，其他3项需要自己另外判断
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
   * @brief 把一个人加入连麦中,带token.(微房业务用到)
   * @param uid，将要加入连麦的人uid.
   * @param appKey，分配的业务id.
   * @param token，权限认证二进制格式.
   * @param context，透传参数.
   */
	virtual void push2MutiMai2(quint32 uid,quint32 appKey,const std::string& token,const QString& context) = 0;

	/**
   * @brief 把一个人从连麦中移除,带token.(微房业务用到)
   * @param uid，将要移除连麦的人uid.
   * @param appKey，分配的业务id.
   * @param token，权限认证二进制格式.
   * @param context，透传参数.
   */
	virtual void kickFromMutiMai2(quint32 uid,quint32 appKey,const std::string& token,const QString& context) = 0;

	/**
   * @brief 麦序操作事件.
   * @param uri，麦序操作标识符.
   * @param errorCode，错误码.
   * @param uiConext，透传参数.
   * @param props，保留字段.
   */
	typedef _def_boost_signals2_signal_type<void (INT32 errorCode, const std::string& uiConext, const std::map<UINT32, std::string>& props)>::type	_SIG_MAIXU_OPERATE_RESULT;
	virtual _SIG_MAIXU_OPERATE_RESULT* getSigMaixuOperateResult() = 0;

	/**
	* @brief 设置首麦麦序时长（权限判断为管理员）.
	* @ param time, 麦序时长
	*/
	virtual void setTopMaixuTime(UINT32 time) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 admin, UINT32 uid, UINT32 sec)>::type	_SIG_TOP_MAI_OCCUPIED;
	//virtual _SIG_TOP_MAI_OCCUPIED *getSigTopMaiOccupied() = 0;


	/**
	* @brief 麦序操作事件.
	* @param uri，麦序操作标识符.
	* @param errorCode，错误码.
	* @param uiConext，透传参数.
	* @param props 保留字段.
	* @param reqOperType  对应SessRequest中的reqType
	* @param subOperType  对reqOperTpe的补充
 	*/
	typedef _def_boost_signals2_signal_type<void (INT32 errorCode, const std::string& uiConext, const std::map<UINT32, std::string>& props, UINT32 reqOperType, UINT32 subOperType)>::type	_SIG_MAIXU_OPERATE_RESULT_EX;
	virtual _SIG_MAIXU_OPERATE_RESULT_EX* getSigMaixuOperateResultEx() = 0;
};
