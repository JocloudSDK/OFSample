/** 
@file
@brief 线程信息表
@version 2011-9-16 bobdeng
*/


#pragma once

#include "stlalloc.h"
#include "logfile.h"

namespace Perf
{
	class CThreadTable
	{
	public:
		static CThreadTable *GetInstance();
		void SetThreadName(DWORD dwTID, LPCSTR lpszThreadName);
		DWORD Save(HANDLE hFile);
		BOOL GetThreadName(DWORD dwTID, char* lpszBuf, UINT cch);

	private:
		CThreadTable();

	private:

		struct THREAD
		{
			THREAD_INFO info;
			std::string strName;
		};
		typedef std::map<DWORD, THREAD, std::less<DWORD>, CMyStlAlloc<std::pair<DWORD, THREAD> > > CMapThread;
		CMapThread	m_mapThread;

		CRITICAL_SECTION	m_cri;
	};
}
