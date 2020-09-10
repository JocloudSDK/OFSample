/** 
@file
@brief 采集控制选项
@version 2011-9-16 bobdeng
*/


#include "stable.h"
#include <windows.h>
#include <Shellapi.h>
#include <assert.h>
#include <tchar.h>
#include <sstream>
#include "control.h"
#include "apihook.h"
#include "toolexchangecenter.h"
#include "perflog.h"
#include "callstacksampling.h"
#include "miscsampling.h"
#include "wmisampling.h"

#include "wmi/comenviroment.h"

const std::wstring PerfRecvHwnd = _T("-PerfRecvHwnd=");
const std::wstring PerfLogFile = _T("-PerfLogFile=");

const int SamplingInterval = 30;
const int WMISamplingInterval = 1000;

namespace Perf
{

	static DWORD WINAPI ThreadProc(LPVOID)
	{
		int i = 0;

		while (1)
		{
			LONGLONG llPerfCount = 0;
			//::QueryPerformanceCounter((LARGE_INTEGER*)&llPerfCount);

			CCallStackSampling *pCallStackInst = CCallStackSampling::GetInstance();
			pCallStackInst->CallStackSampling(llPerfCount);

			CMiscSampling *pMiscInst = CMiscSampling::GetInstance();
			pMiscInst->CpuSampling(llPerfCount);
			pMiscInst->MemSampling();

			::Sleep(SamplingInterval);
			i++;
		}

		return 0;
	}

	static DWORD WINAPI WMISamplingThread(LPVOID)
	{
		HRESULT hr = Perf::EnableComSupport();			// 处理返回值
		if (FAILED(hr))
		{
			// Close SamplingWMI Option??
			return hr;
		}
		
		CWmiSampling* pWmiInst = CWmiSampling::GetInstance();

		while (1)
		{
			pWmiInst->DoHiPerfSampling();
			::Sleep(WMISamplingInterval);
		}
		
		return 0;
	}

	static void InitSamplingThread()
	{
		static bool s_bFirst = true;
		if (s_bFirst)
		{	//只创建一次我主手还没有武器
			s_bFirst = false;		

			DWORD dwThreadId = 0;
 
 			HANDLE hThread = (HANDLE)::CreateThread(NULL,
 				0,
 				&WMISamplingThread,
 				0,
 				CREATE_SUSPENDED,
 				&dwThreadId);
 			::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
 			::ResumeThread(hThread);
 			::CloseHandle(hThread);
			
			hThread = (HANDLE)::CreateThread(NULL,
				0,
				&ThreadProc,
				0,
				CREATE_SUSPENDED,
				&dwThreadId);
			assert(hThread);
			::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
			::ResumeThread(hThread);
			::CloseHandle(hThread);
		}

		return;
	}

	CControl *CControl::GetInstance()
	{
		static CControl *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CControl), TRUE)) CControl();

		return s_pImpl;
	}

	CControl::CControl()
	{
		::InitializeCriticalSection(&m_cri);

//#ifdef OFFICIAL_BUILD
//		m_bSamplingCallStack = FALSE;
//		m_bSamplingResponse = FALSE;
//		m_bSamplingCPUMem = FALSE;
//		m_dwSamplingLogLevel = 0;
//		SetAPIHook(FALSE);
//#else
		m_bSamplingCallStack = FALSE;
		m_bSamplingResponse = TRUE;
		m_bSamplingCPUMem = FALSE;
		m_bSamplingWMI = FALSE;
		m_bEnablePerfStub = TRUE;
		m_bEnableMultiPerlStub = FALSE;
		m_dwSamplingLogLevel = 0;
		SetAPIHook(FALSE);
//#endif

		//处理命令行选项
		LPCTSTR pCommandLine = GetCommandLine();
		if (wcsstr(pCommandLine, _T("/perftool")) != NULL)
		{
			OpenFullOption();
		}

		PaserCommandLine(pCommandLine);
	}

	std::wstring CControl::GetValue(const std::wstring& strOption, const std::wstring& strKey)
	{
		std::wstring strValue;

		std::size_t first = strOption.find(strKey);
		if( first != std::wstring::npos )
 		{
			first += strKey.size();
			strValue = strOption.substr(first);
 		}
		return strValue;
	}

	void CControl::PaserCommandLine(LPCTSTR pCommandLine)
	{
		if( !pCommandLine )
			return;

		LPWSTR *szArglist = NULL;  
		int nArgs = 0;  
		szArglist = CommandLineToArgvW(pCommandLine, &nArgs);  
		if( NULL == szArglist)  
			return;

		for(int i=0; i<nArgs; ++i)
		{
			std::wstring strValue = GetValue(szArglist[i],PerfRecvHwnd);
			if( !strValue.empty() )
			{
				std::wstringstream   ss;
				DWORD hwnd;
				ss << strValue;
				ss >> hwnd;
				CToolExchangeCenter::GetInstance()->RegisterWnd( (HWND)hwnd );
				continue;
			}

			strValue = GetValue(szArglist[i],PerfLogFile);
			if( !strValue.empty() )
				CToolExchangeCenter::GetInstance()->SetPerfLogFile( strValue.c_str() );
		}
		LocalFree(szArglist);
	}

	void CControl::OpenFullOption()
	{
		InitSamplingThread();	//这里才创建线程

		m_bSamplingCallStack = TRUE;
		m_bSamplingResponse = TRUE;
		m_bSamplingCPUMem = TRUE;
		m_bSamplingWMI = TRUE;
		m_dwSamplingLogLevel = 0xFFFF;
		m_bEnableMultiPerlStub = TRUE;
		SetAPIHook(TRUE);
		
	}

	void CControl::CloseAllOption()
	{
		m_bSamplingCallStack = FALSE;
		//m_bSamplingResponse = FALSE;
		m_bSamplingCPUMem = FALSE;
		m_bSamplingWMI = FALSE;
		m_dwSamplingLogLevel = 0;
		SetAPIHook(FALSE);
	}

	void CControl::SetAPIHook(BOOL bEnable)
	{
		CAPIHook::GetInstance()->SetAPIHook(bEnable);
	}

	BOOL CControl::IsAPIHook()
	{
		return CAPIHook::GetInstance()->GetAPIHook();
	}
}
