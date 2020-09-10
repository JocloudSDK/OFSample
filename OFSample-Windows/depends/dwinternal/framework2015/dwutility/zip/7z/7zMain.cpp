#define USE_WINDOWS_FILE

#pragma warning( push )
#pragma warning( disable : 4091 )
#include <stdio.h>
#include <shlobj.h>
#include <cassert>

#include "7z.h"
#include "7zCrc.h"
#include "7zFile.h"
#include "7zVersion.h"

#include "7zAlloc.h"
#include "text/ustring.h"
#include "../unzip.h"

#ifdef _WIN32
#define CHAR_PATH_SEPARATOR '\\'
#else
#define CHAR_PATH_SEPARATOR '/'
#endif

namespace
{
	/*
	模拟内存文件访问
	*/
	struct CMemoryInStream: public CFileInStream
	{
		const char* start;
		size_t len;
		size_t pos;
	};

	SRes MemoryFileRead(void *pp, void *buf, size_t *size)
	{
		CMemoryInStream *p = (CMemoryInStream *)pp;
		const void* src = p->start + p->pos;
		
		assert(p->pos + *size <= p->len);
		
		if( (p->pos + *size) <= p->len )
		{
			memcpy(buf, src, *size);
			p->pos += *size;
			return SZ_OK;
		}
		else
		{
			return SZ_ERROR_ARCHIVE;
		}
	}

	SRes MemoryFileSeek(void *pp, Int64 *pos, ESzSeek origin)
	{
		CMemoryInStream *p = (CMemoryInStream*)pp;
		switch (origin)
		{
		case SZ_SEEK_SET:
			p->pos = static_cast<size_t>(*pos);
			assert(p->pos < p->len);
			break;
		case SZ_SEEK_CUR:
			p->pos += static_cast<size_t>(*pos);
			assert(p->pos < p->len);
			break;
		case SZ_SEEK_END:
			assert(*pos + p->len >= 0);
			p->pos = p->len - static_cast<size_t>(*pos);
			break;
		}

		*pos = p->pos;
		return SZ_OK;
	}

	void MemoryInStream_CreateVTable(CFileInStream *p)
	{
		p->s.Read = MemoryFileRead;
		p->s.Seek = MemoryFileSeek;
	}

	void convertPath(std::wstring& path)
	{
		for (size_t j = 0; j < path.size(); j++)
		{
			if (path[j] == '/')
			{
				path[j] = CHAR_PATH_SEPARATOR;
			}
		}
	}

