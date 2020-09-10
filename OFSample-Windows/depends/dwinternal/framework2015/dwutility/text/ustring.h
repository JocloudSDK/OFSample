
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <string>
#include <vector>

#include "dwglobal.h"

namespace DwUtility
{
	namespace text
	{
		void DW_DWUTILITY_EXPORT toWideString(const char* pStr, int len,  std::wstring& ret, UINT CodePage = CP_ACP);
		std::wstring DW_DWUTILITY_EXPORT toWideString(const char* pStr, int len,UINT CodePage = CP_ACP );

		void DW_DWUTILITY_EXPORT toNarrowString(const wchar_t* pStr, int len, std::string& out, UINT CodePage = CP_ACP);
		std::string DW_DWUTILITY_EXPORT toNarrowString(const wchar_t* pStr, int len, UINT CodePage = CP_ACP);

		void DW_DWUTILITY_EXPORT splitWString(const std::wstring &stringOrg, const std::wstring & delimiter, std::vector<std::wstring> &vectorResult);

		std::string DW_DWUTILITY_EXPORT getHashString( const char *hashData, int length );

		std::wstring DW_DWUTILITY_EXPORT Char2String(unsigned char c);

		std::wstring DW_DWUTILITY_EXPORT BinToHex(const std::string& bin);

		std::string DW_DWUTILITY_EXPORT URLEncode2(const std::string& src);
		std::string DW_DWUTILITY_EXPORT URLEncode2(const std::wstring& str);
		std::wstring DW_DWUTILITY_EXPORT URLEncode(const std::wstring& src);

        bool DW_DWUTILITY_EXPORT isUtf8Text(const char *text, size_t size);

		std::string DW_DWUTILITY_EXPORT toBase64String(const void *data, size_t size);

		std::string DW_DWUTILITY_EXPORT StringFormatA(const char *format, ...);
		std::wstring DW_DWUTILITY_EXPORT StringFormatW(const wchar_t *format, ...);

		void DW_DWUTILITY_EXPORT splitStringA( const std::string &stringOrg, const std::string & delimiter, std::vector<std::string> &vectorResult );
	}
}
