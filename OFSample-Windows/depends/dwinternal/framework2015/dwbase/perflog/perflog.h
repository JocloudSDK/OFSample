/** 
@file
@brief PerfLogϵͳʵ�ֵ�ͷ�ļ�
@version huangyizhao
*/

#pragma once

#ifdef DWBASE_EXPORTS
	#define DWBASE_API _declspec(dllexport)
#else
	#define DWBASE_API _declspec(dllimport)
#endif

#include <vector>
//filter
__declspec( selectany ) extern const char F_EMPTY[] =  "";
__declspec( selectany ) extern const char F_DUI[] = "Dui";
__declspec( selectany ) extern const char F_SLOT[] = "Slot";
__declspec( selectany ) extern const char F_IO[] = "IO";
__declspec( selectany ) extern const char F_QT[] = "Qt";
__declspec( selectany ) extern const char F_QTGUI[] = "QtGui";

namespace Perf
{
#define  MAX_PATH_LEN 260
#define  WM_PERFLOG_YYMSG (WM_APP + 0xff)
	const wchar_t c_szPerfMsgWndNameFmt[] = L"YYPerfLogMsgWnd_%d";
	enum E_MSG_PERFLOG
	{
		MSG_E_INITHOOKSLOT = 0,
		MSG_E_UNHOOKSLOT = 1,
	};
	enum E_LOG_TYPE
	{
		TYPE_E_SINGTION = 0,
		TYPE_E_BEGIN = 1,
		TYPE_E_END = 2
	};
	const unsigned short STUB_FLAG_NONE = 0;
	const unsigned short STUB_FLAG_CPU = 1;
	const unsigned short STUB_FLAG_MEM = 2;

	const unsigned long c_dwLogBlockSize = 128 * 1024;
	const unsigned long c_dwSamplingBlockSize = 128 * 1024;

	struct StubTag
	{
		WORD		wType;
		WORD		wNameID;
		DWORD		dwTime;
		LONGLONG	llPerfCount;
		DWORD		dwCPUKernelTime;
		DWORD		dwCPUUserTime;
		DWORD		dwPhyMem;
		DWORD		dwVirtMem;
		DWORD		dwTID;
		StubTag()
		{
			wType = 0;
			wNameID = 0;
			dwTime = 0;
			llPerfCount = 0;
			dwCPUKernelTime = 0;
			dwCPUUserTime = 0;
			dwPhyMem = 0;
			dwVirtMem = 0;
			dwTID = 0;
		}
	};

	struct LogTag
	{
		unsigned short		wType;
		unsigned short		wLevel;
		unsigned short		wNameID;
		unsigned short		wFilterID;
		__int64	llPerfCount;
		LogTag()
		{
			wType = 0;
			wLevel = 0;
			wNameID = 0;
			wFilterID = 0;
			llPerfCount = 0;
		}
	};
	struct LogTls
	{
		unsigned long		dwCount;
		unsigned long		dwSize;		//��λΪLogTag����
		unsigned long		dwTID;		//�߳�ID
		LogTag *	arLog;
	};

	//�������Log��һ��Log�����������PERFLOG��Ϳ���
	DWBASE_API void Record(E_LOG_TYPE eType, unsigned short wLevel, unsigned short wNameID, unsigned short wFilterID);

	//��׮����Ҫ��¼ʱ������Ϣ
	DWBASE_API void Stub(E_LOG_TYPE eType, const char* lpszName, unsigned short wFlag = STUB_FLAG_NONE);

	//�����ַ������ַ�����
	DWBASE_API unsigned short AddToStrTable(const char * lpszName);

	//�����߳�����
	DWBASE_API void SetThreadName(unsigned long dwThreadId, const char * lpszThreadName);

	//��ʼ������Log
	DWBASE_API void StartPerfLog(const wchar_t* szFilePath = NULL);

	//����Log����
	DWBASE_API void EndPerfLog();

	//��������Log�ļ�����·��
	DWBASE_API void SetPerfLogFile(const wchar_t* szFilePathName);

	//ȡ����Log�ȼ�
	DWBASE_API unsigned long GetPerfLogLevel();

	DWBASE_API void EnablePerfStub(BOOL bEnable);

	DWBASE_API void GetStubs(std::vector<struct StubTag> &vecStubs);

	enum E_THREAD_SAMPLING
	{
		SAMPLING_E_CALLSTACK = 0x1,
		SAMPLING_E_RESPONSE = 0x2,
		SAMPLING_E_CPU = 0x4,
	};

	//���ò����̣߳�Ŀǰֻ�������̺߳�biz�߳�
	DWBASE_API void SetThreadSampling(unsigned long dwThreadId, int nThreadSamplingType);

	DWBASE_API void __PerfLogBegin(unsigned short wLevel, const char *name, const char *flt);
	DWBASE_API void __PerfLogEnd(unsigned short wLevel, const char *name, const char *flt);

	//ȡĳ��׮��ƽ����ʱ
	DWBASE_API WORD GetAverageStubTime(const char* strStub, DWORD dwAfterTickCount = 0);

	//�����ͳ��
	DWBASE_API void MultiPerfInit();
	DWBASE_API void MultiPerfStub(const char* lpszName, E_LOG_TYPE type);
	DWBASE_API void MultiPerfCalc();
	DWBASE_API void MultiPerfUnInit();

	class CEnvState
	{
	public:
		CEnvState(const char *name, const char *flt)
		{
			wNameId = AddToStrTable(name);
			wFltId = AddToStrTable(flt);
		}
		unsigned short wNameId;
		unsigned short wFltId;
	};

