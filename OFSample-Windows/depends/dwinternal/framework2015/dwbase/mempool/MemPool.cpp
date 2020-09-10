/** 
@file
@brief 内存池的实现
@version 2011-9-26 bobdeng
*/


#include "stable.h"
#include <assert.h>
#include <malloc.h>
#include <tchar.h>
#include <stdlib.h>
#include "../perflog/stlalloc.h"

#include "MemPool.h"

#define NUM_BIN			32
#define NUM_CHUNK		1024 * 8 //不够则从堆中分配
#define GAP_FILL		0xCDAB
#define SIZE_CHUNK		(1024 * 32)
#define SIZE_REGION		(1024 * 512)

#define BINMAP_INDEX(h)			(h >> 1)
#define SET_BINMAP_INDEX(h, i)	(h = (h & 1) | (i << 1))  
#define IS_FREE(h)				(BOOL)(h & 1)

#define SET_FREE(h)				(h |= 1)
#define SET_BUSY(h)				(h &= 0xFFFE)

#define ORDER_BLOCK

#define MALLOC(size)			malloc(size)
#define REMALLOC(p, newsize)	realloc(p, newsize)
#define FREE(p)					free(p)
#define MSIZE(p)				_msize(p)

#pragma pack(push, 2)
struct BLOCK
{
	WORD	wHeadFlag;
	BLOCK *	pNext;
};
#pragma pack(pop)

struct BIN
{
	UINT	uBlockSize;
	BLOCK *	pFirstBlock;
};

struct CHUNK_MAP
{
	void *	pAddr;
	BIN	*	pBin;

#ifdef ORDER_BLOCK
	BLOCK * pFirstFreeBlock;
#endif
};

static struct MALLOC_STATE {
	CRITICAL_SECTION    cri;
	BIN					bin[NUM_BIN];
	CHUNK_MAP			chunkmap[NUM_CHUNK];
	volatile UINT		uChunkCount;
} s_state = {0};

static bool s_bDisable = false;
static bool s_DiagMode = false;

void mp_trace_alloc(const void * pMem, DWORD dwBlockSize, DWORD dwUserSize);
void mp_trace_free(const void * pMem);

static void	mp_init_check()
{
	static BOOL s_init = FALSE;
	if (!s_init)
	{
		::InitializeCriticalSection(&s_state.cri);
		
		TCHAR szCmdLine[MAX_PATH] = {0};
		LPTSTR lpszCmd = GetCommandLine();
		_tcsncpy_s(szCmdLine, _countof(szCmdLine), lpszCmd, _TRUNCATE);
		_tcslwr_s(szCmdLine, _countof(szCmdLine));
		if (_tcsstr(szCmdLine, _T("/disablemempool")) != 0)
			s_bDisable = true;

		s_init = TRUE;
	}
}

static void * mp_alloc_region(UINT uSize)
{
	HANDLE hMapFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, uSize, NULL);
	return ::MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, uSize);
}

static void * mp_alloc_chunk(UINT uSize)
{
	assert(uSize <= SIZE_REGION);

	static void * s_pRegion = mp_alloc_region(SIZE_REGION);
	static UINT	s_uLen = 0;

	if (uSize > SIZE_REGION - s_uLen)
	{
		s_pRegion = mp_alloc_region(SIZE_REGION);
		s_uLen = 0;
	}

	void *pRet = (BYTE *)s_pRegion + s_uLen;
	s_uLen += uSize;
	return pRet;
}

static void	mp_error(const TCHAR* lpszFmt, ...)
{
	va_list args;
	va_start(args, lpszFmt);
	TCHAR szTemp[1024 * 4] = {0};
	_vsnwprintf_s(szTemp, _countof(szTemp) - 1, _TRUNCATE, lpszFmt, args);

#ifndef OFFICIAL_BUILD
	::MessageBox(NULL, szTemp, _T("mem pool error dectect!"), MB_SERVICE_NOTIFICATION);
#endif

	::DebugBreak();
	va_end(args);
}

