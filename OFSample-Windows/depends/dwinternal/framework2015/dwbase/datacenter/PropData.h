#pragma once

#include "ITablePub.h"

namespace Data 
{
	class CPropData
	{
	public:
		static CPropData * createInstance();

		inline void addRef()
		{
			assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
			::InterlockedIncrement((LONG *)&m_dwRef);
		}

		inline void release()
		{
			assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
			if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			{
				this->~CPropData();
				mp_free(this);
			}
		}

		//读属性
		BOOL			hasProp(const NAME_ID &name) const;
		LPCWSTR			getPropStr(const NAME_ID &name) const;
		DWORD64			getPropDword64(const NAME_ID &name) const;
		DWORD			getPropDword(const NAME_ID &name) const;
		WORD			getPropWord(const NAME_ID &name) const;
		BYTE			getPropByte(const NAME_ID &name) const;
		BOOL			getPropBool(const NAME_ID &name) const;
		comptr<IUnk>	getPropUnk(const NAME_ID &name) const;

		//写属性
		BOOL			setPropStr(const NAME_ID &name, LPCWSTR lpwszValue);
		BOOL			setPropDword64(const NAME_ID &name, DWORD64 dw64Value);
		BOOL			setPropDword(const NAME_ID &name, DWORD dwValue);
		BOOL			setPropWord(const NAME_ID &name, WORD wValue);
		BOOL			setPropByte(const NAME_ID &name, BYTE byValue);
		BOOL			setPropBool(const NAME_ID &name, BOOL bValue);
		BOOL			setPropUnk(const NAME_ID &name, IUnk *pUnk);
		BOOL			removeProp(const NAME_ID &name);

	private:
		CPropData();
		~CPropData();

		BOOL					getPropXXX(const NAME_ID &name, E_FIELD type, DWORD *lpdwDum) const;
		BOOL					setPropXXX(const NAME_ID &name, E_FIELD type, DWORD dwDum);

		struct PROP_ITEM
		{
			BYTE 	type;
			WORD	id;
			DWORD 	value;
		}; 

		struct PROP_DATA
		{			
			DWORD		dwCount; //属性字段个数
			//PROP_ITEM arItem[m_dwCount]; //属性字段数组
		};

		DWORD				m_dwRef;
		PROP_DATA *			m_pData;
	};
};