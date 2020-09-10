#pragma once

#include "dwglobal.h"

class DW_DWUTILITY_EXPORT DwAsyncTaskBase
{
public:
	DwAsyncTaskBase(){};

	virtual void run() = NULL;
	virtual void finished(){};
	virtual void canceled(){};
	virtual void release()
	{
		delete this;
	}

protected:
	virtual ~DwAsyncTaskBase() {};
};

class DW_DWUTILITY_EXPORT DwThreadPool
{
private:
	DwThreadPool();
	~DwThreadPool();

public:
	static bool init();
	static void terminate();

	static bool addTask(DwAsyncTaskBase *task, int delay = 0);
	static bool addQueuedTask(const char *queueName, DwAsyncTaskBase *task);
};
