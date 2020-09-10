#pragma once

#include "dwglobal.h"

namespace DwUtility
{
	class DwCSLock
	{
	public:
		DwCSLock()
		{
			InitializeCriticalSection(&m_criticalSection);
		}

		~DwCSLock()
		{
			DeleteCriticalSection(&m_criticalSection);
		}

		void lock()
		{
			EnterCriticalSection(&m_criticalSection);
		}

		void unlock()
		{
			LeaveCriticalSection(&m_criticalSection);
		}

		bool tryLock()
		{
			return TryEnterCriticalSection(&m_criticalSection) ? true : false;
		}

	private:
		CRITICAL_SECTION m_criticalSection;
	};

	class DwAutoCSLocker
	{
	public:
		explicit DwAutoCSLocker(DwCSLock *m)
			: m_lock(m)
		{
			m_lock->lock();
		}

		~DwAutoCSLocker()
		{
			m_lock->unlock();
			m_lock = NULL;
		}

	private:
		DwCSLock *m_lock;
	};

	class DW_DWUTILITY_EXPORT DwMutex
	{
	public:
		DwMutex();
		DwMutex(LPCWSTR mutexName);
		~DwMutex();

		void lock();
		void unlock();
		void init(LPCWSTR mutexName);

	public:
		static SECURITY_ATTRIBUTES* getAllAccessSecurityAttributes();

	private:
		HANDLE m_hMutex;

	};

	class DW_DWUTILITY_EXPORT DwScopeMutex
	{
	public:
		explicit DwScopeMutex(DwMutex *m);
		~DwScopeMutex();

	private:
		DwMutex *m_mutex;
	};

	class DW_DWUTILITY_EXPORT DWAutoScopeMutex
	{
	public:
		explicit DWAutoScopeMutex(LPCWSTR mutexName);
		~DWAutoScopeMutex();

	private:
		HANDLE m_hMutex;
	};
}

#define DW_AUTO_LOCKER(x) DwUtility::DwAutoCSLocker ___auto_locker_blabla___(x);
#define DW_SCOPEMUTEX(x) DwUtility::DwScopeMutex __scope_mutex_blabal__(x);
#define DW_AUTO_SCOPEMUTEX(x) DwUtility::DWAutoScopeMutex __autoscope_mutex_blabal__(x);
