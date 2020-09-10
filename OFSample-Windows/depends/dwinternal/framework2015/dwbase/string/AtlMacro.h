#pragma once

#include <errno.h>

namespace ATL_MACRO
{
#ifndef ATLASSERT
#	define ATLASSERT(expr) _ASSERT(expr)
#endif // ATLASSERT

#ifndef ATLENSURE
#	define ATLENSURE(expr) _ASSERTE(expr)
#endif // ATLENSURE

#ifdef _ATL_DISABLE_NOINLINE
#	define ATL_NOINLINE
#else
#	define ATL_NOINLINE __declspec( noinline )
#endif

	ATL_NOINLINE inline void WINAPI AtlThrowImpl( HRESULT hr )
	{
		ATLASSERT( FALSE );
		DWORD dwExceptionCode;
		switch(hr)
		{
		case E_OUTOFMEMORY:
			dwExceptionCode = STATUS_NO_MEMORY;
			break;
		default:
			dwExceptionCode = EXCEPTION_ILLEGAL_INSTRUCTION;
		}

		RaiseException( dwExceptionCode, EXCEPTION_NONCONTINUABLE, 0, NULL );
	}

	inline errno_t AltErrorCheck(errno_t nError)
	{
		switch(nError)
		{
		case ENOMEM:
			AtlThrowImpl(E_OUTOFMEMORY);
			break;
		case EINVAL:
		case ERANGE:
			AtlThrowImpl(E_INVALIDARG);
			break;
		case 0:
		case STRUNCATE:
			break;
		default:
			AtlThrowImpl(E_FAIL);
			break;
		}

		return nError;
	}

}; //namespace ATL_MACRO
