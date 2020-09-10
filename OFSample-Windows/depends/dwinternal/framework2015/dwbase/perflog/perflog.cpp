/** 
@file
@brief PerfLog系统实现
@version huangyizhao
*/
#include "stable.h"
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <vector>
#include <map>
#include <time.h>
#include <intrin.h>
#include <assert.h>

#include "perflog.h"
#include "perflogimpl.h"
#include "callstacksampling.h"
#include "miscsampling.h"
#include "threadresponsetime.h"
#include "strtable.h"
#include "threadtable.h"
#include "control.h"
#include "apihook.h"
#include "logfile.h"
#include "toolexchangecenter.h"
bool g_bEnablePerfLog = false;

static class CPerfSaveLogic
{
public:
	CPerfSaveLogic(){}
	~CPerfSaveLogic()
	{
		//由于游戏大厅问题，部分机器登陆后析构没有被调用，这里有问题，先放在main函数
		//Perf::Stub(Perf::TYPE_E_END, "quitApplication");
		//Perf::EndPerfLog();
	}
}s_perfSave;

namespace Perf
{
	void Stub(E_LOG_TYPE eType, const char * lpszName, unsigned short wFlag)
	{
		unsigned short wNameID = AddToStrTable(lpszName);
		LARGE_INTEGER llPerfCount = {0};
		QueryPerformanceCounter(&llPerfCount);

		CPerfLogImpl::GetInstance()->Stub(eType, wNameID, wFlag, GetTickCount(), llPerfCount);
	}

	void CPerfRecord::StartPerfLog(const wchar_t* szFilePath)
	{
		if (szFilePath)
		{
			wcsncpy_s(m_szFilePath, MAX_PATH_LEN, szFilePath, _TRUNCATE);
		}

		LARGE_INTEGER llPerfCount = {0};
		QueryPerformanceCounter(&llPerfCount);
		m_llBeginPerfCount = llPerfCount.QuadPart;

		m_dwBeginTickcount = GetTickCount();

		//APIHook
		CAPIHook::GetInstance();
		Perf::CToolExchangeCenter::GetInstance();//触发初始化
	}

	void CPerfRecord::EndPerfLog()
	{
		////这里要先关闭，否则一边写文件一般写Log会出现错误
		//CControl::GetInstance()->CloseAllOption();

		LARGE_INTEGER llPerfCount = {0};
		QueryPerformanceCounter(&llPerfCount);
		m_llEndPerfCount = llPerfCount.QuadPart;

		SaveFile(m_szFilePath);
		m_szFilePath[0] = '/0';

		CToolExchangeCenter *pInst = CToolExchangeCenter::GetInstance();
		if (pInst)
		{
			pInst->SendReceiverQuit();
		}
	}

	void CPerfRecord::SetPerfLogFile(const wchar_t* szFilePathName)
	{
		wcsncpy_s(m_szFilePath, MAX_PATH_LEN, szFilePathName, _TRUNCATE);
	}

	DWBASE_API void __PerfLogBegin(unsigned short wLevel, const char *name, const char *flt)
	{
		if (wLevel > CControl::GetInstance()->GetSamplingLogLevel())
			return;
		unsigned short wStrID = AddToStrTable(name);
		unsigned short wFilterID = AddToStrTable(flt);

		Perf::Record(TYPE_E_BEGIN, wLevel, wStrID, wFilterID);
	}

	DWBASE_API void __PerfLogEnd(unsigned short wLevel, const char *name, const char *flt)
	{
		if (wLevel > CControl::GetInstance()->GetSamplingLogLevel())
			return;
		unsigned short wStrID = AddToStrTable(name);
		unsigned short wFilterID = AddToStrTable(flt);

		Perf::Record(TYPE_E_END, wLevel, wStrID, wFilterID);
	}

	DWBASE_API unsigned short AddToStrTable(const char * lpszName)
	{
		return CStrTable::GetInstance()->Add(lpszName);
	}

	DWBASE_API void SetThreadSampling(unsigned long dwThreadId, int nThreadSamplingType)
	{
		if (nThreadSamplingType & SAMPLING_E_CALLSTACK)
			CCallStackSampling::GetInstance()->AddThread(dwThreadId);

		if (nThreadSamplingType & SAMPLING_E_RESPONSE)
			CThreadResponseTime::GetInstance()->AddThread(dwThreadId);

		if (nThreadSamplingType & SAMPLING_E_CPU)
			CMiscSampling::GetInstance()->AddThread(dwThreadId);

		CAPIHook::GetInstance()->AddImportantThread(dwThreadId);
	}

	DWBASE_API void SetThreadName(unsigned long dwThreadId, const char * lpszThreadName)
	{
		CThreadTable::GetInstance()->SetThreadName(dwThreadId, lpszThreadName);
	}

	void Record(E_LOG_TYPE eType, unsigned short wLevel, unsigned short wNameID, unsigned short wFilterID)
	{
		CPerfLogImpl::GetInstance()->Record(eType, wLevel, wNameID, wFilterID);
	}

	void StartPerfLog(const wchar_t* szFilePath)
	{
		CPerfRecord::GetInstance()->StartPerfLog(szFilePath);
	}

	void EndPerfLog()
	{
		CPerfRecord::GetInstance()->EndPerfLog();
	}

	void SetPerfLogFile(const wchar_t* szFilePathName)
	{
		CPerfRecord::GetInstance()->SetPerfLogFile(szFilePathName);
	}

	unsigned long GetPerfLogLevel()
	{
		return CControl::GetInstance()->GetSamplingLogLevel();
	}

	void EnablePerfStub(BOOL bEnable)
	{
		return CControl::GetInstance()->EnablePerfStub(bEnable);
	}

	void GetStubs(std::vector<struct StubTag> &vecStubs)
	{
		return CPerfLogImpl::GetInstance()->GetStubs(vecStubs);
	}

	//取某个桩的平均耗时
	DWBASE_API WORD GetAverageStubTime(const char* strStub, DWORD dwAfterTickCount)
	{
		return CPerfLogImpl::GetInstance()->GetAverageStubTime(strStub, dwAfterTickCount);
	}

	DWBASE_API void MultiPerfInit()
	{
		getMultiPerfImpl()->init();
	}

	DWBASE_API void MultiPerfStub(const char* lpszName, E_LOG_TYPE type)
	{
		getMultiPerfImpl()->stub(lpszName, type);
	}

	DWBASE_API void MultiPerfCalc()
	{
		LPCTSTR pCommandLine = GetCommandLine();
		if (wcsstr(pCommandLine, _T("/makeperf")) != NULL)
		{
			Perf::YYMultiProcessPerf perf;
			perf.init();
			perf.calc();
		}
	}

	DWBASE_API void MultiPerfUnInit()
	{
		getMultiPerfImpl()->uninit();
	}

}
