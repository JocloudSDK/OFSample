/** 
@file
@brief 线程卡检测
@version 2011-8-3 bobdeng
*/

#include "stable.h"
#include "threadresponsetime.h"
#include "perflogimpl.h"
#include "..\detours\Detours.h"
#include <tchar.h>
#include <assert.h>
#include "control.h"
#include "threadtable.h"
#include "toolexchangecenter.h"
#include "dwbase\log.h"

const DWORD c_dwInterval = 1000;

static void **s_pDispatchMessage = (void**)::GetProcAddress(GetModuleHandle(_T("user32.dll")), "DispatchMessageW");
static void * & fnDispatchMessage = *s_pDispatchMessage;
DETOUR_TRAMPOLINE(LRESULT WINAPI Detour_DispatchMessage(const MSG *), fnDispatchMessage);

namespace Perf
{
	LRESULT WINAPI CThreadResponseTime::Tramp_DispatchMessage(const MSG *lpmsg)
	{
		CThreadResponseTime *pInst = CThreadResponseTime::GetInstance();

		::TlsSetValue(pInst->m_dwTlsDirty, 0);
		
		LARGE_INTEGER llPerfCountStart = {0};
		QueryPerformanceCounter(&llPerfCountStart);

		DWORD dwStartTime = ::GetTickCount();

		LRESULT lRet = Detour_DispatchMessage(lpmsg);

		BOOL bDirty = (BOOL)::TlsGetValue(pInst->m_dwTlsDirty);
		if (!bDirty && lpmsg && lpmsg->message != WM_NCLBUTTONDOWN) //拖动窗口消息属WM_NCLBUTTONDOWN，去除掉
		{
			DWORD dwCurTime = ::GetTickCount();
			if (dwCurTime - dwStartTime > c_dwInterval)
			{
				if (CControl::GetInstance()->IsSamplingResponse())
				{
					::EnterCriticalSection(&pInst->m_cri);
					BOOL bFind = pInst->m_setThread.find(::GetCurrentThreadId()) != pInst->m_setThread.end();
					::LeaveCriticalSection(&pInst->m_cri);

					if (bFind)
					{
						LARGE_INTEGER llPerfCountCur = {0};
						QueryPerformanceCounter(&llPerfCountCur);			
						
						CPerfLogImpl::GetInstance()->Stub(Perf::TYPE_E_BEGIN, pInst->m_wIdResponse, Perf::STUB_FLAG_CPU, dwStartTime, llPerfCountStart);
						CPerfLogImpl::GetInstance()->Stub(Perf::TYPE_E_END, pInst->m_wIdResponse, Perf::STUB_FLAG_CPU, dwCurTime, llPerfCountCur);

						if (lpmsg)
							LogFinal("Perf") << L"'Ka!' Time : " << (dwCurTime-dwStartTime) << L"ms, HWND : " << lpmsg->hwnd << L", message : " << lpmsg->message << L", wParam : " << lpmsg->wParam << L", lParam : " << lpmsg->lParam;
						Perf::MSG_KA stMsg;
						stMsg.dwTID				= GetCurrentThreadId();
						stMsg.llPerfCountBegin	= llPerfCountStart.QuadPart;
						stMsg.llPerfCountEnd	= llPerfCountCur.QuadPart;
						CThreadTable::GetInstance()->GetThreadName(GetCurrentThreadId(), stMsg.szThreadName, _countof(stMsg.szThreadName));
						CToolExchangeCenter::GetInstance()->SendToTools(eMsgKa, &stMsg, sizeof(stMsg));
					}
				}
			}
		}


		::TlsSetValue(pInst->m_dwTlsDirty, (void*)TRUE);
		return lRet;
	}


	CThreadResponseTime *CThreadResponseTime::GetInstance()
	{
		static CThreadResponseTime *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CThreadResponseTime), TRUE)) CThreadResponseTime();

		return s_pImpl;

	}

	void CThreadResponseTime::AddThread(DWORD dwTID)
	{
		::EnterCriticalSection(&m_cri);
		m_setThread.insert(dwTID);
		::LeaveCriticalSection(&m_cri);
	}

	CThreadResponseTime::CThreadResponseTime()
	{
		::InitializeCriticalSection(&m_cri);

		m_wIdResponse = AddToStrTable("Ka");

		m_dwTlsDirty = ::TlsAlloc();

		assert(s_pDispatchMessage);
		DetourFunctionWithTrampoline((PBYTE)Detour_DispatchMessage, (PBYTE)Tramp_DispatchMessage);
		//DetourRemove((PBYTE)Detour_DispatchMessage, (PBYTE)Tramp_DispatchMessage);
	}
}
