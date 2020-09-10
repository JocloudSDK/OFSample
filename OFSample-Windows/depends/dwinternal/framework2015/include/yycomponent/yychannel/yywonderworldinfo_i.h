#pragma once

#include "dwbase/dwcomex.h"
#include "yyproperty.h"

/**
  * @brief Ƶ����������������Ϣ
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
		Off // READ:�رյ�. WRITE:�ر�.
		,On // READ:�򿪵�. WRITE:����.
	}; 

	typedef yyproperty::property<Switch> SwitchPro;
	typedef yyproperty::property<std::wstring> TimePro;
	typedef yyproperty::property<std::wstring> StringPro;
	
	/**
   * @brief ��ʾ����������Ϣ, READ/WRITE. 
   * @return Switch
   */
	virtual SwitchPro& pro_publicText() = 0;


	/*
	* @brief ������ʼʱ��,��ģ������, READ/WRITE. 
	* @return std::wstring ��ʽ: hh:mm:ss ��: 14:13:09 ע��: ��wonderWorld::InvalidShowTime ��ʾ���Ϸ�ʱ��
	*/
	virtual TimePro& pro_showTime() = 0;
	
	
	/*
	* @brief �����,��ģ������, READ/WRITE. 
	* @return std::wstring
	*/
	virtual StringPro& pro_activityName() = 0;

	/**
   * @brief ��������ײ��������Ƿ�ɼ�, READ/WRITE. 
   * @return Switch
   */
	virtual SwitchPro &pro_controlBarVisible() = 0;
};
