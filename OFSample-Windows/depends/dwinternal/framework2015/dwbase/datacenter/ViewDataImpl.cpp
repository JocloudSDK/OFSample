#include "stable.h"
#include "RowImpl.h"
#include "TableDataImpl.h"
#include "RowArrayImpl.h"
#include "ViewDataImpl.h"
#include "EventDispatch.h"
#include <assert.h>
#include <algorithm>


namespace Data
{
	CViewDataImpl::CViewDataImpl(const NAME_ID &viewName, CTableDataImpl * pTableData)
	{
		assert(!viewName.IsEmpty() && pTableData != NULL);

		m_viewName = viewName;
		m_pTableData = pTableData;

		m_dwRef = 1;
		m_pMatchTree = NULL;
	}

	CViewDataImpl::~CViewDataImpl(void)
	{
		m_pTableData = NULL;
	
		CConditionExpr::destroyMatchTree(m_pMatchTree);
		m_pMatchTree = NULL;
	}

	CViewDataImpl * CViewDataImpl::createInstance(const NAME_ID &viewName, CTableDataImpl * pTableData)
	{
		return new CViewDataImpl(viewName, pTableData);
	}

	void CViewDataImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CViewDataImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CViewDataImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CViewDataImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	const MATCH_NODE * CViewDataImpl::setFilter(const CFilter *pFilter, MATCH_NODE **ppFilterOrig)
	{
		*ppFilterOrig = NULL;
		
		if (!m_pTableData->verifyEditIsLocked())
			return NULL;

		if (m_filter.isEqual(pFilter->_private()))
		{
			//assert(! L"视图设置的Filter相等，重复设置!!");
			return NULL;
		}

		m_filter = *pFilter->_private();

		if (m_pTableData->isStructInitialized())
		{
			*ppFilterOrig = m_pMatchTree;
			m_pMatchTree = m_filter.convertToMatchTree(m_pTableData->getRealCols());
		}
		else
		{
			assert(!m_pMatchTree);
		}

		return m_pMatchTree;
	}

	void CViewDataImpl::setChangedEventFilter(const NAME_ID *pColNames, UINT nColCount)
	{
		assert(nColCount > 0);
		if (!m_pTableData->verifyEditIsLocked())
			return;

		std::set<NAME_ID> setUnique;
		for (UINT i = 0; i < nColCount; i++)
		{
			if (!setUnique.insert(pColNames[i]).second)
			{
				assert(! L"setChangedEventFilter不能指定重复列");
				return;
			}
		}
		
		for (UINT i = 0; i < nColCount; i++)
			m_vecColFilter.push_back(pColNames[i]);

		if (m_pTableData->isStructInitialized())
		{
			const CTableDataImpl::REAL_COLS * pCols = m_pTableData->getRealCols();
			m_bitColFilter.init(pCols->dwColCount);
			for (UINT i = 0; i < m_vecColFilter.size(); i++)
			{
				BOOL bExist = FALSE;
				for (UINT j = 0; j < pCols->dwColCount; j++)
				{
					if (m_vecColFilter[i] == (WORD)pCols->arCol[j].dwId)
					{
						bExist = TRUE;
						m_bitColFilter.setBit(j, TRUE);
						break;
					}
				}

				if (!bExist)
					assert(! L"setChangedEventFilter指定了不存在的列名");
			}
		}
	}
	
	const MATCH_NODE * CViewDataImpl::getCurFilter()
	{
		if (!m_pTableData->verifyEditIsLocked())
			return NULL;

		return m_pMatchTree;
	}

	void CViewDataImpl::onTableDataInit()
	{
		if (!m_filter.isEmpty())
		{
			assert(m_pMatchTree == NULL);
			m_pMatchTree = m_filter.convertToMatchTree(m_pTableData->getRealCols());
		}

		if (!m_vecColFilter.empty())
		{
			assert(!m_bitColFilter.isInitialized());

			const CTableDataImpl::REAL_COLS * pCols = m_pTableData->getRealCols();
			m_bitColFilter.init(pCols->dwColCount);
			for (UINT i = 0; i < m_vecColFilter.size(); i++)
			{
				BOOL bExist = FALSE;
				for (UINT j = 0; j < pCols->dwColCount; j++)
				{
					if (m_vecColFilter[i] == (WORD)pCols->arCol[j].dwId)
					{
						bExist = TRUE;
						m_bitColFilter.setBit(j, TRUE);
						break;
					}
				}

				if (!bExist)
					assert(! L"setChangedEventFilter指定了不存在的列名");
			}
		}

		if (!m_vecColFilterExcept.empty())
		{
			doExceptFilter();
		}
	}

	void CViewDataImpl::onEndEditMode()
	{
		m_eventCollect.OnTrigger(m_pTableData);
	}
		
