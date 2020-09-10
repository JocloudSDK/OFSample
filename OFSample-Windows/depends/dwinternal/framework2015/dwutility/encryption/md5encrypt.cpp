#include "stable.h"
#include "md5encrypt.h"
#include "encryption/md5.h"

namespace DwUtility
{
	namespace encryption
	{
		bool MD5CryptA(const char *inbuf, size_t buflen, std::string& val)
		{
			if(NULL == inbuf || buflen == 0)
				return false;

			MD5 data;
			data.update((BYTE *)(inbuf), buflen);
			data.finalize();
			char hex[33];
			data.hex_digest (hex);

			val.assign(hex, 32);

			return true;
		}

		bool MD5CryptW(const char *inbuf, size_t buflen, std::wstring& val)
		{
			if(NULL == inbuf || buflen == 0)
				return false;

			MD5 data;
			data.update((BYTE *)(inbuf), buflen);
			data.finalize();
			char hex[33];
			data.hex_digest(hex);

			wchar_t whex[33];
			MultiByteToWideChar(CP_ACP, 0, hex, -1, whex, _countof(whex)-1);

			val.assign(whex, 32);

			return true;
		}

		int hex_ttol(LPCTSTR str)
		{
			LPCTSTR p;
			int	res;

			if (str==0)
				return 0;

			p=str;
			res=0;

			for (;*p!=0;p++)
			{
				if (*p>=TEXT('0') && *p<=TEXT('9'))
				{
					res=res*0x10+*p-TEXT('0');
				}
				else if (*p>=TEXT('a') && *p<=TEXT('z'))
				{
					res=res*0x10+*p-TEXT('a')+0xa;
				}
				else if (*p>=TEXT('A') && *p<=TEXT('Z'))
				{
					res=res*0x10+*p-TEXT('A')+0xa;
				}
				else
				{
					return res;
				}
			}
			return res;
		}

		void MD5FromStrW(PBYTE pbyHash, LPCWSTR hashString)
		{
			WCHAR tszSingleByte[3] = {0};
			DWORD i = 0;

			for (i = 0; i < 16; i++)
			{
				wcsncpy(tszSingleByte, hashString + i*2, 2);
				pbyHash[i] = hex_ttol(tszSingleByte);
			}
		}

		void MD5ToStrA(const BYTE* pbyMd5, std::string& val)
		{
			char md5str[33] = {0};
			if (pbyMd5)
			{
				_snprintf_s(
					md5str,
					33,
					32,
					"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", //lowercase, no space
					pbyMd5[0], pbyMd5[1], pbyMd5[2], pbyMd5[3],
					pbyMd5[4], pbyMd5[5], pbyMd5[6], pbyMd5[7], 
					pbyMd5[8], pbyMd5[9], pbyMd5[10], pbyMd5[11],
					pbyMd5[12], pbyMd5[13], pbyMd5[14],pbyMd5[15]
				);
			}
			val = md5str;
		}

		void MD5ToStrW(const BYTE* pbyMd5, std::wstring& val)
		{
			WCHAR md5str[33] = {0};
			if (pbyMd5)
			{
				_snwprintf_s(
					md5str,
					33,
					32,
					L"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", //lowercase, no space
					pbyMd5[0], pbyMd5[1], pbyMd5[2], pbyMd5[3],
					pbyMd5[4], pbyMd5[5], pbyMd5[6], pbyMd5[7], 
					pbyMd5[8], pbyMd5[9], pbyMd5[10], pbyMd5[11],
					pbyMd5[12], pbyMd5[13], pbyMd5[14],pbyMd5[15]
				);
			}

			val = md5str;
		}

		bool GetFileMD5A(LPCWSTR filename, std::string& val)
		{
			PERFLOG;
			HANDLE hFile = CreateFileW(filename,
				GENERIC_READ,
				FILE_SHARE_READ, 
				NULL,
				OPEN_EXISTING, 
				FILE_FLAG_SEQUENTIAL_SCAN, 
				NULL);
			if(INVALID_HANDLE_VALUE == hFile)
			{
				return false;
			}

			LARGE_INTEGER fileSize = {0};
			if(!GetFileSizeEx(hFile, &fileSize))
			{
				CloseHandle(hFile);
				return false;
			}

			HANDLE fileMapping = CreateFileMappingW(hFile,
				NULL,
				PAGE_READONLY, 
				fileSize.HighPart, 
				fileSize.LowPart,
				NULL);
			if(NULL == fileMapping)
			{
				CloseHandle(hFile);
				return false;
			}

			const unsigned int blockSize = 65536;
			unsigned int curBlockSize = 0;
			LARGE_INTEGER fileOffset = {0};

			MD5 data;

			bool br = true;

			while(fileOffset.QuadPart < fileSize.QuadPart)
			{
				if(fileSize.QuadPart - fileOffset.QuadPart > blockSize)
				{
					curBlockSize = blockSize;
				}
				else
				{
					curBlockSize = static_cast<unsigned int>(fileSize.QuadPart - fileOffset.QuadPart);
				}

			
				PBYTE buf = (PBYTE)MapViewOfFile(fileMapping,
					FILE_MAP_READ,
					fileOffset.HighPart, 
					fileOffset.LowPart,
					curBlockSize);

				if(buf)
				{
					data.update((BYTE *)(buf), curBlockSize);
					UnmapViewOfFile(buf);
				}
				else
				{
					br = false;
					break;
				}

				fileOffset.QuadPart += curBlockSize;
			}

			CloseHandle(fileMapping);
			CloseHandle(hFile);

			data.finalize();
			char hex[33] = {0};
			data.hex_digest(hex);

			val = hex;

			return br;
		}

		bool GetFileMD5W(LPCWSTR filename, std::wstring& val)
		{
			PERFLOG;
			std::string a;
			if(!GetFileMD5A(filename,a))
				return false;

			wchar_t whex[33];
			MultiByteToWideChar(CP_ACP, 0, a.c_str(), a.length(), whex, _countof(whex)-1);

			val.assign(whex, 32);

			return true;
		}
	}
}
