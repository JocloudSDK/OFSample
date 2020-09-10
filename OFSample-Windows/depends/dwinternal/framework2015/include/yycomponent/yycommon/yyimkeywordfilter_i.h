#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYImKeywordFilter) : public IUnk
{
	enum FILTER_RESULT
	{
		FILTER_NO_INCLUDE		= 0,    // 正常发送
		FILTER_FORBID_SUBMIT	= 1, // 不允许发送
		FILTER_REPLACE_SEND		= 2  // 允许发送，但其中的关键字已经被替换了
	};

/**
   * @brief 是否过滤内容.
   * @param message 输入消息内容.
   * @param param 输入消息内容参数.
   * @return 过滤类型.
   */
	virtual FILTER_RESULT filter( QString& message, const QString& param ) = 0;
};