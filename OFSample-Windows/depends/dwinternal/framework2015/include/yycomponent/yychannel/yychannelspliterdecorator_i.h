#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiSpliterDecorator;
DWDEFINE_INTERFACE(IChannelSpliterDecorator) : public IUnk 
{
/**
   * @brief .���÷ָ��ȵ����
   * @param .decorator�����������
   * @return .��
   */
     virtual void setChildDecorator(DuiSpliterDecorator* decorator) = 0;     
};
DWBIND_INTERFACE(DuiSpliterDecorator, DWMAKE_INTERFACE_UUID_NAME(DuiSpliterDecorator))