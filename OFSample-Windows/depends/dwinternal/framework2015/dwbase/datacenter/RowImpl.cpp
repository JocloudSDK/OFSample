#include "stable.h"
#include "RowImpl.h"
#include "MemPool/MemPool.h"
#include "FieldBlob.h"
#include "EventDispatch.h"
#include "DatabaseLeakDetector.h"
#include "ConditionExpr.h"

namespace Data
{
	CRowImpl::CRowImpl(CTableDataImpl *pTableData, BOOL bByClone)
	{
		pTableData->addRef();
		m_pTableData = pTableData;

		m_dwRef = 1;
		m_pPropData = bByClone ? NULL : CPropData::createInstance();
		m_cRowState = ROW_STATE_E_ALLOC;

		DBLEAK_INSERT_ROW(m_pTableData->getDatabaseContext(), this);
	}

	CRowImpl::~CRowImpl(void)
	{
		DBLEAK_REMOVE_PTR(m_pTableData->getDatabaseContext(),this);

		assert(m_dwRef == 0);
		assert(m_cRowState == ROW_STATE_E_ALLOC || m_cRowState == ROW_STATE_E_REMOVE);

		const CTableDataImpl::REAL_COLS *pCols = m_pTableData->getRealCols();
		void *pValue = getValuePtr();
		for (UINT i = 0; i < pCols->dwColCount; i++)
		{
			DWORD dwOffsetByte = pCols->arCol[i].wOffsetBit / 8;

			switch(pCols->arCol[i].wType)
			{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
			case FIELD_E_BOOL:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
			case FIELD_E_DWORD64:
				break;
			case FIELD_E_STR:
				{
					CFieldStringW * p = *(CFieldStringW **)((PBYTE)pValue + dwOffsetByte);
					if (p)
						p ->release();
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob * p = *(CFieldBlob **)((PBYTE)pValue + dwOffsetByte);
					if (p)
						p ->release();
				}
				break;
			case FIELD_E_TABLE:
				{
					ITableEdit *p = *(ITableEdit **)((PBYTE)pValue + dwOffsetByte);
					if (p)
						p->release();
				}
				break;
			default:
				assert(! L"不支持的数据类型");
				break;				
			}
		}

		memset(this + 1, 0, pCols->dwValuesSize + pCols->dwExistBitSize + pCols->dwModifiedBitSize);

		m_pPropData->release();
		m_pPropData = NULL;

		m_pTableData->release();
		m_pTableData = NULL;;
	}

	CRowImpl * CRowImpl::createInstance(CTableDataImpl *pTableData, BOOL bByClone)
	{
		const CTableDataImpl::REAL_COLS *pCols = pTableData->getRealCols();
		DWORD dwExtraSize = pCols->dwValuesSize + pCols->dwExistBitSize + pCols->dwModifiedBitSize;

		CRowImpl *p = new (mp_alloc(sizeof(CRowImpl) + dwExtraSize)) CRowImpl(pTableData, bByClone);
		memset(p + 1, 0, dwExtraSize);

		return p;
	}

