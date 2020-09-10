#pragma once

#include "ITableEdit.h"
#include "IView.h"
#include "StoreProxy.h"
#include "TableDataImpl.h"
#include "RowArrayImpl.h"
#include "RowEditArrayImpl.h"

namespace Data
{
	class CRowEditImpl;
	class CTableEditImpl : public ITableEdit
	{
	public:
		struct STORE_CONTEXT
		{
			DWORD	dwMidifiedVer;
			CRowImpl ** ppStoreValue;
		};

		static CTableEditImpl * createInstance(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext);

		//IUnk
		virtual void				addRef();
		virtual void				release();
		virtual	IWeakRef *			weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);
		

		//读字段
		virtual ITablePtr			getTable();

		//编辑
		virtual void				beginEdit();//如果没有初始化，调用checkStructInitialized初始化
		virtual void				endEdit();
		virtual IRowEditPtr			row(const KEY &key);
		virtual IRowEditPtr			allocRow();//不需要beginEdit，如果没有初始化，调用checkStructInitialized初始化
		virtual INSERT_RES			insert(const KEY &key);
		virtual BOOL				insert(IRowEdit *pRowEdit);
		virtual BOOL				remove(const KEY &key);
		virtual void				reset();//强制清空Table里面的所有row
		virtual BOOL				hasRow(const KEY &key);

		virtual IRowEditArrayPtr	all();
		virtual IRowEditArrayPtr	select(const CFilter *pFilter, const CFilterOption * pFilterOption);
		virtual DWORD				size();

	
		//虚拟字段
		virtual void				setVirtualFieldHandler(IVirtualFieldHandler *pHandler);
		virtual void				removeVirtualFieldHandler(IVirtualFieldHandler *pHandler);

		virtual const NAME_ID		getTableName();
		virtual const NAME_ID		getIndexColName(); //如果没有初始化，调用checkStructInitialized初始化

		virtual E_INDEX_TYPE		getIndexType(); //如果没有初始化，调用checkStructInitialized初始化
		virtual void				getCols(std::vector<COL_PROP> & cols); //如果没有初始化，调用checkStructInitialized初始化

		virtual void				dump();

		BOOL						checkStructInitialized();
		inline BOOL					isStructInitialized() const { return m_bStructInitialized; }
		CTableDataImpl *			getTableData() { return m_pTableData; }

		//这些函数提供给ITable用
		IRow *						ITable_row(const KEY &key);
		IRowArrayPtr				ITable_all();
		IRowArrayPtr				ITable_select(const CFilter *pFilter, const CFilterOption * pFilterOption);
		IViewPtr					ITable_getView(const NAME_ID& viewName);


		//提供给View用
		IRowPtr						IView_row(const KEY &key, const MATCH_NODE * pMatchTree);
		IRowArrayPtr				IView_select(const MATCH_NODE * pMatchTree);//假设外面已经锁定编辑锁
		BOOL						IView_hasRow(const KEY &key, const MATCH_NODE * pMatchTree);

		CRowImpl *					detachRow(const STORE_CONTEXT &storeContext, CRowImpl *pRowOrig);

		void						dumpSummary();
	private:
		CTableEditImpl(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext);
		~CTableEditImpl(void);
		BOOL						verifyKeyTypeValid(E_KEY_TYPE eKeyType);
		void						selectHelper(const CFilter *pFilter, const CFilterOption * pFilterOption, CRowArrayImpl *pArrRow, CRowEditArrayImpl *pArrRowEdit);
		
		//表属性编辑
		BOOL						init(const TABLE_STRUCT *pTableStruct);

		struct CTableImpl : public ITable
		{
			//IUnk
			virtual void			addRef();
			virtual void			release();
			virtual IWeakRef*		weakPtr();
			virtual bool queryInterface(REFDWUUID iid, void **ppv);

			//ITable
			virtual IRowPtr			row(const KEY &key);
			virtual BOOL			hasRow(const KEY &key);
			virtual IRowArrayPtr	all();
			virtual IRowArrayPtr	select(const CFilter *pFilter, const CFilterOption * pFilterOption);

			virtual ITableEditPtr	getEdit();

			virtual IViewPtr		getView(const NAME_ID& viewName);

			virtual const NAME_ID	getTableName();

			virtual void			dump();

			virtual xsignals::signal<void (IRowArrayPtr)> & getSigAddList() { return *getParent()->getTableData()->getSigAddList(); }
			virtual xsignals::signal<void (IRowArrayPtr)> & getSigDelList() { return *getParent()->getTableData()->getSigDelList(); }
			virtual xsignals::signal<void (IRowArrayPtr)> & getSigChangedList() { return *getParent()->getTableData()->getSigChangedList(); }
			virtual xsignals::signal<void (IRowArrayPtr)> & getSigReset() { return *getParent()->getTableData()->getSigReset(); }
			virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigAddList()  { return *getParent()->getTableData()->getQueuedSigAddList(); } 
			virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigDelList() { return *getParent()->getTableData()->getQueuedSigDelList(); }
			virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigChangedList() { return *getParent()->getTableData()->getQueuedSigChangedList(); }
			virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigReset() { return *getParent()->getTableData()->getQueuedSigReset(); }

			virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getSigChangedListEx() { return *getParent()->getTableData()->getSigChangedListEx(); }
			virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getQueuedSigChangedListEx() { return *getParent()->getTableData()->getQueuedSigChangedListEx(); }


			CTableEditImpl *		getParent();
		};

		DWORD					m_dwRef;
		CStoreProxy<CRowImpl> *	m_pStore;
		DWORD					m_dwStoreMidifiedVer;

		CTableDataImpl *		m_pTableData;			//保持了引用计数
		BOOL					m_bStructInitialized;	//表结构是否已经被初始化，读数据只判断该标志，编辑数据的时候才会去初始化表结构
		CTableImpl				m_table;
	};
};