	class CAutoPerfLog
	{
	public:
		inline CAutoPerfLog::CAutoPerfLog(unsigned short wLevel, const CEnvState *pEnv)
		{
			m_wLevel = wLevel;
			if (wLevel > GetPerfLogLevel())
				return;
			m_wStrID = pEnv->wNameId;
			m_wFilterID = pEnv->wFltId;
			Perf::Record(TYPE_E_BEGIN, wLevel, m_wStrID, m_wFilterID);
		}

		inline CAutoPerfLog::CAutoPerfLog(unsigned short wLevel, const char *name, const char *flt)
		{
			m_wLevel = wLevel;
			if (wLevel > GetPerfLogLevel())
				return;
			m_wStrID = AddToStrTable(name);
			m_wFilterID = AddToStrTable(flt);
			Perf::Record(TYPE_E_BEGIN, wLevel, m_wStrID, m_wFilterID);
		}

		inline CAutoPerfLog::CAutoPerfLog(unsigned short wLevel, const char *name, const char *flt, const char* param)
		{
			m_wLevel = wLevel;
			if (wLevel > GetPerfLogLevel())
				return;
			if (param && (*param))
			{
				char fname[MAX_PATH + 1] = { 0 };
				_snprintf_s(fname, MAX_PATH, _TRUNCATE, "%s (%s)", name, param);
				m_wStrID = AddToStrTable(fname);
			}
			else
				m_wStrID = AddToStrTable(name);

			m_wFilterID = AddToStrTable(flt);
			Perf::Record(TYPE_E_BEGIN, wLevel, m_wStrID, m_wFilterID);
		}

		inline CAutoPerfLog::CAutoPerfLog(unsigned short wLevel, const char *name, const char *flt, const wchar_t* param)
		{
			m_wLevel = wLevel;
			if (wLevel > GetPerfLogLevel())
				return;
			if (param && (*param))
			{
				char aparam[MAX_PATH + 1] = { 0 };
				::WideCharToMultiByte(CP_ACP, 0, param, -1, aparam, MAX_PATH, NULL, NULL);

				char fname[MAX_PATH + 1] = { 0 };
				_snprintf_s(fname, MAX_PATH, _TRUNCATE, "%s (%s)", name, aparam);
				m_wStrID = AddToStrTable(fname);
			}
			else
				m_wStrID = AddToStrTable(name);

			m_wFilterID = AddToStrTable(flt);
			Perf::Record(TYPE_E_BEGIN, wLevel, m_wStrID, m_wFilterID);
		}

		inline CAutoPerfLog::~CAutoPerfLog()
		{
			if (m_wLevel > GetPerfLogLevel())
				return;
			Perf::Record(TYPE_E_END, m_wLevel, m_wStrID, m_wFilterID);
		}

	protected:
		unsigned short	m_wLevel;
		unsigned short	m_wStrID;
		unsigned short	m_wFilterID;
	};

	class DWBASE_API CPerfRecord
	{
	public:
		CPerfRecord()
		{
			m_llBeginPerfCount = 0;
			m_llEndPerfCount = 0;
			m_dwBeginTickcount = 0;
			m_szFilePath[0] = '\0';
		}

		static CPerfRecord *GetInstance()
		{
			static CPerfRecord *s_pImpl = NULL;
			if (!s_pImpl)
				s_pImpl = new CPerfRecord();

			return s_pImpl;
		}

		void StartPerfLog(const wchar_t*  szFilePath = NULL);
		void EndPerfLog();
		void SetPerfLogFile(const wchar_t*  szFilePathName);

	public:
		__int64 m_llBeginPerfCount;
		__int64 m_llEndPerfCount;
		unsigned long m_dwBeginTickcount;
		wchar_t m_szFilePath[MAX_PATH_LEN];
	};
}

//PerfLog��PerfLogF��PERFLOG���ھ�̬�ַ���
//����Log
#define PerfLog(level, name, flt)\
	static Perf::CEnvState __env##name##flt(name, flt);\
	Perf::CAutoPerfLog __pfobj##name##flt(level, &__env##name##flt)

#define PerfLogP(level, name, flt, param)\
	Perf::CAutoPerfLog __pfobj##flt(level, name, flt, param)

//����Log���Զ���Filter��ʽ
#define PerfLogF(flt)	PerfLog(1, __FUNCTION__, flt)
#define PerfLogFP(flt, param)	PerfLogP(1, __FUNCTION__, flt, param)
//����2��log�Ƚϳ��ã��������
#define PerfLog2F(flt)	PerfLog(2, __FUNCTION__, flt)
#define PerfLog2FP(flt, param)	PerfLogP(2, __FUNCTION__, flt, param)

//�Զ�����Log��һ��ӵ���ע�ĺ���ͷ������õ�����Log��ʽ��ǿ���Ƽ��������ʽ
#define PERFLOG PerfLog(1, __FUNCTION__, F_EMPTY)
#define PERFLOGP(param) PerfLogP(1, __FUNCTION__, F_EMPTY, param);

//PerfLogDynamic���ڶ�̬�ַ���
#define PerfLogDynamic(level, name, flt)\
	Perf::CAutoPerfLog __pfobj(level, name, flt)

//PerfLogDynamicBegin��PerfLogDynamicEnd�����ڶ�̬�ַ���
//��������̬Log������Բ�һЩ�����Ǳ�Ҫ����ʹ�ã��������ϼ�����̬Log����
//��̬����Log��ʼ
#define PerfLogDynamicBegin(level, name, flt)\
	Perf::__PerfLogBegin(level, name, flt)
//��̬����Log������ÿ�α����Begin��ԣ�����ᵼ��Log������
#define PerfLogDynamicEnd(level, name, flt)\
	Perf::__PerfLogEnd(level, name, flt)

#ifndef DWBASE_EXPORTS
#undef DWBASE_API
#endif