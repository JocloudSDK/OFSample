#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

__declspec(selectany) extern const char KChannelService[] = "ChannelService";
__declspec(selectany) extern const char KChannelServiceLogic[] = "ChannelServiceLogic";
__declspec(selectany) extern const char KIChannel[] = "IChannel";
__declspec(selectany) extern const char KISearch[] = "ISearch";

__declspec(selectany) extern const char KChannelJoin[] = "asyncJoin";
__declspec(selectany) extern const char KChannelJoinFrom[] = "asyncJoinFrom";
__declspec(selectany) extern const char KChannelSilentJoin[] ="asyncSilentJoin";
__declspec(selectany) extern const char KChannelTabJoin[]="asyncTabJoin";
__declspec(selectany) extern const char KChannelTemplateJoin[]="asyncTemplateJoin";
__declspec(selectany) extern const char KChannelTemplateJoinWithData[]="asyncTemplateJoinWithData";
__declspec(selectany) extern const char KChannelTemplateJoinWithDataFrom[]="asyncTemplateJoinWithDataFrom";
__declspec(selectany) extern const char KChannelWonderWorldJoin[] = "asyncWonderWorldJoin";
__declspec(selectany) extern const char KChannelOpenWonderWorldTab[] = "asyncOpenWonderWorldTab";
__declspec(selectany) extern const char KChannelOpenWonderWorldTabEx[] = "asyncOpenWonderWorldTabEx";
__declspec(selectany) extern const char KChannelJoinNoShow[] = "asyncJoinNoShow";
__declspec(selectany) extern const char KChannelJoinWithUserData[] = "asyncJoinWithUserData";
__declspec(selectany) extern const char KChannelFunnyRoomJoin[] = "asyncFunnyRoomJoin";
__declspec(selectany) extern const char KJoinFunnyRoomChannel[] = "asyncJoinFunnyRoomChannel";
__declspec(selectany) extern const char KChannelLeave[] = "asyncLeave";
__declspec(selectany) extern const char KChannelForceLeave[] = "asyncForceLeave";
__declspec(selectany) extern const char KChannelNetBreakLeave[] = "netBreakLeave";
__declspec(selectany) extern const char KHasChannelWindow[] = "hasChannelWindow";
__declspec(selectany) extern const char KChannelShowWindow[] = "showChannelWindow";
__declspec(selectany) extern const char KLeaveWonderWorldChannel[] = "leaveWonderWorldChannel";
__declspec(selectany) extern const char KAsyncShowCloseReason[] = "asyncShowCloseReason";
__declspec(selectany) extern const char KIsCurrentChannel[] = "isCurrentChannelId";
__declspec(selectany) extern const char KIsCurrentChannelMine[] = "isCurrentChannelMine";
__declspec(selectany) extern const char KStartPrivateChat[] = "startPrivateChat";
__declspec(selectany) extern const char KSwitchChannelStyle[] = "switchChannelStyle";
__declspec(selectany) extern const char KCurrentChannelId[] = "currentChannelId";
__declspec(selectany) extern const char KCurrentSubChannelId[] = "currentSubChannelId";
__declspec(selectany) extern const char KBatchQuerySubChannelUsers[] = "batchQuerySubChannelUsers";
__declspec(selectany) extern const char KQueryChannelUserCount[] = "queryChannelUserCount";
__declspec(selectany) extern const char KJoinChannelStyle[] = "joinChannelStyle";
__declspec(selectany) extern const char KGetChannelUserData[] = "getChannelUserData";
__declspec(selectany) extern const char KCaniLeaveChannel[] = "caniLeaveChannel";   // 返回quint32, 具体

