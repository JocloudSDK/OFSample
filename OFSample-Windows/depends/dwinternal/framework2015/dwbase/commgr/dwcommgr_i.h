#pragma once

#include "dwglobal.h"
#include "dwbase/dwcomex.h"

#ifndef	DW_COMPONENT_ENV	//����������ᶨ�������
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
	���ص�ǰyy�Ѿ����ص����������,<id, version>
	*/
	virtual void allRunningComponents(std::vector<std::pair<int,int>> &coms) = 0;

	/*
	��Ҫ�ں��ʵ�ʱ����ComMgrע�����ֻ��YY�ںˡ�����ҵ��������ſ��Ե���
	*/
	virtual bool registerService(const char *name, IUnkPtr p) = 0;
	virtual bool registerService(const char *name) = 0;
	virtual bool unregisterService(const char *name) = 0;

	/*
	YY�ںˡ�����ҵ��������ں���ʱ���������ر�checkpoint
	*/
	virtual void activeCheckPoint(int checkpoint) = 0;
	virtual void deactiveCheckPoint(int checkpoint) = 0;
	virtual void addCheckPoint(int checkpoint) = 0;

	/*
	download: û�м���ʱ���費��Ҫ����.
	ret: -1����ʧ��
		0���سɹ�
		1��������
	*/
	virtual int loadComponent(const char* name, bool download) = 0;
	virtual bool unloadComponent(const char* name, bool free) = 0;

	/*
	0  : �Ѽ���
	1  : ������
	-1 : û����
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