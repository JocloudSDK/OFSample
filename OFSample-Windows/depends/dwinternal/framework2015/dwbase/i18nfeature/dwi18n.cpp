#include "stable.h"

#include "dwutility/ufile.h"
#include "dwutility/dwencrypt.h"
#include "dwutility/dwjsondocument.h"
#include "dwutility/environmentpath.h"
#include "dwutility/ustring.h"
#include "dwutility/md5.h"
#include "dwutility/appdatapath.h"
#include "dwutility/utf8to.h"
#include "perflog.h"
#include "dwi18n.h"

class DwI18nData
{
public:
	DwI18nData()
	{
		m_resourcePath = _getAppResourcePath();
		loadLangCheckedConfig();
		loadFeatureConfig();
	}
	~DwI18nData()
	{

	}

	static DwI18nData& instance()
	{
		static DwI18nData dwI18nData;
		return dwI18nData;
	}
public:
	struct Feature
	{
		Feature()
		{
			defaultFontSize = 12;
			listFontSize = 12;
			enableSetProcessWorkingSet = true;
			defaultFontName = L"SimSun";
			listFontName = L"Microsoft Sans Serif";

			enableDuiSpy = false;
			enableAccessible = false;
			enableAcchelper = true;
			enablePerfStub = true;
			enableYYTicket = true;
			enableYYComStore = true;
			enableYYVIP = true;

			enableChannelFont = true;
			enableChannelAddon = true; 
			enableApplyCener = true;
			enableChannelHang = true;
			enableChannelGame = true;
			enableChannelPrivilege = true;
			enableChannelInvitation = true;
			enableChannelHomePage = true;
			enableChannelMagicSmiles = true;
			enableChannelReportType = true;
			enableApplyCornetChannelID = true;
			enableWebYY = true;

			enableImVipface = true;
			enableImTextAd  = true;
			enableFaceRoam = true;
			enablePersonalFont = true;
			enableGroupVipface = true;
			enablePanelController = true;
			enableSceurityCenter = true;
			enablesecurityCryptoguard = true;
			enableActivity = true;
			enableLocation = true;
			enableDuowanHome = true;
			enableDuowanYYBBS = true;
			enableCustomerService = true;
			enableHelpHome = true;
			enableLinkDComponentInfo = true;
			enableTemplateComponent = true;
			enableChannelComponent = true;
			loadInfoFromLocalXml = false;
			componentTestEnvironment = 0;
		}

		//system feature
		int defaultFontSize;
		int listFontSize;
		bool enableSetProcessWorkingSet;
		std::wstring defaultFontName;
		std::wstring listFontName;

		//general feature
		bool enableDuiSpy;
		bool enableAccessible;
		bool enableAcchelper;
		bool enablePerfStub;	//是否打桩，用于性能
		bool enableYYTicket;
		bool enableYYComStore;
		bool enableYYVIP;

		//channel feature
		bool enableChannelFont;
		bool enableChannelAddon;
		std::wstring channelAddonBlackList;
		std::wstring channelAddonWhiteList;
		bool enableApplyCener;
		bool enableChannelHang;
		bool enableChannelGame;
		bool enableChannelPrivilege;
		bool enableChannelInvitation;
		bool enableChannelHomePage;
		bool enableChannelMagicSmiles;
		bool enableChannelReportType;
		bool enableApplyCornetChannelID;
		bool enableWebYY;


		//im feature
		bool enableImVipface;
		bool enableImTextAd;
		bool enableFaceRoam;
		bool enablePersonalFont;

		//group feature
		bool enableGroupVipface;
		//mainframe feature
		std::wstring componentBlackList;
		std::wstring componentWhiteList;
		bool enablePanelController;
		bool enableSceurityCenter;
		bool enablesecurityCryptoguard;
		bool enableActivity;
		bool enableDuowanHome;
		bool enableDuowanYYBBS;
		bool enableCustomerService;
		bool enableHelpHome;

		//mysetting feature
		bool enableLocation;

		std::wstring componentTestPath;
		bool enableLinkDComponentInfo;
		bool enableTemplateComponent;
		bool enableChannelComponent;
		bool loadInfoFromLocalXml;
		int componentTestEnvironment;
		std::vector<unsigned int> componentDebugList;
	} featureConfig;

