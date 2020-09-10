#pragma once
#include "dwbase/dwcomex.h"

namespace NewComponent
{
	//与componentinfobase 中的type对应
	// 0:普通应用 1:频道模板 2:频道应用
	enum ComponentType 
	{
		NormalApp = 0,
		ChannelTemplate,
		ChannelApp
	};
	enum ProgramType
	{
		QtDllApp = 0,
		ExeApp,
		DllApp,
		WebApp,
		FlashApp,
		ChromeApp,
		InvalidApp
	};
	//与componentinfobase 中的ProgramType定义保持一致
	enum ContainerType
	{
		SUB_WINDOW = 0, //公告
		TAB_PAGE,       //tab页
		POP_WINDOW,     //弹出框
		FULL_TEMPLATE,  //两栏模式
		EMBED_TEMPLATE, //三栏模式
		ONE_TEMPLATE,   //单栏模式
		XML_TEMPLATE    //xml定制
	};

	struct ExternalComponentExtraInfo
	{
		ComponentType	component_type;
		ProgramType		programType;
		ContainerType	container_type;
		QString icon_text;
		QString entry_dll;
		int container_pos_x;
		int container_pos_y;
		int container_width;
		int container_height;
		bool standalone;
	};
}



DWDEFINE_INTERFACE(IOpExternalComponet) : public IUnk
{
	virtual bool Load(const std::wstring &componentName , const NewComponent::ExternalComponentExtraInfo *AppInfo) = 0;

	// 0:普通应用 1:频道模板 2:频道应用
	virtual bool Unload(const std::wstring &componentName, int appType) = 0;
};