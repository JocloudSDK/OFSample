/** 
@file
@brief Logϵͳʵ�ֵ�ͷ�ļ�
@version 2011-4-18 bobdeng
*/

#pragma once
#include <stdio.h> 
#include <string>
#include <assert.h> 

#include "dwglobal.h"
#include "filterdefine.h"//Log������ͳһ�����ͷ�ļ�����

/*********************************************************************************

Log�÷�ʾ����
	LogFinal(LOG::FILTER_NAME) << "YYChecker Error: " << reg.c_str();

	ע�⣺FILTER_NAME��Log ��������ͳһ���ļ�filterdefine.h���壬���ڷ������
/*********************************************************************************/



//LogFinal ���ڼ�¼�����һЩ�ؼ����й켣�����������ڱ���ǰ�ử��һЩ����ͼ���ں�ͬ�²���ǰҲ����Ʋ���������LogFinal�����������
//Ĭ�Ͽ���
#define LogFinal(filter_name)	CLogStream(filter_name, 1, __FILE__, (WORD)__LINE__, __FUNCTION__)

//LogDev ���ڼ�¼��LogFinal����ϸ��Log�������շ���ÿ���������Ϣ�������е��¼������������е�������֧�������Դ�LogDev��
//���Ի����¿������ⷢ�汾�ر�
#define LogDev(filter_name)		CLogStream(filter_name, 2, __FILE__, (WORD)__LINE__, __FUNCTION__)

//LogTemp �����ڲ�Ʒ�������Թ����У�����Ϊ�����ǰ��ĳ����������������ʱLog�������ⶨλ�󣬿�������������ص�Log��������ʱLogԽ��Խ�࣬Ӱ��������ۺͲ�Ʒ�������ܡ��汾�ⷢǰ��Ҫ��鲢������Щ��ʱ��Log��
//���Ի����¿������ⷢ�汾�ر�
#define LogTemp(filter_name)	CLogStream(filter_name, 3, __FILE__, (WORD)__LINE__, __FUNCTION__)


#define TRACESLOT				LogFinal("CommonSlot")

__declspec(selectany) char gsModuleFullPathName[512] = { 0 };
inline const char* GetCurrentModuleName()
{
	HMODULE hModule;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, 
		(LPTSTR)gsModuleFullPathName, &hModule);

	GetModuleFileNameA(hModule, gsModuleFullPathName, 512);
	char* lpwszFullName = gsModuleFullPathName;

	char *lpwszName = lpwszFullName;
	for (int i = 0; lpwszFullName[i] != 0; i++)
	{
		if (lpwszFullName[i] == '\\')
			lpwszName = lpwszFullName + i + 1;
	}
	return lpwszName;
}
__declspec(selectany) const char* gsModuleFileName = GetCurrentModuleName();

#ifdef NO_DWBASE_DEPENDENT
	__declspec(selectany) void* g_pfDoLog = NULL;
	static void DoLogDelegate(WORD wLogLevel, LPCSTR lpszFilter, LPCSTR lpszCppName, WORD wLineNumber, LPCSTR lpszFuncName, LPCWSTR lpwszLog, const char* lpszModuleName)
	{
		if (!g_pfDoLog)
		{
			HMODULE hModule = ::GetModuleHandleW(L"dwbase.dll");
			g_pfDoLog = (void*)::GetProcAddress(hModule, "?DoLog2@@YAXGPBD0G0PBG0@Z");
			assert(g_pfDoLog);
		}
		
		if (g_pfDoLog)
        {
			((void (__cdecl *)(WORD, LPCSTR, LPCSTR, WORD, LPCSTR, LPCWSTR, const char*))g_pfDoLog)(wLogLevel, lpszFilter, lpszCppName, wLineNumber, lpszFuncName, lpwszLog, lpszModuleName);
        }
	}
    __declspec(selectany) void* g_pfIsLogLevelEnabled = NULL;
    static bool IsLogLevelEnabledDelegate(WORD wLogLevel)
    {
        if( !g_pfIsLogLevelEnabled )
        {
            HMODULE hModule = ::GetModuleHandleW(L"dwbase.dll");
            g_pfIsLogLevelEnabled = (void*)::GetProcAddress(hModule, "?IsLogLevelEnabled@@YA_NG@Z");
            assert(g_pfIsLogLevelEnabled);
        }

        if (g_pfIsLogLevelEnabled)
        {
            return ((bool (__cdecl *)(WORD))g_pfIsLogLevelEnabled)(wLogLevel);
        }

        return false;
    }