	struct URLPATH
	{
		//UrlPath
		std::wstring yyHomeUrl;
		std::wstring securityHomepageUrl;
		std::wstring securityChangePasswordUrl;
		std::wstring securityRegisterAccountUrl;
		std::wstring securityForgetPasswordUrl;
		std::wstring securityOtherChangePasswordUrl;
		std::wstring securityMobieTokenUrl;
		std::wstring securityAccountProtectUrl;
		std::wstring securitySystemAffectUrl;
		std::wstring bugFeedbackUrl;
		std::wstring guildWebUrl;
		std::wstring channelRanking;
		std::wstring channelType;
		std::wstring gamePrivilege;
		std::wstring facebookLogin;
		std::wstring flashSupport;
		std::wstring beanfunLogin;
		std::wstring beanfunLoginUdb;
		std::wstring helpCenter;
	}urlPath;

public:
	int yyLangNameID()
	{
		//due to not support the LocaleNameToLCID In
		//xp environment,and need to download  the  NLSDL.DLL 
		//,so following a simple treatment

		if(m_defaultLangName.compare(L"zh-CN") == 0)
		{
			return 2052;
		}
		else if (m_defaultLangName.compare(L"zh-TW") == 0)
		{
			return 1028;
		}
		else if (m_defaultLangName.compare(L"en-US") == 0)
		{
			return 1033;
		}
		else if (m_defaultLangName.compare(L"pt-BR") == 0)
		{
			return 1046;
		}
		else 
		{
			DW_ASSERT_X(false, __FUNCTION__, "no support langid");
			return 0;
		}
	}

	std::wstring yyLangName()
	{
		if (m_defaultLangName.empty())
		{
			return std::wstring(L"zh-CN");
		}
		return m_defaultLangName;
	}

	std::wstring sysLangName()
	{
		WCHAR country[9] = {0};
		WCHAR local[9] = {0};
		WCHAR langName[19] = {0};
		GetLocaleInfoW(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, country, 9);
		GetLocaleInfoW(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, local, 9);
		wsprintfW(langName, L"%s-%s", country, local);	
		return std::wstring(langName);
	}

private:
	bool _upToRelease(std::wstring& strPath)
	{
		DWORD   dwAttr   =   GetFileAttributes(strPath.data()); 
		if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
		{ 
			return false;
		}
		int pos = strPath.rfind(L"/debug");
		if (pos != -1)
		{
			strPath.replace(pos, std::wstring(L"/debug").length(), L"/release");
			return true;
		}
		return false;
	}

	bool _jsonLoadFromEncryptFile(const std::wstring& filename, DWJsonDocument& jsonDoc, const std::string &key, int round)
	{
		std::string featureData;
		if (!DwUtility::ufile::ReadFileData(filename, featureData))
		{
			DW_ASSERT(0);
		}

		DwUtility::encryption::DWEncrypt decrypt(DwUtility::encryption::DWEncrypt::TEADecrypt, key, round);
		decrypt.addData(featureData);
		std::string result = decrypt.result();

		return jsonDoc.loadFromData(result.c_str());
	}

	std::wstring _getAppResourcePath()
	{
		std::wstring appRootPath = DwUtility::app::appRootPath();

#ifdef _DEBUG
		_upToRelease(appRootPath);
#endif // _DEBUG

		if( !appRootPath.empty() && appRootPath.rfind(L"/") == std::string::npos)
		{
			appRootPath += L"/";
		}

		return appRootPath;
	}

	template <class T>
	T convStr(const std::wstring &s, const T &defaultValue);

	template <>
	std::wstring convStr(const std::wstring &s, const std::wstring &defaultValue)
	{
		if (!s.empty())
			return s;
		return defaultValue;
	}

	template <>
	unsigned int convStr(const std::wstring &s, const unsigned int &defaultValue)
	{
		if (!s.empty())
			return wcstoul(s.c_str(), NULL, 10);
		return defaultValue;
	}

	template <>
	int convStr(const std::wstring &s, const int &defaultValue)
	{
		if (!s.empty())
			return _wtoi(s.c_str());
		return defaultValue;
	}

	template <>
	bool convStr(const std::wstring &s, const bool &defaultValue)
	{
		if (!s.empty())
			return s == L"true";
		return defaultValue;
	}

