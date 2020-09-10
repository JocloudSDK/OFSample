#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiSpliterDecorator;
DWDEFINE_INTERFACE(IChannelSpliterDecorator) : public IUnk 
{
/**
   * @brief .设置分割先的外观
   * @param .decorator，外观修饰器
   * @return .无
   */
     virtual void setChildDecorator(DuiSpliterDecorator* decorator) = 0;     
};
DWBIND_INTERFACE(DuiSpliterDecorator, DWMAKE_INTERFACE_UUID_NAME(DuiSpliterDecorator))