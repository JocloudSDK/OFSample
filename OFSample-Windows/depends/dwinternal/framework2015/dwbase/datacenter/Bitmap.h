#pragma once

#include <windows.h>
#include "MemPool/MemPool.h"

namespace Data
{		
	/*---------------------------------------------------------------
	函数说明：
	要求分配的pBitmap起始地址4字节对齐，长度4字节对齐，空白部分必须填0
	----------------------------------------------------------------*/
	namespace Bitmap
	{
		inline void setBit(void *pBitmap, UINT nIndex, BOOL bSet)
		{
			assert((DWORD)pBitmap % 4 == 0);//应该是DWORD对齐

			DWORD *p = (DWORD *)pBitmap + (nIndex >> 5);
			if (bSet)
				*p |= (1 << (nIndex & 0x1F));	
			else
				*p &= ~(1 << (nIndex & 0x1F));
		}

		inline BOOL getBit(const void *pBitmap, UINT nIndex)
		{
			assert((DWORD)pBitmap % 4 == 0);//应该是DWORD对齐

			const DWORD *p = (const DWORD *)pBitmap + (nIndex >> 5);
			return ( (*p) & (1 << (nIndex & 0x1F)) ) != 0;
		}

		inline BOOL isIntersect(const void *pBitmap1, const void *pBitmap2, UINT nMemSize)
		{
			assert(nMemSize % 4 == 0);
			assert((DWORD)pBitmap1 % 4 == 0 && (DWORD)pBitmap2 % 4 == 0);//应该是DWORD对齐

			BOOL bIntersect = FALSE;

			DWORD count = nMemSize >> 2;
			for (UINT i = 0; i < count && !bIntersect; i++)
				bIntersect = ((DWORD *)pBitmap1)[i] & ((DWORD *)pBitmap2)[i];

			return bIntersect;
		}

		inline BOOL isAllZero(const void *pBitmap, UINT nMemSize)
		{
			assert(nMemSize % 4 == 0);
			assert((DWORD)pBitmap % 4 == 0);//应该是DWORD对齐

			BOOL bZero = TRUE;

			DWORD count = nMemSize >> 2;
			for (UINT i = 0; i < count && bZero; i++)
				bZero = ((DWORD *)pBitmap)[i] == 0;

			return bZero;
		}
	}
}


