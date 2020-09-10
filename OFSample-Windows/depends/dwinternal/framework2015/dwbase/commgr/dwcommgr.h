#pragma once
#include "dwbase/dwcommgr_i.h"
#include "dwbase/dwcominterface_i.h"
#include "dwbase/dwcomstore_i.h"
#include "dwutility/dwlocker.h"

class DWComMgr: public IDWComMgr
{
public:
	DWComMgr();

private:	//IDWComMgr
	virtual IDWComRootPtr getComRoot(const char *name);
	virtual IUnkPtr getService(const std::string &name);
	virtual IUnkPtr getService(const std::wstring &name);
	virtual void allRunningComponents(std::vector< std::pair<int,int> > &coms);
	virtual bool registerService(const char *name, IUnkPtr p);
	virtual bool registerService(const char *name);
	virtual bool unregisterService(const char *name);
	virtual void activeCheckPoint(int checkpoint);
	virtual void deactiveCheckPoint(int checkpoint);
	virtual int loadComponent(const char* name, bool download);
	virtual bool unloadComponent(const char* name, bool free);
	virtual int checkComponentState( const char* name );
	virtual std::string componentNameByAlias( const char* alias );
	virtual void addCheckPoint(int checkpoint);
private:
	void getComStore();

private:
	std::map<std::string, IDWComRootPtr> m_comRootMap;	//动态组件名字到root的映射
	std::map<std::string, IUnkPtr> m_objMap;	//内置功能性组件名字到功能接口的映射
	IDWComStorePtr m_comStore;
	DwUtility::DwCSLock m_cs;
	DWORD m_ownerThreadId;
};

IDWComMgr *comMgr();