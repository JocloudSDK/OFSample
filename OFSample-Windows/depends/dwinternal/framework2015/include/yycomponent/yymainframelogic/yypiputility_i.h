#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief ���л��ӿ�,ͨ��Root->getServiceInterface<IYYPipUtility>(KSERVICEMainframe)��ȡ.
  */
DWDEFINE_INTERFACE(IYYPipUtility) : public IDWQComponent
{
/**
   * @brief .
   * @return .
   */
	virtual void doGotoOpenID() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void doRequesOpenID(HWND hwnd, const char* buf, int len, HWND nativeHwnd) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void doSetPipLoader(bool used) = 0;
/*
signals:
	void gotoOpenID();
	void requesOpenID(HWND, const char*, int, HWND);
	void setPipLoader(bool);
*/
};