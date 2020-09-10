#pragma once
#include "dwbase/IData.h"
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/propBag_i.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/

enum NOTIFY_TYPE
{
	NOTIFY_DATA_INS = 0,
	NOTIFY_DATA_DEL = 1,
	NOTIFY_DATA_CHANGED = 2,
	NOTIFY_DATA_RESET = 3,
};

DWDEFINE_INTERFACE(IYYChannelUserList) : public IUnk
{
/**
   * @brief .获取频道总人数
   * @param .无
   * @return .频道总人数
   */
	virtual int userCount() = 0;

/**
   * @brief .单个用户调度
   * @param .uid，被调度用户uid;fromSid,源子频道;toSid,目的子频道
   * @return .暂时无用
   */
	virtual int asyncMoveUser(UINT32 uid, UINT32 fromSid, UINT32 toSid ) = 0;

/**
   * @brief .将一个子频道的人调度到另一个子频道
   * @param .fromSid,源子频道；toSid,目的子频道
   * @return .暂时无用
   */
	virtual int asyncMoveUsers(UINT32 fromSid, UINT32 toSid ) = 0;
	
/**
	* @brief .获取用户所在的子频道
	* @param .uid，用户uid
	* @return .用户所在的子频道。如果返回0，用户已经离开频道
	*/
	virtual UINT32 currentChannelId( UINT32 uid ) = 0;

/**
   * @brief .获取用户在频道里全部信息
   * @param .uid,用户uid
   * @return .指向用户全部信息的数据中心的指针（通过sessiontables.h里面的字段获取想要的信息）
   */
	virtual Data::IRowPtr getUserInfo( UINT32 uid ) = 0;

	// 大频道用户数量很大，谨慎使用这个信号.
	typedef xsignals::signal<void (Data::IRowArrayPtr)> USERS_SIGNAL;
	virtual USERS_SIGNAL* getUsersAddSignal() = 0;
	virtual USERS_SIGNAL* getUsersRemoveSignal() = 0;
	typedef xsignals::signal<void (Data::IRowArrayPtr, Data::IRowArrayPtr)> USERS_CHANGE_SIGNAL;
	virtual USERS_CHANGE_SIGNAL* getUsersChangeSignal() = 0;

	typedef xsignals::signal<void (UINT32, UINT32, const std::vector<UINT32>& )> SIG_USER_MOVED;
	virtual SIG_USER_MOVED* getUserMovedSignal() = 0;

/**
   * @brief .获取某个子频道的人数
   * @param .subChannelId，子频道id
   * @return .子频道的人数，如果是一级子频道，则人数包括二级子频道的人数
   */
	virtual int subChannelUserCount(UINT32 subChannelId) = 0;


/**
   * @brief .获取父亲节点
   * @param .xid，频道id或者用户id；isUser，区分前面的xid是否是用户id
   * @return .父亲节点sid，如果返回0，且传入的是用户id，则该用户已经离开频道，其他情况则可能是数据异常
   */
	virtual UINT32 getParentSidOf(UINT32 xid, bool isUser) = 0;


	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_ALL_USERS_LOADED;
/**
	* @brief .获取所有用户信息加载完毕信号
	* @param .无
	* @return .所有用户信息加载完毕信号,注：这里只是查询用户信息的请求都发出去了，但是不一定有收到所有用户的信息
	*/
	virtual _SIG_ALL_USERS_LOADED* getAllUsersLoadedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_SELF_INFO_COMPLETED;
/**
	* @brief .获取自己信息加载完毕信号
	* @param .无
	* @return .自己信息加载完毕信号
	*/
	virtual _SIG_SELF_INFO_COMPLETED* getSelfInfoCompletedSignal() = 0;


	typedef _def_boost_signals2_signal_type<void (Data::IRowArrayPtr pRowArray) >::type _SIG_MYINFO_CHANGED;
/**
	* @brief .获取自己信息改变信号
	* @param .无
	* @return .自己信息改变信号
	*/
	virtual _SIG_MYINFO_CHANGED* getMyInfoChanged() = 0;

/**
   * @brief .定位某个用户，让这个用户在频道树处于可见
   * @param .uid，用户id
   * @return .无
   */
	virtual void locateUser(quint32 uid) = 0;

/**
   * @brief .设置某个用户在公屏的文本信息是否忽略
   * @param .uid，用户id；isIgnore，是否忽略
   * @return .无
   */
	virtual void setIgnoreText(quint32 uid, bool isIgnore) = 0;

/**
   * @brief .设置某个用户在说话声音是否忽略
   * @param .uid，用户id；isIgnore，是否忽略
   * @return .无
   */
	virtual void setIgnoreVoice(quint32 uid, bool isIgnore) = 0;

/**
   * @brief .调度一批用户，如果这一批用户不在同一个子频道，或者调度者对某个用户没有调度权限，将不会调度成功
   * @param .uids，用户id列表；fromSid，源子频道；toSid，目的子频道
   * @return .无
   */	
	virtual void asyncMoveMultiUsers(const QVector<quint32>& uids, UINT32 fromSid,  UINT32 toSid ) = 0;

/**
	* @brief .获取某个子频道的成员列表
	* @param .sid，子频道id
	* @return .子频道的成员uid列表
	*/	
	virtual QList<quint32> getUserList(quint32 sid) = 0;


	typedef xsignals::signal<void (NOTIFY_TYPE, Data::IRowArrayPtr , Data::IRowArrayPtr)> _SIG_CURRENT_USER_PID_CHANGE;
	virtual _SIG_CURRENT_USER_PID_CHANGE* getCurrentUserPidChange() = 0;

	virtual void getDisplayUnick(UINT32 uid, QString &nick) = 0;

	virtual ROLE_IN_CHANNEL getUserRole(UINT32 uid, UINT32 specSid) = 0;

	typedef _def_boost_signals2_signal_type<void (DATA_NOTIFY_TYPE dataNotifyType, Data::IRowArrayPtr spNewRowArray, Data::IRowArrayPtr spOldRowArray)>::type	_SIG_DATA_NOTIFY;
	virtual _SIG_DATA_NOTIFY* getSigUserTable(const NAME_IDS* fieldFilters) = 0;

	virtual Data::ITablePtr getUserTable() = 0;
	virtual Data::IRowPtr getUserRow(UINT32 uid, USERROW_LOC_TYPE rowLocType) = 0;
	virtual Data::IRowPtr getMyChannelInfo(USERROW_LOC_TYPE rowLocType ) = 0;
	virtual IPropBagPtr getUserBasicInfoPropBag(UINT32 uid) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type	_SIG_REQUEST_LOCATEUSER;
	virtual _SIG_REQUEST_LOCATEUSER* getRequestLocateuserSignal() = 0;

/**
	* @brief .获取某个子频道滚动条前n位的成员列表
	* @param .sid，子频道id
	* @param .count，需要获取的成员数
	* @return .子频道前n位的成员uid列表
	*/
	virtual QList<quint32> getUserTopList(quint32 uid, int count) = 0;

	/**
	* @brief .匿名+非匿名用户批量调度
	* @param .count，匿名用户数
	* @param .topSid，顶级频道号
	* @param .fromSid，源频道号
	* @param .toSid，目标频道号
	* @param .uids，非匿名用户uids(无则空)
	*/
	virtual void asyncMoveAnonyUsers(UINT32 nCount, UINT32 topSid, UINT32 fromSid, UINT32 toSid, const std::set<UINT32> &uids)=0;

	/**
   * @brief .设置某个用户在说话声音大小
   * @param .uid，用户id；nVolume，0-200 用户初始值为100
   * @return .无
   */
	virtual void setSpeakVolume(quint32 uid, quint32 nVolume) = 0;
};
