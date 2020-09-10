#pragma once

#include "ConditionExpr.h"
#include "BitStore.h"
#include "TableDataImpl.h"

namespace Data
{
	class CViewDataImpl : public IUnk
	{
	public:
		static CViewDataImpl * createInstance(const NAME_ID &viewName, CTableDataImpl * pTableData);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual IWeakRef*		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		const MATCH_NODE * 		setFilter(const CFilter *pFilter, MATCH_NODE **ppFilterOrig);
		void					setChangedEventFilter(const NAME_ID *pColNames, UINT nColCount);
		const MATCH_NODE *		getCurFilter();

		inline const NAME_ID	getViewName() { return m_viewName; }

		xsignals::signal<void (IRowArrayPtr)> * getSigAddList() { return m_eventCollect.getSigAddList(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr)> * getSigDelList() { return m_eventCollect.getSigDelList(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr)> * getSigChangedList() { return m_eventCollect.getSigChangedList(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr)> * getSigReset() { return m_eventCollect.getSigReset(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigAddList()  { return m_eventCollect.getQueuedSigAddList(m_pTableData->getEditCri()); } 
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigDelList() { return m_eventCollect.getQueuedSigDelList(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigChangedList() { return m_eventCollect.getQueuedSigChangedList(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr)> * getQueuedSigReset() { return m_eventCollect.getQueuedSigReset(m_pTableData->getEditCri()); }

		xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> * getSigChangedListEx() { return m_eventCollect.getSigChangedListEx(m_pTableData->getEditCri()); }
		xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> * getQueuedSigChangedListEx() { return m_eventCollect.getQueuedSigChangedListEx(m_pTableData->getEditCri()); }

		void					setFilterExcept(const NAME_IDS &pColNames);

		void					onTableDataInit();
		void					onEndEditMode();
		void					onRowEvent(CRowImpl *pRowCur, CRowImpl * pRowOrig, E_ROW_EVENT eRowEvent);//pRowOrig参数仅仅当Row改变事件有效
		void					onTableResetEvent(const std::vector<CRowImpl *> & vecRow);

	private:
		CViewDataImpl(const NAME_ID &viewName, CTableDataImpl * pTableData);
		~CViewDataImpl(void);

		void					doInitColFilter();
		void					doExceptFilter();

		DWORD					m_dwRef;
		NAME_ID					m_viewName;

		CTableDataImpl *		m_pTableData;//没有保持引用

		CEventCollect			m_eventCollect;	//批量事件收集

		CConditionExpr			m_filter;
		MATCH_NODE *			m_pMatchTree;

		std::vector<NAME_ID>	m_vecColFilter;
		CBitStore				m_bitColFilter;
		std::vector<NAME_ID>	m_vecColFilterExcept;
	};
};