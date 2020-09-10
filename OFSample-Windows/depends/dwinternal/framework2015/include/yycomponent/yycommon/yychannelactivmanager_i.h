#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief Channel activity manager interface, ͨ��root->getServiceInterface<IChannelActivManager>(KSERVICECommon)����ȡ.
  */
DWDEFINE_INTERFACE(IChannelActivManager) : public IDWQComponent
{ 
/**
   * @brief �첽��ȡƵ�����Ϣ��ͨ��downloadResult�źŷ���.
   * @param uActivID activity id.
   * @param guid guid.
   */
	virtual void getActivityInfo(UINT32 uActivID, const std::wstring& guid) = 0;

/**
   * @brief �첽�����μӻ��ͨ��registrationResult�źŷ���.
   * @param uActivID activity id.
   */
	virtual void registration(UINT32 uActivID) = 0;

/**
   * @brief ��ȡ�����ں������촰������ʾ�ĺ���ͼƬ·��.
   * @param uActivID activity id.
   * @param strpath path.
   * @return .
   */
	virtual bool getLocalChatImgFile(UINT32 uActivID, std::wstring& strPath) = 0; 

/**
   * @brief �첽��ȡĳ��Ƶ�������ϣ�ͨ��getChannelInfoResult����.
   * @param uChannelID channel id.
   */
	virtual void getChannelInfo(UINT32 uChannelID) = 0; 
};