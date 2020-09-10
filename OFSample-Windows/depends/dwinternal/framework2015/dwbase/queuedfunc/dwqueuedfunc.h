#pragma once

#include "dwglobal.h"
#include "dwutility/dwasynccall.h"
#include "sysbase/tool/xsignals.h"

class DWQueuedFuncImpl;
class  DWBASE_EXPORTS DwQueuedFunc
{
	DwQueuedFunc();
	~DwQueuedFunc();

public:
	static DwQueuedFunc* instance();

public:
	typedef xsignals::signal<void ()> SIG_POLL;
	SIG_POLL& getSigPoll();
	void post(DWASYNCCALLPROC proc, void *param);
	void poll();
	void clear();
	
private:
	DWQueuedFuncImpl* m_pQueuedFuncImpl;
};