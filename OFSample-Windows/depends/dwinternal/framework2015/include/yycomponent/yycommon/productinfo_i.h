#pragma once

/**
  * @brief ���ò�Ʒ��Ϣ�ӿ�, ͨ��Root->getServiceInterface<IProductInfo>(KSERVICECommon)��ȡ.
  */
DWDEFINE_INTERFACE(IProductInfo) : public IUnk
{
/**
   * @brief ���ð汾��.
   * @param version �汾��.
   */
	virtual void setClientVersion(const std::string& version) = 0;

/**
   * @brief ��������id.
   * @param lcid ����id.
   */
	virtual void setLanguageId(UINT lcid) = 0;
};
