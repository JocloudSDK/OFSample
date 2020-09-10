#pragma once

DWDEFINE_INTERFACE(IYYAppView) : public IUnk
{
	enum Position
	{
		MainFrame_AppTab,     //主界面应用TAB栏
		MainFrame_AppBox,	  //主界面应用管理器
	};

	virtual bool isComponentVisible(qint32 appId, Position pos) = 0;
	virtual void setComponentVisible(qint32 appId, Position pos, bool visible) = 0;
};
