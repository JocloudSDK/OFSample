/** 
@file
@brief stlÄÚ´æ·ÖÅäÆ÷
@version 2011-8-1 bobdeng
*/

#include "stable.h"
#include "stlalloc.h"

CMyHeapAlloc * CMyHeapAlloc::GetInstance() 
{
	static CMyHeapAlloc alloc; 
	return &alloc; 
}

void *CMyHeapAlloc::MyAlloc(DWORD dwSize, BOOL bZeroMemory) 
{
#ifdef _DEBUG
	static DWORD s_dwTotoalSize = 0;
	s_dwTotoalSize += dwSize;
	static DWORD s_dwAllocCount = 0;
	++s_dwAllocCount;
#endif
	return ::HeapAlloc(m_hHeap, bZeroMemory ? HEAP_ZERO_MEMORY : 0, dwSize); 
}

void CMyHeapAlloc::MyFree(void *p) 
{ 
	::HeapFree(m_hHeap, 0, p); 
}
