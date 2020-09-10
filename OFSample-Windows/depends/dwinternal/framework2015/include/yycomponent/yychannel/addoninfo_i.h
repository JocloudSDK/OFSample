#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/yyaddoninfo.h"

namespace yyplugin {  namespace appMgr { struct PluginInfoAdd; struct PluginInfoDel;}; }
class IAddonFilter;
/**
* @brief 插件信息.
*/
DWDEFINE_INTERFACE(IYYAddonInfo): public IUnk
{
/**
   * @brief 注册插件过滤器.
   * @param filter，过滤器.
   */
	virtual void registerFilter(IAddonFilter* filter) = 0;
/**
   * @brief 反注册插件过滤器.
   * @param filter，过滤器.
   */
	virtual void unRegisterFilter(IAddonFilter* filter) = 0;
/**
   * @brief 添加过滤器.
   * @param id，插件id.
   * @param type，插件类型.
   * @param from，see IAddonFilter::eFrom
   * @return 见IAddonFilter::eFilter.
   */
    virtual int  addonFilter(quint32 id, quint32 type, int from/* @see IAddonFilter::eFrom */) = 0;

/**
   * @brief 增加插件.
   * @param info，插件的信息.
   */
    virtual void addonAdd(const yyplugin::appMgr::PluginInfoAdd& info) = 0;
/**
   * @brief 移除插件.
   * @param info，插件的信息.
   */
    virtual void addonRemove(const yyplugin::appMgr::PluginInfoDel& info) = 0;

	typedef _def_boost_signals2_signal_type<void ( const AddonInfo &info )>::type	_SIG_OLDADDON_ADDED;
	virtual _SIG_OLDADDON_ADDED* getOldAddonAddedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ( quint32 addonId, const QString &name, bool opByOw )>::type	_SIG_OLDADDON_REMOVED;
	virtual _SIG_OLDADDON_REMOVED* getOldAddonRemovedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ( const PAddAppReq &info )>::type	_SIG_ADDON_ADDED;
	virtual _SIG_ADDON_ADDED* getAddonAddedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ( const PDelAppReq &info )>::type	_SIG_ADDON_REMOVEED;
	virtual _SIG_ADDON_REMOVEED* getAddonRemovedSignal() = 0;

	/**
   * @brief 增加开放平台插件.
   * @param info，插件的信息.
   */
	virtual void addPlatformApp(const yyplugin::appMgr::PluginInfoAdd& info) = 0;

	typedef _def_boost_signals2_signal_type<void ( const PAddAppReq &info )>::type	_SIG_PLATFORMAPP_ADDON_ADDED;
	virtual _SIG_PLATFORMAPP_ADDON_ADDED* getPlatformAddonAddedSignal() = 0;

    /**
     * @brief 类型（executeType != 0x1）app添加时触发
     */
    virtual _SIG_ADDON_ADDED* getAddonAdded2Signal() = 0;
};
