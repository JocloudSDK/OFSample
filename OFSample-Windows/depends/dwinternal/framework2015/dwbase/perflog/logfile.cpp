/** 
@file
@brief 性能Log文件保存
*/
#include "stable.h"
#include "logfile.h"
#include "perflogimpl.h"
#include "perflog.h"
#include "sampling/callstacksampling.h"
#include "sampling/miscsampling.h"
#include "sampling/wmisampling.h"
#include "sampling/threadtable.h"
#include "strtable.h"
#include <assert.h>
#include <shlobj.h>
#include <tchar.h>
#include <stdio.h> 
#include "control.h"
#include "dwutility/appdatapath.h"

using namespace Perf;
namespace Perf
{
	HANDLE OpenFile(LPCTSTR strFilePath);

	void SaveFile(LPCTSTR strFilePath)
	{
		//这里一定要关掉API Hook, 否者写文件是WriteFile还在写Log，文件格式会错误
		CControl::GetInstance()->SetAPIHook(FALSE);

		HANDLE hFile = OpenFile(strFilePath);
		if (!hFile)
		{
			assert(false);
			return;
		}

		struct FILE_HEAD stFileHead;
		stFileHead.dwPID = GetCurrentProcessId();
		DWORD dwWritten = 0; 
		::WriteFile(hFile, &stFileHead, sizeof(stFileHead), &dwWritten, 0);

		//StringTable放Log前面
		DWORD dwDataLen = CStrTable::GetInstance()->Save(hFile);
		
		//临时把LogLevel调为0，为了禁止PerfLog的写操作，这样就不需要PerfLog内部加锁，提高Log性能
		DWORD dwOldLevel = CControl::GetInstance()->GetSamplingLogLevel();
		if (dwOldLevel > 0)
		{
			CControl::GetInstance()->SetSamplingLogLevel(0);
			dwDataLen += CPerfLogImpl::GetInstance()->Save(hFile);
			CControl::GetInstance()->SetSamplingLogLevel(dwOldLevel);
		}
		
		dwDataLen += CThreadTable::GetInstance()->Save(hFile);
		if (CControl::GetInstance()->IsSamplingCallStack())
		{
			dwDataLen += CCallStackSampling::GetInstance()->Save(hFile);
		}
		if (CControl::GetInstance()->IsSamplingCPUMem())
		{
			dwDataLen += CMiscSampling::GetInstance()->Save(hFile);
		}
		// New Added @2012.8.30 By Wuwei 
		if (CControl::GetInstance()->IsSamplingWMI())
		{
			dwDataLen += CWmiSampling::GetInstance()->Save(hFile);
		}

		//File Head
		stFileHead.dwDataLen = dwDataLen;
		stFileHead.llBeginPerfCount = CPerfRecord::GetInstance()->m_llBeginPerfCount;
		stFileHead.llEndPerfCount = CPerfRecord::GetInstance()->m_llEndPerfCount;
		stFileHead.dwBeginTickcount = CPerfRecord::GetInstance()->m_dwBeginTickcount;
		::SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		::WriteFile(hFile, &stFileHead, sizeof(stFileHead), &dwWritten, 0);

		CloseHandle(hFile);
	}

	static HANDLE OpenFile(LPCTSTR strFilePath)
	{
		if (strFilePath == NULL || *strFilePath == '\0')
		{
			TCHAR szAppData[MAX_PATH] = {0};
			SHGetSpecialFolderPath(NULL, szAppData, CSIDL_APPDATA, TRUE);

			TCHAR szLogDir[MAX_PATH] = {0};
			_sntprintf_s(szLogDir, _countof(szLogDir) - 1, _T("%s%s"), szAppData, _T("\\duowan\\yy\\log"));

			//目录是否存在，否则创建
			if (GetFileAttributes(szLogDir) == INVALID_FILE_ATTRIBUTES)
			{
				TCHAR szTemp[MAX_PATH] = {0};
				_sntprintf_s(szTemp, _countof(szTemp) - 1, _T("%s%s"), szAppData, _T("\\duowan"));
				::CreateDirectory(szTemp, NULL);
				_sntprintf_s(szTemp, _countof(szTemp) - 1, _T("%s%s"), szAppData, _T("\\duowan\\yy"));
				::CreateDirectory(szTemp, NULL);
				_sntprintf_s(szTemp, _countof(szTemp) - 1, _T("%s%s"), szAppData, _T("\\duowan\\yy\\log"));
				::CreateDirectory(szTemp, NULL);
			}

			//文件名字
			TCHAR szLogName[MAX_PATH] = {0};
			TCHAR szExePath[MAX_PATH] = {0};
			::GetModuleFileName(NULL, szExePath, _countof(szExePath));
			const TCHAR *p1 = _tcsrchr(szExePath, _T('\\')) + 1;
			const TCHAR *p2 = _tcsrchr(p1, _T('.'));
			assert(p2 != NULL);
			if (p2 == NULL)
				p2 = szExePath + _tcslen(szExePath);

			wcsncpy_s(szLogName, p1, p2 - p1);
			wcsncat_s(szLogName, _T(".plg"), _countof(szLogName) - _tcslen(_T(".plg")) - 1);

			TCHAR	szLogPath[MAX_PATH];
			szLogPath[_countof(szLogPath) - 1] = 0;
			_sntprintf_s(szLogPath, _countof(szLogPath) - 1, _T("%s\\%s"), szLogDir, szLogName);

			strFilePath = szLogPath;
		}
		
		HANDLE hFile = ::CreateFile(strFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return NULL;
		}
		return hFile;
	}
}