void *	mp_alloc(UINT uSize)
{
	mp_init_check();

	void *pRet = NULL;

	if (s_bDisable)
	{
		pRet = MALLOC(uSize);
	}
	else
	{
		if (uSize == 0)
			uSize = 1;

		UINT uBinIndex;
		UINT uBlockSize;
		
		if (uSize <= 12)
		{
			uBinIndex = 0;
			uBlockSize = 12;
		}
		else if (uSize <= 36)
		{
			UINT uIndexOffset = ((uSize - 12 - 1) >> 3);
			uBinIndex = 1 + uIndexOffset;
			uBlockSize = 12 + ((uIndexOffset + 1) << 3);
		}
		else if (uSize <= 100)
		{
			UINT uIndexOffset = ((uSize - 36 - 1) >> 4);
			uBinIndex = 4 + uIndexOffset;
			uBlockSize = 36 + ((uIndexOffset + 1) << 4);
		}
		else if (uSize <= 260)
		{
			UINT uIndexOffset = ((uSize - 100 - 1) >> 5);
			uBinIndex = 8 + uIndexOffset;
			uBlockSize = 100 + ((uIndexOffset + 1) << 5);
		}
		else if (uSize <= 580)
		{
			UINT uIndexOffset = ((uSize - 260 - 1) >> 6);
			uBinIndex = 13 + uIndexOffset;
			uBlockSize = 260 + ((uIndexOffset + 1) << 6);
		}
		else if (uSize <= 1220)
		{
			UINT uIndexOffset = ((uSize - 580 - 1) >> 7);
			uBinIndex = 18 + uIndexOffset;
			uBlockSize = 580 + ((uIndexOffset + 1) << 7);
		}
		else 
		{
			uBinIndex = -1;
			uBlockSize = -1;
		}


		if (uBinIndex != -1)
		{
			::EnterCriticalSection(&s_state.cri);

			assert(uBinIndex < _countof(s_state.bin));
			assert(uSize <= uBlockSize && uBlockSize % 4 == 0 && uBlockSize % 8 != 0);//区间必须4字节对齐，不能8字节对齐，这是为了保证分配出来给用户的内存8字节对齐
			
			//是否需要分配一个CHUNK
			if (!s_state.bin[uBinIndex].pFirstBlock)
			{
				assert(s_state.uChunkCount <= _countof(s_state.chunkmap));
				if (s_state.uChunkCount < _countof(s_state.chunkmap))
				{					
					void *pChunk = mp_alloc_chunk(SIZE_CHUNK);
					memset(pChunk, 0xcc, SIZE_CHUNK);

					BLOCK * pLast = NULL;
					for (BYTE * p = (BYTE *)pChunk + sizeof(WORD) + 4; p + sizeof(WORD) * 2 + uBlockSize < (BYTE *)pChunk + SIZE_CHUNK; p += sizeof(WORD) * 2 + uBlockSize)
					{
						BLOCK * pBlock = (BLOCK *)p;

						assert(((DWORD64)pBlock + sizeof(WORD)) % 8 == 0);//分配给用户的地址应该8字节对齐

						if (!pLast)
							pLast = s_state.bin[uBinIndex].pFirstBlock = pBlock;
						else
							pLast = pLast->pNext = pBlock;
						
						pLast->pNext = NULL;

						SET_BINMAP_INDEX(pLast->wHeadFlag, s_state.uChunkCount);
						SET_FREE(pLast->wHeadFlag);
						*(WORD*)((BYTE *)pLast + sizeof(WORD) + uBlockSize) = GAP_FILL;
					}
					
					s_state.chunkmap[s_state.uChunkCount].pAddr = pChunk;
					s_state.chunkmap[s_state.uChunkCount].pBin = &s_state.bin[uBinIndex];

#ifdef ORDER_BLOCK
					assert(s_state.chunkmap[s_state.uChunkCount].pFirstFreeBlock == NULL);
					s_state.chunkmap[s_state.uChunkCount].pFirstFreeBlock = s_state.bin[uBinIndex].pFirstBlock;
#endif

					if (s_state.bin[uBinIndex].uBlockSize == 0)
						s_state.bin[uBinIndex].uBlockSize = uBlockSize;
					else
						assert(s_state.bin[uBinIndex].uBlockSize == uBlockSize);

					s_state.uChunkCount++;
				}
			}

			assert(s_state.bin[uBinIndex].pFirstBlock);//一般来说，应该够了
			if (s_state.bin[uBinIndex].pFirstBlock)
			{
				BLOCK *pBlockRet = s_state.bin[uBinIndex].pFirstBlock;
				s_state.bin[uBinIndex].pFirstBlock = pBlockRet->pNext;

				if (!IS_FREE(pBlockRet->wHeadFlag) || *(WORD *)((const BYTE*)pBlockRet + sizeof(WORD) + uBlockSize) != GAP_FILL)
					mp_error(_T("mp_alloc - detect block 0x%08X guard corrupt!"), (const BYTE*)pBlockRet + sizeof(WORD));
				
				SET_BUSY(pBlockRet->wHeadFlag);
				*(WORD*)((BYTE *)pBlockRet + sizeof(WORD) + uBlockSize) = GAP_FILL;

				pRet = (BYTE *)pBlockRet + sizeof(WORD);
			
#ifdef ORDER_BLOCK
				//更新Chunk的pFirstFreeBlock
				UINT uChunkIndex = BINMAP_INDEX(pBlockRet->wHeadFlag);	
				assert(uChunkIndex < s_state.uChunkCount);
				if (uChunkIndex < s_state.uChunkCount && (BYTE *)pBlockRet >= (BYTE *)s_state.chunkmap[uChunkIndex].pAddr && (BYTE *)pBlockRet < (BYTE *)s_state.chunkmap[uChunkIndex].pAddr + SIZE_CHUNK)
				{
					if (s_state.chunkmap[uChunkIndex].pFirstFreeBlock == pBlockRet)
					{
						if (pBlockRet->pNext)
						{
							if (uChunkIndex == BINMAP_INDEX(pBlockRet->pNext->wHeadFlag))
								s_state.chunkmap[uChunkIndex].pFirstFreeBlock = pBlockRet->pNext;
							else
								s_state.chunkmap[uChunkIndex].pFirstFreeBlock = NULL;
						}
						else
						{
							s_state.chunkmap[uChunkIndex].pFirstFreeBlock = NULL;
						}
					}
				}
				else
				{
					mp_error(_T("mp_alloc - detect block 0x%08X header corrupt!"), (const BYTE*)pBlockRet + sizeof(WORD));
				}
#endif

				//For诊断
				if (s_DiagMode)
				{
					memset(pRet, 0xcc, uBlockSize);
					mp_trace_alloc(pRet, uBlockSize, uSize); 
				}
				else
				{
#ifdef _DEBUG
					memset(pRet, 0xcc, uBlockSize);
#endif
				}

				assert((DWORD64)pRet % 8 == 0);//分配给用户的地址应该8字节对齐
			}

			::LeaveCriticalSection(&s_state.cri);
		}
		
		//如果内存池已经满了，则从默认分配器中分配
		if (!pRet)
		{
			pRet = MALLOC(uSize);
		}
	}

	return pRet;
}

