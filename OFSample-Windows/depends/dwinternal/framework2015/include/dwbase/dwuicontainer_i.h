#pragma once

#include "dwbase/dwcomex.h"
#include "yycomclsid.h"

/*
比如获取主面板名称下方VIP图标单击事件：
IDWUIContainerPtr buttonBarPtr = root->getServiceInterface<IDWUIContainer>(KSERVICEMainFrameButtonBar);//KSERVICEMainTabItemContainer，KSERVICEMainFrameTitleBar
IDWUIElementWeakPtr elemWeak = containerPtr->getItem("com.yy.vip");
if (elemWeak == NULL)
{
	return;
}
IDWUIElementPtr elemPtr = elemWeak.lock();
if (!elemPtr)
{
	return ;
}
elemPtr->connectSignal(SIGNAL(clicked()), this, SLOT(on_clicked()));
*/

class QSignalMapper;
class DuiWidget;
class QPoint;
DWDEFINE_INTERFACE(IDWUIElement): public IDWQComponent
{
	virtual bool setIcon(const QString &icon) = 0;
	virtual bool setText(const QString &text) = 0;
	virtual bool setNumber(int num) = 0;
	virtual bool setMaxShowNumber(int num) = 0;
	virtual bool setTooltip(const QString &text) = 0;
	//YY8.x改版后，白色皮肤要求显示不一样的图标.
	virtual bool setSkinIcon(const QString &icon) = 0;

/*
signal:
	void clicked();
*/
};

DWDEFINE_INTERFACE(IDWUIElementInner): public IDWUIElement
{
	virtual void setMapping(QSignalMapper &mapper, const QString &key) = 0;	//QSignalMapper::setMapping必须要QObject*
	virtual DuiWidget *widget() const = 0;
	virtual void setIndex(int index) = 0;
	virtual int index() const = 0;
    virtual bool setProperty(const QString& key,const QString& property) = 0;
    virtual QString property(const QString& key) const = 0;
};


DWDEFINE_INTERFACE(IDWUIContainer): public	IUnk
{
	virtual IDWUIElementWeakPtr addItem(const QString &uniqueName, int index) = 0;
	virtual IDWUIElementWeakPtr getItem(const QString &uniqueName) const = 0;
	virtual void removeItem(const QString &uniqueName) = 0;
	virtual QPoint getPos(const QString &uniqueName) const = 0;
};
