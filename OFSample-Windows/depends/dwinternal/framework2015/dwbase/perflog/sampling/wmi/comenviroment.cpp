/** 
@file
@brief 为WMI启用COM支持
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
		// 析构，释放COM环境
		CoUninitialize();
	}

	// 初始化COM环境
	HRESULT CComEnviroment::Initialize( DWORD initFlag /*= COINIT_MULTITHREADED*/ )
	{
		if (m_bInitialized)
		{
			return S_OK;
		}

		// 初始化COM组件
 		HRESULT hr = CoInitializeEx(0, initFlag);
 		if (FAILED(hr))
 		{
 			return hr;
 		}
 
		// 初始化安全选项
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
		// 程序结束时自动释放Com资源
		static CComEnviroment instance;  

		return instance;
	}

	HRESULT EnableComSupport( DWORD initFlag /*= COINIT_MULTITHREADED*/ )
	{
		return CComEnviroment::GetInstance().Initialize();
	}

}