__declspec(selectany) extern const char KChannelAboutToJoin[] = "channelAboutToJoin";
__declspec(selectany) extern const char KChannelBeenJoined[] = "channelBeenJoined";
__declspec(selectany) extern const char KChannelBeenLeft[] = "channelBeenLeft";
__declspec(selectany) extern const char KChannelBeenLeftReason[] = "channelBeenLeftReason";
__declspec(selectany) extern const char KChannelActivBeenJoined[] = "channelActivBeenJoined";
__declspec(selectany) extern const char KChannelActivBeenLeft[] = "channelActivBeenLeft";
__declspec(selectany) extern const char KChannelInfoChanged[] = "channelInfoChanged";
__declspec(selectany) extern const char KChannelTeamBeenJoined[] = "channelTeamBeenJoined";
__declspec(selectany) extern const char KChannelTeamBeenLeft[] = "channelTeamBeenLeft";
__declspec(selectany) extern const char KChannelSetChannelPushValue[] = "setChannelPushValue";
__declspec(selectany) extern const char KChannelPushSpeaker[] = "pushSpeaker";
__declspec(selectany) extern const char KChannelPushTicketLeft[] = "pushTicketLeft";
__declspec(selectany) extern const char KChannelUpdatePushTicketLeft[] = "updatePushTicketLeft";
__declspec(selectany) extern const char KChannelTemplateChanged[] = "channelTemplateChanged";
__declspec(selectany) extern const char KBulletinComponentRemove[] = "bulletinComponentRemove";
__declspec(selectany) extern const char KBulletinComponentAdd[] = "bulletinComponentAdd";

//默认频道模版
__declspec(selectany) extern const char KDefaultChannelService[] = "DefaultChannelService";
__declspec(selectany) extern const char KIDefaultChannel[] = "IChannel";

//精彩世界
__declspec(selectany) extern const char KWonderWorldService[] = "WonderWorldService";
__declspec(selectany) extern const char KIWonderWorld[] = "IWonder";
__declspec(selectany) extern const char KWonderBeenJoined[] ="wonderBeenJoined";
__declspec(selectany) extern const char KWonderBeenLeft[]="wonderBeenLeft";
__declspec(selectany) extern const char KWonderLastChannel[] = "wonderLastChannel";
__declspec(selectany) extern const char KWonderRecommendChannel[] = "wonderRecommendChannel";
__declspec(selectany) extern const char KWonderHasLastChannel[] = "wonderHasLastChannel";
__declspec(selectany) extern const char KWonderSwitch[] = "wonderSwitch";

//for pip
__declspec(selectany) extern const char KChannelChangeCurSubSid[] = "asyncChangeCurSubSid";
__declspec(selectany) extern const char KChannelFillChannelUserInfo[] = "fillChannelUserInfo";
__declspec(selectany) extern const char KChannelFillSubChannelInfo[] = "fillSubChannelInfo";
__declspec(selectany) extern const char KChannelSimulateSpeakSetting[] = "simulateSpeakSetting";

//for bugreport
__declspec(selectany) extern const char KChannelBasicInfo[] = "channelBasicInfo";
__declspec(selectany) extern const char KChannelAddonInfo[] = "channelAddonInfo";

// 进入频道、切换子频道触发此信号
// 发出sid, asid, subsid
__declspec(selectany) extern const char KChannelCurSubSidChanged[] = "curSubSidChanged";	
__declspec(selectany) extern const char KChannelSpeakingChanged[] = "speakingChanged";

// 进频道时需要额外操作
const char KChannelFurtherOperation[] = "asyncFurtherOperation";

// for apply channel
__declspec(selectany) extern const char KShowApplyChannel[] = "showApplyChannel";
__declspec(selectany) extern const char KOpenGHPUrl[] = "openGHPUrl";

// for report channel info
__declspec(selectany) extern const char KReportJoinChannelInfo[] = "reportJoinChannelInfo";

// audio
__declspec(selectany) extern const char KMuteSpeaker[] = "muteSpeaker";
__declspec(selectany) extern const char KSetSpeakerVolumn[] = "setSpeakerVolumn";
__declspec(selectany) extern const char KMuteMicphone[] = "muteMicphone";
__declspec(selectany) extern const char KSetMicphoneVolumn[] = "setMicphoneVolumn";
__declspec(selectany) extern const char KAddMicphoneVolume[] = "addMicphoneVolume";
__declspec(selectany) extern const char KReduceMicphoneVolume[] = "reduceMicphoneVolume";

// IM
__declspec(selectany) extern const char KImService[] = "ImService";
__declspec(selectany) extern const char KIMessenger[] = "IMessenger";

