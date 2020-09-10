#pragma once

#include <list>
#include "MemPool.h"

template<class T, size_t POOL_SIZE, class LOCK>
class DwObjectPool
{
public:
	DwObjectPool()
	{
	}
	~DwObjectPool()
	{
		for (OBJECT_LIST::iterator it = m_cachedObjects.begin();
			it != m_cachedObjects.end(); ++it)
		{
			T* object = *it;
			mp_free(object);
		}
		m_cachedObjects.clear();
	}

public:
	T* allocObject()
	{
		void* object = _allocObjectImpl();
		return new(object)T();
	}
	T* allocObject(const T& o)
	{
		void* object = _allocObjectImpl();
		return new(object)T(o);
	}
	void freeObject(T* o)
	{
		o->~T();
		_freeObjectImpl(o);
	}

private:
	void* _allocObjectImpl()
	{
		void* object = NULL;

		m_lock.lock();
		if (!m_cachedObjects.empty())
		{
			object = m_cachedObjects.front();
			m_cachedObjects.pop_front();
		}
		else
		{
			object = mp_alloc(sizeof(T));
		}
		m_lock.unlock();

		return object;
	}
	void _freeObjectImpl(T* object)
	{
		m_lock.lock();
		if (m_cachedObjects.size() < POOL_SIZE)
		{
			m_cachedObjects.push_back(object);
		}
		else
		{
			mp_free(object);
		}
		m_lock.unlock();
	}

private:
	typedef std::list<T*> OBJECT_LIST;
	OBJECT_LIST m_cachedObjects;
	LOCK m_lock;
};