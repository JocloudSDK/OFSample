#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

/**
* @brief YY config interface，通过root->getServiceInterface<IYYConfig>(KSERVICECommon)来获取.
*/
DWDEFINE_INTERFACE(IYYConfig) : public IUnk
{
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring queryInstallAppPath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/ 
	virtual std::wstring appRootPath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring appLayoutPath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring appWavePath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring queryBuildinAddonsPath(const std::wstring &addonName)=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring appDataPath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring webCachePath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring queryAddonsRootPath()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring queryPluginPath(const std::wstring &fileName)=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring queryLangPackPath(const std::wstring &fileName)=0;
	/**
	* @brief .常用获取版本信息接口，需要更多版本信息可联系YY开发了解
	* @param .
	*    key: ProductName 返回产品名称，如:YY
	*    key: ProductVersion 返回版本号title, 如: “8.45.0.1”版本号返回的是“8.45” 
	*    key: FullProductVersion 返回完整产品版本，如：“YY 8.45.0.1 (2018.10.25)”
	*    key: ProductVersionForStat 返回详细版本号，如：“8.45.0.1”
	* @return .
	*/
	virtual std::wstring getProductInfo(const std::wstring& key)=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual int	getPartnerID()=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual LOGFONT	defaultFont()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual LOGFONT	listDefaultFont()=0;

	//virtual YYSmile&	yysmile()=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool enableFontInfo()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool enableSetProcessWorkingSetSizeInfo()=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool isAddonManagerEnabled()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool isAddonEnabled(int addonId)=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool isDuiSpyEnabled()=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool isAccessibleEnabled()=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool isAcchelperEnabled()=0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool isPerfStubEnabled()=0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring appResourcePath() = 0;

	virtual void setPopupWindowDisabled(const bool disabled) = 0;
	virtual bool isPopupWindowDisabled() = 0;

	//protocol ver
	virtual void setProtocolVersion(UINT32 ver) = 0;

	//proxy
	virtual UINT32 asyncTestNetProxyServer(const NET_PROXY_INFO& info) = 0;
	virtual void cancelTestNetProxyServer(UINT32 testingId) = 0;
	virtual HRESULT setNetProxy(const NET_PROXY_INFO& info) = 0;
	virtual void setTestIsp(int nISPtype) ; //Net setTestIsp 

	//proxy's sig
	typedef _def_boost_signals2_signal_type<void (UINT32 testingId, const NET_PROXY_INFO& info, UINT32 resultCode)>::type	_SIG_TEST_NET_PROXY_SERVER_RESULT;
	virtual _SIG_TEST_NET_PROXY_SERVER_RESULT* getTestNetProxyServerResultSignal() = 0;
};

