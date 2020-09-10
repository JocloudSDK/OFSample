#pragma once
#include "dwcomex.h"

struct ComponentInfo;
DWDEFINE_INTERFACE(IDWComStore): public IUnk
{
    enum ComponentState
    {
        Invalid = -1,
        Loaded = 0,
        NotExistsDownloading = 1,
        ExistsNotLoaded = 2,
        NotExistsNotDownloading = 3
    };
	
	virtual std::wstring componentRootPath(const char *name) = 0;
	virtual int findComponentId(const char *name) = 0;	
	virtual void activeCheckPoint(int checkpoint) = 0;
	virtual void deactiveCheckPoint(int checkpoint) = 0;
	virtual void onComponentRegiste(const char *name, IUnkPtr unk) = 0;
	//这个接口被废弃
	virtual void onComponentCreated(const char *name, const std::map<std::string, std::string> &info) = 0;
	virtual int loadComponent(const char* name, bool download) = 0;
	virtual bool unloadComponent(const char *name, bool free) = 0;
	virtual std::string componentNameByAlias( const char* alias ) = 0;
    virtual IDWComStore::ComponentState checkComponentState( const char* name ) = 0;
	virtual void allRunningComponents( std::vector< std::pair<int,int> > &coms ) = 0;
	virtual bool createInstance(const char *name, IUnk* outUnk, IUnkPtr &pv) = 0;
	virtual std::wstring componentDataPath(const char *name) = 0;
	virtual void addCheckPoint(int checkpoint) = 0;
	virtual void addComponent(const ComponentInfo &info) = 0;
	virtual void addComponent(const std::string &info) = 0;
	virtual void removeComponent(int id) = 0;
	virtual void removeComponentVersion(int id, int version) = 0;
	virtual bool componentInfoFromJson(const std::string &json, ComponentInfo &info) = 0;
	virtual int componentVersion(int id) = 0;
	virtual bool findComponentId(const char* name, int& id) = 0;
    virtual std::string getComponentCacheOpaque(int id, UINT32 channelId) = 0;
    virtual bool getCacheComponentInfo(int id, UINT32 channelId, ComponentInfo& info) = 0;
    virtual void saveComponentCache(int id, UINT32 channelId, const std::string &json, const std::string &opaque) = 0;
	virtual std::wstring componentDescription(const char *name) const = 0;
	virtual bool isComponentExist(const char *name) const = 0;
	virtual int	componentWorkMode(const char *name) const = 0;
	virtual bool queryComponentsByTailLightType(int type, std::vector<std::string> &ret) const = 0;
	virtual const std::set<unsigned int>& getLocalTemplateIdList(void) const = 0;
	virtual void startPreloadComonentInfo(std::map<UINT32,ComponentInfo>& preloadTemplateList) = 0;
};

