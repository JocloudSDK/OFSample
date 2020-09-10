
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <tchar.h>
#include <objbase.h>

namespace stringizing
{
	template <class T>
	inline std::wstring ToString(const T& v)
	{
		std::wstringstream ss;
		ss << v;
		return ss.str();
	}

	template <>
	inline std::wstring ToString(const std::wstring& str)
	{
		return str;
	}
	template <>
	inline std::wstring ToString(const TCHAR& ch)
	{
		if (ch == 0)
			return _T("");
		return std::wstring(1, ch);
	}

	template <>
	inline std::wstring ToString(const bool& f)
	{
		return f ? _T("true") : _T("false");
	}
	
	template <class T>
	inline std::wstring ToString(const T* p); //disabled

	template <>
	inline std::wstring ToString(LPCTSTR sz)
	{
		return sz != NULL ? sz : _T("");
	}

	template<> 
	inline std::wstring ToString(const unsigned int& n)
	{
		TCHAR buffer[16] = { 0 };
		_ultow_s(n, buffer, 16, 10);
		return buffer;
	}
//	template <>
//	inline std::wstring ToString(const void* p)
//	{
//		typedef void	T;
//		return string_helper::string_format(L"%p", p);
//	}

	template <> inline std::wstring ToString(const GUID& v)
	{
		WCHAR buf[64] = L"";
		::StringFromGUID2(v, buf, _countof(buf));
		return buf;
	}
}

