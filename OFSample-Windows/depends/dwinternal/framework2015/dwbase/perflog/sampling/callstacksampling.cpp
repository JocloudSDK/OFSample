/** 
@file
@brief callstack数据采集
@version 2011-8-3 bobdeng
*/

#include "stable.h"
#include <windows.h>
#include <assert.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include "callstacksampling.h"
#include "control.h"

#pragma comment(lib, "psapi.lib")

extern const std::wstring GetModuleIndexString(void *pModBase, BOOL bPDB);

const int c_iMaxLevel = 64;

namespace Perf
{
	CCallStackSampling *CCallStackSampling::GetInstance()
	{
		static CCallStackSampling *s_pImpl = NULL;
		if (!s_pImpl)
			s_pImpl = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CCallStackSampling), TRUE)) CCallStackSampling();

		return s_pImpl;

	}

	CCallStackSampling::CCallStackSampling()
	{
		::InitializeCriticalSection(&m_cri);
	}

	int CCallStackSampling::GetStack(HANDLE hThread, PCONTEXT pCt, BYTE *buf, DWORD dwMaxCount)
	{		
		UINT i = 0;

		LDT_ENTRY sEntry = {0};
		if (hThread && GetThreadSelectorEntry(hThread, (WORD)pCt->SegFs, &sEntry))
		{
			DWORD dwSegFs = (sEntry.HighWord.Bytes.BaseHi << 24 ) | (sEntry.HighWord.Bytes.BaseMid << 16) | sEntry.BaseLow;
			assert(dwSegFs != 0);
			if (dwSegFs != 0)
			{
				void * pStackHigh = NULL;
				void * pStackLow = NULL;
				__asm
				{
					push ebx
					push edx

					mov	ebx,dwSegFs;
					mov edx,[ebx + 4];
					mov pStackHigh,edx;
					mov edx,[ebx + 8];
					mov pStackLow,edx;

					pop edx
					pop ebx
				}



				DWORD pc = pCt->Eip;
				PDWORD p_frame, p_prevFrame;
				p_frame = (PDWORD)pCt->Ebp;
				
				int nBufLen = 0;

				do
				{
					*(DWORD*)(buf + nBufLen) = pc;
					nBufLen += sizeof(DWORD);
					i++;
					
					if ((LPVOID)p_frame < pStackLow || (LPVOID)p_frame > pStackHigh)
						break;

					pc = p_frame[1];
					p_prevFrame = p_frame;	
					p_frame = (PDWORD)p_frame[0];
					
					if (((DWORD)p_frame & 3) || p_frame <= p_prevFrame || pc == 0)
						break;

				} while (i < dwMaxCount);
			}
		}

		return i;
	}

	CCallStackSampling::CallStackTls * CCallStackSampling::AllocCallStackBlock(DWORD dwTID, HANDLE hThread)
	{
		CallStackTls * pBlock = (struct CallStackTls *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CallStackTls), FALSE);
		pBlock->pLast = NULL;
		pBlock->dwLen = 0;
		pBlock->dwSize = c_dwSamplingBlockSize;
		pBlock->dwTID = dwTID;
		pBlock->pBuf = CMyHeapAlloc::GetInstance()->MyAlloc(c_dwSamplingBlockSize, TRUE);
		pBlock->hThread = hThread;

		return pBlock;
	}

	void CCallStackSampling::AddThread(DWORD dwTID)
	{
		::EnterCriticalSection(&m_cri);

		if (m_mapThread.find(dwTID) == m_mapThread.end())
		{
			//先填NULL，使用时再分配
			//HANDLE hThread = ::OpenThread(THREAD_GET_CONTEXT|THREAD_QUERY_INFORMATION|THREAD_SUSPEND_RESUME, FALSE, dwTID);
			//CallStackTls * pBlock = AllocCallStackBlock(dwTID, hThread);
			//m_vecBlock.push_back(pBlock);

			CallStackTls * pBlock = NULL;
			m_mapThread.insert(CMapThread::value_type(dwTID, pBlock));
		}

		::LeaveCriticalSection(&m_cri);
	}

	DWORD CCallStackSampling::Save(HANDLE hFile)
	{		
		::EnterCriticalSection(&m_cri);
		DWORD dwRet = SaveModuleList(hFile);
		dwRet += SaveCallStackData(hFile);
		::LeaveCriticalSection(&m_cri);
		return dwRet;
	}

	DWORD CCallStackSampling::SaveModuleList(HANDLE hFile)
	{
		DWORD dwRet = 0;

		HMODULE hMods[1024];
		DWORD cbRet = 0;
		if (EnumProcessModules(::GetCurrentProcess(), hMods, sizeof(hMods), &cbRet))
		{
			std::vector<MOD_INFO, CMyStlAlloc<MOD_INFO> > vecModules;
		
			for (unsigned int i = 0; i < cbRet/sizeof(HMODULE); i++)
			{
				TCHAR szPath[MAX_PATH] = {0};
				MODULEINFO modInfo = {0};
				
				if (GetModuleFileName(hMods[i], szPath, _countof(szPath))
					&& GetModuleInformation(::GetCurrentProcess(), hMods[i], &modInfo, sizeof(modInfo)))
				{
					MOD_INFO stMod;
					stMod.dwModuleBase = (DWORD)modInfo.lpBaseOfDll;
					_tcsncpy_s(stMod.szModPath, _countof(stMod.szModPath), szPath, _TRUNCATE);
					
					PIMAGE_DOS_HEADER pDH = (PIMAGE_DOS_HEADER)modInfo.lpBaseOfDll;
					PIMAGE_NT_HEADERS pNtH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew);

					stMod.dwTimeStamp = pNtH->FileHeader.TimeDateStamp;
					stMod.dwImageSize = pNtH->OptionalHeader.SizeOfImage;

					std::wstring strSig = GetModuleIndexString(modInfo.lpBaseOfDll, TRUE);
					_tcsncpy_s(stMod.szSigPdb, _countof(stMod.szSigPdb), strSig.c_str(), _TRUNCATE);

					vecModules.push_back(stMod);
				}
			}

			MODULE_BLOCK_HEAD head;
			head.header.dwSize = sizeof(MODULE_BLOCK_HEAD) + sizeof(MOD_INFO) * vecModules.size();
			head.header.dwFlag = FLAG_E_MODULE;
			head.dwCount = vecModules.size();

			DWORD dwWriten = 0;
			::WriteFile(hFile, &head, sizeof(head), &dwWriten, NULL);
			dwRet += dwWriten;

			for (UINT i = 0; i < vecModules.size(); i++)
			{
				dwWriten = 0;
				::WriteFile(hFile, &vecModules[i], sizeof(MOD_INFO), &dwWriten, NULL);
				dwRet += dwWriten;
			}
		}

		return dwRet;
	}

	DWORD CCallStackSampling::SaveCallStackData(HANDLE hFile)
	{
		DWORD dwWritten = 0;

		CALLSTACK_BLOCK_HEAD stHead;
		stHead.header.dwSize = sizeof(stHead);
		stHead.header.dwFlag = FLAG_E_CALLSTACK;
		stHead.count = m_vecBlock.size();

		for (UINT i = 0; i < m_vecBlock.size(); i++)
			stHead.header.dwSize += sizeof(CALLSTACK_THREAD_HEAD) + m_vecBlock[i]->dwLen;
		::WriteFile(hFile, &stHead, sizeof(stHead), &dwWritten, 0);

		for (UINT i = 0; i < m_vecBlock.size(); i++)
		{
			CALLSTACK_THREAD_HEAD stCallStackThread;
			stCallStackThread.dwTID = m_vecBlock[i]->dwTID;
			stCallStackThread.dwDataSize = m_vecBlock[i]->dwLen;
			
			::WriteFile(hFile, &stCallStackThread, sizeof(stCallStackThread), &dwWritten, 0);
			::WriteFile(hFile, m_vecBlock[i]->pBuf, m_vecBlock[i]->dwLen, &dwWritten, 0);
		}

		return stHead.header.dwSize;
	}

	void CCallStackSampling::CallStack(DWORD dwTID, struct CallStackTls** ppBlock, LONGLONG llPerfCount)
	{
		if (!ppBlock)
			return;

		const static DWORD c_dwMaxNeed = sizeof(CALLSTACK) + c_iMaxLevel * sizeof(DWORD);

		if (!(*ppBlock))
		{	//新线程使用
			HANDLE hThread = ::OpenThread(THREAD_GET_CONTEXT|THREAD_QUERY_INFORMATION|THREAD_SUSPEND_RESUME, FALSE, dwTID);
			*ppBlock = AllocCallStackBlock(dwTID, hThread);
		}
		else if ((*ppBlock)->dwLen + c_dwMaxNeed > (*ppBlock)->dwSize) //内存不够
			*ppBlock = AllocCallStackBlock((*ppBlock)->dwTID, (*ppBlock)->hThread);

		struct CallStackTls *pBlock = *ppBlock;

		BYTE buf[c_dwMaxNeed] = {0};
		
		CONTEXT ct = {0};
		ct.ContextFlags = CONTEXT_CONTROL|CONTEXT_SEGMENTS|CONTEXT_INTEGER;
		if (GetThreadContext(pBlock->hThread, &ct))
		{
			int n = GetStack(pBlock->hThread, &ct, buf, c_iMaxLevel);

			//看看能否压缩
			if (pBlock->pLast 
				&& pBlock->pLast->dwCount == n
				&& memcmp((BYTE*)pBlock->pLast + sizeof(CALLSTACK), buf,  n * sizeof(DWORD)) == 0)
			{
				pBlock->pLast->dwRepeat++;
			}
			else
			{
				/*
				PROCESS_MEMORY_COUNTERS status;
				status.cb = sizeof(status);		
				::GetProcessMemoryInfo(::GetCurrentProcess(), &status, sizeof(status));

				DWORD64 dw64CreateTime = 0;
				DWORD64 dw64ExitTime = 0;
				DWORD64 dw64KernelTime = 0;
				DWORD64 dw64UserTime = 0;
				GetThreadTimes(pBlock->hThread, (LPFILETIME)&dw64CreateTime, (LPFILETIME)&dw64ExitTime, (LPFILETIME)&dw64KernelTime, (LPFILETIME)&dw64UserTime);
				*/

				struct CALLSTACK *pCallStack = (CALLSTACK *)((BYTE *)pBlock->pBuf + pBlock->dwLen);
				if (llPerfCount != 0)
					pCallStack->llPerfCount = llPerfCount;
				else
					::QueryPerformanceCounter((LARGE_INTEGER*)&pCallStack->llPerfCount);
				//pCallStack->dwWorkingSet = status.WorkingSetSize;
				//pCallStack->dwPagefileUsage = status.PagefileUsage;
				//pCallStack->dw64KernelTime = dw64KernelTime;
				//pCallStack->dw64UserTime = dw64UserTime;
				pCallStack->dwCount = n;
				pCallStack->dwRepeat = 1;

				memcpy(pCallStack + 1, buf,  n * sizeof(DWORD));

				pBlock->dwLen += sizeof(CALLSTACK) + n * sizeof(DWORD);
				pBlock->pLast = pCallStack;
			}
		}
	}

	void CCallStackSampling::CallStackSampling(LONGLONG llPerfCount)
	{
		if (!CControl::GetInstance()->IsSamplingCallStack())
			return;

		::EnterCriticalSection(&m_cri);
		//const CMapThread mapCopy = m_mapThread;

		for (CMapThread::const_iterator it = m_mapThread.begin(); it != m_mapThread.end(); ++it)
		{
			struct CallStackTls *pBlock = it->second, *pPreBlock = pBlock;

			CallStack(it->first, &pBlock, llPerfCount);
			if (pBlock != pPreBlock)
			{
				m_vecBlock.push_back(pBlock);
				m_mapThread[it->first] = pBlock;
			}
		}
		::LeaveCriticalSection(&m_cri);
	}
}
