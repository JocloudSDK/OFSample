#include "stable.h"

#include "environmentpath.h"

static std::wstring _appRootPath;

static std::wstring _getModulePath(HMODULE hModule)
{
    WCHAR szFilePath[MAX_PATH + 1] = { 0 };
    ::GetModuleFileName(hModule, szFilePath, MAX_PATH);

    WCHAR* lpszSlash = wcsrchr(szFilePath, L'\\');
    if (lpszSlash != NULL)
    {
        *(lpszSlash + 1) = L'\0';
    }

    return szFilePath;
}

static std::wstring _formatPath(const std::wstring& path)
{
    std::wstring result(path);
    std::replace(result.begin(), result.end(), L'\\', L'/');
    if ( !result.empty() && result.find_last_of(L"/") != result.size() - 1)
    {
        result += L"/";
    }

    return result;
}

static std::wstring _getEnvironmentPath(const wchar_t* lpszEnvirVar)
{
    std::wstring envirPath;

    WCHAR szEnvirValue[MAX_PATH + 1] = { 0 };
    if (GetEnvironmentVariable(lpszEnvirVar, szEnvirValue, MAX_PATH))
    {
        envirPath = szEnvirValue;
    }
    else
    {
        envirPath = _getModulePath(NULL);
    }

    return _formatPath(envirPath);
}

namespace DwUtility
{
	namespace app
	{
		std::wstring appRootPath()
		{
			if (_appRootPath.empty())
			{
				_appRootPath = _getEnvironmentPath(ENV_APPROOT_PATH);
			}

			return _appRootPath;
		}
	}
}
