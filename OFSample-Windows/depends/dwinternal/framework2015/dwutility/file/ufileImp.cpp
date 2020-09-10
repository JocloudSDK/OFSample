#include "stable.h"
#include <string>
#include <shlwapi.h>
#pragma warning( push )
#pragma warning( disable : 4091 )
#include <ShlObj.h>
#include <Shellapi.h>
#include <tchar.h>
#include <atlstr.h>
#include <time.h>
#include "ufileImp.h"
#include "ufile.h"
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif//_countof

std::wstring uFileImp::GetFileNameFromPath(LPCTSTR filepath)
{
	size_t orgLen = _tcslen(filepath);

	TCHAR end = filepath[ orgLen - 1];

	if(end !=_T('\\') && end != _T(':'))
	{
		LPCTSTR FN = PathFindFileName(filepath);

		return std::wstring(FN);
	} 
	else
	{
		return std::wstring();
	}
}

std::wstring uFileImp::GetFileDirFromPath(LPCTSTR filepath)
{
	size_t orgLen = _tcslen(filepath);

	TCHAR end = filepath[ orgLen - 1];

	if(end !=_T('\\') && end != _T(':'))
	{
		LPCTSTR FN = PathFindFileName(filepath);

		return std::wstring(filepath, orgLen -  _tcslen(FN));
	} 
	else
	{
		return std::wstring(filepath);
	}
}

bool uFileImp::FileExist( const std::wstring& filename )
{
	if ( PathFileExists( filename.c_str() ) ) 
	{
		return true;
	}
	return false;
}

std::wstring uFileImp::GetModulePathDir()
{
	TCHAR	buffer[MAX_PATH];
	ZeroMemory(buffer, sizeof(TCHAR) * MAX_PATH);
	::GetModuleFileName(NULL, buffer, MAX_PATH);

	return GetFileDirFromPath(buffer);
}

bool uFileImp::CheckDirectoryExist(LPCWSTR strFileName)
{
	DWORD dwAttri = ::GetFileAttributes(strFileName);
	if ((dwAttri == INVALID_FILE_ATTRIBUTES) || !(dwAttri & FILE_ATTRIBUTE_DIRECTORY))
	{
		return false;
	}
	return true;
}

bool uFileImp::CreateAllDirectory(LPCWSTR strPath)
{	
	return ERROR_SUCCESS == SHCreateDirectory( NULL, strPath );
}

//强制删除文件或是删除一个空目录
bool uFileImp::DeleteFileOrDirectory(LPCTSTR filepath)
{
	int nRet = 0;

	if(FileExist(filepath))
	{
		TCHAR szBuffFrom[MAX_PATH +2] ={0};
		_tcsncpy(szBuffFrom, filepath,sizeof(szBuffFrom)/sizeof(szBuffFrom[0]) -2*sizeof(szBuffFrom[0]));

		SHFILEOPSTRUCT sfDelete;
		memset(&sfDelete, 0, sizeof(sfDelete));
		sfDelete.hwnd = NULL;
		sfDelete.wFunc = FO_DELETE;
		sfDelete.pFrom = szBuffFrom;
		sfDelete.pTo   = szBuffFrom;
		sfDelete.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
		nRet = SHFileOperation(&sfDelete);
		if(0 != nRet)
		{
			const TCHAR C_NAME_REFIX[] = {_T("{6B00FE91-95C1-470e-870C-509748F5F3D4}")};

			std::wstring strFile(filepath);
			strFile += C_NAME_REFIX;

			srand((unsigned int)time(NULL));

			for (int i = 0; i < 10 && 0 != nRet; ++i)
			{//删除操作失败,可能是被占用等,尝试去重命名文件或者目录.
				TCHAR bufRand[128] ={0};
				wnsprintf(bufRand, _countof(bufRand) -1,_T("%ld"), rand());
				strFile += bufRand;

				TCHAR szBuffTo[MAX_PATH +2] ={0};
				_tcsncpy(szBuffTo,  strFile.c_str(),_countof(szBuffTo) -1);

				SHFILEOPSTRUCT sfDelete;
				memset(&sfDelete, 0, sizeof(sfDelete));
				sfDelete.hwnd = NULL;
				sfDelete.wFunc = FO_RENAME;
				sfDelete.pFrom = szBuffFrom;
				sfDelete.pTo   = szBuffTo;
				sfDelete.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
				nRet = SHFileOperation(&sfDelete);
			}	
		}

		if(0 != nRet)
		{
			DWORD dwAttrs  = GetFileAttributes(filepath); 
			dwAttrs &= (~FILE_ATTRIBUTE_READONLY);
			dwAttrs &= (~FILE_ATTRIBUTE_HIDDEN);
			SetFileAttributes(filepath, dwAttrs);

			if(!::DeleteFile(filepath))
			{
				if(::RemoveDirectory(filepath))
				{
					nRet = 0;
				}
			}
			else
			{
				nRet = 0;
			}
		}
	}

	return 0 == nRet;
}

#pragma warning( pop )