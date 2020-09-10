#pragma once

#include <map>
#include <vector>
#include "IDatabase.h"
#include "CriticalSection.h"
#include "DatabaseContextImpl.h"

namespace Data
{
	class CDatabaseImpl : public IDatabase
	{
	public:
		static CDatabaseImpl * createInstance(DWORD dwId);

		//IUnk
		virtual void				addRef();
		virtual void				release();
		virtual	IWeakRef *			weakPtr();
		virtual bool queryInterface(REFDWUUID iid, void **ppv);

		//IDatabase
		virtual ITablePtr			getTable(const NAME_ID& name);
		virtual ITableEditPtr		getTableEdit(const NAME_ID& name);

		virtual IFieldSetPtr		getFieldSet(const NAME_ID& name);

		virtual void				deleteTable(const NAME_ID& name);
		virtual void				deleteFieldSet(const NAME_ID& name);

		virtual ITableEditPtr		allocTable(const NAME_ID & name);
		
		virtual DWORD				loadDataTemplate(LPCTSTR lpszFilePath);
		virtual DWORD				loadDataTemplateFromRes(LPCTSTR lpszResouceName);
		virtual void				unloadDataTemplate(DWORD dwFileFlag);

		virtual ITablePtr			loadTable(const NAME_ID& name, LPCTSTR lpszTableDataPath);
		virtual BOOL				saveTable(const NAME_ID& name, LPCTSTR lpszTableDataPath);

		virtual void				getTables(NAME_IDS &tables);
		virtual void				getFieldSets(NAME_IDS &fieldSets);

		CDatabaseContextImpl*		getDatabaseContext() { return m_pContext; }

		virtual xsignals::signal<void (DWORD)> & sigLoadDataTemplated() {return m_sigLoadDataTemplated;}

	private:
		CDatabaseImpl(DWORD dwId);
		~CDatabaseImpl(void);
		ITableEditPtr				queryTableEdit(const NAME_ID& name);


		DWORD					m_dwRef;
		CCriticalLock			m_csLock;

		CDatabaseContextImpl *	m_pContext; //保持了引用计数
		struct TABLE_CACHE
		{
			NAME_ID oName;
			ITableEdit* pTable; //不保持引用计数
		};
		TABLE_CACHE				m_tblCache;

		typedef std::map<NAME_ID, ITableEditPtr> MapTableEdit;
		MapTableEdit m_mapTableEdit;

		typedef std::map<NAME_ID, IFieldSetPtr> MapFieldSetEdit;
		MapFieldSetEdit m_mapFieldSet;

		xsignals::signal<void (DWORD)> m_sigLoadDataTemplated;
	};
};
