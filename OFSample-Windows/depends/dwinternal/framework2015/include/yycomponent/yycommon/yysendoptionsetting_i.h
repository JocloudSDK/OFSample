#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief not used yet, ask He Hao.
*/
DWDEFINE_INTERFACE(IYYSendOptionSetting) : public IUnk
{
	enum SendFlag
	{
		Enter = 0,
		CtrlEnter = 1
	};

	/**
	* @brief ���÷��Ͳ�����ݼ�.
	* @param flag ��ݼ�����.
	*/
	virtual void		setSendFlag( SendFlag flag ) = 0;

	/**
	* @brief ���ط��Ͳ�����ݼ�.
	* @return ��ݼ�����.
	*/
	virtual SendFlag	sendFlag() = 0;
};