#pragma once

#include <windows.h>	//type define
#include <wchar.h>		//wmemcpy support
#include <tchar.h>		//_tcs*** support
#include <cstdio>		//vstprintf support
#include <stdarg.h>		//arglist support
#include <cstdlib>		//itot/a*** support
#include <crtdbg.h> 	//assert support

#include <sal.h>
#include <mbstring.h>	//mbstring support
#include <OleAuto.h>	//BSTR support

#include "AtlMacro.h"

namespace ATL_HELPER
{
	inline void __cdecl memcpy_s(_Out_bytecap_post_bytecount_(_S1max,_N)  void *_S1, _In_ size_t _S1max, _In_bytecount_(_N) const void *_S2, _In_ size_t _N)
	{
		ATL_MACRO::AltErrorCheck(::memcpy_s(_S1, _S1max, _S2, _N));
	}

	inline void __cdecl wmemcpy_s(_Out_cap_post_count_(_N1,_N) wchar_t *_S1, _In_ size_t _N1, _In_count_(_N) const wchar_t *_S2, _In_ size_t _N)
	{
		ATL_MACRO::AltErrorCheck(::wmemcpy_s(_S1, _N1, _S2, _N));
	}

	inline void __cdecl memmove_s(_Out_bytecap_post_bytecount_(_S1max,_N) void *_S1, _In_ size_t _S1max, _In_bytecount_(_N) const void *_S2, size_t _N)
	{
		ATL_MACRO::AltErrorCheck(::memmove_s(_S1, _S1max, _S2, _N));
	}

	inline void __cdecl strcpy_s(_Out_cap_(_S1max) char *_S1, _In_ size_t _S1max, _In_z_ const char *_S2)
	{
		ATL_MACRO::AltErrorCheck(::strcpy_s(_S1, _S1max, _S2));
	}

	inline void __cdecl wcscpy_s(_Out_cap_(_S1max) wchar_t *_S1, _In_ size_t _S1max, _In_z_ const wchar_t *_S2)
	{
		ATL_MACRO::AltErrorCheck(::wcscpy_s(_S1, _S1max, _S2));
	}

	inline void __cdecl tcscpy_s(_Out_cap_(_SizeInChars) TCHAR * _Dst, _In_ size_t _SizeInChars, _In_z_ const TCHAR * _Src)
	{
		ATL_MACRO::AltErrorCheck(::_tcscpy_s(_Dst, _SizeInChars, _Src));
	}

	inline errno_t __cdecl strncpy_s(_Out_cap_(_SizeInChars) char *_Dest, _In_ size_t _SizeInChars, _In_z_ const char *_Source, _In_ size_t _Count)
	{
		return ATL_MACRO::AltErrorCheck(::strncpy_s(_Dest, _SizeInChars, _Source,_Count));	
	}

	inline errno_t __cdecl wcsncpy_s(_Out_cap_(_SizeInChars) wchar_t *_Dest, _In_ size_t _SizeInChars, _In_z_ const wchar_t *_Source, _In_ size_t _Count)
	{
		return ATL_MACRO::AltErrorCheck(::wcsncpy_s(_Dest, _SizeInChars, _Source,_Count));	
	}

	inline errno_t __cdecl tcsncpy_s(_Out_cap_(_SizeInChars) TCHAR *_Dest, _In_ size_t _SizeInChars, _In_z_ const TCHAR *_Source, _In_ size_t _Count)
	{
		return ATL_MACRO::AltErrorCheck(::_tcsncpy_s(_Dest, _SizeInChars, _Source,_Count));	
	}

	inline void __cdecl strcat_s(_Inout_z_cap_(_SizeInChars) char * _Dst, _In_ size_t _SizeInChars, _In_z_ const char * _Src)
	{
		ATL_MACRO::AltErrorCheck(::strcat_s(_Dst, _SizeInChars, _Src));
	}

	inline void __cdecl wcscat_s(_Inout_z_cap_(_SizeInChars) wchar_t * _Dst, _In_ size_t _SizeInChars, _In_z_ const wchar_t * _Src)
	{
		ATL_MACRO::AltErrorCheck(::wcscat_s(_Dst, _SizeInChars, _Src));
	}

	inline void __cdecl tcscat_s(_Inout_z_cap_(_SizeInChars) TCHAR * _Dst, _In_ size_t _SizeInChars, _In_z_ const TCHAR * _Src)
	{
		ATL_MACRO::AltErrorCheck(::_tcscat_s(_Dst, _SizeInChars, _Src));
	}

