#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IImGroupCommon) : public IDWQComponent
{
	//// ��ʾ�����£�������Ϣʧ�ܵ���ʾ
	//virtual void showOfflineSendMsgErrorTips(YYNoticeTipWidget * noticeTip); 

	//// ��ʾ�����£�������Ϣʧ�ܵ���ʾ
	//virtual void showOfflineSendFileErrorTips(YYNoticeTipWidget * noticeTip); 
};