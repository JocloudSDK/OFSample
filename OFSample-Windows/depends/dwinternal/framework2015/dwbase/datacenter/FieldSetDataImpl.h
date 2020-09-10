#pragma once

#include "IFieldSet.h"
#include "IFieldSetData.h"
#include "FieldSetImpl.h"
#include "BitStore.h"
#include "DataCenterPub.h"
#include "FieldString.h"

namespace Data
{
	class CFieldSetDataImpl : public IFieldSetData
	{
	public:
		static CFieldSetDataImpl * createInstance(CFieldSetImpl *pSetEdit);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef *		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		//读字段
		virtual BOOL			hasData(const NAME_ID &name);
		virtual LPCWSTR			getStr(const NAME_ID &name);
		virtual DWORD64			getDword64(const NAME_ID &name);
		virtual DWORD			getDword(const NAME_ID &name);
		virtual WORD			getWord(const NAME_ID &name);
		virtual BYTE			getByte(const NAME_ID &name);
		virtual BOOL			getBool(const NAME_ID &name);
		virtual const BYTE *	getBlob(const NAME_ID &name, DWORD * lpdwSize);
		virtual comptr<ITable>	getSubTable(const NAME_ID &name);
		virtual BOOL			isModified(const NAME_ID &name);
	
		virtual void			dump();//用于调试器下很方便的查看内容
		
		virtual IFieldSetPtr	getEdit() { return m_pSetEdit; }
		virtual const NAME_ID	getFieldSetName() { return m_pSetEdit->getFieldSetName(); }

		//写字段，给CFieldSetImpl用
		BOOL					setStr(const NAME_ID &name, LPCWSTR lpwszValue);
		BOOL					setDword64(const NAME_ID &name, DWORD64 dw64Value);
		BOOL					setDword(const NAME_ID &name, DWORD dwValue);
		BOOL					setWord(const NAME_ID &name, WORD wValue);
		BOOL					setByte(const NAME_ID &name, BYTE byValue);
		BOOL					setBool(const NAME_ID &name, BOOL bValue);
		BOOL					setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize);
		BOOL					setSubTable(const NAME_ID &name, ITableEdit *pSubTable);
		BOOL					removeData(const NAME_ID &name);
		
		void					init();

		CFieldSetDataImpl *		clone() const; //不会复制修改状态

		DWORD					getRefCount() const { return m_dwRef; }
		DWORD					internalrelease();//内部释放，同release差别在于不会释放父对象CFieldSetImpl的引用计数
		inline BOOL				isModified() const { return !m_bitModified.isAllZero(); }
	
		void					getContentStr(std::wstring &wstrOut);

	private:
		CFieldSetDataImpl(CFieldSetImpl *pSetEdit);
		~CFieldSetDataImpl(void);
		
		BOOL					getXXX(const NAME_ID &name, E_FIELD type, DWORD *lpdwDum);
		BOOL					setXXX(const NAME_ID &name, E_FIELD type, DWORD dwDum1, DWORD dwDum2 = 0);

		DWORD					m_dwRef;

		DWORD *					m_pValue;
		CBitStore				m_bitExist;
		CBitStore				m_bitModified;
		CFieldSetImpl *			m_pSetEdit;//不保持引用计数，除非存在外部用户引用对象时，m_pSetEdit引用计数才addRef
	};
};
