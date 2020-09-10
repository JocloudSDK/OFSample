#pragma  once

#include <atlbase.h>
#include <atlstr.h>

namespace DwUtility
{
	namespace http
	{
		template<class T>
		class ATL_NO_VTABLE CLocalDispatch : public T
		{
		public:
			STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObject)
			{
				if (ppvObject == NULL)
					return E_POINTER;
				if (InlineIsEqualUnknown(riid) ||
					riid == IID_IDispatch) {
						*ppvObject = (IDispatch*)this;
						AddRef();
						return S_OK;
					} else
						return E_NOINTERFACE;
			}
			STDMETHOD_(ULONG, AddRef)()
			{return 1;}
			STDMETHOD_(ULONG, Release)()
			{return 1;}
			STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
			{
				return E_NOTIMPL;
			}
			STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
			{
				return E_NOTIMPL;
			}
			STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
				LCID lcid, DISPID* rgdispid)
			{
				return E_NOTIMPL;
			}
			STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
				LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
				EXCEPINFO* pexcepinfo, UINT* puArgErr)
			{
				return S_OK;
			}
		};

		class HttpReqCallBack : public CLocalDispatch<IDispatch>
		{
		public:
			STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
				LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
				EXCEPINFO* pexcepinfo, UINT* puArgErr)
			{
				try
				{
					this->
					onResponse();
				}
				catch (...)
				{
					//::MessageBox(0,_T("HttpReqCallBack"),_T("onResponse"),0);
				}
				return S_OK;
			}
		public:
			virtual void onResponse() = 0;

		};
	}
}
