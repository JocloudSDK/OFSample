#pragma once

#include "dwglobal.h"
#include "sysbase\tool\tiny_bind.h"

typedef void (CALLBACK *DWASYNCCALLPROC)(void *param);

class DW_DWUTILITY_EXPORT DwAsyncCall
{
public:
	static void init();
	static void call(DWASYNCCALLPROC proc, void *param, DWORD threadId = 0);
	static void call(const tiny_function0<void> &f, DWORD threadId = 0);
	static void terminate();
};
