#pragma once

#ifdef QT_DLL
	#include <QtGlobal>
	#define DW_DECL_EXPORT Q_DECL_EXPORT
	#define DW_DECL_IMPORT Q_DECL_IMPORT
#else
	#define DW_DECL_EXPORT __declspec(dllexport)
	#define DW_DECL_IMPORT __declspec(dllimport)
#endif

#  ifdef BUILD_DW_BUS
#    define DW_DWBUS_EXPORT DW_DECL_EXPORT
#  else
#    define DW_DWBUS_EXPORT DW_DECL_IMPORT
#  endif // BUILD_DW_BUS

#  ifdef BUILD_DW_DUIFW
#    define DW_DUIFW_EXPORT DW_DECL_EXPORT
#  else
#    define DW_DUIFW_EXPORT DW_DECL_IMPORT
#  endif // BUILD_DW_DUIFW

#ifdef BUILD_YYWIDGET_COMMON
#  define YYWIDGET_COMMON_EXPORT DW_DECL_EXPORT
#else
#  define YYWIDGET_COMMON_EXPORT DW_DECL_IMPORT
#endif // BUILD_YYWIDGETS_COMMON

#ifdef BUILD_YYWIDGET_CHANNEL
#  define YYWIDGET_CHANNEL_EXPORT DW_DECL_EXPORT
#else
#  define YYWIDGET_CHANNEL_EXPORT DW_DECL_IMPORT
#endif // BUILD_YYWIDGETS_COMMON

#	ifdef BUILD_DW_DWUTILITY
#	define DW_DWUTILITY_EXPORT DW_DECL_EXPORT
#elif  defined(BUILD_DW_DWUTILITY_STATIC)
#	define DW_DWUTILITY_EXPORT
#else
#	define DW_DWUTILITY_EXPORT DW_DECL_IMPORT
#endif

#	ifdef BUILD_DWBASE
#	define DWBASE_EXPORTS DW_DECL_EXPORT
#elif   defined(BUILD_DWBASE_STATIC)
#	define DWBASE_EXPORTS
#else
#	define DWBASE_EXPORTS DW_DECL_IMPORT
#endif

#	ifdef BUILD_DW_DWCOMSTORE
#	define	DWCOMSTORE_EXPORTS	DW_DECL_EXPORT
#else
#	define	DWCOMSTORE_EXPORTS	DW_DECL_IMPORT
#endif

#define DW_SAFE_DELETE(p) { delete (p); p = NULL; }
#define DW_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define DW_KILLTIMER(timerid) { if(0 != timerid) { killTimer(timerid);timerid = 0;} }
#define DW_UNUSED(x) (void)x;

template<bool>
struct CompileTimeChecker
{
	template<typename T>
	CompileTimeChecker(T t) {(t);};
};

template<> struct CompileTimeChecker<false> ;

#define DWJOIN2(x,y) x##y
#define DWJOIN1(x,y) DWJOIN2(x,y)
#define DWJOIN(x) DWJOIN1(x,__COUNTER__)

#define DWSTATIC_CHECK(expr, msg)	\
struct DWJOIN(_msg){	\
class Error_##msg {} _e;	\
	void f() {CompileTimeChecker<(expr) != 0> checker(_e);}	\
};

#ifdef DW_ENABLE_CHECKSIZEOF
#define DWCHECKSIZEOF(cls, size)	DWSTATIC_CHECK((sizeof(cls) == size), binary_compatible_error)
#else
#define DWCHECKSIZEOF(cls, size)
#endif

#pragma warning(error: 4715)
#pragma warning(disable: 4251)

#ifndef DW_ENABLE_EXCEPTION
#undef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#pragma warning(disable:4275)
#pragma warning(disable:4805)
#endif	//DW_ENABLE_EXCEPTION

#define ENV_APPROOT_PATH	L"APPLICATION_ROOT_PATH"


#define QStringFromWCharArray(s) QString((const QChar*)(s))

#ifdef QT_DLL
#include <QString>
inline QString QStringFromWString(const std::wstring& s)
{
	return QString((const QChar*)s.c_str(), s.size());
}
template<size_t size>
inline QString QStringFromLatin1(const char (&str)[size])
{
	return QString::fromLatin1(str, size - 1);
}
#endif

#include "dwutility/dwassert.h"