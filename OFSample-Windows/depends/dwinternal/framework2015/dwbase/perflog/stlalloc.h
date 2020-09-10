/** 
@file
@brief stlÄÚ´æ·ÖÅäÆ÷
@version 2011-8-1 bobdeng
*/

#pragma once 

#include <limits>

class CMyHeapAlloc
{
public:
	static CMyHeapAlloc * GetInstance();
	
	void * MyAlloc(DWORD dwSize, BOOL bZeroMemory = false);
	void MyFree(void *p);

private:
	CMyHeapAlloc() { m_hHeap = ::HeapCreate(0, 0, 0); }

	HANDLE		m_hHeap;
};

template <typename T>
struct CMyStlAlloc 
{
	typedef unsigned int size_type, difference_type;
	typedef T value_type;
	typedef T *pointer, &reference;
	typedef const T *const_pointer, &const_reference;

	template <typename U>
	struct rebind { typedef CMyStlAlloc <U> other; };

	CMyStlAlloc() {}

	template <typename U>
	CMyStlAlloc(const CMyStlAlloc <U> &) {}

	~CMyStlAlloc() {}

	bool operator == (const CMyStlAlloc<T> &) const { return TRUE; }
	pointer address(reference r) const { return &r; }
	const_pointer address(const_reference r) const { return &r; }
	size_type max_size() const 
	{
#ifdef max
#undef max
		return std::numeric_limits<size_type>::max();
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
	}
	void construct(pointer p, const T &t) { new ((void *)p) T(t); }
	void destroy(pointer p) { DW_UNUSED(p);p->~T(); }
	pointer allocate(size_type n) { return (pointer) CMyHeapAlloc::GetInstance()->MyAlloc(n * sizeof(value_type), FALSE); }
	void deallocate(pointer p, int) { CMyHeapAlloc::GetInstance()->MyFree((void *)p); }
};