static BOOL	mp_is_mempool_block(const void * pMem)
{
	BOOL bRet = FALSE;

	if (pMem)
	{
		if ((((DWORD)pMem) & 0x3) == 0 && !IsBadReadPtr((BYTE *)pMem - 4, 4))
		{
			const BLOCK *pBlock = (BLOCK*)((BYTE *)pMem - sizeof(WORD));

			UINT uChunkIndex = BINMAP_INDEX(pBlock->wHeadFlag);						
			if (uChunkIndex < s_state.uChunkCount && (BYTE *)pBlock >= (BYTE *)s_state.chunkmap[uChunkIndex].pAddr && (BYTE *)pBlock < (BYTE *)s_state.chunkmap[uChunkIndex].pAddr + SIZE_CHUNK)
				bRet = TRUE;
		}
	}
	return bRet;
}

void	mp_free(void * pMem)
{
	mp_init_check();

	if (pMem)
	{	
		if (s_bDisable)
		{
			FREE(pMem);
		}
		else
		{
			::EnterCriticalSection(&s_state.cri);
			
			if (mp_is_mempool_block(pMem))
			{
				BLOCK *pBlock = (BLOCK*)((BYTE *)pMem - sizeof(WORD));
				UINT uChunkIndex = BINMAP_INDEX(pBlock->wHeadFlag);	
				
				if ((BYTE *)pBlock >= (BYTE *)s_state.chunkmap[uChunkIndex].pAddr && (BYTE *)pBlock < (BYTE *)s_state.chunkmap[uChunkIndex].pAddr + SIZE_CHUNK)
				{
					if (IS_FREE(pBlock->wHeadFlag))
					{
						mp_error(_T("mp_free - repeat free block 0x%08X!"), pMem);
					}
					else
					{
						BIN *pBin = s_state.chunkmap[uChunkIndex].pBin;

						UINT uBlockSize = pBin->uBlockSize;
						if (*(WORD *)((const BYTE*)pBlock + sizeof(WORD) + uBlockSize) != GAP_FILL)
							mp_error(_T("mp_free - detect block 0x%08X guard corrupt!"), pMem);
						
						if (s_DiagMode)
						{
							memset(pMem, 0xdd, uBlockSize);
							mp_trace_free(pMem);
						}
						else
						{
#ifdef _DEBUG
							memset(pMem, 0xdd, uBlockSize);
#endif
						}

						SET_FREE(pBlock->wHeadFlag);
						*(WORD *)((BYTE *)pBlock + sizeof(WORD) + uBlockSize) = GAP_FILL;

#ifdef ORDER_BLOCK
						//插入空闲块列表，实现更好的空间局部性
						if (s_state.chunkmap[uChunkIndex].pFirstFreeBlock != NULL)
						{
							assert(IS_FREE(s_state.chunkmap[uChunkIndex].pFirstFreeBlock->wHeadFlag));
							
							pBlock->pNext = s_state.chunkmap[uChunkIndex].pFirstFreeBlock->pNext;
							s_state.chunkmap[uChunkIndex].pFirstFreeBlock->pNext = pBlock;		
						}
						else
						{
							pBlock->pNext = pBin->pFirstBlock;
							pBin->pFirstBlock = pBlock;

							s_state.chunkmap[uChunkIndex].pFirstFreeBlock = pBlock;
						}
#else
						pBlock->pNext = pBin->pFirstBlock;
						pBin->pFirstBlock = pBlock;
#endif
					}
				}
				else
				{
					mp_error(_T("mp_free - block 0x%08X not in mem pool!"), pMem);				
				}
			}
			else
			{
				FREE(pMem);
			}
			
			::LeaveCriticalSection(&s_state.cri);
		}
	}
}

