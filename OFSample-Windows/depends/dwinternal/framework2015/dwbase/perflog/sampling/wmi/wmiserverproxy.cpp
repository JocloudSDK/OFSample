/** 
@file
@brief Basis Wbem Server Proxy
@version 2012-08-28 wesleywu
*/

#include "stable.h"

#include <WbemIdl.h>
#include <sstream>
#include "wbemclassobjecthandle.h"
#include "wbemlocator.h"
#include "wbemenumerator.h"
#include "wmiserverproxy.h"
#include "stlalloc.h"

namespace Perf
{
	CWmiServerProxy::CWmiServerProxy( CWbemLocator loc, const wchar_t* serviceClass, const wchar_t* category )
		: m_locator(loc)
		, m_enumerator(NULL)
		, m_szServiceClass(serviceClass)
		, m_szCategoryName(category)
	{

	}

	CWmiServerProxy::~CWmiServerProxy()
	{
		if (m_enumerator)
		{
			CMyHeapAlloc::GetInstance()->MyFree(m_enumerator);
			m_enumerator = NULL;
		}		
	}

	HRESULT CWmiServerProxy::Start()
	{
		if (m_enumerator)
		{
			CMyHeapAlloc::GetInstance()->MyFree(m_enumerator);
		}

		std::wstringstream qry;
		qry << getQuery();
		std::wstring condition = getCondition();
		if (!condition.empty())
		{
			qry << L" where " << condition;
		}

		return CWbemEnumerator::CreateEnumerator(
			m_locator.GetServer(), 
			_bstr_t(qry.str().c_str()),
			&m_enumerator);	
	}

	CWbemClassObjectHandle CWmiServerProxy::Next()
	{
		return m_enumerator ? m_enumerator->Next() : CWbemClassObjectHandle(NULL);
	}

	void CWmiServerProxy::SetEqualFilter( const wchar_t* k, const wchar_t* v )
	{
		if (NULL != k && NULL != v)
		{
			m_filterEqual[k] = v;
		}
	}

	void CWmiServerProxy::SetMatchFilter( const wchar_t* k, const wchar_t* v )
	{
		if (NULL != k && NULL != v)
		{
			m_filterMatch[k] = v;
		}
	}	

	std::wstring CWmiServerProxy::getQuery() const
	{
		std::wstringstream qry;
		qry << L"select * from " << getServiceClass();
		return qry.str();
	}

	std::wstring CWmiServerProxy::getCondition() const
	{
		std::wstringstream ss;  
		if (!m_filterEqual.empty())  
		{  
			for (filter_t::const_iterator i = m_filterEqual.begin(); i != m_filterEqual.end(); ++i)  
			{  
				if (ss.str().size() > 0)  
					ss << L" and ";  
				ss << i->first << L" = '" << i->second << L"'";  
			}  
		}  
		if (!m_filterMatch.empty())  
		{  
			for (filter_t::const_iterator i = m_filterMatch.begin(); i != m_filterMatch.end(); ++i)  
			{  
				if (ss.str().size() > 0)  
					ss << L" and ";  
				ss << i->first << L" like '%" << i->second << L"%'";  
			}  
		}  
		return ss.str();  
	}

}