	void CRowImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CRowImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
		{
			this->~CRowImpl();
			mp_free(this);
		}
	}

	IWeakRef* CRowImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CRowImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	BOOL CRowImpl::findColWithVerify(const NAME_ID &name, E_FIELD eType, DWORD *lpIndex, DWORD *lpOffsetBit) const 
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		E_FIELD eTypeRes;
		if (!m_pTableData->findCol(name, &dwIndex, &dwOffsetBit, &eTypeRes))
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时实体字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		if (eTypeRes != eType)
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时字段 %s 类型错误！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		*lpOffsetBit = dwOffsetBit;
		*lpIndex = dwIndex;

		return TRUE;
	}

	LPCWSTR	CRowImpl::getStr(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_STR, &dwIndex, &dwOffsetBit))
		{
			const CFieldStringW * pStr = getXXXDirect<const CFieldStringW *>(dwOffsetBit);
			if (pStr)
				return pStr->getStr();
		}

		return _T("");
	}

	const BYTE * CRowImpl::getBlob(const NAME_ID &name, DWORD * lpdwSize)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_BLOB, &dwIndex, &dwOffsetBit))
		{
			const CFieldBlob * pBlob = getXXXDirect<const CFieldBlob *>(dwOffsetBit);
			if (pBlob)
			{
				if (lpdwSize)
					*lpdwSize = pBlob->getSize();
				return pBlob->getPtr();
			}
		}

		if (lpdwSize)
			*lpdwSize = 0;
		return NULL;
	}

	DWORD64	CRowImpl::getDword64(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_DWORD64, &dwIndex, &dwOffsetBit))
			return getXXXDirect<DWORD64>(dwOffsetBit);

		return 0;
	}

	DWORD CRowImpl::getDword(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_DWORD, &dwIndex, &dwOffsetBit))
			return getXXXDirect<DWORD>(dwOffsetBit);

		return 0;
	}

	WORD CRowImpl::getWord(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_WORD, &dwIndex, &dwOffsetBit))
			return getXXXDirect<WORD>(dwOffsetBit);

		return 0;
	}

	BYTE CRowImpl::getByte(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_BYTE, &dwIndex, &dwOffsetBit))
			return getXXXDirect<BYTE>(dwOffsetBit);

		return 0;
	}

	BOOL CRowImpl::getBool(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_BOOL, &dwIndex, &dwOffsetBit))
			return  Bitmap::getBit(getValuePtr(), dwOffsetBit);

		return FALSE;
	}

	comptr<ITable> CRowImpl::getSubTable(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_TABLE, &dwIndex, &dwOffsetBit))
		{
			ITableEdit * pEdit = getXXXDirect<ITableEdit *>(dwOffsetBit);
			if (pEdit)
				return pEdit->getTable();
		}

		return NULL;
	}

	BOOL CRowImpl::hasData(const NAME_ID &name)
	{
		BOOL bRet = FALSE;

		DWORD dwIndex;
		DWORD dwOffsetBit;
		E_FIELD eTypeRes;
		if (m_pTableData->findCol(name, &dwIndex, &dwOffsetBit, &eTypeRes))
		{
			bRet = getExistBit(dwIndex);
		}
		else
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时实体字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
		}

		return bRet;
	}

	const DWVariant	CRowImpl::getVirtualField(const NAME_ID &name)
	{
		DWVariant ret;

		DWORD dwIndex;
		E_FIELD eTypeRes;
		if (m_pTableData->findVirtualCol(name, &dwIndex, &eTypeRes))
		{
			if (!m_pTableData->notifyCalcField(this, name, &ret))
			{
				LPCWSTR lpszName = name.GetName();
				Error::OutputError(_T("访问数据中心时虚拟字段 %s 未被回调函数处理！！"), lpszName ? lpszName : _T("空"));
			}
		}
		else
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时虚拟字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
		}

		return ret;
	}

	LPCWSTR CRowImpl::getPropStr(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropStr(name);
	}

	DWORD64 CRowImpl::getPropDword64(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropDword64(name);
	}

	DWORD CRowImpl::getPropDword(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropDword(name);
	}

	WORD CRowImpl::getPropWord(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropWord(name);
	}

	BYTE CRowImpl::getPropByte(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropByte(name);
	}

	BOOL CRowImpl::getPropBool(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropBool(name);
	}

	comptr<IUnk> CRowImpl::getPropUnk(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->getPropUnk(name);
	}

	BOOL CRowImpl::hasProp(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->hasProp(name);
	}

	BOOL CRowImpl::setPropStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropStr(name, lpwszValue);
	}

	BOOL CRowImpl::setPropDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropDword64(name, dw64Value);
	}

	BOOL CRowImpl::setPropDword(const NAME_ID &name, DWORD dwValue)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropDword(name, dwValue);
	}

	BOOL CRowImpl::setPropWord(const NAME_ID &name, WORD wValue)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropWord(name, wValue);
	}

	BOOL CRowImpl::setPropByte(const NAME_ID &name, BYTE byValue)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropByte(name, byValue);
	}

	BOOL CRowImpl::setPropBool(const NAME_ID &name, BOOL bValue)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropBool(name, bValue);
	}

	BOOL CRowImpl::setPropUnk(const NAME_ID &name, IUnk *pUnk)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->setPropUnk(name, pUnk);
	}

	BOOL CRowImpl::removeProp(const NAME_ID &name)
	{
		CAutoLock lock(m_pTableData->getPropEditLock());
		return m_pPropData->removeProp(name);
	}

	BOOL CRowImpl::isModified(const NAME_ID &name)
	{
		BOOL bRet = FALSE;

		DWORD dwIndex;
		DWORD dwOffsetBit;
		E_FIELD eTypeRes;
		if (m_pTableData->findCol(name, &dwIndex, &dwOffsetBit, &eTypeRes))
		{
			bRet = getModifiedBit(dwIndex);
		}
		else
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("访问数据中心时实体字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
		}

		return bRet;
	}

	BOOL CRowImpl::isMatch(const CFilter *pFilter)
	{
		return pFilter 
			? isMatch(pFilter->_private()->getMatchTree(m_pTableData->getRealCols()))
			: FALSE;
	}

	BOOL CRowImpl::getModifyNameIds(NAME_IDS &nameIds)
	{
		if (m_modifyIndexs.empty())
		{
			return FALSE;
		}

		std::vector<DWORD>::iterator iter = m_modifyIndexs.begin();
		while (iter != m_modifyIndexs.end())
		{        
			WORD id = ((WORD)(((DWORD_PTR)(m_pTableData->getRealCols()->arCol[*iter].dwId)) & 0xffff));
			nameIds << NAME_ID(id);

			++iter;
		}

		return TRUE;
	}

	BOOL CRowImpl::removeData(const NAME_ID &name)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		E_FIELD eTypeRes;
		if (!m_pTableData->findCol(name, &dwIndex, &dwOffsetBit, &eTypeRes))
		{
			LPCWSTR lpszName = name.GetName();
			Error::OutputError(_T("remove数据中心时字段 %s 不存在！！"), lpszName ? lpszName : _T("空"));
			
			return FALSE;
		}

		const BOOL isExist = getExistBit(dwIndex);
		if (!isExist)
			return FALSE;

		switch (eTypeRes)
		{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
				*getXXXDirectPtr<DWORD>(dwOffsetBit) = 0;
				break;
			case FIELD_E_BOOL:
				Bitmap::setBit(getValuePtr(), dwOffsetBit, FALSE);
				break;				
			case FIELD_E_WORD:
				*getXXXDirectPtr<WORD>(dwOffsetBit) = 0;
				break;
			case FIELD_E_BYTE:
				*getXXXDirectPtr<BYTE>(dwOffsetBit) = 0;
				break;
			case FIELD_E_STR:
				{
					CFieldStringW **ppStr = getXXXDirectPtr<CFieldStringW *>(dwOffsetBit);
					(*ppStr)->release();
					*ppStr = NULL;
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob **ppBlob = getXXXDirectPtr<CFieldBlob *>(dwOffsetBit);
					(*ppBlob)->release();
					*ppBlob = NULL;
				}
				break;
			case FIELD_E_DWORD64:
				*getXXXDirectPtr<DWORD64>(dwOffsetBit) = 0;
				break;
			case FIELD_E_TABLE:
				{
					ITableEdit **ppTable = getXXXDirectPtr<ITableEdit *>(dwOffsetBit);
					(*ppTable)->release();
					*ppTable = NULL;
				}
				break;
			default:
				assert(! L"不支持的字段类型");
				break;
		}

		setExistBit(dwIndex, FALSE);
		if (m_cRowState == CRowImpl::ROW_STATE_E_INTABLE)
			setModifiedBit(dwIndex, TRUE);

		return TRUE;
	}

	void CRowImpl::finishSetXXX(BOOL bValueChanged, DWORD dwIndex)
	{
		BOOL bDataChanged = bValueChanged || !getExistBit(dwIndex); //getExistBit调用放到后面，提高性能(也许不需要进入呢)
		if (bDataChanged)
		{
			setExistBit(dwIndex, TRUE);

			if (m_cRowState == CRowImpl::ROW_STATE_E_INTABLE)
			{
				setModifiedBit(dwIndex, TRUE);
				m_modifyIndexs.push_back(dwIndex);
			}
		}
		else
		{
			assert(getExistBit(dwIndex) == TRUE);//原来应该是存在这个字段的
		}
	}

	BOOL CRowImpl::setStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_STR, &dwIndex, &dwOffsetBit))
		{
			BOOL bValueChanged = FALSE;

			DWORD cchSrc = wcslen(lpwszValue);
			CFieldStringW **ppStr = getXXXDirectPtr<CFieldStringW *>(dwOffsetBit);
			if (*ppStr)
			{
				bValueChanged = !(*ppStr)->isEqual(lpwszValue, cchSrc);
				if (bValueChanged)
				{
					(*ppStr)->release();
					*ppStr = NULL;
				}
			}
			else
			{
				bValueChanged = TRUE;
			}

			if (bValueChanged)
				*ppStr = CFieldStringW::createInstance(lpwszValue, cchSrc);

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	BOOL CRowImpl::setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_BLOB, &dwIndex, &dwOffsetBit))
		{
			BOOL bValueChanged = FALSE;

			CFieldBlob **ppBlob = getXXXDirectPtr<CFieldBlob *>(dwOffsetBit);
			if (*ppBlob)
			{
				bValueChanged = !(*ppBlob)->isEqual(pbData, cbSize);
				if (bValueChanged)
				{
					(*ppBlob)->release();
					*ppBlob = NULL;
				}
			}
			else
			{
				bValueChanged = TRUE;
			}

			if (bValueChanged)
				*ppBlob = CFieldBlob::createInstance(pbData, cbSize);

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	BOOL CRowImpl::setDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_DWORD64, &dwIndex, &dwOffsetBit))
		{
			DWORD64 *p = getXXXDirectPtr<DWORD64>(dwOffsetBit);
			
			BOOL bValueChanged = *p != dw64Value;
			if (bValueChanged)
				*p = dw64Value;

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	BOOL CRowImpl::setDword(const NAME_ID &name, DWORD dwValue)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_DWORD, &dwIndex, &dwOffsetBit))
		{
			DWORD *p = getXXXDirectPtr<DWORD>(dwOffsetBit);
			
			BOOL bValueChanged = *p != dwValue;
			if (bValueChanged)
				*p = dwValue;

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	BOOL CRowImpl::setWord(const NAME_ID &name, WORD wValue)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_WORD, &dwIndex, &dwOffsetBit))
		{
			WORD *p = getXXXDirectPtr<WORD>(dwOffsetBit);
			
			BOOL bValueChanged = *p != wValue;
			if (bValueChanged)
				*p = wValue;

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}
	
	BOOL CRowImpl::setByte(const NAME_ID &name, BYTE byValue)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_BYTE, &dwIndex, &dwOffsetBit))
		{
			BYTE *p = getXXXDirectPtr<BYTE>(dwOffsetBit);
			
			BOOL bValueChanged = *p != byValue;
			if (bValueChanged)
				*p = byValue;

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	BOOL CRowImpl::setBool(const NAME_ID &name, BOOL bValue)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_BOOL, &dwIndex, &dwOffsetBit))
		{			
			BOOL bValueChanged = Bitmap::getBit(getValuePtr(), dwOffsetBit) != bValue;
			if (bValueChanged)
				Bitmap::setBit(getValuePtr(), dwOffsetBit, bValue);

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	BOOL CRowImpl::setSubTable(const NAME_ID &name, ITableEdit *pSubTable)
	{
		DWORD dwIndex;
		DWORD dwOffsetBit;
		if (findColWithVerify(name, FIELD_E_TABLE, &dwIndex, &dwOffsetBit))
		{
			ITableEdit **ppTblEdit = getXXXDirectPtr<ITableEdit *>(dwOffsetBit);
			
			BOOL bValueChanged = *ppTblEdit != pSubTable;
			if (bValueChanged)
			{
				if (*ppTblEdit)
					(*ppTblEdit)->release();

				pSubTable->addRef();
				*ppTblEdit = pSubTable;
			}

			finishSetXXX(bValueChanged, dwIndex);

			return TRUE;
		}

		return FALSE;
	}

	CRowImpl * CRowImpl::clone() const
	{
		CRowImpl *pNew = CRowImpl::createInstance(m_pTableData, TRUE);
	
		const CTableDataImpl::REAL_COLS *pCols = m_pTableData->getRealCols();
		
		const void *pValue = getValuePtr();
		void *pValueNew = pNew->getValuePtr();
		memcpy(pValueNew, pValue, pCols->dwValuesSize);

		for (UINT i = 0; i < pCols->dwColCount; i++)
		{
			switch(pCols->arCol[i].wType)
			{
			case FIELD_E_DWORD:	//最多可能出现的排前面，提高性能
			case FIELD_E_BOOL:
			case FIELD_E_WORD:
			case FIELD_E_BYTE:
			case FIELD_E_DWORD64:
				break;
			case FIELD_E_STR:
				{
					CFieldStringW *pStr = *(CFieldStringW **)((BYTE *)pValue + (pCols->arCol[i].wOffsetBit >> 3));
					if (pStr)
						pStr->addRef();
				}
				break;
			case FIELD_E_BLOB:
				{
					CFieldBlob *pBlob = *(CFieldBlob **)((BYTE *)pValue + (pCols->arCol[i].wOffsetBit >> 3));
					if (pBlob)
						pBlob->addRef();
				}
				break;
			case FIELD_E_TABLE:
				{
					ITableEdit *p = *(ITableEdit **)((BYTE *)pValue + (pCols->arCol[i].wOffsetBit >> 3));
					if (p)
						p->addRef();
				}
				break;
			default:
				assert(! L"不支持的字段类型");
				break;				
			}
		}

		memcpy(pNew->getExistBitPtr(pCols), getExistBitPtr(pCols), pCols->dwExistBitSize);
		//注意m_bitModified不需要复制

		m_pPropData->addRef();
		assert(pNew->m_pPropData == NULL);
		pNew->m_pPropData = m_pPropData;

		//复制Row状态
		assert(m_cRowState == ROW_STATE_E_INTABLE); //暂时应该只有ROW_STATE_E_INTABLE才可能被克隆
		pNew->m_cRowState = m_cRowState;

		return pNew;
	}

	BOOL CRowImpl::getKey(KEY *pKey) const
	{
		pKey->eType = KEY_E_NULL;
		pKey->unKey.dwKey = 0;

		E_FIELD eFieldType = m_pTableData->getIndexColType();
		DWORD dwOffsetBit = m_pTableData->getIndexColOffsetBit();

		switch (eFieldType)
		{
		case FIELD_E_DWORD:
			pKey->eType = KEY_E_DWORD;
			pKey->unKey.dwKey = getXXXDirect<DWORD>(dwOffsetBit);
			break;
		case FIELD_E_WORD:
			pKey->eType = KEY_E_DWORD;
			pKey->unKey.dwKey = getXXXDirect<WORD>(dwOffsetBit);
			break;
		case FIELD_E_BYTE:
			pKey->eType = KEY_E_DWORD;
			pKey->unKey.dwKey = getXXXDirect<BYTE>(dwOffsetBit);
			break;
		case FIELD_E_STR:
			{
				CFieldStringW *pStr = getXXXDirect<CFieldStringW *>(dwOffsetBit);
				pKey->eType = KEY_E_STR;
				pKey->unKey.lpszKey = pStr->getStr();
			}
			break;
		default:
			assert(0);
			break;
		}

		return pKey->eType != KEY_E_NULL;
	}

	BOOL CRowImpl::setKey(const KEY & key)
	{
		assert(getRowState() == ROW_STATE_E_ALLOC);

		BOOL bError = FALSE;

		E_FIELD eFieldType = m_pTableData->getIndexColType();	
		NAME_ID indexColName = m_pTableData->getIndexColName();

		switch (eFieldType)
		{
			case FIELD_E_DWORD:
				setDword(indexColName, key.unKey.dwKey);
				break;
			case FIELD_E_WORD:
				setWord(indexColName, (WORD)key.unKey.dwKey);
				break;
			case FIELD_E_BYTE:
				setByte(indexColName, (BYTE)key.unKey.dwKey);		
				break;
			case FIELD_E_STR:
				setStr(indexColName, key.unKey.lpszKey);		
				break;
			default:
				bError = TRUE;
				assert(! L"不支持的key字段类型");
				break;
		}

		return !bError;
	}

	BOOL CRowImpl::isMatchCaseItem(const MATCH_CASE & caseItem, const void *pValue, const void *pExistBit, const void *pModifiedBit)
	{
		BOOL bIsMatch = FALSE;

		switch (caseItem.eMatchCase)
		{
		case CASE_E_EQUAL:
		case CASE_E_LESS:
		case CASE_E_GREATER:
			bIsMatch = Bitmap::getBit(pExistBit, caseItem.dwColIndex) && caseItem.pfnMatch(caseItem.dwDum, pValue, caseItem.dwOffsetBit);
			break;
		case CASE_E_NOT_NULL:
			bIsMatch = Bitmap::getBit(pExistBit, caseItem.dwColIndex);
			break;
		case CASE_E_IS_NULL:
			bIsMatch = !Bitmap::getBit(pExistBit, caseItem.dwColIndex);
			break;
		case CASE_E_NOT_EQUAL:
			bIsMatch = !Bitmap::getBit(pExistBit, caseItem.dwColIndex) || caseItem.pfnMatch(caseItem.dwDum, pValue, caseItem.dwOffsetBit);
			break;
		case CASE_E_MODIFIED:
			bIsMatch = Bitmap::getBit(pModifiedBit, caseItem.dwColIndex);
			break;
		default:
			assert(! L"不支持的匹配选项");
			break;
		}

		return bIsMatch;
	}

	BOOL CRowImpl::isMatchHelper(const MATCH_NODE *pMatchNode, const void *pValue, const void *pExistBit, const void *pModifiedBit)
	{
		assert((pMatchNode->arMatchCase == NULL) == (pMatchNode->dwMatchCaseCount == 0));
		assert((pMatchNode->arChild == NULL) == (pMatchNode->dwChildCount == 0));

		if (pMatchNode->dwChildCount == 0 && pMatchNode->dwMatchCaseCount == 0)
			return TRUE;

		BOOL bIsMatch;

		if (pMatchNode->eMode == COMBINE_E_AND)
		{
			bIsMatch = TRUE;

			if (pMatchNode->dwChildCount == 0)//属于叶子节点
			{
				for (UINT i = 0; i < pMatchNode->dwMatchCaseCount && bIsMatch; i++)
					bIsMatch = isMatchCaseItem(pMatchNode->arMatchCase[i], pValue, pExistBit, pModifiedBit);
			}
			else
			{
				for (UINT i = 0; i < pMatchNode->dwChildCount && bIsMatch; i++)
					bIsMatch = isMatchHelper(&pMatchNode->arChild[i], pValue, pExistBit, pModifiedBit);
			}
		}
		else
		{
			assert(pMatchNode->eMode == COMBINE_E_OR);

			bIsMatch = FALSE;

			if (pMatchNode->dwChildCount == 0)//属于叶子节点
			{
				for (UINT i = 0; i < pMatchNode->dwMatchCaseCount && !bIsMatch; i++)
					bIsMatch = isMatchCaseItem(pMatchNode->arMatchCase[i], pValue, pExistBit, pModifiedBit);
			}
			else
			{
				for (UINT i = 0; i < pMatchNode->dwChildCount && !bIsMatch; i++)
					bIsMatch = isMatchHelper(&pMatchNode->arChild[i], pValue, pExistBit, pModifiedBit);
			}
		}

		return bIsMatch;
	}

	BOOL CRowImpl::isLess(const CRowImpl *pRowDst, const ORDER_CASE *pOrderCases, UINT nCount) const
	{
		assert(nCount > 0);
		assert(m_pTableData == pRowDst->m_pTableData);

		const void * pValueSrc = getValuePtr();
		const void * pValueDst = pRowDst->getValuePtr();
		for (UINT i = 0; i < nCount; i++)
		{
			BOOL bIsExistSrc = getExistBit(pOrderCases[i].dwColIndex);
			BOOL bIsExistDst = pRowDst->getExistBit(pOrderCases[i].dwColIndex);
			DWORD dwOffsetByte = pOrderCases[i].dwOffsetBit >> 3;
		
			if (bIsExistSrc && bIsExistDst)
			{
				switch (pOrderCases[i].eType)
				{
				case FIELD_E_DWORD://最大可能的排前面，提高性能
					{
						DWORD dwSrc = *(DWORD *)((BYTE *)pValueSrc + dwOffsetByte);
						DWORD dwDst = *(DWORD *)((BYTE *)pValueDst + dwOffsetByte);

						if (dwSrc < dwDst)
							return pOrderCases[i].bAscending;
						else if (dwSrc > dwDst)
							return !pOrderCases[i].bAscending;
						//注意相等不做处理，继续比较下一个字段
					}
					break;
				case FIELD_E_WORD:
					{
						DWORD dwSrc = *(WORD *)((BYTE *)pValueDst + dwOffsetByte);
						DWORD dwDst = *(WORD *)((BYTE *)pValueDst + dwOffsetByte);

						if (dwSrc < dwDst)
							return pOrderCases[i].bAscending;
						else if (dwSrc > dwDst)
							return !pOrderCases[i].bAscending;
					}
					break;
				case FIELD_E_BYTE:
					{
						DWORD dwSrc = *(BYTE *)((BYTE *)pValueDst + dwOffsetByte);
						DWORD dwDst = *(BYTE *)((BYTE *)pValueDst + dwOffsetByte);

						if (dwSrc < dwDst)
							return pOrderCases[i].bAscending;
						else if (dwSrc > dwDst)
							return !pOrderCases[i].bAscending;
					}
					break;
				case FIELD_E_BOOL:
					{
						BOOL bSrc = Bitmap::getBit(pValueSrc, pOrderCases[i].dwOffsetBit);
						BOOL bDst = Bitmap::getBit(pValueDst, pOrderCases[i].dwOffsetBit);

						if (!bSrc && bDst)
							return pOrderCases[i].bAscending;
						else if (bSrc && !bDst)
							return !pOrderCases[i].bAscending;
					}
					break;
				case FIELD_E_STR:
					{
						const CFieldStringW * pSrc = *(CFieldStringW **)((BYTE *)pValueDst + dwOffsetByte);
						const CFieldStringW * pDst = *(CFieldStringW **)((BYTE *)pValueDst + dwOffsetByte);
						int nCompare = wcscmp(pSrc->getStr(), pDst->getStr());

						if (nCompare < 0)
							return pOrderCases[i].bAscending;
						else if(nCompare > 0)
							return !pOrderCases[i].bAscending;
					}
					break;
				case FIELD_E_DWORD64:
					{
						DWORD64 dw64Src = *(DWORD64 *)((BYTE *)pValueDst + dwOffsetByte);
						DWORD64 dw64Dst = *(DWORD64 *)((BYTE *)pValueDst + dwOffsetByte);

						if (dw64Src < dw64Dst)
							return pOrderCases[i].bAscending;
						else if (dw64Src > dw64Dst)
							return !pOrderCases[i].bAscending;
					}
					break;
				default:
					assert(! L"排序Row出现不支持字段类型");
					break;
				}
			}
			else if (bIsExistSrc)
			{
				return !pOrderCases[i].bAscending;//FALSE
			}
			else if (bIsExistDst)
			{
				return pOrderCases[i].bAscending;//TRUE
			}
		}

		return FALSE;
	}
	
	void CRowImpl::setRowState(E_ROW_STATE eRowState)
	{
		switch (eRowState)
		{
			case ROW_STATE_E_PENGING:
			{
				assert(m_cRowState == ROW_STATE_E_ALLOC);
				m_cRowState = ROW_STATE_E_PENGING;
			}
			break;
		case ROW_STATE_E_INTABLE:
			{
				assert(m_cRowState == ROW_STATE_E_PENGING);
				m_cRowState = ROW_STATE_E_INTABLE;
			}
			break;
		case ROW_STATE_E_REMOVE:
			{
				assert(m_cRowState == ROW_STATE_E_INTABLE);
				m_cRowState = ROW_STATE_E_REMOVE;
			}
			break;
		default:
			assert(! L"Row状态错误");
			break;
		}
	}

	void CRowImpl::getContentStr(std::wstring &wstrOut)
	{
		TCHAR szTemp[1024] = {0};
		
		wstrOut = _T("");
		wstrOut.reserve(_countof(szTemp));
	
		const CTableDataImpl::REAL_COLS *pCols = m_pTableData->getRealCols();			
		for (UINT i = 0; i < pCols->dwColCount; i++)
		{
			const NAME_ID colName = (WORD)pCols->arCol[i].dwId;

			if (hasData(colName))
			{
				switch (pCols->arCol[i].wType)
				{
				case FIELD_E_TABLE:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=0x%08x, "), colName.GetName(), (unsigned int) getSubTable(colName).get());
					break;
				case FIELD_E_STR:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%s, "), colName.GetName(), getStr(colName));
					break;
				case FIELD_E_BLOB:
					{
						DWORD dwSize;
						const BYTE *pData = getBlob(colName, &dwSize);
						_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=ptr:0x%08x size:%d, "), colName.GetName(), (unsigned int) pData, dwSize);
					}
					break;
				case FIELD_E_DWORD64:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%I64u, "), colName.GetName(), getDword64(colName));
					break;
				case FIELD_E_DWORD:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%u, "), colName.GetName(), getDword(colName));
					break;
				case FIELD_E_WORD:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%u, "), colName.GetName(), getWord(colName));
					break;
				case FIELD_E_BYTE:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%u, "), colName.GetName(), getByte(colName));
					break;
				case FIELD_E_BOOL:
					_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=%s, "), colName.GetName(), getBool(colName) ? _T("true") : _T("false"));
					break;
				default:
					assert(! L"不支持的类型");
					break;
				}
			}
			else
			{
				_sntprintf_s(szTemp, _countof(szTemp), _TRUNCATE, _T("%s=NULL, "), colName.GetName());
			}

			wstrOut += szTemp;
		}

		assert(wstrOut.size() > 2);
		if (wstrOut.size() > 2)
			wstrOut.resize(wstrOut.size() - 2);
	}

	void CRowImpl::dump()
	{
		std::wstring strContent;
		getContentStr(strContent);

		Error::OutputDebug(_T("row dump: table=%s, ptr=0x%08X(%u), ref=%d, %s"), 
			m_pTableData->getTableName().GetName(), 
			this,
			this,
			m_dwRef,
			strContent.c_str());
	}
};