unsigned int mp_size(const void * pMem)
{
	mp_init_check();

	UINT uSize = 0;

	if (pMem)
	{
		if (s_bDisable)
		{
			uSize = MSIZE((void*)pMem);
		}
		else
		{
			::EnterCriticalSection(&s_state.cri);

			if (mp_is_mempool_block(pMem))
			{	
				const BLOCK *pBlock = (BLOCK*)((BYTE *)pMem - sizeof(WORD));

				UINT uChunkIndex = BINMAP_INDEX(pBlock->wHeadFlag);	
				
				if ((BYTE *)pBlock >= (BYTE *)s_state.chunkmap[uChunkIndex].pAddr && (BYTE *)pBlock < (BYTE *)s_state.chunkmap[uChunkIndex].pAddr + SIZE_CHUNK)
				{
					uSize = s_state.chunkmap[uChunkIndex].pBin->uBlockSize;
				}
				else
				{
					mp_error(_T("mp_size - block 0x%08X not in mem pool!"), pMem);
					uSize = MSIZE((void*)pMem);
				}
			}
			else
			{
				uSize = MSIZE((void*)pMem);
			}

			::LeaveCriticalSection(&s_state.cri);
		}
	}

	return uSize;
}

void *	mp_realloc(void * pMem, UINT uNewSize)
{
	mp_init_check();

	void *pRet = NULL;

	if (s_bDisable)
	{
		pRet = REMALLOC(pMem, uNewSize);
	}
	else
	{
		if (pMem)
		{
			::EnterCriticalSection(&s_state.cri);

			if (mp_is_mempool_block(pMem))
			{
				BLOCK *pBlock = (BLOCK*)((BYTE *)pMem - sizeof(WORD));
				UINT uChunkIndex = BINMAP_INDEX(pBlock->wHeadFlag);	
				
				if ((BYTE *)pBlock >= (BYTE *)s_state.chunkmap[uChunkIndex].pAddr && (BYTE *)pBlock < (BYTE *)s_state.chunkmap[uChunkIndex].pAddr + SIZE_CHUNK)
				{
					UINT uBlockSize = s_state.chunkmap[uChunkIndex].pBin->uBlockSize;
					if (uBlockSize >= uNewSize)
					{
						if (IS_FREE(pBlock->wHeadFlag) || *(WORD *)((const BYTE*)pBlock + sizeof(WORD) + uBlockSize) != GAP_FILL)
							mp_error(_T("mp_realloc - detect block 0x%08X guard corrupt!"), pMem);

						SET_BUSY(pBlock->wHeadFlag);
						*(WORD *)((const BYTE*)pBlock + sizeof(WORD) + uBlockSize) = GAP_FILL;		

						pRet = pMem;
					}
				}
				else
				{
					mp_error(_T("mp_realloc - block 0x%08X not in mem pool!"), pMem);
				}
			}
			else
			{
				pRet = REMALLOC(pMem, uNewSize);
			}

			::LeaveCriticalSection(&s_state.cri);
		}

		if (!pRet)
		{
			void *pNew = mp_alloc(uNewSize);

			if (pMem)
			{
				memcpy(pNew, pMem, mp_size(pMem));
				mp_free(pMem);
			}

			pRet = pNew;
		}
	}

	return pRet;
}


