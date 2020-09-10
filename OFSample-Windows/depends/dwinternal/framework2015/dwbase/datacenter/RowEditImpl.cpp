#include "stable.h"
#include "RowEditImpl.h"
#include "RowImpl.h"
#include "MemPool/MemPool.h"
#include "CriticalSection.h"
#include "DatabaseLeakDetector.h"

namespace Data
{
	CRowEditImpl::CRowEditImpl(CTableEditImpl * pTableEdit, const CTableEditImpl::STORE_CONTEXT &storeContext, CRowImpl *pRow)
	{
		m_dwRef = 1;

		pTableEdit->addRef();
		m_pTableEdit = pTableEdit;

		m_storeContext = storeContext;

		pRow->addRef();
		m_pRow = pRow;

		DBLEAK_INSERT_ROWEDIT(m_pRow->getTableData()->getDatabaseContext(), this);
	}

	CRowEditImpl::~CRowEditImpl(void)
	{
		DBLEAK_REMOVE_PTR(m_pRow->getTableData()->getDatabaseContext(),this);

		assert(m_dwRef == 0);

		m_pRow->release();
		m_pRow = NULL;

		m_pTableEdit->release();
		m_pTableEdit = NULL;
	}

	CRowEditImpl * CRowEditImpl::createInstance(CTableEditImpl * pTableEdit, const CTableEditImpl::STORE_CONTEXT &storeContext, CRowImpl *pRow)
	{
		return new (mp_alloc(sizeof(CRowEditImpl))) CRowEditImpl(pTableEdit, storeContext, pRow);
	}

	void CRowEditImpl::addRef()
	{		
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CRowEditImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
		{
			this->~CRowEditImpl();
			mp_free(this);
		}
	}

	IWeakRef* CRowEditImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CRowEditImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	LPCWSTR	CRowEditImpl::getStr(const NAME_ID &name)
	{
		return m_pRow->getStr(name);
	}

	DWORD64	CRowEditImpl::getDword64(const NAME_ID &name)
	{
		return m_pRow->getDword64(name);
	}

	DWORD CRowEditImpl::getDword(const NAME_ID &name)
	{
		return m_pRow->getDword(name);
	}

	WORD CRowEditImpl::getWord(const NAME_ID &name)
	{
		return m_pRow->getWord(name);
	}

	BYTE CRowEditImpl::getByte(const NAME_ID &name)
	{
		return m_pRow->getByte(name);
	}

	BOOL CRowEditImpl::getBool(const NAME_ID &name)
	{
		return m_pRow->getBool(name);
	}	

	const BYTE * CRowEditImpl::getBlob(const NAME_ID &name, DWORD * lpdwSize)
	{
		return m_pRow->getBlob(name, lpdwSize);
	}	

	comptr<ITableEdit> CRowEditImpl::getSubTable(const NAME_ID &name)
	{
		ITablePtr p = m_pRow->getSubTable(name);
		if (p)
			return p->getEdit();
		return NULL;
	}

	BOOL CRowEditImpl::hasData(const NAME_ID &name)
	{
		return m_pRow->hasData(name);
	}	

	LPCWSTR CRowEditImpl::getPropStr(const NAME_ID &name)
	{
		return m_pRow->getPropStr(name);
	}

	DWORD64 CRowEditImpl::getPropDword64(const NAME_ID &name)
	{
		return m_pRow->getPropDword64(name);
	}

	DWORD CRowEditImpl::getPropDword(const NAME_ID &name)
	{
		return m_pRow->getPropDword(name);
	}

	WORD CRowEditImpl::getPropWord(const NAME_ID &name)
	{
		return m_pRow->getPropWord(name);
	}

	BYTE CRowEditImpl::getPropByte(const NAME_ID &name)
	{
		return m_pRow->getPropByte(name);
	}

	BOOL CRowEditImpl::getPropBool(const NAME_ID &name)
	{
		return m_pRow->getPropBool(name);
	}

	comptr<IUnk> CRowEditImpl::getPropUnk(const NAME_ID &name)
	{
		return m_pRow->getPropUnk(name);
	}

	BOOL CRowEditImpl::hasProp(const NAME_ID &name)
	{
		return m_pRow->hasProp(name);
	}

