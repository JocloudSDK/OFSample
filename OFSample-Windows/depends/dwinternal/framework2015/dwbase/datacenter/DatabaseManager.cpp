#include "stable.h"
#include "DatabaseManager.h"
#include "DatabaseImpl.h"
#include "../log/log.h"
#include "DatabaseLeakDetector.h"

namespace Data
{
	CDatabaseManager::CDatabaseManager()
	{
		::InitializeCriticalSection(&m_cri);
	}

	CDatabaseManager::~CDatabaseManager()
	{
		m_vecDatabase.clear();
		::DeleteCriticalSection(&m_cri);
	}

	CDatabaseManager*  CDatabaseManager::GetInstance()
	{
		static CDatabaseManager* s_pInstance = NULL;
		if (!s_pInstance)
		{
#ifdef _DEBUG
			int nOldFlag = _CrtSetDbgFlag(0);
#endif
			s_pInstance = new CDatabaseManager();
#ifdef _DEBUG		
			_CrtSetDbgFlag(nOldFlag);
#endif
		}

		return s_pInstance;
	}

	IDatabasePtr CDatabaseManager::createDatabase(DWORD dwId)
	{
		IDatabasePtr ptrRet;
		
		::EnterCriticalSection(&m_cri);

		if (dwId != 0)
		{
			if (findItem(dwId) == -1)
			{
				CDatabaseImpl *pObj = CDatabaseImpl::createInstance(dwId);

				DATABASE db;
				db.dwId_1 = dwId;
				db.dwId_2 = 0;
				db.pDatabase = pObj;
				m_vecDatabase.push_back(db);

				ptrRet.attach(pObj);

				LogFinal(LOG::KDataCenter) << L"createDatabase�ɹ�, id=" << dwId;
			}
			else
			{
				assert(! L"createDatabaseʱdatabase�Ѿ�����");
				LogFinal(LOG::KDataCenter) << L"createDatabaseʧ��, database�Ѿ�����, id=" << dwId;
			}
		}
		else
		{
			assert(! L"createDatabaseʱid����Ϊ0");
			LogFinal(LOG::KDataCenter) << L"createDatabaseʧ��, id����Ϊ0";
		}

		::LeaveCriticalSection(&m_cri);

		m_sigDatabaseCreated(dwId);
		return ptrRet;
	}

	IDatabasePtr CDatabaseManager::queryDatabase(DWORD dwId)
	{
		IDatabasePtr ptrRet;
	
		::EnterCriticalSection(&m_cri);

		DWORD iIndex = findItem(dwId);
		if (iIndex != -1)
			ptrRet = m_vecDatabase[iIndex].pDatabase;
		
		::LeaveCriticalSection(&m_cri);

		return ptrRet;
	}

	void CDatabaseManager::bindAnotherDatabaseId(DWORD dwId, DWORD dwAnotherId)
	{
		LogFinal(LOG::KDataCenter) << L"��ʼdatabase��, dwId=" << dwId << ", dwAnotherId=" << dwAnotherId;
		//assert(dwId != dwAnotherId && dwAnotherId != 0);

		if (dwId != dwAnotherId && dwAnotherId != 0)
		{
			::EnterCriticalSection(&m_cri);

			DWORD iIndex = findItem(dwId);
			if (iIndex != -1)
			{
				if (m_vecDatabase[iIndex].dwId_1 == dwId)
				{
					if (m_vecDatabase[iIndex].dwId_2 != 0)
					{
						assert(! L"database�����bind��");
						LogFinal(LOG::KDataCenter) << L"database�����bind��, id=" << dwId << ", dwAnotherId=" << dwAnotherId;
					}

					m_vecDatabase[iIndex].dwId_2 = dwAnotherId;
				}
				else
				{
					assert(m_vecDatabase[iIndex].dwId_2 == dwId);
					m_vecDatabase[iIndex].dwId_1 = dwAnotherId;
					
					assert(! L"database����η����л�bind");
					LogFinal(LOG::KDataCenter) << L"database����η����л�bind, id=" << dwId << ", dwAnotherId=" << dwAnotherId;
				}
			}
			else
			{
				assert(! L"bindʧ�ܣ�û���ҵ�database����");
				LogFinal(LOG::KDataCenter) << L"bindʧ�ܣ�û���ҵ�database����, id=" << dwId << ", dwAnotherId=" << dwAnotherId;
			}
			
			::LeaveCriticalSection(&m_cri);	
		}
	}

	void CDatabaseManager::dropDatabase(DWORD dwId)
	{
		::EnterCriticalSection(&m_cri);

		DWORD iIndex = findItem(dwId);
		if (iIndex != -1)
		{

			CDatabaseImpl *pObj = (CDatabaseImpl *)m_vecDatabase[iIndex].pDatabase.get();
			DBLEAK_DATABASE_DETECTOR(pObj->getDatabaseContext());

			m_vecDatabase.erase(m_vecDatabase.begin() + iIndex);
			LogFinal(LOG::KDataCenter) << L"dropDatabase�ɹ�, id=" << dwId;
		}
		else
		{
			assert(! L"dropDatabaseʱָ��database������");
			LogFinal(LOG::KDataCenter) << L"dropDatabaseʧ��, ָ��database������, id=" << dwId;
		}

		::LeaveCriticalSection(&m_cri);
	}

	DWORD CDatabaseManager::findItem(DWORD dwId)
	{
		if (dwId != 0)
		{
			for (UINT i = 0; i < m_vecDatabase.size(); i++)
			{
				if (m_vecDatabase[i].dwId_1 == dwId || m_vecDatabase[i].dwId_2 == dwId)
					return i;
			}
		}

		return -1;
	}
};
