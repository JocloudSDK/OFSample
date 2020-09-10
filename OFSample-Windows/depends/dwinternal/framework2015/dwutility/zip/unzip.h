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
		//���ļ���ѹ���е��ļ�
		bool DW_DWUTILITY_EXPORT UnZip7zToDisk(const char *szSrc, const char *szDesFolder);

		//���ڴ��ѹ���е��ļ�
		bool DW_DWUTILITY_EXPORT UnZip7zToDisk(const char* data, size_t len, const char* szDesFolder);

		//���ļ��н�ѹһ���ļ����ڴ�
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char *szSrc, const char *zipFileName, std::string& data);

		//���ڴ��н�ѹһ���ļ����ڴ�
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char* data, size_t len, const char *zipFileName, std::string& outData);

		//���ļ��н�ѹ���е��ڴ�
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char *szSrc, std::map<std::wstring, std::string>& outData);

		//���ڴ��н�ѹ���е��ڴ�
		bool DW_DWUTILITY_EXPORT UnZip7zToMemory(const char* data, size_t len, std::map<std::wstring, std::string>& outData);

		bool DW_DWUTILITY_EXPORT UnZipGzip(const char* gzdata, size_t len, std::string& out);

		bool DW_DWUTILITY_EXPORT ZipGzipEx(const char* gzdata, size_t len, std::string& out);

		// DON'T use it to compress LARGE file(s).
		bool DW_DWUTILITY_EXPORT ZipFiles(const std::vector<std::wstring>& vecFilePath, const std::wstring& outFile);

		bool DW_DWUTILITY_EXPORT UnZipGzipEx(const char* gzdata, size_t len, std::string& out);

		bool DW_DWUTILITY_EXPORT Check7zFile(const char* szSrc);
	}
}
