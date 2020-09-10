#pragma once

#include <new>
#include "dwbase/MemPool.h"

template<class T> inline
void _MemDestroy(T *p)
{	// destroy object at _Ptr
	(void)p;

	p->~T();
}

template<> inline
void _MemDestroy(char *)
{	// destroy a char (do nothing)
}

template<> inline
void _MemDestroy(wchar_t *)
{	// destroy a wchar_t (do nothing)
}

template <class T>
class CMemAlloc
{
public:
	//type definitions
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	//rebind allocator to type U
	template <class U>
	struct rebind
	{
		// convert an MemAlloc<T> to an MemAlloc <U>
		typedef CMemAlloc<U> other;
	};

	//return address of values
	pointer address (reference value) const
	{
		return (&value);
	}

	const_pointer address (const_reference value) const
	{
		return (&value);
	}

	/*constructors and destructor
	*-nothing to do because the allocator has no state
	*/
	CMemAlloc() throw()
	{
	}

	CMemAlloc(const CMemAlloc<T>&) throw()
	{
	}

	template <class U>// idiom: Coercion by Member Template  
	CMemAlloc(const CMemAlloc<U>&) throw()
	{
	}
	
	~CMemAlloc()
	{
	}

	CMemAlloc<T>& operator=(const CMemAlloc<T>&) 
	{ 
		return (*this);
	} 

	template<class U>// idiom: Coercion by Member Template  
	CMemAlloc<T>& operator=(const CMemAlloc<U>&)
	{	
		return (*this);
	}

	//return maximum number of elements that can be allocated
	size_type max_size () const throw()
	{
		size_type _Count = (size_type)(-1) / sizeof (T);

		return (0 < _Count ? _Count : 1);
	}

	//allocate but don't initialize num elements of type T
	pointer allocate (size_type num, const void* hint = 0)
	{
		(void)hint;

		//allocate memory with mempool
		return(pointer) (mp_alloc(num*sizeof(T)));
		//return(pointer) (::operator new(num*sizeof(T)));
	}

	//initialize elements of allocated storage p with value value
	void construct (pointer p, const T& value)
	{
		//initialize memory with placement new
		new((void*)p)T(value);
	}

	//destroy elements of initialized storage p
	void destroy (pointer p)
	{
		// destroy objects by calling their destructor
		//p->~T();
		_MemDestroy(p);
	}

	//deallocate storage p of deleted elements
	void deallocate (pointer p, size_type num)
	{
		(void)num;

		//deallocate memory with global delete
		mp_free((void*)p);
		//::operator delete((void*)p);
	}
};

template<> class CMemAlloc<void>
{	// generic allocator for type void
public:
	typedef void T;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef T value_type;

	template<class U>
	struct rebind
	{	// convert an MemAlloc<void> to an MemAlloc <U>
		typedef CMemAlloc<U> other;
	};

	CMemAlloc() throw()
	{	// construct default allocator (do nothing)
	}

	CMemAlloc(const CMemAlloc<T>&) throw()
	{	// construct by copying (do nothing)
	}

	template<class U>
	CMemAlloc(const CMemAlloc<U>&) throw()
	{	// construct from related allocator (do nothing)
	}

	CMemAlloc<T>& operator=(const CMemAlloc<T>&) 
	{ 
		return (*this);
	} 

	template<class U>
	CMemAlloc<T>& operator=(const CMemAlloc<U>&)
	{	// assign from a related allocator (do nothing)
		return (*this);
	}
};

//return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator== (const CMemAlloc<T1>&,
				 const CMemAlloc<T2>&) throw()
{
	return (true);
}

template <class T1, class T2>
bool operator!= (const CMemAlloc<T1>&,
				 const CMemAlloc<T2>&) throw()
{
	return (false);
}
