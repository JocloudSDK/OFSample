/** 
@file
@brief APIHook，部分API自动加性能Log
*/

#include "stable.h"
#include "apihook.h"
#include "perflogimpl.h"
#include ".\detours\Detours.h"
#include <tchar.h>
#include <assert.h>

const DWORD c_dwInterval = 100;

#pragma warning(disable: 4100)

static void **s_pReadFile = (void**)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "ReadFile");
static void * &fnReadFile = *s_pReadFile;
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped), fnReadFile);

static void **s_pWriteFile = (void**)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "WriteFile");
static void * &fnWriteFile = *s_pWriteFile;
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped), fnWriteFile);

static void **s_pCreateFile = (void**)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "CreateFileW");
static void * &fnCreateFile = *s_pCreateFile;
DETOUR_TRAMPOLINE(HANDLE WINAPI Detour_CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile), fnCreateFile);

static void **s_pLoadLibraryW = (void**)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "LoadLibraryW");
static void * &fnLoadLibraryW = *s_pLoadLibraryW;
DETOUR_TRAMPOLINE(HMODULE WINAPI Detour_LoadLibraryW(LPCTSTR lpFileName), fnLoadLibraryW);

static void **s_pLoadLibraryA = (void**)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
static void * &fnLoadLibraryA = *s_pLoadLibraryA;
DETOUR_TRAMPOLINE(HMODULE WINAPI Detour_LoadLibraryA(LPCSTR lpFileName), fnLoadLibraryA);

static void **s_pWaitForSingleObject = (void**)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "WaitForSingleObject");
static void * &fnWaitForSingleObject = *s_pWaitForSingleObject;
DETOUR_TRAMPOLINE(DWORD WINAPI Detour_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds), fnWaitForSingleObject);

static void **s_pShell_NotifyIconW = (void**)::GetProcAddress(GetModuleHandle(_T("shell32.dll")), "Shell_NotifyIconW");
static void * &fnShell_NotifyIconW = *s_pShell_NotifyIconW;
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_Shell_NotifyIconW(DWORD dwMessage, PNOTIFYICONDATA lpdata), fnShell_NotifyIconW);

namespace Perf
{
#define API_READFILE "API_ReadFile"
#define API_WRITEFILE "API_WriteFile"
#define API_CREATEFILE "API_CreateFile"
#define API_LOADLIBRARY "API_LoadLibrary"
#define API_WAITFROSINGLEOBJECT "API_WaitForSingleObject"
#define API_SHELL_NOTIFYICON "API_Shell_NotifyIconW"

#define F_WAIT "Wait"
#define F_SHELL_NOTIFYICON "Shell_NotifyIconW"
std::set<DWORD> CAPIHook::m_setImportantThread;
	DWORD WINAPI CAPIHook::Tramp_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
	{
		if (CAPIHook::GetInstance()->GetAPIHook() && (m_setImportantThread.find(GetCurrentThreadId()) != m_setImportantThread.end()))
		{	//由于WaitForSingleObject过多，为了性能，只监控重要线程
			DWORD dwLastErr = 0;
			DWORD dwRet = 0;
			{
				PerfLog(1, API_WAITFROSINGLEOBJECT, F_WAIT);
				dwRet = Detour_WaitForSingleObject(hHandle, dwMilliseconds);
				dwLastErr = GetLastError();
			}
			SetLastError(dwLastErr);

			return dwRet;
		}
		DWORD dwRet = Detour_WaitForSingleObject(hHandle, dwMilliseconds);
		return dwRet;
	}

	BOOL WINAPI CAPIHook::Tramp_Shell_NotifyIconW(DWORD dwMessage, PNOTIFYICONDATA lpdata)
	{
		if (CAPIHook::GetInstance()->GetAPIHook())
		{
			DWORD dwLastErr = 0;
			BOOL bRet = 0;
			{
				PerfLog(1, API_SHELL_NOTIFYICON, F_SHELL_NOTIFYICON);
				bRet = Detour_Shell_NotifyIconW(dwMessage, lpdata);
				dwLastErr = GetLastError();
			}
			SetLastError(dwLastErr);

			return bRet;
		}
		BOOL bRet = Detour_Shell_NotifyIconW(dwMessage, lpdata);
		return bRet;
	}

	BOOL WINAPI CAPIHook::Tramp_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		if (CAPIHook::GetInstance()->GetAPIHook())
		{
			DWORD dwLastErr = 0;
			BOOL bRet = 0;
			{
				PerfLog(2, API_READFILE, F_IO);
				bRet = Detour_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
				dwLastErr = GetLastError();
			}
			SetLastError(dwLastErr);
			return bRet;
		}
		BOOL bRet = Detour_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

