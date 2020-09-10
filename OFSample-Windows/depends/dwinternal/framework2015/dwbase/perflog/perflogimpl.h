#pragma once
#include <vector>
#include <map>
#include "dwutility/locker.h"
#include "stlalloc.h"
#include "logfile.h"
#include "perflog.h"

namespace Perf
{
	class CPerfLogImpl
	{
	public:
		static CPerfLogImpl * GetInstance();
		void Record(Perf::E_LOG_TYPE eType, WORD wLevel, WORD wNameID, WORD wFilterID);
		DWORD Save(HANDLE hFile);

		void Stub(E_LOG_TYPE eType, WORD wNameID, WORD wFlag, DWORD dwTime, LARGE_INTEGER llPerfCount);

		//取某个桩的平均耗时
		WORD GetAverageStubTime(const char* strStub, DWORD dwAfterTickCount = 0);
		//取某个时间的下个桩耗时
		WORD GetNextStubTime(const char* strStub, DWORD dwAfterTickCount = 0, DWORD* pdwTimeEnd = NULL);
		//二分查找桩
		int BinaryFindStubByTime(int nBegin, int nEnd, DWORD dwTickCount);

	public:
		void GetStubs(std::vector<struct Perf::StubTag> &vecStubs);

	private:
		CPerfLogImpl();

		std::vector<struct StubTag, CMyStlAlloc<struct StubTag> >	m_vecStubs;
		std::vector<struct LogTls*, CMyStlAlloc<struct LogTls*> >	m_vecLogTls;
		DWORD														m_dwTlsLog;

		CRITICAL_SECTION	m_cri;
	};

	class YYMultiProcessPerf
	{
	public:
		YYMultiProcessPerf();
		virtual ~YYMultiProcessPerf();

		void init();
		void stub(const std::string &name, E_LOG_TYPE type);
		void calc();
		void uninit();

	private:
		void _calc(const std::string &name, unsigned beginTick, unsigned endTick);
		void _serialize(E_LOG_TYPE type, std::string &buffer);
		void _unserialize( const std::string &buffer);
		void _writePerfResultToFile(const std::map<std::string, unsigned> &perfResultMap);
		void _backupAllMultPerfFiles();
		bool _isExistMpfFile();

	private:
		std::wstring m_path;
		std::map<std::string, unsigned>	m_beginStub;
		std::map<std::string, unsigned>	m_endStub;
		DwUtility::DwCSLock	m_lock;
	};

	YYMultiProcessPerf* getMultiPerfImpl();

}
