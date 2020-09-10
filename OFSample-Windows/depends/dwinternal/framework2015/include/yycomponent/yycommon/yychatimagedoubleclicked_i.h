#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiTextEdit;
/**
  * @brief not used yet, ask He Hao.
*/
DWDEFINE_INTERFACE(IYYChatImageDoubleClicked) : public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void attachTextEdit( DuiTextEdit* textEdit ) = 0;
};
