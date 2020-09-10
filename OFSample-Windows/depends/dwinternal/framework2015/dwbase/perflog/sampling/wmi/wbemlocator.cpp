/** 
@file
@brief Wbem Locator Wrapper
@version 2012-08-28 wesleywu
*/

#include "stable.h"

#include <wbemcli.h>
#include "wbemlocator.h"

namespace Perf
{
	CWbemLocator::CWbemLocator( const wchar_t* host, const wchar_t* user, const wchar_t* psw, const wchar_t* locale, const wchar_t* path )
		: m_szHost(host)
		, m_szUser(user)
		, m_szPassword(psw)
		, m_szLocale(locale)
		, m_szPath(path)
	{
		the_locator locator;  
		m_server = locator.connect_to(  
			"\\\\" + _bstr_t(m_szHost) + m_szPath,                    // path  
			_bstr_t(m_szUser ? m_szUser : (const wchar_t*)0),          // User name  
			_bstr_t(m_szPassword? m_szPassword : (const wchar_t*)0),  // User password  
			_bstr_t(m_szLocale? m_szLocale : (const wchar_t*)0)       // Locale  
			);  
	}

	IWbemServices* CWbemLocator::GetServer() const
	{
		return m_server;
	}

	HRESULT CWbemLocator::Initialize()
	{
		return CoSetProxyBlanket(  
			m_server,                     // the proxy to set  
			RPC_C_AUTHN_WINNT,            // authentication service  
			RPC_C_AUTHZ_NONE,             // authorization service  
			NULL,                         // Server principal name  
			RPC_C_AUTHN_LEVEL_CALL,       // authentication level  
			RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level  
			NULL,                         // client identity   
			EOAC_NONE                     // proxy capabilities       
		);  
	}


	CWbemLocator::wbem_server::wbem_server( IWbemServices* svr /*= 0*/ )
		: _svr(svr)
	{
	}

	CWbemLocator::wbem_server::~wbem_server()	
	{
		if (_ref.only() && _svr)
		{
			_svr->Release();
		}
	}

	CWbemLocator::wbem_server& CWbemLocator::wbem_server::operator=( const wbem_server& rhs )
	{
		if (this == &rhs)  
		{
			return *this;  
		}

		if (_ref.attach(rhs._ref) && _svr)  
		{
			_svr->Release();  
		}

		_svr = rhs._svr;  
		return *this;  
	}

	CWbemLocator::wbem_server::operator IWbemServices*() const
	{
		return _svr;
	}


	CWbemLocator::the_locator::the_locator()
		: _locator(NULL)
	{
	}

	CWbemLocator::the_locator::~the_locator()
	{
		if (_locator)
		{
			_locator->Release();
		}		
	}

	CWbemLocator::wbem_server CWbemLocator::the_locator::connect_to( _bstr_t ns, _bstr_t user, _bstr_t password, _bstr_t locale )
	{
		// 进行连接时创建IWbemLocator实例
		HRESULT hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&_locator);

		if (FAILED(hres))
		{
			return wbem_server(0);
		}

		IWbemServices* svr = 0;
		hres = _locator->ConnectServer(ns, user, password, locale, NULL, 0, 0, &svr);
		if (FAILED(hres))
		{
			return wbem_server(0);
		}

		return wbem_server(svr);
	}

}
