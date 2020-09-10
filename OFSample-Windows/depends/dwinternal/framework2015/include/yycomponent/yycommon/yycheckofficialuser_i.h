#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYCheckOfficialUser): public IUnk
{
/**
   * @brief .���uid�Ƿ�Ϊ�ٷ�YY
   * @param .
   * @return .
   */
	virtual bool isOfficialUid(UINT32 uid) = 0;

/**
* @brief .���YY���Ƿ�Ϊ�ٷ�YY
* @param .
* @return .
*/
	virtual bool isOfficialYYNo(UINT32 yyno) = 0;

	/**
	* @brief .��uid�ж��Ƿ�YYԱ����
	* @param .
	* @return .
	*/
	virtual bool isYYStaff(UINT32 uid) = 0;
};