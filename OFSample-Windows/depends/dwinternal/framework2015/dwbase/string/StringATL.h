#pragma once

#ifndef _ATL_NO_PRAGMA_WARNINGS
#pragma warning (push)
#pragma warning(disable : 4668)	// is not defined as a preprocessor macro, replacing with '0' for '#if/#elif
#pragma warning(disable : 4820)	// padding added after member
#pragma warning(disable : 4127)  // conditional expression constant
#endif //!_ATL_NO_PRAGMA_WARNINGS

#pragma warning(disable:4786)	// avoid 255-character limit warnings

#include "StringSimp.h"

#ifndef _AFX
#define _AFX_FUNCNAME(_Name) _Name
#endif

#pragma push_macro("new")
#undef new

#if defined(_AFX)
#pragma push_macro("FormatMessage")
#undef FormatMessage
#endif

/////////////////////////////////////////////////////////////////////////////
// Naming conventions:
//    The term "length" can be confusing when dealing with ANSI, Unicode, and
//    MBCS character sets, so this file will use the following naming 
//    conventions to differentiate between the different meanings of 
//    "length":
//
//    'Byte Length' - Length of a buffer in bytes, regardless of character 
//       size
//    'Char Length' - Number of distinct characters in string.  For wide-
//       character strings, this is equivalent to half the 'Byte Length'.  
//       For ANSI strings, this is equivalent to the 'Byte Length'.  For MBCS
//       strings, 'Char Length' counts a lead-byte/trail-byte combination
//       as one character.
//    'Length' - When neither of the above terms is used, 'Length' refers to 
//       length in XCHARs, which is equal to 'Byte Length'/sizeof(XCHAR).
/////////////////////////////////////////////////////////////////////////////

