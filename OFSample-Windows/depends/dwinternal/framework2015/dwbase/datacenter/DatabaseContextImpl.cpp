
#include "stable.h"
#include "DatabaseContextImpl.h"
#include <assert.h>
#include "DatabaseLeakDetector.h"

namespace Data
{
	CDatabaseContextImpl::CDatabaseContextImpl(DWORD dwId) 
		: m_tablePersistent(this)
		, m_dwId(dwId)
		, m_dwRef(1)
	{
		DBLEAK_DETECTOR_CREATE();
	}

	CDatabaseContextImpl::~CDatabaseContextImpl(void)
	{
		assert(m_dwRef == 0);

		if (m_pDetector)
			m_pDetector->release();
		m_pDetector = NULL;
	}

	CDatabaseContextImpl * CDatabaseContextImpl::createInstance(DWORD dwId)
	{
		return new CDatabaseContextImpl(dwId);
	}

	void CDatabaseContextImpl::addRef()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		::InterlockedIncrement((LONG *)&m_dwRef);
	}

	void CDatabaseContextImpl::release()
	{
		assert(m_dwRef > 0 && m_dwRef < 0x7fffffff);
		if (::InterlockedDecrement((LONG *)&m_dwRef) == 0)
			delete this;
	}

	IWeakRef* CDatabaseContextImpl::weakPtr()
	{
		assert(! L"不支持弱指针");
		return NULL;
	}

	bool CDatabaseContextImpl::queryInterface(REFDWUUID iid, void **ppv)
	{
		(void)iid;
		*ppv = NULL;
		assert(! L"不支持queryInterface");
		return false;
	}

	DWORD CDatabaseContextImpl::loadDataTemplate(LPCTSTR lpszFilePath)
	{
		CAutoLock autoLock(m_csLock);
		return m_xmlParser.LoadDataTemplate(lpszFilePath);
	}

	DWORD CDatabaseContextImpl::loadDataTemplate(const char *pBuffer, DWORD dwSize)
	{
		CAutoLock autoLock(m_csLock);
		return m_xmlParser.LoadDataTemplate(pBuffer, dwSize);
	}

	void CDatabaseContextImpl::unloadDataTemplate(DWORD dwFileFlag, std::vector<NAME_ID> &vecName)
	{
		CAutoLock autoLock(m_csLock);
		m_xmlParser.UnloadDataTemplate(dwFileFlag, vecName);
	}

	BOOL CDatabaseContextImpl::queryTableStruct(const NAME_ID& tableName, TABLE_STRUCT* pTableStruct) const
	{
		CAutoLock autoLock(m_csLock);
		return m_xmlParser.QueryTableStruct(tableName, pTableStruct);
	}

	BOOL CDatabaseContextImpl::queryFieldSetStruct(const NAME_ID& fieldSetName, FIELDSET_STRUCT* pStruct) const
	{
		CAutoLock autoLock(m_csLock);
		return m_xmlParser.QueryFieldSetStruct(fieldSetName, pStruct);
	}

	BOOL CDatabaseContextImpl::loadTable(ITableEditPtr spTable, LPCTSTR lpszTableDataPath)
	{
		return m_tablePersistent.load(spTable, lpszTableDataPath);
	}

	BOOL CDatabaseContextImpl::saveTable(ITablePtr spTable, LPCTSTR lpszTableDataPath)
	{
		return m_tablePersistent.save(spTable, lpszTableDataPath);
	}

	void CDatabaseContextImpl::queryViewStruct(const NAME_ID &tableName, std::vector<VIEW_STRUCT> &vecViewStruct) const
	{
		CAutoLock autoLock(m_csLock);
		m_xmlParser.QueryViewStruct(tableName, vecViewStruct);
	}


	void CDatabaseContextImpl::InsertDetectorPtr(PVOID ptr, DETECTOR_E_TYPE eObjType)
	{
		m_pDetector->InsertPtr(ptr, eObjType);
	}

	void CDatabaseContextImpl::RemoveDetectorPtr(PVOID ptr)
	{
		m_pDetector->RemovePtr(ptr);
	}

	void CDatabaseContextImpl::StartDetector()
	{
		m_pDetector->StartDetector();
	}
};
