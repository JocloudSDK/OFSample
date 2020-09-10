#pragma once

class IAsyncTaskBase
{
public:
	virtual ~IAsyncTaskBase(){};
	virtual void run() = NULL;
	virtual void finished(){};
};