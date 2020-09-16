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
		// ����Dump�ļ�  
		HANDLE hDumpFile = CreateFile((lpstrDumpFilePathName), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Dump��Ϣ  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;

		// д��Dump�ļ�����  
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		CloseHandle(hDumpFile);
	}

	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
	{
		return NULL;
	}

	BOOL PreventSetUnhandledExceptionFilter()
	{
		//Windows �����32λ�Ķ�̬���ӿ⣬ȷ���Լ��������
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
		// ���� dmp �ļ���
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
			//��������ɵ�DUMP�ļ�������
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