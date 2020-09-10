#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yyimkeywordfilter_i.h"

/**
  * @brief ������Ϣ�ӿ�, ͨ��root->getServiceInterface<IChatMessage>(KSERVICECommon)����ȡ.
  */
DWDEFINE_INTERFACE(IChatMessage) : public IUnk
{
/**
   * @brief . �õ�IM�ؼ��ֹ��˽ӿڡ�
   * @param .
   * @return.
   */
	virtual IYYImKeywordFilterPtr getImKeywordFilter() = 0;
/**
   * @brief . �õ�Ⱥ�ؼ��ֹ��˽ӿ�
   * @param .
   * @return.
   */
	virtual IYYImKeywordFilterPtr getGroupKeywordFilter() = 0;
};