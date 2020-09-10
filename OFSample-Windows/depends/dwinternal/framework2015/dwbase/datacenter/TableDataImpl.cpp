#include "stable.h"
#include "TableDataImpl.h"
#include "RowImpl.h"
#include "RowArrayImpl.h"
#include "ViewDataImpl.h"
#include <assert.h>
#include <algorithm>

namespace Data
{
	void Dump(void* ptr, const char* ptrType)
	{
		if(IsBadReadPtr(ptr, sizeof(void*)))
			return;

		WCHAR dbgstr[1024];
		swprintf_s(dbgstr, 1024, L"ptr:%p, type:%s\n", ptr, (const wchar_t*) ptrType);
		OutputDebugStringW(dbgstr);

		__try{
			if(strcmp(ptrType, "Data::IRowEdit") == 0)
			{
				IRowEdit *p = (IRowEdit*)ptr;
				p->dump();
			}
			else if(strcmp(ptrType, "Data::IRow") == 0)
			{
				IRow *p = (IRow*)ptr;
				p->dump();
			}
			else if(strcmp(ptrType, "Data::ITableEdit") == 0)
			{
				ITableEdit *p = (ITableEdit*)(ptr);
				if(p)
					p->dump();
			}
			else if(strcmp(ptrType, "Data::ITable") == 0)
			{
				ITable *p = (ITable*)ptr;
				p->dump();
			}
			else
			{
				swprintf_s(dbgstr, 1024, L"ptr:%p, 不支持此类型:%s\n", ptr, (const wchar_t*) ptrType);
				OutputDebugStringW(dbgstr);
			}
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{

		}
	}

	CTableDataImpl::CTableDataImpl(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext)
	{
		assert(!tableName.IsEmpty());
		m_tableName = tableName;
		
		pDatabaseContext->addRef();
		m_pDatabaseContext = pDatabaseContext;

		m_dwRef = 1;

		m_bStructInitialized = FALSE;

		memset(&m_realCols, 0, sizeof(m_realCols));
		memset(&m_virtualCols, 0, sizeof(m_virtualCols));

		m_dwFindColCacheIndex = -1;

		m_dwIndexColIndex = -1;
		m_dwIndexColOffsetBit = -1;
		m_eIndexColType = FIELD_E_NULL;

		m_pRowBackup = NULL;
		m_pRowLastEdited = NULL;
	
		::InitializeCriticalSection(&m_criEdit);
		::InitializeCriticalSection(&m_criVirtualFieldCB);
	}

	CTableDataImpl::~CTableDataImpl(void)
	{
		assert(m_dwRef == 0);
		assert(m_pRowBackup == NULL && m_pRowLastEdited == NULL);

		for (UINT i = 0; i < m_vecViewData.size(); i++)
			m_vecViewData[i]->release();
		m_vecViewData.clear();

		m_pDatabaseContext->release();
		m_pDatabaseContext = NULL;

		delete [] m_realCols.arCol;
		m_realCols.arCol = NULL;

		delete [] m_virtualCols.arType;
		m_virtualCols.arType = NULL;

		delete [] m_virtualCols.arId;
		m_virtualCols.arId = NULL;

		::DeleteCriticalSection(&m_criEdit);
		::DeleteCriticalSection(&m_criVirtualFieldCB);
	}

	CTableDataImpl * CTableDataImpl::createInstance(const NAME_ID &tableName, CDatabaseContextImpl *pDatabaseContext)
	{
		return new CTableDataImpl(tableName, pDatabaseContext);
	}

	void CTableDataImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CTableDataImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CTableDataImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CTableDataImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	//编辑
	void CTableDataImpl::beginEditMode()
	{
		::EnterCriticalSection(&m_criEdit);
		assert((DWORD)m_criEdit.OwningThread == ::GetCurrentThreadId());
		if (m_criEdit.RecursionCount > 1)//暂时不允许重入
		{
			LPCWSTR lpszName = m_tableName.GetName();
			Error::OutputError(_T("修改表 %s beginEditMode重入或者上一次beginEditMode未调用endEditMode！！"), lpszName ? lpszName : _T("空"));
		}
	}

	void CTableDataImpl::endEditMode()
	{
		if (verifyEditEnable())
		{
			notifyRowEditActionEnd();

			::LeaveCriticalSection(&m_criEdit);

			//触发事件
			m_eventCollect.OnTrigger(this);

			//触发视图事件
			enterEditCri();
			std::vector<CViewDataImpl *> vecCopy;
			vecCopy = m_vecViewData;
			for (UINT i = 0; i < vecCopy.size(); i++)
				vecCopy[i]->addRef();
			leaveEditCri();
			
			for (UINT i = 0; i < vecCopy.size(); i++)
			{
				vecCopy[i]->onEndEditMode();
				vecCopy[i]->release();
			}

			vecCopy.clear();
		}
	}

	BOOL CTableDataImpl::init(const TABLE_STRUCT *pTableStruct)
	{
		assert(!pTableStruct->indexColName.IsEmpty());
		
		const std::vector<COL_PROP> & cols = pTableStruct->vecCol;
		
		//检查是否有效的cols
		for (UINT i = 0; i < cols.size(); i++)
		{
			assert(cols[i].eColType != FIELD_E_NULL && !cols[i].colName.IsEmpty());
			if (cols[i].eColType == FIELD_E_NULL || cols[i].colName.IsEmpty())
			{
				assert(! L"列属性无效");
				return FALSE;
			}
		}

		//检查是否存在重复
		std::set<DWORD> setUnique;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (!setUnique.insert(cols[i].colName.m_nId).second)
			{
				assert(! L"列名重复");
				return FALSE;
			}
		}

		//索引列是否存在和有效
		BOOL bIndexValid = FALSE;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (cols[i].colName == pTableStruct->indexColName)
			{
				assert(!cols[i].bVirtual);//索引列不能是虚拟字段
				if (!cols[i].bVirtual)
				{
					//索引列是否类型支持
					switch (cols[i].eColType)
					{
					case FIELD_E_DWORD:
					case FIELD_E_WORD:
					case FIELD_E_BYTE:
					case FIELD_E_STR:
						bIndexValid = TRUE;
						break;
					default:
						assert(! L"索引列类型不支持");
						break;
					}
				}

				break;
			}
		}

