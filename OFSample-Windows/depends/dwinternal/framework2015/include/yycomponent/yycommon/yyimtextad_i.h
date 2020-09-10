#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

struct TEXTAD_INFO
{
	int nIndex;

	std::wstring strText;
	std::wstring strUrl;

	TEXTAD_INFO() : nIndex(-1)
	{

	}
};


/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
  ÒÑ·ÏÆú
*/
DWDEFINE_INTERFACE(IYYImTextAD) : public IDWQComponent
{
/**
   * @brief . ÒÑ·ÏÆú
   * @param .
   * @return .
   */
	virtual TEXTAD_INFO nextAd( int index ) = 0;

/*
signals:
    void adUpdated();
*/
};