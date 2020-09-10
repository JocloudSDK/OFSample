#pragma once

#include "yycomponent/yycommon/yycomstoreinterface_i.h"

DWDEFINE_INTERFACE(IYYComStoreInner) : public IYYComStore
{
    /** 替换app service，必须com接口加载前调用
    * name  : 名字，如: com.yy.weideng
    * path  : dll名字（或路径）
    * entry : service入口，空表示默认（createComInstance）
    */
    virtual void replaceAppService(const QString& name, const QString& path, const QString& entry) = 0;

    /** 替换component info，必须com接口加载前调用
    * name      : 名字，如: com.yy.weideng
    * entrydll  : dll名字
    * url       : （服务器）路径，或者dll名字
    * entry     : dll入口，空表示默认（createComInstance）
    */    
    virtual void replaceComponent(const QString& name, const QString& entrydll, const QString& url, const QString& entry) = 0;

    virtual void activeComponentCheckPoint(const char* name, int checkpoint) = 0;

    virtual std::wstring componentDllPath(const char* name) = 0;
    virtual void setIsolatedProxyDll(const std::wstring& dllpath, const char* createfun) = 0;
    virtual void readComponentInfoCache() = 0;
    virtual void activeIsolatedCheckPoint(int checkpoint) = 0;

	/** 
	* 在频道独立进程加载普通组件，在组件属性expand里设置该值"comload_isolate",1为频道进程也加载，0为不加载
	*/
	virtual void activeIsolateComponentCheckPoint() = 0;
};