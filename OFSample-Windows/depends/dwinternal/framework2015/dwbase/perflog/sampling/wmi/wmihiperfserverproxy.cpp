#include "stable.h"

#include <WbemCli.h>
#include <sstream>
#include "wbemlocator.h"
#include "wbemclassobjecthandle.h"
#include "wmihiperfserverproxy.h"
#include "stlalloc.h"

namespace Perf
{
	CWmiHiPerfServerProxy::CWmiHiPerfServerProxy(CWbemLocator loc)
		: m_locator(loc)
		, m_llRefreshPerfCount(0)
		, m_pRefresher(NULL)
		, m_pConfig(NULL)
	{
	}

	CWmiHiPerfServerProxy::~CWmiHiPerfServerProxy(void)
	{
	}

	HRESULT CWmiHiPerfServerProxy::Refresh( long lFlags /*= 0*/ )
	{
		HRESULT hr = m_pRefresher ? m_pRefresher->Refresh(lFlags) : S_FALSE;
		::QueryPerformanceCounter((LARGE_INTEGER *)&m_llRefreshPerfCount);
		return hr;
	}

	CWbemClassObjectHandle* CWmiHiPerfServerProxy::NamedObject( std::wstring name )
	{
		std::map<std::wstring, CWbemClassObjectHandle>::iterator it;
		if ((it = m_NamedObjectMap.find(name)) != m_NamedObjectMap.end())
		{
			return &it->second;
		}

		return NULL;
	}

	CWbemClassObjectHandle* CWmiHiPerfServerProxy::AddNamedObject( std::wstring name, const wchar_t* className, const wchar_t* nameFilter )
	{
		long lID = 0;
		// 插入时判断是否有重复键值
		std::pair<std::map<std::wstring, CWbemClassObjectHandle>::iterator, bool> ret = 
			m_NamedObjectMap.insert(std::map<std::wstring, CWbemClassObjectHandle>::value_type(name, CWbemClassObjectHandle(0)));

		if (ret.second)
		{
			std::wstringstream filter;
			filter << className << L".Name=\'" << nameFilter << L"\'";
			// 无重复键值, 插入成功, 添加路径
			if (S_OK != m_pConfig->AddObjectByPath(
				m_locator.GetServer(), 
				filter.str().c_str(), 
				0, 
				NULL, 
				&ret.first->second.RawObject(), 
				&lID) )
			{
				m_NamedObjectMap.erase(ret.first);
				return NULL;
			};
		}
		
		return &ret.first->second;
	}

	HRESULT CWmiHiPerfServerProxy::CreateWmiHiPerfServerProxy( CWbemLocator loc, CWmiHiPerfServerProxy** ppWmiHiPerfServerProxy )
	{
		if (ppWmiHiPerfServerProxy == NULL)
		{
			return S_FALSE;
		}

		if (NULL != *ppWmiHiPerfServerProxy)
		{
			CMyHeapAlloc::GetInstance()->MyFree(*ppWmiHiPerfServerProxy);
		}

		*ppWmiHiPerfServerProxy = new ((void *)CMyHeapAlloc::GetInstance()->MyAlloc(sizeof(CWmiHiPerfServerProxy), TRUE)) CWmiHiPerfServerProxy(loc);

		HRESULT hr;
		if((hr = (*ppWmiHiPerfServerProxy)->initialize()) != S_OK)
		{
			CMyHeapAlloc::GetInstance()->MyFree(*ppWmiHiPerfServerProxy);
			*ppWmiHiPerfServerProxy = NULL;
		}
		
		return hr;
	}

	HRESULT CWmiHiPerfServerProxy::initialize()
	{
		// 初始化m_pRefresher及m_pConfig
		HRESULT hr = CoCreateInstance(
			CLSID_WbemRefresher,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWbemRefresher,
			(void**) &m_pRefresher);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = m_pRefresher->QueryInterface(
			IID_IWbemConfigureRefresher,
			(void**) &m_pConfig);

		return hr;
	}

}