	template <class T>
	T queryValue(const DWJsonObject &jsonObj, const char *name, const T &defaultValue)
	{
		if (!jsonObj.isValid())
			return defaultValue;

		return convStr(jsonObj.get(name), defaultValue);
	}

	template <class T>
	std::vector<T> queryVectorValue(const DWJsonObject &jsonObject, const char *name, const std::vector<T> &defaultValue = std::vector<T>())
	{
		const DWJsonObject c = jsonObject.getChildObject(name);
		if (!c.isValid())
			return defaultValue;

		std::vector<T> ret;
		ret.reserve(c.getChildCount());

		for (int i = 0; i < c.getChildCount(); ++i)
		{
			ret.push_back(convStr<T>(net2app(c.getChildObject(i).name()), T()));
		}

		return ret;
	}

	void loadLangCheckedConfig()
	{
		PERFLOG;

		m_defaultLangName = KI18nzhCN;
		std::wstring defaultLangFile = m_resourcePath + L"/lang/lang.config";

		if (GetFileAttributes(defaultLangFile.data()) == -1)
		{
			DW_ASSERT_X(false, __FUNCTION__, "lang.config file no exit");
		}

		if (GetFileAttributes(defaultLangFile.data()) != -1)
		{
			DWJsonDocument defaultLangJsonDoc;
			if (defaultLangJsonDoc.loadFromFile(defaultLangFile))
			{
				DWJsonObject jsonObj = defaultLangJsonDoc.rootObject();
				DW_ASSERT(jsonObj.isValid());
				if(jsonObj.isValid())
				{
					m_defaultLangName = queryValue(jsonObj, "defaultlangname", m_defaultLangName);
				}
			}
			else
			{
				DW_ASSERT(0);
			}
		}
	}

	void loadFeatureConfig()
	{
		PERFLOG;
		std::wstring configFile;
		configFile = m_resourcePath + L"config/feature_" + yyLangName() + L".config";
		if( GetFileAttributes(configFile.data()) == -1)
		{
			DW_ASSERT_X(false, __FUNCTION__, "feature_X.config file no exit");
		}

		DWJsonDocument jsonDoc;
		if (
#ifndef OFFICIAL_BUILD
			jsonDoc.loadFromFile(configFile) ||
#endif
			_jsonLoadFromEncryptFile(configFile, jsonDoc, "feature", 64))
		{
			DWJsonObject jsonObj = jsonDoc.rootObject();
			DW_ASSERT(jsonObj.isValid());
			if(jsonObj.isValid())
			{
				_loadFeatureConfig(jsonObj);
			}
		}
		else
		{
			DW_ASSERT(0);
		}
	}

	void _loadFeatureConfig(DWJsonObject jsonObj)
	{
		if (!jsonObj.isValid())
		{
			return;
		}
		DWJsonObject systemConfig = jsonObj.getChildObject("system");
		loadSystemConfig(systemConfig);

		DWJsonObject generalConfig = jsonObj.getChildObject("general");
		loadGeneralConfig(generalConfig);

		DWJsonObject channelConfig = jsonObj.getChildObject("channel");
		loadChannelConfig(channelConfig);

		DWJsonObject   imConfig = jsonObj.getChildObject("im");
		loadImConfig(imConfig);

		DWJsonObject groupConfig = jsonObj.getChildObject("group");
		loadGroupConfig(groupConfig);

		DWJsonObject mainframeConfig = jsonObj.getChildObject("mainframe");
		loadMainFrame(mainframeConfig);

		DWJsonObject mysettingConfig = jsonObj.getChildObject("mysetting");
		loadMySetting(mysettingConfig);

		DWJsonObject yyacc = jsonObj.getChildObject("yyacc");
		loadAccSetting(yyacc);

		DWJsonObject urlPath = jsonObj.getChildObject("urlpath");
		loadUrlPath(urlPath);

		DWJsonObject component = jsonObj.getChildObject("component");
		loadComponentConfig(component);
	}

