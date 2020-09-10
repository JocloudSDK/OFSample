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
				swprintf_s(dbgstr, 1024, L"ptr:%p, ��֧�ִ�����:%s\n", ptr, (const wchar_t*) ptrType);
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
		assert(! L"��֧����ָ��");
		return NULL;
	}

	bool CTableDataImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"��֧��queryInterface");
		return false;
	}

	//�༭
	void CTableDataImpl::beginEditMode()
	{
		::EnterCriticalSection(&m_criEdit);
		assert((DWORD)m_criEdit.OwningThread == ::GetCurrentThreadId());
		if (m_criEdit.RecursionCount > 1)//��ʱ����������
		{
			LPCWSTR lpszName = m_tableName.GetName();
			Error::OutputError(_T("�޸ı� %s beginEditMode���������һ��beginEditModeδ����endEditMode����"), lpszName ? lpszName : _T("��"));
		}
	}

	void CTableDataImpl::endEditMode()
	{
		if (verifyEditEnable())
		{
			notifyRowEditActionEnd();

			::LeaveCriticalSection(&m_criEdit);

			//�����¼�
			m_eventCollect.OnTrigger(this);

			//������ͼ�¼�
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
		
		//����Ƿ���Ч��cols
		for (UINT i = 0; i < cols.size(); i++)
		{
			assert(cols[i].eColType != FIELD_E_NULL && !cols[i].colName.IsEmpty());
			if (cols[i].eColType == FIELD_E_NULL || cols[i].colName.IsEmpty())
			{
				assert(! L"��������Ч");
				return FALSE;
			}
		}

		//����Ƿ�����ظ�
		std::set<DWORD> setUnique;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (!setUnique.insert(cols[i].colName.m_nId).second)
			{
				assert(! L"�����ظ�");
				return FALSE;
			}
		}

		//�������Ƿ���ں���Ч
		BOOL bIndexValid = FALSE;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (cols[i].colName == pTableStruct->indexColName)
			{
				assert(!cols[i].bVirtual);//�����в����������ֶ�
				if (!cols[i].bVirtual)
				{
					//�������Ƿ�����֧��
					switch (cols[i].eColType)
					{
					case FIELD_E_DWORD:
					case FIELD_E_WORD:
					case FIELD_E_BYTE:
					case FIELD_E_STR:
						bIndexValid = TRUE;
						break;
					default:
						assert(! L"���������Ͳ�֧��");
						break;
					}
				}

				break;
			}
		}

		if (!bIndexValid)
			return FALSE;

		//ɸѡʵ���ֶκ������ֶ�
		std::vector<COL_PROP> vecRealCol;
		std::vector<COL_PROP> vecVirtualCol;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (cols[i].bVirtual)
				vecVirtualCol.push_back(cols[i]);
			else
				vecRealCol.push_back(cols[i]);
		}

		//��ʼ����ʵ���ֶ�
		std::stable_sort(vecRealCol.begin(), vecRealCol.end(), colComp);

		//����m_realCols
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
				assert(! L"��֧�ֵ��ֶ�����");
				dwBitSize = 32;
				break;
			}
			
			m_realCols.arCol[i].wOffsetBit = (WORD)((dwOffsetBit + dwBitSize - 1) / dwBitSize * dwBitSize);
			dwOffsetBit = m_realCols.arCol[i].wOffsetBit + dwBitSize;
		}

		m_realCols.dwValuesSize = (dwOffsetBit + 31) / 32 * 4;

		//������������Ϣ
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

		//����m_virtualCols
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


		//���������Ѿ���������ͼ�Ƿ���ģ���д��ڣ���������ͼ��ʼ��
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
				Error::OutputError(_T("��ͼ%s��ģ����û�ж���"), lpszName ? lpszName : _T("��"));
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
			Error::OutputError(_T("�޸ı� %s δ���ñ༭ģʽ����"), lpszName ? lpszName : _T("��"));

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
			Error::OutputError(_T("�� %s δ�����༭������"), lpszName ? lpszName : _T("��"));

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

		//֪ͨ��ͼ�¼�
		for (UINT i = 0; i < m_vecViewData.size(); i++)
			m_vecViewData[i]->onTableResetEvent(vecRow);

		m_eventCollect.AddResetEvent(vecRow);
	}

	void CTableDataImpl::notifyRowEditActionEnd()
	{
		if (m_pRowLastEdited)
		{
			CRowImpl::E_ROW_STATE eState = m_pRowLastEdited->getRowState();
			
			if (eState == CRowImpl::ROW_STATE_E_PENGING)//������
			{
				assert(!m_pRowLastEdited->isModified());

				m_pRowLastEdited->setRowState(CRowImpl::ROW_STATE_E_INTABLE);
				rowEvent(m_pRowLastEdited, ROW_E_ADD, NULL);

				m_pRowLastEdited = NULL;
				assert(m_pRowBackup == NULL);
			}
			else//�༭��
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
		//֪ͨ��ͼ�¼�
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

		//�������Ѿ���������ͼ�Ƿ���ģ���д���
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
				Error::OutputError(_T("��ͼ%s��ģ����û�ж���"), lpszName ? lpszName : _T("��"));
			}
		}

		return pRet;
	}
};
