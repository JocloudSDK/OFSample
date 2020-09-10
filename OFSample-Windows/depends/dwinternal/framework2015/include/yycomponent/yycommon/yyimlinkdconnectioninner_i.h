#pragma once

#ifndef	DW_COMPONENT_ENV	//����������ᶨ�������
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
	* @brief �������������ȡβ����Ϣ.
	* @param pos β��λ�ã�uids����ȡ��Щ�û�����pos��β����Ϣ.
	* @return .
	*/
	virtual void getTailLightData(TailLightPos pos, unsigned int seq, unsigned int languageId, const std::vector<unsigned int> &uids) = 0;
	
	/**
	* @brief �������������ȡβ����Ϣ.
	* @param appId ��Щ����ڴ�����ȡβ����Ϣʱ���������Ϣ�������ڣ�û���õ���Ӧ��comroot�����Ա�¶һ��ex�ӿڣ�����紫id.
	* @param pos β��λ�ã�uids����ȡ��Щ�û�����pos��β����Ϣ.
	* @return .
	*/
	virtual void getTailLightDataEx(unsigned int appId, TailLightPos pos, unsigned int seq, unsigned int languageId, const std::vector<unsigned int> &uids) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int, const std::string&, unsigned int uUri, const std::string &strPacket)>::type	_SIG_PLATFORM_DATA_ARRIVE;
	virtual _SIG_PLATFORM_DATA_ARRIVE* getPlatformDataArrivedSignal() = 0;
};
#endif	//DW_COMPONENT_ENV