	void loadSystemConfig(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0);
		if (object.isValid())
		{
			featureConfig.enableSetProcessWorkingSet = queryValue(object, "enableSetProcessWorkingSet", true);
			featureConfig.defaultFontName = queryValue<std::wstring>(object, "defaultFontName", L"SimSun");
			featureConfig.defaultFontSize = queryValue(object, "defaultFontSize", 12);
			featureConfig.listFontName = queryValue<std::wstring>(object, "listFontName", L"Microsoft Sans Serif");
			featureConfig.listFontSize = queryValue(object, "listFontSize", 12);
		}
	}

	void loadGeneralConfig(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0 );
		if (object.isValid())
		{
			featureConfig.enableDuiSpy = queryValue(object, "enableDuiSpy", false);
			featureConfig.enableYYTicket = queryValue(object, "enableYYTicket", true);
			featureConfig.enableYYComStore = queryValue(object, "enableYYComStore", true);
			featureConfig.enableYYVIP = queryValue(object, "enableYYVIP", true);
			featureConfig.enablePerfStub = queryValue(object, "enablePerfStub", true);
		}
	}

	void loadChannelConfig(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0);
		if (object.isValid())
		{
			featureConfig.enableChannelFont = queryValue(object, "enableChannelFont", true);
			featureConfig.enableChannelAddon = queryValue(object, "enableChannelAddon", true);
			featureConfig.enableApplyCener = queryValue(object, "enableChannelApplyCener", true);
			featureConfig.enableChannelHang = queryValue(object, "enableChannelHang", true);
			featureConfig.enableChannelGame = queryValue(object, "enableChannelGame", true);
			featureConfig.enableChannelPrivilege = queryValue(object, "enableChannelPrivilege", true);
			featureConfig.enableChannelInvitation = queryValue(object, "enableChannelInvitation", true);
			featureConfig.enableChannelHomePage = queryValue(object, "enableChannelHomePage", true);
			featureConfig.enableChannelMagicSmiles = queryValue(object, "enableChannelMagicSmiles", true);
			featureConfig.enableChannelReportType = queryValue(object, "enableChannelReportType", true);
			featureConfig.enableApplyCornetChannelID = queryValue(object, "enableApplyCornetChannelID", true);
			featureConfig.enableWebYY = queryValue(object, "enableWebYY", true);
			featureConfig.channelAddonBlackList = queryValue<std::wstring>(object,
				"channelAddonBlackList",
				L"");
			featureConfig.channelAddonWhiteList = queryValue<std::wstring>(object,
				"channelAddonWhiteList", 
				L"");
		}
	}

	void loadImConfig(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0);
		if (object.isValid())
		{
			featureConfig.enableImVipface = queryValue(object, "enableImVipface", true);
			featureConfig.enableImTextAd = queryValue(object, "enableImTextAd", true);
			featureConfig.enableFaceRoam = queryValue(object, "enableFaceRoam", true);
			featureConfig.enablePersonalFont= queryValue(object, "enablePersonalFont", true);
		}
	}

	void loadGroupConfig(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0);
		if (object.isValid())
		{
			featureConfig.enableGroupVipface = queryValue(object, "enableGroupVipface", true);
		}
	}

	void loadMainFrame(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0);
		if (object.isValid())
		{
			featureConfig.componentBlackList = queryValue<std::wstring>(object,
				"componentBlackList",
				L"");
			featureConfig.componentWhiteList = queryValue<std::wstring>(object,
				"componentWhiteList", 
				L"");
			featureConfig.enablePanelController = queryValue(object, "enablePanelController", true);
			featureConfig.enableSceurityCenter = queryValue(object, "enableSceurityCenter", true);
			featureConfig.enablesecurityCryptoguard = queryValue(object, "enableSecurityCryptoGuard", true);
			featureConfig.enableActivity = queryValue(object, "enableActivity", true);
			featureConfig.enableDuowanHome = queryValue(object, "enableDuowanHome", true);
			featureConfig.enableDuowanYYBBS = queryValue(object, "enableDuowanYYBBS", true);
			featureConfig.enableCustomerService = queryValue(object, "enableCustomerService", true);
			featureConfig.enableHelpHome = queryValue(object, "enableHelpHome", true);
		}
	}

	void loadMySetting(DWJsonObject object)
	{
		DW_ASSERT(object.isValid() || yyLangName().compare(KI18nzhCN) == 0);
		if (object.isValid())
		{
			featureConfig.enableLocation = queryValue(object, "enableLocation", true);
		}
	}

	void loadAccSetting(DWJsonObject object)
	{
		//DW_ASSERT(object.isValid());
		if (object.isValid())
		{
			featureConfig.enableAccessible = queryValue(object, "enableAccessible", false);
			featureConfig.enableAcchelper = queryValue(object, "enableAcchelper", true);
		}
	}

	void loadUrlPath(DWJsonObject object)
	{
		DW_ASSERT(object.isValid());
		if (object.isValid())
		{
			urlPath.yyHomeUrl = queryValue<std::wstring>(object, "yyHome", L"");
			urlPath.securityHomepageUrl = queryValue<std::wstring>(object, "securityHomepage", L"");
			urlPath.securityChangePasswordUrl = queryValue<std::wstring>(object, "securityChangePassword", L"");
			urlPath.securityForgetPasswordUrl = queryValue<std::wstring>(object, "securityForgetPassword", L"");
			urlPath.securityRegisterAccountUrl = queryValue<std::wstring>(object, "securityRegisterAccount", L"");
			urlPath.securityOtherChangePasswordUrl = queryValue<std::wstring>(object, "securityOtherChangePassword", L"");
			urlPath.securityMobieTokenUrl = queryValue<std::wstring>(object, "securityMobieToken", L"");
			urlPath.securityAccountProtectUrl = queryValue<std::wstring>(object, "securityAccountProtect", L"");
			urlPath.securitySystemAffectUrl = queryValue<std::wstring>(object, "securitySystemAffect", L"");
			urlPath.bugFeedbackUrl = queryValue<std::wstring>(object, "bugFeedback", L"");
			urlPath.guildWebUrl = queryValue<std::wstring>(object, "guildWeb", L"");
			urlPath.channelRanking = queryValue<std::wstring>(object, "channelRanking", L"");
			urlPath.channelType    = queryValue<std::wstring>(object, "channelType", L"");
			urlPath.gamePrivilege = queryValue<std::wstring>(object, "gamePrivilege", L"");
			urlPath.facebookLogin = queryValue<std::wstring>(object, "facebookLogin", L"");
			urlPath.flashSupport = queryValue<std::wstring>(object, "flashSupport", L"");
			urlPath.beanfunLogin = queryValue<std::wstring>(object, "beanfunLogin", L"");
			urlPath.beanfunLoginUdb = queryValue<std::wstring>(object, "beanfunLoginUdb", L"");
			urlPath.helpCenter    = queryValue<std::wstring>(object, "helpCenter", L"");
		}
	}

	void loadComponentConfig(const DWJsonObject &object)
	{
		if (object.isValid())
		{
			featureConfig.componentTestPath = queryValue<std::wstring>(object, "componentTestPath", L"");
			featureConfig.enableLinkDComponentInfo = queryValue(object, "enableLinkDComponentInfo", true);
			featureConfig.enableTemplateComponent = queryValue(object, "enableTemplateComponent", true);
			featureConfig.enableChannelComponent = queryValue(object, "enableChannelComponent", true);
			featureConfig.loadInfoFromLocalXml = queryValue(object, "loadInfoFromLocalXml", false);
			featureConfig.componentTestEnvironment = queryValue(object, "componentTestEnvironment", 0);
			featureConfig.componentDebugList = queryVectorValue<unsigned int>(object, "componentDebugList");
		}
	}

