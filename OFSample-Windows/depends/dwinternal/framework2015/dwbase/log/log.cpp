/** 
@file
@brief Log系统实现
@version 2011-4-18 bobdeng
*/

#include "stable.h"
#include <assert.h>
#include <tchar.h>
#include <process.h>
#include <set>
#include <vector>
#include <shlobj.h>
#include <time.h>
#include <intrin.h>
#include <limits>
#include "../perflog/stlalloc.h"
#include "logdefine.h"
#include "dwutility/appdatapath.h"

typedef void (* LogFilterCallback)(WORD wLogLevel, LPCSTR lpszModuleName, LPCSTR lpszFilter, LPCWSTR lpwszLog);

//#pragma warning(disable: 4996 4189 4100)
#pragma intrinsic(_ReturnAddress)

//#define PERFERMANCE_TEST

#ifndef MSGFLT_ADD
	#define MSGFLT_ADD 1
#endif

#define WM_APP_LOG (WM_APP + 0xFF)
enum LOG_MSG
{
	eAddReceiver = 1,//lParam 指定窗口
	eDelReceiver,	 //lParam 指定窗口
};

enum LOG_LEVEL
{
	LogFinal = 1,	//最终发布版本有效
	LogDev,			//内部测试版本有效
	LogTemp,		//为解决当前版本某个具体问题的临时Log，最终发布前必须清理
};

static DWORD c_dwMaxFileSize = 100 * 1024 * 1024;
const DWORD c_dwMaxFlushCount = 1000;
const DWORD c_dwMaxIntervalFlush = 5000;
const TCHAR c_szLogWndNameFmt[] = _T("szDWLogWndName_%d");
const TCHAR c_szLogMutexNameFmt[] = _T("szDWLogMutexName_%s");
const int c_dwMaxSizeToRemain = 10 * 1204 * 1024;//每次清理日志文件时保留10M最新的日志
const GUID c_key = { 0x492d4e95, 0xe38a, 0x433d, { 0x89, 0x2, 0x25, 0x57, 0x30, 0x7e, 0x4e, 0x11 } };

void encrypt(unsigned long *v, unsigned long *k)
{
	unsigned long y=v[0], z=v[1], sum=0, i;         /* set up */
	unsigned long delta=0x9e3779b9;                 /* a key schedule constant */
	unsigned long a=k[0], b=k[1], c=k[2], d=k[3];   /* cache key */
	for (i=0; i < 32; i++) {                        /* basic cycle start */
		sum += delta;
		y += ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
		z += ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);/* end cycle */
	}
	v[0]=y;
	v[1]=z;
}
 
void decrypt(unsigned long *v, unsigned long *k) 
{
	unsigned long y=v[0], z=v[1], sum=0xC6EF3720, i; /* set up */
	unsigned long delta=0x9e3779b9;                  /* a key schedule constant */
	unsigned long a=k[0], b=k[1], c=k[2], d=k[3];    /* cache key */
	for(i=0; i<32; i++) {                            /* basic cycle start */
		z -= ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
		y -= ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
		sum -= delta;                                /* end cycle */
	}
	v[0]=y;
	v[1]=z;
}

void encryptData(unsigned long *v)
{
	encrypt(v, (unsigned long*)&c_key);
}

void decryptData(unsigned long *v)
{
	decrypt(v, (unsigned long*)&c_key);
}

//如果指定buf为NULL，计算需要的缓冲大小，包括NULL
int utf16to8(const wchar_t *w, char *buf, int size)
{
	if (buf == NULL)
	{
		int j = 0;
		//for (int i = 0, count = wcslen(w); i < count; i++)
		while (unsigned short c = (unsigned short)*w++)
		{
			if (c <= 0x007F)
				j++;
			else if (c > 0x07FF)
				j += 3;
			else
				j += 2;
		}

		return j + 1;//包括NULL
	}

	if (buf && size > 0)
	{
		int j = 0;
		//for (int i = 0, count = wcslen(w); i < count; i++)
		while (unsigned short c = (unsigned short)*w++)
		{
			//unsigned short c = (unsigned short)w[i];
			if (c <= 0x007F)
			{
				if (j >= size - 1)
					break;
				buf[j++] = (char)c;
			}
			else if (c > 0x07FF)
			{
				if (j + 2 >= size - 1)
					break;
				buf[j++] = 0xE0 | ((c >> 12) & 0x0F);
				buf[j++] = 0x80 | ((c >>  6) & 0x3F);
				buf[j++] = 0x80 | ((c >>  0) & 0x3F);
			}
			else
			{
				if (j + 1 >= size - 1)
					break;
				buf[j++] = 0xC0 | ((c >>  6) & 0x1F);
				buf[j++] = 0x80 | ((c >>  0) & 0x3F);
			}
		}

		buf[j++] = 0;
		return j;
	}

	return 0;
}