__declspec(selectany) extern const char KImLogin[] = "login";
__declspec(selectany) extern const char KClearMsg[] = "clearMsg";
__declspec(selectany) extern const char KImGetLogoIndex[] = "getImLogoIndex";
__declspec(selectany) extern const char KImGetLogoUrl[] = "getImLogoUrl";
__declspec(selectany) extern const char KImGetLogoUrlOfSize100[] = "getImLogoUrlOfSize100";
__declspec(selectany) extern const char KImGetImId[] = "getImId";
__declspec(selectany) extern const char KImGetImStatus[] = "getImStatus";
__declspec(selectany) extern const char KImGetImListView[] = "createImListView";
__declspec(selectany) extern const char KImGetImSearchBuddyView[] = "createImBuddySearchView";
__declspec(selectany) extern const char KImOpenChatWindow[] = "openChatWnd";
__declspec(selectany) extern const char KImClearImHistoryMsg[] = "clearImHistoryMsg";
__declspec(selectany) extern const char KImIsOpenChatWindow[] = "isOpenChatWnd";
__declspec(selectany) extern const char KImOpenChatWindowFromPal[] = "openChatWndFromYYFriend";
__declspec(selectany) extern const char KImOpenChatWindowFromPal2[] = "openChatWndFromYYFriend2";
__declspec(selectany) extern const char KImAddFriend[] = "addFriend";
__declspec(selectany) extern const char KImAddFriend2[] = "addFriend2";
__declspec(selectany) extern const char KImAddFriend3[] = "addFriend3";
__declspec(selectany) extern const char KImAddFriendForPal[] = "addFriendForPal";
__declspec(selectany) extern const char KImInviteBuddy[] = "inviteBuddy";
__declspec(selectany) extern const char KChangeImStatus[] = "changeImStatus";
__declspec(selectany) extern const char KScreenShot[] = "screenShot";
__declspec(selectany) extern const char KImIsBuddy[] = "isBuddy";
__declspec(selectany) extern const char KImGetBuddylistDataItemModel[] = "getBuddylistDataItemModel";
__declspec(selectany) extern const char KImAddFriendForGame[]  = "addFriendForGame";
__declspec(selectany) extern const char KImPalyVoiceFile[]	  = "PalyVoiceFile";
__declspec(selectany) extern const char KImStopPalyVoiceFile[] = "StopPalyVoiceFile";
__declspec(selectany) extern const char KPause[]	 = "Pause";
__declspec(selectany) extern const char KResume[] = "Resume";
__declspec(selectany) extern const char KImReleaseBuddyListDataItemModel[] ="releaseBuddyItemDataModel";

__declspec(selectany) extern const char KImLoginChanged[] = "imLoginChanged";
__declspec(selectany) extern const char KImInfoChanged[] = "imInformationChanged";
__declspec(selectany) extern const char KImShareChannelGameCheckStatus[] = "imShareChannelGameCheckStatus";
__declspec(selectany) extern const char KImIsInBlackList[] = "isInBlackList";

__declspec(selectany) extern const char KImSetNoAddBuddyNotifyInSameChannel[] = "imSetNoAddBuddyNotifyInSameChannel";		// do not invoke this one.
__declspec(selectany) extern const char KImSetNoAddBuddyNotifyIfInChannel[] = "imSetNoAddBuddyNotifyIfInChannel";
__declspec(selectany) extern const char KIMCreateSecureVerifyLogic[]		=	"createSecureVerifyLogic";

// Group
__declspec(selectany) extern const char KIGroup[] = "IGroup";
__declspec(selectany) extern const char KGroupService[] = "GroupService";
__declspec(selectany) extern const char KGroupGetGListView[] = "createGroupListView";
__declspec(selectany) extern const char KGroupGetFindView[] = "createGroupFindView";
__declspec(selectany) extern const char KGroupShowImportChannelWnd[] = "showImportChannelWnd";
__declspec(selectany) extern const char KGroupGetGuildGrpCookie[] = "getGuildGroupCookie";
__declspec(selectany) extern const char KGroupGetGuildGrpUserAgent[] = "getGuildGroupUserAgent";
__declspec(selectany) extern const char KGroupOpenGuildGroupWnd[] = "openGuildGroupWnd";
__declspec(selectany) extern const char KGroupGetGuildOperation[] = "getGuildOperation";
__declspec(selectany) extern const char KGroupShowGuildSetupWnd[] = "showGuildSetupWnd";
__declspec(selectany) extern const char KGroupShowGroupJoinFrame[] = "showGroupJoinFrame";

