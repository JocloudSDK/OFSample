/** 
@file
@brief 从Log文件提取最近Log内容
@version 2011-7-10 bobdeng
*/

#include "stable.h"
#include <string>
#include <time.h>
#include <shlobj.h>
#include <queue>
#include <assert.h>
#include "logdefine.h"
#include "dwutility/ustring.h"
#include "dwutility/appdatapath.h"

extern void decryptData(unsigned long *v);
extern int utf16to8(const wchar_t *w, char *buf, int size);


static BOOL UTF8toW(LPCSTR lpszSrc, int cch/*=-1*/, std::wstring &wstrDst)
{
	if (cch == -1)
		cch = (int)strlen(lpszSrc);

	wstrDst.resize(cch, NULL);
	int n = MultiByteToWideChar(CP_UTF8, 0, lpszSrc, cch, &wstrDst[0], cch);
	wstrDst.resize(n);
	return n > 0;
}

static const std::string ConvertLog(DWORD dwOrder, LOG_ITEM *pItem)
{
	char szDate[128] = {0};
	_snprintf_s(szDate, _countof(szDate), _TRUNCATE, "%d-%d-%d %d:%d:%d %dms",
		pItem->stTime.wYear, 
		pItem->stTime.wMonth,
		pItem->stTime.wDay,
		pItem->stTime.wHour,
		pItem->stTime.wMinute,
		pItem->stTime.wSecond,
		pItem->stTime.wMilliseconds);

	char szFilter[128] = {0};
	char szFuncName[128] = {0};
	char szCppName[128] = {0};
	char szModule[128] = {0};
	strncpy_s(szFilter,		_countof(szFilter),		(char*)(pItem + 1), pItem->cchFilter);
	strncpy_s(szFuncName,	_countof(szFuncName),	(char*)(pItem + 1) + pItem->cchFilter, pItem->cchFunc);
	strncpy_s(szCppName,	_countof(szCppName),	(char*)(pItem + 1) + pItem->cchFilter + pItem->cchFunc, pItem->cchCppName);
	strncpy_s(szModule,		_countof(szModule),		(char*)(pItem + 1) + pItem->cchFilter + pItem->cchFunc + pItem->cchCppName, pItem->cchModule);
	
	//log内容
	std::wstring wstrLog;
	UTF8toW((const char *)((BYTE*)(pItem + 1) + pItem->cchFilter + pItem->cchFunc + pItem->cchCppName + pItem->cchModule),
		pItem->cchLog,
		wstrLog);
	std::string astrLog = DwUtility::text::toNarrowString(wstrLog.c_str(), wstrLog.size());

	char szLog[1024] = {0};
	_snprintf(szLog, _countof(szLog) - 1, "%7d PID=%-5d TID=%-5d %s  %-16s  %-12s \"%s\"  %s  %s  line:%d\r\n", 
		dwOrder, 
		pItem->dwProcessId,
		pItem->dwThreadId,
		szDate,
		szModule,
		szFilter,
		astrLog.c_str(),
		szFuncName,
		szCppName,
		(DWORD)pItem->wLine);
	szLog[_countof(szLog) - 1] = 0;

	return std::string(szLog);
}

