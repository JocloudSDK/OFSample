#pragma once
#include <dwbase/dwcomex.h>

DWDEFINE_INTERFACE(IYYProcessMonitor) : public IDWQComponent
{
   virtual bool isProcessExists(const QString& process_name) const = 0;
   virtual bool isProcessesExists(const QList<QString>& process_names) const = 0;
   virtual QList<QString> runningSubsetOfWhiteList() const = 0;
};
