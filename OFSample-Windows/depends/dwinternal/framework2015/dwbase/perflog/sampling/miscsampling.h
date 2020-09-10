/** 
@file
@brief CPU/内存数据采集
@version 2011-9-7 bobdeng
*/

#pragma once

#include "stlalloc.h"
#include "logfile.h"

namespace Perf
{
	class CMiscSampling
	{
		struct CPU_THREAD_DATA
		{
			DWORD		dwLen;
			DWORD		dwSize;		//单位为BYTE
			DWORD		dwTID;		//线程ID
			HANDLE		hThread;	//线程句柄
			void *		pBuf;
		};

	public:
		static CMiscSampling *GetInstance();
		DWORD Save(HANDLE hFile);
		void AddThread(DWORD dwTID);
		void CpuSampling(LONGLONG llPerfCount = 0);
		void MemSampling(LONGLONG llPerfCount = 0);

	private:
		CMiscSampling();
		DWORD SaveCpuData(HANDLE hFile);
		DWORD SaveMemData(HANDLE hFile);
		void SendCpuToTools(DWORD dwTID, const CPU *p);
		void SendMemToTools(const MEMORY *p);
		void Cpu(DWORD dwTID, struct CPU_THREAD_DATA **ppBlock, LONGLONG llPerfCount = 0);

	private:
		static CPU_THREAD_DATA * AllocCpuBlock(DWORD dwTID, HANDLE hThread);

		typedef std::map<DWORD, CPU_THREAD_DATA*, std::less<DWORD>, CMyStlAlloc<std::pair<DWORD, CPU_THREAD_DATA *> > > CMapThread;
		CMapThread			m_mapThread;
		
		std::vector<CPU_THREAD_DATA*, CMyStlAlloc<CPU_THREAD_DATA*> > m_vecCpuBlock;

		struct MEM_DATA
		{
			DWORD		dwCount;
			DWORD		dwSize;		////单位为MEMORY个数
			MEMORY *	arItem;
		};
		std::vector<MEM_DATA*, CMyStlAlloc<MEM_DATA*> > m_vecMemBlock;

		DWORD				m_dwNumProcessor;

		CRITICAL_SECTION	m_cri;
	};
}
