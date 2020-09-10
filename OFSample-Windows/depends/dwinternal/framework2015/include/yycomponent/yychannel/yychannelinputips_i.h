#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"


class DuiWidget;

/**
signals:
    void tipsVisibleRequest(bool visible)   // ֪ͨʹ�÷���Ҫ��tips��widget(��������)��visible�仯
*/

/**
* @brief �����������ʾ�Ľӿ�
*/
DWDEFINE_INTERFACE(IChannelInputTips) : public IDWQComponent 
{
   /**
   * @brief invoke����.
   * @param method��Ҫ���õĺ�����inParam�����������outParam���������.
   * @return ���÷����룬��InvokeCode.
   */
	virtual int invoke(const QString& method, InvokeParam* inParam, InvokeParam* outParam) = 0;

    /**
    * @brief ��ȡ��ʾ��widget
    * @param 
    * @return .
    */
	virtual DuiWidget* widget() = 0;

    /**
    * @brief �ú�������������ʾ����ʽ
    * @param json�ַ���,json��ʽΪ{\"time\":interval}
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