class CLog
{
public:
	CLog();
	static CLog* GetInstance();
	
	void FlushLog();
	void CancelCacheWrite();
	void AddLog(WORD wLogLevel, LPCSTR lpszFilter, LPCSTR lpszCppName, WORD wLineNumber, LPCSTR lpszFuncName, LPCWSTR lpwszLog, const char* lpszModuleName = NULL);
	void SetLogFilter(LogFilterCallback pfCallback) { m_pfFilterCallback = pfCallback; }
	void SetLogInfo(DWORD dwYY) { m_dwYY = dwYY; }
    bool IsLogLevelEnabled(WORD wLogLevel) const;

	static BOOL GetModuleNameByAddr(DWORD dwAddr, char *szName, UINT cch);
private:
	static LRESULT __stdcall RecvMsgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static unsigned int _stdcall LogThread(void* );
    static unsigned int _stdcall flushRemainedLog(void* lpPara);
	static void DeleteBackup(const TCHAR *lpszDir, const TCHAR *lpszFileName);
	static void customizeUIPIPolicy( HWND hwnd, UINT message, DWORD dwFlag );

	TCHAR							m_szLogPath[MAX_PATH];
	HANDLE							m_hEventFlush;
	CRITICAL_SECTION				m_criLog;

	BOOL							m_bCacheWrite;
	HANDLE							m_hFile;
	HANDLE							m_hProcessMutex;
	enum LOG_LEVEL					m_nLogLevel;

	typedef std::vector<CLog::LOG_CRYPT_BLOCK *, CMyStlAlloc<CLog::LOG_CRYPT_BLOCK*> > CVecLog;
	typedef std::set<HWND, std::less<HWND>, CMyStlAlloc< HWND > > CSetReceiver;
	
	CSetReceiver					m_setReceiver;
	CVecLog							m_vecLog;

	LogFilterCallback				m_pfFilterCallback;
	DWORD							m_dwYY;
};

CLog* CLog::GetInstance()
{
	static CLog *pLog = NULL;
	if (!pLog)
	{
#ifdef _DEBUG
		int nOldFlag = _CrtSetDbgFlag(0);
#endif

		pLog = new CLog();

#ifdef _DEBUG
		_CrtSetDbgFlag(nOldFlag);
#endif
	}
	return pLog;
}

