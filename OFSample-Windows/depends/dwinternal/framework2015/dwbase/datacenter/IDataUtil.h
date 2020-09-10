#pragma once

#include "ITable.h"
#include "ITableEdit.h"

namespace Data
{	
	struct CEditLock
	{
		CEditLock(ITableEdit *pTable) 
		{ 
			pTable->beginEdit(); 
			pTable->addRef();
			m_pTable = pTable;
		}

		~CEditLock()
		{
			m_pTable->endEdit(); 
			m_pTable->release(); 
			m_pTable = NULL; 
		}
		
		ITableEdit * m_pTable;
	};

	extern DWBASE_EXPORTS	IDatabasePtr getGlobalDatabase();
	
	inline ITablePtr		getTable(const NAME_ID& name)			{ return getGlobalDatabase()->getTable(name); }
	inline ITableEditPtr	getTableEdit(const NAME_ID& name)		{ return getGlobalDatabase()->getTableEdit(name); }
	inline void				deleteTable(const NAME_ID& name)		{ return getGlobalDatabase()->deleteTable(name); }
	inline ITableEditPtr	allocTable(const NAME_ID & name)		{ return getGlobalDatabase()->allocTable(name); }
	
	inline IFieldSetPtr		getFieldSet(const NAME_ID& name)		{ return getGlobalDatabase()->getFieldSet(name); }
	inline void				deleteFieldSet(const NAME_ID& name)		{ return getGlobalDatabase()->deleteFieldSet(name); }
	
	inline DWORD			loadDataTemplate(LPCTSTR lpszFilePath)	{ return getGlobalDatabase()->loadDataTemplate(lpszFilePath); }
	inline DWORD			loadDataTemplateFromRes(LPCTSTR lpszResouceName) { return getGlobalDatabase()->loadDataTemplateFromRes(lpszResouceName); }
	inline void				unloadDataTemplate(DWORD dwFileFlag)	{ return getGlobalDatabase()->unloadDataTemplate(dwFileFlag); }
};
