#include "stable.h"
#include "IData.h"
#include "DatabaseManager.h"
#include "DatabaseImpl.h"
#include "EventDispatch.h"

namespace Data
{
	IDatabasePtr getGlobalDatabase()
	{
		static CDatabaseImpl *s_pInstance = NULL; 
		if (!s_pInstance)
		{
#ifdef _DEBUG
			int nOldFlag = _CrtSetDbgFlag(0);
#endif
			s_pInstance = CDatabaseImpl::createInstance(0);
#ifdef _DEBUG		
			_CrtSetDbgFlag(nOldFlag);
#endif
		}

		return s_pInstance;
	}

	IDatabasePtr createDatabase(DWORD dwId)
	{
		return CDatabaseManager::GetInstance()->createDatabase(dwId);
	}

	IDatabasePtr queryDatabase(DWORD dwId)
	{
		return CDatabaseManager::GetInstance()->queryDatabase(dwId);
	}

	void bindAnotherDatabaseId(DWORD dwId, DWORD dwAnotherId)
	{
		CDatabaseManager::GetInstance()->bindAnotherDatabaseId(dwId, dwAnotherId);
	}

	void dropDatabase(DWORD dwId)
	{
		CDatabaseManager::GetInstance()->dropDatabase(dwId);
	}

	xsignals::signal<void (DWORD)>&	sigDatabaseCreated()
	{
		return CDatabaseManager::GetInstance()->m_sigDatabaseCreated;
	}
};
