/** 
@file
@brief 实现VARIANT的类型转换
@version 2012-08-28 wesleywu
*/

#include "variant.h"
#include <sstream>
#include <wbemcli.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsupp.lib") 

namespace Perf
{
	CVariant::CVariant()
	{
		VariantInit(this);
	}

	CVariant::~CVariant()
	{
		VariantClear(this);
	}

// 实现Variant To Array函数
#define IMPLEMENT_VARIANT_TO_ARRAY_FUNC(otype_, type_, ref_) \
	assert(V_VT(this) == (type_ | VT_ARRAY));	\
	std::vector<otype_> values;					\
	SAFEARRAYBOUND sab = parray->rgsabound[0];	\
	for (LONG i = sab.lLbound; i < (LONG)sab.cElements; ++i)	\
	{	\
		VARIANT Value;		\
		long Index[1];		\
		Index[0] = i;		\
		SafeArrayPtrOfIndex(parray, Index, &Value.byref);		\
		values.push_back((ref_(&Value))[0]);		\
	}	\
	return values;	

	std::vector<BSTR> CVariant::toStringArray() const
	{
		IMPLEMENT_VARIANT_TO_ARRAY_FUNC(BSTR, VT_BSTR, V_BSTRREF);
	}

	std::vector<IUnknown*> CVariant::toIUnknownArray() const
	{
		IMPLEMENT_VARIANT_TO_ARRAY_FUNC(IUnknown*, VT_UNKNOWN, V_UNKNOWNREF);
	}

	std::vector<UINT32> CVariant::toUIntArray() const
	{
		IMPLEMENT_VARIANT_TO_ARRAY_FUNC(UINT32, VT_I4, V_UI4REF);  
	}

	std::vector<BYTE> CVariant::toByteArray() const
	{
		IMPLEMENT_VARIANT_TO_ARRAY_FUNC(BYTE, VT_UI1, V_UI1REF);  
	}

#undef IMPLEMENT_VARIANT_TO_ARRAY_FUNC
	
	std::wstring CVariant::toStdWString()
	{
		std::wstringstream wss;
		switch (V_VT(this))
		{
		case CIM_ILLEGAL: 
			wss << L"<Illegal>"; 
			break;  
		case VT_NULL:  
		case CIM_EMPTY:
			wss << L"<Empty>"; 
			break;  
		case CIM_SINT8: 
			wss << V_I1(this); 
			break;  
		case CIM_UINT8:
			wss << V_UI1(this);
			break;  
		case CIM_SINT16: 
			wss << V_I2(this); 
			break;  
		case CIM_UINT16: 
			wss << V_UI2(this); 
			break;  
		case CIM_SINT32: 
			wss << V_I4(this);
			break;  
		case CIM_UINT32: 
			wss << V_UI4(this);
			break;  
		case CIM_SINT64: 
			wss << V_I8(this); 
			break;  
		case CIM_UINT64: 
			wss << V_UI8(this); 
			break;  
		case CIM_REAL32: 
			wss << V_R4(this); 
			break;  
		case CIM_REAL64:
			wss << V_R8(this); 
			break;  
		case CIM_BOOLEAN: 
			wss << (V_BOOL(this) ? L"true" : L"false"); 
			break;  
		case CIM_STRING:   
		case CIM_DATETIME:  
		case CIM_REFERENCE:  
		case CIM_CHAR16: 
			wss << V_BSTR(this);
			break;  
		case CIM_OBJECT:
			wss << L"<Object>";
			break;  
		default:  
			{  
				if (V_VT(this) > CIM_FLAG_ARRAY)  
				{  
					assert(SafeArrayGetDim(this->parray) == 1);  
					wss << L"ARRAY(";  
					SAFEARRAYBOUND* sab = &this->parray->rgsabound[0];  
					for (LONG i = sab->lLbound; i < (LONG)sab->cElements; ++i)  
					{  
						VARIANT Value;  
						long Index[1];  
						Index[0] = i;  
						SafeArrayPtrOfIndex(this->parray, Index, &Value.byref);  
						if (i > this->parray->rgsabound[0].lLbound)  
						{
							wss << L", ";  
						}
						switch (V_VT(this) & ~CIM_FLAG_ARRAY)  
						{  
						case CIM_SINT8:
							wss << (V_I1REF(&Value))[0];
							break;  
						case CIM_UINT8: 
							wss << (V_UI1REF(&Value))[0];
							break;  
						case CIM_SINT16: 
							wss << (V_I2REF(&Value))[0];
							break;  
						case CIM_UINT16:
							wss << (V_UI2REF(&Value))[0]; 
							break;  
						case CIM_SINT32: 
							wss << (V_I4REF(&Value))[0]; 
							break;  
						case CIM_UINT32: 
							wss << (V_UI4REF(&Value))[0];
							break;  
						case CIM_SINT64: 
							wss << (V_I8REF(&Value))[0]; 
							break;  
						case CIM_UINT64:
							wss << (V_UI8REF(&Value))[0]; 
							break;  
						case CIM_REAL32: 
							wss << (V_R4REF(&Value))[0]; 
							break;  
						case CIM_REAL64: 
							wss << (V_R8REF(&Value))[0]; 
							break;  
						case CIM_BOOLEAN: 
							wss << ((V_BOOLREF(&Value))[0] ? L"true" : L"false"); 
							break;  
						case CIM_STRING:   
						case CIM_DATETIME:  
						case CIM_REFERENCE:  
						case CIM_CHAR16: 
							wss << (V_BSTRREF(&Value))[0]; 
							break;  
						}  
					}  
					wss << L")";  
				}  
				else  
				{  
					wss << L"<Unknown Type>";  
				}  
			}  
		}  
		return wss.str();  
	}  
}