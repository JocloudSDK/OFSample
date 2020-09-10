#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#define KDefaultSkinPageName "normalSkin"
#define KVipSkinPageName     "vipSkin"
/**
  * @brief 主题切换, 通过Root->getServiceInterface<IYYThemeColorFrame>(KSERVICEMainframe)获取.
  */
class DuiWidget;
class QPoint;
class DuiFrameWindow;

DWDEFINE_INTERFACE(IYYThemeColorFrame) : public IDWQComponent
{
	/**
   * @brief 显示皮肤配置窗口.activePageName是当前处于active状态的页,传空则只是显示皮肤窗口
   * @return 无.
   */
	virtual void showThemeColorFrame( const std::wstring& activePageName ) = 0;

	/**
   * @brief 设置自定义皮肤按钮, button 由组件释放
   * @return 无.
   */
	virtual bool setCustomizeButton( DuiWidget* button ) = 0;

	/**
   * @brief 返回皮肤设置窗口入口widget
   * @return 无.
   */
	virtual DuiWidget* entranceWidget() = 0;

	/**
   * @brief 获取皮肤窗口,如果当前没有打开皮肤窗口，会返回NULL
   * @return DuiFrameWindow*.
   */
	virtual DuiFrameWindow* getThemeColorFrame(void) = 0;
};