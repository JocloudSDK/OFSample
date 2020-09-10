#pragma  once
#include "dwglobal.h"
#include <string>

namespace DwUtility
{
    namespace encryption
    {
        bool DW_DWUTILITY_EXPORT MD5CryptA(const char *inbuf, size_t buflen, std::string& val);
        bool DW_DWUTILITY_EXPORT MD5CryptW(const char *inbuf, size_t buflen, std::wstring& val);
        void DW_DWUTILITY_EXPORT MD5ToStrA(const BYTE* pbyMd5, std::string& val);
        void DW_DWUTILITY_EXPORT MD5ToStrW(const BYTE* pbyMd5, std::wstring& val);
        void DW_DWUTILITY_EXPORT MD5FromStrW(PBYTE pbyHash, const LPCWSTR hashString);
        bool DW_DWUTILITY_EXPORT GetFileMD5A(LPCWSTR filename, std::string& val);
        bool DW_DWUTILITY_EXPORT GetFileMD5W(LPCWSTR filename, std::wstring& val);
    }
}
