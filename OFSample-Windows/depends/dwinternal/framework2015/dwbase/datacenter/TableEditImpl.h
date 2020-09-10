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
		

		//���ֶ�
		virtual ITablePtr			getTable();

		//�༭
		virtual void				beginEdit();//���û�г�ʼ��������checkStructInitialized��ʼ��
		virtual void				endEdit();
		virtual IRowEditPtr			row(const KEY &key);
		virtual IRowEditPtr			allocRow();//����ҪbeginEdit�����û�г�ʼ��������checkStructInitialized��ʼ��
		virtual INSERT_RES			insert(const KEY &key);
		virtual BOOL				insert(IRowEdit *pRowEdit);
		virtual BOOL				remove(const KEY &key);
		virtual void				reset();//ǿ�����Table���������row
		virtual BOOL				hasRow(const KEY &key);

		virtual IRowEditArrayPtr	all();
		virtual IRowEditArrayPtr	select(const CFilter *pFilter, const CFilterOption * pFilterOption);
		virtual DWORD				size();

	
		//�����ֶ�
		virtual void				setVirtualFieldHandler(IVirtualFieldHandler *pHandler);
		virtual void				removeVirtualFieldHandler(IVirtualFieldHandler *pHandler);

		virtual const NAME_ID		getTableName();
		virtual const NAME_ID		getIndexColName(); //���û�г�ʼ��������checkStructInitialized��ʼ��

		virtual E_INDEX_TYPE		getIndexType(); //���û�г�ʼ��������checkStructInitialized��ʼ��
		virtual void				getCols(std::vector<COL_PROP> & cols); //���û�г�ʼ��������checkStructInitialized��ʼ��

		virtual void				dump();

		BOOL						checkStructInitialized();
		inline BOOL					isStructInitialized() const { return m_bStructInitialized; }
		CTableDataImpl *			getTableData() { return m_pTableData; }

		//��Щ�����ṩ��ITable��
		IRow *						ITable_row(const KEY &key);
		IRowArrayPtr				ITable_all();
		IRowArrayPtr				ITable_select(const CFilter *pFilter, const CFilterOption * pFilterOption);
		IViewPtr					ITable_getView(const NAME_ID& viewName);


		//�ṩ��View��
		IRowPtr						IView_row(const KEY &key, const MATCH_NODE * pMatchTree);
		IRowArrayPtr				IView_select(const MATCH_NODE * pMatchTree);//���������Ѿ������༭��
		BOOL						IView_hasRow(const KEY &key, const MATCH_NODE * pMatchTree);

		CRowImpl *					detachRow(const STORE_CONTEXT &storeContext, CRowImpl *pRowOrig);

		void						dumpSummary();
	private:
		CTableEditImpl(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext);
		~CTableEditImpl(void);
		BOOL						verifyKeyTypeValid(E_KEY_TYPE eKeyType);
		void						selectHelper(const CFilter *pFilter, const CFilterOption * pFilterOption, CRowArrayImpl *pArrRow, CRowEditArrayImpl *pArrRowEdit);
		
		//�����Ա༭
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

		CTableDataImpl *		m_pTableData;			//���������ü���
		BOOL					m_bStructInitialized;	//��ṹ�Ƿ��Ѿ�����ʼ����������ֻ�жϸñ�־���༭���ݵ�ʱ��Ż�ȥ��ʼ����ṹ
		CTableImpl				m_table;
	};
};
