#pragma once
#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

/* use for getObject() */
/**
* @brief .频道回显接口
*/
DWDEFINE_INTERFACE( IYYChannelEcho) : public IUnk
{
/**
* @brief .信号定义
	* @param .op，操作码，详细见channelEcho_i.h里CHANNEL_OP_ERR_CODE；res，返回码，RES_EAUTH为密码错误，详细见res_code.h
	*/
	typedef _def_boost_signals2_signal_type<void (int op, int res)>::type	_SIG_ECHO_OPERATION_RES;
	
/**
	* @brief .获取操作结果通知信号
	*/
	virtual _SIG_ECHO_OPERATION_RES* getEchoOperationSignal() = 0;

    /**
* @brief .信号定义
	* @param .op，操作码，详细见channelEcho_i.h里CHANNEL_OP_ERR_CODE；res，返回码，RES_EAUTH为密码错误，详细见res_code.h
    * @param .spEchoInfo, 额外信息, biz_consts.h预定义。目前CPROP_ErrorHandled表示外接处理该signal，客户端不再处理
	*/
    typedef _def_boost_signals2_signal_type<void (int op, int res, IPropBagWrapperPtr spEchoInfo)>::type _SIG_ECHO_OPERATION_EX_RES;
    virtual _SIG_ECHO_OPERATION_EX_RES* getEchoOperationExSignal() = 0;

	/**
	* @brief .信号定义
	* @param .isme true表示我自己被T/封
	* @param .spEchoInfo, 额外信息, biz_consts.h预定义。
	*                     CPROP_Name 频道名字
	*                     GUILDPROP_AdminNick 操作者昵称
	*                     GUILDPROP_AdminUid 操作者uid
	*                     LPROP_BanReason 理由
	*                     UPROP_Nick 被T/封的用户昵称
	*                     UPROP_Uid 被T/封的用户uid
	*                     LPROP_BanTime 时间，表示“x分钟不能进入频道”
	*/
	typedef _def_boost_signals2_signal_type<void (bool isme, IPropBagWrapperPtr spEchoInfo)>::type _SIG_ECHO_KICKOUT;
	virtual _SIG_ECHO_KICKOUT* getEchoKickoutSignal() = 0;

	/**
	* @brief .订阅/取消订阅用户进出大频道公屏提示
	* @param .subscribe true:订阅；false:取消订阅
	*/
	virtual void subscribeUserChangedPrompt(UINT32 subSid, bool subscribe) = 0;
};