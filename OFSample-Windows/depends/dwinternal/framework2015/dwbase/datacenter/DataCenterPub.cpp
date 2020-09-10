#pragma once

#include "stable.h"
#include "../log/log.h"

namespace Data
{
	namespace Error
	{
		void OutputDebug(const TCHAR* lpszFmt, ...)
		{
			va_list args;
			va_start(args, lpszFmt);
			
			TCHAR szTemp[4096] = {0};
			_vsntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, lpszFmt, args);
			_tcsncat_s(szTemp, _countof(szTemp), _T("\r\n"), _TRUNCATE);
			OutputDebugString(szTemp);
			
			va_end(args);
		}

		void OutputWarning(const TCHAR* lpszFmt, ...)
		{
			va_list args;
			va_start(args, lpszFmt);

			TCHAR szTemp[4096] = {0};
			_vsntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, lpszFmt, args);
			LogFinal(LOG::KDataCenter) << _T("Warning: ") << szTemp;

#ifndef OFFICIAL_BUILD
			MessageBox(NULL, szTemp, _T("内部开发版本调试警告"), MB_OK|MB_SERVICE_NOTIFICATION);
#endif

			va_end(args);
		}

		void OutputError(const TCHAR* lpszFmt, ...)
		{
			va_list args;
			va_start(args, lpszFmt);

			TCHAR szTemp[4096] = {0};
			_vsntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, lpszFmt, args);
			LogFinal(LOG::KDataCenter) << _T("Error: ") << szTemp;

#ifndef OFFICIAL_BUILD
			MessageBox(NULL, szTemp, _T("内部开发版本调试错误"), MB_OK|MB_SERVICE_NOTIFICATION);
			::DebugBreak();
#endif

			va_end(args);
		}
	}
};