#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/

/**
* @brief 组包，解包.
*/
DWDEFINE_INTERFACE(IYYChannelMarshal) : public IUnk
{
/**
   * @brief 解包.
   * @param data.
   * @param keyMap.
   */
	virtual void unmarshal( const std::string& data, std::map<UINT16, std::string>& keyMap) = 0;
};