#pragma once

#include <windows.h>
#include "Bitmap.h"

namespace Data
{
	class CBitStore
	{
	public:
		CBitStore() { m_pBitmap = NULL; m_dwBitCount = 0; }
		~CBitStore(void) { clear(); }

		void init(DWORD dwBitCount)
		{
			clear();

			DWORD dwSize = ((dwBitCount + 31) >> 5) << 2;
			m_pBitmap = new BYTE [dwSize];
			memset(m_pBitmap, 0, dwSize);

			m_dwBitCount = dwBitCount;
		}

		inline void clear() { delete [] m_pBitmap; m_pBitmap = NULL; m_dwBitCount = 0; }
		inline BOOL isInitialized() const { return m_pBitmap != NULL; }
		inline DWORD getMemSize() const  { return ((m_dwBitCount + 31) >> 5) << 2; }
		inline const void * getMemPtr() const { return m_pBitmap; }
		
		inline void setBit(UINT nIndex, BOOL bSet)
		{
			assert(nIndex < m_dwBitCount);
			if (nIndex < m_dwBitCount)
				Bitmap::setBit(m_pBitmap, nIndex, bSet);
		}

		inline BOOL getBit(UINT nIndex) const
		{
			assert(nIndex < m_dwBitCount);
			if (nIndex < m_dwBitCount)
				return Bitmap::getBit(m_pBitmap, nIndex);
		
			return FALSE;
		}

		inline BOOL isAllZero() const
		{
			return Bitmap::isAllZero(m_pBitmap, getMemSize());
		}

		CBitStore & operator = (const CBitStore &right)
		{
			delete [] m_pBitmap; 
			m_pBitmap = new BYTE[right.getMemSize()];
			memcpy(m_pBitmap, right.getMemPtr(), right.getMemSize());

			m_dwBitCount = right.m_dwBitCount;

			return *this;
		}

	private:

		BYTE *	m_pBitmap;
		DWORD	m_dwBitCount;	
	};
}


