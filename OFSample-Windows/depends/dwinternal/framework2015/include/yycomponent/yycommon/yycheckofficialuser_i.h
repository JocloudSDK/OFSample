#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYCheckOfficialUser): public IUnk
{
/**
   * @brief .检查uid是否为官方YY
   * @param .
   * @return .
   */
	virtual bool isOfficialUid(UINT32 uid) = 0;

/**
* @brief .检查YY号是否为官方YY
* @param .
* @return .
*/
	virtual bool isOfficialYYNo(UINT32 yyno) = 0;

	/**
	* @brief .从uid判断是否YY员工号
	* @param .
	* @return .
	*/
	virtual bool isYYStaff(UINT32 uid) = 0;
};