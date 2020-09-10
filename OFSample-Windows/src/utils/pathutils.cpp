#include "pathutils.h"
#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>
#include <tchar.h>

std::string PathParentA(const std::string& path)
{
    std::string strTempPath(path);
    char strParentPath[ MAX_PATH ] = { 0 };

    if ('\\' == *(strTempPath.end() - 1) || '/' == *(strTempPath.end() - 1))
        strTempPath = strTempPath.substr(0, strTempPath.length() - 1);

    std::string::size_type pos = strTempPath.find_last_of('\\');
    if (pos != std::string::npos)
    {
        strcpy_s(strParentPath, MAX_PATH, strTempPath.substr(0, pos + 1).c_str());
    }
    else
    {
        pos = strTempPath.find_last_of('/');
        if (pos == std::string::npos)
            return std::string();

        strcpy_s(strParentPath, MAX_PATH, strTempPath.substr(0, pos + 1).c_str());
    }

    return strParentPath;
}

std::wstring PathParentW(const std::wstring& path)
{
    std::wstring strTempPath(path);
    wchar_t strParentPath[ MAX_PATH ] = { 0 };

    if (L'\\' == *(strTempPath.end() - 1) || L'/' == *(strTempPath.end() - 1))
        strTempPath = strTempPath.substr(0, strTempPath.length() - 1);

    std::wstring::size_type pos = strTempPath.find_last_of(L'\\');
    if (pos != std::wstring::npos)
    {
        wcscpy_s(strParentPath, MAX_PATH, strTempPath.substr(0, pos + 1).c_str());
    }
    else
    {
        pos = strTempPath.find_last_of(L'/');
        if (pos == std::wstring::npos)
            return std::wstring();

        wcscpy_s(strParentPath, MAX_PATH, strTempPath.substr(0, pos + 1).c_str());
    }

    return strParentPath;
}

std::string CurrentApplicationDirA()
{
    char szModulePath[ MAX_PATH ];
    ::GetModuleFileNameA(NULL, szModulePath, MAX_PATH);
    std::string path = szModulePath;
    return PathParentA(path);
}

std::wstring CurrentApplicationDirW()
{
    wchar_t szModulePath[ MAX_PATH ];
    ::GetModuleFileNameW(NULL, szModulePath, MAX_PATH);
    std::wstring path = szModulePath;
    return PathParentW(path);
}

void PathJoinA(std::string& dst, const std::string& base, const std::string& name)
{
    dst = base;
    size_t len = strlen(dst.c_str());
    if (dst[ len - 1 ] != '/' && dst[ len - 1 ] != '\\')
    {
        dst += "/";
    }
    dst += name;
}

void PathJoinW(std::wstring& dst, const std::wstring& base, const std::wstring& name)
{
    dst = base;
    size_t len = wcslen(dst.c_str());
    if (dst[ len - 1 ] != L'/' && dst[ len - 1 ] != L'\\')
    {
        dst += L"/";
    }
    dst += name;
}

