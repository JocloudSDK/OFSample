#include "stable.h"
#include "ViewImpl.h"
#include "EventDispatch.h"
#include "RowArrayImpl.h"
#include <assert.h>


namespace Data
{
	CViewImpl::CViewImpl(CTableEditImpl *pTableEdit, CViewDataImpl *pViewData)
	{
		pTableEdit->addRef();
		m_pTableEdit = pTableEdit;
		m_pViewData = pViewData;

		m_dwRef = 1;
	}

	CViewImpl::~CViewImpl(void)
	{
		m_pTableEdit->release();
		m_pTableEdit = NULL;
		
		m_pViewData = NULL;
	}

	CViewImpl * CViewImpl::createInstance( CTableEditImpl *pTableEdit, CViewDataImpl *pViewData)
	{
		return new CViewImpl(pTableEdit, pViewData);
	}

	void CViewImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CViewImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CViewImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CViewImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	void CViewImpl::setFilter(const CFilter *pFilter)
	{
		CTableDataImpl *pTableData = m_pTableEdit->getTableData();
		
		pTableData->enterEditCri();

		IRowArrayPtr pRowArrayReset;
		IRowArrayPtr pRowArrayAdd;
		
		MATCH_NODE * pFilterOrig = NULL;
		const MATCH_NODE * pFilterNew = m_pViewData->setFilter(pFilter, &pFilterOrig);

		if (pFilterOrig)
		{
			pRowArrayReset = m_pTableEdit->IView_select(pFilterOrig);
			CConditionExpr::destroyMatchTree(pFilterOrig);
			pFilterOrig = NULL;
		}

		if (pFilterNew)
			pRowArrayAdd = m_pTableEdit->IView_select(pFilterNew);

		//抛出异步事件
		if (pRowArrayReset)
			CEventDispatch::GetInstance()->QueuedEmit(&getQueuedSigReset(), pRowArrayReset);
		if (pRowArrayAdd)
			CEventDispatch::GetInstance()->QueuedEmit(&getQueuedSigAddList(), pRowArrayAdd);


		pTableData->leaveEditCri();//抛出同步事件前必须解锁

		//抛出同步事件
		if (pRowArrayReset)
			CEventDispatch::GetInstance()->Emit(&getSigReset(), pRowArrayReset);
		if (pRowArrayAdd)
			CEventDispatch::GetInstance()->Emit(&getSigAddList(), pRowArrayAdd);
	}

	void CViewImpl::setChangedEventFilter(const NAME_ID *pColNames, UINT nColCount)
	{
		CTableDataImpl *pTableData = m_pTableEdit->getTableData();
		pTableData->enterEditCri();
		
		m_pViewData->setChangedEventFilter(pColNames, nColCount);

		pTableData->leaveEditCri();
	}

	IRowPtr CViewImpl::row(const KEY &key)
	{
		IRowPtr pRet;

		CTableDataImpl *pTableData = m_pTableEdit->getTableData();
		pTableData->enterEditCri();
		
		const MATCH_NODE * pMatchTree = m_pViewData->getCurFilter();
		if (pMatchTree)
			pRet = m_pTableEdit->IView_row(key, pMatchTree);

		pTableData->leaveEditCri();

		return pRet;
	}


	BOOL CViewImpl::hasRow(const KEY &key)
	{
		BOOL bRet = FALSE;

		CTableDataImpl *pTableData = m_pTableEdit->getTableData();
		pTableData->enterEditCri();
		
		const MATCH_NODE * pMatchTree = m_pViewData->getCurFilter();
		if (pMatchTree)
			bRet = m_pTableEdit->IView_hasRow(key, pMatchTree);

		pTableData->leaveEditCri();

		return bRet;
	}

	IRowArrayPtr CViewImpl::all()
	{
		IRowArrayPtr pRet;

		CTableDataImpl *pTableData = m_pTableEdit->getTableData();
		pTableData->enterEditCri();
		
		const MATCH_NODE * pMatchTree = m_pViewData->getCurFilter();
		if (pMatchTree)
			pRet = m_pTableEdit->IView_select(pMatchTree);

		pTableData->leaveEditCri();

		if (!pRet)
			pRet.attach(CRowArrayImpl::CreateInstance(pTableData));

		return pRet;
	}

	void CViewImpl::setFilterExcept( const NAME_IDS& pColNames )
	{
		CTableDataImpl *pTableData = m_pTableEdit->getTableData();
		pTableData->enterEditCri();

		m_pViewData->setFilterExcept(pColNames);

		pTableData->leaveEditCri();
	}

};
