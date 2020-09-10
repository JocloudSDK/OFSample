#include "stable.h"
#include "RowImpl.h"
#include "TableEditImpl.h"
#include "RowEditImpl.h"
#include "ConditionExpr.h"
#include "FilterOptionPrivate.h"
#include "ViewImpl.h"
#include <assert.h>
#include <algorithm>
#include "DatabaseLeakDetector.h"

const DWORD c_dwInvalidVer = 0;

namespace Data
{
	//ITable 实现 -------------------------------------------------------------
	void CTableEditImpl::CTableImpl::addRef()
	{
		getParent()->addRef();
	}

	void CTableEditImpl::CTableImpl::release()
	{
		getParent()->release();
	}

	IWeakRef* CTableEditImpl::CTableImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CTableEditImpl::CTableImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	IRowPtr	CTableEditImpl::CTableImpl::row(const KEY &key)
	{
		IRowPtr ptr;
		ptr.attach(getParent()->ITable_row(key));
		return ptr;
	}

	BOOL CTableEditImpl::CTableImpl::hasRow(const KEY &key)
	{
		return getParent()->hasRow(key);
	}

	IRowArrayPtr CTableEditImpl::CTableImpl::all()
	{
		return getParent()->ITable_all();
	}

	IRowArrayPtr CTableEditImpl::CTableImpl::select(const CFilter *pFilter, const CFilterOption * pFilterOption)
	{
		return getParent()->ITable_select(pFilter, pFilterOption);
	}

	ITableEditPtr CTableEditImpl::CTableImpl::getEdit()
	{
		return getParent();
	}

	IViewPtr CTableEditImpl::CTableImpl::getView(const NAME_ID& viewName)
	{
		return getParent()->ITable_getView(viewName);
	}

	const NAME_ID CTableEditImpl::CTableImpl::getTableName()
	{
		return getParent()->getTableName();
	}

	void CTableEditImpl::CTableImpl::dump()
	{
		getParent()->dump();
	}

	CTableEditImpl *	CTableEditImpl::CTableImpl::getParent()
	{
		return (CTableEditImpl *)((BYTE *)this - offsetof(CTableEditImpl, m_table));
	}



	//ITableEdit 实现 -------------------------------------------------------------
	CTableEditImpl::CTableEditImpl(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext)
	{
		DBLEAK_INSERT_TABLEEDIT(pDatabaseContext, this);

		m_dwRef = 1;
		m_pStore = NULL;
		m_bStructInitialized = FALSE;
		m_pTableData = CTableDataImpl::createInstance(tableName, pDatabaseContext);
		m_dwStoreMidifiedVer = c_dwInvalidVer + 1;
	}

	CTableEditImpl::~CTableEditImpl(void)
	{
		DBLEAK_REMOVE_PTR(m_pTableData->getDatabaseContext(), this);
		
		assert(m_dwRef == 0);

		if (m_pStore)
		{
			std::vector<CRowImpl *> vec;
			m_pStore->GetAll(vec);
			for (UINT i = 0; i < vec.size(); i++)
			{
				CRowImpl *pObj = vec[i];
				pObj->setRowState(CRowImpl::ROW_STATE_E_REMOVE);
				pObj->release();
			}
			m_pStore->Clear();
			delete m_pStore;
			m_pStore = NULL;
		}


		m_pTableData->release();
		m_pTableData = NULL;
	}

	CTableEditImpl * CTableEditImpl::createInstance(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext)
	{
		return new CTableEditImpl(tableName, pDatabaseContext);
	}

