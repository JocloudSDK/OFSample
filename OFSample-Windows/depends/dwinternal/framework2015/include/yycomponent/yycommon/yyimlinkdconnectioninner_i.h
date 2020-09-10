#pragma once

#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏
#include "yyimlinkdconnection_i.h"

DWDEFINE_INTERFACE(IYYImLinkdConnectionInner): public IUnk
{
	static unsigned int generateSeq() {static unsigned int s_seq = 0;return s_seq++;}

	enum TailLightPos
	{
		MiniCard = 1,
		InfoPage = 2,
		Bulletin = 4,
		ChannelUserList = 8,
		ChannelMaixuList = 16,
		IMUserList = 32,
		IMFrame = 64,
		MiniCardSignature = 128,
	};
	virtual void loginAppServer() = 0;

	/**
	* @brief 向组件服务器拉取尾灯信息.
	* @param pos 尾灯位置，uids：获取哪些用户的在pos的尾灯信息.
	* @return .
	*/
	virtual void getTailLightData(TailLightPos pos, unsigned int seq, unsigned int languageId, const std::vector<unsigned int> &uids) = 0;
	
	/**
	* @brief 向组件服务器拉取尾灯信息.
	* @param appId 有些组件在触发拉取尾灯信息时，其组件信息还不存在，没法得到对应的comroot，所以暴露一个ex接口，让外界传id.
	* @param pos 尾灯位置，uids：获取哪些用户的在pos的尾灯信息.
	* @return .
	*/
	virtual void getTailLightDataEx(unsigned int appId, TailLightPos pos, unsigned int seq, unsigned int languageId, const std::vector<unsigned int> &uids) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int, const std::string&, unsigned int uUri, const std::string &strPacket)>::type	_SIG_PLATFORM_DATA_ARRIVE;
	virtual _SIG_PLATFORM_DATA_ARRIVE* getPlatformDataArrivedSignal() = 0;
};
#endif	//DW_COMPONENT_ENV