CLog::CLog()
{
	m_dwYY = 0;
	m_pfFilterCallback = NULL;
	m_bCacheWrite = TRUE;
	m_hEventFlush = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	
	::InitializeCriticalSection(&m_criLog);

#ifdef OFFICIAL_BUILD
	m_nLogLevel = LogFinal;
#else
	m_nLogLevel = LogTemp;
#endif
    
	//处理命令行选项
	LPTSTR lpszCmdLine = ::GetCommandLine();
	assert(lpszCmdLine);
	if (lpszCmdLine && _tcsstr(lpszCmdLine, _T("/LogDev")) != 0)
	{
		m_nLogLevel = LogDev;
	}

	TCHAR szAppData[MAX_PATH] = {0};
	SHGetSpecialFolderPath(NULL, szAppData, CSIDL_APPDATA, TRUE);

	TCHAR szLogDir[MAX_PATH] = {0};
	_sntprintf_s(szLogDir, _countof(szLogDir), _TRUNCATE, _T("%s%s"), szAppData, _T("\\duowan\\yy\\log"));

	//目录是否存在，否则创建
	if (GetFileAttributes(szLogDir) == INVALID_FILE_ATTRIBUTES)
	{
		TCHAR szTemp[MAX_PATH] = {0};
		_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s%s"), szAppData, _T("\\duowan"));
		::CreateDirectory(szTemp, NULL);
		_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s%s"), szAppData, _T("\\duowan\\yy"));
		::CreateDirectory(szTemp, NULL);
		_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s%s"), szAppData, _T("\\duowan\\yy\\log"));
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
		
	_tcsncpy_s(szLogName, _countof(szLogName), p1, p2 - p1);
	_tcsncat_s(szLogName, _countof(szLogName), _T(".lg"), _TRUNCATE);

	//检查是否需要做备份
	DeleteBackup(szLogDir, szLogName);

	//打开进程间Mutex
	wchar_t szMutexName[MAX_PATH] = {0};
	_sntprintf_s(szMutexName, _countof(szMutexName), _TRUNCATE, c_szLogMutexNameFmt, szLogName);
	_tcslwr_s(szMutexName, _countof(szMutexName));//Mutex名字是大小写敏感的
	m_hProcessMutex = ::CreateMutex(NULL, FALSE, szMutexName);
	assert(m_hProcessMutex != NULL);

	//打开Log文件
	m_szLogPath[_countof(m_szLogPath) - 1] = 0;
	_sntprintf_s(m_szLogPath, _countof(m_szLogPath), _TRUNCATE, _T("%s\\%s"), szLogDir, szLogName);
    m_hFile = ::CreateFile(m_szLogPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);
    
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        m_hFile = ::CreateFile(m_szLogPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, CREATE_NEW, 0, NULL);
        LOG_FILE_HEAD head = {0};
        head.dwFlag = c_dwFileFlag;
        head.dwVer = c_dwFileVersion;
        DWORD dwWriten = 0;
        ::WriteFile(m_hFile, &head, sizeof(head), &dwWriten, NULL);
    }
    else
    {
        //若存在，检测log版本号
        LOG_FILE_HEAD head = {0};
        DWORD dwReadBytes = 0;
        DWORD dwWriten    = 0;
        ::ReadFile(m_hFile, &head, sizeof(head), &dwReadBytes, NULL);

        if (head.dwVer != c_dwFileVersion)
        {
            head.dwVer = c_dwFileVersion;
            ::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);
            ::WriteFile(m_hFile, &head, sizeof(head), &dwWriten, NULL);
        }
	}
	assert(m_hFile != INVALID_HANDLE_VALUE);	

	HANDLE hThread = (HANDLE)_beginthreadex(0, 0, LogThread, 0, 0, 0);
	CloseHandle(hThread);
}

void CLog::CancelCacheWrite()
{
	m_bCacheWrite = FALSE;
	FlushLog();
}

LRESULT __stdcall CLog::RecvMsgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CLog::GetModuleNameByAddr(DWORD dwAddr, char *szName, UINT cch)
{
	typedef struct _PEB_LDR_DATA {                          // Size = 0x24
		ULONG           Length;                             // 00
		BOOLEAN         Initialized;                        // 04
		PVOID           SsHandle;                           // 08
		LIST_ENTRY      InLoadOrderModuleList;              // 0C
		LIST_ENTRY      InMemoryOrderModuleList;            // 14
		LIST_ENTRY      InInitializationOrderModuleList;    // 1C
		//  void *          EntryInProgress;                // 24
	} PEB_LDR_DATA, *PPEB_LDR_DATA;


	szName[0] = 0;

	void	*PEB = NULL,
			*Flink = NULL,
			*p = NULL;		
	PEB_LDR_DATA *pLdr = NULL;
	__asm
	{
		push edx
		mov edx,fs:[0x30]
		mov PEB,edx
		pop edx
	}
	pLdr = *(PEB_LDR_DATA **)((char *)PEB + 0x0c);
	Flink = &pLdr->InLoadOrderModuleList;
	p = pLdr->InLoadOrderModuleList.Flink;
	
	__try
	{
		do
		{
			void *pBaseAddress = *(void **)((char *)p + 0x18);
			assert(pBaseAddress && ((DWORD)pBaseAddress & 0xFFFF) == 0);
			if (!pBaseAddress)
				break;
				
			wchar_t *lpwszFullName = *( ( wchar_t ** )( ( unsigned char * )p + 0x28 ) );
			DWORD dwImageSize = *( ( DWORD * )( ( unsigned char * )p + 0x20 ) );

			if (dwAddr >= (DWORD)pBaseAddress && dwAddr < (DWORD)pBaseAddress + dwImageSize)
			{
				wchar_t *lpwszName = lpwszFullName;
				for (int i = 0; lpwszFullName[i] != 0; i++)
				{
					if (lpwszFullName[i] == L'\\')
						lpwszName = lpwszFullName + i + 1;
				}

				::WideCharToMultiByte(CP_ACP, 0, lpwszName, -1, szName, cch, NULL, NULL);

				break;
			}
			
			p = *( ( void ** )p);
		} while ( Flink != p );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		assert(0);
	}

	return szName[0] != 0;
}

