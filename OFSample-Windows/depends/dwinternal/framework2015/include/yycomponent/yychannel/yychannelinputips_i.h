#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"


class DuiWidget;

/**
signals:
    void tipsVisibleRequest(bool visible)   // 通知使用方，要求tips的widget(及其容器)的visible变化
*/

/**
* @brief 代表输入框提示的接口
*/
DWDEFINE_INTERFACE(IChannelInputTips) : public IDWQComponent 
{
   /**
   * @brief invoke函数.
   * @param method，要调用的函数；inParam，输入参数；outParam，输出参数.
   * @return 调用返回码，见InvokeCode.
   */
	virtual int invoke(const QString& method, InvokeParam* inParam, InvokeParam* outParam) = 0;

    /**
    * @brief 获取提示框widget
    * @param 
    * @return .
    */
	virtual DuiWidget* widget() = 0;

    /**
    * @brief 该函数用于配置提示框样式
    * @param json字符串,json格式为{\"time\":interval}
    * @return .
    */
    virtual void setControl(const char* json) = 0;

	/**
	* @brief 
	* @param strTipsText
	* @param strTipsIconPath
	* @return .
	*/
	virtual void show(const QString& strTipsText, const QString& strTipsIconPath) = 0;
};
