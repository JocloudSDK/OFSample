#include "stable.h"
#include "dwcomroot.h"
#include "dwcomloader.h"
#include "dwbase/dwcomcreator.h"
#include "dwbase/dwcominterface_i.h"
#include "dwbase/dwi18n.h"
#include "dwcommgr.h"
#include "dwutility/environmentpath.h"
#include "componentsdkversion.h"

DWCOM_IMPLEMENT();

HRESULT DWComRoot::coCreateComService(const char *name, IUnk* pOutUnk, comptr<IUnk>& pv)
{
	return comLoader()->coCreateComInstance(name, pOutUnk, pv);
};

IUnkPtr DWComRoot::getService(const char *name) const
{
	DW_AUTO_LOCKER(const_cast<DwUtility::DwCSLock*>(&m_cs));
	std::map<std::string, IUnkPtr>::const_iterator It = m_objMap.find(name);
	if (It != m_objMap.end())
	{
		return (*It).second;
	}

	IUnkPtr unk = comMgr()->getService(name);
	if (unk)
	{
		/*query另外一个接口，如果存在，所以此接口是每个component需要一份的*/
		IDWComponentLevelPtr com;
		if (unk->queryInterface(com))
		{
			IDWComponentLevelPtr newLevel = com->clone();
			newLevel->setComponentId(componentId());
			newLevel->setComponentName(componentName());
			
			IUnkPtr newUnk;
			newLevel->queryInterface(newUnk);
			DW_ASSERT(newUnk != NULL);
			if (newUnk)
			{
				const_cast<DWComRoot*>(this)->m_objMap.insert(std::make_pair(name, newUnk));
			}
			return newUnk;
		}
	}
	return unk;
};

std::wstring DWComRoot::componentRootPath() const
{
	IDWComStorePtr store = getServiceInterface<IDWComStore>(KSERVICEYYComStore);
	if (!store)
	{
		return L"";
	}
    
	return store->componentRootPath(componentName().c_str());
}

std::wstring DWComRoot::yypath() const
{
	return DwUtility::app::appRootPath();
}

std::wstring DWComRoot::componentDataPath() const
{
	IDWComStorePtr store = getServiceInterface<IDWComStore>(KSERVICEYYComStore);
	if (!store)
	{
		return L"";
	}

	return store->componentDataPath(componentName().c_str());
}

int DWComRoot::version() const
{
	IDWComStorePtr store = getServiceInterface<IDWComStore>(KSERVICEYYComStore);
	if (!store)
	{
		return -1;
	}

	return store->componentVersion(componentId());
}

void DWComRoot::set(const std::string& key, const std::string& value)
{
    m_data[key] = value;
}

std::string DWComRoot::value(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = m_data.find(key);
    if (it != m_data.end())
        return it->second;

    return std::string();
}


unsigned int DWComRoot::componentSdkVersion() const
{
	return ::componentSdkVersion();
}

std::wstring DWComRoot::componentSdkVersionString() const
{
	return ::componentSdkVersionString();
}

std::wstring DWComRoot::componentSdkVersionStringHex() const
{
	return ::componentSdkVersionStringHex();
}

unsigned int DWComRoot::majorVersion() const
{
	return ::majorVersion();
}

unsigned int DWComRoot::minorVersion() const
{
	return ::minorVersion();
}

unsigned int DWComRoot::patchVersion() const
{
	return ::patchVersion();
}

IDWComponentLevelPtr DWComRoot::clone()
{
	DW_ASSERT(false);
	return NULL ;
}

void DWComRoot::onUnRegister(const char *name)
{
	std::map<std::string, IUnkPtr>::iterator It = m_objMap.find(name);
	if (It != m_objMap.end())
	{
		m_objMap.erase(It);
	}
}

std::wstring DWComRoot::languageId() const
{
	wchar_t languageId[5];
	_itow_s(DwI18n::yyLangNameID(), languageId, 10);
	return languageId;
}

bool DWComRoot::registerService(const char *name, IUnkPtr p)
{
	return comMgr()->registerService(name, p);
};

bool DWComRoot::unregisterService(const char *name)
{
	return comMgr()->unregisterService(name);
};

bool DWComRoot::cmdLine(const std::wstring &key, std::wstring &value) const
{
	bool ret = true;
	std::map<std::wstring, std::wstring>::const_iterator It = m_cmdLine.find(key);
	if (It != m_cmdLine.end())
	{
		value = It->second;
	}
	else
	{
		ret = false;
		value.clear();
	}

	return ret;
};

bool DWComRoot::cmdLine(std::map<std::wstring, std::wstring> &out) const
{
	out = m_cmdLine;
	return true;
};

bool DWComRoot::addCmdLine(const std::wstring &key, const std::wstring &value)
{
	std::map<std::wstring, std::wstring>::const_iterator It = m_cmdLine.find(key);
	if (It != m_cmdLine.end())
	{
		return false;
	}

	m_cmdLine.insert(std::make_pair(key, value));
	return true;
};

bool DWComRoot::removeCmdLine(const std::wstring &key)
{
	std::map<std::wstring, std::wstring>::iterator It = m_cmdLine.find(key);
	if (It == m_cmdLine.end())
	{
		return false;
	}

	m_cmdLine.erase(It);
	return true;
};