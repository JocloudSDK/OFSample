#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief Channel activity manager interface, 通过root->getServiceInterface<IChannelActivManager>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IChannelActivManager) : public IDWQComponent
{ 
/**
   * @brief 异步获取频道活动信息，通过downloadResult信号返回.
   * @param uActivID activity id.
   * @param guid guid.
   */
	virtual void getActivityInfo(UINT32 uActivID, const std::wstring& guid) = 0;

/**
   * @brief 异步报名参加活动，通过registrationResult信号返回.
   * @param uActivID activity id.
   */
	virtual void registration(UINT32 uActivID) = 0;

/**
   * @brief 获取本地在好友聊天窗口上显示的海报图片路径.
   * @param uActivID activity id.
   * @param strpath path.
   * @return .
   */
	virtual bool getLocalChatImgFile(UINT32 uActivID, std::wstring& strPath) = 0; 

/**
   * @brief 异步获取某个频道的资料，通过getChannelInfoResult返回.
   * @param uChannelID channel id.
   */
	virtual void getChannelInfo(UINT32 uChannelID) = 0; 
};