private:
	std::wstring m_resourcePath;
	std::wstring m_defaultLangName;
};

//---------------------------------------
DwI18n::DwI18n()
{

}
DwI18n::~DwI18n()
{

}
int  DwI18n::defaultFontSize()
{
	return DwI18nData::instance().featureConfig.defaultFontSize;
}

std::wstring DwI18n::defaultFontName()
{
	return DwI18nData::instance().featureConfig.defaultFontName;
}

int  DwI18n::listDefaultFontSize()
{
	return  DwI18nData::instance().featureConfig.listFontSize ;
}

std::wstring DwI18n::listDefaultFontName()
{
	return DwI18nData::instance().featureConfig.listFontName;
}

std::wstring DwI18n::channelAddonBlackList()
{
	return DwI18nData::instance().featureConfig.channelAddonBlackList;
}

std::wstring DwI18n::channelAddonWhiteList()
{
	return DwI18nData::instance().featureConfig.channelAddonWhiteList;
}

std::wstring DwI18n::componentBlackList()
{
	return DwI18nData::instance().featureConfig.componentBlackList;
}

std::wstring DwI18n::componentWhiteList()
{
	return DwI18nData::instance().featureConfig.componentWhiteList;
}

std::wstring DwI18n::sysLangName()
{

	return DwI18nData::instance().sysLangName();
}

