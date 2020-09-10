#pragma once
#include <windows.h>

class DwRingObjectPoolEmptyLocker
{
public:
	void lock() {};
	void unlock() {};
};

template<class T, size_t OBJECT_CNT, class LOCK = DwRingObjectPoolEmptyLocker>
class DwRingObjectPool
{
public:
	DwRingObjectPool() : m_curAllocIdx(-1), m_totalBufferFullCnt(0)
	{
		m_ringBuffer = new char[OBJECT_CNT * sizeof(T)];
		m_ringBufferFlags = new char[OBJECT_CNT];
		::memset(m_ringBufferFlags, 0, OBJECT_CNT);
		m_ringObject = (T*)m_ringBuffer;
	}
	~DwRingObjectPool()
	{
		delete[] m_ringBuffer;
		delete[] m_ringBufferFlags;
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
		int curAllocIdx = m_curAllocIdx;

		m_lock.lock();
		++curAllocIdx;
		curAllocIdx %= OBJECT_CNT;
		if (m_ringBufferFlags[curAllocIdx])
		{
			object = malloc(sizeof(T));
			++m_totalBufferFullCnt;
		}
		else
		{
			object = m_ringBuffer + curAllocIdx * sizeof(T);
			m_ringBufferFlags[curAllocIdx] = true;
			m_curAllocIdx = curAllocIdx;
		}
		m_lock.unlock();

		return object;
	}
	void _freeObjectImpl(T* object)
	{
		if ((char*)object < m_ringBuffer 
			|| (char*)object > m_ringBuffer + OBJECT_CNT * sizeof(T))
		{
			free(object);
			return;
		}

		if (((char*)object - m_ringBuffer) % sizeof(T))
		{
			_OutputError(_T("非法位置释放循环缓冲内存！"));
		}
		
		int curReleaseIdx = ((char*)object - m_ringBuffer) / sizeof(T);

		m_lock.lock();
		m_ringBufferFlags[curReleaseIdx] = false;
		m_lock.unlock();

	}

	void _OutputError(const TCHAR* lpszFmt, ...)
	{
		va_list args;
		va_start(args, lpszFmt);

		TCHAR szTemp[4096] = {0};
		_vsntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, lpszFmt, args);
		LogFinal(LOG::KDataCenter) << _T("Error: ") << szTemp;

#ifndef OFFICIAL_BUILD
		::MessageBox(NULL, szTemp, _T("内部开发版本调试错误"), MB_OK|MB_SERVICE_NOTIFICATION);
		::DebugBreak();
#endif

		va_end(args);
	}

protected:
	int m_curAllocIdx;
	int m_totalBufferFullCnt;
	char* m_ringBuffer;
	char* m_ringBufferFlags;
	T* m_ringObject;
	LOCK m_lock;
};