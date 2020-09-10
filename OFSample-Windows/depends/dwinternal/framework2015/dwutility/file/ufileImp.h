#pragma once

#pragma comment( lib, "shlwapi" )

class uFileImp
{
public:
    static std::wstring GetModulePathDir();
    static std::wstring GetFileNameFromPath(LPCTSTR filepath);
    static std::wstring GetFileDirFromPath(LPCTSTR strFilePath);
    static std::string GetFileMD5(const std::wstring& filename);
    static bool FileExist( const std::wstring& filename );
    static bool DeleteFileOrDirectory(LPCTSTR filepath);//ǿ��ɾ���ļ�����ɾ��һ����Ŀ¼
    static bool CheckDirectoryExist(LPCWSTR strFileName);
    static bool CreateAllDirectory(LPCWSTR strPath);
};
