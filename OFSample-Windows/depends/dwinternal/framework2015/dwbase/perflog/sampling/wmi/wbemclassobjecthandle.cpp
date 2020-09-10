/** 
@file
@brief Wbem Class Object Wrapper
@version 2012-08-28 wesleywu
*/

#include <Wbemcli.h>
#include <comutil.h>

#include "variant.h"
#include "stable.h"
#include "wbemclassobjecthandle.h"

namespace Perf
{
	CWbemClassObjectHandle::CWbemClassObjectHandle( IWbemClassObject *obj /*= 0*/ )
		: m_classObject(obj)
	{
	}

	CWbemClassObjectHandle::~CWbemClassObjectHandle(void)
	{	
		if (m_refCount.only() && m_classObject)
		{
			m_classObject->Release();
		}
	}

	CWbemClassObjectHandle& CWbemClassObjectHandle::operator=( const CWbemClassObjectHandle& rhs )
	{
		// 本对象赋值
		if (&rhs == this)
		{
			return *this;
		}

		// 赋值引用计数, 若attach返回真(原对象引用计数为0), 释放m_classObject
		if (m_refCount.attach(rhs.m_refCount) && m_classObject)
		{
			m_classObject->Release();
		}

		// 赋值m_classObject
		m_classObject = rhs.m_classObject;
		return *this;
	}

	CWbemClassObjectHandle::operator bool() const
	{
		return NULL != m_classObject;
	}

	CWbemClassObjectHandle::operator IWbemClassObject*() const
	{
		return m_classObject;
	}	
	
	IWbemClassObject*& CWbemClassObjectHandle::RawObject()
	{
		return m_classObject;
	}

	CWbemClassObjectHandle CWbemClassObjectHandle::Clone()
	{
		IWbemClassObject* cpy;
		HRESULT hr = m_classObject->SpawnInstance(0, &cpy);
		if (SUCCEEDED(hr))
		{
			return CWbemClassObjectHandle(cpy);
		}
		else
		{
			return CWbemClassObjectHandle(NULL);
		}
	}
	
	_bstr_t CWbemClassObjectHandle::GetPath() const 
	{
		CVariant var;
		if (GetProperty(path_property_name(), var))
		{
			return var.toString();
		}

		return _bstr_t();
	}

	bool CWbemClassObjectHandle::GetProperty(const wchar_t* name, CVariant& var) const
	{
		if (!m_classObject)
		{
			return false;
		}

		HRESULT hr = m_classObject->Get(name, 0, &var, 0, 0);

		return SUCCEEDED(hr);
	}

	const wchar_t* CWbemClassObjectHandle::path_property_name() const
	{
		return L"__PATH";
	}

}
