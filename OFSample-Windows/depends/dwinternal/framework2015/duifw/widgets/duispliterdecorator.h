
/****************************************************************************
Author: AaronWang
Email : wangqiang@chinaduo.com
Mobile: 15915985665
Remark:
****************************************************************************/
#pragma once

#include "duispliterwidget.h"

class DuiSpliterMediatorImpl;

class DW_DUIFW_EXPORT DuiSpliterDecorator
{
public:
	virtual ~DuiSpliterDecorator();
	virtual bool getPreferredSize(const QString& objectName, int idx, int& w, int& h);
	virtual bool doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect);
};

class DW_DUIFW_EXPORT DuiSpliterMediator : public ISpliterPolicy
{
private: // ISpliterPolicy
	virtual void getPreferredSize(const QString& objectName, int idx, int& w, int& h);
	virtual bool doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect);

public: // DuiSpliterMediator
	static DuiSpliterMediator* inst();
	static void release();
	void addDecorator(DuiSpliterDecorator *decorator);
	void removeDecorator(DuiSpliterDecorator *decorator);
	void clearAll();

private:
	DuiSpliterMediator();
	~DuiSpliterMediator();

private:
	DuiSpliterMediatorImpl* m_impl;
};

class DW_DUIFW_EXPORT DuiSpliterNoLine : public DuiSpliterDecorator
{
public:
	virtual bool doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect);
};