__declspec(selectany) extern const char KGroupOpenGroupWnd[] = "openGroupWnd";
__declspec(selectany) extern const char KGroupShowSetupWnd[] = "showSetupWnd";
__declspec(selectany) extern const char KGroupDeleteSetupWnd[] = "deleteSetupWnd";
__declspec(selectany) extern const char getGroupListModel[] = "getGroupListModel";
__declspec(selectany) extern const char KBindGroupToChannel[] = "bindGroupFromChannel";

// ImGroupApp
__declspec(selectany) extern const char KImAppService[] = "ImAppService";
__declspec(selectany) extern const char KImGroupApp[] = "ImGroupApp";

__declspec(selectany) extern const char KImAppInit[] = "ImAppInit";
__declspec(selectany) extern const char KImTipsStrategy[] = "imTipsStategy";
__declspec(selectany) extern const char KImPersonalChatHeaderStatus[] = "headerStatus";
__declspec(selectany) extern const char KImChatLeftInputEdit[] = "inputEdit";


// YYMainFrame
__declspec(selectany) extern const char KMainframeService[] = "MainframeService";
__declspec(selectany) extern const char KFeatureService[] = "FeatureService";
__declspec(selectany) extern const char KFeatureConfigChanged[] = "featureConfigChanged";

__declspec(selectany) extern const char KMainframeLogin[] = "ILogin";
__declspec(selectany) extern const char KPipInterface[] = "IPipInterface";
__declspec(selectany) extern const char KIStatistics[] = "IStatistics";

__declspec(selectany) extern const char KMainframeLoginChanged[] = "loginChanged";
__declspec(selectany) extern const char KMainframeNotifyBox[] = "NotificationBox";
__declspec(selectany) extern const char KMainframeCommonUI[] = "commonUI";
__declspec(selectany) extern const char KPostYYIPCMessage[] = "postYYIPCMessage";
__declspec(selectany) extern const char KGetMyUid[] = "getMyUid";
__declspec(selectany) extern const char KIsLogined[] = "isLogined";
__declspec(selectany) extern const char KIsKickOff[] = "isKickOff";
__declspec(selectany) extern const char KGetWANIp[] = "getWANIp";
__declspec(selectany) extern const char KOpenPrivilege[] = "showPrivilegeDialog";
__declspec(selectany) extern const char KSetPrivilegeRedirect[] = "setPrivilegeRedirect";
__declspec(selectany) extern const char KShowGameCharge[] = "showGameChargeDialog";
__declspec(selectany) extern const char KMainframeColorThemeChanged[] = "colorThemeChanged";
__declspec(selectany) extern const char KRecentSessLstChanged[] = "sigRecentSessLstChanged";
__declspec(selectany) extern const char KMyGuildLstChanged[] = "sigMyGuildLstChanged";
__declspec(selectany) extern const char KAddToRecentNick[] = "addToRecentNick";
__declspec(selectany) extern const char KaddCrashProperty[] = "addCrashProperty";
__declspec(selectany) extern const char KGetRecentNickList[] = "getRecentNickList";
__declspec(selectany) extern const char KAddChannelStatus[] = "addChannelStatus";
__declspec(selectany) extern const char KArriveGFBanner[] = "arriveGFBanner";//gamefriend
__declspec(selectany) extern const char KGetConfigRoam[] = "getConfigRoam";
__declspec(selectany) extern const char KGetTrayIconRect[] = "getTrayIconRect";
__declspec(selectany) extern const char KStatisticsService[] = "StatisticsService";
__declspec(selectany) extern const char KMainframeShowed[] = "mainframeShowed";
__declspec(selectany) extern const char KMainframeHidden[] = "mainframeHidden";
__declspec(selectany) extern const char KIsPopupWindowDisabled[] = "isPopupWindowDisabled";
__declspec(selectany) extern const char KSetPopupWindowDisabled[] = "setPopupWindowDisabled";
__declspec(selectany) extern const char KNewPopupWindow[] = "sigNewPopupWindow";
__declspec(selectany) extern const char KLiveProgramNumChanged[] = "liveProgramNumChanged";

