#include "gzip.h"
#include "../unzip.h"
#include <stdlib.h>
#include <cassert>
#include "zlib.h"
#include "zip.h"
#include "text/ustring.h"

DwGzip::DwGzip(size_t buffLen)
:m_buffLen(buffLen),
m_buff(NULL)
{
    m_buff = (unsigned char*)malloc(m_buffLen);
}

DwGzip::~DwGzip()
{
    if (m_buff)
    {
        free(m_buff);
    }
}

bool DwGzip::ungzip(unsigned char* gzdata, size_t len, std::string& out)
{
	PERFLOG;
    int err;
    unsigned long out_count = 0;
    z_stream d_stream = {0}; /* decompression stream */

    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in = gzdata;
    d_stream.avail_in = len;
    d_stream.avail_out = m_buffLen;
    d_stream.next_out = m_buff;

    if(inflateInit2(&d_stream, 47) != Z_OK)
	{
		return false;
	}

    out.clear();

    while (d_stream.total_in < len)
    {
        if((err = inflate(&d_stream, Z_SYNC_FLUSH)) == Z_STREAM_END)
        {
            out.append((const char*)m_buff, d_stream.total_out - out_count);
            err = inflateEnd(&d_stream);
            break;
        }

        if(err == Z_OK)
        {
            out.append((const char*)m_buff, d_stream.total_out - out_count);
            out_count = d_stream.total_out;
            d_stream.avail_out = m_buffLen;
            d_stream.next_out = m_buff;
        }
        else
        {
            goto unzip_exit;
        }
    }

unzip_exit:
	assert(err == Z_OK);
    return err == Z_OK;
}

bool DwGzip::gzipex(unsigned char* gzdata, size_t len, std::string& out)
{
	PERFLOG;
    int err = 0;
    z_stream c_stream = { 0 }; /* compression stream */

    int destlen = len + (len >> 12) + (len >> 14) + (len >> 25) + 13;
    char* buf = NULL;

    if (!gzdata || len <= 0)
        return false;
    out.clear();
    buf = new char[destlen];
    if (!buf)
        return false;

    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    c_stream.next_in = gzdata;
    c_stream.avail_in = len;
    c_stream.avail_out = destlen;
    c_stream.next_out = (Bytef*)buf;

    err = deflateInit(&c_stream, Z_BEST_COMPRESSION);
    if (err != Z_OK)
    {
        goto unzip_exit;
    }

    err = deflate(&c_stream, Z_FINISH);
    if (err != Z_STREAM_END)
    {
        deflateEnd(&c_stream);
        if (Z_OK == err)
            err = Z_BUF_ERROR;
        goto unzip_exit;
    }

    err = deflateEnd(&c_stream);
    if (err != Z_OK)
        goto unzip_exit;

    out.append(buf, c_stream.total_out);
    out.append((const char*)&len, sizeof(len));

unzip_exit:

    if (buf)
        delete []buf;
    buf = NULL;

    return err == Z_OK;
}

bool DwGzip::ungzipex(unsigned char* gzdata, size_t len, std::string& out)
{
	PERFLOG;
    z_stream stream = { 0 };
    int err = Z_OK;
    int destlen = 0;
    char* buf = NULL;

    if (!gzdata ||len <= sizeof(stream.total_out))
        return false;
    out.clear();
    ::memcpy(&destlen, (void*)&gzdata[len - sizeof(stream.total_out)], sizeof(stream.total_out));
    if (destlen <= 0 || destlen > 0x0fffffff) // 255 MB
        return false;
    buf = new char[destlen];
    if (!buf)
        return false;

    stream.next_in = (Bytef*)gzdata;
    stream.avail_in = (uInt)len - sizeof(stream.total_out);
    stream.next_out = (Bytef*)buf;
    stream.avail_out = (uInt)destlen;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit(&stream);
    if (err != Z_OK)
        goto exit;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END)
    {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            err = Z_DATA_ERROR;
        goto exit;
    }

    err = inflateEnd(&stream);
    if (Z_OK != err)
        goto exit;

    out.append(buf, stream.total_out);

exit:

    if (buf)
        delete []buf;
    buf = NULL;

    return err == Z_OK;
}

namespace DwUtility
{
	namespace zip
	{
		bool UnZipGzip(const char* gzdata, size_t len, std::string& out)
		{
			DwGzip gzip;
			return gzip.ungzip((unsigned char*)gzdata, len, out);
		}

		bool ZipGzipEx(const char* gzdata, size_t len, std::string& out)
		{
			DwGzip gzip;
			return gzip.gzipex((unsigned char*)gzdata, len, out);
		}

		bool UnZipGzipEx(const char* gzdata, size_t len, std::string& out)
		{
			DwGzip gzip;
			return gzip.ungzipex((unsigned char*)gzdata, len, out);
		}

		bool ZipFiles(const std::vector<std::wstring>& vecFilePath, const std::wstring& outFile)
		{
			std::string astrOutPath = DwUtility::text::toNarrowString(outFile.c_str(), outFile.length());

			zipFile file = zipOpen(astrOutPath.c_str(), 0);
			assert(file);
			if (!file)
			{
				return false;
			}

			bool result = true;
			for (UINT i = 0; i < vecFilePath.size(); i++)
			{
				HANDLE hFile = ::CreateFile(vecFilePath[i].c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					result = false;
					break;
				}
				else
				{
					FILETIME filetime = {0};
					::GetFileTime(hFile, NULL, &filetime, NULL);
					SYSTEMTIME systime = {0};
					::FileTimeToSystemTime(&filetime, &systime);

					zip_fileinfo zi;
					zi.tmz_date.tm_sec = systime.wSecond;
					zi.tmz_date.tm_min = systime.wMinute;
					zi.tmz_date.tm_hour = systime.wHour;
					zi.tmz_date.tm_mday = systime.wDay;
					zi.tmz_date.tm_mon = systime.wMonth;
					zi.tmz_date.tm_year = systime.wYear;
					zi.dosDate = 0;
					zi.internal_fa = 0;
					zi.external_fa = 0;

					const WCHAR *pName = wcsrchr(vecFilePath[i].c_str(), L'\\');
					if (pName)
						pName++;
					else
						pName = vecFilePath[i].c_str();

					std::string astrFileName = DwUtility::text::toNarrowString(pName, -1);	

					int err = zipOpenNewFileInZip(file, astrFileName.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
					assert(err == ZIP_OK);
					if (err != ZIP_OK)
					{
						result = false;
						::CloseHandle(hFile);
						break;
					}
					else
					{	
						DWORD dwSize = ::GetFileSize(hFile, NULL);
						void *pBuf = new BYTE[dwSize];
						assert(pBuf != NULL);
						if (pBuf == NULL)
						{
							result = false;
							::CloseHandle(hFile);
							break;
						}

						DWORD dwReadBytes = 0;
						::ReadFile(hFile, pBuf, dwSize, &dwReadBytes, NULL);

						err = zipWriteInFileInZip(file, pBuf, dwReadBytes);
						assert(err == ZIP_OK);

						delete [] pBuf;
						pBuf = NULL;
					}

					::CloseHandle(hFile);
				}
			}

			int err = zipClose(file, NULL);
			assert(err == ZIP_OK);
			UNREFERENCED_PARAMETER(err);

			return result;
		}
	}
}
