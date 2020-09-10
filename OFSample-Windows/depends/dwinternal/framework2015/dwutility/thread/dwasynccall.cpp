#include "stable.h"

#include "dwutility/locker.h"
#include "dwasynccall.h"

const int KMessageAsyncCall = WM_USER + 700;

typedef std::map<DWORD, HWND> AsyncCallDispatchersMap;
static AsyncCallDispatchersMap _dw_asyncCallDispatchers;
static DwUtility::DwCSLock _dw_asyncCallLock;

static LRESULT CALLBACK dwDispatcherWndProc( HWND hwnd, UINT uMsg, WPARAM w, LPARAM l )
{
	if(uMsg == KMessageAsyncCall)
	{
		DWASYNCCALLPROC func = (DWASYNCCALLPROC)w;
		void *param = (void*)l;
		func(param);

		return 0;
	}
	return ::DefWindowProc(hwnd, uMsg, w, l);
}

void DwAsyncCall::init()
{
	DW_AUTO_LOCKER(&_dw_asyncCallLock);

	HWND hWnd = CreateWindow(L"STATIC", L"", WS_POPUP, 0, 0, 0, 0, HWND_MESSAGE, NULL, ::GetModuleHandle(NULL), NULL);
	::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)dwDispatcherWndProc);
	_dw_asyncCallDispatchers[GetCurrentThreadId()] = hWnd;
}

void DwAsyncCall::terminate()
{
	DW_AUTO_LOCKER(&_dw_asyncCallLock);

	AsyncCallDispatchersMap::iterator it = _dw_asyncCallDispatchers.find(GetCurrentThreadId());
	if(it != _dw_asyncCallDispatchers.end())
	{
		::DestroyWindow(it->second);
		_dw_asyncCallDispatchers.erase(it);
	}
}

void DwAsyncCall::call( DWASYNCCALLPROC proc, void *param, DWORD threadId )
{
	DW_AUTO_LOCKER(&_dw_asyncCallLock);
	if(threadId == 0)
	{
		threadId = GetCurrentThreadId();
	}

	AsyncCallDispatchersMap::iterator it = _dw_asyncCallDispatchers.find(threadId);
    DW_ASSERT(it != _dw_asyncCallDispatchers.end());

	if(it != _dw_asyncCallDispatchers.end())
	{
		::PostMessage(it->second, KMessageAsyncCall, (WPARAM)proc, (LPARAM)param);
	}
}

inline void CALLBACK tiny_function0_call(void *p)
{
	tiny_function0<void> *f = (tiny_function0<void>*)p;
	(*f)();
	delete f;
}

void DwAsyncCall::call(const tiny_function0<void> &f, DWORD threadId)
{
	DwAsyncCall::call(tiny_function0_call, new tiny_function0<void>(f), threadId);
}
