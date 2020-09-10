#pragma once

#include "yycomponent/yycommon/yycomstoreinterface_i.h"

DWDEFINE_INTERFACE(IYYComStoreInner) : public IYYComStore
{
    /** �滻app service������com�ӿڼ���ǰ����
    * name  : ���֣���: com.yy.weideng
    * path  : dll���֣���·����
    * entry : service��ڣ��ձ�ʾĬ�ϣ�createComInstance��
    */
    virtual void replaceAppService(const QString& name, const QString& path, const QString& entry) = 0;

    /** �滻component info������com�ӿڼ���ǰ����
    * name      : ���֣���: com.yy.weideng
    * entrydll  : dll����
    * url       : ����������·��������dll����
    * entry     : dll��ڣ��ձ�ʾĬ�ϣ�createComInstance��
    */    
    virtual void replaceComponent(const QString& name, const QString& entrydll, const QString& url, const QString& entry) = 0;

    virtual void activeComponentCheckPoint(const char* name, int checkpoint) = 0;

    virtual std::wstring componentDllPath(const char* name) = 0;
    virtual void setIsolatedProxyDll(const std::wstring& dllpath, const char* createfun) = 0;
    virtual void readComponentInfoCache() = 0;
    virtual void activeIsolatedCheckPoint(int checkpoint) = 0;

	/** 
	* ��Ƶ���������̼�����ͨ��������������expand�����ø�ֵ"comload_isolate",1ΪƵ������Ҳ���أ�0Ϊ������
	*/
	virtual void activeIsolateComponentCheckPoint() = 0;
};