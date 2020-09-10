#include "stable.h"

#include "winwmi.h"
#include "dwbase/log.h"
#include <objbase.h>
#include <comDef.h>

#pragma comment(lib, "wbemuuid.lib")

template <typename _Interface>
void ReleaseInterface(_Interface*& p)
{
	__try
	{
		if (p != NULL && !IsBadReadPtr(p, sizeof(_Interface)))
		{
			p->Release();
		}
	}
	__except(1)
	{

	}
	p = NULL;
}

void reportWmiException( const char* what )
{
    LogFinal(LOG::KDuiFw) << what;
}

HRESULT wbemConnectServer( __in IWbemLocator* wbemLocator, __in const wchar_t* networkResource,__out IWbemServices **ppNamespace )
{
    HRESULT hres = WBEM_E_FAILED;
    if ( wbemLocator && ppNamespace )
    {
        BSTR bstrNetworkResource = SysAllocString( networkResource );
        *ppNamespace = NULL;
        __try
        {
            hres = wbemLocator->ConnectServer( bstrNetworkResource,
                NULL,
                NULL,
                0,
                NULL,
                0,
                0,
                ppNamespace);
        }
        __except(EXCEPTION_EXECUTE_HANDLER )
        {
            hres = WBEM_E_FAILED;
            reportWmiException("IWbemLocator::ConnectServer exception!");
        }

        SysFreeString( bstrNetworkResource );
        bstrNetworkResource = NULL;
    }
    return hres;
}

CWinWmi::CWinWmi()
	: m_pLoc(NULL)
	, m_pSvc(NULL)
{

}

CWinWmi::~CWinWmi()
{

}

BOOL CWinWmi::Init()
{
	PERFLOG;
	HRESULT hres;

	// 获取访问wmi权限
	hres = CoInitializeSecurity(
		NULL, 
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, 
		EOAC_NONE,
		NULL);

	switch (hres)
	{
	case RPC_E_TOO_LATE:
	case S_OK:
		break;
	default:
		return FALSE;
	}

	// 通过IWbemLocator 接口访问wmi，获取系统信息
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0, 
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&m_pLoc);

	if (FAILED(hres))
	{
		return FALSE;
	}

	// connect to wmi through the IwbemLocator::ConnectServer;
	hres = wbemConnectServer( m_pLoc, L"ROOT\\CIMV2", &m_pSvc );
	
	if (FAILED(hres))
	{
		return FALSE;
	}

	// set security levels on the proxy
	hres = CoSetProxyBlanket(m_pSvc, RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE);

	if (FAILED(hres))
	{
		return FALSE;
	}

	return TRUE;
}

void CWinWmi::Uninit()
{
	ReleaseInterface(m_pSvc);
	ReleaseInterface(m_pLoc);
}

HRESULT enumerateNext(IEnumWbemClassObject *pEnumerator, IWbemClassObject **p)
{
    __try
    {
        ULONG uReturn = 0;
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, p, &uReturn);
        if(uReturn == 0 || *p == 0)
        {
            return E_FAIL;
        }

        return hr;
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        reportWmiException("enumerate IEnumWbemClassObject exception!");
    }

    return E_FAIL;
}

std::wstring CWinWmi::GetInfo(const std::wstring& name, const std::wstring& param)
{
	PERFLOG;

	std::wstring result; // 返回值
	if(NULL == m_pSvc)
	{
		return result;
	}

	std::wstring selSQL = L"select * from " + name;
	IEnumWbemClassObject* pEnumerator = NULL;
	HRESULT hres = m_pSvc->ExecQuery(bstr_t("WQL"), bstr_t(selSQL.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

	if (FAILED(hres))
	{
		// ExecQuery 返回的 pEnumerator 可能为NULL
		ReleaseInterface(pEnumerator);
		return result;
	}

	while (pEnumerator)
	{
        IWbemClassObject* pclsObj = NULL;
		if (FAILED(enumerateNext(pEnumerator, &pclsObj)))
		{
			break;
		}

		VARIANT vtNameProp;
		HRESULT hr = pclsObj->Get( param.c_str(), 0, &vtNameProp, 0, 0);
		if ( SUCCEEDED(hr) && (V_VT(&vtNameProp) == VT_BSTR)) 
		{
			result.append(vtNameProp.bstrVal);
		}

		VariantClear(&vtNameProp);

		ReleaseInterface(pclsObj);
	}

	ReleaseInterface(pEnumerator);

	return result;
}
