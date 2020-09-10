#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief 主题接口, 通过Root->getServiceInterface<IYYTheme>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYTheme) : public IDWQComponent
{
/**
   * @brief 获取主题颜色.
   * @return 主题颜色值.
   */
	virtual QRgb getColor() = 0 ;
/**
   * @brief 获取主题.
   * @return 主题的值.
   */
	virtual int getColorTheme() = 0;

	/*
	signal:
		void themeChanged();	请自行去get新的值
	*/
};