#ifndef __WIN_PROCESSEX_H
#define __WIN_PROCESSEX_H

#pragma once

#include <process.h>

HANDLE xthread_create(int (*start_routine)(void *), void *arg);
int xthread_close(HANDLE h);

int	xthread_wait(HANDLE h, DWORD timeout);

bool xthread_is_equal_id(unsigned long tid1, unsigned long tid2);

unsigned long xthread_get_id_of(HANDLE h);
unsigned long xthread_self_id();

unsigned long xprocess_self_id();

namespace __apiimp
{
	struct _thread_arg
	{
		int (*start_routine)(void *);
		void* arg;
	};

	inline unsigned __stdcall _thread_proc(void* p)
	{
		_thread_arg arg = *(_thread_arg*)p;
		delete (_thread_arg*)p;
		return arg.start_routine(arg.arg);
	}

    inline DWORD _XGetThreadId(HANDLE h)
    {
        typedef long _NTSTATUS;
        enum _THREADINFOCLASS {_ThreadBasicInformation = 0};
        struct _CLIENT_ID
        {
            DWORD UniqueProcess; 
            DWORD UniqueThread;
        };
        typedef long _KPRIORITY;
        struct _THREAD_BASIC_INFORMATION
        {
            _NTSTATUS   ExitStatus;
            PVOID      TebBaseAddress;
            _CLIENT_ID  ClientId;
            KAFFINITY  AffinityMask;
            _KPRIORITY  Priority;
            _KPRIORITY  BasePriority;
        };

        typedef _NTSTATUS
            (__stdcall *LPFN_NtQueryInformationThread) (
            IN HANDLE ThreadHandle,
            IN _THREADINFOCLASS ThreadInformationClass,
            OUT PVOID ThreadInformation,
            IN unsigned long ThreadInformationLength,
            OUT unsigned long* ReturnLength OPTIONAL );
        static LPFN_NtQueryInformationThread lpfnNtQueryInformationThread = NULL;
        if (lpfnNtQueryInformationThread == NULL)
        {
            HMODULE hNtDll = ::LoadLibraryW(L"ntdll.dll");
            ASSERT(hNtDll != NULL);
            if (hNtDll != NULL)
            {
                lpfnNtQueryInformationThread = (LPFN_NtQueryInformationThread)::GetProcAddress(hNtDll, "NtQueryInformationThread");
                ASSERT(lpfnNtQueryInformationThread != NULL);
            }

            if (lpfnNtQueryInformationThread == NULL)
                lpfnNtQueryInformationThread = (LPFN_NtQueryInformationThread)-1;	//err
        }

        if (lpfnNtQueryInformationThread == (LPFN_NtQueryInformationThread)-1)
            return 0;

        _THREAD_BASIC_INFORMATION ThreadInfo = {0};
        unsigned long ThreadInfoLength = sizeof(ThreadInfo);
        _NTSTATUS r = (*lpfnNtQueryInformationThread)(h, _ThreadBasicInformation, &ThreadInfo, ThreadInfoLength, NULL);
        if ((long)r < 0)
            return 0;	//err

        return ThreadInfo.ClientId.UniqueThread;
    }
} //namespace __apiimp

inline 
HANDLE xthread_create(int (*start_routine)(void *), void *arg)
{
	__apiimp::_thread_arg* pArg = new __apiimp::_thread_arg;
	pArg->start_routine = start_routine;
	pArg->arg = arg;

	unsigned t = 0;
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, __apiimp::_thread_proc, pArg, 0, &t);
	if (h != NULL)
		return h;	//ok

	//err
	delete pArg;
	return NULL;
}

inline 
int xthread_close(HANDLE h)
{
	if (h == NULL)
		return -1;

	if (! ::CloseHandle(h))
		return -1;

	return 0;
}

inline 
int	xthread_wait(HANDLE h, DWORD timeout)
{
	if (h == NULL)
		return -1;

	if (::WaitForSingleObject(h, timeout) != WAIT_OBJECT_0)
		return -1;

	return 0;
}

inline 
bool xthread_is_equal_id(unsigned long tid1, unsigned long tid2)
{
	return tid1 == tid2;
}

inline 
unsigned long xthread_get_id_of(HANDLE h)
{
	if (h == NULL)
		return 0;

    return (unsigned long) (__apiimp::_XGetThreadId(h));
}

inline 
unsigned long xthread_self_id()
{
	return (unsigned long) (GetCurrentThreadId());
}

inline 
unsigned long xprocess_self_id()
{
	return (unsigned long) (GetCurrentProcessId());
}

#endif //__WIN_PROCESSEX_H
