#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duiframewindow.h"

/**
  * @brief 通过root->getServiceInterface<IYYCommonComponent>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IYYCommonComponent) : public IDWQComponent
{
/**
   * @brief . 打开消息管理器
   * @param . 无
   * @return. 无
   */
	virtual void OpenHistoryMsgFrame() = 0;

	/**
   * @brief . 打开应用
   * @param . 无
   * @return. 无
   */
	virtual int startComponent( const char* name, const char* args ) = 0;

    typedef _def_boost_signals2_signal_type< void (const std::string&, const std::vector<UINT32>&) >::type	_SIG_GET_COMPONENTINFO_RES;
    virtual _SIG_GET_COMPONENTINFO_RES* getSigGetComponentInfoRes() = 0;

    typedef _def_boost_signals2_signal_type< void (const std::string&) >::type    _SIG_PUSH_COMPONENT_INFO;
    virtual _SIG_PUSH_COMPONENT_INFO* getSigPushComponentInfo() = 0;

    typedef _def_boost_signals2_signal_type< void (const std::string&, const std::string&, UINT32, UINT8, UINT32) >::type	_SIG_GET_COMPONENTINFOEX_RES;
    virtual _SIG_GET_COMPONENTINFOEX_RES* getSigGetComponentInfoExRes() = 0;

    typedef _def_boost_signals2_signal_type<void (const UINT32&, const UINT32&, UINT32, UINT8, UINT32)>::type _SIG_DOWNLOAD_COMPONENTEX_RES;
    virtual _SIG_DOWNLOAD_COMPONENTEX_RES* getSigDownloadComponentExRes() = 0;

//signals:
//	void historyMsgServiceLoaded();	
};