//用于跟踪和调试内存相关错误
/******************************************/
struct CALLSTACK
{
	const void *	pMem;
	DWORD			dwBlockSize;
	DWORD			dwUserSize;
	DWORD			dwCount;
};

typedef std::map<DWORD, CALLSTACK *, std::less<DWORD>, CMyStlAlloc<std::pair<DWORD, CALLSTACK *> > > CMapTrace;
static CMapTrace s_mapTrace;

static int GetStack(void *_ebp, void *buf, int nMaxLevel)
{
	unsigned char *pbBuf = (unsigned char *)buf;
	DWORD dwStackHigh, dwStackLow;
	__asm
	{
		push edx

		mov edx,fs:[4];
		mov dwStackHigh,edx;
		mov edx,fs:[8];
		mov dwStackLow,edx;

		pop edx
	}

    PDWORD p_frame = (PDWORD)_ebp;
	int i = 0;
	while (i < nMaxLevel)
    {
		if (p_frame > (DWORD*)dwStackHigh || p_frame < (DWORD*)dwStackLow)
			break;	
		DWORD ret = p_frame[1];
		if (ret == 0)
			break;
		
		*(DWORD*)pbBuf = ret;
		pbBuf += 4;
		i++;

		PDWORD p_prevFrame = p_frame;
        p_frame = (PDWORD)p_frame[0];

		if (p_frame > (DWORD*)dwStackHigh || p_frame < (DWORD*)dwStackLow)
			break;
		
        if ((DWORD)p_frame & 3)    // Frame pointer must be aligned on a
            break;                  // DWORD boundary.  Bail if not so.
		
        if (p_frame <= p_prevFrame)
            break;
    }
	return i;
}

