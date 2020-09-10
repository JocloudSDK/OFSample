#pragma once
#include "detail/yycomclsid.h"

/*
组件：完成某特定业务的逻辑体(例如：游戏名片)，组件必须实现固定的接口作为宿主(YY.exe)加载的入口。
服务：完成某功能的的逻辑体(例如：获取频道信息)，服务。
YY.exe、组件、服务之间的关系。
1、YY.exe可以提供服务，这样的服务成为内置服务。
2、组件也可以提供服务。
3、YY.exe会根据服务器的配置，在合适时机加载组件（调用组件实现的固定接口）。
4、组件通过YY.exe提供的内置服务，以及别的组件提供的服务来完成其业务需求。
5、组件可以内置，也可以动态下载；不管是内置还是动态下载，组件都可以被升级。
6、升级的最小单位是组件，不是服务。
7、大多数的服务是全局唯一的（或者至少对每个组件来说是唯一的），这种类型的服务通过root->getObject，获取；
但也有少数服务是可以被创建多个的，此类服务通过root->coCreateComInstance创建
*/

/*
通过root->getObject获取的内置服务
*/
__declspec(selectany) extern const char KSERVICECommon[] = "com.yy.commonservice";
__declspec(selectany) extern const char KSERVICEMainframe[] = "com.yy.mainframeservice";
__declspec(selectany) extern const char KSERVICEMainframeLogic[] = "com.yy.mainframelogicservice";
__declspec(selectany) extern const char KSERVICEChannel[] = "com.yy.channelservice";
__declspec(selectany) extern const char KSERVICEChannelLogic[] = "com.yy.channelservice";
__declspec(selectany) extern const char KSERVICEImGroup[] = "com.yy.imgroupservice";
__declspec(selectany) extern const char KSERVICEPlugin[] = "com.yy.pluginservice";
__declspec(selectany) extern const char KSERVICEOpenplatform[] = "com.yy.openplatform";
__declspec(selectany) extern const char KSERVICEGroup[] = "com.yy.groupservice";
__declspec(selectany) extern const char KSERVICEAppLogin[] = "com.yy.loginuiservice";

__declspec(selectany) extern const char KSERVICECommonComponentLevel[] = "com.yy.commoncomponentlevelservice";
__declspec(selectany) extern const char KSERVICEChannelComponentLevel[] = "com.yy.channelcomponentlevelservice";
__declspec(selectany) extern const char KSERVICEChannelLogicComponentLevel[] = "com.yy.channelcomponentlevelservice";
__declspec(selectany) extern const char KSERVICEGroupComponentLevel[] = "com.yy.groupservicecomponentlevelservice";

/*
UI container名称，是一种较为特殊的服务，向组件提供插入UI入口的能力，通过root->getObject获取
*/
__declspec(selectany) extern const char KSERVICEMainFrameButtonBar[] = "com.yy.mainframebuttonbar";//按钮容器实例1，在YY主窗口工具栏
__declspec(selectany) extern const char KSERVICEChannelChatButtonBar[] = "com.yy.channelchatbuttonbar";//按钮容器实例2，在频道聊天工具栏
__declspec(selectany) extern const char KSERVICEChannelToolButtonBar[] = "com.yy.channeltoolbuttonbar";//按钮容器实例3，在频道公告上方工具条
__declspec(selectany) extern const char KSERVICEMainTabItemContainer[] = "com.yy.maintabitemcontainer";//Item容器实例1，在主窗口第四Tab页
__declspec(selectany) extern const char KSERVICEMainFrameTitleBar[] = "com.yy.mainframetitlebar";//Item容器实例1，在主界面的标题栏图标
__declspec(selectany) extern const char KSERVICEMainFrameGroupTopBar[] = "com.yy.mainframegrouptopbar";//Item容器实例1，在主界面的群组tab页标题栏位置，不对外开放只给陌生人语聊业务使用.

