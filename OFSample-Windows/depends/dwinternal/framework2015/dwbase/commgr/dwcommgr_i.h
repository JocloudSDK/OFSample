#pragma once

#include "dwglobal.h"
#include "dwbase/dwcomex.h"

#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏
enum DW_CHECK_POINT
{
	KYYLifeCheckPoint = 0,
	KLoginCheckPoint,
	KChannelCheckPoint,
	KIMCheckPoint,
	KGroupCheckPoint,
	KSettingCheckPoint,
	KChannelExCheckPoint,
	KBizCheckPoint,
};

struct IDWComMgr
{
	virtual IDWComRootPtr getComRoot(const char *name) = 0;
	virtual IUnkPtr getService(const std::string &name) = 0;
	virtual IUnkPtr getService(const std::wstring &name) = 0;

	/*
	返回当前yy已经加载的所有组件的,<id, version>
	*/
	virtual void allRunningComponents(std::vector<std::pair<int,int>> &coms) = 0;

	/*
	需要在合适的时机向ComMgr注册服务，只有YY内核、内置业务性组件才可以调用
	*/
	virtual bool registerService(const char *name, IUnkPtr p) = 0;
	virtual bool registerService(const char *name) = 0;
	virtual bool unregisterService(const char *name) = 0;

	/*
	YY内核、内置业务性组件在合适时机启动、关闭checkpoint
	*/
	virtual void activeCheckPoint(int checkpoint) = 0;
	virtual void deactiveCheckPoint(int checkpoint) = 0;
	virtual void addCheckPoint(int checkpoint) = 0;

	/*
	download: 没有加载时，需不需要下载.
	ret: -1加载失败
		0加载成功
		1正在下载
	*/
	virtual int loadComponent(const char* name, bool download) = 0;
	virtual bool unloadComponent(const char* name, bool free) = 0;

	/*
	0  : 已加载
	1  : 在下载
	-1 : 没加载
	*/
	virtual int checkComponentState( const char* name ) = 0;

	virtual std::string componentNameByAlias( const char* alias ) = 0;

	template<typename Interface>
	bool registerService(const char *name, comptr<Interface> inter)
	{
		if (!inter)
		{
			DW_ASSERT(false);
			return false;
		}

		IUnkPtr unk;
		inter->queryInterface(unk);

		return registerService(name, unk);
	}

	template<typename Interface>
	comptr<Interface> getServiceInterface(const char *name)
	{
		IUnkPtr unk = getService(name);
		if (!unk)
		{
			return NULL;
		}

		comptr<Interface> inter;
		unk->queryInterface(inter);

		return inter;
	}
	template<typename Interface>
	comptr<Interface> getServiceInterface(const std::string& name)
	{
		IUnkPtr unk = getService(name);
		if (!unk)
		{
			return NULL;
		}

		comptr<Interface> inter;
		unk->queryInterface(inter);

		return inter;
	}
};

DWBASE_EXPORTS IDWComMgr *comMgr();
#endif	//DW_COMPONENT_ENV