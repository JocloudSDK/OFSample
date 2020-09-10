/** 
@file
@brief Wbem Enumerator Wrapper
@version 2012-08-28 wesleywu
*/

#include "stable.h"

#include <wbemcli.h>
#include <comutil.h>
#include "wbemclassobjecthandle.h"
#include "wbemenumerator.h"
#include "stlalloc.h"

namespace Perf
{
	CWbemEnumerator::CWbemEnumerator(void)
		: m_enumerator(NULL)
	{
	}

	CWbemEnumerator::~CWbemEnumerator(void)
	{
		if (m_enumerator)
		{
			m_enumerator->Release();
		}
	}

	HRESULT CWbemEnumerator::CreateEnumerator( IWbemServices* svr, const _bstr_t& query, CWbemEnumerator** ppEnumerator )
	{
		if (!svr) 
		{
			return S_FALSE;
		}

		if (NULL != *ppEnumerator)
		{
			CMyHeapAlloc::GetInstance()->MyFree(*ppEnumerator);
		}

		*ppEnumerator = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CWbemEnumerator), TRUE)) CWbemEnumerator();
		
		HRESULT hr = svr->ExecQuery(
			bstr_t("WQL"), query,  
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,   
			NULL, &(*ppEnumerator)->m_enumerator);  

		if (S_OK != hr) 
		{
			CMyHeapAlloc::GetInstance()->MyFree(*ppEnumerator);
			*ppEnumerator = NULL;
		}
		return hr;
	}

	void CWbemEnumerator::Restart()
	{
		m_enumerator->Reset();  
	}

	CWbemClassObjectHandle Perf::CWbemEnumerator::Next()
	{
		ULONG uReturn = 0;  
		IWbemClassObject* obj = 0;  
		HRESULT hres = m_enumerator->Next(WBEM_INFINITE, 1, &obj, &uReturn);  
		if (hres != WBEM_S_NO_ERROR || uReturn == 0)  
			obj = 0;  
		return CWbemClassObjectHandle(obj);  
	}
}