std::wstring DwI18n::yyLangName()
{
	return DwI18nData::instance().yyLangName();
}

int DwI18n::yyLangNameID()
{
	return DwI18nData::instance().yyLangNameID();
}

std::wstring DwI18n::queryUrl(UrlPath urlType)
{
	std::wstring urlPath;
	switch (urlType)
	{
	case YYHome:
		urlPath = DwI18nData::instance().urlPath.yyHomeUrl;
		break;
	case SecurityHomepage:
		urlPath = DwI18nData::instance().urlPath.securityHomepageUrl;
		break;
	case SecurityChangePassword:
		urlPath = DwI18nData::instance().urlPath.securityChangePasswordUrl;
		break;
	case SecurityRegisterAccount:
		urlPath = DwI18nData::instance().urlPath.securityRegisterAccountUrl;
		break;
	case SecurityForgetPassword:
		urlPath = DwI18nData::instance().urlPath.securityForgetPasswordUrl;
		break;
	case SecurityOtherChangePassword:
		urlPath = DwI18nData::instance().urlPath.securityOtherChangePasswordUrl;
		break;
	case SecurityMobieToken:
		urlPath = DwI18nData::instance().urlPath.securityMobieTokenUrl;
		break;
	case SecurityAccountProtect:
		urlPath = DwI18nData::instance().urlPath.securityAccountProtectUrl;
		break;
	case SecuritySystemAffect:
		urlPath = DwI18nData::instance().urlPath.securitySystemAffectUrl;
		break;
	case BugFeedback:
		urlPath = DwI18nData::instance().urlPath.bugFeedbackUrl;
		break;
	case GuildWeb:
		urlPath = DwI18nData::instance().urlPath.guildWebUrl;
		break;
	case ChannelRanking:
		urlPath = DwI18nData::instance().urlPath.channelRanking;
		break;
	case ChannelType:
		urlPath = DwI18nData::instance().urlPath.channelType;
		break;
	case GamePrivilege:
		urlPath = DwI18nData::instance().urlPath.gamePrivilege;
		break;
	case FacebookLogin:
		urlPath = DwI18nData::instance().urlPath.facebookLogin;
		break;
	case FlashSupport:
		urlPath = DwI18nData::instance().urlPath.flashSupport;
		break;
	case BeanfunLogin:
		urlPath = DwI18nData::instance().urlPath.beanfunLogin;
		break;
	case BeanfunLoginUdb:
		urlPath = DwI18nData::instance().urlPath.beanfunLoginUdb;
		break;
	case HelpCenter:
		urlPath = DwI18nData::instance().urlPath.helpCenter;
		break;;
	default:
		DW_ASSERT(0);
		break;
	}
	return urlPath;
}

std::wstring DwI18n::componentTestPath()
{
	return DwI18nData::instance().featureConfig.componentTestPath;
}

