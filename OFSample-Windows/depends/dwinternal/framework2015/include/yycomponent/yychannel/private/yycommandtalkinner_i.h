#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE(IYYCommandTalkInner) : public IUnk
{
	/*
	* @brief ֱ���˳�YYʱ����鲢�˳�ָ��ͨ����ʱƵ��
	*/
	virtual void cmdTalkTmpChlLeave() = 0;
};