	struct IOutputer
	{
		virtual bool isThisWillBeProcess(const std::wstring& fileName) const {(void)fileName; return true;};
		virtual bool output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size) = 0;
		virtual bool continueProcess() const {return true;};
	};

	/*
	负责disk文件的写入
	*/
	class CDiskOutputer :public IOutputer
	{
	public:
		CDiskOutputer(const std::wstring& folder):m_desFolder(folder) 
		{
			if (!m_desFolder.empty())
			{
				convertPath(m_desFolder);
				if (m_desFolder[m_desFolder.size() - 1] != CHAR_PATH_SEPARATOR)
				{
					m_desFolder += CHAR_PATH_SEPARATOR;
				}
			}
		};

	private:
		virtual bool output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size);

	private:
		std::wstring m_desFolder;
	};

	class CMemoryOutputer: public IOutputer
	{
	public:
		CMemoryOutputer(std::map<std::wstring, std::string>& data):m_memData(data) {}

	private:
		virtual bool output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size);

	private:
		std::map<std::wstring, std::string>& m_memData;
	};

	/*
	只解压7z中一个文件
	*/
	class CSingleFileMemoryOutputer :public IOutputer
	{
	public:
		CSingleFileMemoryOutputer(const std::wstring& fileName, std::string& outData):m_fileName(fileName),m_memData(outData) 
		{
			convertPath(m_fileName);
		};

	private:
		virtual bool isThisWillBeProcess(const std::wstring& fileName) const;
		virtual bool output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size);
		virtual bool continueProcess() const;

	private:
		std::wstring m_fileName;
		std::string& m_memData;
	};

	bool CDiskOutputer::output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size)
	{
		std::wstring name = fileName;
		CSzFile outFile;
		size_t processedSize;
		for (size_t j = 0; j < name.size(); j++)
		{
			if (name[j] == '/')
			{
				name[j] = 0;
				std::wstring fullPath = m_desFolder + name;
				CreateDirectoryW(fullPath.c_str(), NULL);
				name[j] = CHAR_PATH_SEPARATOR;
			}
		}

		std::wstring fullPath = m_desFolder + name;
		if (isDirectory)
		{
			CreateDirectoryW(fullPath.c_str(), NULL);
		}
		else if (OutFile_OpenW(&outFile, fullPath.c_str()) == SZ_OK)
		{
			processedSize = *size;
			if (File_Write(&outFile, data, &processedSize) != 0 || processedSize != *size)
			{
				File_Close(&outFile);
				return false;
			}
			File_Close(&outFile);
		}

		return true;
	}

	bool CMemoryOutputer::output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size)
	{
		std::wstring name = fileName;
		convertPath(name);

		if (!isDirectory)
		{
			std::string memFile;
			memFile.append((const char*)data, *size);
			m_memData.insert(std::make_pair(name, memFile));
		}

		return true;
	}

	bool CSingleFileMemoryOutputer::isThisWillBeProcess(const std::wstring& fileName) const
	{
		if (fileName.size() != m_fileName.size())
		{
			return false;
		}

		std::wstring name = fileName;
		convertPath(name);

		return name == m_fileName;
	}

	bool CSingleFileMemoryOutputer::output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size)
	{
		std::wstring name = fileName;
		convertPath(name);
		assert(name == m_fileName);
		if (!isDirectory)
		{
			m_memData.clear();
			m_memData.append((const char*)data, *size);
		}

		return true;
	}

	bool CSingleFileMemoryOutputer::continueProcess() const
	{
		return m_memData.empty();
	}

	/*
	用于文件检验
	*/
	class CCheckOutputer :public IOutputer
	{
	public:
		CCheckOutputer() 
		{
		};

	private:
		virtual bool output(const std::wstring& fileName, bool isDirectory, const void* data, size_t *size)
		{
			DW_UNUSED(fileName);
			DW_UNUSED(isDirectory);
			DW_UNUSED(data);
			DW_UNUSED(size);
			return true;
		}
	};
}
namespace DwUtility
{
	namespace zip
	{
		bool _UnZip7zToDisk(CFileInStream *archiveStream, IOutputer* outputer)
		{
			CLookToRead lookStream;
			CSzArEx db;
			SRes res = 0;
			ISzAlloc allocImp;
			ISzAlloc allocTempImp;
			UInt16 *temp = NULL;
			size_t tempSize = 0;

			allocImp.Alloc = SzAlloc;
			allocImp.Free = SzFree;

			allocTempImp.Alloc = SzAllocTemp;
			allocTempImp.Free = SzFreeTemp;
			LookToRead_CreateVTable(&lookStream, False);
			lookStream.realStream = &archiveStream->s;
			LookToRead_Init(&lookStream);
			CrcGenerateTable();
			SzArEx_Init(&db);
			res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
			if (res == SZ_OK)
			{
				UInt32 i;
				UInt32 blockIndex = 0xFFFFFFFF; 
				Byte *outBuffer = 0; 
				size_t outBufferSize = 0; 
				for (i = 0; i < db.db.NumFiles && outputer->continueProcess(); i++)
				{
					size_t offset = 0;
					size_t outSizeProcessed = 0;
					const CSzFileItem *f = db.db.Files + i;
					size_t len;
					
					len = SzArEx_GetFileNameUtf16(&db, i, NULL);
					if (len > tempSize)
					{
						SzFree(NULL, temp);
						tempSize = len;
						temp = (UInt16 *)SzAlloc(NULL, tempSize * sizeof(temp[0]));
						if (temp == 0)
						{
							res = SZ_ERROR_MEM;
							break;
						}
					}

					SzArEx_GetFileNameUtf16(&db, i, temp);
					if (outputer->isThisWillBeProcess(std::wstring((wchar_t*) temp)))
					{
						if (f->IsDir == 0)
						{
							res = SzArEx_Extract(&db, &lookStream.s, i,
								&blockIndex, &outBuffer, &outBufferSize,
								&offset, &outSizeProcessed,
								&allocImp, &allocTempImp);
							if (res != SZ_OK)
							{
								break;
							}
						}
						if(!outputer->output(std::wstring((wchar_t*) temp), f->IsDir != 0, outBuffer + offset, &outSizeProcessed))
						{
							res = SZ_ERROR_FAIL;
							break;
						}
					}
				}
				IAlloc_Free(&allocImp, outBuffer);
			}
			SzArEx_Free(&db, &allocImp);
			SzFree(NULL, temp);

			return (res == SZ_OK);
		}