//dwProcessId == 0 表示匹配全部进程
//dwYY == 0 表示匹配全部YY号码
DWBASE_EXPORTS void GetLastLog(LPCTSTR lpszLogName, 
							   DWORD dwProcessId, 
							   DWORD dwYY,
							   char *pBufDest, 
							   DWORD dwBufSize)
{
	//for debug
	//::MessageBox(NULL, L"GetLastLog", L"GetLastLog", MB_SERVICE_NOTIFICATION);
	
	pBufDest[0] = 0;
	std::string strError;
	std::queue<std::string> queExtractLog;
	DWORD dwExtractLogLen = 0;

	//打开Log文件
	TCHAR szAppData[MAX_PATH] = {0};
	SHGetSpecialFolderPath(NULL, szAppData, CSIDL_APPDATA, TRUE);

	TCHAR szLogPath[MAX_PATH] = {0};
	_sntprintf(szLogPath, _countof(szLogPath) - 1, _T("%s%s\\%s.lg"), szAppData, _T("\\duowan\\yy\\log"), lpszLogName);

	HANDLE hFile = ::CreateFile(szLogPath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, NULL);
	do
	{
		if (hFile == INVALID_HANDLE_VALUE)
		{
			char  szTemp[128];
			DWORD dwErr =  GetLastError();
			_snprintf(szTemp, _countof(szTemp) - 1, ("Open log failed,Error=%u\r\n"), dwErr); 
			
			strError = szTemp;
			break;
		}

		DWORD dwReadBytes = 0;
		LOG_FILE_HEAD head;
		ReadFile(hFile, &head, sizeof(head), &dwReadBytes, NULL);
		if (dwReadBytes != sizeof(head))
		{
			strError = "Read log header failed\r\n";
			break;
		}
		if (head.dwFlag != c_dwFileFlag)
		{
			strError = "Check log dwFlag failed\r\n";
			break;
		}

		if (head.dwVer != c_dwFileVersion)
		{
			strError = "Check log dwVer failed\r\n";
			break;
		}

		DWORD c_dwBufSize = 1024 * 1024;
		BYTE *pBuf = new BYTE[c_dwBufSize];

		DWORD dwOrder = 0;
		while (1)
		{
			LOG_CRYPT_BLOCK block = {0};

			::ReadFile(hFile, &block, sizeof(LOG_CRYPT_BLOCK), &dwReadBytes, NULL);
			if (dwReadBytes != sizeof(LOG_CRYPT_BLOCK))
			{
				if (dwReadBytes != 0)
					strError = "Read LOG_CRYPT_BLOCK failed\r\n";

				break;
			}
			
			DWORD dwCryptDataLen = block.dwBlockSize - sizeof(LOG_CRYPT_BLOCK);
			if (dwCryptDataLen > c_dwBufSize || dwCryptDataLen == 0)
			{
				strError = "Check dwCryptDataLen failed\r\n";
				break;
			}

			::ReadFile(hFile, pBuf, dwCryptDataLen, &dwReadBytes, NULL);
			if (dwReadBytes != dwCryptDataLen)
			{
				strError = "read dwCryptDataLen failed\r\n";
				break;
			}			

			LOG_ITEM * pItem = (LOG_ITEM *)pBuf;
			if (pItem->header.wType != TYPE_E_CONTENT2)
			{
				if (pItem->header.dwCookie == c_dwCookie)
				{
					for (unsigned long* p = (unsigned long*)((BYTE*)pBuf + sizeof(LOG_ITEM)); p < (unsigned long*)((BYTE*)pBuf + dwCryptDataLen); p += 2)
					{
						decryptData(p);
					}
				}
				else
				{
					for (unsigned long* p = (unsigned long*)pBuf; p < (unsigned long*)((BYTE*)pBuf + dwCryptDataLen); p += 2)
					{
						decryptData(p);
					}
				}
			}
				//只收集当前崩溃进程的log
				if ( (pItem->header.wType == TYPE_E_CONTENT || pItem->header.wType == TYPE_E_CONTENT2)
					&& (dwProcessId == 0 || pItem->dwProcessId == dwProcessId)
					&& (dwYY == 0 || pItem->dwYY == dwYY))
				{
					std::string strLog = ConvertLog(dwOrder, pItem);
					queExtractLog.push(strLog);
					dwExtractLogLen += strLog.length();
					dwOrder++;

					//限制log的大小
					while (dwExtractLogLen > dwBufSize)
					{
						assert(dwExtractLogLen >= queExtractLog.front().length());

						dwExtractLogLen -= queExtractLog.front().length();
						queExtractLog.pop();
					}
				}
		}
	
		delete [] pBuf;
		pBuf = NULL;
	} while (false);

	CloseHandle(hFile);

	//填充pBufDest
	while (dwExtractLogLen + strError.length() + 1 > dwBufSize && !queExtractLog.empty())
	{
		assert(dwExtractLogLen >= queExtractLog.front().length());

		dwExtractLogLen -= queExtractLog.front().length();
		queExtractLog.pop();
	}

	DWORD dwCurLen = 0; 
	while (!queExtractLog.empty())
	{
		std::string str = queExtractLog.front();
		memcpy((BYTE *)pBufDest + dwCurLen, str.c_str(), str.length());
		dwCurLen += str.length();
		queExtractLog.pop();
	}
	assert(dwExtractLogLen == dwCurLen);
	pBufDest[dwCurLen] = 0;
	
	strncat_s(pBufDest, dwBufSize, strError.c_str(), _TRUNCATE);
}
