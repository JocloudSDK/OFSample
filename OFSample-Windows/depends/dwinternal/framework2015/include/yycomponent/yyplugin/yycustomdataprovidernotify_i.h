#pragma once
#include "dwbase/dwcomex.h"

/**
  * @brief �ⲿ���ݷ���֪ͨ��serviceLoad��serviceAboutToUnload�������ṩ�ߵ��á�ʹ�������������źŻ��֪ͨ��
*/
DWDEFINE_INTERFACE(IYYCustomDataProviderNotify): public IDWQComponent
{
	virtual void serviceLoad(const QString &serviceName) = 0;
	virtual void serviceAboutToUnload(const QString &serviceName) = 0;
	virtual bool isServiceLoaded(const QString &serviceName) = 0;
    virtual std::vector<std::wstring> loadedServices() const = 0;

//signals:
//	void serviceLoaded(const QString &serviceName);
//	void serviceAboutToUnloaded(const QString &serviceName);
};
