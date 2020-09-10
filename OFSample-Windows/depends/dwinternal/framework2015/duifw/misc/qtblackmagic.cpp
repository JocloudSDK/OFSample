#include "stable.h"
#include "qtblackmagic.h"

namespace
{
#ifdef _DEBUG
	const wchar_t KQtGui4Dll[] = L"QtGuid4.dll";
#else
	const wchar_t KQtGui4Dll[] = L"QtGui4.dll";
#endif

	template <typename T1>
	static bool inline callFunc(const wchar_t *moduleName, const char *funcName, T1 arg1)
	{
		typedef void(*fnT)(T1);
		if (fnT fn = (fnT)::GetProcAddress(::GetModuleHandleW(moduleName), funcName))
		{
			fn(arg1);
			return true;
		}
		return false;
	}

	template <typename T1, typename T2>
	static bool inline callFunc(const wchar_t *moduleName, const char *funcName, T1 arg1, T2 arg2)
	{
		typedef void(*fnT)(T1, T2);
		if (fnT fn = (fnT)::GetProcAddress(::GetModuleHandleW(moduleName), funcName))
		{
			fn(arg1, arg2);
			return true;
		}
		return false;
	}
}

void setDefaultLoadAllFonts(const bool b)
{
	callFunc(KQtGui4Dll, "?setDefaultLoadAllFonts@@YAX_N@Z", b);
}

void setQLineEditDefaultMargin(int vertical, int horizontal)
{
	callFunc(KQtGui4Dll, "?setQLineEditDefaultMargin@@YAXHH@Z", vertical, horizontal);
}

void setDefaultFontConfig(const QStringList &fallbacks, const QStringList &blacklist)
{
	callFunc<const QStringList &, const QStringList &>(KQtGui4Dll, "?setDefaultFontConfig@@YAXABVQStringList@@0@Z", fallbacks, blacklist);
}
