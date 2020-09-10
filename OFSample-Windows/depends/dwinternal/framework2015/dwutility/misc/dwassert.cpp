#include "stable.h"
#include <cassert>
#include "dwassert.h"

#ifdef _DEBUG
struct DwAssertInfo
{
	const char *file;
	const char *where;
	const char *what;
	int line;
	bool bAssert;	//是否需要在返回后assert
};

std::set<UINT64> _assertOnceSet;

DWORD WINAPI assertProc(LPVOID lpParameter)
{
	DwAssertInfo *info = (DwAssertInfo*)lpParameter;
	int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
	_CrtSetReportMode(_CRT_ERROR, reportMode);
	int ret = _CrtDbgReport(_CRT_ERROR, info->file, info->line, info->where, info->what);
	if (ret == 0  && reportMode & _CRTDBG_MODE_WNDW)
	{
		return 0; // ignore
	}
	else if (ret == 1)
	{
		//_CrtDbgBreak();
		info->bAssert = true;
	}
	return 0;
}

bool dwAssert(bool exp, const char *file, const char *where, int line, const char *what)
{
	if(!(exp))
	{
		DwAssertInfo info = {file, where, what, line, false};
		HANDLE handle = ::CreateThread(NULL, NULL, assertProc, (LPVOID)(&info), 0, 0);
		::WaitForSingleObject(handle, INFINITE);
		::CloseHandle(handle);

		return info.bAssert;
	}

	return false;
}

UINT64 makeAssertHash(const char *file, int line)
{
	UINT64 h = ((UINT64)file) << 32;
	UINT64 l = line;
	return h | l;
}

bool dwAssertOnce(bool exp, const char *file, const char *where, int line, const char *what)
{
	UINT64 assertHash = makeAssertHash(file, line);
	if(_assertOnceSet.find(assertHash) != _assertOnceSet.end())
	{
		return false;		
	}

	_assertOnceSet.insert(assertHash);
	return dwAssert(exp, file, where, line, what);	
}

#endif //_DEBUG