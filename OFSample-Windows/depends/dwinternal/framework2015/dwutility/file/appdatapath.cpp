/*
%appdata%目录整理20121121

*/
#pragma warning( push )
#pragma warning( disable : 4091 )
#include <ShlObj.h>
#include <atlstr.h>
#include "encryption/md5encrypt.h"
#include "text/ustring.h"

#include "appdatapath.h"

namespace DwUtility
{
	const WCHAR * KAppdataSubDirectory			= L"duowan\\";
	const WCHAR * KBusinessDirectory			= L"business\\";
	const WCHAR * KCacheDirectory				= L"cache\\";
	const WCHAR * KDownloadDirectory			= L"cache\\download\\";
	const WCHAR * KTempDirectory				= L"cache\\temp\\";
	const WCHAR * KChannelDirectory				= L"channel\\";
	const WCHAR * KImgroupDirectory				= L"imgroup\\";
	const WCHAR * KLogDirectory					= L"log\\";
	const WCHAR * KMainframeDirectory			= L"mainframe\\";
	const WCHAR * KOpenplatformDirectory		= L"openplatform\\";
	const WCHAR * KAddonsexDirectory			= L"openplatform\\addonsex\\";
	const WCHAR * KSetupDirectory				= L"setup\\";
	const WCHAR * KUpdateDirectory				= L"update\\";
	const WCHAR * KYYcomstoreDirectory			= L"yycomstore\\";

	std::wstring g_applicationName;

	std::wstring AppDataPath::GetBusinessPath()
	{
		return _getSubPath(KBusinessDirectory);
	}

	std::wstring AppDataPath::GetMainframePath()
	{
		return _getSubPath(KMainframeDirectory);
	}

	std::wstring AppDataPath::GetImGroupPath()
	{
		return _getSubPath(KImgroupDirectory);
	}

	std::wstring AppDataPath::GetImGroupUserPath(unsigned long uid)
	{
		if (uid)
		{
			WCHAR userDir[20] = { 0 };
			_snwprintf_s(userDir, 20, L"%lu", uid);

			return _getSubPath(KImgroupDirectory, userDir);
		}

		return std::wstring();
	}

	std::wstring AppDataPath::GetChannelPath()
	{
		return _getSubPath(KChannelDirectory);
	}

	std::wstring AppDataPath::GetChannelUserPath(unsigned long uid)
	{
		if (uid)
		{
			WCHAR userDir[20] = { 0 };
			_snwprintf_s(userDir, 20, L"%lu", uid);

			return _getSubPath(KChannelDirectory, userDir);
		}

		return std::wstring();
	}

	std::wstring AppDataPath::GetYYComstorePath()
	{
		return _getSubPath(KYYcomstoreDirectory);
	}

	std::wstring AppDataPath::GetOpenplatformPath()
	{
		return _getSubPath(KOpenplatformDirectory);
	}

	std::wstring AppDataPath::GetOpenplatformAddonsexPath()
	{
		return _getSubPath(KAddonsexDirectory);
	}

	std::wstring AppDataPath::GetCachePath()
	{
		return _getSubPath(KCacheDirectory);
	}

	std::wstring AppDataPath::GetTempPath()
	{
		return _getSubPath(KTempDirectory);
	}

	std::wstring AppDataPath::GetDownloadPath()
	{
		return _getSubPath(KDownloadDirectory);
	}

	std::wstring AppDataPath::GetLogPath()
	{
		return _getSubPath(KLogDirectory);
	}

	std::wstring AppDataPath::GetUpdatePath()
	{
		return _getSubPath(KUpdateDirectory);
	}

	std::wstring AppDataPath::GetSetupPath()
	{
		return _getSubPath(KSetupDirectory);
	}

	std::wstring AppDataPath::GetSetupMd5Path(LPCWSTR appRootPath)
	{
		return _getSubPath(KSetupDirectory, _transformInstallPath2MD5(appRootPath).c_str());
	}

#define LibC_PTHREAD_ONCE_INIT 0
	static volatile long ponce_control = LibC_PTHREAD_ONCE_INIT;
	static std::wstring appdataName;
	const std::wstring& AppDataPath::GetUserAppDataRoot()
	{
		if (InterlockedCompareExchange(&ponce_control, LibC_PTHREAD_ONCE_INIT + 1, LibC_PTHREAD_ONCE_INIT) == LibC_PTHREAD_ONCE_INIT)
		{
			TCHAR buffer[MAX_PATH] = { 0 };

			SHGetSpecialFolderPath(NULL, buffer, CSIDL_APPDATA, NULL);

			std::wstring strPath = std::wstring(buffer) + L"\\" + KAppdataSubDirectory + applicationName() + L"\\";
			if (!IsDirExist(strPath.c_str()))
			{
				CreateDirectory(strPath.c_str());
			}

			appdataName = strPath;
			ponce_control = LibC_PTHREAD_ONCE_INIT + 2;
		}
		else
		{
			while (ponce_control != LibC_PTHREAD_ONCE_INIT + 2)
				Sleep(0);
		}
		return appdataName;
	}

