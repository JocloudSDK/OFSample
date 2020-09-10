#include "stable.h"

#include <sstream>
#include <iomanip>
#pragma warning( push )
#pragma warning( disable : 4091 )
#include <shlobj.h>
#include <atlstr.h>
#include <string>

#include "ufile.h"
//#include "mfile.h"
#include "ufileImp.h"

#include "encryption/md5encrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMAGE_FILE_SIZE_LIMIT_SIZE  1048576  // 1M
#define IMAGE_FILE_SIZE_LIMIT_HEIGHT  4000
#define IMAGE_FILE_SIZE_LIMIT_WIDTH   4000

namespace DwUtility
{
	std::wstring ufile::GetFileNameFromPath(LPCTSTR filepath)
	{
		return uFileImp::GetFileNameFromPath(filepath);
	}

	std::wstring ufile::GetFileDirFromPath(LPCTSTR filepath)
	{
		return uFileImp::GetFileDirFromPath(filepath);
	}

	bool ufile::FileExist( const std::wstring& filename )
	{
		return uFileImp::FileExist(filename);
	}

	std::wstring ufile::GetModulePathDir()
	{
		return uFileImp::GetModulePathDir();
	}

	bool ufile::ReadFileData(const std::wstring &fileName, std::string &data)
	{
		ULONGLONG fileSize = 0;
		if(GetFileSize(fileName, fileSize) == false)
		{
			return false;
		}

		//the file size must be less than 16 MB
		DW_ASSERT_X(fileSize <= 16 * 1024 * 1024, __FUNCTION__, "Can't read more 16MB file data in one time.");

		FILE *fp = _wfopen(fileName.c_str(), L"rb");
		if(fp)
		{
			data.resize((size_t)fileSize);
			size_t count = fread((void*)data.data(), 1, data.size(), fp);
			fclose(fp);

			if(count == data.size())
			{
				return true;
			}
		}

		data.clear();
		return false;
	}

	bool ufile::WriteFileData(const std::wstring &fileName, const std::string &data)
	{
		DW_ASSERT_X(data.size() <= 16 * 1024 * 1024, __FUNCTION__, "Can't write more 16MB file data in one time.");

		FILE *fp = _wfopen(fileName.c_str(), L"wb+");
		if(fp)
		{
			size_t count = fwrite(data.data(), 1, data.size(), fp);
			fclose(fp);

			if(count == data.size())
			{
				return true;
			}
		}

		return false;
	}

	std::string ufile::GetFileMD5(const std::wstring& filepath)
	{
		std::string filecontent;

		ReadFileData(filepath, filecontent);

		std::string md5;

		DwUtility::encryption::MD5CryptA(filecontent.c_str(), filecontent.length(), md5);
		
		return md5;
	}

	// 获取“size-MD5”形式的文件名，注意，此文件大小不能超过512M
	std::wstring ufile::GetMD5SizeFileName(const std::wstring& filepath)
	{
		CAtlString strName;
		CAtlStringA strMd5 = GetFileMD5(filepath).c_str();

		ULONGLONG lSize = 0;
		GetFileSize(filepath, lSize);

		USES_CONVERSION;
		strName.Format(L"%u-%s", (ULONG)lSize, A2W(strMd5));

		std::wstring strResult = strName;
		return strResult;
	}

	bool ufile::GetFileSize(const std::wstring& strFilePath, ULONGLONG& nFileSize)
	{
		HANDLE hFileHandle = CreateFile(strFilePath.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

		if( INVALID_HANDLE_VALUE == hFileHandle )
		{
			return false;
		}

		LARGE_INTEGER filesize;

		if( GetFileSizeEx( hFileHandle,&filesize ) )
		{
			nFileSize = filesize.QuadPart;
			::CloseHandle(hFileHandle);
			return true;
		}
		::CloseHandle(hFileHandle);
		return false;
	}

	std::wstring ufile::SizeToString(const ULONGLONG nSize)
	{
		std::wostringstream oss;

		const UINT K = 1024;
		const UINT M = K * 1024;
		const ULONG G = M * 1024;

		oss<<std::setiosflags(std::ios::fixed);
		oss<<std::setprecision(2);

		if ( nSize > G )
		{
			oss<<(double)nSize/G << _T("GB"); 
		}
		else if ( nSize > M )
		{
			oss<<(double)nSize/M <<_T("MB");
		}
		else
		{
			oss<<std::setprecision(0);
			oss<<(double)nSize/K<<_T("KB");
		}
		return oss.str();
	}

	bool ufile::CheckDirectoryExist(LPCWSTR strFileName)
	{
		return uFileImp::CheckDirectoryExist(strFileName);
	}

	bool ufile::CreateAllDirectory(LPCWSTR strPath)
	{	
		return uFileImp::CreateAllDirectory(strPath);
	}

	//强制删除文件或是删除一个空目录
	bool ufile::DeleteFileOrDirectory(LPCTSTR filepath)
	{
		return uFileImp::DeleteFileOrDirectory(filepath); 
	}

	// 获取对图片文件的限制（应用在自定义表情、截图、发送图片的场景）
	void ufile::GetImageLimitSize(ULONG& lFileSize, UINT& nHeight, UINT& nWidth)
	{
		lFileSize = IMAGE_FILE_SIZE_LIMIT_SIZE;
		nHeight = IMAGE_FILE_SIZE_LIMIT_HEIGHT;
		nWidth = IMAGE_FILE_SIZE_LIMIT_WIDTH;
	}

	std::wstring ufile::GetYYTempFile() // 在系统临时目录获取一个临时文件名
	{
		std::wstring strName;

		WCHAR szTempName[MAX_PATH] = {0};
		WCHAR lpPathBuffer[4096] = {0};
		DWORD  dwBufSize = 4096;

		GetTempPath(dwBufSize, lpPathBuffer);
		if (GetTempFileName(lpPathBuffer, L"YY", 0, szTempName) == 0) // 出错
		{
			 _ASSERTE(false);
			 return strName;
		}
		
		strName = szTempName;

		return strName;
	}

	//目录分割符号统一规整
	//规则文件名，统一使用小写；统一使用\ 目录分割；第一个字符不能使用目录分割
	std::wstring ufile::FormatPathSpan(LPCTSTR lpszPath)
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

	bool ufile::getFreeSpace(std::wstring& strPath, ULONGLONG& nFreeSpaceSize)
	{
		BOOL bRet = FALSE;
		_ULARGE_INTEGER freeSpace = {0};
		if(strPath.length() > 2 && strPath.at(1) == ':')
		{
			bRet = GetDiskFreeSpaceEx(strPath.substr(0,2).c_str(), NULL, NULL, &freeSpace);
		}
		else
		{
			bRet = GetDiskFreeSpaceEx(strPath.c_str(), NULL, NULL, &freeSpace);
		}
		nFreeSpaceSize = freeSpace.QuadPart;
		return (bRet == TRUE);
	}
}
#pragma warning( pop )