namespace ATL_STR
{
/////////////////////////////////////////////////////////////////////////////
// inline helpers
inline int _wcstombsz(_Out_cap_(count) char* mbstr, _In_z_ const wchar_t* wcstr, _In_ ULONG count) throw()
{
	// count is number of bytes
	if (count == 0 && mbstr != NULL)
		return 0;

	int result = ::WideCharToMultiByte(CP_THREAD_ACP, 0, wcstr, -1,
		mbstr, count, NULL, NULL);
	ATLASSERT(mbstr == NULL || result <= (int)count);
	return result;

	return 0;
}

inline int _mbstowcsz(_Out_z_cap_(count) wchar_t* wcstr, _In_z_ const char* mbstr, _In_ ULONG count)
{
	// count is number of wchar_t's
	if (count == 0 && wcstr != NULL)
		return 0;

	int result = ::MultiByteToWideChar(CP_THREAD_ACP, 0, mbstr, -1,
		wcstr, count);
	ATLENSURE(wcstr == NULL || result <= (int)count);
	if ((result > 0) && (wcstr != NULL))
	{
		wcstr[result-1] = 0;
	}

	return result;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
template< typename _CharType = char>
class ChTraitsCRT :	public ChTraitsBase< _CharType >
{
public:
	static char* __cdecl CharNext( _In_z_ const char* p ) throw()
	{
		return reinterpret_cast< char* >( _mbsinc( reinterpret_cast< const unsigned char* >( p ) ) );
	}

	static int __cdecl IsDigit( _In_ char ch ) throw()
	{
		return _ismbcdigit( ch );
	}

	static int __cdecl IsSpace( _In_ char ch ) throw()
	{
		return _ismbcspace( ch );
	}

	static int __cdecl StringCompare( _In_z_ LPCSTR pszA, _In_z_ LPCSTR pszB ) throw()
	{
		return _mbscmp( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static int __cdecl StringCompareIgnore( _In_z_ LPCSTR pszA, _In_z_ LPCSTR pszB ) throw()
	{
		return _mbsicmp( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static int __cdecl StringCollate( _In_z_ LPCSTR pszA, _In_z_ LPCSTR pszB ) throw()
	{
		return _mbscoll( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static int __cdecl StringCollateIgnore( _In_z_ LPCSTR pszA, _In_z_ LPCSTR pszB ) throw()
	{
		return _mbsicoll( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static LPCSTR __cdecl StringFindString( _In_z_ LPCSTR pszBlock, _In_z_ LPCSTR pszMatch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbsstr( reinterpret_cast< const unsigned char* >( pszBlock ),
			reinterpret_cast< const unsigned char* >( pszMatch ) ) );
	}

	static LPSTR __cdecl StringFindString( _In_z_ LPSTR pszBlock, _In_z_ LPCSTR pszMatch ) throw()
	{
		return( const_cast< LPSTR >( StringFindString( const_cast< LPCSTR >( pszBlock ), pszMatch ) ) );
	}

	static LPCSTR __cdecl StringFindChar( _In_z_ LPCSTR pszBlock, _In_ char chMatch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbschr( reinterpret_cast< const unsigned char* >( pszBlock ), (unsigned char)chMatch ) );
	}

	static LPCSTR __cdecl StringFindCharRev( _In_z_ LPCSTR psz, _In_ char ch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbsrchr( reinterpret_cast< const unsigned char* >( psz ), (unsigned char)ch ) );
	}

	static LPCSTR __cdecl StringScanSet( _In_z_ LPCSTR pszBlock, _In_z_ LPCSTR pszMatch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbspbrk( reinterpret_cast< const unsigned char* >( pszBlock ),
			reinterpret_cast< const unsigned char* >( pszMatch ) ) );
	}

	static int __cdecl StringSpanIncluding( _In_z_ LPCSTR pszBlock, _In_z_ LPCSTR pszSet ) throw()
	{
		return (int)_mbsspn( reinterpret_cast< const unsigned char* >( pszBlock ), reinterpret_cast< const unsigned char* >( pszSet ) );
	}

	static int __cdecl StringSpanExcluding( _In_z_ LPCSTR pszBlock, _In_z_ LPCSTR pszSet ) throw()
	{
		return (int)_mbscspn( reinterpret_cast< const unsigned char* >( pszBlock ), reinterpret_cast< const unsigned char* >( pszSet ) );
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringUppercase")
	static LPSTR __cdecl StringUppercase( _Inout_ LPSTR psz ) throw()
	{
#pragma warning (push)
#pragma warning(disable : 4996)
		return reinterpret_cast< LPSTR >( _mbsupr( reinterpret_cast< unsigned char* >( psz ) ) );
#pragma warning (pop)
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringLowercase")
	static LPSTR __cdecl StringLowercase( _Inout_ LPSTR psz ) throw()
	{
#pragma warning (push)
#pragma warning(disable : 4996)
		return reinterpret_cast< LPSTR >( _mbslwr( reinterpret_cast< unsigned char* >( psz ) ) );
#pragma warning (pop)
	}

	static LPSTR __cdecl StringUppercase( _Inout_cap_(size) LPSTR psz, _In_ size_t size ) throw()
	{
		ATL_HELPER::mbsupr_s(reinterpret_cast< unsigned char* >( psz ), size);
		return psz;
	}

	static LPSTR __cdecl StringLowercase( _Inout_cap_(size) LPSTR psz, _In_ size_t size ) throw()
	{
		ATL_HELPER::mbslwr_s( reinterpret_cast< unsigned char* >( psz ), size );
		return psz;
	}

	static LPSTR __cdecl StringReverse( _Inout_ LPSTR psz ) throw()
	{
		return reinterpret_cast< LPSTR >( _mbsrev( reinterpret_cast< unsigned char* >( psz ) ) );
	}

	static int __cdecl GetFormattedLength( _In_ _Printf_format_string_ LPCSTR pszFormat, va_list args ) throw()
	{
		return _vscprintf( pszFormat, args );
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::Format")
	static int __cdecl Format( _Out_ LPSTR pszBuffer, _In_ _Printf_format_string_ LPCSTR pszFormat, va_list args ) throw()
	{
		#pragma warning (push)
		#pragma warning(disable : 4996)
		return vsprintf( pszBuffer, pszFormat, args );
		#pragma warning (pop)

	}
	static int __cdecl Format
		( _Out_cap_post_count_(nlength, return) LPSTR pszBuffer, _In_ size_t nlength, _In_ _Printf_format_string_ LPCSTR pszFormat, va_list args ) throw()
	{
		return vsprintf_s( pszBuffer, nlength, pszFormat, args );
	}

	static int __cdecl GetBaseTypeLength( _In_z_ LPCSTR pszSrc ) throw()
	{
		// Returns required buffer length in XCHARs
		return int( strlen( pszSrc ) );
	}

	static int __cdecl GetBaseTypeLength( _In_count_(nLength) LPCSTR pszSrc, int nLength ) throw()
	{
		(void)pszSrc;
		// Returns required buffer length in XCHARs
		return nLength;
	}

	static int __cdecl GetBaseTypeLength( _In_z_ LPCWSTR pszSource ) throw()
	{
		// Returns required buffer length in XCHARs
		return ::WideCharToMultiByte( CP_THREAD_ACP, 0, pszSource, -1, NULL, 0, NULL, NULL )-1;
	}

	static int __cdecl GetBaseTypeLength( _In_count_(nLength) LPCWSTR pszSource, int nLength ) throw()
	{
		// Returns required buffer length in XCHARs
		return ::WideCharToMultiByte( CP_THREAD_ACP, 0, pszSource, nLength, NULL, 0, NULL, NULL );
	}

	static void __cdecl ConvertToBaseType( _Out_cap_(nDestLength) LPSTR pszDest, _In_ int nDestLength,
		_In_z_ LPCSTR pszSrc, int nSrcLength = -1 ) throw()
	{
		if (nSrcLength == -1) { nSrcLength=1 + GetBaseTypeLength(pszSrc); }
		// nLen is in XCHARs
		ATL_HELPER::memcpy_s( pszDest, nDestLength*sizeof( char ), 
			pszSrc, nSrcLength*sizeof( char ) );
	}

	static void __cdecl ConvertToBaseType( _Out_cap_(nDestLength) LPSTR pszDest, _In_ int nDestLength,
		_In_z_ LPCWSTR pszSrc, _In_ int nSrcLength = -1) throw()
	{
		// nLen is in XCHARs
		::WideCharToMultiByte( CP_THREAD_ACP, 0, pszSrc, nSrcLength, pszDest, nDestLength, NULL, NULL );
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::ConvertToOem")
	static void ConvertToOem(_Inout_ _CharType* pstrString) throw()
	{
		BOOL fSuccess=::CharToOemA(pstrString, pstrString);
		// old version can't report error
		ATLASSERT(fSuccess);
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::ConvertToAnsi")
	static void ConvertToAnsi(_Inout_ _CharType* pstrString) throw()
	{
		BOOL fSuccess=::OemToCharA(pstrString, pstrString);
		// old version can't report error
		ATLASSERT(fSuccess);
	}

	static void ConvertToOem(_Inout_cap_(size) _CharType* pstrString, _In_ size_t size)
	{
		if(size>UINT_MAX)
		{
			// API only allows DWORD size
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);
		}
		DWORD dwSize=static_cast<DWORD>(size);
		BOOL fSuccess=::CharToOemBuffA(pstrString, pstrString, dwSize);

		ATLASSERT(fSuccess);//Modified by TaoRenyong 20110923
		//if(!fSuccess)
		//{
		//	AtlThrowLastWin32();
		//}
	}

	static void ConvertToAnsi(_Inout_cap_(size) _CharType* pstrString, _In_ size_t size)
	{
		if(size>UINT_MAX)
		{
			// API only allows DWORD size
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);
		}
		DWORD dwSize=static_cast<DWORD>(size);
		BOOL fSuccess=::OemToCharBuffA(pstrString, pstrString, dwSize);
		ATLASSERT(fSuccess);//Modified by TaoRenyong 20110923
		//if(!fSuccess)
		//{
		//	AtlThrowLastWin32();
		//}
	}

	static void __cdecl FloodCharacters( _In_ char ch, _In_ int nLength, _Out_capcount_(nLength) char* pch ) throw()
	{
		// nLength is in XCHARs
		memset( pch, ch, nLength );
	}

	static BSTR __cdecl AllocSysString( _In_count_(nDataLength) const char* pchData, int nDataLength ) throw()
	{
		int nLen = ::MultiByteToWideChar( CP_THREAD_ACP, 0, pchData, nDataLength,
			NULL, NULL );
		BSTR bstr = ::SysAllocStringLen( NULL, nLen );
		if( bstr != NULL )
		{
			::MultiByteToWideChar( CP_THREAD_ACP, 0, pchData, nDataLength,
				bstr, nLen );
		}

		return bstr;
	}

	static BOOL __cdecl ReAllocSysString( _In_count_(nDataLength) const char* pchData, _Out_ BSTR* pbstr, _In_ int nDataLength ) throw()
	{
		int nLen = ::MultiByteToWideChar( CP_THREAD_ACP, 0, pchData, nDataLength, NULL, NULL );
		BOOL bSuccess = ::SysReAllocStringLen( pbstr, NULL, nLen );
		if( bSuccess )
		{
			::MultiByteToWideChar( CP_THREAD_ACP, 0, pchData, nDataLength, *pbstr, nLen );
		}

		return bSuccess;
	}

	static DWORD __cdecl _AFX_FUNCNAME(FormatMessage)( _In_ DWORD dwFlags, LPCVOID pSource,
		_In_ DWORD dwMessageID, _In_ DWORD dwLanguageID, _Out_cap_(nSize) LPSTR pszBuffer,
		DWORD nSize, va_list* pArguments ) throw()
	{
		return ::FormatMessageA( dwFlags, pSource, dwMessageID, dwLanguageID,
				pszBuffer, nSize, pArguments );
	}

#if defined(_AFX)
	static DWORD __cdecl FormatMessage( _In_ DWORD dwFlags, LPCVOID pSource,
		_In_ DWORD dwMessageID, _In_ DWORD dwLanguageID, _Out_cap_(nSize) LPSTR pszBuffer,
		DWORD nSize, va_list* pArguments ) throw()
	{
		return _AFX_FUNCNAME(FormatMessage)(dwFlags, pSource, dwMessageID, dwLanguageID, pszBuffer, nSize, pArguments);
	}
#endif

	static int __cdecl SafeStringLen( _In_opt_z_ LPCSTR psz ) throw()
	{
		// returns length in bytes
		return (psz != NULL) ? int( strlen( psz ) ) : 0;
	}

	static int __cdecl SafeStringLen( _In_opt_z_ LPCWSTR psz ) throw()
	{
		// returns length in wchar_ts
		return (psz != NULL) ? int( wcslen( psz ) ) : 0;
	}

	static int __cdecl GetCharLen( _In_z_ const wchar_t* pch ) throw()
	{
		(void)pch;
		// returns char length
		return 1;
	}

	static int __cdecl GetCharLen( _In_z_ const char* pch ) throw()
	{
		// returns char length
		return int( _mbclen( reinterpret_cast< const unsigned char* >( pch ) ) );
	}

	static DWORD __cdecl GetEnvironmentVariable( _In_z_ LPCSTR pszVar,
		_Out_opt_cap_(dwSize) LPSTR pszBuffer, _In_ DWORD dwSize ) throw()
	{
		return ::GetEnvironmentVariableA( pszVar, pszBuffer, dwSize );
	}
};

// specialization for wchar_t
template<>
class ChTraitsCRT< wchar_t > :
	public ChTraitsBase< wchar_t >
{
	static DWORD __cdecl _GetEnvironmentVariableW( _In_z_ LPCWSTR pszName, _Out_opt_cap_post_count_(nSize, return) LPWSTR pszBuffer, _In_ DWORD nSize ) throw()
	{
		return ::GetEnvironmentVariableW( pszName, pszBuffer, nSize );
	}

public:
	static LPWSTR __cdecl CharNext( _In_z_ LPCWSTR psz ) throw()
	{
		return const_cast< LPWSTR >( psz+1 );
	}

	static int __cdecl IsDigit( _In_ wchar_t ch ) throw()
	{
		return iswdigit( static_cast<unsigned short>(ch) );
	}

	static int __cdecl IsSpace( _In_ wchar_t ch ) throw()
	{
		return iswspace( static_cast<unsigned short>(ch) );
	}

	static int __cdecl StringCompare( _In_z_ LPCWSTR pszA, _In_z_ LPCWSTR pszB ) throw()
	{
		return wcscmp( pszA, pszB );
	}

	static int __cdecl StringCompareIgnore( _In_z_ LPCWSTR pszA, _In_z_ LPCWSTR pszB ) throw()
	{
		return _wcsicmp( pszA, pszB );
	}

	static int __cdecl StringCollate( _In_z_ LPCWSTR pszA, _In_z_ LPCWSTR pszB ) throw()
	{
		return wcscoll( pszA, pszB );
	}

	static int __cdecl StringCollateIgnore( _In_z_ LPCWSTR pszA, _In_z_ LPCWSTR pszB ) throw()
	{
		return _wcsicoll( pszA, pszB );
	}

	static LPCWSTR __cdecl StringFindString( _In_z_ LPCWSTR pszBlock, _In_z_ LPCWSTR pszMatch ) throw()
	{
		return wcsstr( pszBlock, pszMatch );
	}

	static LPWSTR __cdecl StringFindString( _In_z_ LPWSTR pszBlock, _In_z_ LPCWSTR pszMatch ) throw()
	{
		return( const_cast< LPWSTR >( StringFindString( const_cast< LPCWSTR >( pszBlock ), pszMatch ) ) );
	}

	static LPCWSTR __cdecl StringFindChar( _In_z_ LPCWSTR pszBlock, _In_ wchar_t chMatch ) throw()
	{
		return wcschr( pszBlock, chMatch );
	}

	static LPCWSTR __cdecl StringFindCharRev( _In_z_ LPCWSTR psz, _In_ wchar_t ch ) throw()
	{
		return wcsrchr( psz, ch );
	}

	static LPCWSTR __cdecl StringScanSet( _In_z_ LPCWSTR pszBlock, _In_z_ LPCWSTR pszMatch ) throw()
	{
		return wcspbrk( pszBlock, pszMatch );
	}

	static int __cdecl StringSpanIncluding( _In_z_ LPCWSTR pszBlock, _In_z_ LPCWSTR pszSet ) throw()
	{
		return (int)wcsspn( pszBlock, pszSet );
	}

	static int __cdecl StringSpanExcluding( _In_z_ LPCWSTR pszBlock, _In_z_ LPCWSTR pszSet ) throw()
	{
		return (int)wcscspn( pszBlock, pszSet );
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringUppercase")
	static LPWSTR __cdecl StringUppercase( _Inout_ LPWSTR psz ) throw()
	{
#pragma warning (push)
#pragma warning(disable : 4996)
		return _wcsupr( psz );
#pragma warning (pop)
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringLowercase")
	static LPWSTR __cdecl StringLowercase( _Inout_ LPWSTR psz ) throw()
	{
#pragma warning (push)
#pragma warning(disable : 4996)
		return _wcslwr( psz );
#pragma warning (pop)
	}

	static LPWSTR __cdecl StringUppercase( _Inout_cap_(size) LPWSTR psz, _In_ size_t size ) throw()
	{
		errno_t err = _wcsupr_s( psz, size );
		return (err == 0) ? psz : NULL;
	}

	static LPWSTR __cdecl StringLowercase( _Inout_cap_(size) LPWSTR psz, _In_ size_t size ) throw()
	{
		errno_t err = _wcslwr_s( psz, size );
		return (err == 0) ? psz : NULL;
	}

	static LPWSTR __cdecl StringReverse( _Inout_ LPWSTR psz ) throw()
	{
		return _wcsrev( psz );
	}

	static int __cdecl GetFormattedLength( _In_ _Printf_format_string_ LPCWSTR pszFormat, va_list args) throw()
	{
		return _vscwprintf( pszFormat, args );
	}

	//_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::Format")
	static int __cdecl Format( _Out_ LPWSTR pszBuffer, _In_ _Printf_format_string_ LPCWSTR pszFormat, va_list args) throw()
	{
		#pragma warning (push)
		#pragma warning(disable : 4996)
		return vswprintf( pszBuffer, pszFormat, args );
		#pragma warning (pop)
	}
	static int __cdecl Format
		( _Out_cap_(nLength) LPWSTR pszBuffer, _In_ size_t nLength, _In_ __format_string LPCWSTR pszFormat, va_list args) throw()
	{
		return vswprintf_s( pszBuffer, nLength, pszFormat, args );
	}

	static int __cdecl GetBaseTypeLength( _In_z_ LPCSTR pszSrc ) throw()
	{
		// Returns required buffer size in wchar_ts
		return ::MultiByteToWideChar(CP_THREAD_ACP, 0, pszSrc, -1, NULL, 0 )-1;
	}

	static int __cdecl GetBaseTypeLength( _In_count_(nLength) LPCSTR pszSrc, _In_ int nLength ) throw()
	{
		// Returns required buffer size in wchar_ts
		return ::MultiByteToWideChar(CP_THREAD_ACP, 0, pszSrc, nLength, NULL, 0 );
	}

	static int __cdecl GetBaseTypeLength( _In_z_ LPCWSTR pszSrc ) throw()
	{
		// Returns required buffer size in wchar_ts
		return (int)wcslen( pszSrc );
	}

	static int __cdecl GetBaseTypeLength( _In_count_(nLength) LPCWSTR pszSrc, _In_ int nLength ) throw()
	{
		(void)pszSrc;
		// Returns required buffer size in wchar_ts
		return nLength;
	}

	static void __cdecl ConvertToBaseType( _Out_cap_(nDestLength) LPWSTR pszDest, _In_ int nDestLength,
		_In_z_ LPCSTR pszSrc, _In_ int nSrcLength = -1) throw()
	{
		// nLen is in wchar_ts
		::MultiByteToWideChar(CP_THREAD_ACP, 0, pszSrc, nSrcLength, pszDest, nDestLength );
	}

	static void __cdecl ConvertToBaseType( _Out_cap_(nDestLength) LPWSTR pszDest, _In_ int nDestLength,
		_In_z_ LPCWSTR pszSrc, int nSrcLength = -1 ) throw()
	{		
		if (nSrcLength == -1) { nSrcLength=1 + GetBaseTypeLength(pszSrc); }
		// nLen is in wchar_ts
		ATL_HELPER::wmemcpy_s(pszDest, nDestLength, pszSrc, nSrcLength);
	}

	static void __cdecl FloodCharacters( _In_ wchar_t ch, _In_ int nLength, _Out_capcount_(nLength) LPWSTR psz ) throw()
	{
		// nLength is in XCHARs
		for( int i = 0; i < nLength; i++ )
		{
			psz[i] = ch;
		}
	}

	static BSTR __cdecl AllocSysString( _In_count_(nDataLength) const wchar_t* pchData, _In_ int nDataLength ) throw()
	{
		return ::SysAllocStringLen( pchData, nDataLength );
	}

	static BOOL __cdecl ReAllocSysString( _In_count_(nDataLength) const wchar_t* pchData, _Inout_ BSTR* pbstr, _In_ int nDataLength ) throw()
	{
		return ::SysReAllocStringLen( pbstr, pchData, nDataLength );
	}

	static int __cdecl SafeStringLen( _In_opt_z_ LPCSTR psz ) throw()
	{
		// returns length in bytes
		return (psz != NULL) ? (int)strlen( psz ) : 0;
	}

	static int __cdecl SafeStringLen( _In_opt_ LPCWSTR psz ) throw()
	{
		// returns length in wchar_ts
		return (psz != NULL) ? (int)wcslen( psz ) : 0;
	}

	static int __cdecl GetCharLen( _In_z_ const wchar_t* pch ) throw()
	{
		(void)pch;
		// returns char length
		return 1;
	}

	static int __cdecl GetCharLen( _In_z_ const char* pch ) throw()
	{
		// returns char length
		return (int)( _mbclen( reinterpret_cast< const unsigned char* >( pch ) ) );
	}

	static DWORD __cdecl GetEnvironmentVariable( _In_z_ LPCWSTR pszVar, _Out_opt_cap_(dwSize) LPWSTR pszBuffer, _In_ DWORD dwSize ) throw()
	{
		return _GetEnvironmentVariableW( pszVar, pszBuffer, dwSize );
	}

	static void __cdecl ConvertToOem( __reserved LPWSTR /*psz*/ )
	{
		ATLENSURE(FALSE); // Unsupported Feature 
	}

	static void __cdecl ConvertToAnsi( __reserved LPWSTR /*psz*/ )
	{
		ATLENSURE(FALSE); // Unsupported Feature 
	}

	static void __cdecl ConvertToOem( __reserved LPWSTR /*psz*/, size_t )
	{
		ATLENSURE(FALSE); // Unsupported Feature 
	}

	static void __cdecl ConvertToAnsi( __reserved LPWSTR /*psz*/, size_t ) 
	{
		ATLENSURE(FALSE); // Unsupported Feature 
	}

#ifdef _UNICODE
public:
	static DWORD __cdecl _AFX_FUNCNAME(FormatMessage)( _In_ DWORD dwFlags, LPCVOID pSource,
		_In_ DWORD dwMessageID, _In_ DWORD dwLanguageID, _Out_cap_(nSize) LPWSTR pszBuffer,
		_In_ DWORD nSize, va_list* pArguments ) throw()
	{
		return ::FormatMessageW( dwFlags, pSource, dwMessageID, dwLanguageID,
				pszBuffer, nSize, pArguments );
	}

#if defined(_AFX)
	static DWORD __cdecl FormatMessage( _In_ DWORD dwFlags, LPCVOID pSource,
		_In_ DWORD dwMessageID, _In_ DWORD dwLanguageID, _Out_cap_(nSize) LPWSTR pszBuffer,
		_In_ DWORD nSize, va_list* pArguments ) throw()
	{
		return _AFX_FUNCNAME(FormatMessage)(dwFlags, pSource, dwMessageID, dwLanguageID, pszBuffer, nSize, pArguments);
	}
#endif

#else
	static DWORD __cdecl _AFX_FUNCNAME(FormatMessage)( DWORD /*dwFlags*/, LPCVOID /*pSource*/,
		DWORD /*dwMessageID*/, DWORD /*dwLanguageID*/, __reserved LPWSTR /*pszBuffer*/,
		DWORD /*nSize*/, va_list* /*pArguments*/ )
	{
		ATLENSURE(FALSE); // Unsupported Feature 
		return 0;
	}

#if defined(_AFX)
	static DWORD __cdecl FormatMessage( DWORD dwFlags, LPCVOID pSource,
		DWORD dwMessageID, DWORD dwLanguageID, __reserved LPWSTR pszBuffer,
		DWORD nSize, va_list* pArguments )
	{
		return _AFX_FUNCNAME(FormatMessage)(dwFlags, pSource, dwMessageID, dwLanguageID, pszBuffer, nSize, pArguments);
	}
#endif

#endif

};

};  // namespace ATL_STR


namespace ATL_STR
{
// The CStringT class has a few varargs member functions that will cause 4793
// warnings if compiled /clr. Because of the way template classes are parsed,
// we need to disable the warning for the entire class.
#pragma warning( push )
#pragma warning( disable : 4793 )

//, class StringTraits
template< typename BaseType >	
class CStringT : public CSimpleStringT< BaseType >
{	

public:
	typedef CSimpleStringT< BaseType > CThisSimpleString;
	typedef typename CThisSimpleString::XCHAR XCHAR;
	typedef typename CThisSimpleString::PXSTR PXSTR;
	typedef typename CThisSimpleString::PCXSTR PCXSTR;
	typedef typename CThisSimpleString::YCHAR YCHAR;
	typedef typename CThisSimpleString::PYSTR PYSTR;
	typedef typename CThisSimpleString::PCYSTR PCYSTR;

	typedef typename ChTraitsCRT< BaseType > StringTraits;

public:
	CStringT()
	{
	}

	static void __cdecl Construct( CStringT* pString )
	{
		new( pString ) CStringT;
	}

	// Copy constructor
	CStringT( const CStringT& strSrc ) :
		CThisSimpleString( strSrc )
	{
	}

	// Construct from CSimpleStringT
	operator CSimpleStringT<BaseType>&()
	{
		return *(CSimpleStringT<BaseType> *)this;
	}

	CStringT( _In_ const CSimpleStringT<BaseType>& strSrc ) :
	CThisSimpleString( strSrc )
	{
	}

	CStringT( _In_opt_z_ const XCHAR* pszSrc ) 
	{
		//if( !CheckImplicitLoad( pszSrc ) )//ÆÁ±ÎLoadString 20111017 TaoRenyong
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}
	}

	CStringT( _In_opt_z_ const YCHAR* pszSrc ) 
	{
		//if( !CheckImplicitLoad( pszSrc ) )//ÆÁ±ÎLoadString 20111017 TaoRenyong
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}
	}

	CStringT( _In_opt_z_ const unsigned char* pszSrc )
	{
		*this = reinterpret_cast< const char* >( pszSrc );
	}

	CStringT( _In_ char ch, _In_ int nLength = 1 ) 
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{
			PXSTR pszBuffer = GetBuffer( nLength );
			StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
			releaseBufferSetLength( nLength );
		}
	}

	CStringT( _In_ wchar_t ch, _In_ int nLength = 1 )
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{			
			//Convert ch to the BaseType
			wchar_t pszCh[2] = { ch , 0 };
			int nBaseTypeCharLen = 1;

			if(ch != L'\0')
			{
				nBaseTypeCharLen = StringTraits::GetBaseTypeLength(pszCh);
			}

			CTempBuffer<XCHAR,10> buffBaseTypeChar;			
			buffBaseTypeChar.Allocate(nBaseTypeCharLen+1);
			StringTraits::ConvertToBaseType( buffBaseTypeChar, nBaseTypeCharLen+1, pszCh, 1 );
			//Allocate enough characters in String and flood (replicate) with the (converted character)*nLength
			PXSTR pszBuffer = GetBuffer( nLength*nBaseTypeCharLen );
			if (nBaseTypeCharLen == 1)
			{   //Optimization for a common case - wide char translates to 1 ansi/wide char.
				StringTraits::FloodCharacters( buffBaseTypeChar[0], nLength, pszBuffer );				
			} else
			{
				XCHAR* p=pszBuffer;
				for (int i=0 ; i < nLength ;++i)
				{
					for (int j=0 ; j < nBaseTypeCharLen ;++j)
					{	
						*p=buffBaseTypeChar[j];
						++p;
					}
				}
			}
			releaseBufferSetLength( nLength*nBaseTypeCharLen );			
		}
	}

	CStringT( _In_count_(nLength) const XCHAR* pch, _In_ int nLength ) :
		CThisSimpleString( pch, nLength )
	{
	}

	CStringT( _In_count_(nLength) const YCHAR* pch, _In_ int nLength ) 
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{
			ATLASSERT(pch != NULL); //Modified by TaoRenyong 20110923 AtlIsValidAddress

			int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
			PXSTR pszBuffer = GetBuffer( nDestLength );
			StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
			releaseBufferSetLength( nDestLength );
		}
	}

	// Destructor
	~CStringT() throw()
	{
	}

	// Assignment operators
	CStringT& operator=( _In_ const CStringT& strSrc )
	{
		CThisSimpleString::operator=( strSrc );

		return( *this );
	}
	
	CStringT& operator=( _In_ const CSimpleStringT<BaseType>& strSrc )
	{
		CThisSimpleString::operator=( strSrc );

		return( *this );
	}

	CStringT& operator=( _In_opt_z_ PCXSTR pszSrc )
	{
		CThisSimpleString::operator=( pszSrc );

		return( *this );
	}

	CStringT& operator=( _In_opt_z_ PCYSTR pszSrc )
	{
		// nDestLength is in XCHARs
		int nDestLength = (pszSrc != NULL) ? StringTraits::GetBaseTypeLength( pszSrc ) : 0;
		if( nDestLength > 0 )
		{
			PXSTR pszBuffer = GetBuffer( nDestLength );
			StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pszSrc);
			releaseBufferSetLength( nDestLength );
		}
		else
		{
			Empty();
		}

		return( *this );
	}

	CStringT& operator=( _In_opt_z_ const unsigned char* pszSrc )
	{
		return( operator=( reinterpret_cast< const char* >( pszSrc ) ) );
	}

	CStringT& operator=( _In_ char ch )
	{
		char ach[2] = { ch, 0 };

		return( operator=( ach ) );
	}

	CStringT& operator=( _In_ wchar_t ch )
	{
		wchar_t ach[2] = { ch, 0 };

		return( operator=( ach ) );
	}

//	CStringT& operator=( _In_ const VARIANT& var );

	CStringT& operator+=( _In_ const CThisSimpleString& str )
	{
		CThisSimpleString::operator+=( str );

		return( *this );
	}

	CStringT& operator+=( _In_opt_z_ PCXSTR pszSrc )
	{
		CThisSimpleString::operator+=( pszSrc );

		return( *this );
	}

	CStringT& operator+=( _In_opt_z_ PCYSTR pszSrc )
	{
		CStringT str( pszSrc );

		return( operator+=( str ) );
	}

	CStringT& operator+=( _In_ char ch )
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

	CStringT& operator+=( _In_ unsigned char ch )
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

	CStringT& operator+=( _In_ wchar_t ch )
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

//	CStringT& operator+=( _In_ const VARIANT& var );

	// Comparison
	int Compare( _In_opt_z_ PCXSTR psz ) const
	{
		//ATLENSURE( AtlIsValidString( psz ) );
		//__analysis_assume(psz); // AtlIsValidString guarantees that psz != NULL
		ATLASSERT(NULL != psz); //Modified by TaoRenyong 20110923
		return( StringTraits::StringCompare( GetString(), psz ) );
	}

	int CompareNoCase( _In_opt_z_ PCXSTR psz ) const
	{
		//ATLENSURE( AtlIsValidString( psz ) );
		//__analysis_assume(psz); // AtlIsValidString guarantees that psz != NULL
		ATLASSERT(NULL != psz); //Modified by TaoRenyong 20110923
		return( StringTraits::StringCompareIgnore( GetString(), psz ) );
	}

	int Collate( _In_opt_z_ PCXSTR psz ) const throw()
	{
		//ATLASSERT( AtlIsValidString( psz ) );
		ATLASSERT(NULL != psz); //Modified by TaoRenyong 20110923
		return( StringTraits::StringCollate( GetString(), psz ) );
	}

	int CollateNoCase( _In_opt_z_ PCXSTR psz ) const throw()
	{
		//ATLASSERT( AtlIsValidString( psz ) );
		ATLASSERT(NULL != psz); //Modified by TaoRenyong 20110923
		return( StringTraits::StringCollateIgnore( GetString(), psz ) );
	}

	// Advanced manipulation

	// Delete 'nCount' characters, starting at index 'iIndex'
	int Delete( _In_ int iIndex, _In_ int nCount = 1 )
	{
		if( iIndex < 0 )
			iIndex = 0;
		
		if( nCount < 0 )
			nCount = 0;

		int nLength = GetLength();
//		if( (::ATL::AtlAddThrow(nCount, iIndex)) > nLength )
		if( (nCount + iIndex) > nLength ) //Modified by TaoRenyong 20110923
		{
			nCount = nLength-iIndex;
		}
		if( nCount > 0 )
		{
			int nNewLength = nLength-nCount;
			int nXCHARsToCopy = nLength-(iIndex+nCount)+1;
			PXSTR pszBuffer = GetBuffer();
			ATL_HELPER::memmove_s( pszBuffer+iIndex, nXCHARsToCopy*sizeof( XCHAR ), 
				pszBuffer+iIndex+nCount, nXCHARsToCopy*sizeof( XCHAR ) );
			releaseBufferSetLength( nNewLength );
		}

		return( GetLength() );
	}

	// Insert character 'ch' before index 'iIndex'
	int Insert( _In_ int iIndex, _In_ XCHAR ch )
	{
		if( iIndex < 0 )
			iIndex = 0;
			
		if( iIndex > GetLength() )
		{
			iIndex = GetLength();
		}
		int nNewLength = GetLength()+1;

		PXSTR pszBuffer = GetBuffer( nNewLength );

		// move existing bytes down 
		ATL_HELPER::memmove_s( pszBuffer+iIndex+1, (nNewLength-iIndex)*sizeof( XCHAR ), 
			pszBuffer+iIndex, (nNewLength-iIndex)*sizeof( XCHAR ) );
		pszBuffer[iIndex] = ch;

		releaseBufferSetLength( nNewLength );

		return( nNewLength );
	}

	// Insert string 'psz' before index 'iIndex'
	int Insert( _In_ int iIndex, _In_ PCXSTR psz )
	{
		if( iIndex < 0 )
			iIndex = 0;

		if( iIndex > GetLength() )
		{
			iIndex = GetLength();
		}

		// nInsertLength and nNewLength are in XCHARs
		int nInsertLength = StringTraits::SafeStringLen( psz );
		int nNewLength = GetLength();
		if( nInsertLength > 0 )
		{
			nNewLength += nInsertLength;

			PXSTR pszBuffer = GetBuffer( nNewLength );
			// move existing bytes down 
			ATL_HELPER::memmove_s( pszBuffer+iIndex+nInsertLength, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ), 
				pszBuffer+iIndex, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ) );
			ATL_HELPER::memcpy_s( pszBuffer+iIndex, nInsertLength*sizeof( XCHAR ), 
				psz, nInsertLength*sizeof( XCHAR ) );
			releaseBufferSetLength( nNewLength );
		}

		return( nNewLength );
	}

	// Replace all occurrences of character 'chOld' with character 'chNew'
	int Replace( _In_ XCHAR chOld, _In_ XCHAR chNew )
	{
		int nCount = 0;

		// short-circuit the nop case
		if( chOld != chNew )
		{
			// otherwise modify each character that matches in the string
			bool bCopied = false;
			PXSTR pszBuffer = const_cast< PXSTR >( GetString() );  // We don't actually write to pszBuffer until we've called GetBuffer().

			int nLength = GetLength();
			int iChar = 0;
			while( iChar < nLength )
			{
				// replace instances of the specified character only
				if( pszBuffer[iChar] == chOld )
				{
					if( !bCopied )
					{
						bCopied = true;
						pszBuffer = GetBuffer( nLength );
					}
					pszBuffer[iChar] = chNew;
					nCount++;
				}
				iChar = int( StringTraits::CharNext( pszBuffer+iChar )-pszBuffer );
			}
			if( bCopied )
			{
				releaseBufferSetLength( nLength );
			}
		}

		return( nCount );
	}

	// Replace all occurrences of string 'pszOld' with string 'pszNew'
	int Replace( _In_ PCXSTR pszOld, _In_ PCXSTR pszNew )
	{
		// can't have empty or NULL lpszOld

		// nSourceLen is in XCHARs
		int nSourceLen = StringTraits::SafeStringLen( pszOld );
		if( nSourceLen == 0 )
			return( 0 );
		// nReplacementLen is in XCHARs
		int nReplacementLen = StringTraits::SafeStringLen( pszNew );

		// loop once to figure out the size of the result string
		int nCount = 0;
		{
			PCXSTR pszStart = GetString();
			PCXSTR pszEnd = pszStart+GetLength();
			while( pszStart < pszEnd )
			{
				PCXSTR pszTarget;
				while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL)
				{
					nCount++;
					pszStart = pszTarget+nSourceLen;
				}
				pszStart += StringTraits::SafeStringLen( pszStart )+1;
			}
		}

		// if any changes were made, make them
		if( nCount > 0 )
		{
			// if the buffer is too small, just
			//   allocate a new buffer (slow but sure)
			int nOldLength = GetLength();
			int nNewLength = nOldLength+(nReplacementLen-nSourceLen)*nCount;

			PXSTR pszBuffer = GetBuffer( __max( nNewLength, nOldLength ) );

			PXSTR pszStart = pszBuffer;
			PXSTR pszEnd = pszStart+nOldLength;

			// loop again to actually do the work
			while( pszStart < pszEnd )
			{
				PXSTR pszTarget;
				while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL )
				{
					int nBalance = nOldLength-int(pszTarget-pszBuffer+nSourceLen);
					ATL_HELPER::memmove_s( pszTarget+nReplacementLen, nBalance*sizeof( XCHAR ), 
						pszTarget+nSourceLen, nBalance*sizeof( XCHAR ) );
					ATL_HELPER::memcpy_s( pszTarget, nReplacementLen*sizeof( XCHAR ), 
						pszNew, nReplacementLen*sizeof( XCHAR ) );
					pszStart = pszTarget+nReplacementLen;
					pszTarget[nReplacementLen+nBalance] = 0;
					nOldLength += (nReplacementLen-nSourceLen);
				}
				pszStart += StringTraits::SafeStringLen( pszStart )+1;
			}
			ATLASSERT( pszBuffer[nNewLength] == 0 );
			releaseBufferSetLength( nNewLength );
		}

		return( nCount );
	}

	// Remove all occurrences of character 'chRemove'
	int Remove( _In_ XCHAR chRemove )
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );

