#include "stable.h"
#include "FieldSetDataImpl.h"
#include "FieldSetImpl.h"
#include "CriticalSection.h"
#include "EventDispatch.h"
#include "perflog.h"

namespace Data
{
	CFieldSetImpl::CFieldSetImpl(const NAME_ID &fieldSetName, CDatabaseContextImpl *pDatabaseContext)
	{
		m_fieldSetName = fieldSetName;

		pDatabaseContext->addRef();
		m_pDatabaseContext = pDatabaseContext;

		m_dwRef = 1;
		m_pSetData = CFieldSetDataImpl::createInstance(this);
		m_bCloneDataOnWrite = FALSE;
		m_bStructInitialized = FALSE;
		memset(&m_cols, 0, sizeof(m_cols));
	}

	CFieldSetImpl::~CFieldSetImpl(void)
	{
		assert(m_dwRef == 0);

		DWORD dwRef = m_pSetData->internalrelease();
		if (dwRef != 0)
			Error::OutputError(_T("CFieldSetImpl析构时发现m_pSetData引用计数错误, dwRef=%d"), dwRef);

		m_pSetData = NULL;

		m_pDatabaseContext->release();
		m_pDatabaseContext = NULL;

		delete [] m_cols.arId;
		m_cols.arId = NULL;

		delete [] m_cols.arType;
		m_cols.arType = NULL;
	}

	CFieldSetImpl * CFieldSetImpl::createInstance(const NAME_ID &fieldSetName, CDatabaseContextImpl *pDatabaseContext)
	{
		return new CFieldSetImpl(fieldSetName, pDatabaseContext);
	}

	void CFieldSetImpl::addRef()
	{		
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CFieldSetImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CFieldSetImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CFieldSetImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	LPCWSTR	CFieldSetImpl::getStr(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getStr(name);
	}

	const BYTE * CFieldSetImpl::getBlob(const NAME_ID &name, DWORD * lpdwSize)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getBlob(name, lpdwSize);
	}

	DWORD64	CFieldSetImpl::getDword64(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getDword64(name);
	}

	DWORD CFieldSetImpl::getDword(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getDword(name);
	}

	WORD CFieldSetImpl::getWord(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getWord(name);
	}

	BYTE CFieldSetImpl::getByte(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getByte(name);
	}

	BOOL CFieldSetImpl::getBool(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->getBool(name);
	}	
	
	comptr<ITableEdit> CFieldSetImpl::getSubTable(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);