	inline void __cdecl strlwr_s(_Inout_z_cap_(_SizeInChars) char * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_strlwr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl wcslwr_s(_Inout_z_cap_(_SizeInChars) wchar_t * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_wcslwr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl mbslwr_s(_Inout_z_bytecap_(_SizeInChars) unsigned char * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_mbslwr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl tcslwr_s(_Inout_z_cap_(_SizeInChars) TCHAR * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_tcslwr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl strupr_s(_Inout_z_cap_(_SizeInChars) char * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_strupr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl wcsupr_s(_Inout_z_cap_(_SizeInChars) wchar_t * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_wcsupr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl mbsupr_s(_Inout_z_bytecap_(_SizeInChars) unsigned char * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_mbsupr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl tcsupr_s(_Inout_z_cap_(_SizeInChars) TCHAR * _Str, _In_ size_t _SizeInChars)
	{
		ATL_MACRO::AltErrorCheck(::_tcsupr_s(_Str, _SizeInChars));	
	}

	inline void __cdecl itoa_s(_In_ int _Val, _Out_z_cap_(_SizeInChars) char *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_itoa_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl itot_s(_In_ int _Val, _Out_z_cap_(_SizeInChars) TCHAR *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_itot_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ltoa_s(_In_ long _Val, _Out_z_cap_(_SizeInChars) char *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ltoa_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ltot_s(_In_ long _Val, _Out_z_cap_(_SizeInChars) TCHAR *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ltot_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ultoa_s(_In_ unsigned long _Val, _Out_z_cap_(_SizeInChars) char *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ultoa_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ultow_s(_In_ unsigned long _Val, _Out_z_cap_(_SizeInChars) wchar_t *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ultow_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ultot_s(_In_ unsigned long _Val, _Out_z_cap_(_SizeInChars) TCHAR *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ultot_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl i64toa_s(_In_ __int64 _Val, _Out_z_cap_(_SizeInChars) char *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_i64toa_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl i64tow_s(_In_ __int64 _Val, _Out_z_cap_(_SizeInChars) wchar_t *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_i64tow_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ui64toa_s(_In_ unsigned __int64 _Val, _Out_z_cap_(_SizeInChars) char *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ui64toa_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl ui64tow_s(_In_ unsigned __int64 _Val, _Out_z_cap_(_SizeInChars) wchar_t *_Buf, _In_ size_t _SizeInChars, _In_ int _Radix)
	{
		ATL_MACRO::AltErrorCheck(::_ui64tow_s(_Val, _Buf, _SizeInChars, _Radix));
	}

	inline void __cdecl gcvt_s(_Out_z_cap_(_SizeInChars) char *_Buffer, _In_ size_t _SizeInChars, _In_ double _Value, _In_ int _Ndec)
	{
		ATL_MACRO::AltErrorCheck(::_gcvt_s(_Buffer, _SizeInChars, _Value, _Ndec));
	}

	inline void __cdecl tsplitpath_s(_In_z_ const TCHAR *_Path, _Out_opt_z_cap_(_Drive_len) TCHAR *_Drive, _In_ size_t _Drive_len, 
		_Out_opt_z_cap_(_Dir_len) TCHAR *_Dir, _In_ size_t _Dir_len, 
		_Out_opt_z_cap_(_Fname_len) TCHAR *_Fname, _In_ size_t _Fname_len, 
		_Out_opt_z_cap_(_Ext_len) TCHAR *_Ext, _In_ size_t _Ext_len)
	{
		ATL_MACRO::AltErrorCheck(::_tsplitpath_s(_Path, _Drive, _Drive_len, _Dir, _Dir_len, _Fname, _Fname_len, _Ext, _Ext_len));
	}

	inline void __cdecl tmakepath_s(_Out_z_cap_(_SizeInChars) TCHAR *_Path, _In_ size_t _SizeInChars, _In_z_ const TCHAR *_Drive, 
		_In_z_ const TCHAR *_Dir, _In_z_ const TCHAR *_Fname, _In_z_ const TCHAR *_Ext)
	{
		ATL_MACRO::AltErrorCheck(::_tmakepath_s(_Path, _SizeInChars, _Drive, _Dir, _Fname, _Ext));
	}

	inline size_t __cdecl strnlen(_In_count_(_Maxsize) const char *_Str, _In_ size_t _Maxsize)
	{
		return ::strnlen(_Str, _Maxsize);
	}

	inline size_t __cdecl wcsnlen(_In_count_(_Maxsize) const wchar_t *_Wcs, _In_ size_t _Maxsize)
	{
		return ::wcsnlen(_Wcs, _Maxsize);
	}

	inline size_t __cdecl tcsnlen(_In_count_(_Maxsize) const TCHAR *_Str, _In_ size_t _Maxsize)
	{
		return ::_tcsnlen(_Str, _Maxsize);
	}

	//inline int get_errno() throw()
	//{
	//	int nErrNo=0;
	//	errno_t nErrCall=::_get_errno(&nErrNo);
	//	if(nErrCall)
	//	{
	//		return nErrCall;
	//	}
	//	return nErrNo;
	//}

	//inline void set_errno(_In_ int _Value)
	//{
	//	ATL_MACRO::AltErrorCheck(::_set_errno(_Value));
	//}

} // namespace ATL_HELPER