	bool AppDataPath::IsFileExist(LPCWSTR lpszFile)
	{
		return _isFileExist(lpszFile);
	}

	bool AppDataPath::IsDirExist(LPCWSTR lpszDir)
	{
		return _isDirExist(lpszDir);
	}

	std::wstring AppDataPath::TransformInstallPath2MD5(LPCWSTR lpszPath)
	{
		return _transformInstallPath2MD5(lpszPath);
	}

	bool AppDataPath::CreateDirectory(LPCWSTR lpszDir)
	{
		return _createDirectory(lpszDir);
	}

	// ------------------------------------------------
	// 以下是私有方法

	bool AppDataPath::_isFileExist(LPCWSTR lpszFile)
	{
		DWORD dwAttrib = ::GetFileAttributesW(lpszFile);
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool AppDataPath::_isDirExist(LPCWSTR lpszDir)
	{
		DWORD dwAttrib = ::GetFileAttributesW(lpszDir);
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool AppDataPath::_createDirectory(LPCWSTR lpszDir)
	{
		return (ERROR_SUCCESS == ::SHCreateDirectory(NULL, lpszDir));
	}

	bool AppDataPath::_toWindowPath(std::wstring& output, LPCWSTR lpszFile)
	{
		if (!lpszFile || !(*lpszFile))
		{
			return false;
		}
		output = lpszFile;

		for (size_t i = 0; i < output.size(); ++i)
		{
			if (L'/' == output[i])
			{
				output[i] = L'\\';
			}
		}
		return true;
	}

	std::wstring AppDataPath::_getSubPath(LPCWSTR strSubDir, LPCWSTR strChildDir)
	{
		if (!strSubDir || '\0' == *strSubDir)
		{
			return L"";
		}

		// 一级目录
		std::wstring strDir = GetUserAppDataRoot();
		strDir.append(strSubDir);

		if (strDir.empty() || (!_isDirExist(strDir.c_str()) && !_createDirectory(strDir.c_str())))
		{
			return L"";
		}

		// 还有下一级子目录
		if (strChildDir && '\0' != strChildDir[0])
		{
			WCHAR wchEnd = strDir[strDir.size() - 1];
			if (wchEnd != '\\' && wchEnd != '/')
			{
				strDir += L"\\";
			}
			strDir += strChildDir;
			strDir += L"\\";
			if (strDir.empty() || !_isDirExist(strDir.c_str()) && !_createDirectory(strDir.c_str()))
			{
				return L"";
			}
		}

		return strDir;
	}

	//目录分割符号统一规整
	//规则文件名，统一使用小写；统一使用\ 目录分割；第一个字符不能使用目录分割
	std::wstring AppDataPath::_formatPathSpan(LPCTSTR lpszPath)
	{
		CAtlString strPath(lpszPath);
		strPath.MakeLower();
		while(strPath.Replace( _T("/"), _T("\\")) > 0)
		{
			;
		}
		while(strPath.Replace( _T("\\\\"), _T("\\")) > 0)
		{
			;
		}

		while(_T("\\") == strPath.Left(1))
		{
			strPath.Delete(0);
		}
		while(_T("\\") == strPath.Right(1))
		{
			strPath.Delete(strPath.GetLength()-1);
		}

		std::wstring strPathNew(strPath);
		return strPathNew;
	}
	
	std::wstring AppDataPath::_transformInstallPath2MD5(const std::wstring &strPath)
	{
		std::wstring strDirNew	= _formatPathSpan(strPath.c_str());
		std::string strDirAssic = text::toNarrowString(strDirNew.c_str(), (int)strDirNew.size(),CP_ACP);

		std::string strMD5;
		encryption::MD5CryptA(strDirAssic.c_str(), (int)strDirAssic.size(), strMD5);

		std::wstring strDirMD5 = text::toWideString(strMD5.c_str(), (int)strMD5.size(), CP_ACP);
		return strDirMD5;
	}

	std::wstring AppDataPath::applicationName()
	{
		if (g_applicationName.empty())
		{
			setApplicationName(L"yy");
		}
		return g_applicationName;
	}

	void AppDataPath::setApplicationName(const std::wstring &name)
	{
		g_applicationName = name;
	}

}

#pragma warning( pop )