		bool UnZip7zToDisk(const char* szSrc, const char* szDesFolder)
		{
			CFileInStream archiveStream;
			if (InFile_Open(&archiveStream.file, szSrc))
			{
				return false;
			}
			std::string strTemp(szDesFolder);
			std::wstring folder = DwUtility::text::toWideString(strTemp.c_str(), strTemp.size());
			FileInStream_CreateVTable(&archiveStream);
			CDiskOutputer outputer(folder);
			bool b = _UnZip7zToDisk(&archiveStream, &outputer);
			File_Close(&archiveStream.file);
			return b;
		}

		bool UnZip7zToDisk(const char* data, size_t len, const char* szDesFolder)
		{
			CMemoryInStream archiveStream;
			archiveStream.start = data;
			archiveStream.pos = 0;
			archiveStream.len = len;
			std::string strTemp(szDesFolder);
			std::wstring folder = DwUtility::text::toWideString(strTemp.c_str(), strTemp.size());
			MemoryInStream_CreateVTable(&archiveStream);
			CDiskOutputer outputer(folder);
			return _UnZip7zToDisk(&archiveStream, &outputer);
		}


		bool UnZip7zToMemory(const char *szSrc, const char *zipFileName, std::string& outData)
		{
			CFileInStream archiveStream;
			if (InFile_Open(&archiveStream.file, szSrc))
			{
				return false;
			}
			FileInStream_CreateVTable(&archiveStream);
			std::string strTemp(zipFileName);
			std::wstring fileName = DwUtility::text::toWideString(strTemp.c_str(), strTemp.size());
			CSingleFileMemoryOutputer outputer(fileName, outData);
			bool b = _UnZip7zToDisk(&archiveStream, &outputer);
			File_Close(&archiveStream.file);
			return b;
		}

		bool UnZip7zToMemory(const char* data, size_t len, const char *zipFileName, std::string& outData)
		{
			CMemoryInStream archiveStream;
			archiveStream.start = data;
			archiveStream.pos = 0;
			archiveStream.len = len;
			MemoryInStream_CreateVTable(&archiveStream);
			std::string strTemp(zipFileName);
			std::wstring fileName = DwUtility::text::toWideString(strTemp.c_str(), strTemp.size());
			CSingleFileMemoryOutputer outputer(fileName, outData);
			return _UnZip7zToDisk(&archiveStream, &outputer);
		}

		bool UnZip7zToMemory(const char *szSrc, std::map<std::wstring, std::string>& outData)
		{
			CFileInStream archiveStream;
			if (InFile_Open(&archiveStream.file, szSrc))
			{
				return false;
			}
			FileInStream_CreateVTable(&archiveStream);
			CMemoryOutputer outputer(outData);
			bool b = _UnZip7zToDisk(&archiveStream, &outputer);
			File_Close(&archiveStream.file);
			return b;
		}

		bool UnZip7zToMemory(const char* data, size_t len, std::map<std::wstring, std::string>& outData)
		{
			CMemoryInStream archiveStream;
			archiveStream.start = data;
			archiveStream.pos = 0;
			archiveStream.len = len;
			MemoryInStream_CreateVTable(&archiveStream);
			CMemoryOutputer outputer(outData);
			return _UnZip7zToDisk(&archiveStream, &outputer);
		}

		bool _7zChecksum(CFileInStream *archiveStream)
		{
			CLookToRead lookStream;
			CSzArEx db;
			SRes res = SZ_OK;
			ISzAlloc allocImp;
			ISzAlloc allocTempImp;
			allocImp.Alloc = SzAlloc;
			allocImp.Free = SzFree;

			allocTempImp.Alloc = SzAllocTemp;
			allocTempImp.Free = SzFreeTemp;
			LookToRead_CreateVTable(&lookStream, False);
			lookStream.realStream = &archiveStream->s;
			LookToRead_Init(&lookStream);
			CrcGenerateTable();
			SzArEx_Init(&db);
			res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
			SzArEx_Free(&db, &allocImp);

			return (res == SZ_OK);
		}

		bool Check7zFile(const char* szSrc)
		{
			CFileInStream archiveStream;
			if (InFile_Open(&archiveStream.file, szSrc))
			{
				return false;
			}
			FileInStream_CreateVTable(&archiveStream);
			bool result = _7zChecksum(&archiveStream);
			File_Close(&archiveStream.file);
			return result;
		}
	}
}

#pragma warning( pop )