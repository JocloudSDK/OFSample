#pragma once

#include "ITableEdit.h"
#include "ITable.h"
#include "TableDataImpl.h"
#include "TableEditImpl.h"
#include "DataCenterPub.h"
#include "RowImpl.h"

namespace Data
{

	class CRowEditImpl : public IRowEdit
	{
	public:
		static CRowEditImpl*	createInstance(CTableEditImpl * pTableEdit, const CTableEditImpl::STORE_CONTEXT &storeContext, CRowImpl *pRow);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef*		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		//写字段
		virtual BOOL			setStr(const NAME_ID &name, LPCWSTR lpwszValue);
		virtual BOOL			setDword64(const NAME_ID &name, DWORD64 dw64Value);
		virtual BOOL			setDword(const NAME_ID &name, DWORD dwValue);
		virtual BOOL			setWord(const NAME_ID &name, WORD wValue);
		virtual BOOL			setByte(const NAME_ID &name, BYTE byValue);
		virtual BOOL			setBool(const NAME_ID &name, BOOL bValue);
		virtual BOOL			setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize);
		virtual BOOL			setSubTable(const NAME_ID &name, ITableEdit *pSubTable);
		virtual BOOL			removeData(const NAME_ID &name);

		//读字段
		virtual BOOL			hasData(const NAME_ID &name);
		virtual LPCWSTR			getStr(const NAME_ID &name);
		virtual DWORD64			getDword64(const NAME_ID &name);
		virtual DWORD			getDword(const NAME_ID &name);
		virtual WORD			getWord(const NAME_ID &name);
		virtual BYTE			getByte(const NAME_ID &name);
		virtual BOOL			getBool(const NAME_ID &name);
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize);
		virtual comptr<ITableEdit>	getSubTable(const NAME_ID &name);
		
		virtual void			dump();//用于调试器下很方便的查看Row的内容

		//读属性
		virtual BOOL			hasProp(const NAME_ID &name);
		virtual LPCWSTR			getPropStr(const NAME_ID &name);
		virtual DWORD64			getPropDword64(const NAME_ID &name);
		virtual DWORD			getPropDword(const NAME_ID &name);
		virtual WORD			getPropWord(const NAME_ID &name);
		virtual BYTE			getPropByte(const NAME_ID &name);
		virtual BOOL			getPropBool(const NAME_ID &name);
		virtual comptr<IUnk>	getPropUnk(const NAME_ID &name);

		//写属性
		virtual BOOL			setPropStr(const NAME_ID &name, LPCWSTR lpwszValue);
		virtual BOOL			setPropDword64(const NAME_ID &name, DWORD64 dw64Value);
		virtual BOOL			setPropDword(const NAME_ID &name, DWORD dwValue);
		virtual BOOL			setPropWord(const NAME_ID &name, WORD wValue);
		virtual BOOL			setPropByte(const NAME_ID &name, BYTE byValue);
		virtual BOOL			setPropBool(const NAME_ID &name, BOOL bValue);
		virtual BOOL			setPropUnk(const NAME_ID &name, IUnk *pUnk);
		virtual BOOL			removeProp(const NAME_ID &name);

		inline CRowImpl *		getRowObj() const { return m_pRow; }

	private:
		CRowEditImpl(CTableEditImpl * pTableEdit, const CTableEditImpl::STORE_CONTEXT &storeContext, CRowImpl *pRow);
		~CRowEditImpl(void);

		void					detach();			
		BOOL					verifyEditEnable(const NAME_ID &name);

		DWORD					m_dwRef;
		CRowImpl *				m_pRow;
		CTableEditImpl *		m_pTableEdit;

		CTableEditImpl::STORE_CONTEXT m_storeContext;
	};
};