bool DwI18n::isFeatureEnabled(FeatureType featureType)
{
	bool bEnable = true;
	switch (featureType)
	{
	case EnableSetProcessWorkingSet:
		bEnable = DwI18nData::instance().featureConfig.enableSetProcessWorkingSet;
		break;
	case EnableChannelFont:
		bEnable = DwI18nData::instance().featureConfig.enableChannelFont;
		break;
	case EnableChannelAddon:
		bEnable = DwI18nData::instance().featureConfig.enableChannelAddon;
		break;
	case EnableChannelApplyCener:
		bEnable = DwI18nData::instance().featureConfig.enableApplyCener;
		break;
	case EnableChannelHang:
		bEnable = DwI18nData::instance().featureConfig.enableChannelHang;
		break;
	case EnableChannelGame:
		bEnable = DwI18nData::instance().featureConfig.enableChannelGame;
		break;
	case EnableChannelPrivilege:
		bEnable = DwI18nData::instance().featureConfig.enableChannelPrivilege;
		break;
	case EnableChannelInvitation:
		bEnable = DwI18nData::instance().featureConfig.enableChannelInvitation;
		break;
	case EnableChannelHomePage:
		bEnable = DwI18nData::instance().featureConfig.enableChannelHomePage;
		break;
	case EnableChannelMagicSmiles:
		bEnable = DwI18nData::instance().featureConfig.enableChannelMagicSmiles;
		break;
	case EnableChannelReportType:
		bEnable = DwI18nData::instance().featureConfig.enableChannelReportType;
		break;
	case EnableApplyCornetChannelID:
		bEnable = DwI18nData::instance().featureConfig.enableApplyCornetChannelID;
		break;
	case EnableWebYY:
		bEnable = DwI18nData::instance().featureConfig.enableWebYY;
		break;
	case EnbleImVipface:
		bEnable = DwI18nData::instance().featureConfig.enableImVipface;
		break;
	case EnableGroupVipface:
		bEnable = DwI18nData::instance().featureConfig.enableImVipface;
		break;
	case EnableImTextAd:
		bEnable = DwI18nData::instance().featureConfig.enableImTextAd;
		break;
	case EnableFaceRoam:
		bEnable = DwI18nData::instance().featureConfig.enableFaceRoam;
		break;
	case EnablePersonalFont:
		bEnable = DwI18nData::instance().featureConfig.enablePersonalFont;
		break;
	case EnablePanelController:
		bEnable = DwI18nData::instance().featureConfig.enablePanelController;
		break;
	case EnablesecurityCryptoguard:
		bEnable = DwI18nData::instance().featureConfig.enablesecurityCryptoguard;
		break;
	case EnableSceurityCenter:
		bEnable = DwI18nData::instance().featureConfig.enableSceurityCenter;
		break;
	case EnableDuowanHome:
		bEnable = DwI18nData::instance().featureConfig.enableDuowanHome;
		break;
	case EnableDuowanYYBBS:
		bEnable = DwI18nData::instance().featureConfig.enableDuowanYYBBS;
		break;
	case EnableCustomerService:
		bEnable = DwI18nData::instance().featureConfig.enableCustomerService;
		break;
	case EnableHelpHome:
		bEnable = DwI18nData::instance().featureConfig.enableHelpHome;
		break;
	case EnableActivity:
		bEnable = DwI18nData::instance().featureConfig.enableActivity;
		break;
	case EnableLocation:
		bEnable = DwI18nData::instance().featureConfig.enableLocation;
		break;
	case EnableYYTicket:
		bEnable = DwI18nData::instance().featureConfig.enableYYTicket;
		break;
	case EnableYYComStore:
		bEnable = DwI18nData::instance().featureConfig.enableYYComStore;
		break;
	case EnableYYVIP:
		bEnable = DwI18nData::instance().featureConfig.enableYYVIP;
		break;
	case EnableDuiSpy:
		bEnable = DwI18nData::instance().featureConfig.enableDuiSpy;
		break;
	case EnablePerfStub:
		bEnable = DwI18nData::instance().featureConfig.enablePerfStub;
		break;
	case EnableAccessible:
		bEnable = DwI18nData::instance().featureConfig.enableAccessible;
		break;
	case EnableAcchelper:
		bEnable = DwI18nData::instance().featureConfig.enableAcchelper;
		break;
	case EnableLinkDComponentInfo:
		bEnable = DwI18nData::instance().featureConfig.enableLinkDComponentInfo;
		break;
	case EnableTemplateComponent:
		bEnable = DwI18nData::instance().featureConfig.enableTemplateComponent;
		break;
	case EnableChannelComponent:
		bEnable = DwI18nData::instance().featureConfig.enableChannelComponent;
		break;
	case LoadInfoFromLocalXml:
		bEnable = DwI18nData::instance().featureConfig.loadInfoFromLocalXml;
		break;
	default:
		DW_ASSERT(0);
	}
	return bEnable;
}

int DwI18n::componentTestEnvironment()
{
	return DwI18nData::instance().featureConfig.componentTestEnvironment;
}

std::vector<unsigned int> DwI18n::componentDebugList()
{
	return DwI18nData::instance().featureConfig.componentDebugList;
}