void CLog::customizeUIPIPolicy( HWND hwnd, UINT message, DWORD dwFlag )
{
	typedef BOOL (WINAPI *PtrChangeWindowMessageFilter)(UINT message, DWORD dwFlag);
	typedef BOOL (WINAPI *PtrChangeWindowMessageFilterEx)(HWND hWnd, UINT message, DWORD action, void* pChangeFilterStruct);

	PtrChangeWindowMessageFilterEx pChangeWindowMessageFilterEx =
		(PtrChangeWindowMessageFilterEx)::GetProcAddress(::GetModuleHandle(_T("user32")), "ChangeWindowMessageFilterEx");

	// win7
	if (pChangeWindowMessageFilterEx != NULL) 
	{
		// Call the safer ChangeWindowMessageFilterEx API if available
		pChangeWindowMessageFilterEx(hwnd, message, dwFlag, 0);
	} 
	else 
	{
		// vista
		PtrChangeWindowMessageFilter pChangeWindowMessageFilter =
			(PtrChangeWindowMessageFilter)::GetProcAddress(::GetModuleHandle(_T("user32")), "ChangeWindowMessageFilter");

		if (pChangeWindowMessageFilter) 
		{
			// Call the deprecated ChangeWindowMessageFilter API otherwise
			pChangeWindowMessageFilter(message, dwFlag);
		}
	}
}

bool CheckDirectoryExist(LPCWSTR strFileName)
{
	DWORD dwAttri = ::GetFileAttributes(strFileName);
	if ((dwAttri == INVALID_FILE_ATTRIBUTES) || !(dwAttri & FILE_ATTRIBUTE_DIRECTORY))
	{
		return false;
	}
	return true;
}