__declspec(selectany) extern const char KStatisticsPlayingGame[] = "StatisticsPlayingGame";
__declspec(selectany) extern const char KStatisticsOnTimer[] = "KStatisticsOnTimer";

__declspec(selectany) extern const char KStatisticsOnTimeForReputation[] = "KStatisticsOnTimeForReputation";
__declspec(selectany) extern const char KQueryTicket[] = "queryTicket";
__declspec(selectany) extern const char KMainFrameFocus[] = "isMainFrameFocus";
__declspec(selectany) extern const char KQueryOTP[] = "queryOTP";


__declspec(selectany) extern const char KAudioFilePlay[] = "audioFilePlay";
__declspec(selectany) extern const char KAudioFileStop[] = "audioFileStop";
__declspec(selectany) extern const char KAudioFileDestroy[] = "audioFileDestroy";

__declspec(selectany) extern const char KShowThemeColorFrame[] = "showThemeColorFrame"; 
__declspec(selectany) extern const char KOpenYYNews[] = "asyncOpenYYNews";

// for pip
__declspec(selectany) extern const char KRecentSessList[] = "getRecentSessList";
__declspec(selectany) extern const char KRecentSess2List[] = "getRecentSess2List";
__declspec(selectany) extern const char KPipMyGuildList[] = "getMyGuildList";
__declspec(selectany) extern const char KPipMyGuildItemList[] = "getMyGuildItemList";

// yyplugin
__declspec(selectany) extern const char KPluginService[] = "PluginService";
__declspec(selectany) extern const char KPluginPlayingGame[] = "MyPlayingGameChanged";

// histroy message service
__declspec(selectany) extern const char KHistroyMsgService[] = "HistroyMsgService";

enum LeaveChannelAnswerType
{
    kLCAT_OfCourse = 0x0,   // 可以离开频道
    kLCAT_InMaixuList,      // 正在麦序列表里
    kLCAT_Recording,        // 正在录音
	kLCAT_EnterChannelTooMuch, //切换频道太频繁
	kLCAT_BusinessNotAllowLeave, // 业务不允许离开
	kLCAT_BusinessActionTakeover  //业务接管离开逻辑（自定义弹窗提醒等）
};

class QVariant;

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYDebusHelper):public IUnk
{
private:
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QVariant _debusCall(const char *serviceName, const char *interfaceName, const char *methodName, const QVariantList& args) = 0;

public:
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void connectSignal(const char *serviceName, const char*signalName, QObject *receiver, const char *slot) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void disconnectSignal(const char *serviceName, const char*signalName, QObject *receiver, const char *slot) = 0;

public:
/**
   * @brief .
   * @param .
   * @return .
   */
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName)
	{
		QVariantList args;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T t)
	{
		QVariantList args;
		args<<t;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2)
	{
		QVariantList args;
		args<<t1 << t2;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2, typename T3>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2, T3 t3)
	{
		QVariantList args;
		args<<t1 << t2 << t3;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2, typename T3, typename T4>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2, T3 t3, T4 t4)
	{
		QVariantList args;
		args<<t1 << t2 << t3 << t4;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
	{
		QVariantList args;
		args<<t1 << t2 << t3 << t4 << t5;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
	{
		QVariantList args;
		args<<t1 << t2 << t3 << t4 << t5 << t6;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
	{
		QVariantList args;
		args<<t1 << t2 << t3 << t4 << t5 << t6 << t7;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	QVariant debusCall(const char *serviceName, const char *interfaceName, const char *methodName, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
	{
		QVariantList args;
		args<<t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;
		return _debusCall(serviceName, interfaceName, methodName, args);
	}
};