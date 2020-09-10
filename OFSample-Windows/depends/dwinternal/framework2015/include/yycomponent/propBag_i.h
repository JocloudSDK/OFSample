
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once


//////////////////////////////////////////////////////////////////////////
//propBag
define_interface(IPropBag) : IUnk
{
	virtual LPCWSTR get(LPCWSTR key) = 0;
	virtual bool has(LPCWSTR key) = 0;

	virtual void set(LPCWSTR key, LPCWSTR value) = 0;
	virtual bool setWithoutReplace(LPCWSTR key, LPCWSTR value) = 0;

	virtual void removeAll() = 0;

	virtual IPropBagPtr clone() = 0;

    virtual void remove(LPCWSTR key) = 0;

	virtual void dump(std::vector<unsigned int> &keys, std::vector<std::wstring> &values) = 0;

	virtual void clone2(const std::vector<unsigned int> &keys, const std::vector<std::wstring> &values) = 0;
};


//////////////////////////////////////////////////////////////////////////
//propBagColl
define_interface(IPropBagColl) : IUnk
{
	virtual UINT32 getCount() = 0;
	virtual IPropBagPtr getItem(UINT32 idx) = 0;

	virtual IPropBagPtr findItemById(UINT32 xid) = 0;

	virtual void appendItem(IPropBagPtrCR spPB, UINT32 xid) = 0;
	//virtual void removeItemAt(UINT32 idx) = 0;
	//virtual void removeItemById(UINT32 xid) = 0;
	virtual void removeAllItems() = 0;

	virtual IPropBagPtr newPropBag() = 0;
};




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//helper
//IPropBag_setDx(IPropBagPtrCR pPB, LPCWSTR key, const T& v);
template <class T> inline 
void IPropBag_setDx(IPropBagPtrCR pPB, LPCWSTR key, const T& v)
	{pPB->set(key, stringizing::ToString(v).c_str());}
template <> inline 
void IPropBag_setDx(IPropBagPtrCR pPB, LPCWSTR key, const LPCWSTR& v)
	{pPB->set(key, v != NULL ? v : L"");}
template <class WSTR_Traits, class WSTR_Ax>
void IPropBag_setDx(IPropBagPtrCR pPB, LPCWSTR key, const std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>& v)
	{pPB->set(key, v.c_str());}

//T IPropBag_getDx(IPropBagPtrCR pPB, LPCWSTR key, const T& defV = T());
template <class T> inline 
T IPropBag_getDx(IPropBagPtrCR pPB, LPCWSTR key, const T& defV = T())
	{return stringparser::Parse2(pPB->get(key), defV);}
template <> inline 
LPCWSTR IPropBag_getDx(IPropBagPtrCR pPB, LPCWSTR key, const LPCWSTR& defV)
	{LPCWSTR v = pPB->get(key); return v != NULL ? v : defV != NULL ? defV : L"";}
template <size_t LEN> inline 
LPCWSTR IPropBag_getDx(IPropBagPtrCR pPB, LPCWSTR key, const WCHAR (&defV)[LEN])
 	{LPCWSTR v = pPB->get(key); return v != NULL ? v : defV;}
template <class WSTR_Traits, class WSTR_Ax>
std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax> IPropBag_getDx(IPropBagPtrCR pPB, LPCWSTR key, const std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>& defV)
 	{LPCWSTR v = pPB->get(key); return v != NULL ? std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>(v) : std::basic_string<WCHAR, WSTR_Traits, WSTR_Ax>();}

