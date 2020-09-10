#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

define_interface(IChannel);
/**
* @brief 内部接口，设置频道指针，组件勿调用.
*/
DWDEFINE_INTERFACE(IYYVideoRegulatoryInner): public IUnk
{
/**
   * @brief 设置频道指针.
   * @param spChannel，频道指针.
   */
	virtual void setChannel(IChannelPtr spChannel) = 0;
};



// 单个主播的状态
struct CompereStatus
{
	UINT32 uid;
	UINT32 reportedCount; // 举报次数
	UINT32 version;
	UINT32 fine; // 罚款金额
	bool isPunished; // 是否被处罚

	CompereStatus() : uid(0), reportedCount(0), version(0), fine(0), isPunished(false){}
};

// 主播权限信息和人数限制
struct ComperePrivilege
{
	UINT32 uidMaxCount; // 主播的观看人数上限, -1表示无限制
	UINT32 channelMaxCount; // 频道的观看人数上限（目前只有娱乐会用到这个）
};

struct UserImage
{
	UINT32 uid; // 被举报的用户
	std::vector<std::string> images; // 被举报用户的图片证据，可以多张
};


DWDEFINE_INTERFACE(IYYVideoRegulatory) : public IUnk
{
	/**
	* 提供给各业务的举报入口
	* @param appid 业务id
	* @param compereId 被举报的uid
	* @param uids 有视频的用户id（可能有多个，比如多人视频），这里包含主播uid
	*/
	virtual void report(UINT32 appId, UINT32 compereId, const QList<UINT32>& uids) = 0;

	/**
	* 进频道的时候，查询主播的状态 (是否被举报，异步取的，通过连_SIG_COMPERE_STATUS来获取结果)
	* @param uids 有视频的用户id（可能有多个，比如多人视频）
	*/
	virtual void queryCompereStatus(const QList<UINT32>& uids) = 0;

	/**
	* 主播改变的时候，通知服务器 (服务器决定是否需要广播，通过连_SIG_COMPERE_STATUS来获取结果)
	* @param uids 有视频的用户id（可能有多个，比如多人视频）
	*/
	virtual void notifyCompereChanged(const QList<UINT32>& uids) = 0;

	// 主播的状态（被举报、罚款信息）
	typedef _def_boost_signals2_signal_type<void (const QList<CompereStatus>&)>::type _SIG_COMPERE_STATUS;
	virtual _SIG_COMPERE_STATUS* getCompereStatusSignal() = 0;

	// 查询主播、频道的观看直播人数上限
	virtual void queryComperePrivilege(UINT32 appId, UINT32 uid, UINT32 curSid) = 0;

	typedef _def_boost_signals2_signal_type<void (ComperePrivilege)>::type _SIG_COMPERE_PRIVILEGE;
	virtual _SIG_COMPERE_PRIVILEGE* getComperePrivilegeSignal() = 0;

	// 主播违规通知（各业务自己处罚）
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_COMPERE_PUNISHED;
	virtual _SIG_COMPERE_PUNISHED* getComperePunishedSignal() = 0;

	/**
	* 提供给开发平台的举报入口
	* @param appid 业务id
	* @param owId OW的UID
	* @param uidImages 被举报的用户证据（可以举报多个用户，每个用户可以有多张图片）
	*/
	virtual void reportWithUserData(UINT32 appId, UINT32 owId, const std::list<UserImage>& userImages) = 0;

    // 主播违规所在频道禁播（广播各业务）
    typedef _def_boost_signals2_signal_type<void (const QString&)>::type _SIG_CHANNEL_VIDEO_BAN;
    virtual _SIG_CHANNEL_VIDEO_BAN* getChannelVideoBanSignal() = 0;

	/**
	* 提供给各业务的举报入口
	* @param appid 业务id
	* @param compereId 被举报的uid
	* @param uids 有视频的用户id（可能有多个，比如多人视频），这里包含主播uid
	* @param showPageIndex 显示页的索引，0表示显示视频页，1表示语音页.
	*/
	virtual void report2(UINT32 appId, UINT32 compereId, const QList<UINT32>& uids,INT32 showPageIndex) = 0;

	/**
	* 提供给各业务的举报入口
	* @param appid 业务id
	* @param compereId 被举报的uid
	* @param uids 有视频的用户id（可能有多个，比如多人视频），这里包含主播uid
	* @param names uid对应的昵称map列表.
	* @param showPageIndex 显示页的索引，0表示显示视频页，1表示语音页.
	*/
	virtual void report3(UINT32 appId, UINT32 compereId, const QList<UINT32>& uids, const QMap<UINT32,QString>& names,INT32 showPageIndex) = 0;
};