/*
通过root->coCreateComInstance创建内置服务
*/
__declspec(selectany) extern const char KSERVICEYYApplication[] = "com.yy.yyapplication";
__declspec(selectany) extern const char KSERVICEPropbag[] = "com.yy.uidpropbag";
__declspec(selectany) extern const char KSERVICEPropbagSet[] ="com.yy.propbagset";
__declspec(selectany) extern const char KSERVICEOpConnection[] = "com.yy.opconnection";	
__declspec(selectany) extern const char KSERVICEFontInfoWidget[] = "com.yy.fontinfowidget";
__declspec(selectany) extern const char KSERVICEYYFontConfig[] = "com.yy.fontconfig";	
__declspec(selectany) extern const char KSERVICEDownloader[] = "com.yy.downloader";
__declspec(selectany) extern const char KSERVICEServiceConnection[] = "com.yy.serviceconnection";	//频道内的service链接
__declspec(selectany) extern const char KSERVICECommonServiceConnection[] = "com.yy.commonserviceconnection";	//频道外的service链接
__declspec(selectany) extern const char KSERVICEImLinkdConnection[] = "com.yy.imlinkdconnection";	//imlinkd链接
__declspec(selectany) extern const char KSERVICEIPCWidget[] = "com.yy.ipcwidget";
__declspec(selectany) extern const char KSERVICEIPCWidgetEx[] = "com.yy.ipcwidgetex";
__declspec(selectany) extern const char KSERVICEIPCCefWidget[] = "com.yy.ipccefwidget";
__declspec(selectany) extern const char KSERVICECompatibleHostWidget[] = "com.yy.compatiblehostwidget";
__declspec(selectany) extern const char KSERVICEPIPManager[] = "com.yy.pipmanager";
__declspec(selectany) extern const char KSERVICESpeakSetting[] = "com.yy.speaksetting";
__declspec(selectany) extern const char KSERVICENoticeTipDefaultItem[] = "com.yy.noticetipdefaultitem";
__declspec(selectany) extern const char KSERVICENoticeTipHtmlItem[] = "com.yy.noticetiphtmlitem";
__declspec(selectany) extern const char KSERVICENoticeTipSimpleItem[] = "com.yy.noticetipsimpleitem";
__declspec(selectany) extern const char KSERVICEDynamicHeadService[] = "com.yy.dynamicheadservice";
__declspec(selectany) extern const char KSERVICEHighDefinitionHeadHandlerService[] = "com.yy.highdefinitionheadhandlerservice";
__declspec(selectany) extern const char KSERVICEHighDefinitionHeadWidgetService[] = "com.yy.highdefinitionheadWidgetservice";
__declspec(selectany) extern const char KSERVICENoticeTipWidgetWrap[] = "com.yy.noticetipwidgetwrap";
__declspec(selectany) extern const char KSERVICEImageDecorator[] = "com.yy.imagedecorator";
__declspec(selectany) extern const char KSERVICEUnjoinChannelInfo[] = "com.yy.unjoinchannelinfo";	// 未进入频道时获得频道信息
__declspec(selectany) extern const char KSERVICEChannelShareBroadcastHelper[] = "com.yy.channelsharebroadcasthelper";
__declspec(selectany) extern const char KSERVICEChannelComInfoFetcher[] = "com.yy.chanelcomonentinfofetcher";
__declspec(selectany) extern const char KSERVICEChannelBroadcastCenter[] = "com.yy.channelbroadcastcenter";
__declspec(selectany) extern const char KSERVICEChannelAutoBroadcastReceiver[] = "com.yy.channelautobroadcastreceiver";
__declspec(selectany) extern const char KSERVICEPropbagEx[] = "com.yy.uidpropbagex";

// 特效对象
__declspec(selectany) extern const char KSERVICEDecorateImage[] = "com.yy.decorateimage";
__declspec(selectany) extern const char KSERVICEDecorateFlash[] = "com.yy.decorateflash";
__declspec(selectany) extern const char KSERVICEDecorateColorize[] = "com.yy.decoratecolorize";
__declspec(selectany) extern const char KSERVICEDecorateEnhence[] = "com.yy.decorateenhence";
__declspec(selectany) extern const char KSERVICEDecorateFilter[] = "com.yy.decoratefilter";
__declspec(selectany) extern const char KSERVICEDecorateLyric[] = "com.yy.decoratelyric";
__declspec(selectany) extern const char KSERVICEDecorateStretch[] = "com.yy.decoratestretch";
__declspec(selectany) extern const char KSERVICEDecorateText[] = "com.yy.decoratetext";

//视频及屏幕捕捉对象
__declspec(selectany) extern const char KSERVICEVideoPlayer[] = "com.yy.videoplayer";
__declspec(selectany) extern const char KSERVICEScreenCapture[] = "com.yy.screencapture";

/*
通过root->getService获取组件接口
*/
__declspec(selectany) extern const char KServiceAntiCom[] = "com.yy.anticom";
__declspec(selectany) extern const char KServiceInnerAntiCom[] = "com.yy.inneranticom";
/*
应用盒子新接口
*/
__declspec(selectany) extern const char KSERVICEAddonBoxEx[] = "com.yy.channelserviceex";

//vip服务
__declspec(selectany) extern const char KSERVICEYYVip[] = "com.yy.vipservice";

__declspec(selectany) extern const char KCLSIDYYStatisticInfoDownloader[] = "com.yy.statisticinfodownloader";	//实际上是一个checkpoint类型的组件
