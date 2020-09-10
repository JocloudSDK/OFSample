#pragma once
#include "dwbase/dwcomex.h"

/**
  * @brief 外部数据服务通知。serviceLoad和serviceAboutToUnload由数据提供者调用。使用者链接两个信号获得通知。
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
