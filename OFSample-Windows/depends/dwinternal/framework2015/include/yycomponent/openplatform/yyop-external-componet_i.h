#pragma once
#include "dwbase/dwcomex.h"

namespace NewComponent
{
	//��componentinfobase �е�type��Ӧ
	// 0:��ͨӦ�� 1:Ƶ��ģ�� 2:Ƶ��Ӧ��
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
	//��componentinfobase �е�ProgramType���屣��һ��
	enum ContainerType
	{
		SUB_WINDOW = 0, //����
		TAB_PAGE,       //tabҳ
		POP_WINDOW,     //������
		FULL_TEMPLATE,  //����ģʽ
		EMBED_TEMPLATE, //����ģʽ
		ONE_TEMPLATE,   //����ģʽ
		XML_TEMPLATE    //xml����
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

	// 0:��ͨӦ�� 1:Ƶ��ģ�� 2:Ƶ��Ӧ��
	virtual bool Unload(const std::wstring &componentName, int appType) = 0;
};