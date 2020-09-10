#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
  * @brief not used yet, ask He Hao.
*/
DWDEFINE_INTERFACE(IYYLinkHandler) : public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void handleLink(const std::wstring &linkKey, const std::wstring &linkValue) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool handleWebJumpChannel(const std::wstring& linkKey, const std::wstring& linkValue) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool handleUrlLink( const std::wstring& linkKey, const std::wstring& linkValue ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool handleYYLink( const std::wstring& linkKey, const std::wstring& linkValue ) = 0;
    /**
   * @brief .可分别处理yy://以及http://
   * @param .needLogin：是否带登录带
   * @return .
   */
	virtual void openLink( const QString& urlLink , bool needLogin ) = 0;

};

