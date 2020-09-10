#include "dwlocker.h"

namespace DwUtility
{
	DwMutex::DwMutex() : m_hMutex(NULL)
	{
	}

	DwMutex::DwMutex(LPCWSTR mutexName) : m_hMutex(NULL)
	{
		if(mutexName)
		{
			init(mutexName);
		}
	}

	DwMutex::~DwMutex()
	{
		if(m_hMutex)
		{
			ReleaseMutex(m_hMutex);
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
	}


	void DwMutex::init(LPCWSTR mutexName)
	{
		m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexName);

		if(NULL == m_hMutex)
		{
			m_hMutex = CreateMutex(getAllAccessSecurityAttributes(), FALSE, mutexName);
		}
	}

	void DwMutex::lock()
	{
		if(m_hMutex)
		{
			WaitForSingleObject(m_hMutex, INFINITE);
		}
	}

	void DwMutex::unlock()
	{
		if(m_hMutex)
		{
			ReleaseMutex(m_hMutex);
		}
	}

	SECURITY_ATTRIBUTES* DwMutex::getAllAccessSecurityAttributes()
	{
		static BOOL bIsInitialize = FALSE;
		static SECURITY_ATTRIBUTES SecurityAttributes;
		static SECURITY_DESCRIPTOR SecurityDescriptor;

		OSVERSIONINFO VerInfo;  
		VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&VerInfo);
		if(VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			return NULL;
		}

		if(bIsInitialize)
		{
			return &SecurityAttributes;
		}

		if(!InitializeSecurityDescriptor(&SecurityDescriptor, SECURITY_DESCRIPTOR_REVISION)
			|| !SetSecurityDescriptorDacl(&SecurityDescriptor, TRUE, NULL, FALSE))
		{
			return NULL;
		}

		SecurityAttributes.nLength = sizeof(SecurityAttributes); 
		SecurityAttributes.lpSecurityDescriptor	= &SecurityDescriptor;
		SecurityAttributes.bInheritHandle = FALSE; 

		bIsInitialize = TRUE;

		return &SecurityAttributes;
	}


	DwScopeMutex::DwScopeMutex(DwMutex *m) : m_mutex(m)
	{
		if(m_mutex)
		{
			m_mutex->lock();
		}
	}

	DwScopeMutex::~DwScopeMutex()
	{
		if(m_mutex)
		{
			m_mutex->unlock();
			m_mutex = NULL;
		}
	}

	DWAutoScopeMutex::DWAutoScopeMutex(LPCWSTR mutexName)
	{
		m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexName);

		if(NULL == m_hMutex)
		{
			m_hMutex = CreateMutex(DwMutex::getAllAccessSecurityAttributes(), FALSE, mutexName);
		}

		if(m_hMutex)
		{
			WaitForSingleObject(m_hMutex, INFINITE);
		}
	}

	DWAutoScopeMutex::~DWAutoScopeMutex()
	{
		if(m_hMutex)
		{
			ReleaseMutex(m_hMutex);
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
	}
}
