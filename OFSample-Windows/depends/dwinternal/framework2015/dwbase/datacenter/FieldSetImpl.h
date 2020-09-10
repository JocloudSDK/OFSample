#pragma once

#include "IFieldSet.h"
#include "ITableEdit.h"
#include "TableDataImpl.h"
#include "DataCenterPub.h"
#include "CriticalSection.h"

namespace Data
{
	class CFieldSetDataImpl;

	class CFieldSetImpl : public IFieldSet
	{
	public:
		struct COLS
		{
			BYTE * 			arType; 	//类型
			WORD * 			arId;		//id表
			DWORD			dwCount;
		};

		static CFieldSetImpl*	createInstance(const NAME_ID &fieldSetName, CDatabaseContextImpl *pDatabaseContext);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef*		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		virtual IFieldSetDataPtr	getData();

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


		virtual void			beginEdit();
		virtual void			endEdit();

		virtual const NAME_ID	getFieldSetName() { return m_fieldSetName; }
		virtual void			getCols(std::vector<COL_PROP> & cols);

		//事件信号
		virtual xsignals::signal<void (comptr<IFieldSetData>)> & getSigChanged() { return m_sigChanged; }
		virtual xsignals::signal<void (comptr<IFieldSetData>)> & getQueuedSigChanged() { return m_sigQueuedChanged; }

		BOOL					init(const FIELDSET_STRUCT *pStruct);	
		BOOL					findCol(const NAME_ID &col, DWORD *lpIndex, E_FIELD *lpType);
		inline const COLS *		getCols() const { return &m_cols; }
		void					getContentStr(std::wstring &wstrOut);
		inline 	BOOL			isStructInitialized() const { return m_bStructInitialized; }

	private:
		CFieldSetImpl(const NAME_ID &fieldSetName, CDatabaseContextImpl *pDatabaseContext);
		~CFieldSetImpl(void);

		BOOL					checkStructInitialized();

		void					detach();			
		BOOL					verifyEditEnable();


		DWORD					m_dwRef;
		CCriticalLock			m_csEditLock;

		NAME_ID					m_fieldSetName;
		COLS					m_cols;
	
		CFieldSetDataImpl *		m_pSetData;		
		BOOL					m_bCloneDataOnWrite; //是否因为修改已经克隆了一份数据

		BOOL					m_bStructInitialized;
		CDatabaseContextImpl *	m_pDatabaseContext;	//保持了引用计数

		xsignals::signal<void (comptr<IFieldSetData>)> m_sigChanged;
		xsignals::signal<void (comptr<IFieldSetData>)> m_sigQueuedChanged;
	};
};
