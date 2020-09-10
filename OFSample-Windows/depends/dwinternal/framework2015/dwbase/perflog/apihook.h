
#pragma once 

#include <set>
#include "shellapi.h"

namespace Perf
{
	class CAPIHook
	{
	public:
		static CAPIHook *GetInstance();
		BOOL GetAPIHook();
		void SetAPIHook(BOOL bEnable);
		void AddImportantThread(DWORD dwThreadID);
	private:
		CAPIHook();
		static DWORD WINAPI Tramp_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
		static BOOL WINAPI Tramp_Shell_NotifyIconW(DWORD dwMessage, PNOTIFYICONDATA lpdata);

		static BOOL WINAPI Tramp_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
		static BOOL WINAPI Tramp_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
		static HANDLE WINAPI Tramp_CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

		static HMODULE WINAPI Tramp_LoadLibraryW(LPCTSTR lpFileName);
		static HMODULE WINAPI Tramp_LoadLibraryA(LPCSTR lpFileName);

		BOOL m_bAPIHook;
		static std::set<DWORD> m_setImportantThread;
	};
}
