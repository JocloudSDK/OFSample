#pragma once
#include "devicesetting_i.h"
#include "videoinfo.h"

DWDEFINE_INTERFACE(IYYP2PVideoDataReoprt) : public IDWComponent
{
	enum stopChatType
	{
		user_reject = 1, //接收方拒绝
		user_cancel = 2, //发起方取消
		network_error = 3, //网络错误
		user_no_respone = 4,//接收方没应答
		user_stop = 5,//用户中止
	};

	enum TrasferType
	{
		use_punch = 1,
		use_server = 2,
	};

	enum ServerType
	{
		typeUnknown,
		typePunch,
		typeBridge,
		typeLinkd,
	};

	enum AppId
	{
		AppIm1V1 = 0,
		AppVideoMagicalbox = 1,
	};

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initParam(const std::string &guid,unsigned int peerId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void startVideo(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stopVideo(unsigned int duration) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void resetReport(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setStopVideoChatType(stopChatType type) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initAppId(AppId appId) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void reportHaveCamera(void);
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void reportVideoChatData(unsigned int duration);
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void reoprtResolutionDuartion();
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void reportChannelInfo(short serverNum, bool bPunch,ImP2PVideoInfo::eVideoChanMethod chanMethod,short serverhots);
};