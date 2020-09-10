#pragma once

DWDEFINE_INTERFACE(IYYAppView) : public IUnk
{
	enum Position
	{
		MainFrame_AppTab,     //������Ӧ��TAB��
		MainFrame_AppBox,	  //������Ӧ�ù�����
	};

	virtual bool isComponentVisible(qint32 appId, Position pos) = 0;
	virtual void setComponentVisible(qint32 appId, Position pos, bool visible) = 0;
};
