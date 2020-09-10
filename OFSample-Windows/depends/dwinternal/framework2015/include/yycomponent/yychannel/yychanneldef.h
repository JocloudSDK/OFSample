#pragma once


namespace
{
	/** 每个应用拥有的属性值 */
	const char* KAttributeWindowKey     = "addon.window"; // default = 1
	const char* KAttributeFullButtonKey = "addon.fullbutton"; // default = 0
	const char* KAttributeErrorKey      = "addon.error";	
	const char* KAttributeImageFileKey  = "addon.imagefile";	
	const char* KAttributeShowStateWidgetKey = "addon.statewidget"; //true|false,状态是否显示出来
	const char* KAttributeBKGColorStateWidgetKey = "addon.bkgcolor.statewidget"; // 状态区域颜色值
	const char* KAttributeVersion                = "addon.version";
	const char* KAttributeForcedTemplateLoadingKey = "addon.forcedtemplateloading"; // true|false
	const char* KAttributeWindowStyle = "addon.windowstyle"; // 窗口模式. 0:默认模式, 2:只有模版模式
	const char* KAttributeRunAddon	= "addon.runable"; //true|false

	const char*  KTemplateAttribte_changed = "template.changed";
	/** 应用类型　*/
	enum eAppType
	{
		AppType_None = 0,
		AppType_JS,
		AppType_Exe,
		AppType_Dll,
		AppType_Flash,
		AppType_QtDll,
		AppType_Web,
		AppType_SelfDefine
	};

	enum eAppEvent
	{
		Event_unk = 0,
		Event_createReal = 1,///< 请求创建真实窗体		
		Event_create = 2,
		Event_show = 3, ///< 请求显示
		Event_hide = 4,
		Event_fullscreen = 5,		
		Event_exitfull = 6,
		Event_move = 7,
		Event_close = 8,
		Event_createdFail = 9,///< 创建失败
		Event_createdSucceed = 10,///< 创建成功
	};

	// 与进程交互使用的协议
	enum eAppProtocolVersion
	{
		APVersion_unk = 0,
		APVersion_old = 1,
		APVersion_new_5 = 2, ///< 5.0
		APVersion_template = 3,
	};

	enum eAppSource
	{
		APSource_unk = 0,
		APSource_old = 1,
		APSource_bulletin = 2,
	};

	/** openplatform interface version */
	enum ePlatformVersion
	{
		APlatform_yy_version = 1,
		APlatform_dw_version = 2,
	};
};