		PXSTR pszSource = pszBuffer;
		PXSTR pszDest = pszBuffer;
		PXSTR pszEnd = pszBuffer+nLength;

		while( pszSource < pszEnd )
		{
			PXSTR pszNewSource = StringTraits::CharNext( pszSource );
			if( *pszSource != chRemove )
			{
				// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
	   			// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
				size_t NewSourceGap = (pszNewSource-pszSource);
				PXSTR pszNewDest = pszDest + NewSourceGap;
				size_t i = 0;
				for (i = 0;  pszDest != pszNewDest && i < NewSourceGap; i++)
				{
					*pszDest = *pszSource;
					pszSource++;
					pszDest++;
				}
			}
			pszSource = pszNewSource;
		}
		*pszDest = 0;
		int nCount = int( pszSource-pszDest );
		releaseBufferSetLength( nLength-nCount );

		return( nCount );
	}

	CStringT Tokenize( _In_ PCXSTR pszTokens, _Inout_ int& iStart ) const
	{
		ATLASSERT( iStart >= 0 );
			
		if(iStart < 0)
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);			
			
		if( (pszTokens == NULL) || (*pszTokens == (XCHAR)0) )
		{
			if (iStart < GetLength())
			{
//				return( CStringT( GetString()+iStart, GetManager() ) );
				return( CStringT( GetString()+iStart ) ); //Modified by TaoRenyong 20110923
			}
		}
		else
		{
			PCXSTR pszPlace = GetString()+iStart;
			PCXSTR pszEnd = GetString()+GetLength();
			if( pszPlace < pszEnd )
			{
				int nIncluding = StringTraits::StringSpanIncluding( pszPlace,
					pszTokens );

				if( (pszPlace+nIncluding) < pszEnd )
				{
					pszPlace += nIncluding;
					int nExcluding = StringTraits::StringSpanExcluding( pszPlace, pszTokens );

					int iFrom = iStart+nIncluding;
					int nUntil = nExcluding;
					iStart = iFrom+nUntil+1;

					return( Mid( iFrom, nUntil ) );
				}
			}
		}

		// return empty string, done tokenizing
		iStart = -1;