static void mp_trace_alloc(const void * pMem, DWORD dwBlockSize, DWORD dwUserSize)
{
	void *__ebp;
	
	_asm
	{
		push edx
		mov edx, [ebp]
		mov __ebp, edx
		pop edx
	}

	const DWORD c_iMaxLevel = 64;
	const DWORD c_dwMaxNeed = sizeof(CALLSTACK) + c_iMaxLevel * sizeof(DWORD);
		
	HANDLE hThread = ::GetCurrentThread();
	CONTEXT ct = {0};
	ct.ContextFlags = CONTEXT_CONTROL|CONTEXT_SEGMENTS|CONTEXT_INTEGER;
	if (GetThreadContext(hThread, &ct))
	{
		BYTE buf[c_dwMaxNeed] = {0};
		int n = GetStack(__ebp, buf, c_iMaxLevel);

		CALLSTACK *pCallStack = (CALLSTACK *)CMyHeapAlloc::GetInstance()->MyAlloc(c_dwMaxNeed, FALSE);
		pCallStack->dwCount = n;
		pCallStack->pMem = pMem;
		pCallStack->dwBlockSize = dwBlockSize;
		pCallStack->dwUserSize = dwUserSize;

		memcpy(pCallStack + 1, buf,  n * sizeof(DWORD));
		
		s_mapTrace[(DWORD)pMem] = pCallStack;
	}
}

static void mp_trace_free(const void * pMem)
{
	CMapTrace::iterator it = s_mapTrace.find((DWORD)pMem);
	if (it != s_mapTrace.end())
	{
		CMyHeapAlloc::GetInstance()->MyFree(it->second);
		s_mapTrace.erase(it);
	}
}

static void mp_output_debug(const TCHAR* lpszFmt, ...)
{
	va_list args;
	va_start(args, lpszFmt);
	
	TCHAR szTemp[4096] = {0};
	_vsntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, lpszFmt, args);
	_tcsncat_s(szTemp, _countof(szTemp), _T("\r\n"), _TRUNCATE);
	OutputDebugString(szTemp);
	
	va_end(args);
}

__declspec(dllexport) void mp_dump_block(const void * pMem)
{
	CMapTrace::const_iterator it = s_mapTrace.find((DWORD)pMem);
	if (it != s_mapTrace.end())
	{
		const CALLSTACK *pCallStack = it->second;
		mp_output_debug(_T("BlockSize=%d, UserSize=%d\r\nCallStack(0x%08x):"), pCallStack->dwBlockSize, pCallStack->dwUserSize, pCallStack + 1);
		for (UINT i = 0; i < pCallStack->dwCount; i++)
			mp_output_debug(_T("0x%08x"), ((DWORD *)(pCallStack + 1))[i]);
	}
	else
	{
		mp_output_debug(_T("Block 0x%08x not found"), pMem);
	}
}

__declspec(dllexport) void mp_dump()
{
	struct BIN_INFO
	{
		UINT uBlockCount;
		UINT uAllUserSize;
	};
	typedef std::map<DWORD, BIN_INFO, std::less<DWORD>, CMyStlAlloc<std::pair<DWORD, BIN_INFO> > > CMapBin;

	CMapBin mapBin;
	for (CMapTrace::const_iterator it = s_mapTrace.begin(); it != s_mapTrace.end(); ++it)
	{
		CMapBin::iterator it_bin = mapBin.find(it->second->dwBlockSize);
		if (it_bin != mapBin.end())
		{
			it_bin->second.uBlockCount++;
			it_bin->second.uAllUserSize += it->second->dwUserSize;
		}
		else
		{
			BIN_INFO bin;
			bin.uBlockCount = 1;
			bin.uAllUserSize = it->second->dwUserSize;

			mapBin.insert(CMapBin::value_type(it->second->dwBlockSize, bin));
		}
	}

	mp_output_debug(_T("dump mempool info:"));
	for (CMapBin::iterator it = mapBin.begin(); it != mapBin.end(); ++it)
	{
		mp_output_debug(_T("bin-size=%d, count=%d, all-size=%d, user-size=%d, rate=%0.2f"),
			it->first, 
			it->second.uBlockCount,
			it->first * it->second.uBlockCount,
			it->second.uAllUserSize,
			(double)it->second.uAllUserSize / (it->first * it->second.uBlockCount));
	}
}

/******************************************/