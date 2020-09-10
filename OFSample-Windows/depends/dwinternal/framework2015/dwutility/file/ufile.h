#pragma once

#include <string>
/* ����½ӿ�ʱ������ر�֤ԭ���ĺ������Ⱥ�˳�򣬼��¼ӵĽӿ�����ؼ������*/

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

        //ǿ��ɾ���ļ�����ɾ��һ����Ŀ¼
        static bool DeleteFileOrDirectory(LPCTSTR filepath);

        static void GetImageLimitSize(ULONG& lFileSize, UINT& nHeight, UINT& nWidth);

        static std::wstring GetYYTempFile(); // ��ϵͳ��ʱĿ¼��ȡһ����ʱ�ļ���

        // ��ȡ��size-MD5����ʽ���ļ���
        static std::wstring GetMD5SizeFileName(const std::wstring& filepath);
        static bool GetFileSize(const std::wstring& strFilePath, ULONGLONG& nFileSize);

		//Ŀ¼�ָ����ͳһ����
		//�����ļ�����ͳһʹ��Сд��ͳһʹ��\ Ŀ¼�ָ��һ���ַ�����ʹ��Ŀ¼�ָ�
		static std::wstring FormatPathSpan(LPCTSTR lpszPath);

		// ��ȡָ��·���£����̿ռ�Ĵ�С.
		static bool getFreeSpace(std::wstring& strPath, ULONGLONG& nFreeSpaceSize);
    };
}