	void CViewDataImpl::onRowEvent(CRowImpl * pRowCur, CRowImpl * pRowOrig, E_ROW_EVENT eRowEvent)
	{
		if (m_pMatchTree || m_bitColFilter.isInitialized())
		{
			switch(eRowEvent)
			{
			case ROW_E_ADD:
			case ROW_E_DEL:
				{
					assert(!pRowOrig);
					if (m_pMatchTree == NULL || pRowCur->isMatch(m_pMatchTree))
						m_eventCollect.AddRowEvent(pRowCur, NULL, eRowEvent);
				}
				break;
			case ROW_E_CHANGED:
				{
					if (m_pMatchTree != NULL)
					{
						BOOL bMatchOrigVer = pRowOrig->isMatch(m_pMatchTree);
						BOOL bMatchCurVer = pRowCur->isMatch(m_pMatchTree);
						
						if (bMatchOrigVer && bMatchCurVer)
						{
							if (m_bitColFilter.isInitialized())
							{
								if (Bitmap::isIntersect(m_bitColFilter.getMemPtr(), pRowCur->getModifiedBit(), m_bitColFilter.getMemSize()))
									m_eventCollect.AddRowEvent(pRowCur, pRowOrig, ROW_E_CHANGED);		
							}
							else
							{
								m_eventCollect.AddRowEvent(pRowCur, pRowOrig, ROW_E_CHANGED);
							}
						}
						else if (bMatchOrigVer && !bMatchCurVer)
						{
							m_eventCollect.AddRowEvent(pRowCur, NULL, ROW_E_DEL);	
						}
						else if (!bMatchOrigVer && bMatchCurVer)
						{
							m_eventCollect.AddRowEvent(pRowCur, NULL, ROW_E_ADD);
						}
						else
						{
							assert(!bMatchOrigVer && !bMatchCurVer);
						}
					}
					else
					{
						if (Bitmap::isIntersect(m_bitColFilter.getMemPtr(), pRowCur->getModifiedBit(), m_bitColFilter.getMemSize()))
							m_eventCollect.AddRowEvent(pRowCur, pRowOrig, ROW_E_CHANGED);	
					}
				}
				break;
			default:
				assert(! L"不支持的事件类型");
				break;
			}
		}
	}

	void CViewDataImpl::onTableResetEvent(const std::vector<CRowImpl *> & vecRow)
	{
		if (m_pMatchTree || m_bitColFilter.isInitialized())
		{
			std::vector<CRowImpl *> vecSel;
			for (UINT i = 0; i < vecRow.size(); i++)
			{
				CRowImpl * pRow = vecRow[i];
				if (m_pMatchTree == NULL || pRow->isMatch(m_pMatchTree))
					vecSel.push_back(pRow);
			}

			m_eventCollect.AddResetEvent(vecSel);
		}
	}

	void CViewDataImpl::setFilterExcept( const NAME_IDS& pColNames )
	{
		if (!m_pTableData->verifyEditIsLocked())
			return;

		for (UINT i = 0; i < pColNames.count(); i++)
		{
			m_vecColFilterExcept.push_back(pColNames.item(i));
		}

		const CTableDataImpl::REAL_COLS * pCols = m_pTableData->getRealCols();

		BOOL hasInit = m_pTableData->isStructInitialized();
		if (hasInit)
		{
			m_bitColFilter.init(pCols->dwColCount);
		}

		for (UINT j = 0; j < pCols->dwColCount; j++)
		{
			BOOL bExcept = FALSE;
			for (UINT i = 0; i < pColNames.count(); i++)
			{
				if (pColNames.item(i) == (WORD)pCols->arCol[j].dwId)
				{
					bExcept = TRUE;
					break;
				}
			}

			if (bExcept == FALSE && hasInit)
			{
				m_bitColFilter.setBit(j, TRUE);
			}
		}
	}

	void CViewDataImpl::doExceptFilter()
	{
		const CTableDataImpl::REAL_COLS * pCols = m_pTableData->getRealCols();

		BOOL hasInit = m_pTableData->isStructInitialized();
		if (hasInit)
		{
			m_bitColFilter.init(pCols->dwColCount);
		}

		for (UINT j = 0; j < pCols->dwColCount; j++)
		{
			BOOL bExcept = FALSE;
			for (UINT i = 0; i < m_vecColFilterExcept.size(); i++)
			{
				if (m_vecColFilterExcept[i] == (WORD)pCols->arCol[j].dwId)
				{
					bExcept = TRUE;
					break;
				}
			}

			if (bExcept == FALSE && hasInit)
			{
				m_bitColFilter.setBit(j, TRUE);
			}
		}
	}

};
