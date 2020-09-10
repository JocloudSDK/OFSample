
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <string>
#include <sstream>

#include "stringizing.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//implement
namespace string_helper
{
	namespace __inlineimp
	{
		inline LPCTSTR __get_string_sz(TCHAR ch, TCHAR tempbuff[2])
			{tempbuff[0] = ch; tempbuff[1] = 0; return tempbuff;}
		inline LPCTSTR __get_string_sz(LPCTSTR sz, void*)
			{return sz != NULL ? sz : _T("");}
		//inline LPCTSTR __get_string_sz(const std::wstring& str, void*)
		//	{return str.c_str();}
		template <class AL> inline LPCTSTR __get_string_sz(const std::basic_string<TCHAR, std::char_traits<TCHAR>, AL>& str, void*)
			{return str.c_str();}

		inline size_t __get_string_len(TCHAR ch, const void*)
			{return ch != 0 ? 1 : 0;}
		inline size_t __get_string_len(LPCTSTR sz, const void*)
			{return sz != NULL ? _tcslen(sz) : 0;}
		//inline size_t __get_string_len(const std::wstring& str, const void*)
		//	{return str.length();}
		template <class AL> inline size_t __get_string_len(const std::basic_string<TCHAR, std::char_traits<TCHAR>, AL>& str, const void*)
			{return str.length();}


		template <class T> inline LPCTSTR __get_string_sz(const T& s)
			{return __get_string_sz(s, NULL);}
		inline LPCTSTR __get_string_sz(TCHAR c);	//disabled
		template <class T> inline size_t __get_string_len(const T& s)
			{return __get_string_len(s, NULL);}
		inline size_t __get_string_len(TCHAR c);	//disabled



		struct __AUTO_END_VA
		{
			va_list va;
			~__AUTO_END_VA() {va_end(va);}
		};


		inline bool __is_wild_match(LPCTSTR szS, LPCTSTR szWild, bool fCaseSensitive)
		{
			for ( ; ; )
			{
				{{
					//find next wild char
					LPCTSTR szNextWC = szWild;
					while (*szNextWC != 0)
					{
						if (*szNextWC == '*' || *szNextWC == '?')
							break;
					}

					//and cmp part of before wild char
					if (szNextWC != szWild)
					{
						if ((fCaseSensitive ? _tcsncmp(szS, szWild, szNextWC - szWild) : _tcsnicmp(szS, szWild, szNextWC - szWild)) != 0)
							return false;

						//then goto wild char
						szS += szNextWC - szWild;
						szWild = szNextWC;
					}
				}}

				if (*szWild == 0)
					return (*szS == 0);	//fin

				if (*szWild == '?')
				{
					if (*szS == 0)
						return false;	//less
					++ szWild; //go over '?'
					++ szS; //led by '?'
					continue;
				}


				//if (*szWild == '*')

				++ szWild; //go over '*'
				for ( ; ; )
				{
					if (__is_wild_match(szS, szWild, fCaseSensitive))
						return true;
					if (*szS == 0)
						return false;	//less
					++ szS; //led by '*', go forward, and try again
				} //for ( ; ; )
			} //for ( ; ; )

		} //bool __is_wild_match(...

		inline unsigned __char_numeric(TCHAR ch)
		{
#ifdef _UNICODE
			return (unsigned)(wchar_t)ch;
#else
			return (unsigned)(unsigned char)ch;
#endif
		}
	} //namespace __inlineimp
} //namespace string_helper

namespace string_helper
{
	inline
	LPCTSTR _left_trim_cstr(LPCTSTR sz, LPCTSTR szEnd)
	{
		if (sz == NULL)
			return NULL;
		while ((szEnd != NULL ? sz < szEnd : *sz != 0) && isspace(*sz))
			++ sz;
		return sz;
	}

	template <class T> inline
	bool is_string_empty(const T& s)
	{
		return *__inlineimp::__get_string_sz(s) == 0;
	}

	template <class T, class T2> inline
	bool is_string_begin_with(const T& s, const T2& with, bool fCaseSensitive)
	{
		TCHAR tempbuff[2];
		return (fCaseSensitive	? _tcsncmp(__inlineimp::__get_string_sz(s), __inlineimp::__get_string_sz(with, tempbuff), __inlineimp::__get_string_len(with, tempbuff)) 
			: _tcsnicmp(__inlineimp::__get_string_sz(s), __inlineimp::__get_string_sz(with, tempbuff), __inlineimp::__get_string_len(with, tempbuff)) )
			== 0;
	}
}

#include "stringparser.h"
