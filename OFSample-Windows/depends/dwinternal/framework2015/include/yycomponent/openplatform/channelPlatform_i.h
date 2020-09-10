#pragma once

#include "biz/biz_types.h"

DWDEFINE_INTERFACE(IChannelPlatform) : IUnk
{
	//virtual bool sendAppSessionData(const std::string& data) = 0; //obsoleted
	virtual bool sendPluginStatus(UINT32 pluginVersion) = 0;
	virtual UINT32 getAppTypeId() = 0;
	virtual UINT32 getPluginVersion() = 0;
	virtual UINT32 getForceUpdateVersion() = 0; 
	virtual std::wstring getPluginMD5() = 0; 
	virtual std::wstring getAppName() = 0;
	virtual std::wstring getPluginURL() = 0;
	virtual bool sendAppUserData(UINT32 appTypeId, const std::string& appUsrData) = 0;
	//这个接口不保证多线安全，业务应用调之个接口之前，先把线程抛回主线程，否则会崩.
	virtual bool sendAppVideoData(UINT32 appTypeId, const std::string& appData) = 0;

	//for service transparent tunnel
	virtual UINT32 getServiceConnection(TRANSPSVC_T svcType, bool bLongConnection) = 0;
	virtual bool sendServiceConnectionData(TRANSPSVC_T svcType, const std::string & msg, UINT32 connId) = 0;
	virtual bool closeServiceConnection(TRANSPSVC_T svcType, UINT32 connId) = 0;

public: //Sig
	typedef _def_boost_signals2_signal_type<void (const CHANNEL_PLATFORM_APP_INFO& appInfo)>::type	_SIG_ON_ADD_APP;
	virtual _SIG_ON_ADD_APP *getSigOnAddApp() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 appId, const std::wstring& appName, bool opByOw)>::type	_SIG_ON_REMOVE_APP;
	virtual _SIG_ON_REMOVE_APP *getSigOnRemoveApp() = 0;
	typedef _def_boost_signals2_signal_type<void (const yyplugin::appMgr::PluginInfoAdd& info)>::type _SIG_ON_ADD_APP2;
	virtual _SIG_ON_ADD_APP2 *getSigOnAddApp2() = 0;
	typedef _def_boost_signals2_signal_type<void (const yyplugin::appMgr::PluginInfoDel& info)>::type _SIG_ON_REMOVE_APP2;
	virtual _SIG_ON_REMOVE_APP2 *getSigOnRemoveApp2() = 0;
	typedef _def_boost_signals2_signal_type<void (const std::string& rawData)>::type _SIG_ON_ADD_ANNOUNCE_APP;
	virtual _SIG_ON_ADD_ANNOUNCE_APP *getSigOnAnnounceAppMsg() = 0;

	//app info add by lsm
	typedef _def_boost_signals2_signal_type<void (const yyplugin::appMgr::PluginInfoAdd& info)>::type _SIG_ON_APP_PLATFORM_ADD;
	virtual _SIG_ON_APP_PLATFORM_ADD *getSigOnAddPlatformApp() = 0;
	typedef _def_boost_signals2_signal_type<void (const yyplugin::appMgr::PluginInfoDel& info)>::type _SIG_ON_APP_PLATFORM_REMOVE;
	virtual _SIG_ON_APP_PLATFORM_REMOVE *getSigOnRemovePlatformApp() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 appId, const std::string& appData)>::type	_SIG_ON_APP_PLATFORM_DOWN_LINK_DATA;
	virtual _SIG_ON_APP_PLATFORM_DOWN_LINK_DATA *getSigOnPlatformAppDownLinkData() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 appId, const std::string& appData)>::type	_SIG_ON_APP_DOWN_LINK_DATA;
	virtual _SIG_ON_APP_DOWN_LINK_DATA *getSigOnAppDownLinkData() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 fromUid, const std::string& msg)>::type	_SIG_ON_CUSTOM_MESSAGE;
	virtual _SIG_ON_CUSTOM_MESSAGE *getSigOnCustomMessage() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SERVICE_READY;
	virtual _SIG_SERVICE_READY *getSigServiceReady() = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32 appId, const std::string& appData)>::type	_SIG_ON_APP_VIDEO_DATA;
	virtual _SIG_ON_APP_VIDEO_DATA *getSigOnAppVideoData() = 0;

	//signals for service transparent tunnel
	typedef _def_boost_signals2_signal_type<void (TRANSPSVC_T svcType, TRANSPSVC_STATUS status, UINT32 connId)>::type _SIG_ON_SERVICE_CONN_STATUS;
	virtual _SIG_ON_SERVICE_CONN_STATUS *getSigOnServiceConnStatus() = 0;
	typedef _def_boost_signals2_signal_type<void (TRANSPSVC_T svcType, const std::string& msg, UINT32 connId)>::type _SIGN_ON_SERVICE_CONN_DATA;
	virtual _SIGN_ON_SERVICE_CONN_DATA *getSigOnServiceConnData() = 0;

public:
	virtual bool joinGroup(TRANSPSVC_T svcType, const AppGroupId& groupId) = 0;
	virtual bool leaveGroup(TRANSPSVC_T svcType, const AppGroupId& groupId) = 0;

	//signals for service_by_pass
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_ON_BYPASS_SERVICE_TRANS_READY;
	virtual _SIG_ON_BYPASS_SERVICE_TRANS_READY *getSigOnBypassServiceTransReady() = 0;
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_ON_BYPASS_SERVICE_TRANS_ERROR;
	virtual _SIG_ON_BYPASS_SERVICE_TRANS_ERROR *getSigOnBypassServiceTransError() = 0;
	typedef _def_boost_signals2_signal_type<void (TRANSPSVC_T svcType, const std::string& data)>::type _SIG_ON_BYPASS_SERVICE_TRANS_DATA;
	virtual _SIG_ON_BYPASS_SERVICE_TRANS_DATA  *getSigOnByPassServiceTransData() = 0;


	//send Data through service_by_pass
	virtual void sendAppDatabyPassSvc(TRANSPSVC_T type, const std::string& data) = 0;
};
