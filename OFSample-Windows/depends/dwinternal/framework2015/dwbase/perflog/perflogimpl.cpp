/** 
@file
@brief 性能Log实现类
*/
#include "stable.h"
#include "dwutility/ufile.h"
#include "dwutility/appdatapath.h"
#include <mmsystem.h>
#include <assert.h>
#include <time.h>
#include "perflog.h"
#include "PerfLogImpl.h"
#include "sysutil.h"
#include <tchar.h>
#include "control.h"
#include <winbase.h>

#pragma comment(lib, "Winmm.lib")   //timeGetTime()

namespace Perf
{
	CPerfLogImpl * CPerfLogImpl::GetInstance()
	{
		static CPerfLogImpl *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CPerfLogImpl), TRUE)) CPerfLogImpl();

		return s_pImpl;
	}

	CPerfLogImpl::CPerfLogImpl()
	{
		m_dwTlsLog = ::TlsAlloc();
		::InitializeCriticalSection(&m_cri);
		m_vecStubs.reserve(1000);
	}

	//返回数据保存的长度
	DWORD CPerfLogImpl::Save(HANDLE hFile)
	{	
		::EnterCriticalSection(&m_cri);
		DWORD dwWritten = 0;
		//Stub
		STUB_BLOCK_HEAD stStubHead;
		stStubHead.header.dwSize = sizeof(stStubHead) + sizeof(StubTag) * m_vecStubs.size();
		stStubHead.header.dwFlag = FLAG_E_STUB;
		stStubHead.count = m_vecStubs.size();
		::WriteFile(hFile, &stStubHead, sizeof(stStubHead), &dwWritten, 0);

		for (UINT i = 0; i < m_vecStubs.size(); i++)
			::WriteFile(hFile, &m_vecStubs[i], sizeof(StubTag), &dwWritten, 0);

		//Log
		LOG_BLOCK_HEAD stLogHead;
		stLogHead.header.dwSize = sizeof(stLogHead);
		stLogHead.header.dwFlag = FLAG_E_LOG;
		stLogHead.count = m_vecLogTls.size();

		for (UINT i = 0; i < m_vecLogTls.size(); i++)
			stLogHead.header.dwSize += m_vecLogTls[i]->dwCount * sizeof(LogTag) + sizeof(LOG_THREAD_HEAD);

		::WriteFile(hFile, &stLogHead, sizeof(stLogHead), &dwWritten, 0);

		for (UINT i = 0; i < m_vecLogTls.size(); i++)
		{
			LOG_THREAD_HEAD stLogThread;
			stLogThread.dwTID = m_vecLogTls[i]->dwTID;
			stLogThread.count = m_vecLogTls[i]->dwCount;
			::WriteFile(hFile, &stLogThread, sizeof(stLogThread), &dwWritten, 0);

			::WriteFile(hFile, m_vecLogTls[i]->arLog, m_vecLogTls[i]->dwCount * sizeof(LogTag), &dwWritten, 0);
		}

		::LeaveCriticalSection(&m_cri);
		return stStubHead.header.dwSize + stLogHead.header.dwSize;
	}

	void CPerfLogImpl::Record(Perf::E_LOG_TYPE eType, WORD wLevel, WORD wNameID, WORD wFilterID)
	{
		if (wLevel > CControl::GetInstance()->GetSamplingLogLevel())
		{
			return;
		}
#ifdef _DEBUG
		static int s_nLogCount = 0;
		++s_nLogCount;
#endif

		struct LogTls * pLogTls = (struct LogTls *)::TlsGetValue(m_dwTlsLog);
		if (!pLogTls || pLogTls->dwCount >= pLogTls->dwSize - 1) //新线程或者内存不够
		{
			pLogTls = (struct LogTls *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(LogTls), FALSE);
			pLogTls->dwCount = 0;
			pLogTls->dwSize = c_dwLogBlockSize / sizeof(LogTag);
			pLogTls->dwTID = ::GetCurrentThreadId();
			pLogTls->arLog = (struct LogTag *)CMyHeapAlloc::GetInstance()->MyAlloc(c_dwLogBlockSize, TRUE);
			::EnterCriticalSection(&m_cri);
			m_vecLogTls.push_back(pLogTls);
			::LeaveCriticalSection(&m_cri);

			::TlsSetValue(m_dwTlsLog, pLogTls);
		}

		if (pLogTls && pLogTls->arLog)
		{
			struct LogTag *pLog = &pLogTls->arLog[pLogTls->dwCount++];		
			pLog->wType = (WORD)eType;
			pLog->wLevel = wLevel;
			pLog->wNameID = wNameID;
			pLog->wFilterID = wFilterID;
			LARGE_INTEGER llPerfCount = {0};
			QueryPerformanceCounter(&llPerfCount);
			pLog->llPerfCount = llPerfCount.QuadPart;
		}
	}

	void CPerfLogImpl::Stub(E_LOG_TYPE eType, WORD wNameID, WORD wFlag, DWORD dwTime, LARGE_INTEGER llPerfCount)
	{
		if (!CControl::GetInstance()->isPerfStubEnabled())
			return;

		::EnterCriticalSection(&m_cri);
		const int MAX_STUB_COUNT = 1000;	//限制最大打桩数1000，防止错误用法，导致内存占用过大
		static int s_nStubCount = 0;
		if (s_nStubCount < MAX_STUB_COUNT)
		{
			++s_nStubCount;

			struct StubTag stStub;
			stStub.wType = (WORD)eType;
			stStub.wNameID = wNameID;
			stStub.dwTime = dwTime;
			stStub.llPerfCount = llPerfCount.QuadPart;
			stStub.dwTID = GetCurrentThreadId();

			if (wFlag & STUB_FLAG_CPU)
			{
				FILETIME CreationTime; 
				FILETIME ExitTime; 
				FILETIME KernelTime; 
				FILETIME UserTime; 
				GetProcessTimes(GetCurrentProcess(), &CreationTime, &ExitTime, &KernelTime, &UserTime);
				stStub.dwCPUKernelTime = KernelTime.dwLowDateTime;
				stStub.dwCPUUserTime = KernelTime.dwLowDateTime;
			}

			if (wFlag & STUB_FLAG_MEM)
			{
				DWORD dwPhy = 0;
				DWORD dwVirt = 0;
				GetMemoryUsage(&dwPhy, &dwVirt);
				stStub.dwPhyMem = dwPhy;
				stStub.dwVirtMem = dwVirt;
			}

			m_vecStubs.push_back(stStub);		
		}
		
		::LeaveCriticalSection(&m_cri);
	}

	//取某个桩的平均耗时
	WORD CPerfLogImpl::GetAverageStubTime(const char* strStub, DWORD dwAfterTickCount)
	{
		DWORD dwTimeEnd = dwAfterTickCount;
		DWORD dwTotalTime = 0;
		int nCount = 0;
		for (int i = 0; i < 10; ++i)
		{
			//同一操作最多10条，做个容错
			WORD wTime = GetNextStubTime(strStub, dwTimeEnd, &dwTimeEnd);
			++dwTimeEnd; //下条Log从dwTimeEnd+1开始取，防止重复取耗时为0的Stub
			if (wTime == 0)
			{
				break;
			}
			dwTotalTime += wTime;
			++nCount;
		}

		WORD wAverageTime = 0;
		if (nCount > 0)
		{
			wAverageTime = WORD(dwTotalTime / nCount);
		}
		return wAverageTime;
	}

	//取某个时间的下个桩耗时
	WORD CPerfLogImpl::GetNextStubTime(const char* strStub, DWORD dwAfterTickCount, DWORD* pdwTimeEnd)
	{
		WORD wStub = AddToStrTable(strStub);

		int nIndex = BinaryFindStubByTime(0, m_vecStubs.size() - 1, dwAfterTickCount);
		if (nIndex < 0 || nIndex >= (int)m_vecStubs.size())
		{
			return 0;
		}

		if (strncmp(strStub, "[m]", 3) == 0)
		{
			//如果包含"[m]"的桩要取最后一个End的Time值
			DWORD dwTimeBegin = 0;
			DWORD dwTimeEnd = 0;	
			for (std::vector<struct StubTag, CMyStlAlloc<struct StubTag>>::iterator it = m_vecStubs.begin() + nIndex; 
				it != m_vecStubs.end(); ++it)
			{
				if (it->wNameID == wStub && it->wType == (WORD)TYPE_E_BEGIN)
				{
					if (dwTimeEnd > 0)
					{	//这里已经是第二个Begin了
						break;
					}
					dwTimeBegin = it->dwTime;
				}
				else if (it->wNameID == wStub && it->wType == (WORD)TYPE_E_END && dwTimeBegin != 0)
				{
					if (dwTimeEnd > 0 && it->dwTime - dwTimeBegin > 60000)
					{	
						//这里提供60s容错功能，一般用例不会超过60s
						break;
					}
					dwTimeEnd = it->dwTime;
				}
			}
			if (pdwTimeEnd)
			{
				*pdwTimeEnd = dwTimeEnd;
			}
			if (dwTimeEnd > dwTimeBegin)
			{
				if (dwTimeEnd - dwTimeBegin > 0xffff)
					return 0xffff;
				else
					return WORD(dwTimeEnd - dwTimeBegin);
			}
			return 0;
		}
		else
		{
			//取首次Begin和之后的首次End
			DWORD dwTimeBegin = 0;
			for (std::vector<struct StubTag, CMyStlAlloc<struct StubTag>>::iterator it = m_vecStubs.begin() + nIndex; 
				it != m_vecStubs.end(); ++it)
			{
				if (it->wNameID == wStub && it->wType == (WORD)TYPE_E_BEGIN)
				{
					dwTimeBegin = it->dwTime;
				}
				else if (it->wNameID == wStub && it->wType == (WORD)TYPE_E_END && dwTimeBegin != 0)
				{
					if (pdwTimeEnd)
					{
						*pdwTimeEnd = it->dwTime;
					}

					if (it->dwTime - dwTimeBegin > 0xffff)
						return 0xffff;
					else
						return WORD(it->dwTime - dwTimeBegin);
				}
			}
			return 0;
		}
	}

	//二分查找桩
	int CPerfLogImpl::BinaryFindStubByTime(int nBegin, int nEnd, DWORD dwTickCount)
	{
		int nSize = m_vecStubs.size();
		if (nBegin >= nSize || nEnd >= nSize)
		{
			return -1;
		}
		if (nBegin >= nEnd)
		{
			return nBegin;
		}
		int nMid = (nBegin + nEnd) / 2;
		if (m_vecStubs.at(nMid).dwTime >= dwTickCount)
		{
			return BinaryFindStubByTime(nBegin, nMid, dwTickCount);
		}
		return BinaryFindStubByTime(nMid + 1, nEnd, dwTickCount);
	}

	void CPerfLogImpl::GetStubs(std::vector<struct Perf::StubTag> &vecStubs)
	{
		::EnterCriticalSection(&m_cri);
		vecStubs.clear();
		for (std::vector<struct StubTag, CMyStlAlloc<struct StubTag>>::iterator it = m_vecStubs.begin(); it != m_vecStubs.end(); ++it)
		{
			vecStubs.push_back(*it);
		}
		::LeaveCriticalSection(&m_cri);

	}
	

	//////////////////////////////////////////////////////////////////////////

	YYMultiProcessPerf::YYMultiProcessPerf()
	{

	}

	YYMultiProcessPerf::~YYMultiProcessPerf()
	{

	}

	void YYMultiProcessPerf::init()
	{
		m_path = DwUtility::AppDataPath::GetLogPath() + L"multiperf\\";
		DwUtility::ufile::CreateAllDirectory(m_path.c_str());

		//_backupAllMultPerfFiles();
	}

	void YYMultiProcessPerf::stub(const std::string &name, E_LOG_TYPE type)
	{
		if (!CControl::GetInstance()->isMultiPerfStubEnabled())
		{
			return;
		}

		DwUtility::DwAutoCSLocker locker(&m_lock);

		if (m_path.empty())
		{
			return;
		}

		if (type == TYPE_E_BEGIN)
		{
			m_beginStub.insert(std::pair<std::string, unsigned>(name, ::timeGetTime()) );
		}
		else if (type == TYPE_E_END)
		{
			m_endStub.insert(std::pair<std::string, unsigned>(name, ::timeGetTime()) );
		}	
	}

	void YYMultiProcessPerf::calc()
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		if (m_path.empty())
		{
			return;
		}

		std::wstring finder = m_path + L"*.mpf";
		m_beginStub.clear();
		m_endStub.clear();

		WIN32_FIND_DATA wfd = {0};
		HANDLE hFind = ::FindFirstFile(finder.c_str(), &wfd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::wstring stubFile = m_path + wfd.cFileName;
				std::string buffer;
				if (DwUtility::ufile::ReadFileData(stubFile, buffer))
				{
					_unserialize(buffer);
				}

			} while(::FindNextFile(hFind, &wfd));

			::FindClose(hFind);
		}

		std::map<std::string, unsigned>	perfResultMap;
		std::map<std::string, unsigned>::const_iterator iter = m_beginStub.begin();
		while (iter != m_beginStub.end())
		{	
			const std::string &name = iter->first;
			int beginTick = iter->second;

			std::map<std::string, unsigned>::const_iterator finderIter = m_endStub.find(name);
			if (finderIter != m_endStub.end())
			{
				int endTick = finderIter->second;
				if (beginTick != 0 && endTick != 0 && endTick >= beginTick)
				{
					perfResultMap.insert( std::pair<std::string, unsigned>(name, (endTick - beginTick)) );
				}
			}

			
			++iter;
		}

		_writePerfResultToFile(perfResultMap);
	}

	void YYMultiProcessPerf::uninit()
	{
		if (m_path.empty())
		{
			return;
		}

		std::string buffer;
		{
			DwUtility::DwAutoCSLocker locker(&m_lock);
			_serialize(TYPE_E_BEGIN, buffer);
			_serialize(TYPE_E_END, buffer);
			m_beginStub.clear();
			m_endStub.clear();
		}

		if (buffer.empty())
		{
			return;
		}

		//文件名字
		wchar_t szPid[MAX_PATH] = {0};
		swprintf_s(szPid, MAX_PATH-1, L"%d", ::GetCurrentProcessId());

		wchar_t szLogName[MAX_PATH] = {0};
		wchar_t szExePath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szExePath, _countof(szExePath));
		const wchar_t *p1 = _tcsrchr(szExePath, _T('\\')) + 1;
		const wchar_t *p2 = _tcsrchr(p1, _T('.'));
		assert(p2 != NULL);
		if (p2 == NULL)
			p2 = szExePath + _tcslen(szExePath);

		wcsncpy_s(szLogName, p1, p2 - p1);

		std::wstring filePath = m_path + szLogName + L"_" + szPid + L".mpf";
		DwUtility::ufile::WriteFileData(filePath, buffer);
	}

	void YYMultiProcessPerf::_serialize(E_LOG_TYPE type, std::string &buffer)
	{
		std::map<std::string, unsigned>	*mapStubs = NULL;
		if (type == TYPE_E_BEGIN)
		{
			mapStubs = &m_beginStub;
		}
		else if (type == TYPE_E_END)
		{
			mapStubs = &m_endStub;
		}

		if (mapStubs == NULL)
		{
			return;
		}

		//type(char)|namesize(int)|name(char*)|tick(int)
		std::map<std::string, unsigned>::const_iterator iter = mapStubs->begin();
		while (iter != mapStubs->end())
		{	
			const std::string &name = iter->first;
			int nameSize = name.size();
			int tick = iter->second;
			
			buffer.append((char*)&type, sizeof(char));
			buffer.append((char*)&nameSize, sizeof(int));
			buffer.append(name.data(), nameSize);
			buffer.append((char*)&tick, sizeof(int));
	
			++iter;
		}
	}

	void YYMultiProcessPerf::_unserialize(const std::string &buffer)
	{
		char *data = (char*)buffer.data();
		int	size = buffer.size();

		while (size > 0)
		{
			//type(char)|namesize(int)|name(char*)|tick(int)
			int itemSize = sizeof(char);
			E_LOG_TYPE type = (E_LOG_TYPE)(*((char*)data));
			data += itemSize; size -= itemSize;
			
			itemSize = sizeof(int);
			int nameSize = *((int*)data);
			data += itemSize; size -= itemSize;

			itemSize = nameSize;
			std::string name(data, nameSize);
			data += itemSize; size -= itemSize;

			itemSize = sizeof(int);
			int tick = *((int*)data);
			data += itemSize; size -= itemSize;

			if (type == TYPE_E_BEGIN)
			{
				m_beginStub.insert( std::pair<std::string, unsigned>(name, tick) );
			}
			else if (type == TYPE_E_END)
			{
				m_endStub.insert( std::pair<std::string, unsigned>(name, tick) );
			}

		}
	}

	void YYMultiProcessPerf::_writePerfResultToFile(const std::map<std::string, unsigned> &perfResultMap)
	{		
		std::string buffer;
		std::map<std::string, unsigned>::const_iterator iter = perfResultMap.begin();
		while (iter != perfResultMap.end())
		{	
			const std::string &name = iter->first;
			int tick = iter->second;

			std::string tick_;
			tick_.resize(1024);
			char szTick[1024] = {0};
			sprintf_s(szTick, sizeof(szTick)-1, "%d", tick);

			std::string line = name + + " " + szTick + "\n";
			buffer.append(line);

			++iter;
		}

		std::wstring perfFile = m_path + L"perf.txt";
		DwUtility::ufile::WriteFileData(perfFile, buffer);
	}

	void YYMultiProcessPerf::_backupAllMultPerfFiles()
	{
		if (!_isExistMpfFile())
		{
			return;
		}

		std::wstring backupPath = m_path + L"backup/";
		DwUtility::ufile::CreateAllDirectory(backupPath.c_str());

		wchar_t szPid[1024] = {0};
		swprintf_s(szPid, 1024-1, L"%I64d", _time64(NULL));

		std::wstring path = backupPath + szPid;
		if (!DwUtility::ufile::CheckDirectoryExist(path.c_str()))
		{
			DwUtility::ufile::CreateAllDirectory(path.c_str());
		}

		std::wstring finder = m_path + L"*.mpf";
		WIN32_FIND_DATA wfd = {0};
		HANDLE hFind = ::FindFirstFile(finder.c_str(), &wfd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::wstring srcFileName = m_path + wfd.cFileName;
				std::wstring destFileName = backupPath + szPid + L"/" + wfd.cFileName;
				::MoveFile(srcFileName.c_str(), destFileName.c_str());
			
			} while(::FindNextFile(hFind, &wfd));

			::FindClose(hFind);
		}	
	}

	bool YYMultiProcessPerf::_isExistMpfFile()
	{
		std::wstring finder = m_path + L"*.mpf";
		WIN32_FIND_DATA wfd = {0};
		HANDLE hFind = ::FindFirstFile(finder.c_str(), &wfd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			::FindClose(hFind);
			return true;
		}
		else
		{
			return false;
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	YYMultiProcessPerf* getMultiPerfImpl()
	{
		static YYMultiProcessPerf *s_multiPerf = NULL;
		if (!s_multiPerf)
		{
			s_multiPerf = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(YYMultiProcessPerf), TRUE)) YYMultiProcessPerf();
		}

		return s_multiPerf;
	}

}
