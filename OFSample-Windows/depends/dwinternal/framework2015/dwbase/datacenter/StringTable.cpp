#include "stable.h"

#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>

#include "StringTable.h"
#include "..\mempool\MemPool.h"
#include "CriticalSection.h"
#include "IData.h"

const UINT DATA_NUM		= 256;
const UINT BLOCK_NUM	= 256;
const UINT HASH_MODE	= 0x7FFF; 
const UINT BUCKET_NUM	= HASH_MODE + 1;

#define LOWADDR(W)		((W) & 0x00FF)
#define HIGHADDR(W)		(((W) & 0xFF00) >> 8)

#define INVALID_ADDR	INVALID_NAME_ID	 //无效的table地址 

typedef struct ENTRY 
{
	WORD	nLen;
	WORD	nNext;
	LPTSTR	pszName;

	void Construct()
	{
		nLen	= 0;
		nNext	= INVALID_ADDR;
		pszName = NULL;
	}

}*PENTRY;

class CStringTable
{
private:
	CStringTable(void);
	~CStringTable(void);

public:
	WORD QueryNameId(LPCTSTR pszSrc);
	WORD InsertNameId(LPCTSTR pszSrc);
	inline LPCTSTR QueryName(WORD nNameId);

	static CStringTable* GetInstance();
	WORD HashString(LPCTSTR pszSrc);
	void FreeTable();

private:
	void InitTable();

	void AllocBlock(WORD nHighAddr);
	WORD QueryNameId(LPCTSTR pszSrc, WORD nBucket);
	WORD AttachNewAddr(LPCTSTR pszSrc, WORD nBucket);

private:
	WORD			m_nCurAddr;
	WORD*			m_pHashAddr;			//Hash首地址表(逻辑地址)
	PENTRY*			m_pTblBlock;			//Block首地址表(物理地址)
	CCriticalLock	m_csLock;				//数据同步锁
};

extern "C"  WORD query_name_id(LPCTSTR pszSrc)
{
	return CStringTable::GetInstance()->QueryNameId(pszSrc);
}

extern "C"  WORD insert_name_id(LPCTSTR pszSrc)
{
	return CStringTable::GetInstance()->InsertNameId(pszSrc);
}

extern "C"  LPCTSTR query_name(WORD nNameId)
{
	return CStringTable::GetInstance()->QueryName(nNameId);
}

void free_string_table()
{
	CStringTable::GetInstance()->FreeTable();
}

CStringTable::CStringTable(void)
{
	InitTable();
}

CStringTable::~CStringTable(void)
{
	FreeTable();
}

CStringTable* CStringTable::GetInstance()
{
	static CStringTable* s_pInstance = NULL;
	if (!s_pInstance)
	{
#ifdef _DEBUG
		int nOldFlag = _CrtSetDbgFlag(0);
#endif
		s_pInstance = new CStringTable();

#ifdef _DEBUG
		_CrtSetDbgFlag(nOldFlag);
#endif	
	}

	return s_pInstance;
}

void CStringTable::InitTable()
{
	m_nCurAddr = 0;

	UINT nSize = sizeof(WORD) * BUCKET_NUM;
	m_pHashAddr = (WORD *)mp_alloc(nSize);
	memset(m_pHashAddr, 0xFF, nSize);
	
	nSize = sizeof(PENTRY) * BLOCK_NUM;
	m_pTblBlock = (PENTRY*)mp_alloc(nSize);
	memset(m_pTblBlock, 0x00, nSize);
}

void CStringTable::FreeTable()
{
	mp_free(m_pHashAddr);
	
	for (WORD i = 0; i < BLOCK_NUM; i++)
	{
		if (NULL != m_pTblBlock[i])
		{
			for (WORD k = 0; k < DATA_NUM; k++)
			{
				if (NULL != m_pTblBlock[i][k].pszName)
				{
					mp_free(m_pTblBlock[i][k].pszName);
				}
			}

			mp_free(m_pTblBlock[i]);
		}
	}

	mp_free(m_pTblBlock);
}


void CStringTable::AllocBlock(WORD nHighAddr)
{
	m_pTblBlock[nHighAddr] = (PENTRY)mp_alloc(sizeof(ENTRY) * DATA_NUM);

	for (WORD i = 0; i < DATA_NUM; i++)
	{
		m_pTblBlock[nHighAddr][i].Construct();
	}
}

