#pragma once

#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE(IYYFindCenterFrameWrap) : public IDWQComponent
{
	typedef xsignals::signal<void(QString&)> SIG_APPEND_PARAMS;
	/**
	*@brief 发现模块的url需要添加一些特殊参数，可由组件添加                                                                        
	*/
	virtual SIG_APPEND_PARAMS* sigAppendParamsToLink() = 0;
};