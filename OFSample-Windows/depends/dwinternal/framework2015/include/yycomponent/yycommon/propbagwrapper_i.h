#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/propBag_i.h"
/*
    use coCreateComInstance function
*/
DWDEFINE_INTERFACE(IPropBagWrapper):public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual LPCWSTR get(LPCWSTR key) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool has(LPCWSTR key) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void set(LPCWSTR key, LPCWSTR value) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool setWithoutReplace(LPCWSTR key, LPCWSTR value) = 0;
};

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IMyInfoWritter): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void asyncChangeMyInfo(IPropBagWrapperPtr bag) = 0;
};

DWDEFINE_INTERFACE(IPropBagConverter): public IUnk
{
    virtual IPropBagPtr bizPropbag() const = 0;
    virtual void setPropbag(IPropBagPtr bag) = 0;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//helper
//IPropBag_setDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const T& v);
template <class T> inline 
void IPropBag_setDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const T& v)
{pPB->set(key, stringizing::ToString(v).c_str());}
template <> inline 
void IPropBag_setDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const LPCWSTR& v)
{pPB->set(key, v != NULL ? v : L"");}
template <class WSTR_Traits, class WSTR_Ax>
void IPropBag_setDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>& v)
{pPB->set(key, v.c_str());}

//T IPropBag_getDx(IPropBagWrapperPtr pPB, LPCWSTR key, const T& defV = T());
template <class T> inline 
T IPropBag_getDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const T& defV = T())
{return stringparser::Parse2(pPB->get(key), defV);}
template <> inline 
LPCWSTR IPropBag_getDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const LPCWSTR& defV)
{LPCWSTR v = pPB->get(key); return v != NULL ? v : defV != NULL ? defV : L"";}
template <size_t LEN> inline 
LPCWSTR IPropBag_getDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const WCHAR (&defV)[LEN])
{LPCWSTR v = pPB->get(key); return v != NULL ? v : defV;}
template <class WSTR_Traits, class WSTR_Ax>
std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax> IPropBag_getDxx(IPropBagWrapperPtr pPB, LPCWSTR key, const std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>& defV)
{LPCWSTR v = pPB->get(key); return v != NULL ? std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>(v) : std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>();}
