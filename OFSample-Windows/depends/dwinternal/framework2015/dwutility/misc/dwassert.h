#pragma once

#include <cstdio>
#include <crtdbg.h>
#include <windows.h>

#include "dwglobal.h"

#ifdef _DEBUG
	bool DW_DWUTILITY_EXPORT dwAssert(bool exp, const char *file, const char *where, int line, const char *what);
	bool DW_DWUTILITY_EXPORT dwAssertOnce(bool exp, const char *file, const char *where, int line, const char *what);

	#define DW_ASSERT(exp) \
		do { \
			if (dwAssert((bool(exp)), __FILE__, "", __LINE__, "")) \
				DebugBreak(); \
		} while(0)

	#define DW_ASSERT_X(exp, where, what) \
		do { \
			if (dwAssert((bool(exp)), __FILE__, where, __LINE__, what)) \
				DebugBreak(); \
		} while(0)

	#define DW_ASSERT_ONCE(exp) \
		do { \
			if (dwAssertOnce((bool(exp)), __FILE__, "", __LINE__, "")) \
				DebugBreak(); \
		} while(0)
	#define DW_ASSERT_ONCE_X(exp, where, what) \
		do { \
			if (dwAssertOnce((bool(exp)), __FILE__, where, __LINE__, what)) \
				DebugBreak(); \
		} while(0)
#else
	#define DW_ASSERT(exp) do { \
		if (0) (void)(exp);\
	} while(0)

	#define DW_ASSERT_X(exp, where, what) do {\
		if (0)	\
		{	\
			(void)(where); \
			(void)(what); \
			(void)(exp);  \
		}\
	} while(0)

	#define DW_ASSERT_ONCE(exp)
	#define DW_ASSERT_ONCE_X(exp, where, what)
#endif