#else
	//dwProcessId == 0 ��ʾƥ��ȫ������
	//dwYY == 0 ��ʾƥ��ȫ��YY����
	DWBASE_EXPORTS void GetLastLog(LPCTSTR lpszLogName, DWORD dwProcessId, DWORD dwYY, char *pBufDest, DWORD dwBufSize);
	//����YY������Ϣ
	DWBASE_EXPORTS void SetLogInfo(DWORD dwYY);
	DWBASE_EXPORTS void FlushLog(LPCWSTR lpszReason = NULL);//�����쳣���ˢ�»��浽���̣������ڴ��е�Log��ʧ
	typedef void (* LogFilterCallback)(WORD wLogLevel, LPCSTR lpszModuleName, LPCSTR lpszFilter, LPCWSTR lpwszLog);
	DWBASE_EXPORTS void SetLogFilter(LogFilterCallback pfCallback);
	DWBASE_EXPORTS void DoLog(WORD wLogLevel, LPCSTR lpszFilter, LPCSTR lpszCppName, WORD wLineNumber, LPCSTR lpszFuncName, LPCWSTR lpwszLog);
	DWBASE_EXPORTS void DoLog2(WORD wLogLevel, LPCSTR lpszFilter, LPCSTR lpszCppName, WORD wLineNumber, LPCSTR lpszFuncName, LPCWSTR lpwszLog, const char* lpszModuleName);
    DWBASE_EXPORTS bool IsLogLevelEnabled(WORD wLogLevel);
	DWBASE_EXPORTS bool IsLogLevelEnabled(WORD wLogLevel);
	DWBASE_EXPORTS void SetLogMaximumFileSize(DWORD dwSize); // Byte, default is 100 * 1024 * 1024
	DWBASE_EXPORTS DWORD GetLogMaximumFileSize();
#endif



struct CLogStream
{
	inline CLogStream(LPCSTR lpszFilter, WORD wLogLevel, LPCSTR lpszCppName, WORD wLineNumber, LPCSTR lpszFuncName)	
	{ 
		m_szBuf[0] = 0;
		m_lpwszData = m_szBuf;
		m_cchMax = sizeof(m_szBuf)/sizeof(wchar_t) - 1;
		m_cchData = 0;
		m_lpszFilter = lpszFilter;
		m_wLogLevel = wLogLevel; 
		m_lpszCppName = lpszCppName; 
		m_wLineNumber = wLineNumber;
		m_lpszFuncName = lpszFuncName;

#ifdef NO_DWBASE_DEPENDENT
        m_enableLog = IsLogLevelEnabledDelegate(wLogLevel);        
#else        
        m_enableLog = IsLogLevelEnabled(wLogLevel);
#endif
	}
	inline ~CLogStream()
	{
#ifndef NO_DWBASE_DEPENDENT
		DoLog2(m_wLogLevel, m_lpszFilter, m_lpszCppName, m_wLineNumber, m_lpszFuncName, m_lpwszData, gsModuleFileName);
#else
		DoLogDelegate(m_wLogLevel, m_lpszFilter, m_lpszCppName, m_wLineNumber, m_lpszFuncName, m_lpwszData, gsModuleFileName);
#endif
		if (m_lpwszData != m_szBuf) delete [] m_lpwszData; 
	}
	
