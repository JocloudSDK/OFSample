#pragma once


namespace
{
	/** ÿ��Ӧ��ӵ�е�����ֵ */
	const char* KAttributeWindowKey     = "addon.window"; // default = 1
	const char* KAttributeFullButtonKey = "addon.fullbutton"; // default = 0
	const char* KAttributeErrorKey      = "addon.error";	
	const char* KAttributeImageFileKey  = "addon.imagefile";	
	const char* KAttributeShowStateWidgetKey = "addon.statewidget"; //true|false,״̬�Ƿ���ʾ����
	const char* KAttributeBKGColorStateWidgetKey = "addon.bkgcolor.statewidget"; // ״̬������ɫֵ
	const char* KAttributeVersion                = "addon.version";
	const char* KAttributeForcedTemplateLoadingKey = "addon.forcedtemplateloading"; // true|false
	const char* KAttributeWindowStyle = "addon.windowstyle"; // ����ģʽ. 0:Ĭ��ģʽ, 2:ֻ��ģ��ģʽ
	const char* KAttributeRunAddon	= "addon.runable"; //true|false

	const char*  KTemplateAttribte_changed = "template.changed";
	/** Ӧ�����͡�*/
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
		Event_createReal = 1,///< ���󴴽���ʵ����		
		Event_create = 2,
		Event_show = 3, ///< ������ʾ
		Event_hide = 4,
		Event_fullscreen = 5,		
		Event_exitfull = 6,
		Event_move = 7,
		Event_close = 8,
		Event_createdFail = 9,///< ����ʧ��
		Event_createdSucceed = 10,///< �����ɹ�
	};

	// ����̽���ʹ�õ�Э��
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