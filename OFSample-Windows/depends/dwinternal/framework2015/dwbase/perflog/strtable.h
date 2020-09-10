/** 
@file
@brief Stable Table
@version 2011-9-16 bobdeng
*/

#pragma once

#include "stlalloc.h"

namespace Perf
{
	class CStrTable
	{
	public:
		static CStrTable *GetInstance();
		WORD Add(LPCSTR lpszName);
		DWORD Save(HANDLE hFile);

	private:
		CStrTable();

	private:
		typedef std::map<std::string, WORD, std::less<std::string>, CMyStlAlloc<std::pair<std::string, WORD> > > CMapStrTable;
		CMapStrTable												m_mapStrTable;

		CRITICAL_SECTION	m_cri;
	};
}