	void CTableEditImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CTableEditImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CTableEditImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CTableEditImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}


	ITablePtr CTableEditImpl::getTable()
	{
		return &m_table;
	}

	//编辑
	void CTableEditImpl::beginEdit()
	{
		if (!checkStructInitialized())
			return;

		m_pTableData->beginEditMode();
	}

	void CTableEditImpl::endEdit()
	{
		assert(m_bStructInitialized);
		if (!m_bStructInitialized)
			return;

		m_pTableData->endEditMode();
	}

	IRowEditPtr CTableEditImpl::row(const KEY &key)
	{
		if (!m_bStructInitialized)
			return NULL;

		IRowEditPtr pRowEdit;
		if (verifyKeyTypeValid(key.eType))
		{
			m_pTableData->enterEditCri();

			CRowImpl **ppRow = m_pStore->Find(key);
			if (ppRow)
			{
				STORE_CONTEXT ct;
				ct.dwMidifiedVer = m_dwStoreMidifiedVer;
				ct.ppStoreValue = ppRow;

				pRowEdit.attach(CRowEditImpl::createInstance(this, ct, *ppRow));
			}

			m_pTableData->leaveEditCri();
		}

		return pRowEdit;
	}

	IRowEditPtr CTableEditImpl::allocRow()
	{
		if (!checkStructInitialized())
			return NULL;

		IRowEditPtr pRowEdit;
		
		CRowImpl *pRowObj = CRowImpl::createInstance(m_pTableData);
		
		STORE_CONTEXT ct;
		ct.dwMidifiedVer = c_dwInvalidVer;
		ct.ppStoreValue = NULL;
		pRowEdit.attach(CRowEditImpl::createInstance(this, ct, pRowObj));
		pRowObj->release();
		pRowObj = NULL;

		return pRowEdit;
	}

	INSERT_RES CTableEditImpl::insert(const KEY &key)
	{
		INSERT_RES ret;
		ret.bInserted = FALSE;
		ret.pRow = NULL;

		assert(m_bStructInitialized);
		if (!m_bStructInitialized)
			return ret;

		if (verifyKeyTypeValid(key.eType) && m_pTableData->verifyEditEnable())
		{
			std::pair<CRowImpl **, BOOL> res = m_pStore->Insert(key, NULL);
			
			if (res.second)
			{
				m_dwStoreMidifiedVer++;

				CRowImpl *pRowObj = CRowImpl::createInstance(m_pTableData);
				pRowObj->setKey(key);
				*res.first = pRowObj;

				ret.bInserted = TRUE;

				STORE_CONTEXT ct;
				ct.dwMidifiedVer = m_dwStoreMidifiedVer;
				ct.ppStoreValue = res.first;
				ret.pRow.attach(CRowEditImpl::createInstance(this, ct, pRowObj));
				
				m_pTableData->notifyRowAdd(pRowObj);
			}
			else
			{
				ret.bInserted = FALSE;

				if (res.first)
				{
					STORE_CONTEXT ct;
					ct.dwMidifiedVer = m_dwStoreMidifiedVer;
					ct.ppStoreValue = res.first;
					
					ret.pRow.attach(CRowEditImpl::createInstance(this, ct, *res.first));
				}
			}
		}
		
		return ret;
	}

	BOOL CTableEditImpl::insert(IRowEdit *pRowEdit)
	{	
		assert(m_bStructInitialized);
		if (!m_bStructInitialized)
			return FALSE;


		CRowEditImpl * pRowEditObj = (CRowEditImpl *)pRowEdit;	

		//状态必须为ROW_STATE_E_ALLOC
		if (CRowImpl::ROW_STATE_E_ALLOC != pRowEditObj->getRowObj()->getRowState())
		{
			assert(! L"状态必须为ROW_STATE_E_ALLOC");
			return FALSE;
		}
		
		//Row不属于本表
		if (m_pTableData != pRowEditObj->getRowObj()->getTableData())
		{
			assert(! L"Row不属于本表");
			return FALSE;
		}

		if (m_pTableData->verifyEditEnable())
		{
			CRowImpl *pRowObj = pRowEditObj->getRowObj();
		
			KEY key;
			if (pRowObj->getKey(&key))
			{
				if (m_pStore->Insert(key, pRowObj).second)
				{
					m_dwStoreMidifiedVer++;

					pRowObj->addRef();
					m_pTableData->notifyRowAdd(pRowObj);

					return TRUE;
				}
			}
			else
			{
				LPCWSTR lpszName = m_pTableData->getTableName().GetName();
				Error::OutputError(_T("向表 %s 插入row时key不能为空！！"), lpszName ? lpszName : _T("空"));
			}
		}

		return FALSE;
	}

	BOOL CTableEditImpl::remove(const KEY &key)
	{
		assert(m_bStructInitialized);
		if (!m_bStructInitialized)
			return FALSE;

		if (verifyKeyTypeValid(key.eType) && m_pTableData->verifyEditEnable())
		{
			CRowImpl * pObj = m_pStore->Erase(key);
			if (pObj)
			{
				m_dwStoreMidifiedVer++;

				m_pTableData->notifyRowDel(pObj);
				pObj->release();
				return TRUE;
			}
		}
		return FALSE;
	}

	void CTableEditImpl::reset()
	{
		if (m_pTableData->verifyEditEnable())
		{
			std::vector<CRowImpl *> vec;
			m_pStore->GetAll(vec);
			m_pStore->Clear();
			m_dwStoreMidifiedVer++;

			m_pTableData->notifyTableReset(vec);

			for (UINT i = 0; i < vec.size(); i++)
			{
				CRowImpl *pRowObj = vec[i];
				pRowObj->setRowState(CRowImpl::ROW_STATE_E_REMOVE);
				pRowObj->release();
			}
		}
	}

	BOOL CTableEditImpl::hasRow(const KEY &key)
	{
		if (!m_bStructInitialized)
			return FALSE;

		BOOL bRet = FALSE;
		if (verifyKeyTypeValid(key.eType))
		{
			m_pTableData->enterEditCri();
			bRet = m_pStore->Find(key) != NULL;
			m_pTableData->leaveEditCri();
		}

		return bRet;
	}

	BOOL CTableEditImpl::init(const TABLE_STRUCT *pTableStruct)
	{
		assert(!m_pStore);
		if (m_pStore)
			return FALSE;

		if (m_pTableData->init(pTableStruct))
		{
			E_KEY_TYPE eKeyType = KEY_E_NULL;
			switch (m_pTableData->getIndexColType())
			{
			case FIELD_E_DWORD:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
				eKeyType = KEY_E_DWORD;
				break;
			case FIELD_E_STR:
				eKeyType = KEY_E_STR;
				break;
			default:
				break;
			}

			assert(eKeyType != KEY_E_NULL);
			if (eKeyType != KEY_E_NULL)
			{
				m_pStore = new CStoreProxy<CRowImpl>(eKeyType, pTableStruct->eIndexType);
				return TRUE;
			}
		}

		return FALSE;
	}

	const NAME_ID CTableEditImpl::getTableName()
	{ 
		return m_pTableData->getTableName();
	}

	const NAME_ID CTableEditImpl::getIndexColName()
	{ 
		if (!checkStructInitialized())
			return NAME_ID();

		return m_pTableData->getIndexColName();
	}

	E_INDEX_TYPE CTableEditImpl::getIndexType()
	{
		if (!checkStructInitialized())
			return INDEX_E_NULL;

		return m_pStore->GetIndexType();
	}

	void CTableEditImpl::getCols(std::vector<COL_PROP> & cols)
	{
		cols.clear();

		if (checkStructInitialized())
			m_pTableData->getCols(cols);
	}

	IRow * CTableEditImpl::ITable_row(const KEY &key)
	{	
		if (!m_bStructInitialized)
			return NULL;


		IRow *pRow = NULL;

		if (verifyKeyTypeValid(key.eType))
		{
			m_pTableData->enterEditCri();
			
			CRowImpl **ppObj = m_pStore->Find(key);
			if (ppObj)
			{
				(*ppObj)->addRef();
				pRow = *ppObj;
			}

			m_pTableData->leaveEditCri();
		}

		return pRow;
	}

	IRowArrayPtr CTableEditImpl::ITable_all()
	{
		CRowArrayImpl *pArrRow = CRowArrayImpl::CreateInstance(m_pTableData);
		IRowArrayPtr ptr;
		ptr.attach(pArrRow);

		if (!m_bStructInitialized)
			return ptr;

		m_pTableData->enterEditCri();

		std::vector<CRowImpl *> vec;
		m_pStore->GetAll(vec);
		
		pArrRow->reserve(vec.size());
		for (UINT i = 0; i < vec.size(); i++)
			pArrRow->push_back(vec[i]);
		
		m_pTableData->leaveEditCri();

		return ptr;
	}

	IRowEditArrayPtr CTableEditImpl::all()
	{
		CRowEditArrayImpl *pArrRow = CRowEditArrayImpl::CreateInstance();
		IRowEditArrayPtr ptr;
		ptr.attach(pArrRow);

		if (!m_bStructInitialized)
			return ptr;

		m_pTableData->enterEditCri();

		std::vector<CRowImpl **> vecAddr;
		m_pStore->GetAllAddr(vecAddr);
		pArrRow->reserve(vecAddr.size());
		
		for (UINT i = 0; i < vecAddr.size(); i++)
		{
			STORE_CONTEXT ct;
			ct.dwMidifiedVer = m_dwStoreMidifiedVer;
			ct.ppStoreValue = vecAddr[i];

			pArrRow->push_back_take_ownership(CRowEditImpl::createInstance(this, ct, *vecAddr[i]));
		}
		
		m_pTableData->leaveEditCri();

		return ptr;
	}
	
	IRowArrayPtr CTableEditImpl::ITable_select(const CFilter *pFilter, const CFilterOption * pFilterOption)
	{
		CRowArrayImpl *pArrRow = CRowArrayImpl::CreateInstance(m_pTableData);
		IRowArrayPtr ptr;
		ptr.attach(pArrRow);

		if (!m_bStructInitialized)
			return ptr;
		
		selectHelper(pFilter, pFilterOption, pArrRow, NULL);
		return ptr;
	}

	IViewPtr CTableEditImpl::ITable_getView(const NAME_ID& viewName)
	{
		IViewPtr pRet;

		m_pTableData->enterEditCri();

		CViewDataImpl * pViewData = m_pTableData->queryView(viewName);
		if (!pViewData)
			pViewData = m_pTableData->createView(viewName);

		if (pViewData)
			pRet.attach(CViewImpl::createInstance(this, pViewData));
		
		m_pTableData->leaveEditCri();

		return pRet;
	}

	IRowArrayPtr CTableEditImpl::IView_select(const MATCH_NODE * pMatchTree)
	{
		if (!m_pTableData->verifyEditIsLocked())
			return NULL;

		CRowArrayImpl *pArrRow = CRowArrayImpl::CreateInstance(m_pTableData);

		std::vector<CRowImpl *> vec;
		m_pStore->GetAll(vec);	

		pArrRow->reserve(1024);//预先准备1024个元素（占用4k大小的内存块），提升性能
		for (UINT i = 0; i < vec.size(); i++)
		{
			CRowImpl *pObj = vec[i];
			if (pObj->isMatch(pMatchTree))
				pArrRow->push_back(pObj);
		}

		IRowArrayPtr ptr;
		ptr.attach(pArrRow);
		return ptr;
	}

	BOOL CTableEditImpl::IView_hasRow(const KEY &key, const MATCH_NODE * pMatchTree)
	{
		if (!m_bStructInitialized)
			return FALSE;

		BOOL bRet = FALSE;
		if (verifyKeyTypeValid(key.eType))
		{
			m_pTableData->enterEditCri();

			CRowImpl ** ppObj = m_pStore->Find(key);
			if (ppObj)
				bRet = (*ppObj)->isMatch(pMatchTree);

			m_pTableData->leaveEditCri();
		}

		return bRet;
	}

	IRowPtr CTableEditImpl::IView_row(const KEY &key, const MATCH_NODE * pMatchTree)
	{
		if (!m_bStructInitialized)
			return FALSE;

		IRowPtr pRet;
		if (verifyKeyTypeValid(key.eType))
		{
			m_pTableData->enterEditCri();

			CRowImpl ** ppObj = m_pStore->Find(key);
			if (ppObj && (*ppObj)->isMatch(pMatchTree))
				pRet = *ppObj;

			m_pTableData->leaveEditCri();
		}

		return pRet;
	}

	IRowEditArrayPtr CTableEditImpl::select(const CFilter *pFilter, const CFilterOption * pFilterOption)
	{
		CRowEditArrayImpl *pArrRow = CRowEditArrayImpl::CreateInstance();
		IRowEditArrayPtr ptr;
		ptr.attach(pArrRow);

		if (!m_bStructInitialized)
			return ptr;

		selectHelper(pFilter, pFilterOption, NULL, pArrRow);
		return ptr;
	}

	void CTableEditImpl::selectHelper(const CFilter *pFilter, const CFilterOption * pFilterOption, CRowArrayImpl *pArrRow, CRowEditArrayImpl *pArrRowEdit)
	{
		assert(!pArrRow || !pArrRowEdit);

		BOOL bCheckError = FALSE;
		
		MATCH_NODE *pMatchTreeHeader = NULL;
		std::vector<ORDER_CASE> vecOrderCase;
		DWORD dwLimitCount = 0xFFFFFFFF;

		if (pFilter != NULL)
		{
			pMatchTreeHeader = pFilter->_private()->convertToMatchTree(m_pTableData->getRealCols());
			if (pMatchTreeHeader == NULL)
				bCheckError = TRUE;
		}

		if (!bCheckError && pFilterOption != NULL)
		{
			dwLimitCount = pFilterOption->_private()->getLimit();

			if (!pFilterOption->_private()->getAllOrdercase(m_pTableData->getRealCols(), vecOrderCase))
				bCheckError = TRUE;
		}

		if (!bCheckError)
		{
			m_pTableData->enterEditCri();

			//是否需要对记录排序
			BOOL bNeedOrder;
			if (vecOrderCase.empty())
			{
				bNeedOrder = FALSE;
			}
			else if (vecOrderCase.size() == 1)
			{
				if (m_pStore->GetIndexType() == INDEX_E_MAP
					 && vecOrderCase[0].bAscending == TRUE
					 && vecOrderCase[0].dwColIndex == m_pTableData->getIndexColIndex())
					bNeedOrder = FALSE;
				else
					bNeedOrder = TRUE;
			}
			else
			{
				bNeedOrder = TRUE;
			}

			std::vector<CRowImpl **> vecAddr;
			m_pStore->GetAllAddr(vecAddr);	

			if (!bNeedOrder)//不需要排序
			{
				if (pArrRow)
				{
					pArrRow->reserve(1024);//预先准备1024个元素（占用4k大小的内存块），提升性能
					UINT nCount = 0;
					for (UINT i = 0; i < vecAddr.size() && nCount < dwLimitCount; i++)
					{
						CRowImpl *pObj = *vecAddr[i];
						if (pMatchTreeHeader == NULL || pObj->isMatch(pMatchTreeHeader))
						{
							pArrRow->push_back(pObj);
							nCount++;
						}
					}
				}
				else //选择pArrRowEdit
				{
					pArrRowEdit->reserve(1024);
					UINT nCount = 0;
					for (UINT i = 0; i < vecAddr.size() && nCount < dwLimitCount; i++)
					{
						CRowImpl *pObj = *vecAddr[i];
						if (pMatchTreeHeader == NULL || pObj->isMatch(pMatchTreeHeader))
						{
							STORE_CONTEXT ct;
							ct.dwMidifiedVer = m_dwStoreMidifiedVer;
							ct.ppStoreValue = vecAddr[i];
							pArrRowEdit->push_back_take_ownership(CRowEditImpl::createInstance(this, ct, *vecAddr[i]));

							nCount++;
						}
					}
				}
			}
			else//需要排序
			{
				//assert(! L"不要慌！如果你真走到这儿了，只是证实你开创性的使用了数据中心一项新特征，请通知bobdeng删除这个assert就行了");

				//先筛选
				std::vector<CRowImpl **> vecTemp;
				if (pMatchTreeHeader == NULL)//没有指定过滤
				{
					vecTemp.reserve(vecAddr.size());
					for (UINT i = 0; i < vecAddr.size(); i++)
						vecTemp.push_back(vecAddr[i]);
				}
				else//需要过滤
				{
					vecTemp.reserve(1024);//预先准备1024个元素（占用4k大小的内存块），提升性能
					for (UINT i = 0; i < vecAddr.size(); i++)
					{
						if ((*vecAddr[i])->isMatch(pMatchTreeHeader))
							vecTemp.push_back(vecAddr[i]);
					}
				}
				
				//开始排序
				struct CPred
				{
					const std::vector<ORDER_CASE> * pVecOrderCase;
					bool operator()(CRowImpl ** _Left, CRowImpl ** _Righ) const
					{
						return (*_Left)->isLess(*_Righ, &(*pVecOrderCase)[0], pVecOrderCase->size()) == TRUE;
					}
				};

				CPred pr;
				pr.pVecOrderCase = &vecOrderCase;
				if (dwLimitCount == 0xFFFFFFFF)//全排序
				{
					std::sort(vecTemp.begin(), vecTemp.end(), pr);
				}
				else//做部分排序，提升性能
				{
					const DWORD dwMaxSortCount = min(dwLimitCount, vecTemp.size());
					std::partial_sort(vecTemp.begin(), vecTemp.begin() + dwMaxSortCount, vecTemp.end(), pr);
				}

				if (pArrRow)
				{
					pArrRow->reserve(vecTemp.size());
					for (UINT i = 0; i < vecTemp.size() && i < dwLimitCount; i++)
						pArrRow->push_back(*vecTemp[i]);
				}
				else
				{
					pArrRowEdit->reserve(vecTemp.size());
					for (UINT i = 0; i < vecTemp.size() && i < dwLimitCount; i++)
					{
						STORE_CONTEXT ct;
						ct.dwMidifiedVer = m_dwStoreMidifiedVer;
						ct.ppStoreValue = vecTemp[i];

						pArrRowEdit->push_back_take_ownership(CRowEditImpl::createInstance(this, ct, *vecTemp[i]));
					}
				}
			}

			m_pTableData->leaveEditCri();
		}			
		
		CConditionExpr::destroyMatchTree(pMatchTreeHeader);
	}

	DWORD CTableEditImpl::size()
	{
		if (!m_bStructInitialized)
			return 0;

		DWORD dwSize = 0;

		m_pTableData->enterEditCri();		
		if (m_pStore)
			dwSize = m_pStore->Size();
		m_pTableData->leaveEditCri();

		return dwSize;
	}

	BOOL CTableEditImpl::verifyKeyTypeValid(E_KEY_TYPE eKeyType)
	{
		BOOL bRet = FALSE;

		E_FIELD eFieldType = m_pTableData->getIndexColType();	
		switch (eFieldType)
		{
			case FIELD_E_DWORD:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
				bRet = eKeyType == KEY_E_DWORD;
				break;
			case FIELD_E_STR:
				bRet = eKeyType == KEY_E_STR;
				break;
			default:
				assert(! L"不支持的key字段类型");
				break;
		}

		if (!bRet)
		{
			LPCWSTR lpszName = m_pTableData->getTableName().GetName();
			Error::OutputError(_T("操作表 %s key类型错误！！"), lpszName ? lpszName : _T("空"));
		}

		return bRet;
	}

	void CTableEditImpl::setVirtualFieldHandler(IVirtualFieldHandler *pHandler)
	{
		m_pTableData->setVirtualFieldHandler(pHandler);
	}
	
	void CTableEditImpl::removeVirtualFieldHandler(IVirtualFieldHandler *pHandler)
	{
		m_pTableData->removeVirtualFieldHandler(pHandler);
	}

	CRowImpl * CTableEditImpl::detachRow(const STORE_CONTEXT &storeContext, CRowImpl *pRowOrig)
	{
		assert(pRowOrig->getRowState() == CRowImpl::ROW_STATE_E_INTABLE);
		assert(storeContext.ppStoreValue == NULL || *storeContext.ppStoreValue == pRowOrig);

		CRowImpl *pNewRow = NULL;

		if (storeContext.dwMidifiedVer == m_dwStoreMidifiedVer)
		{
			assert(pRowOrig->getRefCount() > 1);

#ifdef _DEBUG
			KEY key;
			pRowOrig->getKey(&key);
			CRowImpl ** ppObj = m_pStore->Find(key);
			assert(ppObj != NULL);
			assert(ppObj == storeContext.ppStoreValue);
#endif

			pNewRow = pRowOrig->clone();
			pRowOrig->release();
			pNewRow->addRef();
			*storeContext.ppStoreValue = pNewRow;

			assert(*m_pStore->Find(key) == pNewRow);
		}
		else
		{
			KEY key;
			pRowOrig->getKey(&key);

			CRowImpl ** ppObj = m_pStore->Find(key);
			if (ppObj)
			{
				if (*ppObj != pRowOrig)
				{
					LPCTSTR lpszName = m_pTableData->getTableName().GetName();
					Error::OutputError(_T("表%s正在编辑中的行已经被悄悄替换了"), lpszName ? lpszName : _T("空"));
				}

				pNewRow = pRowOrig->clone();
				(*ppObj)->release();
				pNewRow->addRef();
				*ppObj = pNewRow;
			}
			else
			{
				pNewRow = pRowOrig->clone();

				LPCTSTR lpszName = m_pTableData->getTableName().GetName();
				Error::OutputError(_T("表%s正在编辑中的行已经被删除掉了"), lpszName ? lpszName : _T("空"));
			}
		}

		return pNewRow;
	}

	BOOL CTableEditImpl::checkStructInitialized()
	{
		if (!m_bStructInitialized)
		{
			m_pTableData->enterEditCri();
			
			if (!m_bStructInitialized)
			{
				CDatabaseContextImpl *pDatabaseContext = m_pTableData->getDatabaseContext();
				NAME_ID tableName = m_pTableData->getTableName();
				
				TABLE_STRUCT tableStruct;
				if (pDatabaseContext->queryTableStruct(tableName, &tableStruct))
				{
					m_bStructInitialized = init(&tableStruct);	
				}
				else
				{
					LPCTSTR lpszName = tableName.GetName();
					Error::OutputError(_T("表%s延迟的初始化时发现表结构信息不存在"), lpszName ? lpszName : _T("空"));
				}
			}

			m_pTableData->leaveEditCri();
		}
		
		return m_bStructInitialized;
	}

	void CTableEditImpl::dumpSummary()
	{
		Error::OutputDebug(_T("table dump: table=%s, ptr=0x%08X(%u), ref=%d"), 
			m_pTableData->getTableName().GetName(), 
			this,
			this,
			m_dwRef);
	}

	void CTableEditImpl::dump()
	{
		if (!m_bStructInitialized)
			return;
		
		std::wstring wstrContent;
		wstrContent.reserve(1024 * 4);
		std::wstring wstrTemp;
		wchar_t szTemp[4096] = {0};

		m_pTableData->enterEditCri();

		std::vector<CRowImpl *> vec;
		m_pStore->GetAll(vec);
		for (UINT i = 0; i < vec.size(); i++)
		{
			vec[i]->getContentStr(wstrTemp);

			_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%u. %s\r\n"), i, wstrTemp.c_str());
			wstrContent += szTemp;
		}

		m_pTableData->leaveEditCri();

		Error::OutputDebug(_T("table dump: table=%s, ptr=0x%08X(%u), ref=%d"), 
			m_pTableData->getTableName().GetName(), 
			this,
			this,
			m_dwRef);
	
		::OutputDebugString(_T("content begin ==========================================================================\r\n"));
		::OutputDebugString(wstrContent.c_str());
		::OutputDebugString(_T("content end ==========================================================================\r\n\r\n"));
	}
};