	inline CLogStream &write2(bool b)				{ if (!m_enableLog) return *this; write2(b == true ? L"true" : L"false");  return (*this); }
    inline CLogStream &write2(char ch)				{ if(!m_enableLog) return *this;  write2((signed long)ch); return (*this);}
    inline CLogStream &write2(signed short i)		{ if(!m_enableLog) return *this; write2((signed long)i); return (*this);}
    inline CLogStream &write2(signed int i)			{ if(!m_enableLog) return *this; write2((signed long)i); return (*this);}
    inline CLogStream &write2(unsigned int i)		{ if(!m_enableLog) return *this; write2((unsigned long)i); return (*this);}
	inline CLogStream &write2(unsigned short i)	    {if (!m_enableLog) return *this; write2((unsigned long)i); return (*this);}
	inline CLogStream &write2(float f)				{if (!m_enableLog) return *this; write2((double)f); return (*this);}
	inline CLogStream &write2(unsigned long i)		{ if(!m_enableLog) return *this; int n = _snwprintf_s(GetBuf(32), 32, _TRUNCATE, L"%u", i);	if (n > 0) m_cchData += n; m_lpwszData[m_cchData] = 0; return *this; }
    inline CLogStream &write2(signed long i)		{ if(!m_enableLog) return *this; int n = _snwprintf_s(GetBuf(32), 32, _TRUNCATE, L"%d", i);	if (n > 0) m_cchData += n; m_lpwszData[m_cchData] = 0; return *this; }
	inline CLogStream &write2(__int64 i)			{ if(!m_enableLog) return *this; int n = _snwprintf_s(GetBuf(32), 32, _TRUNCATE, L"%I64d", i); if (n > 0) m_cchData += n; m_lpwszData[m_cchData] = 0; return *this; }
    inline CLogStream &write2(unsigned __int64 i)	{ if(!m_enableLog) return *this; int n = _snwprintf_s(GetBuf(32), 32, _TRUNCATE, L"%I64u", i); if (n > 0) m_cchData += n; m_lpwszData[m_cchData] = 0; return *this; }
    inline CLogStream &write2(double f)				{ if(!m_enableLog) return *this; int n = _snwprintf_s(GetBuf(32), 32, _TRUNCATE, L"%0.6f", f);	if (n > 0) m_cchData += n; m_lpwszData[m_cchData] = 0; return *this; }
	inline CLogStream &write2(const void *ptr)		{ if (!m_enableLog) return *this; int n = _snwprintf_s(GetBuf(32), 32, _TRUNCATE, L"0x%08X", (unsigned long)ptr);	if (n > 0) m_cchData += n; m_lpwszData[m_cchData] = 0; return *this; }
	inline CLogStream &write2(const wchar_t* s)		{ write(s, wcslen(s));  return *this; }
	inline CLogStream &write2(const char *s)		{  write(s, strlen(s)); return *this; }
	inline CLogStream &write2(const std::string &s) { write(s.c_str(), s.length()); return *this; }
	inline CLogStream &write2(const std::wstring &s) { write(s.c_str(), s.length()); return *this; }

	inline void write(const wchar_t* s, size_t len)
	{
		if (!m_enableLog)
			return;
		memcpy(GetBuf(len), s, len * sizeof(wchar_t)); m_cchData += len; m_lpwszData[m_cchData] = 0;
	}
	inline void write(const char* s, size_t len)
	{
		if (!m_enableLog)
			return;
		int n = MultiByteToWideChar(CP_ACP, 0, s, len, GetBuf(len), len); m_cchData += n; m_lpwszData[m_cchData] = 0;
	}
private:
	inline wchar_t * GetBuf(UINT cch) 
	{
		if (m_cchData + cch > m_cchMax)
		{
			UINT cchAlloc = sizeof(m_szBuf)/sizeof(wchar_t);
			while (cchAlloc < m_cchData + cch + 1) cchAlloc *= 2;

			wchar_t *p = new wchar_t[cchAlloc];
			memcpy(p, m_lpwszData, m_cchData * sizeof(wchar_t));
			p[m_cchData] = 0;
			if (m_lpwszData != m_szBuf)
				delete [] m_lpwszData;
			m_lpwszData = p;
			m_cchMax = cchAlloc - 1;
		}
		return m_lpwszData + m_cchData;
	}
	wchar_t		m_szBuf[512];
	wchar_t *	m_lpwszData;
	UINT		m_cchData;
	UINT		m_cchMax;
	
	LPCSTR		m_lpszFilter;
	WORD		m_wLogLevel;
	LPCSTR		m_lpszCppName;
	WORD		m_wLineNumber;
	LPCSTR		m_lpszFuncName;
    bool        m_enableLog;
};

template<class T, size_t size>
inline CLogStream& operator<<(CLogStream& log, const T(&ar)[size])
{
	log.write(ar, size - 1);
	return log;
}
template<class T>
inline CLogStream& operator<<(CLogStream& log, const T& t) 
{ 
	log.write2(t);
	return log;
}

#ifdef QT_DLL
    #include <QString>
    inline CLogStream &operator<<(CLogStream &log, const QString &s)
    {
		
		log.write(reinterpret_cast<const wchar_t*>(s.utf16()), s.length());
        return log;
    }
#endif