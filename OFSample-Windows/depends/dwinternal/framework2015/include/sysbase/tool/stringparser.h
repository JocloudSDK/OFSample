
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "string_helper.h"

namespace stringparser
{
	template <class T>
	inline bool Parse(LPCTSTR sz, T* pv)
	{
		if (sz == NULL)
			return false;

		std::wstringstream ss;
		ss << sz;

		ss >> *pv;
		return !ss.fail();
	}

	//str and ch
	template <>
	inline bool Parse(LPCTSTR sz, std::wstring* pstr)
	{
		if (sz == NULL)
			return false;

		pstr->assign(sz);
		return true;
	}

	template <>
	inline bool Parse(LPCTSTR sz, TCHAR* pch)
	{
		if (sz == NULL || sz[0] == 0)
			return false;

		*pch = sz[0];
		return true;
	}

	//bool
	template <>
	inline bool Parse(LPCTSTR sz, bool* pf)
	{
		if (sz == NULL)
			return false;

		sz = string_helper::_left_trim_cstr(sz, NULL);
		if (sz[0] == 0)
			return false;

		switch (sz[0])
		{
		case 't':
		case 'T':
		case 'y':
		case 'Y':
		case '1':
			*pf = true;
			return true;

		case 'f':
		case 'F':
		case 'n':
		case 'N':
		case '0':
			*pf = false;
			return true;
		}

		return false;
	}


	template <class T>
	inline bool Parse(LPCTSTR sz, T** pp); //disabled
	template <class T>
	inline bool Parse(LPCTSTR sz, const T** pp); //disabled



	//////////////////////////////////////////////////////////////////////////

	template <class T> inline
	T Parse2(LPCTSTR sz, const T& defV = T())
	{
		T v;
		if (! Parse(sz, &v))
			return defV;
		return v;
	}

	template <> inline bool Parse(LPCWSTR sz, GUID* pv)
	{
		return SUCCEEDED( CLSIDFromString((LPWSTR)sz, pv) );
	}
}
