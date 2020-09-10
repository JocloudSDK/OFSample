#pragma once

#include "dwbase/dwcomex.h"
#include "yyproperty.h"

/**
  * @brief 频道精彩世界设置信息
  */
namespace wonderWorld
{
	const wchar_t InvalidShowTime[] = L"-1";
	const char TimeFormat[] = "hh:mm:ss";
	
}

DWDEFINE_INTERFACE( IYYWonderWorldInfo ) : public IUnk
{
public:
	enum Switch	
	{ 
		Off // READ:关闭的. WRITE:关闭.
		,On // READ:打开的. WRITE:开启.
	}; 

	typedef yyproperty::property<Switch> SwitchPro;
	typedef yyproperty::property<std::wstring> TimePro;
	typedef yyproperty::property<std::wstring> StringPro;
	
	/**
   * @brief 显示公屏文字消息, READ/WRITE. 
   * @return Switch
   */
	virtual SwitchPro& pro_publicText() = 0;


	/*
	* @brief 开播开始时间,由模板设置, READ/WRITE. 
	* @return std::wstring 格式: hh:mm:ss 例: 14:13:09 注意: 用wonderWorld::InvalidShowTime 表示不合法时间
	*/
	virtual TimePro& pro_showTime() = 0;
	
	
	/*
	* @brief 活动名称,由模板设置, READ/WRITE. 
	* @return std::wstring
	*/
	virtual StringPro& pro_activityName() = 0;

	/**
   * @brief 精彩世界底部控制条是否可见, READ/WRITE. 
   * @return Switch
   */
	virtual SwitchPro &pro_controlBarVisible() = 0;
};
