#pragma once

#include <map>
#include <vector>
#include "CriticalSection.h"
#include "TableParser.h"
#include "TablePersistent.h"
#include "DatabaseLeakDetector.h"
#include "IDatabase.h"

namespace Data
{
	class CDatabaseLeakDetector;

	class CDatabaseContextImpl : public IUnk
	{
	public:
		static CDatabaseContextImpl * createInstance(DWORD dwId);

		//IUnk
		virtual void			addRef();
		virtual void			release();
		virtual	IWeakRef *		weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		DWORD					loadDataTemplate(LPCTSTR lpszFilePath);
		DWORD					loadDataTemplate(const char *pBuffer, DWORD dwSize);
		void					unloadDataTemplate(DWORD dwFileFlag, std::vector<NAME_ID> &vecName);
		BOOL					queryTableStruct(const NAME_ID& tableName, TABLE_STRUCT* pTableStruct) const;
		void					queryViewStruct(const NAME_ID &tableName, std::vector<VIEW_STRUCT> &vecViewStruct) const;
		BOOL					queryFieldSetStruct(const NAME_ID& fieldSetName, FIELDSET_STRUCT* pStruct) const;

		BOOL					loadTable(ITableEditPtr spTable, LPCTSTR lpszTableDataPath);
		BOOL					saveTable(ITablePtr spTable, LPCTSTR lpszTableDataPath);

		DWORD					getDatabaseId() const { return m_dwId; }

		//Leak Detector
		void					InsertDetectorPtr(PVOID ptr, DETECTOR_E_TYPE eObjType);
		void					RemoveDetectorPtr(PVOID ptr);
		void					StartDetector();
	private:
		CDatabaseContextImpl(DWORD dwId);
		~CDatabaseContextImpl(void);

		DWORD					m_dwRef;
		DWORD					m_dwId;
		mutable CCriticalLock	m_csLock;
		CTableParser			m_xmlParser;
		CTablePersistent		m_tablePersistent;
		CDatabaseLeakDetector*	m_pDetector;
	};
};
