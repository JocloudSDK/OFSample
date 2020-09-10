#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IImGroupCommon) : public IDWQComponent
{
	//// 显示离线下，发送消息失败的提示
	//virtual void showOfflineSendMsgErrorTips(YYNoticeTipWidget * noticeTip); 

	//// 显示离线下，发送消息失败的提示
	//virtual void showOfflineSendFileErrorTips(YYNoticeTipWidget * noticeTip); 
};