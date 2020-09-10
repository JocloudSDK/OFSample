#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yyimkeywordfilter_i.h"

/**
  * @brief 聊天消息接口, 通过root->getServiceInterface<IChatMessage>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IChatMessage) : public IUnk
{
/**
   * @brief . 得到IM关键字过滤接口。
   * @param .
   * @return.
   */
	virtual IYYImKeywordFilterPtr getImKeywordFilter() = 0;
/**
   * @brief . 得到群关键字过滤接口
   * @param .
   * @return.
   */
	virtual IYYImKeywordFilterPtr getGroupKeywordFilter() = 0;
};