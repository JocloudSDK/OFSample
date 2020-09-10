/** 
@file
@brief 线程信息表
@version 2011-9-16 bobdeng
*/


#include "stable.h"
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include "threadtable.h"
#include "strtable.h"
#include "perflog.h"

namespace Perf
{
	CThreadTable *CThreadTable::GetInstance()
	{
		static CThreadTable *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CThreadTable), TRUE)) CThreadTable();

		return s_pImpl;
	}

	CThreadTable::CThreadTable()
	{
		::InitializeCriticalSection(&m_cri);
	}

	void CThreadTable::SetThreadName(DWORD dwTID, LPCSTR lpszThreadName)
	{
		assert(dwTID != 0);

		::EnterCriticalSection(&m_cri);

		CMapThread::iterator it = m_mapThread.find(dwTID);
		if (it == m_mapThread.end())
		{
			THREAD stThread;
			stThread.info.dwTID = dwTID;
			stThread.info.wNameID = AddToStrTable(lpszThreadName);
			stThread.strName = lpszThreadName;
			m_mapThread.insert(CMapThread::value_type(dwTID, stThread));
		}
		else
		{
			it->second.info.wNameID = AddToStrTable(lpszThreadName);
		}

		::LeaveCriticalSection(&m_cri);
	}

	BOOL CThreadTable::GetThreadName(DWORD dwTID, char* lpszBuf, UINT cch)
	{
		BOOL bRet = FALSE;
		lpszBuf[0] = 0;

		::EnterCriticalSection(&m_cri);
		CMapThread::iterator it = m_mapThread.find(dwTID);
		if (it != m_mapThread.end())
		{
			strncpy_s(lpszBuf, cch, it->second.strName.c_str(), _TRUNCATE);
			bRet = TRUE;
		}
		::LeaveCriticalSection(&m_cri);

		return bRet;
	}

	DWORD CThreadTable::Save(HANDLE hFile)
	{
		::EnterCriticalSection(&m_cri);

		DWORD dwWritten = 0;

		THREAD_TABLE stHead;
		stHead.header.dwSize = sizeof(stHead) + m_mapThread.size() * sizeof(THREAD_INFO);
		stHead.header.dwFlag = FLAG_E_THREAD_TABLE;
		stHead.dwCount = m_mapThread.size();

		::WriteFile(hFile, &stHead, sizeof(stHead), &dwWritten, 0);

		for (CMapThread::iterator it = m_mapThread.begin(); it != m_mapThread.end(); ++it)
			::WriteFile(hFile, &it->second.info, sizeof(it->second.info), &dwWritten, 0);

		::LeaveCriticalSection(&m_cri);

		return stHead.header.dwSize;
	}
}
