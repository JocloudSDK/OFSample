/** 
@file
@brief ΪWMI����COM֧��
@version 2012-08-28 wesleywu
*/

#include "stable.h"

#include "comenviroment.h"

namespace Perf
{
	CComEnviroment::CComEnviroment()
		: m_bInitialized(false)
	{	
	}

	CComEnviroment::~CComEnviroment()
	{
		// �������ͷ�COM����
		CoUninitialize();
	}

	// ��ʼ��COM����
	HRESULT CComEnviroment::Initialize( DWORD initFlag /*= COINIT_MULTITHREADED*/ )
	{
		if (m_bInitialized)
		{
			return S_OK;
		}

		// ��ʼ��COM���
 		HRESULT hr = CoInitializeEx(0, initFlag);
 		if (FAILED(hr))
 		{
 			return hr;
 		}
 
		// ��ʼ����ȫѡ��
 		hr =  CoInitializeSecurity(  
 				NULL,       
 				-1,				// COM negotiates service                    
 				NULL,			// Authentication services  
 				NULL,			// Reserved  
 				RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication  
 				RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation  
 				NULL,			// Authentication info   
 				EOAC_NONE,		// Additional capabilities  
 				NULL			// Reserved  
 			);  
 
 		if (S_OK != hr && RPC_E_TOO_LATE != hr)
 		{
 			CoUninitialize();
			return hr;
 		}

		m_bInitialized = true;
		return S_OK;
	}

	CComEnviroment& CComEnviroment::GetInstance()
	{
		// �������ʱ�Զ��ͷ�Com��Դ
		static CComEnviroment instance;  

		return instance;
	}

	HRESULT EnableComSupport( DWORD initFlag /*= COINIT_MULTITHREADED*/ )
	{
		return CComEnviroment::GetInstance().Initialize();
	}

}