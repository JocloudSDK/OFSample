#pragma once

#include "dwbase/dwcomex.h"

/**
   * @brief 上麦名片设置接口.
   */
DWDEFINE_INTERFACE(IYYOnAirCardSettingFrameHelper) : public IDWQComponent
{
/**
   * @brief 显示上麦名片设置窗口.
   * @param parent parent widget.
   */
	virtual void showSettingFrame(QWidget *parent) = 0;
/**
   * @brief 获取上麦名片设置窗口.
   */
	virtual QWidget *settingFrame() const = 0;

//signals:
//	void settingFrameCreated();
};
