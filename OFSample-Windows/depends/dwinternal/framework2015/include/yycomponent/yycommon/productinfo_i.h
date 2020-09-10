#pragma once

/**
  * @brief 设置产品信息接口, 通过Root->getServiceInterface<IProductInfo>(KSERVICECommon)获取.
  */
DWDEFINE_INTERFACE(IProductInfo) : public IUnk
{
/**
   * @brief 设置版本号.
   * @param version 版本号.
   */
	virtual void setClientVersion(const std::string& version) = 0;

/**
   * @brief 设置语言id.
   * @param lcid 语言id.
   */
	virtual void setLanguageId(UINT lcid) = 0;
};
