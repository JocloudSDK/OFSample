/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <string>
#include <map>

#include "dwglobal.h"

namespace DwUtility
{
	namespace zip
	{
		//从文件解压所有到文件
		bool DW_DWUTILITY_EXPORT UnZip7zToDisk(const char *szSrc, const char *szDesFolder);

		//从内存解压所有到文件
		bool DW_DWUTILITY_EXPORT UnZip7zToDisk(const char* data, size_t len, const char* szDesFolder);

		//从文件中解压一个文件到内存
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char *szSrc, const char *zipFileName, std::string& data);

		//从内存中解压一个文件到内存
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char* data, size_t len, const char *zipFileName, std::string& outData);

		//从文件中解压所有到内存
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char *szSrc, std::map<std::wstring, std::string>& outData);

		//从内存中解压所有到内存
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char* data, size_t len, std::map<std::wstring, std::string>& outData);

		bool DW_DWUTILITY_EXPORT UnZipGzip(const char* gzdata, size_t len, std::string& out);

		bool DW_DWUTILITY_EXPORT ZipGzipEx(const char* gzdata, size_t len, std::string& out);

		// DON'T use it to compress LARGE file(s).
		bool DW_DWUTILITY_EXPORT ZipFiles(const std::vector<std::wstring>& vecFilePath, const std::wstring& outFile);

		bool DW_DWUTILITY_EXPORT UnZipGzipEx(const char* gzdata, size_t len, std::string& out);

		bool DW_DWUTILITY_EXPORT Check7zFile(const char* szSrc);
	}
}
