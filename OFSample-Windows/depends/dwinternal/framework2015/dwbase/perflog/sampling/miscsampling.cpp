/** 
@file
@brief CPU/内存数据采集
@version 2011-9-7 bobdeng
*/


#include "stable.h"
#include <windows.h>
#include <assert.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include "miscsampling.h"
#include "toolexchangecenter.h"
#include "logfile.h"
#include "struct.h"
#include "sysutil.h"
#include "control.h"

//#define TEST_CPU_SAMPLING 

namespace Perf
{
	typedef NTSTATUS (__stdcall *NTQUERYSYSTEMINFORMATION) (
		__in       SYSTEM_INFORMATION_CLASS SystemInformationClass,
		__inout    PVOID SystemInformation,
		__in       ULONG SystemInformationLength,
		__out_opt  PULONG ReturnLength);	

	typedef NTSTATUS (__stdcall *GETSYSTEMTIMES) (
		__out         LPFILETIME lpIdleTime,
		__out         LPFILETIME lpKernelTime,
		__out         LPFILETIME lpUserTime);

	CMiscSampling *CMiscSampling::GetInstance()
	{
		static CMiscSampling *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CMiscSampling), TRUE)) CMiscSampling();

		return s_pImpl;
	}

	CMiscSampling::CMiscSampling()
	{
		::InitializeCriticalSection(&m_cri);

		//使用时再分配
		//CPU_THREAD_DATA * pBlock = AllocCpuBlock(0, 0);//进程CPU
		//m_vecCpuBlock.push_back(pBlock);
		//m_mapThread.insert(CMapThread::value_type(0, pBlock));

		//pBlock = AllocCpuBlock(-1, 0);//系统CPU
		//m_vecCpuBlock.push_back(pBlock);
		//m_mapThread.insert(CMapThread::value_type(-1, pBlock));

		CPU_THREAD_DATA * pBlock = NULL;
		m_mapThread.insert(CMapThread::value_type(0, pBlock));
		m_mapThread.insert(CMapThread::value_type(-1, pBlock));

		m_dwNumProcessor = 1;
		static NTQUERYSYSTEMINFORMATION s_pNtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQuerySystemInformation");	
		if (s_pNtQuerySystemInformation)
		{
			SYSTEM_BASIC_INFORMATION stSysBaseInfo = {0};
			LONG status = s_pNtQuerySystemInformation(SystemBasicInformation, &stSysBaseInfo, sizeof(stSysBaseInfo),NULL);
			if (status == NO_ERROR)
			{
				assert(stSysBaseInfo.bKeNumberProcessors > 0);
				m_dwNumProcessor = stSysBaseInfo.bKeNumberProcessors;
			}
		}
	}

	CMiscSampling::CPU_THREAD_DATA * CMiscSampling::AllocCpuBlock(DWORD dwTID, HANDLE hThread)
	{
		CPU_THREAD_DATA * pBlock = (struct CPU_THREAD_DATA *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CPU_THREAD_DATA), FALSE);
		pBlock->dwLen = 0;
		pBlock->dwSize = c_dwSamplingBlockSize;
		pBlock->dwTID = dwTID;
		pBlock->pBuf = CMyHeapAlloc::GetInstance()->MyAlloc(c_dwSamplingBlockSize, TRUE);
		pBlock->hThread = hThread;

		return pBlock;
	}

	void CMiscSampling::AddThread(DWORD dwTID)
	{
		::EnterCriticalSection(&m_cri);

		if (m_mapThread.find(dwTID) == m_mapThread.end())
		{
			//使用时再分配
			//HANDLE hThread = ::OpenThread(THREAD_GET_CONTEXT|THREAD_QUERY_INFORMATION|THREAD_SUSPEND_RESUME, FALSE, dwTID);	
			//CPU_THREAD_DATA * pBlock = AllocCpuBlock(dwTID, hThread);
			//m_vecCpuBlock.push_back(pBlock);
			//m_mapThread.insert(CMapThread::value_type(dwTID, pBlock));
	
			CPU_THREAD_DATA * pBlock = NULL;
			m_mapThread.insert(CMapThread::value_type(dwTID, pBlock));
		}

		::LeaveCriticalSection(&m_cri);
	}

	DWORD CMiscSampling::Save(HANDLE hFile)
	{
		DWORD dwRet = SaveCpuData(hFile);
		dwRet += SaveMemData(hFile);
		return dwRet;
	}


	DWORD CMiscSampling::SaveCpuData(HANDLE hFile)
	{
		::EnterCriticalSection(&m_cri);
		DWORD dwWritten = 0;

		CPU_BLOCK_HEAD stHead;
		stHead.header.dwSize = sizeof(stHead);
		stHead.header.dwFlag = FLAG_E_CPU;
		stHead.count = m_vecCpuBlock.size();
		stHead.dwNumProcessor = m_dwNumProcessor;

		for (UINT i = 0; i < m_vecCpuBlock.size(); i++)
			stHead.header.dwSize += sizeof(CPU_THREAD_HEAD) + m_vecCpuBlock[i]->dwLen;
		::WriteFile(hFile, &stHead, sizeof(stHead), &dwWritten, 0);

		for (UINT i = 0; i < m_vecCpuBlock.size(); i++)
		{
			CPU_THREAD_HEAD stCpuThread;
			stCpuThread.dwTID = m_vecCpuBlock[i]->dwTID;
			stCpuThread.count = m_vecCpuBlock[i]->dwLen / sizeof(CPU);

			::WriteFile(hFile, &stCpuThread, sizeof(stCpuThread), &dwWritten, 0);
			::WriteFile(hFile, m_vecCpuBlock[i]->pBuf, m_vecCpuBlock[i]->dwLen, &dwWritten, 0);
		}

		::LeaveCriticalSection(&m_cri);
		return stHead.header.dwSize;
	}

	DWORD CMiscSampling::SaveMemData(HANDLE hFile)
	{
		::EnterCriticalSection(&m_cri);
		DWORD dwWritten = 0;

		MEM_BLOCK_HEAD stHead;
		stHead.header.dwSize = sizeof(stHead);
		stHead.header.dwFlag = FLAG_E_MEMORY;
		stHead.dwCount = 0;

		for (UINT i = 0; i < m_vecMemBlock.size(); i++)
		{
			stHead.header.dwSize += m_vecMemBlock[i]->dwCount * sizeof(MEMORY);
			stHead.dwCount += m_vecMemBlock[i]->dwCount;
		}

		::WriteFile(hFile, &stHead, sizeof(stHead), &dwWritten, 0);

		for (UINT i = 0; i < m_vecMemBlock.size(); i++)
		{
			::WriteFile(hFile, m_vecMemBlock[i]->arItem, m_vecMemBlock[i]->dwCount * sizeof(MEMORY), &dwWritten, 0);
		}

		::LeaveCriticalSection(&m_cri);
		return stHead.header.dwSize;
	}

	void CMiscSampling::SendCpuToTools(DWORD dwTID, const CPU *p)
	{
		Perf::MSG_CPU stMsg;
		stMsg.dwNumProcessor	= m_dwNumProcessor;
		stMsg.dwTID				= dwTID;
		stMsg.llPerfCount		= p->llPerfCount;
		stMsg.dwUserTime		= p->dwUserTime;
		stMsg.dwKernelTime		= p->dwKernelTime;
		CToolExchangeCenter::GetInstance()->SendToTools(eMsgCPU, &stMsg, sizeof(stMsg));
	}

	void CMiscSampling::SendMemToTools(const MEMORY *p)
	{
		Perf::MSG_MEM stMsg;
		stMsg.llPerfCount		= p->llPerfCount;
		stMsg.dwMemUse			= p->dwMemUse;
		stMsg.dwVMSize			= p->dwVMSize;
		CToolExchangeCenter::GetInstance()->SendToTools(eMsgMem, &stMsg, sizeof(stMsg));
	}

	void CMiscSampling::Cpu(DWORD dwTID, struct CPU_THREAD_DATA **ppBlock, LONGLONG llPerfCount)
	{
		if (!ppBlock)
			return;

		if (!(*ppBlock))	//新线程
		{
			HANDLE hThread = NULL;
			if (dwTID != 0 && dwTID != -1)
				hThread = ::OpenThread(THREAD_GET_CONTEXT|THREAD_QUERY_INFORMATION|THREAD_SUSPEND_RESUME, FALSE, dwTID);
			*ppBlock = AllocCpuBlock(dwTID, hThread);
		}
		else if ((*ppBlock)->dwLen + sizeof(CPU) > (*ppBlock)->dwSize) //内存不够
			*ppBlock = AllocCpuBlock((*ppBlock)->dwTID, (*ppBlock)->hThread);

		struct CPU_THREAD_DATA *pBlock = *ppBlock;

		struct CPU *p = (struct CPU *)((PBYTE)pBlock->pBuf + pBlock->dwLen);
		if (llPerfCount != 0)
			p->llPerfCount = llPerfCount;
		else
			::QueryPerformanceCounter((LARGE_INTEGER*)&p->llPerfCount);

		if (pBlock->dwTID == 0)
		{
			DWORD64 dw64CreateTime = 0;
			DWORD64 dw64ExitTime = 0;
			DWORD64 dw64KernelTime = 0;
			DWORD64 dw64UserTime = 0;
			::GetProcessTimes(::GetCurrentProcess(), (LPFILETIME)&dw64CreateTime, (LPFILETIME)&dw64ExitTime, (LPFILETIME)&dw64KernelTime, (LPFILETIME)&dw64UserTime);						

			p->dwKernelTime = (dw64KernelTime & 0xFFFFFFFF);
			p->dwUserTime = (dw64UserTime & 0xFFFFFFFF);
			::InterlockedExchangeAdd((LONG*)&pBlock->dwLen, sizeof(CPU));
			SendCpuToTools(pBlock->dwTID, p);

			//for test
#ifdef TEST_CPU_SAMPLING
			static DWORD s_c = 0;
			static DWORD s_dwKernelTime = p->dwKernelTime;
			static DWORD s_dwUserTime = p->dwUserTime;
			static LONGLONG s_llPerfCount = p->llPerfCount; 

			s_c++;
			if (s_c % 25 == 0)
			{
				DWORD64 dw64Freq = 0;
				DWORD64 dw64End = 0;
				QueryPerformanceFrequency((LARGE_INTEGER *)&dw64Freq);
				QueryPerformanceCounter((LARGE_INTEGER *)&dw64End);

				TCHAR szText[1024] = {0};
				double fCpuUser = ((double)(p->dwUserTime - s_dwUserTime)/10000 * 100) / ((double)(dw64End-s_llPerfCount)*1000/dw64Freq) / m_dwNumProcessor;
				double fCpuKernel = (double)(p->dwKernelTime - s_dwKernelTime)/10000 * 100 / ((double)(dw64End-s_llPerfCount)*1000/dw64Freq) / m_dwNumProcessor;

				_sntprintf_s(szText, _countof(szText),  _TRUNCATE, _T("[process]  user-cpu = %0.2f, kernel-cpu = %0.2f\n"), fCpuUser, fCpuKernel);
				//OutputDebugString(szText);
				_tprintf(szText);

				s_dwKernelTime = p->dwKernelTime;
				s_dwUserTime = p->dwUserTime;
				s_llPerfCount = p->llPerfCount; 
			}
#endif
		}
		else if (pBlock->dwTID == -1)
		{
			DWORD64 dw64IdleTime = 0;
			DWORD64 dw64KernelTime = 0;
			DWORD64 dw64UserTime = 0;

			static GETSYSTEMTIMES s_pGetSystemTimes = (GETSYSTEMTIMES)::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "GetSystemTimes");
			assert(s_pGetSystemTimes != NULL);

			if (s_pGetSystemTimes)
			{
				BOOL bRes = s_pGetSystemTimes((FILETIME *)&dw64IdleTime, (FILETIME *)&dw64KernelTime, (FILETIME *)&dw64UserTime);
				assert(bRes);
				if (bRes)
				{
					assert(dw64KernelTime >= dw64IdleTime);
					p->dwKernelTime = ((dw64KernelTime - dw64IdleTime) & 0xFFFFFFFF);
					p->dwUserTime = (dw64UserTime & 0xFFFFFFFF);

					::InterlockedExchangeAdd((LONG*)&pBlock->dwLen, sizeof(CPU));
					SendCpuToTools(pBlock->dwTID, p);

					//for test
#ifdef TEST_CPU_SAMPLING
					static DWORD s_c = 0;
					static DWORD s_dwKernelTime = p->dwKernelTime;
					static DWORD s_dwUserTime = p->dwUserTime;
					static LONGLONG s_llPerfCount = p->llPerfCount; 

					s_c++;
					if (s_c % 25 == 0)
					{
						DWORD64 dw64Freq = 0;
						DWORD64 dw64End = 0;
						QueryPerformanceFrequency((LARGE_INTEGER *)&dw64Freq);
						QueryPerformanceCounter((LARGE_INTEGER *)&dw64End);

						TCHAR szText[1024] = {0};
						double fCpuUser = ((double)(p->dwUserTime - s_dwUserTime)/10000 * 100) / ((double)(dw64End-s_llPerfCount)*1000/dw64Freq) / m_dwNumProcessor;
						double fCpuKernel = (double)(p->dwKernelTime - s_dwKernelTime)/10000 * 100 / ((double)(dw64End-s_llPerfCount)*1000/dw64Freq) / m_dwNumProcessor;

						_sntprintf_s(szText, _countof(szText), _TRUNCATE, _T("[system]  user-cpu = %0.2f, kernel-cpu = %0.2f\n"), fCpuUser, fCpuKernel);
						//OutputDebugString(szText);
						_tprintf(szText);

						s_dwKernelTime = p->dwKernelTime;
						s_dwUserTime = p->dwUserTime;
						s_llPerfCount = p->llPerfCount; 
					}
#endif
				}
			}
		}
		else
		{
			DWORD64 dw64CreateTime = 0;
			DWORD64 dw64ExitTime = 0;
			DWORD64 dw64KernelTime = 0;
			DWORD64 dw64UserTime = 0;

			DWORD dwExitCode = 0;
			::GetExitCodeThread(pBlock->hThread, &dwExitCode);
			if (dwExitCode == STILL_ACTIVE && ::GetThreadTimes(pBlock->hThread, (LPFILETIME)&dw64CreateTime, (LPFILETIME)&dw64ExitTime, (LPFILETIME)&dw64KernelTime, (LPFILETIME)&dw64UserTime))
			{
				p->dwKernelTime = (dw64KernelTime & 0xFFFFFFFF);
				p->dwUserTime = (dw64UserTime & 0xFFFFFFFF);
				::InterlockedExchangeAdd((LONG*)&pBlock->dwLen, sizeof(CPU));
			}
		}
	}

	void CMiscSampling::CpuSampling(LONGLONG llPerfCount)
	{
		if (!CControl::GetInstance()->IsSamplingCPUMem())
			return;

		::EnterCriticalSection(&m_cri);
		//CMapThread mapCopy = m_mapThread;

		for (CMapThread::iterator it = m_mapThread.begin(); it != m_mapThread.end(); ++it)
		{
			struct CPU_THREAD_DATA *pBlock = it->second, *pPreBlock = pBlock;

			Cpu(it->first, &pBlock, llPerfCount);
			if (pBlock != pPreBlock)
			{
				m_vecCpuBlock.push_back(pBlock);
				m_mapThread[it->first] = pBlock;
			}
		}
		::LeaveCriticalSection(&m_cri);
	}

	void CMiscSampling::MemSampling(LONGLONG llPerfCount)
	{
		if (!CControl::GetInstance()->IsSamplingCPUMem())
			return;

		::EnterCriticalSection(&m_cri);
		if (m_vecMemBlock.empty() || m_vecMemBlock[m_vecMemBlock.size()-1]->dwCount == m_vecMemBlock[m_vecMemBlock.size()-1]->dwSize)
		{
			MEM_DATA * pBlock = (MEM_DATA *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(MEM_DATA), FALSE);
			pBlock->dwCount = 0;
			pBlock->dwSize = c_dwSamplingBlockSize / sizeof(MEMORY);
			pBlock->arItem = (MEMORY *)CMyHeapAlloc::GetInstance()->MyAlloc(c_dwSamplingBlockSize, TRUE);

			m_vecMemBlock.push_back(pBlock);
		}

		MEM_DATA * pBlock = m_vecMemBlock[m_vecMemBlock.size()-1];//m_vecMemBlock.back();
		GetMemoryUsage(&pBlock->arItem[pBlock->dwCount].dwMemUse, &pBlock->arItem[pBlock->dwCount].dwVMSize);
		if (llPerfCount != 0)
			pBlock->arItem[pBlock->dwCount].llPerfCount = llPerfCount;
		else
			::QueryPerformanceCounter((LARGE_INTEGER *)&pBlock->arItem[pBlock->dwCount].llPerfCount);

		SendMemToTools(&pBlock->arItem[pBlock->dwCount]);
		
		::InterlockedIncrement((LONG*)&pBlock->dwCount); 
		::LeaveCriticalSection(&m_cri);
	}
}
