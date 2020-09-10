#pragma once

#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE(IYYFindCenterFrameWrap) : public IDWQComponent
{
	typedef xsignals::signal<void(QString&)> SIG_APPEND_PARAMS;
	/**
	*@brief ����ģ���url��Ҫ���һЩ�������������������                                                                        
	*/
	virtual SIG_APPEND_PARAMS* sigAppendParamsToLink() = 0;
};