		return bRet;
	}

	BOOL WINAPI CAPIHook::Tramp_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		if (CAPIHook::GetInstance()->GetAPIHook())
		{
			DWORD dwLastErr = 0;
			BOOL bRet = 0;
			{
				PerfLog(2, API_WRITEFILE, F_IO);
				bRet = Detour_WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
				dwLastErr = GetLastError();
			}
			SetLastError(dwLastErr);
			return bRet;
		}
		BOOL bRet = Detour_WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

		return bRet;
	}

	HANDLE WINAPI CAPIHook::Tramp_CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
	{
		if (CAPIHook::GetInstance()->GetAPIHook())
		{
			DWORD dwLastErr = 0;
			HANDLE hRet = 0;
			{
				//CreateFile改为动态性能Log，增加文件名
				char szLog[MAX_PATH] = API_CREATEFILE;
				WideCharToMultiByte(CP_ACP, 0, lpFileName, -1, szLog + strlen(API_CREATEFILE), 150, NULL, NULL);
				PerfLogDynamicBegin(1, szLog, F_IO);

				hRet = Detour_CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
				dwLastErr = GetLastError();

				PerfLogDynamicEnd(1, szLog, F_IO);
			}
			SetLastError(dwLastErr);
			return hRet;
		}
		HANDLE hRet = Detour_CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

		return hRet;
	}

	HMODULE WINAPI CAPIHook::Tramp_LoadLibraryW(LPCTSTR lpFileName)
	{
		if (CAPIHook::GetInstance()->GetAPIHook())
		{
			DWORD dwLastErr = 0;
			HMODULE hRet = 0;
			{
				//PerfLog(1, API_LOADLIBRARY, F_IO);
				//LoadLibrary改为动态性能Log，增加dll名
				//Get Dll Name
				LPCTSTR p1 = wcsrchr(lpFileName, _T('\\'));
				LPCTSTR p2 = wcsrchr(lpFileName, _T('/'));
				LPTSTR lpDllName = (LPTSTR)lpFileName;
				if (p1 + 1 > lpDllName)
				{
					lpDllName = (LPTSTR)p1 + 1;
				}
				if (p2 + 1 > lpDllName)
				{
					lpDllName = (LPTSTR)p2 + 1;
				}

				char szLog[MAX_PATH] = API_LOADLIBRARY;
				WideCharToMultiByte(CP_ACP, 0, lpDllName, -1, szLog + strlen(API_LOADLIBRARY), 150, NULL, NULL);
				PerfLogDynamicBegin(1, szLog, F_IO);

				hRet = Detour_LoadLibraryW(lpFileName);
				dwLastErr = GetLastError();

				PerfLogDynamicEnd(1, szLog, F_IO);
			}
			SetLastError(dwLastErr);

			return hRet;
		}
		HMODULE hRet = Detour_LoadLibraryW(lpFileName);
		return hRet;
	}

	HMODULE WINAPI CAPIHook::Tramp_LoadLibraryA(LPCSTR lpFileName)
	{
		if (CAPIHook::GetInstance()->GetAPIHook())
		{
			DWORD dwLastErr = 0;
			HMODULE hRet = 0;
			{
				PerfLog(1, API_LOADLIBRARY, F_IO);
				hRet = Detour_LoadLibraryA(lpFileName);
				dwLastErr = GetLastError();
			}
			SetLastError(dwLastErr);

			return hRet;
		}
		HMODULE hRet = Detour_LoadLibraryA(lpFileName);
		return hRet;
	}

	CAPIHook *CAPIHook::GetInstance()
	{
		static CAPIHook *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CAPIHook), TRUE)) CAPIHook();

		return s_pImpl;

	}

	CAPIHook::CAPIHook()
	{
		m_bAPIHook = FALSE;

		assert(s_pReadFile);
		assert(s_pWriteFile);
		assert(s_pCreateFile);
		assert(s_pLoadLibraryW);
	}

	BOOL CAPIHook::GetAPIHook()
	{
		return m_bAPIHook;
	}

	void CAPIHook::SetAPIHook(BOOL bEnable)
	{
		m_bAPIHook = bEnable;

		static BOOL s_bInit = FALSE;
		if (bEnable && !s_bInit)
		{
			s_bInit = TRUE;
			DetourFunctionWithTrampoline((PBYTE)Detour_WaitForSingleObject, (PBYTE)Tramp_WaitForSingleObject);
			DetourFunctionWithTrampoline((PBYTE)Detour_Shell_NotifyIconW, (PBYTE)Tramp_Shell_NotifyIconW);
			DetourFunctionWithTrampoline((PBYTE)Detour_ReadFile, (PBYTE)Tramp_ReadFile);
			DetourFunctionWithTrampoline((PBYTE)Detour_WriteFile, (PBYTE)Tramp_WriteFile);
			DetourFunctionWithTrampoline((PBYTE)Detour_CreateFile, (PBYTE)Tramp_CreateFile);
			DetourFunctionWithTrampoline((PBYTE)Detour_LoadLibraryW, (PBYTE)Tramp_LoadLibraryW);
			DetourFunctionWithTrampoline((PBYTE)Detour_LoadLibraryA, (PBYTE)Tramp_LoadLibraryA);
		}
		//Detour 不要Remove，容易出问题
	}

	void CAPIHook::AddImportantThread(DWORD dwThreadID)
	{	
		m_setImportantThread.insert(dwThreadID);
	}


}
