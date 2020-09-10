#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "duifw/duiview.h"
/**
  * @brief 主面板状态接口, 通过Root->getServiceInterface<IYYMainFrameState>(KSERVICEMainframe)获取.
*/

DWDEFINE_INTERFACE(IYYMainFrameState) : public IDWQComponent
{
	virtual bool isMainFrameFocus() = 0;//主窗口是否是激活状态（是否置前）
	virtual bool isVisible() = 0;
	virtual bool isMinimized() = 0;
	virtual bool isMaximized() = 0;

	// 从靠边隐藏状态弹出显示
	// isMouseLeaveTrigger - 界面展开后，鼠标离开界面访问是否会触发隐藏
	// 目前用快捷键或双击任务栏YY图标呼出界面时需要传递false
	virtual bool showFromEdge(bool isMouseLeaveTrigger) = 0;
	virtual bool hideToEdge() = 0; 

	virtual bool addView(DuiView* view) = 0;
	virtual bool isHideInEdge() = 0;		// 是否处于贴边状态（贴边隐藏与贴边显示都算）
	virtual bool isCloseToEdge() = 0;	// 是否处于贴边显示状态
	virtual bool isHideToEdge() = 0;		// 是否处于贴边隐藏状态

	virtual QRect realFrameGeometry() = 0;//返回窗口大小

	virtual void setChildBlockDock(bool block) = 0;	// 是否处于子窗体呼出状态，暂时屏蔽dock效果

	virtual bool isLargeModel() = 0; // 是否新面板模式

	virtual bool openMiniWonderWorldTab(const QString &joinInfo) = 0; // 在新面板导航页面跳转指定tab页

	virtual quint64 getLoginTick() = 0; // 获取登录成功时间
	virtual void switchMainFrame(QWidget *mainframe) = 0; // 切主面板,获取mainframe
};
