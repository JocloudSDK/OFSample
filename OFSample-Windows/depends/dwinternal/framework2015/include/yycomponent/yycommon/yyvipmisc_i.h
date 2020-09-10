#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"

/**
   * @brief vip相关内容接口.
   */
DWDEFINE_INTERFACE(IYYVipMisc) : public IUnk
{
	/**
   * @brief 获取自身vip信息到达信号.
   * @param .
   * @return vip表情版本号更新通知信号.
   */
	typedef _def_boost_signals2_signal_type<void (UINT32, UINT32)>::type _SIG_MY_VIPINFO_CHANGED;
	virtual _SIG_MY_VIPINFO_CHANGED *myVipInfoChangedSignal() = 0;

	/**
   * @brief 获取vip表情白名单.
   * @param .
   * @return vip表情白名单更新通知信号.
   */
	typedef _def_boost_signals2_signal_type<void (std::vector<UINT32>)>::type _SIG_VIP_WHITELIST_RES;
	virtual _SIG_VIP_WHITELIST_RES *vipWhiteListResSignal() = 0;

	/**
   * @brief 异步获取vip表情推广列表.
   * @param .
   * @return . 
   */
	virtual void asyncGetPopularize() = 0;

	/**
   * @brief 异步获取vip表情白名单.
   * @param .
   * @return . 
   */
	virtual void asyncGetVipExpWhiteList(const std::vector<UINT32>& vecUids) = 0;

	/**
   * @brief  获取vip表情推广列表.
   * @param 
   * @return vip表情推广列表更新通知信号.
   */
	struct PopularizeAttr
	{
		int     m_uGrpId;         // 表情组件ID
		std::wstring  m_strKey;         // 表情key
	};
	typedef std::vector<PopularizeAttr> PopularizeAttrList;

	typedef _def_boost_signals2_signal_type<void (const PopularizeAttrList&)>::type _SIG_POPULARIZE_RES;
	virtual _SIG_POPULARIZE_RES *popularizeResSignal() = 0;

	virtual void getUserOffLineScore( const UINT32 uid,const UINT32 from ) = 0;

	typedef _def_boost_signals2_signal_type<void ( const UINT32& uid,const UINT32& score,const UINT32& add_score )>::type _SIG_GET_USER_OFFLINE_SCORE_FOR_CHANNEL;
	virtual _SIG_GET_USER_OFFLINE_SCORE_FOR_CHANNEL *getSigGetOfflineScoreForChannel() = 0;


	// 查询回收站好友
	virtual void asyncGetRecycleBuddy(UINT32 uLastPos, UINT32 uCount) = 0;

	// 恢复好友
	virtual void asyncRestoreRecycleBuddy(const std::vector<UINT32>& vecBid) = 0;

	// 查询回收站好友响应
	typedef _def_boost_signals2_signal_type<void (UINT32 totalCount)>::type _SIG_RECYCLE_BUDDIES_READY;
	virtual _SIG_RECYCLE_BUDDIES_READY *recycleBuddiesReadySignal() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 currentPos, IPropBagCollPtrCR buddiesUinfo)>::type _SIG_RECYCLE_BUDDIES_RECEIVED;
	virtual _SIG_RECYCLE_BUDDIES_RECEIVED* recycleBuddiesReceivedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 resCode)>::type _SIG_RECYCLE_BUDDIES_RECEIVED_ERROR;
	virtual _SIG_RECYCLE_BUDDIES_RECEIVED_ERROR *recycleBuddiesReceivedErrorSignal() = 0;

	// 恢复好友响应
	typedef _def_boost_signals2_signal_type<void (UINT32 uRescode, const std::vector<UINT32>& vecBidSuc, const std::vector<UINT32>& vecBidFailed)>::type _SIG_RESTORE_BUDDIES_RES;
	virtual _SIG_RESTORE_BUDDIES_RES *restoreBuddiesResSignal() = 0;

    // 会员消息
    typedef _def_boost_signals2_signal_type<void (UINT32 id, const std::string& msg)>::type _SIG_VIP_MSG;
    virtual _SIG_VIP_MSG *vipMsgSignal() = 0;

	virtual std::wstring queryUrl(const std::wstring &key) = 0;

    struct PurpleInfo
    {
        UINT8 grade;
        UINT8 type;
        PurpleInfo() { grade = type = 0; }
    };
    virtual PurpleInfo getPurpleInfo(UINT32 uid) = 0;  // 本地有缓存会马上返回
    // grade 0 非紫钻会员  1 ~ 7 ,  type 0 月费  1 年费
    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 grade, UINT32 type)>::type _SIG_PURPLE_INFO_RES;
    virtual _SIG_PURPLE_INFO_RES *purpleInfoSignal() = 0;

    virtual UINT8 myPurpleGrade() = 0;
    virtual UINT8 myPurpleType() = 0;

    enum EPurplePrivi
    {
        E_PURPLE_NICK = 0,       // 紫色昵称
        E_PURPLE_SUPER,          // 超级紫钻
        E_PURPLE_EXVIP,          // 至尊会员
        E_PURPLE_GAME,           // 游戏紫钻
		E_SUPER_VIP,             //超凡会员
    };

    virtual int getPurplePrivi(UINT32 uid, EPurplePrivi privi) = 0;  // 本地有缓存会马上返回, -1表示不可用

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, EPurplePrivi privi, int value)>::type _SIG_PURPLE_PRIVI_RES;
    virtual _SIG_PURPLE_PRIVI_RES *purplePriviSignal() = 0;

    virtual void GetOwVipInfo(UINT32 uid) = 0;

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT8 isOwvip)>::type _SIG_OWVIP_RES;
    virtual _SIG_OWVIP_RES *owvipInfoSignal() = 0;

    virtual UINT8 isOwvip() = 0;

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 score, UINT32 addScore, UINT32 type)>::type _SIG_VIP_PURPLE_SCORE_RES;
    virtual _SIG_VIP_PURPLE_SCORE_RES *getSigPurpleScore() = 0; 

    virtual void getPurpleScore(UINT32 type, UINT32 uid) = 0;

    enum { PrettyNumAppId = 6 };
    virtual QString miniLightInfo(UINT32 appId, UINT32 dengji, const QString& key) = 0;

   /**
   * @brief  获取频道公屏靓号皇族图标信息.
   * @param	dengji，靓号皇族图标等级
   * @return 靓号皇族图标信息.
   */
	virtual IPropBagWrapperPtr getVipPrettyNumInfo(UINT32 dengji) = 0;
};