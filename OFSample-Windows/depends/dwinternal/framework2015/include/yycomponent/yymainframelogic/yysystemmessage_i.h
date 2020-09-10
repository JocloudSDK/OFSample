#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

class QString;
/**
  * @brief 系统消息接口,通过Root->getServiceInterface<IYYSystemMessage>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYSystemMessage): public IUnk
{
    typedef _def_boost_signals2_signal_type<void ( NORMAL_SYSTEM_MESSAGE *normalMessage )>::type    _SIG_NORMAL_SYSTEM_MESSAGE;
/**
  * @brief get normal message signal.
  */
    virtual _SIG_NORMAL_SYSTEM_MESSAGE*  getSigNormalMessage() = 0;

	virtual void sendPopMessageNotify(const UINT32 toid, const QString &msg) = 0;

	// 粉丝邀请弹窗信号
	typedef _def_boost_signals2_signal_type<void ( UINT32 uid, UINT32 uri, const std::wstring &packet )>::type    _SIG_FOLLOW_MESSAGE;
	virtual _SIG_FOLLOW_MESSAGE*  getSigFollowMessage() = 0;  
};

