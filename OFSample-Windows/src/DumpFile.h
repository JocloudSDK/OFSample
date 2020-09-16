#pragma once
#include <windows.h>
#include <Dbghelp.h>
#include <iostream>  
#include <vector>  
#include <tchar.h>
using namespace std;

#pragma comment(lib, "Dbghelp.lib")

namespace NSDumpFile
{
	void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
	{
		// 创建Dump文件  
		HANDLE hDumpFile = CreateFile((lpstrDumpFilePathName), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Dump信息  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;

		// 写入Dump文件内容  
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		CloseHandle(hDumpFile);
	}

	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
	{
		return NULL;
	}

	BOOL PreventSetUnhandledExceptionFilter()
	{
		//Windows 本身的32位的动态链接库，确保自己有这个库
		HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
		if (hKernel32 == NULL)
			return FALSE;


		void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
		if (pOrgEntry == NULL)
			return FALSE;

		unsigned char newJump[100];
		DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
		dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far

		void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
		DWORD dwNewEntryAddr = (DWORD)pNewFunc;
		DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

		newJump[0] = 0xE9;  // JMP absolute
		memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
		SIZE_T bytesWritten;
		BOOL bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
		return bRet;
	}

	LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException)
	{
		TCHAR szMbsFile[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szMbsFile, MAX_PATH);

		CreateDirectory(L"Dump", NULL);
		// 创建 dmp 文件件
		TCHAR szFileName[MAX_PATH] = { 0 };
		SYSTEMTIME stLocalTime;
		GetLocalTime(&stLocalTime);
		wsprintf(szFileName, L"%s-%04d%02d%02d-%02d%02d%02d.dmp",
			L"Dump\\OFSample", stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
			stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
		TCHAR* pFind = _tcsrchr(szMbsFile, '\\');
		if (pFind)
		{
			*(pFind + 1) = 0;
			//这边是生成的DUMP文件的名称
			_tcscat(szMbsFile, szFileName);
			CreateDumpFile((LPCWSTR)szMbsFile, pException);
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	void RunCrashHandler()
	{
		SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
		PreventSetUnhandledExceptionFilter();
	}
};

#define DeclareDumpFile() NSDumpFile::RunCrashHandler();