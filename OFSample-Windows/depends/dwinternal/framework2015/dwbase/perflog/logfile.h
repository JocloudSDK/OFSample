#pragma once 

#include "perflog.h"
namespace Perf
{
	enum E_BLOCK_FLAG
	{
		FLAG_E_UNKNOWN,
		FLAG_E_LOG,
		FLAG_E_STUB,
		FLAG_E_STR_TABLE,
		FLAG_E_MODULE,
		FLAG_E_CALLSTACK,
		FLAG_E_CPU,
		FLAG_E_MEMORY,
		FLAG_E_THREAD_TABLE,

		FLAG_E_DISK,
		FLAG_E_NETWORK
	};

	struct BLOCK_HEAD
	{
		DWORD dwFlag;
		DWORD dwSize;	//����BLOCK_HEAD����Ĵ�С
	};

	struct LogTag;

	struct StubTag;

	struct LOG_THREAD_HEAD
	{
		DWORD				dwTID;		//�߳�ID
		int					count;
		//LogTag			data[count];
	};

	struct LOG_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		int					count;
		//LOG_THREAD_HEAD	data[count];
	};

	//����ʱ��
	struct STUB_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		int					count;
		//StubTag	data[count];
	};

	struct CALLSTACK
	{
		LONGLONG			llPerfCount;
		DWORD				dwRepeat;
		DWORD				dwCount;
		//DWORD				stack[dwCount];
	};

	struct CALLSTACK_THREAD_HEAD
	{
		DWORD				dwTID;		//�߳�ID
		DWORD				dwDataSize;
		//CALLSTACK			data[varlen];
	};

	struct CALLSTACK_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		int					count;
		//CALLSTACK_THREAD_HEAD	data[varlen];
	};

	struct CPU
	{
		LONGLONG			llPerfCount;
		DWORD				dwUserTime;
		DWORD				dwKernelTime;
	};

	struct CPU_THREAD_HEAD
	{
		DWORD				dwTID;		//�߳�ID, 0��ʾ���̣���1��ʾϵͳ
		DWORD				count;
		//CPU				data[count];
	};

	struct CPU_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		DWORD				dwNumProcessor;
		int					count;
		//CPU_THREAD_HEAD	data[varlen];
	};

	struct MEMORY
	{
		LONGLONG			llPerfCount;
		DWORD				dwMemUse;
		DWORD				dwVMSize; 
	};

	struct MEM_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		DWORD				dwCount;
		//MEMORY			data[dwCount];
	};

	struct DISK
	{
		LONGLONG			llPerfCount;
		DWORD				dwDiskQueueLength;
		DWORD				dwDiskWriteBytesPerSec;
		DWORD				dwDiskReadBytesPerSec;
	};

	struct DISK_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		DWORD				dwCount;
	};

	struct NETWORK
	{
		LONGLONG			llPerfCount;
		DWORD				dwBytesSentPerSec;
		DWORD				dwBytesReceivedPerSec;
	};

	struct NETWORK_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		DWORD				dwCount;
	};

	struct THREAD_INFO
	{
		DWORD dwTID;
		WORD wNameID;		//�߳�����ID��λ��stringtable��
	};

	struct THREAD_TABLE
	{
		BLOCK_HEAD			header;
		DWORD				dwCount;
		//THREAD_INFO		data[dwCount];
	};

	struct MOD_INFO
	{
		DWORD dwModuleBase;
		DWORD dwImageSize;
		DWORD dwTimeStamp;
		wchar_t szModPath[MAX_PATH];
		wchar_t szSigPdb[64];
	};

	struct MODULE_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		DWORD				dwCount;
		//MOD_INFO			data[dwCount];		
	};

	struct STR_TABLE_BLOCK_HEAD
	{
		BLOCK_HEAD			header;
		//char				strtable[len]; "abcd\0asfd\0eeff\0"
	};

	struct FILE_HEAD
	{
		DWORD	dwMagic;	//perf��ʶ����perf�� = 0x66726570
		DWORD	dwVer;		//�ļ��汾��
		DWORD	dwPID;
		LONGLONG llPerfFrequency;
		LONGLONG llBeginPerfCount;
		LONGLONG llEndPerfCount;
		DWORD	dwBeginTickcount;
		DWORD	dwDataLen;
		FILE_HEAD()
		{
			dwMagic = 0x66726570;
			dwVer = 1;
			dwPID = 0;
			LARGE_INTEGER llPerf = {0};
			QueryPerformanceFrequency(&llPerf);
			llPerfFrequency = llPerf.QuadPart;
			llBeginPerfCount = 0;
			llEndPerfCount = 0;
			dwBeginTickcount = 0;
			dwDataLen = 0;
		}
	};
	void SaveFile(LPCTSTR strFilePath);
}
