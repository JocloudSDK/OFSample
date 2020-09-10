// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stable.h"
#include "perflog/perflog.h"
#include "perflog/control.h"
#include "perflog/sampling/toolexchangecenter.h"
#include "prefetch/YY_events.h"

const char KPerfStarup[] = "startup";

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	DW_UNUSED(hModule);
	DW_UNUSED(lpReserved);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			Perf::CControl::GetInstance(); //触发初始化
			Perf::StartPerfLog();
			Perf::Stub(Perf::TYPE_E_BEGIN, KPerfStarup);
			Perf::Stub(Perf::TYPE_E_BEGIN, "startup.init");

			Perf::MultiPerfCalc();
			Perf::MultiPerfInit();

			/// add for ETW Trace
			EventRegisterDW_YY();
			EventWriteyy_startup_begin();
			EventUnregisterDW_YY();
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		//Perf::CPerfRecord::GetInstance()->EndPerfLog();
		//Perf::MultiPerfUnInit();
		break;
	}
	return TRUE;
}

