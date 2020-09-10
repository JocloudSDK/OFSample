#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/

//ROLE_IN_CHANNEL需要依赖biz的头文件，怎么办？
DWDEFINE_INTERFACE(IYYUserMiscInfo): public IUnk
{
	/**
	* @brief 通过YY等级算出levelicon图片index.
	* @param level YY等级.
	* @return levelicon图片index.
	*/
	virtual int getLevelIconIndex(unsigned int level) = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual int iconIndexFromRole(ROLE_IN_CHANNEL role, GENDER gender, bool settle, bool isDistinguishCA) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::wstring getTranslateRole(ROLE_IN_CHANNEL role, bool isDistinguishCA) = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual QPixmap getIconFromLevel( quint32 level ) = 0;
};