//		return( CStringT( GetManager() ) );
		return( CStringT( ) ); //Modified by TaoRenyong 20110923
	}

	// find routines

	// Find the first occurrence of character 'ch', starting at index 'iStart'
	int Find( _In_ XCHAR ch, _In_ int iStart = 0 ) const throw()
	{
		// iStart is in XCHARs
		ATLASSERT( iStart >= 0 );

		// nLength is in XCHARs
		int nLength = GetLength();
		if( iStart < 0 || iStart >= nLength)
		{
			return( -1 );
		}

		// find first single character
		PCXSTR psz = StringTraits::StringFindChar( GetString()+iStart, ch );

		// return -1 if not found and index otherwise
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// look for a specific sub-string

	// Find the first occurrence of string 'pszSub', starting at index 'iStart'
	int Find( _In_ PCXSTR pszSub, _In_ int iStart = 0 ) const throw()
	{
		// iStart is in XCHARs
		ATLASSERT( iStart >= 0 );
		//ATLASSERT( AtlIsValidString( pszSub ) );
		ATLASSERT(NULL != pszSub); //Modified by TaoRenyong 20110923

		if(pszSub == NULL)
		{
			return( -1 );
		}
		// nLength is in XCHARs
		int nLength = GetLength();
		if( iStart < 0 || iStart > nLength )
		{
			return( -1 );
		}

		// find first matching substring
		PCXSTR psz = StringTraits::StringFindString( GetString()+iStart, pszSub );

		// return -1 for not found, distance from beginning otherwise
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// Find the first occurrence of any of the characters in string 'pszCharSet'
	int FindOneOf( _In_ PCXSTR pszCharSet ) const throw()
	{
		//ATLASSERT( AtlIsValidString( pszCharSet ) );
		ATLASSERT(NULL != pszCharSet); //Modified by TaoRenyong 20110923
		PCXSTR psz = StringTraits::StringScanSet( GetString(), pszCharSet );
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// Find the last occurrence of character 'ch'
	int ReverseFind( _In_ XCHAR ch ) const throw()
	{
		// find last single character
		PCXSTR psz = StringTraits::StringFindCharRev( GetString(), ch );

		// return -1 if not found, distance from beginning otherwise
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// manipulation

	// Convert the string to uppercase
	CStringT& MakeUpper()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::StringUppercase( pszBuffer, nLength+1 );
		releaseBufferSetLength( nLength );

		return( *this );
	}

	// Convert the string to lowercase
	CStringT& MakeLower()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::StringLowercase( pszBuffer, nLength+1 );
		releaseBufferSetLength( nLength );

		return( *this );
	}

	// Reverse the string
	CStringT& MakeReverse()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::StringReverse( pszBuffer );
		releaseBufferSetLength( nLength );

		return( *this );
	}

	// trimming

	// Remove all trailing whitespace
	CStringT& TrimRight()
	{
		// find beginning of trailing spaces by starting
		// at beginning (DBCS aware)

		PCXSTR psz = GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( StringTraits::IsSpace( *psz ) )
			{
				if( pszLast == NULL )
					pszLast = psz;
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at trailing space start
			int iLast = int( pszLast-GetString() );

			Truncate( iLast );
		}

		return( *this );
	}

	// Remove all leading whitespace
	CStringT& TrimLeft()
	{
		// find first non-space character

		PCXSTR psz = GetString();

		while( StringTraits::IsSpace( *psz ) )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != GetString() )
		{
			// fix up data and length
			int iFirst = int( psz-GetString() );
			PXSTR pszBuffer = GetBuffer( GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = GetLength()-iFirst;
			ATL_HELPER::memmove_s( pszBuffer, (GetLength()+1)*sizeof( XCHAR ), 
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			releaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Remove all leading and trailing whitespace
	CStringT& Trim()
	{
		return( TrimRight().TrimLeft() );
	}

	// Remove all leading and trailing occurrences of character 'chTarget'
	CStringT& Trim( _In_ XCHAR chTarget )
	{
		return( TrimRight( chTarget ).TrimLeft( chTarget ) );
	}

	// Remove all leading and trailing occurrences of any of the characters in the string 'pszTargets'
	CStringT& Trim( _In_ PCXSTR pszTargets )
	{
		return( TrimRight( pszTargets ).TrimLeft( pszTargets ) );
	}

	// trimming anything (either side)

	// Remove all trailing occurrences of character 'chTarget'
	CStringT& TrimRight( _In_ XCHAR chTarget )
	{
		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		PCXSTR psz = GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( *psz == chTarget )
			{
				if( pszLast == NULL )
				{
					pszLast = psz;
				}
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at left-most matching character  
			int iLast = int( pszLast-GetString() );
			Truncate( iLast );
		}

		return( *this );
	}

	// Remove all trailing occurrences of any of the characters in string 'pszTargets'
	CStringT& TrimRight( _In_ PCXSTR pszTargets )
	{
		// if we're not trimming anything, we're not doing any work
		if( (pszTargets == NULL) || (*pszTargets == 0) )
		{
			return( *this );
		}

		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		PCXSTR psz = GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( StringTraits::StringFindChar( pszTargets, *psz ) != NULL )
			{
				if( pszLast == NULL )
				{
					pszLast = psz;
				}
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at left-most matching character  
			int iLast = int( pszLast-GetString() );
			Truncate( iLast );
		}

		return( *this );
	}

	// Remove all leading occurrences of character 'chTarget'
	CStringT& TrimLeft( _In_ XCHAR chTarget )
	{
		// find first non-matching character
		PCXSTR psz = GetString();

		while( chTarget == *psz )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != GetString() )
		{
			// fix up data and length
			int iFirst = int( psz-GetString() );
			PXSTR pszBuffer = GetBuffer( GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = GetLength()-iFirst;
			ATL_HELPER::memmove_s( pszBuffer, (GetLength()+1)*sizeof( XCHAR ), 
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			releaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Remove all leading occurrences of any of the characters in string 'pszTargets'
	CStringT& TrimLeft( _In_ PCXSTR pszTargets )
	{
		// if we're not trimming anything, we're not doing any work
		if( (pszTargets == NULL) || (*pszTargets == 0) )
		{
			return( *this );
		}

		PCXSTR psz = GetString();
		while( (*psz != 0) && (StringTraits::StringFindChar( pszTargets, *psz ) != NULL) )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != GetString() )
		{
			// fix up data and length
			int iFirst = int( psz-GetString() );
			PXSTR pszBuffer = GetBuffer( GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = GetLength()-iFirst;
			ATL_HELPER::memmove_s( pszBuffer, (GetLength()+1)*sizeof( XCHAR ), 
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			releaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Convert the string to the OEM character set
	void AnsiToOem()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::ConvertToOem( pszBuffer, nLength+1 );
		releaseBufferSetLength( nLength );
	}

	// Convert the string to the ANSI character set
	void OemToAnsi()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::ConvertToAnsi( pszBuffer, nLength+1 );
		releaseBufferSetLength( nLength );
	}

	// Very simple sub-string extraction

	// Return the substring starting at index 'iFirst'
	CStringT Mid( _In_ int iFirst ) const
	{
		return( Mid( iFirst, GetLength()-iFirst ) );
	}

	// Return the substring starting at index 'iFirst', with length 'nCount'
	CStringT Mid( _In_ int iFirst, _In_ int nCount ) const
	{
		// nCount is in XCHARs

		// out-of-bounds requests return sensible things
		if (iFirst < 0)
			iFirst = 0;

		if (nCount < 0)
			nCount = 0;

//		if( (::ATL::AtlAddThrow(iFirst, nCount)) > GetLength() )
		if( (iFirst + nCount) > GetLength() )//Modified by TaoRenyong 20110923
		{
			nCount = GetLength()-iFirst;
		}
		if( iFirst > GetLength() )
		{
			nCount = 0;
		}

		ATLASSERT( (nCount == 0) || ((iFirst+nCount) <= GetLength()) );

		// optimize case of returning entire string
		if( (iFirst == 0) && ((iFirst+nCount) == GetLength()) )
		{
			return( *this );
		}

//		return( CStringT( GetString()+iFirst, nCount, GetManager() ) );
		return( CStringT( GetString()+iFirst, nCount ) ); //Modified by TaoRenyong 20110923
	}

	// Return the substring consisting of the rightmost 'nCount' characters
	CStringT Right( _In_ int nCount ) const
	{
		// nCount is in XCHARs
		if (nCount < 0)
			nCount = 0;

		int nLength = GetLength();
		if( nCount >= nLength )
		{
			return( *this );
		}

//		return( CStringT( GetString()+nLength-nCount, nCount, GetManager() ) );
		return( CStringT( GetString()+nLength-nCount, nCount ) ); //Modified by TaoRenyong 20110923
	}

	// Return the substring consisting of the leftmost 'nCount' characters
	CStringT Left( _In_ int nCount ) const
	{
		// nCount is in XCHARs
		if (nCount < 0)
			nCount = 0;

		int nLength = GetLength();
		if( nCount >= nLength )
		{
			return( *this );
		}

//		return( CStringT( GetString(), nCount, GetManager() ) );
		return( CStringT( GetString(), nCount ) ); //Modified by TaoRenyong 20110923
	}

	// Return the substring consisting of the leftmost characters in the set 'pszCharSet'
	CStringT SpanIncluding( _In_ PCXSTR pszCharSet ) const
	{
		//ATLASSERT( AtlIsValidString( pszCharSet ) ); //Modified by TaoRenyong 20110923
		if(pszCharSet == NULL)
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);

		return( Left( StringTraits::StringSpanIncluding( GetString(), pszCharSet ) ) );
	}

	// Return the substring consisting of the leftmost characters not in the set 'pszCharSet'
	CStringT SpanExcluding( _In_ PCXSTR pszCharSet ) const
	{
		//ATLASSERT( AtlIsValidString( pszCharSet ) ); //Modified by TaoRenyong 20110923
		if(pszCharSet == NULL)
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);

		return( Left( StringTraits::StringSpanExcluding( GetString(), pszCharSet ) ) );
 	}

	// Format data using format string 'pszFormat'
	void __cdecl Format( _In_ _Printf_format_string_ PCXSTR pszFormat, ... );

	// Format data using format string loaded from resource 'nFormatID'
	void __cdecl Format( _Printf_format_string_ UINT nFormatID, ... );

	// Append formatted data using format string loaded from resource 'nFormatID'
	void __cdecl AppendFormat( _In_ UINT nFormatID, ... );

	// Append formatted data using format string 'pszFormat'
	void __cdecl AppendFormat( _In_ _Printf_format_string_ PCXSTR pszFormat, ... );
	void AppendFormatV( _In_ _Printf_format_string_ PCXSTR pszFormat, va_list args )
	{
		//ATLASSERT( AtlIsValidString( pszFormat ) );
		ATLASSERT(NULL != pszFormat); //Modified by TaoRenyong 20110923

		int nCurrentLength = GetLength();
		int nAppendLength = StringTraits::GetFormattedLength( pszFormat, args );
		PXSTR pszBuffer = GetBuffer( nCurrentLength+nAppendLength );
		StringTraits::Format( pszBuffer+nCurrentLength, 
			nAppendLength+1, pszFormat, args );
		releaseBufferSetLength( nCurrentLength+nAppendLength );
	}

	void FormatV( _In_ _Printf_format_string_ PCXSTR pszFormat, va_list args )
	{
		//ATLASSERT( AtlIsValidString( pszFormat ) ); //Modified by TaoRenyong 20110923
		if(pszFormat == NULL)
			ATL_MACRO::AtlThrowImpl(E_INVALIDARG);

		int nLength = StringTraits::GetFormattedLength( pszFormat, args );
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::Format( pszBuffer, nLength+1, pszFormat, args );
		releaseBufferSetLength( nLength );
	}

	// Format a message using format string 'pszFormat'
	void __cdecl _AFX_FUNCNAME(FormatMessage)( _In_ _Printf_format_string_ PCXSTR pszFormat, ... );

	// Format a message using format string loaded from resource 'nFormatID'
	void __cdecl _AFX_FUNCNAME(FormatMessage)( _In_ UINT nFormatID, ... );

#if defined(_AFX)
	void __cdecl FormatMessage( _In_ _Printf_format_string_ PCXSTR pszFormat, ... );

	void __cdecl FormatMessage( _In_ UINT nFormatID, ... );
#endif

	// Format a message using format string 'pszFormat' and va_list
	void FormatMessageV( _In_ _Printf_format_string_ PCXSTR pszFormat, va_list* pArgList )
	{
		// format message into temporary buffer pszTemp
		CHeapPtr< XCHAR, CLocalAllocator > pszTemp;
		DWORD dwResult = StringTraits::_AFX_FUNCNAME(FormatMessage)( FORMAT_MESSAGE_FROM_STRING|
			FORMAT_MESSAGE_ALLOCATE_BUFFER, pszFormat, 0, 0, reinterpret_cast< PXSTR >( &pszTemp ),
			0, pArgList );
		if( dwResult == 0 )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}

		*this = pszTemp;
	}

	// OLE BSTR support

	// Allocate a BSTR containing a copy of the string
	BSTR AllocSysString() const
	{
		BSTR bstrResult = StringTraits::AllocSysString( GetString(), GetLength() );

#pragma warning(push)
#pragma warning(disable:4068)
#pragma prefast(push)
#pragma prefast(disable:325, "We are deliberately checking if this has already been allocated")
		if( bstrResult == NULL )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}
#pragma prefast(pop)
#pragma warning(pop)

		return( bstrResult );
	}

	BSTR SetSysString( _Out_ BSTR* pbstr ) const
	{
		ATLASSERT(pbstr != NULL); //Modified by TaoRenyong 20110923 AtlIsValidAddress

		if( !StringTraits::ReAllocSysString( GetString(), pbstr,
			GetLength() ) )
		{
			ATL_MACRO::AtlThrowImpl(E_OUTOFMEMORY);
		}

#pragma warning(push)
#pragma warning(disable:4068)
#pragma prefast(push)
#pragma prefast(disable:325, "We are deliberately checking if this has already been allocated")
		ATLASSERT( *pbstr != NULL );
#pragma prefast(pop)
#pragma warning(pop)

		return( *pbstr );
	}

	// Set the string to the value of environment variable 'pszVar'
	_Check_return_ BOOL GetEnvironmentVariable( _In_ PCXSTR pszVar )
	{
		ULONG nLength = StringTraits::GetEnvironmentVariable( pszVar, NULL, 0 );
		BOOL bRetVal = FALSE;

		if( nLength == 0 )
		{
			Empty();
		}
		else
		{
			PXSTR pszBuffer = GetBuffer( nLength );
			StringTraits::GetEnvironmentVariable( pszVar, pszBuffer, nLength );
			releaseBuffer();
			bRetVal = TRUE;
		}

		return( bRetVal );
	}

	/* Added by TaoRenyong 20110923 below */
	// Windows support
	BOOL LoadString(UINT nID)   // load from string resource (255 chars max.)
	{
#ifdef _UNICODE
		const int CHAR_FUDGE = 1;   // one TCHAR unused is good enough
#else
		const int CHAR_FUDGE = 2;   // two BYTES unused for case of DBC last char
#endif

		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[256];
		int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
		int nLen = _LoadString(nID, szTemp, nCount);
		if (nCount - nLen > CHAR_FUDGE)
		{
			*this = szTemp;
			return (nLen > 0);
		}

		// try buffer size of 512, then larger size until entire string is retrieved
		int nSize = 256;
		do
		{
			nSize += 256;
			LPTSTR lpstr = GetBuffer(nSize - 1);
			if(lpstr == NULL)
			{
				nLen = 0;
				break;
			}
			nLen = _LoadString(nID, lpstr, nSize);
		} while (nSize - nLen <= CHAR_FUDGE);
		releaseBuffer();

		return (nLen > 0);
	}

	static HINSTANCE __stdcall _GetResourceInstance()
	{
//		return AfxGetResourceHandle();
		ATLASSERT(FALSE);
		return NULL;
	}

	static int __stdcall _LoadString(UINT nID, LPTSTR lpszBuf, UINT nMaxBuf)
	{
#ifdef _DEBUG
		// LoadString without annoying warning from the Debug kernel if the
		//  segment containing the string is not present
		if (::FindResource(_GetResourceInstance(), MAKEINTRESOURCE((nID >> 4) + 1), RT_STRING) == NULL)
		{
			lpszBuf[0] = _T('\0');
			return 0;   // not found
		}
#endif // _DEBUG

		int nLen = ::LoadString(_GetResourceInstance(), nID, lpszBuf, nMaxBuf);
		if (nLen == 0)
			lpszBuf[0] = _T('\0');

		return nLen;
	}
	/* Added by TaoRenyong 20110923 above */

	friend CStringT operator+( _In_ const CStringT& str1, _In_ const CStringT& str2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923

		Concatenate( strResult, str1, str1.GetLength(), str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+( _In_ const CStringT& str1, _In_ PCXSTR psz2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923

		Concatenate( strResult, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

		return( strResult );
	}

	friend CStringT operator+( _In_ PCXSTR psz1, _In_ const CStringT& str2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923

		Concatenate( strResult, psz1, StringLength( psz1 ), str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+( _In_ const CStringT& str1, _In_ wchar_t ch2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923
		XCHAR chTemp = XCHAR( ch2 );

		Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

		return( strResult );
	}

	friend CStringT operator+( _In_ const CStringT& str1, _In_ char ch2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923
		XCHAR chTemp = XCHAR( ch2 );

		Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

		return( strResult );
	}

	friend CStringT operator+( _In_ wchar_t ch1, _In_ const CStringT& str2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923
		XCHAR chTemp = XCHAR( ch1 );

		Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+( _In_ char ch1, _In_ const CStringT& str2 )
	{
//		CStringT strResult( str1.GetManager() );
		CStringT strResult; //Modified by TaoRenyong 20110923
		XCHAR chTemp = XCHAR( ch1 );

		Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

		return( strResult );
	}

	friend bool operator==( _In_ const CStringT& str1, _In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) == 0 );
	}

	friend bool operator==(
		_In_ const CStringT& str1, _In_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) == 0 );
	}

	friend bool operator==(
		_In_ PCXSTR psz1, _In_ const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) == 0 );
	}

	friend bool operator==(
		_In_ const CStringT& str1, _In_ PCYSTR psz2 ) throw( ... )
	{
//		CStringT str2( psz2, str1.GetManager() );
		CStringT str2( psz2 ); //Modified by TaoRenyong 20110923

		return( str1 == str2 );
	}

	friend bool operator==(
		_In_ PCYSTR psz1, _In_ const CStringT& str2 ) throw( ... )
	{
//		CStringT str1( psz1, str2.GetManager() );
		CStringT str1( psz1 ); //Modified by TaoRenyong 20110923

		return( str1 == str2 );
	}

	friend bool operator!=(
		_In_ const CStringT& str1, _In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) != 0 );
	}

	friend bool operator!=(
		_In_ const CStringT& str1, _In_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) != 0 );
	}

	friend bool operator!=(
		_In_ PCXSTR psz1, _In_ const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) != 0 );
	}

	friend bool operator!=(
		_In_ const CStringT& str1, _In_ PCYSTR psz2 ) throw( ... )
	{
//		CStringT str2( psz2, str1.GetManager() );
		CStringT str2( psz2 ); //Modified by TaoRenyong 20110923

		return( str1 != str2 );
	}

	friend bool operator!=(
		_In_ PCYSTR psz1, _In_ const CStringT& str2 ) throw( ... )
	{
//		CStringT str1( psz1, str2.GetManager() );
		CStringT str1( psz1 ); //Modified by TaoRenyong 20110923

		return( str1 != str2 );
	}

	friend bool operator<( _In_ const CStringT& str1, _In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) < 0 );
	}

	friend bool operator<( _In_ const CStringT& str1, _In_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) < 0 );
	}

	friend bool operator<( _In_ PCXSTR psz1, _In_ const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) > 0 );
	}

	friend bool operator>( _In_ const CStringT& str1, _In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) > 0 );
	}

	friend bool operator>( _In_ const CStringT& str1, _In_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) > 0 );
	}

	friend bool operator>( _In_ PCXSTR psz1, _In_ const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) < 0 );
	}

	friend bool operator<=( _In_ const CStringT& str1, _In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) <= 0 );
	}

	friend bool operator<=( _In_ const CStringT& str1, _In_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) <= 0 );
	}

	friend bool operator<=( _In_ PCXSTR psz1, _In_ const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) >= 0 );
	}

	friend bool operator>=( _In_ const CStringT& str1, _In_ const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) >= 0 );
	}

	friend bool operator>=( _In_ const CStringT& str1, _In_ PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) >= 0 );
	}

	friend bool operator>=( _In_ PCXSTR psz1, _In_ const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) <= 0 );
	}

	friend bool operator==( _In_ XCHAR ch1, _In_ const CStringT& str2 ) throw()
	{
		return( (str2.GetLength() == 1) && (str2[0] == ch1) );
	}

	friend bool operator==( _In_ const CStringT& str1, _In_ XCHAR ch2 ) throw()
	{
		return( (str1.GetLength() == 1) && (str1[0] == ch2) );
	}

	friend bool operator!=( _In_ XCHAR ch1, _In_ const CStringT& str2 ) throw()
	{
		return( (str2.GetLength() != 1) || (str2[0] != ch1) );
	}

	friend bool operator!=( _In_ const CStringT& str1, _In_ XCHAR ch2 ) throw()
	{
		return( (str1.GetLength() != 1) || (str1[0] != ch2) );
	}

