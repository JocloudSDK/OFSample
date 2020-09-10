#pragma once
#include "dwbase/dwcomex.h"

/**
* @brief 组件属性配置接口，通过root->getServiceInterface<IDWComponentConfigure>(KSERVICECommonComponentLevel)来获取.
*/
DWDEFINE_INTERFACE(IDWComponentConfigure): public IUnk
{
	enum Position
	{
		MainFrame_AppTab,     //主界面应用TAB栏
		MainFrame_AppBox,	  //主界面应用管理器
	};

	/**
   * @brief . 设置组件应用在某个位置上显示状态
   * @param . pos 显示的位置(Position)
   * @param . visible 是否显示
   * @return. 无
   */
	virtual void setVisible( Position pos, bool visible ) = 0;

   /**
   * @brief . 返回组件应用在某个位置上是否显示
   * @param . pos 显示的位置(Position)
   * @return. 已在该位置上处于显示状态 true, 否则 false
   */
	virtual bool visible( Position pos ) = 0;

};