		ITablePtr p = m_pSetData->getSubTable(name);
		if (p)
			return p->getEdit();
		return NULL;
	}

	BOOL CFieldSetImpl::hasData(const NAME_ID &name)
	{
		CAutoLock lock(m_csEditLock);
		return m_pSetData->hasData(name);
	}

	void CFieldSetImpl::detach()
	{
		if (!m_bCloneDataOnWrite)
		{
			CFieldSetDataImpl * p = m_pSetData->clone();
			m_pSetData->internalrelease();
			m_pSetData = p;

			m_bCloneDataOnWrite = TRUE;
		}
	}
	
	IFieldSetDataPtr CFieldSetImpl::getData()
	{		
		return m_pSetData;
	}

	BOOL CFieldSetImpl::verifyEditEnable()
	{
		if (m_csEditLock.GetHandle()->RecursionCount > 0 && (DWORD)m_csEditLock.GetHandle()->OwningThread == ::GetCurrentThreadId())
		{
			return TRUE;
		}
		else
		{
			LPCWSTR lpszName = m_fieldSetName.GetName();
			Error::OutputError(_T("修改fieldset %s 未设置编辑模式！！"), lpszName ? lpszName : _T("空"));

			return FALSE;
		}
	}

	BOOL CFieldSetImpl::setStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		if (verifyEditEnable())
		{
			detach();
			return m_pSetData->setStr(name, lpwszValue);
		}
		return FALSE;
	}

	BOOL CFieldSetImpl::setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize)
	{
		if (verifyEditEnable())
		{
			detach();
			return m_pSetData->setBlob(name, pbData, cbSize);
		}
		return FALSE;
	}

	BOOL CFieldSetImpl::setDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		if (verifyEditEnable())
		{
			detach();
			return m_pSetData->setDword64(name, dw64Value);
		}
		return FALSE;
	}

	BOOL CFieldSetImpl::setDword(const NAME_ID &name, DWORD dwValue)
	{
		if (verifyEditEnable())
		{
			detach();
			return m_pSetData->setDword(name, dwValue);
		}
		return FALSE;
	}

	BOOL CFieldSetImpl::setWord(const NAME_ID &name, WORD wValue)
	{
		if (verifyEditEnable())
		{	
			detach();
			return m_pSetData->setWord(name, wValue);
		}
		return FALSE;
	}
	
	BOOL CFieldSetImpl::setByte(const NAME_ID &name, BYTE byValue)
	{
		if (verifyEditEnable())
		{	
			detach();
			return m_pSetData->setByte(name, byValue);
		}
		return FALSE;
	}
	
	BOOL CFieldSetImpl::setBool(const NAME_ID &name, BOOL bValue)
	{
		if (verifyEditEnable())
		{	
			detach();
			return m_pSetData->setBool(name, bValue);
		}
		return FALSE;
	}

	BOOL CFieldSetImpl::setSubTable(const NAME_ID &name, ITableEdit *pSubTable)
	{
		if (verifyEditEnable())
		{	
			detach();
			return m_pSetData->setSubTable(name, pSubTable);
		}
		return FALSE;
	}

	BOOL CFieldSetImpl::removeData(const NAME_ID &name)
	{
		if (verifyEditEnable())
		{	
			detach();
			return m_pSetData->removeData(name);
		}
		return FALSE;
	}

	//编辑
	void CFieldSetImpl::beginEdit()
	{
		if (!checkStructInitialized())
			return;

		m_csEditLock.Lock();

		assert(!m_bCloneDataOnWrite); //beginEdit重入或者上一次beginEdit未调用endEdit?
		assert((DWORD)m_csEditLock.GetHandle()->OwningThread == ::GetCurrentThreadId());
		if (m_csEditLock.GetHandle()->RecursionCount > 1)//暂时不允许重入
		{
			LPCWSTR lpszName = m_fieldSetName.GetName();
			Error::OutputError(_T("修改fieldset %s beginEdit重入或者上一次beginEdit未调用endEdit！！"), lpszName ? lpszName : _T("空"));
		}
	}

	void CFieldSetImpl::endEdit()
	{
		assert(m_bStructInitialized);
		if (!m_bStructInitialized)
			return;

		if (verifyEditEnable())
		{
			BOOL bModified = (m_bCloneDataOnWrite && m_pSetData->isModified());
			IFieldSetDataPtr pSetData = m_pSetData;

			//触发异步事件	
			if (bModified)
				CEventDispatch::GetInstance()->QueuedEmit(&m_sigQueuedChanged, pSetData);
			
			m_bCloneDataOnWrite = FALSE;
			m_csEditLock.Unlock();

			//触发同步事件	
			if (bModified)
			{
#define F_DATACENTER "DataCenter"
#define DataCenter_SigChanged "DataCenter_SigChanged"
				PerfLog(2, DataCenter_SigChanged, F_DATACENTER);
				CEventDispatch::GetInstance()->Emit(&m_sigChanged, pSetData);
			}
		}	
	}
	
	BOOL CFieldSetImpl::init(const FIELDSET_STRUCT *pStruct)
	{		
		const std::vector<COL_PROP> & cols = pStruct->vecCol;
		//是否有效的cols
		for (UINT i = 0; i < cols.size(); i++)
		{
			assert(cols[i].eColType != FIELD_E_NULL && !cols[i].colName.IsEmpty());
			if (cols[i].eColType == FIELD_E_NULL || cols[i].colName.IsEmpty())
				return FALSE;
		}

		std::set<DWORD> setUnique;
		for (UINT i = 0; i < cols.size(); i++)
		{
			if (!setUnique.insert(cols[i].colName.m_nId).second)
			{
				LPCWSTR lpszName = m_fieldSetName.GetName();
				Error::OutputError(_T("创建fieldset%s时存在重复的字段！！"), lpszName ? lpszName : _T("空"));

				return FALSE;
			}
		}

		//更新m_cols
		delete [] m_cols.arId;
		delete [] m_cols.arType;

		m_cols.dwCount = cols.size();
		m_cols.arId = new WORD[cols.size()];
		m_cols.arType = new BYTE[cols.size()];

		for (UINT i = 0; i < cols.size(); i++)
		{
			m_cols.arId[i] = cols[i].colName.m_nId;
			m_cols.arType[i] = (BYTE)cols[i].eColType;
		}

		m_pSetData->init();
		m_bStructInitialized = TRUE;
		return TRUE;
	}

	void CFieldSetImpl::getCols(std::vector<COL_PROP> & cols)
	{
		cols.clear();

		if (checkStructInitialized())
		{
			for (UINT i = 0; i < m_cols.dwCount; i++)
			{
				COL_PROP prop;
				prop.colName = m_cols.arId[i];
				prop.eColType = (E_FIELD)m_cols.arType[i];
				prop.bVirtual = FALSE;

				cols.push_back(prop);
			}	
		}
	}

	BOOL CFieldSetImpl::findCol(const NAME_ID &col, DWORD *lpIndex, E_FIELD *lpType)
	{
		for (UINT i = 0; i < m_cols.dwCount; i++)
		{
			if (m_cols.arId[i] == col.m_nId)
			{
				*lpIndex = i;
				*lpType = (E_FIELD)m_cols.arType[i];

				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CFieldSetImpl::checkStructInitialized()
	{
		if (!m_bStructInitialized)
		{
			CAutoLock lock(m_csEditLock);
			
			if (!m_bStructInitialized)
			{				
				FIELDSET_STRUCT fieldSetStruct;
				if (m_pDatabaseContext->queryFieldSetStruct(m_fieldSetName, &fieldSetStruct))
				{
					init(&fieldSetStruct);	
				}
				else
				{
					LPCTSTR lpszName = m_fieldSetName.GetName();
					Error::OutputError(_T("fieldset%s延迟的初始化时发现fieldset结构信息不存在"), lpszName ? lpszName : _T("空"));
				}
			}
		}
		
		return m_bStructInitialized;
	}

	void CFieldSetImpl::getContentStr(std::wstring &wstrOut)
	{
		m_pSetData->getContentStr(wstrOut); 
	}

	void CFieldSetImpl::dump()
	{
		assert(m_bStructInitialized);
		if (!m_bStructInitialized)
			return;

		std::wstring strContent;
		m_pSetData->getContentStr(strContent);

		Error::OutputDebug(_T("fieldsetedit dump: name=%s, ptr=0x%08X(%u), ref=%d, fieldset-ref=%d, %s"), 
			m_fieldSetName.GetName(), 
			this,
			this,
			m_dwRef,
			m_pSetData->getRefCount(),
			strContent.c_str());
	}
};