		if (!bIndexValid)
			return FALSE;

		//筛选实体字段和虚拟字段
		std::vector<COL_PROP> vecRealCol;
		std::vector<COL_PROP> vecVirtualCol;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (cols[i].bVirtual)
				vecVirtualCol.push_back(cols[i]);
			else
				vecRealCol.push_back(cols[i]);
		}

		//开始排序实体字段
		std::stable_sort(vecRealCol.begin(), vecRealCol.end(), colComp);

		//更新m_realCols
		delete [] m_realCols.arCol;
		m_realCols.dwColCount = vecRealCol.size();
		m_realCols.dwExistBitSize = (vecRealCol.size() + 31) / 32 * 4;
		m_realCols.dwModifiedBitSize = (vecRealCol.size() + 31) / 32 * 4;
		m_realCols.arCol = new COL[vecRealCol.size()];

		DWORD dwOffsetBit = 0;
		for (UINT i = 0; i < vecRealCol.size(); i++)
		{
			m_realCols.arCol[i].dwId = vecRealCol[i].colName.m_nId;
			m_realCols.arCol[i].wType = (WORD)vecRealCol[i].eColType;

			DWORD dwBitSize;

			switch (vecRealCol[i].eColType)
			{
			case FIELD_E_DWORD:
			case FIELD_E_STR:
			case FIELD_E_BLOB:
			case FIELD_E_TABLE:	
				dwBitSize = 32;
				break;
			case FIELD_E_WORD:
				dwBitSize = 16;
				break;
			case FIELD_E_BYTE:
				dwBitSize = 8;
				break;
			case FIELD_E_BOOL:
				dwBitSize = 1;
				break;
			case FIELD_E_DWORD64:
				dwBitSize = 64;
				break;
			default:	
				assert(! L"不支持的字段类型");
				dwBitSize = 32;
				break;
			}
			
			m_realCols.arCol[i].wOffsetBit = (WORD)((dwOffsetBit + dwBitSize - 1) / dwBitSize * dwBitSize);
			dwOffsetBit = m_realCols.arCol[i].wOffsetBit + dwBitSize;
		}

		m_realCols.dwValuesSize = (dwOffsetBit + 31) / 32 * 4;

		//保存索引列信息
		for (UINT i = 0; i < m_realCols.dwColCount; i++)
		{
			if (pTableStruct->indexColName == (WORD)m_realCols.arCol[i].dwId)
			{
				m_indexColName = pTableStruct->indexColName;
				m_dwIndexColIndex = i;
				m_dwIndexColOffsetBit = m_realCols.arCol[i].wOffsetBit;
				m_eIndexColType = (E_FIELD)m_realCols.arCol[i].wType;
			}
		}

		//更新m_virtualCols
		delete [] m_virtualCols.arId;
		delete [] m_virtualCols.arType;

		if (!vecVirtualCol.empty())
		{
			m_virtualCols.dwColCount = vecVirtualCol.size();
			m_virtualCols.arId = new WORD[vecVirtualCol.size()];
			m_virtualCols.arType = new BYTE[vecVirtualCol.size()];

			for (UINT i = 0; i < vecVirtualCol.size(); i++)
			{
				m_virtualCols.arId[i] = vecVirtualCol[i].colName.m_nId;
				m_virtualCols.arType[i] = (BYTE)vecVirtualCol[i].eColType;
			}
		}
		else
		{
			m_virtualCols.dwColCount = 0;
			m_virtualCols.arId = NULL;
			m_virtualCols.arType = NULL;
		}

		m_bStructInitialized = TRUE;


		//最后检验下已经创建的视图是否在模版中存在，并触发视图初始化
		std::vector<VIEW_STRUCT> vecViewStruct;
		m_pDatabaseContext->queryViewStruct(m_tableName, vecViewStruct);
		for (UINT i = 0; i < m_vecViewData.size(); i++)
		{
			BOOL bIsExist = FALSE;
			for (UINT j = 0; j < vecViewStruct.size(); j++)
			{
				if (m_vecViewData[i]->getViewName() == vecViewStruct[j].viewName)
				{
					bIsExist = TRUE;
					m_vecViewData[i]->onTableDataInit();
					break;
				}
			}

			if (!bIsExist)
			{
				LPCTSTR lpszName = m_vecViewData[i]->getViewName().GetName();
				Error::OutputError(_T("视图%s在模版中没有定义"), lpszName ? lpszName : _T("空"));
			}
		}

		return TRUE;
	}

	void CTableDataImpl::getCols(std::vector<COL_PROP> & cols)
	{
		cols.clear();

		for (UINT i = 0; i < m_realCols.dwColCount; i++)
		{
			COL_PROP prop;
			prop.colName = (WORD)m_realCols.arCol[i].dwId;
			prop.eColType = (E_FIELD)m_realCols.arCol[i].wType;
			prop.bVirtual = FALSE;

			cols.push_back(prop);
		}

		for (UINT i = 0; i < m_virtualCols.dwColCount; i++)
		{
			COL_PROP prop;
			prop.colName = m_virtualCols.arId[i];
			prop.eColType = (E_FIELD)m_virtualCols.arType[i];
			prop.bVirtual = TRUE;

			cols.push_back(prop);
		}		
	}

	BOOL CTableDataImpl::findCol(const NAME_ID &col, DWORD *lpIndex, DWORD *lpOffsetBit, E_FIELD *lpType)
	{
		DWORD dwId = col.m_nId;

		DWORD dwIndexTemp = m_dwFindColCacheIndex;
		if (dwIndexTemp != -1 && dwId == m_realCols.arCol[dwIndexTemp].dwId)
		{
			*lpType = (E_FIELD)m_realCols.arCol[dwIndexTemp].wType;
			*lpOffsetBit = m_realCols.arCol[dwIndexTemp].wOffsetBit;
			*lpIndex = dwIndexTemp;

			return TRUE;
		}

		for (UINT i = 0; i < m_realCols.dwColCount; i++)
		{
			if (m_realCols.arCol[i].dwId == dwId)
			{
				m_dwFindColCacheIndex = i;
				
				*lpType = (E_FIELD)m_realCols.arCol[i].wType;
				*lpOffsetBit = m_realCols.arCol[i].wOffsetBit;
				*lpIndex = i;
				
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CTableDataImpl::findVirtualCol(const NAME_ID &col, DWORD *lpIndex, E_FIELD *lpType)
	{
		WORD wId = col.m_nId;

		for (UINT i = 0; i < m_virtualCols.dwColCount; i++)
		{
			if (m_virtualCols.arId[i] == wId)
			{
				*lpIndex = i;
				*lpType = (E_FIELD)m_virtualCols.arType[i];

				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CTableDataImpl::verifyEditEnable()
	{
		if (m_criEdit.RecursionCount > 0 && (DWORD)m_criEdit.OwningThread == ::GetCurrentThreadId())
		{
			return TRUE;
		}
		else
		{
			LPCWSTR lpszName = m_tableName.GetName();
			Error::OutputError(_T("修改表 %s 未设置编辑模式！！"), lpszName ? lpszName : _T("空"));

			return FALSE;
		}
	}

	BOOL CTableDataImpl::verifyEditIsLocked() const
	{
		if (m_criEdit.RecursionCount > 0 && (DWORD)m_criEdit.OwningThread == ::GetCurrentThreadId())
		{
			return TRUE;
		}
		else
		{
			LPCWSTR lpszName = m_tableName.GetName();
			Error::OutputError(_T("表 %s 未锁定编辑锁！！"), lpszName ? lpszName : _T("空"));

			return FALSE;
		}
	}

	void CTableDataImpl::notifyRowAdd(CRowImpl *pRow)
	{
		notifyRowEditActionEnd();

		pRow->setRowState(CRowImpl::ROW_STATE_E_PENGING);
		notifyRowEditActionBegin(NULL, pRow);
	}

	void CTableDataImpl::notifyRowDel(CRowImpl *pRow)
	{
		notifyRowEditActionEnd();

		pRow->setRowState(CRowImpl::ROW_STATE_E_REMOVE);
		rowEvent(pRow, ROW_E_DEL, NULL);
	}

	void CTableDataImpl::notifyTableReset(const std::vector<CRowImpl *> & vecRow)
	{
		notifyRowEditActionEnd();

		//通知视图事件
		for (UINT i = 0; i < m_vecViewData.size(); i++)
			m_vecViewData[i]->onTableResetEvent(vecRow);

		m_eventCollect.AddResetEvent(vecRow);
	}

	void CTableDataImpl::notifyRowEditActionEnd()
	{
		if (m_pRowLastEdited)
		{
			CRowImpl::E_ROW_STATE eState = m_pRowLastEdited->getRowState();
			
			if (eState == CRowImpl::ROW_STATE_E_PENGING)//插入行
			{
				assert(!m_pRowLastEdited->isModified());

				m_pRowLastEdited->setRowState(CRowImpl::ROW_STATE_E_INTABLE);
				rowEvent(m_pRowLastEdited, ROW_E_ADD, NULL);

				m_pRowLastEdited = NULL;
				assert(m_pRowBackup == NULL);
			}
			else//编辑行
			{
				assert(eState == CRowImpl::ROW_STATE_E_INTABLE);
				
				if (m_pRowLastEdited->isModified())
					rowEvent(m_pRowLastEdited, ROW_E_CHANGED, m_pRowBackup);

				m_pRowLastEdited = NULL;
				m_pRowBackup->release();
				m_pRowBackup = NULL;
			}
		}
		else
		{
			assert(m_pRowBackup == NULL);
		}
	}

	void CTableDataImpl::rowEvent(CRowImpl *pRowCur, E_ROW_EVENT eRowEvent, CRowImpl * pRowOrig)
	{
		//通知视图事件
		for (UINT i = 0; i < m_vecViewData.size(); i++)
			m_vecViewData[i]->onRowEvent(pRowCur, pRowOrig, eRowEvent);

		m_eventCollect.AddRowEvent(pRowCur, pRowOrig, eRowEvent);
	}

	void CTableDataImpl::setVirtualFieldHandler(IVirtualFieldHandler *pHandler)
	{
		::EnterCriticalSection(&m_criVirtualFieldCB);
		m_setVirtualFieldCB.insert(pHandler);
		::LeaveCriticalSection(&m_criVirtualFieldCB);
	}
	
	void CTableDataImpl::removeVirtualFieldHandler(IVirtualFieldHandler *pHandler)
	{
		::EnterCriticalSection(&m_criVirtualFieldCB);
		m_setVirtualFieldCB.erase(pHandler);
		::LeaveCriticalSection(&m_criVirtualFieldCB);
	}

	BOOL CTableDataImpl::notifyCalcField(IRow *pRow, const NAME_ID &name, DWVariant *pVar)
	{
		BOOL bRet = FALSE;

		::EnterCriticalSection(&m_criVirtualFieldCB);
		
		for (std::set<comptr<IVirtualFieldHandler> >::iterator it = m_setVirtualFieldCB.begin(); it != m_setVirtualFieldCB.end(); ++it)
		{
			if ((*it)->onCalcField(m_pDatabaseContext->getDatabaseId(), m_tableName, pRow, name, pVar))
			{
				bRet = TRUE;
				break;
			}
		}
		
		::LeaveCriticalSection(&m_criVirtualFieldCB);

		return bRet;
	}

	CViewDataImpl * CTableDataImpl::queryView(const NAME_ID& viewName)
	{
		for (UINT i = 0; i < m_vecViewData.size(); i++)
		{
			if (viewName == m_vecViewData[i]->getViewName())
				return m_vecViewData[i];
		}

		return NULL;
	}

	CViewDataImpl * CTableDataImpl::createView(const NAME_ID& viewName)
	{
		assert(!viewName.IsEmpty());
		if (viewName.IsEmpty())
			return NULL;

		CViewDataImpl * pRet = queryView(viewName);
		if (!pRet)
		{
			pRet = CViewDataImpl::createInstance(viewName, this);
			m_vecViewData.push_back(pRet);
		}

		//检验下已经创建的视图是否在模版中存在
		if (m_bStructInitialized)
		{
			std::vector<VIEW_STRUCT> vecViewStruct;
			m_pDatabaseContext->queryViewStruct(m_tableName, vecViewStruct);
			
			BOOL bIsExist = FALSE;
			for (UINT i = 0; i < vecViewStruct.size(); i++)
			{
				if (viewName == vecViewStruct[i].viewName)
				{
					bIsExist = TRUE;
					break;
				}
			}

			if (!bIsExist)
			{
				LPCTSTR lpszName = viewName.GetName();
				Error::OutputError(_T("视图%s在模版中没有定义"), lpszName ? lpszName : _T("空"));
			}
		}

		return pRet;
	}
};