WORD CStringTable::HashString(LPCTSTR pszSrc)
{
	assert((NULL != pszSrc) && (0 != pszSrc[0]));

	DWORD nHash = 1315423911;
	while (*pszSrc)
	{
		nHash ^= ((nHash << 5) + (*pszSrc++) + (nHash >> 2));
	}

/*
	DWORD nHash = 0;
	DWORD nSeed = 33; // 31 131 1313 13131 131313 etc.. or 33??
	while (*pszSrc)
	{
		nHash = nHash * nSeed + (*pszSrc++);
	}
*/
	return(nHash & HASH_MODE);
}

WORD CStringTable::AttachNewAddr(LPCTSTR pszSrc, WORD nBucket)
{
	//assert((NULL != pszSrc) && (0 != pszSrc[0])); inner invoke, needn't asserting
	
	/*Ensure m_nCurAddr < 65535 (0-65534),address 65535 reserve.*/
	assert(m_nCurAddr < INVALID_ADDR);
	if (INVALID_ADDR == m_nCurAddr)
	{
		return INVALID_ADDR;
	}

	WORD wLow = LOWADDR(m_nCurAddr);
	WORD wHigh = HIGHADDR(m_nCurAddr);

	if (0 == wLow) //Allocate Block
	{
		AllocBlock(wHigh);
	}

	WORD nLen = (WORD) _tcslen(pszSrc);
	m_pTblBlock[wHigh][wLow].nLen = nLen;
	m_pTblBlock[wHigh][wLow].pszName = (LPTSTR) mp_alloc((nLen + 1) * sizeof(TCHAR));

#if defined(_UNICODE) || defined(UNICODE)
	wmemcpy(m_pTblBlock[wHigh][wLow].pszName, pszSrc, nLen + 1);
#else
	memcpy(m_pTblBlock[wHigh][wLow].pszName, pszSrc, nLen + 1);
#endif

	if (INVALID_ADDR != m_pHashAddr[nBucket])
	{
		m_pTblBlock[wHigh][wLow].nNext = m_pHashAddr[nBucket];
	}

	m_pHashAddr[nBucket] = m_nCurAddr++;

	return m_pHashAddr[nBucket]; 
}

WORD CStringTable::InsertNameId(LPCTSTR pszSrc)
{
	assert((NULL != pszSrc) && (0 != pszSrc[0]));
	if((NULL == pszSrc) || (0 == pszSrc[0]))
	{
		return INVALID_NAME_ID;
	}

	WORD nBucket = HashString(pszSrc);

	//统一加锁
	WORD nAddr = INVALID_ADDR;
	{
		CAutoLock autoLock(m_csLock);

		nAddr = QueryNameId(pszSrc, nBucket);
		if (INVALID_ADDR == nAddr)
		{
			nAddr = AttachNewAddr(pszSrc, nBucket);
		}
	}
	//统一加锁

	return nAddr;
}

WORD CStringTable::QueryNameId(LPCTSTR pszSrc, WORD nBucket)
{
	//assert((NULL != pszSrc) && (0 != pszSrc[0])); inner invoke, needn't asserting

	WORD nAddr = m_pHashAddr[nBucket];

	while(INVALID_ADDR != nAddr)
	{
#if defined(_UNICODE) || defined(UNICODE)
		if (!wmemcmp(pszSrc, m_pTblBlock[HIGHADDR(nAddr)][LOWADDR(nAddr)].pszName,
			m_pTblBlock[HIGHADDR(nAddr)][LOWADDR(nAddr)].nLen + 1))
#else
		if (!memcmp(pszSrc, m_pTblBlock[HIGHADDR(nAddr)][LOWADDR(nAddr)].pszName,
			m_pTblBlock[HIGHADDR(nAddr)][LOWADDR(nAddr)].nLen + 1))
#endif
		{
			break;
		}

		nAddr = m_pTblBlock[HIGHADDR(nAddr)][LOWADDR(nAddr)].nNext;
	}

	return nAddr;
}

WORD CStringTable::QueryNameId(LPCTSTR pszSrc)
{
	assert((NULL != pszSrc) && (0 != pszSrc[0]));
	if((NULL == pszSrc) || (0 == pszSrc[0]))
	{
		return INVALID_ADDR;
	}

	WORD nBucket = HashString(pszSrc);

	return QueryNameId(pszSrc, nBucket);
}

inline LPCTSTR CStringTable::QueryName(WORD nNameId)
{
	if(nNameId < m_nCurAddr)
	{
		return m_pTblBlock[HIGHADDR(nNameId)][LOWADDR(nNameId)].pszName;
	}

	return _T("");
}