private:
	bool CheckImplicitLoad( _In_opt_ const void* pv )
	{
		bool bRet = false;

		if( (pv != NULL) && IS_INTRESOURCE( pv ) )
		{
			UINT nID = LOWORD( reinterpret_cast< DWORD_PTR >( pv ) );
			if( !LoadString( nID ) )
			{
				ATLASSERT(!"Warning: implicit LoadString failed\n");
			}
			bRet = true;
		}

		return( bRet );
	}

};

#ifndef _CSTRING_BUFFER_SIZE
#define _CSTRING_BUFFER_SIZE(_CStringObj) ((_CStringObj).GetAllocLength() + 1)
#endif

#pragma warning(push)
#pragma warning(disable : 4793)
// Format data using format string 'pszFormat'
template< typename BaseType >
inline void __cdecl CStringT<BaseType>::Format( _In_ _Printf_format_string_ PCXSTR pszFormat, ... )
{
	//ATLASSERT( AtlIsValidString( pszFormat ) );
	ATLASSERT(NULL != pszFormat); //Modified by TaoRenyong 20110923

	va_list argList;
	va_start( argList, pszFormat );
	FormatV( pszFormat, argList );
	va_end( argList );
}

// Append formatted data using format string 'pszFormat'
template< typename BaseType >
inline void __cdecl CStringT<BaseType>::AppendFormat( _In_ _Printf_format_string_ PCXSTR pszFormat, ... )
{
	//ATLASSERT( AtlIsValidString( pszFormat ) );
	ATLASSERT(NULL != pszFormat); //Modified by TaoRenyong 20110923

	va_list argList;
	va_start( argList, pszFormat );

	AppendFormatV( pszFormat, argList );

	va_end( argList );
}

// Format a message using format string 'pszFormat'
template< typename BaseType>
inline void __cdecl CStringT<BaseType>::_AFX_FUNCNAME(FormatMessage)( _In_ _Printf_format_string_ PCXSTR pszFormat, ... )
{
	if(pszFormat == NULL)
		ATL_MACRO::AtlThrowImpl(E_INVALIDARG);

	va_list argList;
	va_start( argList, pszFormat );

	FormatMessageV( pszFormat, &argList );

	va_end( argList );
}

#if defined(_AFX)
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::FormatMessage( _In_ _Printf_format_string_ PCXSTR pszFormat, ... )
{
	if(pszFormat == NULL)
		ATL_MACRO::AtlThrowImpl(E_INVALIDARG);

	va_list argList;
	va_start( argList, pszFormat );

	FormatMessageV( pszFormat, &argList );

	va_end( argList );
}
#endif

#pragma warning( pop )

#pragma pop_macro("new")

#if defined(_AFX)
#pragma pop_macro("FormatMessage")
#endif

};  // namespace ATL_STR

#ifndef _ATL_NO_PRAGMA_WARNINGS
#pragma warning (pop)
#endif



