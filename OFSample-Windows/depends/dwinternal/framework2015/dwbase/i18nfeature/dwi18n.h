#pragma once

#include "dwglobal.h"

__declspec( selectany ) extern const wchar_t KI18nzhCN[] = L"zh-CN";
__declspec( selectany ) extern const wchar_t KI18nzhTW[] = L"zh-TW";
__declspec( selectany ) extern const wchar_t KI18nenUS[] = L"en-US";
__declspec( selectany ) extern const wchar_t KI18nptBR[] = L"pt-BR";

class DWBASE_EXPORTS DwI18n 
{
public:
	enum FeatureType
	{
		DefaultType = 0X1,

		//system
		EnableSetProcessWorkingSet,

		//channel
		EnableChannelFont,
		EnableChannelAddon,
		EnableChannelApplyCener,
		EnableChannelHang,
		EnableChannelGame,
		EnableChannelPrivilege,
		EnableChannelInvitation,
		EnableChannelHomePage,
		EnableChannelMagicSmiles,
		EnableChannelReportType,
		EnableApplyCornetChannelID,
		EnableWebYY,

		//im
		EnbleImVipface,
		EnableGroupVipface,
		EnableImTextAd,
		EnableFaceRoam,
		EnablePersonalFont,

		//mainframe
		EnablePanelController,
		EnablesecurityCryptoguard,
		EnableSceurityCenter,
		EnableActivity,
		EnableDuowanHome,
		EnableDuowanYYBBS,
		EnableCustomerService,
		EnableHelpHome,

		//mysetting
		EnableLocation,

		//general
		EnableYYTicket,
		EnableYYComStore,
		EnableDuiSpy,
		EnableYYVIP,
		EnablePerfStub,

		//yyacc
		EnableAccessible,
		EnableAcchelper,

		//component
		EnableLinkDComponentInfo,
		EnableTemplateComponent,
		EnableChannelComponent,
		LoadInfoFromLocalXml,
	};

	enum UrlPath
	{
		YYHome,
		SecurityChangePassword,
		SecurityRegisterAccount,
		SecurityForgetPassword,
		SecurityOtherChangePassword,
		SecurityMobieToken,
		SecuritySystemAffect,
		BugFeedback,
		GuildWeb,
		ChannelRanking,
		ChannelType,
		GamePrivilege,
		FacebookLogin,
		FlashSupport,
		BeanfunLogin,
		BeanfunLoginUdb,
		HelpCenter,
		SecurityHomepage,
		SecurityAccountProtect,
	};

public:
	static int defaultFontSize();
	static int listDefaultFontSize();
	static std::wstring defaultFontName();
	static std::wstring listDefaultFontName();
	static std::wstring channelAddonBlackList();
	static std::wstring channelAddonWhiteList();
	static std::wstring componentBlackList();
	static std::wstring componentWhiteList();

	static std::wstring sysLangName();
	static std::wstring yyLangName();
	static int yyLangNameID();
	static std::wstring queryUrl(UrlPath urlType);

	static std::wstring componentTestPath();

	static bool  isFeatureEnabled(FeatureType featureType);

	template <class widget>
	static void  shiledWidgetByFeature(widget *w, DwI18n::FeatureType feature)
	{
		DW_ASSERT(w);
		if (!DwI18n::isFeatureEnabled(feature))
		{
			w->hide();
			w->setFixSize(QSize(0, 0));
		}
	}

	template <class action, class string>
	static void sheildSubMenu(action* act, const string &subMenuName, DwI18n::FeatureType type)
	{
		DW_ASSERT(act);
		QMenu *menu  = act->menu();
		DW_ASSERT(menu);

		QList<action *> actionList = menu->actions();
		foreach( action *act, actionList )
		{
			if ( (act->objectName() == subMenuName && !DwI18n::isFeatureEnabled(type)))
			{
				menu->removeAction(act);
				return;
			}
		}
	}

	static bool isChinese()
	{
		return yyLangName() == KI18nzhCN || yyLangName() == KI18nzhTW;
	}

	static int componentTestEnvironment();

	static std::vector<unsigned int> componentDebugList();
private:
	DwI18n();
	~DwI18n();
};





