#include "stable.h"
#include "dwbase/dwcomcreator.h"
#include "dwbase/dwcominterface_i.h"
#include "dwbase/dwcomstore_i.h"
#include "dwbase/yycomclsid.h"
#include "dwbase/log.h"
#include "dwutility/ustring.h"

#include "dwcommgr.h"
#include "dwcomloader.h"
#include "dwcomroot.h"

DWComMgr::DWComMgr()
:m_ownerThreadId(GetCurrentThreadId())
{
}

void DWComMgr::getComStore()
{
	DW_AUTO_LOCKER(&m_cs);
	if (!m_comStore)
	{
		IUnkPtr unk = getService(KSERVICEYYComStore);
        if (unk)
        {
            unk->queryInterface(m_comStore);
        }
	}
}

IDWComRootPtr DWComMgr::getComRoot(const char *name)
{
	bool needCreate = false;
	IDWComRootPtr rootRet;
	{
		DW_AUTO_LOCKER(&m_cs);

		std::map<std::string, IDWComRootPtr>::iterator It = m_comRootMap.find(name);
		if (It == m_comRootMap.end())
		{
			needCreate = true;
		}
		else
		{
			return (*It).second;
		}
	}

	IDWComRootPtr root;
	coCreateComInstance(KSERVICEComRoot, root);
	DW_ASSERT(root.get() != NULL);
	do 
	{
		if (!root)
		{
			break;
		}

		IDWComponentLevelPtr level;
		root->queryInterface(level);
		if (!level)
		{
			break;
		}

		IDWComStorePtr store = comLoader()->comStore();
		int id = -1;
		if (!store->findComponentId(name, id))
		{
			break;
		}

		level->setComponentId(id);
		level->setComponentName(name);
		{
			DW_AUTO_LOCKER(&m_cs);
			m_comRootMap.insert(std::make_pair(name, root));
		}
		rootRet = root;

	} while (false);
	return rootRet;
}

IUnkPtr DWComMgr::getService(const std::string &name)
{
	DW_AUTO_LOCKER(&m_cs);
	std::map<std::string, IUnkPtr>::const_iterator It = m_objMap.find(name);
	if (It == m_objMap.end())
	{
		LogFinal(LOG::KComponent)<< L"Failed to getComponent, not found: " << name;
		return NULL;
	}

	return (*It).second;
};

IUnkPtr DWComMgr::getService(const std::wstring &name)
{
	std::string n = DwUtility::text::toNarrowString(name.c_str(), name.size());
	return getService(n);
};

void DWComMgr::allRunningComponents( std::vector< std::pair<int,int> > &coms )
{
	getComStore();
	if ( m_comStore )
	{
		return m_comStore->allRunningComponents( coms );
	}
}

bool DWComMgr::registerService(const char *name)
{
	IUnkPtr unk;
	coCreateComInstance(name, unk);

	return registerService(name, unk);
}

bool DWComMgr::registerService(const char *name, IUnkPtr p)
{
	{
		DW_AUTO_LOCKER(&m_cs);
		if (!p)
		{
			DW_ASSERT(false);
			LogFinal(LOG::KComponent)<< L"Failed to registerComponent, component is NULL: " << name;
			return false;
		}

		std::map<std::string, IUnkPtr>::iterator It = m_objMap.find(name);
		if (It != m_objMap.end())
		{
			LogFinal(LOG::KComponent)<< L"Failed to registerComponent, already registered: " << name;
			return false;
		}

		m_objMap.insert(std::make_pair(name, p));
	}
	getComStore();
	if (m_comStore)
	{
		m_comStore->onComponentRegiste(name, p);
	}
	return true;
};

bool DWComMgr::unregisterService(const char *name)
{
	DW_AUTO_LOCKER(&m_cs);
	std::map<std::string, IUnkPtr>::iterator It = m_objMap.find(name);
	if (It == m_objMap.end())
	{
		LogFinal(LOG::KComponent)<< L"Failed to unregisterComponent, not found: " << name;
		return false;
	}

	/*
	需要同时通知root们删除
	*/
	IDWComponentLevelPtr level;
	if (It->second && It->second->queryInterface(level))
	{
		std::map<std::string, IDWComRootPtr>::iterator innerIt = m_comRootMap.begin();
		for (; innerIt != m_comRootMap.end(); ++innerIt)
		{
			IRootSinkPtr sink;
			innerIt->second->queryInterface(sink);
			if (sink)
			{
				sink->onUnRegister(name);
			}
		}
	}
	m_objMap.erase(It);

	return true;
};

void DWComMgr::activeCheckPoint(int checkpoint)
{
	if (m_ownerThreadId != GetCurrentThreadId())
	{
		DW_ASSERT_X(false, __FUNCTION__, "you should not call in different thread.");
		return ;
	}
	getComStore();
	if (m_comStore)
	{
		m_comStore->activeCheckPoint(checkpoint);
	}
};

void DWComMgr::deactiveCheckPoint(int checkpoint)
{
	if (m_ownerThreadId != GetCurrentThreadId())
	{
		DW_ASSERT_X(false, __FUNCTION__, "you should not call in different thread.");
		return ;
	}
	getComStore();
	if (m_comStore)
	{
		m_comStore->deactiveCheckPoint(checkpoint);
	}
};

void DWComMgr::addCheckPoint( int checkpoint )
{
	if (m_ownerThreadId != GetCurrentThreadId())
	{
		DW_ASSERT_X(false, __FUNCTION__, "you should not call in different thread.");
		return ;
	}
	getComStore();
	if (m_comStore)
	{
		m_comStore->addCheckPoint(checkpoint);
	}
}

int DWComMgr::loadComponent(const char* name, bool download )
{
	getComStore();
	if (m_comStore)
	{
		return m_comStore->loadComponent(name, download);
	}

	return -1;
}

bool DWComMgr::unloadComponent(const char* name, bool free)
{
	getComStore();
	if (m_comStore)
	{
		return m_comStore->unloadComponent(name, free);
	}

	return false;
}

int DWComMgr::checkComponentState( const char* name )
{
	if (m_ownerThreadId != GetCurrentThreadId())
	{
		DW_ASSERT_X(false, __FUNCTION__, "you should not call in different thread.");
		return IDWComStore::Invalid;
	}
	getComStore();
	if ( m_comStore )
	{
		return m_comStore->checkComponentState( name );
	}

	return IDWComStore::Invalid;
}

std::string DWComMgr::componentNameByAlias( const char* alias )
{
	if (m_ownerThreadId != GetCurrentThreadId())
	{
		DW_ASSERT_X(false, __FUNCTION__, "you should not call in different thread.");
		return std::string();
	}
	getComStore();
	if ( m_comStore )
	{
		return m_comStore->componentNameByAlias( alias );
	}

	return std::string();
}

DWComMgr *g_mgr = NULL;
IDWComMgr *comMgr()
{
	if (g_mgr == NULL)
	{
		g_mgr = new DWComMgr;
	}

	return g_mgr;
};
