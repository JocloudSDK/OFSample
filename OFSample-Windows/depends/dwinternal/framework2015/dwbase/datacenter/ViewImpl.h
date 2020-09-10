#pragma once

#include "IView.h"
#include "ViewDataImpl.h"
#include "TableEditImpl.h"

namespace Data
{
	class CViewImpl : public IView
	{
	public:
		static CViewImpl * createInstance(CTableEditImpl *pTableEdit, CViewDataImpl *pViewData);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual IWeakRef*		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		virtual void			setFilter(const CFilter *pFilter);
		virtual void			setChangedEventFilter(const NAME_ID *pColNames, UINT nColCount);

		virtual IRowPtr			row(const KEY &key);
		virtual BOOL			hasRow(const KEY &key);
		virtual IRowArrayPtr	all();

		virtual ITablePtr		getOwner() { return m_pTableEdit->getTable(); }
		virtual const NAME_ID	getViewName() { return m_pViewData->getViewName(); }

		virtual xsignals::signal<void (IRowArrayPtr)> & getSigAddList() { return *m_pViewData->getSigAddList(); }
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigDelList() { return *m_pViewData->getSigDelList(); }
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigChangedList() { return *m_pViewData->getSigChangedList(); }
		virtual xsignals::signal<void (IRowArrayPtr)> & getSigReset() { return *m_pViewData->getSigReset(); }
		virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigAddList()  { return *m_pViewData->getQueuedSigAddList(); } 
		virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigDelList() { return *m_pViewData->getQueuedSigDelList(); }
		virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigChangedList() { return *m_pViewData->getQueuedSigChangedList(); }
		virtual xsignals::signal<void (IRowArrayPtr)> & getQueuedSigReset() { return *m_pViewData->getQueuedSigReset(); }

		virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getSigChangedListEx() { return *m_pViewData->getSigChangedListEx(); }
		virtual xsignals::signal<void (IRowArrayPtr pArrCur, IRowArrayPtr pArrOrig)> & getQueuedSigChangedListEx() { return *m_pViewData->getQueuedSigChangedListEx(); }

		virtual void			setFilterExcept(const NAME_IDS &pColNames);
	private:
		CViewImpl(CTableEditImpl *pTableEdit, CViewDataImpl *pViewData);
		~CViewImpl(void);

		DWORD					m_dwRef;
		CViewDataImpl *			m_pViewData;
		CTableEditImpl *		m_pTableEdit;		//没有保持引用计数，TableView归TableEdit对象所有
	};
};