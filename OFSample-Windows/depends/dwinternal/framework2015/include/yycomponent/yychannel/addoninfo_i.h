#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/yyaddoninfo.h"

namespace yyplugin {  namespace appMgr { struct PluginInfoAdd; struct PluginInfoDel;}; }
class IAddonFilter;
/**
* @brief �����Ϣ.
*/
DWDEFINE_INTERFACE(IYYAddonInfo): public IUnk
{
/**
   * @brief ע����������.
   * @param filter��������.
   */
	virtual void registerFilter(IAddonFilter* filter) = 0;
/**
   * @brief ��ע����������.
   * @param filter��������.
   */
	virtual void unRegisterFilter(IAddonFilter* filter) = 0;
/**
   * @brief ��ӹ�����.
   * @param id�����id.
   * @param type���������.
   * @param from��see IAddonFilter::eFrom
   * @return ��IAddonFilter::eFilter.
   */
    virtual int  addonFilter(quint32 id, quint32 type, int from/* @see IAddonFilter::eFrom */) = 0;

/**
   * @brief ���Ӳ��.
   * @param info���������Ϣ.
   */
    virtual void addonAdd(const yyplugin::appMgr::PluginInfoAdd& info) = 0;
/**
   * @brief �Ƴ����.
   * @param info���������Ϣ.
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
   * @brief ���ӿ���ƽ̨���.
   * @param info���������Ϣ.
   */
	virtual void addPlatformApp(const yyplugin::appMgr::PluginInfoAdd& info) = 0;

	typedef _def_boost_signals2_signal_type<void ( const PAddAppReq &info )>::type	_SIG_PLATFORMAPP_ADDON_ADDED;
	virtual _SIG_PLATFORMAPP_ADDON_ADDED* getPlatformAddonAddedSignal() = 0;

    /**
     * @brief ���ͣ�executeType != 0x1��app���ʱ����
     */
    virtual _SIG_ADDON_ADDED* getAddonAdded2Signal() = 0;
};