void CLog::FlushLog()
{
#ifdef PERFERMANCE_TEST
	DWORD _dwLogCount = 0;
	DWORD64 _dw64Freq;
	DWORD64 _dw64Start;
	DWORD64 _dw64End;
	QueryPerformanceFrequency((LARGE_INTEGER *)&_dw64Freq);
	QueryPerformanceCounter((LARGE_INTEGER *)&_dw64Start);
#endif

	::EnterCriticalSection(&m_criLog);
	CVecLog vecLogCpy;
	vecLogCpy.swap(m_vecLog);
	::LeaveCriticalSection(&m_criLog);

#ifdef PERFERMANCE_TEST
	_dwLogCount = vecLogCpy.size();
#endif

	//进程互斥
	DWORD dwRes = ::WaitForSingleObject(m_hProcessMutex, INFINITE);
	assert(dwRes == WAIT_OBJECT_0);
	DW_UNUSED(dwRes);   

    DWORD dwLow = 0;
    DWORD dwHigh = 0;
    dwLow = ::GetFileSize(m_hFile, &dwHigh);
    assert(dwLow != INVALID_FILE_SIZE);
    if (dwLow > c_dwMaxFileSize || dwHigh != 0)
    {         
		//TCHAR bakPath[MAX_PATH] = {0};
		//TCHAR fileName[MAX_PATH] = {0};
		//_tcscpy(bakPath, m_szLogPath);

		////获取日志文件的所在路径
		//TCHAR *find = _tcsrchr(bakPath, '\\');
		//if (find != NULL)
		//{
		//	*find = '\0';

		//	//获取日志文件的名字
		//	TCHAR* temp = find+1;
		//	find = _tcsrchr(temp, '.');
		//	if (find != NULL)
		//	{
		//		*find = '\0';
		//	}
		//	_tcscpy(fileName, temp);
		//	wcscat(bakPath, _T("\\logbak\\"));

		//	if (!CheckDirectoryExist(bakPath))
		//	{
		//		::CreateDirectory(bakPath, NULL);
		//	}

		//	_stprintf_s(fileName, _countof(fileName), _T("%s-%u.lg"), fileName, time(NULL));  
		//	wcscat(bakPath, fileName);
		//}

		//::CopyFile(m_szLogPath, bakPath, FALSE);

		//::SetFilePointer(m_hFile, sizeof(LOG_FILE_HEAD), NULL, FILE_BEGIN);
		//::SetEndOfFile(m_hFile);

        const DWORD c_dwBufferSize = 128 * 1024;
        DWORD bytesRead = 0;
        char *tmpBuffer = new char[c_dwBufferSize + 1];          
        ::SetFilePointer(m_hFile, 0 - c_dwMaxSizeToRemain, NULL, FILE_END);//左移文件指针
        ::ReadFile(m_hFile, tmpBuffer, c_dwBufferSize, &bytesRead, NULL);  
        tmpBuffer[bytesRead] = '\0';

        DWORD nPos = 0;
        for (; nPos < bytesRead; nPos++)
        {
            if (*(DWORD *)(tmpBuffer+nPos) == c_dwCookie)
            {
                break;
            }
        }
        delete []tmpBuffer;
        tmpBuffer = NULL;

        if (nPos < bytesRead)
        {          
            nPos -= sizeof(LOG_CRYPT_BLOCK);//c_dwCookie作为两条日志间分割字符串 LOG_CRYPT_BLOCK作为每条日志的开始
            /*读取准备保留的旧的日志文件*/
            ::SetFilePointer(m_hFile, nPos - bytesRead, NULL, FILE_CURRENT);            

            DWORD dwNewMemorySize     = c_dwMaxSizeToRemain - nPos;//real size of the logfile to read
            OLD_LOGBUFFER_INFO *pInfo = (OLD_LOGBUFFER_INFO*)CMyHeapAlloc::GetInstance()->MyAlloc(dwNewMemorySize + sizeof(OLD_LOGBUFFER_INFO));
            if(pInfo)
            {//分配10M内存，增加分配成功判断,分配成功才截断LOG，hjx 2014.11.11
                BYTE * pBuffer = (BYTE*)(pInfo + 1);
                bytesRead      = 0;
                ::ReadFile(m_hFile, pBuffer, dwNewMemorySize, &bytesRead, NULL);
                pInfo->dwSize      = bytesRead;

                ::SetFilePointer(m_hFile, dwNewMemorySize + sizeof(LOG_FILE_HEAD), NULL, FILE_BEGIN);
                ::SetEndOfFile(m_hFile);//重新设置log文件大小等于 5*1024*1024 - (pos - tmpBuffer)

                HANDLE hThread = (HANDLE)_beginthreadex(0, 0, flushRemainedLog, (void*)pInfo, 0, 0);
                if (hThread != NULL)
                    CloseHandle(hThread);
            }
        }
    }

	::SetFilePointer(m_hFile, 0, NULL, FILE_END);

	const DWORD c_dwSizeCache = 1024 * 128; //用于写文件临时缓存
	BYTE *pCache = (BYTE*)CMyHeapAlloc::GetInstance()->MyAlloc(c_dwSizeCache);
	DWORD dwLen = 0;
	
	for (UINT i = 0; i < vecLogCpy.size(); i++)
	{
		{
			BYTE *pBuf = (BYTE*)vecLogCpy[i];
			LOG_ITEM* pItem = (LOG_ITEM*)(pBuf + sizeof(LOG_CRYPT_BLOCK));
			if (pItem->header.wType == (WORD)TYPE_E_CONTENT2)
			{
				const DWORD dwEncryptSize = pItem->cchFilter + pItem->cchFunc + pItem->cchCppName + pItem->cchModule + pItem->cchLog;
				const DWORD dwDataSize = sizeof(LOG_ITEM) + (dwEncryptSize + 7) / 8 * 8;
				for (unsigned long * p = (unsigned long*)((BYTE*)pItem + sizeof(LOG_ITEM)); p < (unsigned long*)((BYTE*)pItem + dwDataSize); p += 2)
				{
					encryptData(p);
				}
				pItem->header.wType = (WORD)TYPE_E_CONTENT;//修复崩溃报告没有带上日志的问题。
			}
		}
		if (dwLen + vecLogCpy[i]->dwBlockSize > c_dwSizeCache)
		{
			DWORD dwWriten = 0;
			if (dwLen > 0)
			{
				::WriteFile(m_hFile, pCache, dwLen, &dwWriten, NULL);
				dwLen = 0;
			}
			::WriteFile(m_hFile, vecLogCpy[i], vecLogCpy[i]->dwBlockSize, &dwWriten, NULL);
		}
		else
		{
			memcpy(pCache + dwLen, vecLogCpy[i], vecLogCpy[i]->dwBlockSize);
			dwLen += vecLogCpy[i]->dwBlockSize;
		}
	}
	
	if (dwLen > 0)
	{
		DWORD dwWriten = 0;
		::WriteFile(m_hFile, pCache, dwLen, &dwWriten, NULL);
	}
   
	CMyHeapAlloc::GetInstance()->MyFree(pCache);
	pCache = NULL;

	for (UINT i = 0; i < vecLogCpy.size(); i++)
		CMyHeapAlloc::GetInstance()->MyFree(vecLogCpy[i]);

	::ReleaseMutex(m_hProcessMutex);

	::EnterCriticalSection(&m_criLog);
	if (m_vecLog.size() >= c_dwMaxFlushCount)
		::SetEvent(m_hEventFlush);
	else
		::ResetEvent(m_hEventFlush);
	::LeaveCriticalSection(&m_criLog);

#ifdef PERFERMANCE_TEST
	QueryPerformanceCounter((LARGE_INTEGER *)&_dw64End);
	TCHAR _szPerf[1024] = {0};
	_sntprintf(_szPerf, _countof(_szPerf) - 1, _T("Flush log spend = %f ms, log count = %d\n"), (double)(_dw64End-_dw64Start)*1000/_dw64Freq, _dwLogCount);
	OutputDebugString(_szPerf);
#endif
}
unsigned int _stdcall CLog::flushRemainedLog(void* lpPara)
{
    //将lpPara中数据写到文件中
    CLog *pLog = CLog::GetInstance();

    HANDLE hFile = ::CreateFile(pLog->m_szLogPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);    
    assert(hFile != INVALID_HANDLE_VALUE);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten             = 0;
        OLD_LOGBUFFER_INFO *ptrBuffer = (OLD_LOGBUFFER_INFO*)(lpPara);
        DWORD bytesToWrite             = ptrBuffer->dwSize;
        BOOL writeFlag                 = true;
        BYTE *tmpPtr                   = (BYTE*)(ptrBuffer+1);

        ::SetFilePointer(hFile, sizeof(LOG_FILE_HEAD), NULL, FILE_BEGIN);
        writeFlag = ::WriteFile(hFile, tmpPtr, bytesToWrite, &bytesWritten, NULL);

        CloseHandle(hFile);
    }
    
    CMyHeapAlloc::GetInstance()->MyFree(lpPara);
    lpPara = NULL;

    return 0;
}
unsigned int _stdcall CLog::LogThread(void* )
{
	CLog *pLog = CLog::GetInstance();

	//初始化窗口
	wchar_t szWindowName[128] = {0};
	_sntprintf_s(szWindowName, _countof(szWindowName), _TRUNCATE, c_szLogWndNameFmt, ::GetCurrentProcessId());

	HMODULE hModule = ::GetModuleHandle(NULL);
	const TCHAR* lpszClass = _T("szDWLogWndClass");
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

	HWND hWnd = ::CreateWindowEx(0, lpszClass, szWindowName, 0,
		0, 0, 0, 0, HWND_MESSAGE, (HMENU)0, (HINSTANCE)hModule, 0);

	//避免消息权限问题
	customizeUIPIPolicy(hWnd, WM_APP_LOG, MSGFLT_ADD);

	MSG msg;
	while(1)
	{
		DWORD dwWaitRes = ::MsgWaitForMultipleObjects(1, &pLog->m_hEventFlush, FALSE, c_dwMaxIntervalFlush, QS_ALLINPUT);
		if (dwWaitRes == WAIT_OBJECT_0 + 1)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
			{				
				if (msg.message == WM_APP_LOG)
				{
					::EnterCriticalSection(&pLog->m_criLog);

					if (msg.wParam == eAddReceiver)
					{
						pLog->m_setReceiver.insert((HWND)msg.lParam);
					}
					else if (msg.wParam == eDelReceiver)
					{
						pLog->m_setReceiver.erase((HWND)msg.lParam);
					}
					else
					{
						assert(0);
					}

					::LeaveCriticalSection(&pLog->m_criLog);
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if (dwWaitRes == WAIT_OBJECT_0 || dwWaitRes == WAIT_TIMEOUT)
		{
			//Flush File
			pLog->FlushLog();
		}
		else
		{
			assert(!_T("MsgWaitForMultipleObjects failed"));
			break;
		}
	}

	return 0;
}

void CLog::DeleteBackup(const TCHAR *lpszDir, const TCHAR *lpszFileName)
{
	TCHAR szPath[MAX_PATH] = {0};
	_sntprintf_s(szPath, _countof(szPath), _TRUNCATE, _T("%s\\%s.bak"), lpszDir, lpszFileName);  
    ::DeleteFile(szPath);//若文件不存在 ErrorCode == //ERROR_FILE_NOT_FOUND
}

inline char * __cdecl __strrchr(
	const char * string,
	int ch
)
{
	const char *start = (const char *)string;
	/* search towards front */
	while (*string)
	{
		if (*string == (char)ch)
			start = string;
		++string;
	}
	if (*start == (char)ch)                /* char found ? */
		return((char *)start);
	return(NULL);
}

void CLog::AddLog(WORD		wLogLevel, 
				  LPCSTR	lpszFilter, 
				  LPCSTR	lpszCppName, 
				  WORD		wLineNumber,
				  LPCSTR	lpszFuncName, 
				  LPCWSTR	lpwszLog,
				  const char* szModuleName)
{
	LPCSTR lpszCppFileName = __strrchr(lpszCppName, '\\');
	if (lpszCppFileName)
		lpszCppFileName += 1;
	else
		lpszCppFileName = lpszCppName;

	if (m_pfFilterCallback)
		m_pfFilterCallback(wLogLevel, szModuleName, lpszFilter, lpwszLog);

	//生成一条log记录
	int cchFilter	= min(0xFFFF, strlen(lpszFilter));
	int cchFunc		= min(0xFFFF, strlen(lpszFuncName));
	int cchCppName	= min(0xFFFF, strlen(lpszCppFileName));
	int cchModule	= min(0xFFFF, strlen(szModuleName));
	int cchLog		= min(0xFFFF, utf16to8(lpwszLog, NULL, 0));
        
    const DWORD dwEncryptSize = cchFilter + cchFunc + cchCppName + cchModule + cchLog;
    const DWORD dwDataSize    = sizeof(LOG_ITEM) + (dwEncryptSize + 7) / 8 * 8;
    const DWORD dwBlockSize   = dwDataSize + sizeof(LOG_CRYPT_BLOCK);
    
	//分配和初始化LOG_FILE_ITEM
	BYTE *pBuf = (BYTE*)CMyHeapAlloc::GetInstance()->MyAlloc(max(512, dwBlockSize));

	LOG_ITEM* pItem = (LOG_ITEM*)(pBuf + sizeof(LOG_CRYPT_BLOCK));
	pItem->header.dwCookie = c_dwCookie;
	pItem->header.wReserved = 0;
	pItem->header.wType = (WORD)TYPE_E_CONTENT;

	pItem->dwYY			= m_dwYY;
	pItem->wLevel		= wLogLevel;
	pItem->wLine		= wLineNumber;
	pItem->dwProcessId	= ::GetCurrentProcessId();
	pItem->dwThreadId	= ::GetCurrentThreadId();
	::GetLocalTime(&pItem->stTime);

	pItem->cchFilter	= (WORD)cchFilter;
	pItem->cchFunc		= (WORD)cchFunc;
	pItem->cchCppName	= (WORD)cchCppName;
	pItem->cchModule	= (WORD)cchModule;
	pItem->cchLog		= (WORD)cchLog;
	
	BYTE *pTemp = (BYTE*)(pItem + 1);

	memcpy(pTemp, lpszFilter, cchFilter);
	pTemp += cchFilter;

	memcpy(pTemp, lpszFuncName, cchFunc);
	pTemp += cchFunc;

	memcpy(pTemp, lpszCppFileName, cchCppName);
	pTemp += cchCppName;
	
	memcpy(pTemp, szModuleName, cchModule);
	pTemp += cchModule; 

	utf16to8(lpwszLog, (char *)pTemp, cchLog);
	pTemp += cchLog;

	((LOG_CRYPT_BLOCK*)pBuf)->dwBlockSize = dwBlockSize;
	if (!m_setReceiver.empty())
	{
		//有logview时，需要实时显示log, 兼容logview，需要实时加密
		for (unsigned long * p = (unsigned long*)((BYTE*)pItem + sizeof(LOG_ITEM)); p < (unsigned long*)((BYTE*)pItem + dwDataSize); p += 2)
		{
			encryptData(p);
		}

		std::vector<HWND, CMyStlAlloc<HWND> > vecToolHwnd;
		::EnterCriticalSection(&m_criLog);
		vecToolHwnd.assign(m_setReceiver.begin(), m_setReceiver.end());
		::LeaveCriticalSection(&m_criLog);

		//发送给LogView Tool？
		COPYDATASTRUCT cd = { 0 };
		cd.cbData = dwBlockSize;
		cd.dwData = 0;
		cd.lpData = pBuf;
		for (UINT i = 0; i < vecToolHwnd.size(); ++i)
			::SendMessage(vecToolHwnd[i], WM_COPYDATA, NULL, (LPARAM)&cd);
	}
	else
	{
		//延迟加密
		pItem->header.wType = (WORD)TYPE_E_CONTENT2;
	}
	

    ::EnterCriticalSection(&m_criLog);
    if (m_vecLog.empty())
        m_vecLog.reserve(c_dwMaxFlushCount);
    m_vecLog.push_back((LOG_CRYPT_BLOCK*)pBuf);

    if (m_bCacheWrite)
    {		
       if (m_vecLog.size() >= c_dwMaxFlushCount)
		::SetEvent(m_hEventFlush);

        ::LeaveCriticalSection(&m_criLog);
    }
    else
    {
        ::LeaveCriticalSection(&m_criLog);
        FlushLog();
    }	
}

bool CLog::IsLogLevelEnabled( WORD wLogLevel ) const
{
    if (wLogLevel > m_nLogLevel && m_setReceiver.empty())
        return false;

    return true;
}

DWBASE_EXPORTS void __declspec(noinline)DoLog(WORD		wLogLevel, 
												   LPCSTR	lpszFilter, 
												   LPCSTR	lpszCppName, 
												   WORD		wLineNumber,
												   LPCSTR	lpszFuncName, 
												   LPCWSTR	lpwszLog)
{
	//是否需要输出
	if (!CLog::GetInstance()->IsLogLevelEnabled(wLogLevel))
		return;

	char szModuleName[MAX_PATH] = { 0 };
	CLog::GetModuleNameByAddr((DWORD)_ReturnAddress(), szModuleName, _countof(szModuleName));
	CLog::GetInstance()->AddLog(wLogLevel, lpszFilter, lpszCppName, wLineNumber, lpszFuncName, lpwszLog, szModuleName);
}

DWBASE_EXPORTS void __declspec(noinline)DoLog2(WORD		wLogLevel,
	LPCSTR	lpszFilter,
	LPCSTR	lpszCppName,
	WORD		wLineNumber,
	LPCSTR	lpszFuncName,
	LPCWSTR	lpwszLog,
	const char* lpszModuleName)
{
	//是否需要输出
	if (!CLog::GetInstance()->IsLogLevelEnabled(wLogLevel))
		return;

	CLog::GetInstance()->AddLog(wLogLevel, lpszFilter, lpszCppName, wLineNumber, lpszFuncName, lpwszLog, lpszModuleName);
}

static struct CAutoExecLogic
{
	CAutoExecLogic() 
	{
		char szModuleName[MAX_PATH] = { 0 };
		CLog::GetModuleNameByAddr((DWORD)_ReturnAddress(), szModuleName, _countof(szModuleName));

		CLog::GetInstance()->AddLog(LogFinal, "----", "----", 0, "----", L"Log 初始化...", szModuleName);
	}

	~CAutoExecLogic()
	{ 
		char szModuleName[MAX_PATH] = { 0 };
		CLog::GetModuleNameByAddr((DWORD)_ReturnAddress(), szModuleName, _countof(szModuleName));
		CLog::GetInstance()->AddLog(LogFinal, "----", "----", 0, "----", L"Log 对象析构...", szModuleName);
		CLog::GetInstance()->CancelCacheWrite();
	}

}s_autoexec;


DWBASE_EXPORTS void FlushLog(LPCWSTR lpszReason/* = NULL*/)
{
	if (lpszReason)
	{
		char szModuleName[MAX_PATH] = { 0 };
		CLog::GetModuleNameByAddr((DWORD)_ReturnAddress(), szModuleName, _countof(szModuleName));
		CLog::GetInstance()->AddLog(LogFinal, "----", "----", 0, "----", lpszReason, szModuleName);
	}	
	CLog::GetInstance()->FlushLog();
}

DWBASE_EXPORTS void SetLogFilter(LogFilterCallback pfCallback)
{
	CLog::GetInstance()->SetLogFilter(pfCallback);
}

DWBASE_EXPORTS void SetLogInfo(DWORD dwYY)
{
	CLog::GetInstance()->SetLogInfo(dwYY);
}

DWBASE_EXPORTS bool IsLogLevelEnabled(WORD wLogLevel)
{
	return CLog::GetInstance()->IsLogLevelEnabled(wLogLevel);
}

DWBASE_EXPORTS void SetLogMaximumFileSize(DWORD dwSize)
{
	c_dwMaxFileSize = dwSize;
}

DWBASE_EXPORTS DWORD GetLogMaximumFileSize()
{
	return c_dwMaxFileSize;
}