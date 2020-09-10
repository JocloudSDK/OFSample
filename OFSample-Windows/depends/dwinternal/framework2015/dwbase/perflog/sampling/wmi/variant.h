/*! @file variant.h 
 *  @brief 包装VARIANT
 *
 *  实现VARIANT的类型转换
 *
 *  @author wesleywu
 *  @version 1.0
 *  @data 2012-08-28
 */

#pragma once

#include "wtypes.h"
#include <vector>
#include <comutil.h>

#include <assert.h>
#include <sstream>

namespace Perf
{
	class CVariant : public VARIANT
	{
		// no copy and assignment supported
		CVariant(const CVariant&);
		CVariant& operator = (const CVariant&);

	public:
		CVariant();
		~CVariant();

		// 空值判断
		inline bool isNull() const	{
			return V_VT(this) == VT_EMPTY || V_VT(this) == VT_NULL;
		}

		// 非空值判断
		inline bool isNotNull() const 
		{
			return V_VT(this) != VT_EMPTY && V_VT(this) != VT_NULL;
		}

		// 单值转化函数
		inline BYTE toByte() const
		{
			assert(V_VT(this) == VT_UI1);
			return V_UI1(this); 
		}

		inline BOOL toBool() const
		{
			assert(V_VT(this) == VT_BOOL); 
			return V_BOOL(this);
		}

		inline INT16 toShort() const
		{
			assert(V_VT(this) == VT_I2); 
			return V_I2(this); 
		}

		inline UINT16 toUShort() const
		{
			assert(V_VT(this) == VT_UI2);
			return V_UI2(this);
		}

		inline INT32 toLong() const
		{
			assert(V_VT(this) == VT_I4); 
			return V_I4(this);
		}

		inline UINT32 toULong() const
		{
			assert(V_VT(this) == VT_UI4);
			return V_UI4(this);
		}

		inline INT32 toInt() const
		{
			assert(V_VT(this) == VT_I4); 
			return V_I4(this);
		}

		inline UINT32 toUInt() const
		{
			assert(V_VT(this) == VT_UI4);
			return V_UI4(this);
		}

		inline FLOAT toFloat() const
		{
			assert(V_VT(this) == VT_R4);
			return V_R4(this); 
		}

		inline DOUBLE toDouble() const
		{
			assert(V_VT(this) == VT_R8); 
			return V_R8(this); 
		}

		inline LONGLONG toLongLong() const
		{
			assert(V_VT(this) == VT_I8); 
			return V_I8(this);
		}

		inline ULONGLONG toULongLong() const
		{
			assert(V_VT(this) == VT_UI8);
			return V_UI8(this);
		}

		inline BSTR toString() const
		{
			assert(V_VT(this) == VT_BSTR); 
			return V_BSTR(this);
		}

		inline SYSTEMTIME toDate() const
		{
			assert(V_VT(this) == VT_DATE);  
			SYSTEMTIME st;  
			VariantTimeToSystemTime(V_DATE(this), &st);  
			return st;  
		}

		inline IUnknown* toUnKnown() const
		{
			assert(V_VT(this) == VT_UNKNOWN);
			return V_UNKNOWN(this);
		}

		// 转化为标准库中的wstring类型
		std::wstring toStdWString();

		// 转化为数组类型
		std::vector<BSTR>		toStringArray() const;
		std::vector<IUnknown*>	toIUnknownArray() const;
		std::vector<UINT32>		toUIntArray() const;
		std::vector<BYTE>		toByteArray() const;

		// 用wstringstream实现安全转换
		template<typename T>
		T safeCast()
		{
			std::wstringstream wss;
			wss << toStdWString();

			T out;
			wss >> out;
			return out;
		}
	};
}