#pragma once

#include <string>
/* 添加新接口时，请务必保证原来的函数的先后顺序，即新加的接口请务必加在最后*/

namespace DwUtility
{
    class DW_DWUTILITY_EXPORT ufile
    {
    public:
        static bool ReadFileData(const std::wstring &fileName, std::string &data);

        static bool	WriteFileData(const std::wstring &fileName, const std::string &data);

        static std::wstring GetModulePathDir();

        static std::wstring GetFileNameFromPath(LPCTSTR filepath);

        static std::wstring GetFileDirFromPath(LPCTSTR strFilePath);

        static std::string GetFileMD5(const std::wstring& filename);

        static bool FileExist( const std::wstring& filename );

        static std::wstring SizeToString(const ULONGLONG nSize);

        static bool CheckDirectoryExist(LPCWSTR strFileName);
        static bool CreateAllDirectory(LPCWSTR strPath);

        //强制删除文件或是删除一个空目录
        static bool DeleteFileOrDirectory(LPCTSTR filepath);

        static void GetImageLimitSize(ULONG& lFileSize, UINT& nHeight, UINT& nWidth);

        static std::wstring GetYYTempFile(); // 在系统临时目录获取一个临时文件名

        // 获取“size-MD5”形式的文件名
        static std::wstring GetMD5SizeFileName(const std::wstring& filepath);
        static bool GetFileSize(const std::wstring& strFilePath, ULONGLONG& nFileSize);

		//目录分割符号统一规整
		//规则文件名，统一使用小写；统一使用\ 目录分割；第一个字符不能使用目录分割
		static std::wstring FormatPathSpan(LPCTSTR lpszPath);

		// 获取指定路径下，磁盘空间的大小.
		static bool getFreeSpace(std::wstring& strPath, ULONGLONG& nFreeSpaceSize);
    };
}
