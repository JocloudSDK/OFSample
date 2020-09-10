#pragma once
#include <vector>
#include "dwbase/dwcomex.h"
#include "dwbase/IData.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/* use for getObject() */
struct ChannelSimpleInfo
{
	quint32 id;
	quint32 parentId;
	QString name;
};

/**
* @brief 频道信息接口，读取频道具体信息字段请用辅助类YYChannelInfoHelper
*/
DWDEFINE_INTERFACE( IYYChannelInfomation ) : public IUnk
{
/**
   * @brief 获取一个子频道的信息，字段见sessiontables.h.
   * @param sid，子频道id.
   * @return 指向子频道信息的指针.
   */
	virtual Data::IRowPtr getChannelRow( UINT32 sid ) = 0;
/**
   * @brief 获取频道短位Id.
   * @return 频道短位Id，如没有，返回频道id.
   */
	virtual UINT32 getAsid() = 0;
/**
   * @brief 获取频道id.
   * @return 频道id.
   */
	virtual UINT32 getSid() = 0;
/**
   * @brief 获取当前子频道id.
   * @return 当前子频道id.
   */
	virtual UINT32 getCurSubSid() = 0;
/**
   * @brief 获取频道名字.
   * @return 频道名字.
   */
	virtual QString getChannelName() = 0;
/**
   * @brief 获取频道模式.
   * @return 频道模式.
   */	
	virtual CHANNEL_STYLE getCurChannelStyle() = 0;
/**
	* @brief 获取当前子频道改变信号.
	* @param newSid，新的子频道id.
	* @return 当前子频道改变信号.
	*/	
	typedef _def_boost_signals2_signal_type<void (UINT32 newSid)>::type	_SIG_CUR_SUB_SID_CHANGED;
	virtual _SIG_CUR_SUB_SID_CHANGED* getCurSubSidChangedSignal() = 0;

/**
	* @brief 获取频道进入信号.
	* @return 频道进入信号.
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_JOINED;
	virtual _SIG_JOINED* getJoinedSignal() = 0;
	
/**
	* @brief 获取频道离开信号.
	* @return 频道离开信号.
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_LEAVED;
	virtual _SIG_LEAVED* getLeavedSignal() = 0;

/**
	* @brief 获取频道模式改变信号.
	* @param admin，改变频道模式的管理员Uid； newChannelStyle，新的频道模式.
	* @return 频道模式改变信号.
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 admin, UINT32 newChannelStyle)>::type	_SIG_CUR_CHANNEL_STYLE_CHANGED;
	virtual _SIG_CUR_CHANNEL_STYLE_CHANGED* getCurChannelStyleChangedSignal() = 0;

/**
	* @brief 获取透传数据到达信号.
	* @param 透传的数据.
	* @return 透传数据到达信号.
	*/
	typedef _def_boost_signals2_signal_type<void (const QString&)>::type	_SIG_DATA_ARRIVED;
	virtual _SIG_DATA_ARRIVED* getDataArrivedSignal() = 0;

/**
	* @brief 获取子频道列表.
	* @return 子频道id列表.
	*/
	virtual QVector<ChannelSimpleInfo> getChannelSimpleInfos() = 0;

	virtual QString getDisplayName(UINT32 sid) = 0;

	virtual bool hasJiedai() = 0;
	virtual bool isJieDaiSubSid(UINT32 subSid) = 0;
	virtual bool hasJieDaiSubSid(UINT32 subSid) = 0;
	virtual UINT32 getSpeakerRtt(UINT32 uid) = 0;
	virtual UINT32 getPing() = 0; // 网络延迟

	typedef _SIG_CUR_SUB_SID_CHANGED _SIG_CUR_CHANNEL_INFO_CHANGED;
	virtual _SIG_CUR_CHANNEL_INFO_CHANGED* getCurChannelInfoChangedSignal() = 0;

    // spErrInfo目前CPROP_ErrorHandled表示外接处理该signal，客户端不再处理
	typedef _def_boost_signals2_signal_type<void (CHANNEL_ERROR_CODE, IPropBagWrapperPtr spErrInfo)>::type	_SIG_ERROR;
	virtual _SIG_ERROR *getErroredSignal() = 0;
	virtual void leave() = 0;

	typedef _def_boost_signals2_signal_type<void (DATA_NOTIFY_TYPE dataNotifyType, Data::IRowArrayPtr spNewRowArray, Data::IRowArrayPtr spOldRowArray)>::type	_SIG_DATA_NOTIFY;
	virtual _SIG_DATA_NOTIFY* getSigChannelTable() = 0;
	virtual _SIG_DATA_NOTIFY* getSigManagerList() = 0;
	typedef _def_boost_signals2_signal_type<void (Data::IFieldSetDataPtr spFieldSet)>::type	_SIG_FIELDSET_NOTIFY;
	virtual _SIG_FIELDSET_NOTIFY* getSigChannelFieldSet() = 0;


	//大频道优化
	virtual bool hasMoreUsers(UINT32 sid) = 0;
	virtual void asynGetMoreUsers(UINT32 sid) = 0;
	virtual void notifyUpdateUsers(const std::vector<BCUser>& users) = 0;
	virtual void getSearchResult(const std::wstring& keyword) = 0;


	typedef _def_boost_signals2_signal_type<void (const std::vector<UINT32>& results, const std::wstring& keyword,UINT8 resWithAuth)>::type _SIG_SEARCHRESULT_RES;
	virtual _SIG_SEARCHRESULT_RES* getSearchResultResSignal() = 0;

	virtual Data::IFieldSetPtr getChannelFieldSet() = 0;

	/**
	* @brief 获取当前频道真实在线人数.
	*/
	virtual UINT32 getTotalUserCount() = 0;

	virtual Data::ITablePtr getChannelTable() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SUBSCRIBE_UIDS_READY;
	virtual _SIG_SUBSCRIBE_UIDS_READY *getSigSubscribeUidsReady() = 0;

	typedef _def_boost_signals2_signal_type<void (const std::vector<ETEntry>& res_entries, int status, UINT32 locateUid)>::type _SIG_SCROLLBAR_INFO;
	virtual _SIG_SCROLLBAR_INFO *getSigScrollbarInfo() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_CUR_SID_CONFIRMED;
	virtual _SIG_CUR_SID_CONFIRMED *getSigCurSidConfirmed() = 0;

	typedef _def_boost_signals2_signal_type<void (LPCTSTR,UINT32)>::type	_SIG_IMAGE_URL_RECEIVED;
	virtual _SIG_IMAGE_URL_RECEIVED*		getSigImageUrlReceived() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type _SIG_KICKEDFROMVIPCHN;
	virtual _SIG_KICKEDFROMVIPCHN* getSigKickedFromVipChannel() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type _SIG_VIPCHNREACH;
	virtual _SIG_VIPCHNREACH* getSigVipChannelReachable() = 0;

	/**
	* @brief 获取频道公屏打字权限设置.
	* @return 1表示允许所有人打字，2表示禁止所有人打字，3表示禁止游客打字.
	*/
	virtual UINT32 getChannelTextStatus(UINT32 sid) = 0;

	virtual bool isExist(UINT32 uid , UINT32 sid) = 0;
	virtual bool isFolder(UINT32 sid) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 subSid)>::type _SIG_JUMP_SUB_CHANNEL_NEED_PASSWORD;
	virtual _SIG_JUMP_SUB_CHANNEL_NEED_PASSWORD *getSigJumpSubChannelNeedPassword() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_JUMP_SUB_CHANNEL_FAIL;
	virtual _SIG_JUMP_SUB_CHANNEL_FAIL *getSigJumpSubChannelFail() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 subSid, LPCWSTR tips)>::type _SIG_JUMP_SUB_CHANNEL_NEED_CHARGE;
	virtual _SIG_JUMP_SUB_CHANNEL_NEED_CHARGE *getSigJumpSubChannelNeedCharge() = 0;

	virtual bool isHugeChannel() const = 0;

	typedef _def_boost_signals2_signal_type<void (LPCWSTR, UINT32)>::type	_SIG_STAT_CHANNEL_REPORT;
	virtual _SIG_STAT_CHANNEL_REPORT*  sigStatChannelReport() = 0;

    virtual void reportJoinFromByString( const std::string& tag ) = 0;

    virtual _SIG_DATA_NOTIFY* getSigChannelTableEx(const NAME_IDS* exceptFieldFilters) = 0;

	typedef _def_boost_signals2_signal_type<void (const std::string &strSeq, const std::string &internalId, const std::string &strImg, const std::string &strReason) >::type _SIG_NEED_VERIFY_PICCODE;
	virtual _SIG_NEED_VERIFY_PICCODE* sigNeedVerifyPicCode() = 0;

	virtual void queryAppUInfo(const std::vector<UINT32>& uids) = 0;
	typedef _def_boost_signals2_signal_type<void (const std::vector<IPropBagPtr>& res_Uinfos, int retCode)>::type _SIG_APP_QUERY_UINFO_RES;
	virtual _SIG_APP_QUERY_UINFO_RES* getSigAppQueryUinfoRes() = 0;

	virtual void getUserLocation(UINT32 uid)=0;
	typedef _def_boost_signals2_signal_type<void (const UINT32 uid, UINT8 resCode)>::type _SIG_USERLOCATION_RES;
	virtual _SIG_USERLOCATION_RES* getSigUserLocationRes() = 0;

	virtual void getSearchMoreResult(const std::wstring& keyword, UINT32 lastSearchedUid)=0;

	/**
	  * @brief 获得当前频道和子频道管理员列表
	  * @return Data::IRowArrayPtr 列表集合
	  */
	virtual Data::IRowArrayPtr getManagerList() = 0;

	/**
	  * @brief 请求特定子频道管理员列表
	  * @param subSid 需要获得的子频道id
	  */
	virtual void querySubChannelManagerList(const UINT32 subSid) = 0;

	/**
	  * @brief 特定子频道列表请求返回
	  * @param subSid 子频道id
	  * @param managerColl 管理员属性集合
	  */
	DWDECLARE_SIGNAL(SigSubChannelManagerListResponsed, void(const UINT32 subSid, IPropBagCollPtrCR managerColl));

	/**
	* @brief 获取当前频道业务设置在线人数.
	* @return 返回-1表示没有设置过数据.
	*/
	virtual INT64 getAppTotalUserCount(void) = 0;

	/**
	* @brief 提供给业务设置当前频道在线人数.
	* @param nCount 在线人数,值为-1时，表示取消当前的设置，频道窗口标题则还原显示真实的人数.
	*/
	virtual bool setAppTotalUserCount(INT64 nCount) = 0;

	/**
	* @brief 将UED认证的code传给服务器.
	* @param strCode UED认证的code.
	*/
	virtual void sendSwitchTemplateVerifyCode(const QString& strCode) = 0;

	/**
	* @brief 切换模板认证信号.
	* @param nCode  参见protocol::session::ETSesspOperateAuthRes::OperateResCode.
	* @param strUrl UED认证的url.
	*/
	typedef _def_boost_signals2_signal_type<void (int nCode,const QString &strUrl) >::type _SIG_SWITCH_TEMPLATE_VERIFY;
	virtual _SIG_SWITCH_TEMPLATE_VERIFY* getSigSwitchTemplateVerify() = 0;

	/**
	* @brief 异步获取ping 数据.
	* @param 
	*/
	virtual void asynGetPing() = 0;
	
	typedef _def_boost_signals2_signal_type<void (UINT32 value)>::type _SIG_GET_PING;
	virtual _SIG_GET_PING* getSigGetPing() = 0;

	/**
	* @brief 异步获取SpeakerRtt 数据.
	* @param 
	*/
	virtual void asynGetSpeakerRtt(UINT32 uid) = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 value)>::type _SIG_GET_SPEAKERRTT;
	virtual _SIG_GET_SPEAKERRTT* getSigGetSpeakerRtt() = 0;

    /**
    * @brief 设置是否忽略基础模板显示的“限制中国地区ip”提示
    * @param true表示忽略（基础模板样式的提示不显示）
    */
    virtual void setIgnoreLimitChinaIpDefaultTips(bool setting) = 0;
    /*
    * @brief 获取setIgnoreLimitChinaIpDefaultTips设置的值
    */
    virtual bool isIgnoreLimitChinaIpDefaultTips() = 0;
    typedef _def_boost_signals2_signal_type<void ()>::type _SIG_LIMIT_CHINA_IP;
    virtual _SIG_LIMIT_CHINA_IP* getSigLimitChinaIp() = 0;


	/*
	* @brief 获取模板设置退出频道的自定义弹窗文案
	*     若文案为空，忽略弹窗提示；
	*/
	virtual void setLeaveChannelRemindStr(const std::wstring& str) = 0;
	virtual void restoreLeaveChannelRemindStr() = 0;
	virtual std::wstring getLeaveChannelRemindString() = 0;
	
	// 对匿名用户进行了封禁
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_ANONYMOUS_USER;
	virtual _SIG_ANONYMOUS_USER* getSigAnonymousUser() = 0;

	/* 
	    从主面板/频道/精彩世界 的开播快速入口点击进入绑定直播间/签约频道channel
		如用户已在绑定直播间channel 内，抛出该信号
		如用户当前不在绑定直播间channel，会通过飞机票进入频道，并携带userData信息: {startLiveKey:0}
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 sid, UINT32 ssid)>::type _SIG_IN_CHANNEL_ALREADY;
	virtual _SIG_IN_CHANNEL_ALREADY* getSigInChannelAlready() = 0;

    /*
    ** @brief 将其他已进入（当前终端即将进入的频道）的终端从频道中踢出
    ** @param(s) now_from, froms 从getSigChannelCheckKickoff或KConfirmPCJoinChannel2得到
    */
    virtual void kickoff(UINT8 now_from, const std::vector<UINT8>& froms) = 0;

    /*
    ** @brief   信号。
    **          该信号在进入尝试进入频道后，检测到相同账号在其他端进入了频道时触发
    **          收到该信号后，如需要确认从当前终端进入频道，调用kickoff接口，参数直接传入
    **          否则，应该从当前终端退出当前频道
    **          等同于KConfirmPCJoinChannel2
    ** @param now_from 表示当前尝试进入频道的终端id
    ** @param froms    表示已进入了频道的其他终端id列表
    */
    typedef _def_boost_signals2_signal_type<void (UINT8 now_from, const std::vector<UINT8>& froms)>::type _SIG_CHANNEL_CHECK_KICKOFF;
    virtual _SIG_CHANNEL_CHECK_KICKOFF* getSigChannelCheckKickoff() = 0;

	/*
	* @brief 退出频道自定义弹窗提醒(设置标志后，就会抛出信号_SIG_LEAVE_CHANNEL_TAKEOVER)
	* @param tmpId: 模板ID 或者组件ID，不可为0，否则无效（可用于查找到哪个业务设置了该标志）
	*/
	virtual void setLeaveChannelTakeoverFlag(UINT32 tmpId) = 0;
	/*
	*@brief 与resetLeaveChannelTakeoverFlag配套使用，重置标志位0
	*/
	virtual void resetLeaveChannelTakeoverFlag() = 0;
	virtual bool getLeaveChannelTakeoverFlag() = 0;
	
	/*
	* @brief 业务处理完_SIG_LEAVE_CHANNEL_TAKEOVER信号后，回调此接口交由客户端处理
	* 业务调用此回调接口，客户端会调用resetLeaveChannelTakeoverFlag重置标志！！！
	*/
	virtual void leaveChannelTakeoverCallback(const QMap<int,QVariant>&) = 0;

	/*
	* @brief 准备退出频道信号，通过setLeaveChannelTakeoverFlag接口设置标志后才会抛出
	*/
	typedef _def_boost_signals2_signal_type<void (const QMap<int,QVariant>&)>::type _SIG_LEAVE_CHANNEL_TAKEOVER;
	virtual _SIG_LEAVE_CHANNEL_TAKEOVER* getSigLeaveChannelTakeover() = 0;
	
	virtual bool isSuperChannel() const = 0;
	virtual bool isChannelTreeReady() const = 0;
};

DWDEFINE_SIGNAL(IYYChannelInfomation::SigSubChannelManagerListResponsed);
