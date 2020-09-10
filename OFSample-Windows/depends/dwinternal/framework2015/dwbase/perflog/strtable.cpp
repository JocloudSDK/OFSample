/** 
@file
@brief 字符串表
*/

#include "stable.h"
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include "strtable.h"
#include "perflog.h"
#include "logfile.h"

namespace Perf
{
	CStrTable *CStrTable::GetInstance()
	{
		//static CStrTable *s_pImpl = NULL;
		//if (!s_pImpl)
		//	s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CStrTable), TRUE)) CStrTable();
		
		//改为静态变量，退出时自动析构，否则会报内存泄漏
		static CStrTable s_Impl;
			
		return &s_Impl;
	}

	CStrTable::CStrTable()
	{
		::InitializeCriticalSection(&m_cri);
	}

	WORD CStrTable::Add(LPCSTR lpszName)
	{
		WORD wRet = 0;
		
		::EnterCriticalSection(&m_cri);

		WORD wIndex = m_mapStrTable.size();
		assert(wIndex <= 0xFFFF);
		std::pair<CMapStrTable::iterator, bool> res = m_mapStrTable.insert(CMapStrTable::value_type(lpszName, wIndex));
		wRet = res.first->second;
		
		::LeaveCriticalSection(&m_cri);

		return wRet;
	}

	DWORD CStrTable::Save(HANDLE hFile)
	{
		::EnterCriticalSection(&m_cri);
		
		DWORD dwWriten = 0;
		DWORD dwTotalLen = 0;	

		std::map<WORD, std::string> mapSort;
		for (CMapStrTable::iterator it = m_mapStrTable.begin(); it != m_mapStrTable.end(); ++it)
		{
			if (mapSort.insert(std::map<WORD, std::string>::value_type(it->second, it->first)).second)
				dwTotalLen += it->first.size() + 1;
			else
				assert(0);
		}


		STR_TABLE_BLOCK_HEAD head = {0};
		head.header.dwSize = sizeof(head) + dwTotalLen;
		head.header.dwFlag = FLAG_E_STR_TABLE;
		::WriteFile(hFile, &head, sizeof(head), &dwWriten, NULL);

		for (std::map<WORD, std::string>::iterator it = mapSort.begin(); it != mapSort.end(); ++it)
		{
			dwWriten = 0;
			::WriteFile(hFile, it->second.c_str(), it->second.size() + 1, &dwWriten, NULL);
		}
		
		::LeaveCriticalSection(&m_cri);

		return head.header.dwSize;
	}
}
