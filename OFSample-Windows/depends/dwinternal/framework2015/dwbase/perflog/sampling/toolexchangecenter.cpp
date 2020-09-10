/** 
@file
@brief 处理采集模块和工具的交互
@version 2011-9-7 bobdeng
*/


#include "stable.h"
#include <windows.h>
#include <assert.h>
#include <vector>
#include <string>
#include <tchar.h>
#include "logfile.h"
#include "toolexchangecenter.h"
#include "sysutil.h"
#include "control.h"

namespace Perf
{

	CToolExchangeCenter *CToolExchangeCenter::GetInstance()
	{
		static CToolExchangeCenter *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CToolExchangeCenter), TRUE)) CToolExchangeCenter();

		return s_pImpl;
	}

	CToolExchangeCenter::CToolExchangeCenter()
	{
		::InitializeCriticalSection(&m_cri);

		DWORD dwThreadId = 0;
		HANDLE hThread = (HANDLE)::CreateThread(NULL,
						0,
						&MsgThread,
						0,
						CREATE_SUSPENDED,
						&dwThreadId);
		assert(hThread);
		::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
		::ResumeThread(hThread);
		::CloseHandle(hThread);
	}

	void CToolExchangeCenter::RegisterWnd(HWND hWnd)
	{
		AddReceiverWnd(hWnd);
	}

	void CToolExchangeCenter::AddReceiverWnd(HWND hWnd)
	{
		if( hWnd == 0x00000000 )
			return;
		::EnterCriticalSection(&m_cri);
		m_setReceiver.insert(hWnd);
		::LeaveCriticalSection(&m_cri);

		Perf::StartPerfLog();
	}

	void CToolExchangeCenter::UnRegisterWnd(HWND hWnd)
	{
		DelReceiverWnd(hWnd);
	}

	void CToolExchangeCenter::DelReceiverWnd(HWND hWnd)
	{
		bool bFind = false;
		::EnterCriticalSection(&m_cri);
		CSetReceiver::iterator it = m_setReceiver.find(hWnd);
		bFind = ( it != m_setReceiver.end() );
		if( bFind )
			m_setReceiver.erase(it);
		::LeaveCriticalSection(&m_cri);

		if( bFind )
		{
			Perf::EndPerfLog();
			PostUnHookSlots();
			::PostMessage(hWnd,WM_APP_PERF,eCmdEndPerfLog,0);
		}
	}

	void CToolExchangeCenter::PostInitHookSlots()
	{
		//初始化窗口
		wchar_t szWindowName[128] = {0};
		_sntprintf_s(szWindowName, _countof(szWindowName), _TRUNCATE, c_szPerfMsgWndNameFmt, ::GetCurrentProcessId());
		HWND hWnd = FindWindow(_T("szDWPerfLogMsgWndClass"), szWindowName);
		PostMessage(hWnd,WM_PERFLOG_YYMSG,0,(LPARAM)MSG_E_INITHOOKSLOT);
	}

	void CToolExchangeCenter::PostUnHookSlots()
	{
		//初始化窗口
		wchar_t szWindowName[128] = {0};
		_sntprintf_s(szWindowName, _countof(szWindowName), _TRUNCATE, c_szPerfMsgWndNameFmt, ::GetCurrentProcessId());
		HWND hWnd = FindWindow(_T("szDWPerfLogMsgWndClass"), szWindowName);
		PostMessage(hWnd,WM_PERFLOG_YYMSG,0,(LPARAM)MSG_E_UNHOOKSLOT);
	}

	void CToolExchangeCenter::SendReceiverQuit()
	{
		::EnterCriticalSection(&m_cri);
		const CSetReceiver setCopy = m_setReceiver;
		::LeaveCriticalSection(&m_cri);

		CSetReceiver::const_iterator it;
		for(it = setCopy.begin(); it!=setCopy.end(); ++it)
		{
			::PostMessage(*it,WM_APP_PERF,eCmdEndPerfLog,0);
		}
	}

	void CToolExchangeCenter::SetPerfLogFile(const TCHAR* szPathName)
	{
		if( szPathName != NULL )
			Perf::SetPerfLogFile(szPathName);
	}

	LRESULT __stdcall CToolExchangeCenter::RecvMsgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if( uMsg == WM_COPYDATA )
		{
			COPYDATASTRUCT * pData = reinterpret_cast<COPYDATASTRUCT*>(lParam);
			switch(pData->dwData)
			{
			case eCmdFilePathName:
				{
					TCHAR szPathName[1024] = {0};
					memcpy(szPathName,pData->lpData,pData->cbData);
					GetInstance()->SetPerfLogFile(szPathName);
				}
				break;
			}
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	DWORD WINAPI CToolExchangeCenter::MsgThread(void *)
	{
		CToolExchangeCenter *pInst = CToolExchangeCenter::GetInstance();
		
		//初始化窗口
		wchar_t szWindowName[128] = {0};
		_sntprintf_s(szWindowName, _countof(szWindowName), _TRUNCATE, c_szPerfWndNameFmt, ::GetCurrentProcessId());

		HMODULE hModule = ::GetModuleHandle(NULL);
		const TCHAR* lpszClass = _T("szDWPerfLogWndClass");
		WNDCLASSEX wcex;
		wcex.cbSize			= sizeof(WNDCLASSEX); 
		wcex.style			= CS_CLASSDC;
		wcex.lpfnWndProc	= RecvMsgWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= (HINSTANCE)hModule;
		wcex.hIcon			= NULL;
		wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= lpszClass;
		wcex.hIconSm		= NULL;
		static ATOM atom = ::RegisterClassExW(&wcex);

		::CreateWindowEx(0, lpszClass, szWindowName, 0,
			0, 0, 0, 0, HWND_MESSAGE, (HMENU)0, (HINSTANCE)hModule, 0);

		MSG msg;
		while(1)
		{
			DWORD dwRes = GetMessage(&msg, NULL, 0, 0);
			if (dwRes == -1 || dwRes == 0)
			{
				assert(!_T("GetMessage failed"));
				break;
			}
			else
			{
				if (msg.message == WM_APP_PERF)
				{
					if (msg.wParam == eCmdAddReceiver)
					{
						pInst->PostInitHookSlots();
						pInst->AddReceiverWnd((HWND)msg.lParam);
						CControl::GetInstance()->OpenFullOption();
					}
					else if (msg.wParam == eCmdDelReceiver)
					{
						pInst->DelReceiverWnd((HWND)msg.lParam);
						CControl::GetInstance()->CloseAllOption();
					}
					else
					{
						assert(0);
					}
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return 0;
	}

	void CToolExchangeCenter::SendToTools(UINT uMsgId, const void *pBuf, DWORD dwSize)
	{
		::EnterCriticalSection(&m_cri);
		const CSetReceiver setCopy = m_setReceiver;
		::LeaveCriticalSection(&m_cri);

		COPYDATASTRUCT cd = {0};
		cd.cbData = dwSize;
		cd.dwData = uMsgId;
		cd.lpData = (void *)pBuf;
		for (CSetReceiver::const_iterator it = setCopy.begin(); it != setCopy.end(); ++it)
			::SendMessage(*it, WM_COPYDATA, NULL, (LPARAM)&cd);
	}
}
