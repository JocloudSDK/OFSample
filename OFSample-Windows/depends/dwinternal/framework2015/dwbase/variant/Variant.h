#pragma once

#include <assert.h>
#include "dwbase/MemPool.h"

namespace Data
{
	struct DWVariant
	{
	public:
		enum E_TYPE
		{
			TYPE_E_NULL,
			TYPE_E_STR,
			TYPE_E_INT64,
			TYPE_E_UINT64,
			TYPE_E_INT,
			TYPE_E_UINT,
			TYPE_E_BOOL,
		};

		DWVariant() : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
		}

		DWVariant(LPCWSTR lpwszVal) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setValue(lpwszVal);
		}

		DWVariant(INT64 i64Val) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setValue(i64Val);
		}

		DWVariant(UINT64 u64Val) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setValue(u64Val);
		}

		DWVariant(INT iVal) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setValue(iVal);
		}

		DWVariant(UINT uVal) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setValue(uVal);
		}

		DWVariant(bool bVal) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setValue(bVal);
		}

		DWVariant(const DWVariant & src) : m_eType(TYPE_E_NULL), m_dwDum(0)
		{
			setXXX(src.m_eType, src.m_dwDum);
		}	

		~DWVariant()
		{
			clear();
		}


		E_TYPE getType() const
		{
			return m_eType;
		}

		void clear()
		{
			if (m_eType == TYPE_E_STR || m_eType == TYPE_E_INT64 || m_eType == TYPE_E_UINT64)
				mp_free((void *)m_dwDum);
			
			m_dwDum = 0;
			m_eType = TYPE_E_NULL;
		}

		BOOL	isNull() const
		{ 
			return m_eType == TYPE_E_NULL;
		}

		LPCWSTR	toStr() const
		{
			assert(m_eType == TYPE_E_STR);
			return m_eType == TYPE_E_STR ? (LPCWSTR)m_dwDum : L"";
		}

		INT	toInt() const
		{
			return (INT)toUInt();
		}

		UINT toUInt() const
		{
			switch (m_eType)
			{
				case TYPE_E_UINT:
				case TYPE_E_INT:
				case TYPE_E_BOOL:
					return (UINT)m_dwDum;
				case TYPE_E_UINT64:
				case TYPE_E_INT64:
					return (UINT)*(UINT64 *)m_dwDum;
				default:
					assert(! L"DWVariant不能转换为指定的类型");
					return 0;
			}
		}

		INT64 toInt64() const
		{
			return (INT64)toUInt64();
		}

		UINT64 toUInt64() const
		{
			switch (m_eType)
			{
				case TYPE_E_UINT64:
				case TYPE_E_INT64:
					return *(UINT64 *)m_dwDum;
				case TYPE_E_UINT:
				case TYPE_E_INT:
				case TYPE_E_BOOL:
					return (UINT64)m_dwDum;
				default:
					assert(! L"DWVariant不能转换为指定的类型");
					return 0;
			}
		}

		bool toBool() const
		{
			switch (m_eType)
			{
				case TYPE_E_BOOL:
					return !!m_dwDum;
				case TYPE_E_UINT:
				case TYPE_E_INT:
					return m_dwDum != 0;
				case TYPE_E_UINT64:
				case TYPE_E_INT64:
					return *(INT64 *)m_dwDum != 0;
				default:
					assert(! L"DWVariant不能转换为指定的类型");
					return 0;
			}
		}

		void	setValue(LPCWSTR lpwszVal)
		{
			setXXX(TYPE_E_STR, (DWORD)lpwszVal);
		}

		void	setValue(INT iVal)
		{
			setXXX(TYPE_E_INT, (DWORD)iVal);
		}

		void	setValue(LONG lVal)
		{
			setXXX(TYPE_E_INT, (DWORD)lVal);
		}

		void	setValue(UINT uVal)
		{
			setXXX(TYPE_E_UINT, (DWORD)uVal);
		}

		void	setValue(ULONG ulVal)
		{
			setXXX(TYPE_E_UINT, (DWORD)ulVal);
		}

		void	setValue(INT64 i64Val)
		{
			setXXX(TYPE_E_INT64, (DWORD)&i64Val);
		}


		void	setValue(UINT64 u64Val)
		{
			setXXX(TYPE_E_UINT64, (DWORD)&u64Val);
		}

		void	setValue(bool bVal)
		{
			setXXX(TYPE_E_BOOL, (DWORD)bVal);
		}

		DWVariant &	operator = ( const DWVariant & src )
		{
			setXXX(src.m_eType, src.m_dwDum);
			return *this;
		}

		BOOL operator == (const DWVariant& src) const
		{
			BOOL bRet = FALSE;

			if (src.m_eType == m_eType)
			{
				switch (m_eType)
				{
					case TYPE_E_INT:
					case TYPE_E_UINT:
					case TYPE_E_BOOL:
						bRet = m_dwDum == src.m_dwDum;
						break;
					case TYPE_E_STR:
						bRet = wcscmp((LPCTSTR)m_dwDum, (LPCTSTR)src.m_dwDum) == 0;
						break;
					case TYPE_E_INT64:
					case TYPE_E_UINT64:
						bRet = *(INT64 *)m_dwDum == *(INT64 *)src.m_dwDum;
						break;
					case TYPE_E_NULL:
						bRet = TRUE;
						break;
					default:
						assert(! L"不支持的E_TYPE");
						break;
				}
			}
			else
			{
				assert(m_eType == TYPE_E_NULL || src.m_eType == TYPE_E_NULL);
			}

			return bRet;
		}

		BOOL operator != (const DWVariant& oVal) const
		{
			return !(operator ==(oVal));
		}

	private:
		void	setXXX(E_TYPE eType, DWORD dwDum)
		{
			clear();

			m_eType = eType;

			switch (eType)
			{
				case TYPE_E_INT:
				case TYPE_E_UINT:
				case TYPE_E_BOOL:
					m_dwDum = dwDum;
					break;
				case TYPE_E_STR:
					{
						DWORD cchSrc = wcslen((LPCWSTR)dwDum);
						
						wchar_t * p = (wchar_t *)mp_alloc((cchSrc + 1) * sizeof(wchar_t));
						memcpy(p, (const void *)dwDum, (cchSrc + 1) * sizeof(wchar_t));

						m_dwDum = (DWORD)p;
					}
					break;
				case TYPE_E_INT64:
				case TYPE_E_UINT64:
					{
						UINT64* p = (UINT64 *)mp_alloc(sizeof(UINT64));
						*p = *(UINT64 *)dwDum;
						m_dwDum = (DWORD)p;
					}
					break;
				case TYPE_E_NULL:
					break;
				default:
					assert(! L"不支持的E_TYPE");
					break;
			}
		}

		E_TYPE	m_eType;
		DWORD	m_dwDum;
	};
};
