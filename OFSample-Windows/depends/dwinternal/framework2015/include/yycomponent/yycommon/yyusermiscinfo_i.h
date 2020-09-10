#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/

//ROLE_IN_CHANNEL��Ҫ����biz��ͷ�ļ�����ô�죿
DWDEFINE_INTERFACE(IYYUserMiscInfo): public IUnk
{
	/**
	* @brief ͨ��YY�ȼ����leveliconͼƬindex.
	* @param level YY�ȼ�.
	* @return leveliconͼƬindex.
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
