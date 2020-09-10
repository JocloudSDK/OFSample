/** 
@file
@brief 处理采集模块和工具的交互
@version 2011-9-7 bobdeng
*/

#pragma once

#include "stlalloc.h"
#include "toolmsg.h"

namespace Perf
{
	struct CallStackTls;
	class CToolExchangeCenter
	{
	public:
		static CToolExchangeCenter *GetInstance();
		void SendToTools(UINT uMsgId, const void *pBuf, DWORD dwSize);
		void RegisterWnd(HWND hWnd);
		void UnRegisterWnd(HWND hWnd);
		void SetPerfLogFile( const TCHAR* szPathName);
		void SendReceiverQuit();

	private:
		CToolExchangeCenter();
		static DWORD WINAPI MsgThread(void* );
		static LRESULT __stdcall RecvMsgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void AddReceiverWnd(HWND hWnd);
		void DelReceiverWnd(HWND hWnd);
		void PostInitHookSlots();
		void PostUnHookSlots();

	private:
		typedef std::set<HWND, std::less<HWND>, CMyStlAlloc< HWND > > CSetReceiver;
		CSetReceiver		m_setReceiver;

		CRITICAL_SECTION	m_cri;
	};
}
