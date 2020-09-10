/** 
@file
@brief ����Logϵͳʵ�ֵ�ί��ͷ�ļ�,ʵ�ֲ��ֳ��ù��ܣ�����dwutility��qt�ȣ�����Ҫ��̬����dwbase
@version 
*/

#pragma once
	
/** 
@file
@brief PerfLogϵͳʵ�ֵ�ͷ�ļ�
@version huangyizhao
*/

#pragma once

//filter
__declspec( selectany ) extern const char F_EMPTY[] =  "";
__declspec( selectany ) extern const char F_DUI[] = "Dui";
__declspec( selectany ) extern const char F_SLOT[] = "Slot";
__declspec( selectany ) extern const char F_IO[] = "IO";
__declspec( selectany ) extern const char F_QT[] = "Qt";
__declspec( selectany ) extern const char F_QTGUI[] = "QtGui";

__declspec(selectany) void* g_pfRecord = NULL;
__declspec(selectany) void* g_pfStub = NULL;
__declspec(selectany) void* g_pfAddToStrTable = NULL;
__declspec(selectany) void* g_pfGetPerfLogLevel = NULL;

namespace Perf
{
	enum E_LOG_TYPE
	{
		TYPE_E_SINGTION = 0,
		TYPE_E_BEGIN = 1,
		TYPE_E_END = 2
	};
	const unsigned short STUB_FLAG_NONE = 0;
	const unsigned short STUB_FLAG_CPU = 1;
	const unsigned short STUB_FLAG_MEM = 2;

	static void Init()
	{
		static bool s_bFirst = true;
		if (s_bFirst)
		{
			s_bFirst = false; //ֻ����һ�Σ�dll�������ǲ��ᷴ������

			HMODULE hModule = ::GetModuleHandleW(L"dwbase.dll");
			if (!g_pfRecord)
			{
				g_pfRecord = (void*)::GetProcAddress(hModule, "?Record@Perf@@YAXW4E_LOG_TYPE@1@GGG@Z");
			}
			if (!g_pfStub)
			{
				g_pfStub = (void*)::GetProcAddress(hModule, "?Stub@Perf@@YAXW4E_LOG_TYPE@1@PBDG@Z");
			}
			if (!g_pfAddToStrTable)
			{
				g_pfAddToStrTable = (void*)::GetProcAddress(hModule, "?AddToStrTable@Perf@@YAGPBD@Z");
			}
			if (!g_pfGetPerfLogLevel)
			{
				g_pfGetPerfLogLevel = (void*)::GetProcAddress(hModule, "?GetPerfLogLevel@Perf@@YAKXZ");
			}

		}
	}
	//�������Log��һ��Log�����������PERFLOG��Ϳ���
	static void Record(E_LOG_TYPE eType, unsigned short wLevel, unsigned short wNameID, unsigned short wFilterID)
	{
		Init();
		if (g_pfRecord)
			((void (__cdecl *)(E_LOG_TYPE, unsigned short, unsigned short, unsigned short))g_pfRecord)(eType, wLevel, wNameID, wFilterID);

	}

	//��׮����Ҫ��¼ʱ������Ϣ
	static void Stub(E_LOG_TYPE eType, const char* lpszName, unsigned short wFlag = STUB_FLAG_NONE)
	{
		Init();
		if (g_pfStub)
			((void (__cdecl *)(E_LOG_TYPE, const char*, unsigned short))g_pfStub)(eType, lpszName, wFlag);

	}

	//�����ַ������ַ�����
	static unsigned short AddToStrTable(const char * lpszName)
	{
		Init();
		if (g_pfAddToStrTable)
			return ((unsigned short (__cdecl *)(const char *))g_pfAddToStrTable)(lpszName);
		else
			return 0;

	}

	//ȡ����Log�ȼ�
	static unsigned long GetPerfLogLevel()
	{
		Init();
		if (g_pfGetPerfLogLevel)
		{
			return ((unsigned long(__cdecl *)())g_pfGetPerfLogLevel)();
		}
		else
			return 1;
	}

	class CEnvState
	{
	public:
		inline CEnvState(const char *name, const char *flt)
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
