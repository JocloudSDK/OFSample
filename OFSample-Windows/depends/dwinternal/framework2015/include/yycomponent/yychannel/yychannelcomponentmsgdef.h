#pragma once
enum
{
	KBeforeJoinChannel ,
	KBeforeLeaveChannel,

	KCheckCanJoinChannel,
	KCheckCanLeaveChannel,
	
	KRealJoinChannel,
	KRealLeaveChannel,
	KActiveJoinChannel,
    KShowChannelWindow,

	KJoinChannelTour,

	KRealJoinWonderWorld,
	KRealLeaveWonderWorld,
	KRealJoinWonderWorldEx,

	KRemoveDefaultChannelWidget,
	KActiveDefaultChannelWidget,

	KQueryChannelJoinInfo ,
	//in wstring,out bool
	Klt_selectStyleByString ,
	//in int, out bool
	Klt_selectStyle ,
	//no param
	Klt_setDefalutStyle ,
	//in wstring
	Klt_setSubStyleByString ,
	//in int
	Klt_setSubStyle ,
	//out int
	Klt_getSubStyle ,
	//in int int
	Klt_setMinSize ,
	//in int int
	Klt_setMaxSize ,
	//in duiwidget* IChannelTemplateUICallBack*, out bool
	Klt_createWindow ,
	//in duiwidget* IChannelTemplateUICallBack*
	Klt_createRealWindow ,
	//
	Klt_destroyWindow ,
	//in bool
	Klt_showWindow ,
	//out duiwidget*
	Klt_getWindow ,
	//out TemplateWindowPosition, bool
	Klt_getWindowPosition ,
	//in unsigned int
	Klt_setWindowOwnerById ,
	//in int, out bool
	Klt_showState ,
	//in bool,out bool
	Klt_setFullScreen ,
	//out bool
	Klt_isFullScreen ,
	//in bool
	Klt_setViewVisible ,
	//out bool
	Klt_update ,
	//in string,out duiwidget*
	Klt_getWidget ,
	//out duiwidget*
	Klt_getWidgetByState ,
	//in int
	Klt_setWidth ,
	//out int
	Klt_getWidth ,
	//in int
	Klt_setHeight ,
	//out int
	Klt_getHeight ,
	//in QString int
	KsigWidgetChanged ,
	//in int int
	KsigSubStyleChanged,

	KHandleCommand ,
	//in uint, out bool
	KisSelfById,
	//in int,out QString
	KgetTemplateName,
	//out YYSubChannelIconInfo
	KgetSubChannelIconInfo,
	//in YYSubChannelIconInfo
	KSubChannelIconInfoAdded,
	//in QList<int>,QList<int> 
	KSubChannelIconInfoRemoved,
	//in QString, bool out int
	KsetEnableClose,
	//in QString, out bool
	KisEnableClose,
	//in QString, QString out int
	KsetTabTooltip,
	//in QString, QString out int
	KsetTabText,
	//out int
	KtabCount,
	//in int out int
	KselectTab,
	//in QString out int
	KselectTabByName,
	//out QString
	KcurrentTab,
	//out int
	KcurrentIndex,
	//in QString out int
	KindexOfTab,
	//in int out QString
	KtabName,
	//in QString out int
	KremoveTab,
	//in int out int
	KremoveTabByIndex,
	//in QString, QString, QString out DuiWidget*
	KaddTab,
	//in int QString, QString, QString out DuiWidget*
	KinsertTab,
	//in int, QString, QString out IUnkPtr
	KinsertIpcHostTab,
	//in QString out DuiWidget*
	KfindTabItem,
	//in DuiWidget* out int
	KextractHomePage,
	//in DuiWidget*
	KrestoreHomePage,
	//out _SIG_ACTIVE_PAGE_BY_INDEX*
	KgetStartActivePageByIndex,
	//out _SIG_ACTIVE_PAGE_BY_WIDGET*
	KgetStartActivePageByWidget,
	//in const char* const QObject* const char* int, out bool
	KChannelTabConnectSignal,
	//in const char* const QObject* const char*, out bool
	KChannelTabDisconnectSignal,
	//in DuiSpliterDecorator*
	KsetChildDecorator,
	//out DuiFrameWindow*
	KgetAddonBoxWindow,
	//in quint32, out bool
	KremoveChannelAddonItem,
	//in quint32 QString, out bool
	KsetChannelAddonImage,
	//in bool, out bool
	KsetDeactivatedHide,
	//in bool int
	KsetAddonMsgNotifyVisible,
	//in quint32 QString
	KsetChannelAddonTooltips,
	//in quint32 QString QString QString QString, out quint32
	KaddChannelAddonItem2,
	//in quint32
	KclickAddon,
	//in quint32, out QList<quint32>
	KqueryItemId,
	//in quint32, out QString
	KuserData,
	//in const char* const QObject* const char* int, out bool
	KaddonBoxConnectSignal,
	//in const char* const QObject* const char*, out bool
	KaddonBoxDisconnectSignal,
	//out DuiFrameWindow*
	KchannelWindow,
	//in QString QMap<QString, QVariant>, out DuiMenu*
	KchannelGetMenu,
	//in int QMap<QString, QVariant>, out DuiFrameWindow*
	KgetSubWindow,
	//in int QMap<QString, QVariant>
	KpopSubWindow,
	//in bool
	KshowDefaultChannel,
	//out DuiWidget*
	KfullCustomContainer,
	//
	KShowBulletin,
	//
	KHideBulletin,

	KCanNotJoinChannel,
	KJoinWithPassword,

	//in int, out duiwidget*
	KGetAvailableWidget,
	//发图片临时处理
	//in QString
	KSendImageUrl,

	//二麦提醒
	//quint32 managerUid, const QString& managerNick
	KSecondMaiNotify,
    KConfirmPCJoinChannel,
    KEnterChannelTooMuch,
	KSetChannelPushValue,
	KPushSpeaker,
	KPushTicketLeft,
	KUpdateTicketLeft,
	KCreateChannelBaseBulletinWidget,

    // 非YY客户端已经在频道中
    KConfirmPCJoinChannel2,

    // 错误提示
    KChannelErrorMessage,

	//频道窗口最小尺寸
	Klt_setMinSize_ChannelWindow,

	// 快捷通道跳转子频道[业务是否允许离开]
	Klt_FastWayJoinSubChannelCanLeave,

    // 切换到标题栏里的频道页
    KActiveChannelPage,

    KBeforeLeaveFunnyRoom,

	//ChannelBottomBar 业务按钮容器插入/移除widget
	KBtmBarBussContainerOper,

	//频道窗口最大化预留右侧位置宽度（RQ-16300）
	KChannelWndMaximizedReserve,
	//频道信息拉取中
	KChannelInfoLoading,
	//频道用户举报窗口
	KGetInfoFrame,
};