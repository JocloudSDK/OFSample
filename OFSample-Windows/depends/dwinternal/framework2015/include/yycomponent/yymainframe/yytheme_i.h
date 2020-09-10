#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief ����ӿ�, ͨ��Root->getServiceInterface<IYYTheme>(KSERVICEMainframe)��ȡ.
  */
DWDEFINE_INTERFACE(IYYTheme) : public IDWQComponent
{
/**
   * @brief ��ȡ������ɫ.
   * @return ������ɫֵ.
   */
	virtual QRgb getColor() = 0 ;
/**
   * @brief ��ȡ����.
   * @return �����ֵ.
   */
	virtual int getColorTheme() = 0;

	/*
	signal:
		void themeChanged();	������ȥget�µ�ֵ
	*/
};