	BOOL CRowEditImpl::setPropStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		return m_pRow->setPropStr(name, lpwszValue);
	}

	BOOL CRowEditImpl::setPropDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		return m_pRow->setPropDword64(name, dw64Value);
	}

	BOOL CRowEditImpl::setPropDword(const NAME_ID &name, DWORD dwValue)
	{
		return m_pRow->setPropDword(name, dwValue);
	}

	BOOL CRowEditImpl::setPropWord(const NAME_ID &name, WORD wValue)
	{
		return m_pRow->setPropWord(name, wValue);
	}

	BOOL CRowEditImpl::setPropByte(const NAME_ID &name, BYTE byValue)
	{
		return m_pRow->setPropByte(name, byValue);
	}

	BOOL CRowEditImpl::setPropBool(const NAME_ID &name, BOOL bValue)
	{
		return m_pRow->setPropBool(name, bValue);
	}

	BOOL CRowEditImpl::setPropUnk(const NAME_ID &name, IUnk *pUnk)
	{
		return m_pRow->setPropUnk(name, pUnk);
	}

	BOOL CRowEditImpl::removeProp(const NAME_ID &name)
	{
		return m_pRow->removeProp(name);
	}

	void CRowEditImpl::detach()
	{
		CTableDataImpl * pTableData = m_pRow->getTableData();

		if (m_pRow->getRowState() == CRowImpl::ROW_STATE_E_INTABLE && !pTableData->isCurrentEditRow(m_pRow))
		{
			pTableData->notifyRowEditActionEnd();

			CRowImpl * pRowNew = m_pTableEdit->detachRow(m_storeContext, m_pRow);
			CRowImpl * pRowOrig = m_pRow;
			m_pRow = pRowNew;

			assert(pRowNew->getRowState() == CRowImpl::ROW_STATE_E_INTABLE);
			pRowOrig->setRowState(CRowImpl::ROW_STATE_E_REMOVE);

			pTableData->notifyRowEditActionBegin(pRowOrig, pRowNew);
		}
	}

	BOOL CRowEditImpl::verifyEditEnable(const NAME_ID &name)
	{
		if (m_pRow->getTableData()->getIndexColName().m_nId == name.m_nId && m_pRow->getRowState() != CRowImpl::ROW_STATE_E_ALLOC)
		{
			LPCWSTR lpszName = m_pRow->getTableData()->getTableName().GetName();
			Error::OutputError(_T("禁止修改已插入表 %s 的row索引字段！！"), lpszName ? lpszName : _T("空"));

			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	BOOL CRowEditImpl::setStr(const NAME_ID &name, LPCWSTR lpwszValue)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{
			detach();
			return m_pRow->setStr(name, lpwszValue);
		}
		return FALSE;
	}

	BOOL CRowEditImpl::setDword64(const NAME_ID &name, DWORD64 dw64Value)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{
			detach();
			return m_pRow->setDword64(name, dw64Value);
		}
		return FALSE;
	}

	BOOL CRowEditImpl::setDword(const NAME_ID &name, DWORD dwValue)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{
			detach();
			return m_pRow->setDword(name, dwValue);
		}
		return FALSE;
	}

	BOOL CRowEditImpl::setWord(const NAME_ID &name, WORD wValue)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{	
			detach();
			return m_pRow->setWord(name, wValue);
		}
		return FALSE;
	}
	
	BOOL CRowEditImpl::setByte(const NAME_ID &name, BYTE byValue)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{	
			detach();
			return m_pRow->setByte(name, byValue);
		}
		return FALSE;
	}
	
	BOOL CRowEditImpl::setBool(const NAME_ID &name, BOOL bValue)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{	
			detach();
			return m_pRow->setBool(name, bValue);
		}
		return FALSE;
	}

	BOOL CRowEditImpl::setBlob(const NAME_ID &name, const BYTE * pbData, DWORD cbSize)
	{
		if (m_pRow->getTableData()->verifyEditEnable() && verifyEditEnable(name))
		{	
			detach();
			return m_pRow->setBlob(name, pbData, cbSize);
		}
		return FALSE;
	}

	BOOL CRowEditImpl::setSubTable(const NAME_ID &name, ITableEdit *pSubTable)
	{
		if (m_pRow->getTableData()->verifyEditEnable())
		{	
			detach();
			return m_pRow->setSubTable(name, pSubTable);
		}
		return FALSE;
	}

	BOOL CRowEditImpl::removeData(const NAME_ID &name)
	{
		if (m_pRow->getTableData()->verifyEditEnable())
		{	
			detach();
			return m_pRow->removeData(name);
		}
		return FALSE;
	}

	void CRowEditImpl::dump()
	{
		std::wstring strContent;
		m_pRow->getContentStr(strContent);

		Error::OutputDebug(_T("row-edit dump: table=%s, ptr=0x%08X(%u), ref=%d, row-ref=%d, %s"), 
			m_pRow->getTableData()->getTableName().GetName(), 
			this,
			this,
			m_dwRef,
			m_pRow->getRefCount(),
			strContent.c_str());
	}
};
