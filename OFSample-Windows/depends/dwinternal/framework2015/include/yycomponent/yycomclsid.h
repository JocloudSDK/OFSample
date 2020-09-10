#pragma once
#include "detail/yycomclsid.h"

/*
��������ĳ�ض�ҵ����߼���(���磺��Ϸ��Ƭ)���������ʵ�̶ֹ��Ľӿ���Ϊ����(YY.exe)���ص���ڡ�
�������ĳ���ܵĵ��߼���(���磺��ȡƵ����Ϣ)������
YY.exe�����������֮��Ĺ�ϵ��
1��YY.exe�����ṩ���������ķ����Ϊ���÷���
2�����Ҳ�����ṩ����
3��YY.exe����ݷ����������ã��ں���ʱ������������������ʵ�ֵĹ̶��ӿڣ���
4�����ͨ��YY.exe�ṩ�����÷����Լ��������ṩ�ķ����������ҵ������
5������������ã�Ҳ���Զ�̬���أ����������û��Ƕ�̬���أ���������Ա�������
6����������С��λ����������Ƿ���
7��������ķ�����ȫ��Ψһ�ģ��������ٶ�ÿ�������˵��Ψһ�ģ����������͵ķ���ͨ��root->getObject����ȡ��
��Ҳ�����������ǿ��Ա���������ģ��������ͨ��root->coCreateComInstance����
*/

/*
ͨ��root->getObject��ȡ�����÷���
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
UI container���ƣ���һ�ֽ�Ϊ����ķ���������ṩ����UI��ڵ�������ͨ��root->getObject��ȡ
*/
__declspec(selectany) extern const char KSERVICEMainFrameButtonBar[] = "com.yy.mainframebuttonbar";//��ť����ʵ��1����YY�����ڹ�����
__declspec(selectany) extern const char KSERVICEChannelChatButtonBar[] = "com.yy.channelchatbuttonbar";//��ť����ʵ��2����Ƶ�����칤����
__declspec(selectany) extern const char KSERVICEChannelToolButtonBar[] = "com.yy.channeltoolbuttonbar";//��ť����ʵ��3����Ƶ�������Ϸ�������
__declspec(selectany) extern const char KSERVICEMainTabItemContainer[] = "com.yy.maintabitemcontainer";//Item����ʵ��1���������ڵ���Tabҳ
__declspec(selectany) extern const char KSERVICEMainFrameTitleBar[] = "com.yy.mainframetitlebar";//Item����ʵ��1����������ı�����ͼ��
__declspec(selectany) extern const char KSERVICEMainFrameGroupTopBar[] = "com.yy.mainframegrouptopbar";//Item����ʵ��1�����������Ⱥ��tabҳ������λ�ã������⿪��ֻ��İ��������ҵ��ʹ��.

/*
ͨ��root->coCreateComInstance�������÷���
*/
__declspec(selectany) extern const char KSERVICEYYApplication[] = "com.yy.yyapplication";
__declspec(selectany) extern const char KSERVICEPropbag[] = "com.yy.uidpropbag";
__declspec(selectany) extern const char KSERVICEPropbagSet[] ="com.yy.propbagset";
__declspec(selectany) extern const char KSERVICEOpConnection[] = "com.yy.opconnection";	
__declspec(selectany) extern const char KSERVICEFontInfoWidget[] = "com.yy.fontinfowidget";
__declspec(selectany) extern const char KSERVICEYYFontConfig[] = "com.yy.fontconfig";	
__declspec(selectany) extern const char KSERVICEDownloader[] = "com.yy.downloader";
__declspec(selectany) extern const char KSERVICEServiceConnection[] = "com.yy.serviceconnection";	//Ƶ���ڵ�service����
__declspec(selectany) extern const char KSERVICECommonServiceConnection[] = "com.yy.commonserviceconnection";	//Ƶ�����service����
__declspec(selectany) extern const char KSERVICEImLinkdConnection[] = "com.yy.imlinkdconnection";	//imlinkd����
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
__declspec(selectany) extern const char KSERVICEUnjoinChannelInfo[] = "com.yy.unjoinchannelinfo";	// δ����Ƶ��ʱ���Ƶ����Ϣ
__declspec(selectany) extern const char KSERVICEChannelShareBroadcastHelper[] = "com.yy.channelsharebroadcasthelper";
__declspec(selectany) extern const char KSERVICEChannelComInfoFetcher[] = "com.yy.chanelcomonentinfofetcher";
__declspec(selectany) extern const char KSERVICEChannelBroadcastCenter[] = "com.yy.channelbroadcastcenter";
__declspec(selectany) extern const char KSERVICEChannelAutoBroadcastReceiver[] = "com.yy.channelautobroadcastreceiver";
__declspec(selectany) extern const char KSERVICEPropbagEx[] = "com.yy.uidpropbagex";

// ��Ч����
__declspec(selectany) extern const char KSERVICEDecorateImage[] = "com.yy.decorateimage";
__declspec(selectany) extern const char KSERVICEDecorateFlash[] = "com.yy.decorateflash";
__declspec(selectany) extern const char KSERVICEDecorateColorize[] = "com.yy.decoratecolorize";
__declspec(selectany) extern const char KSERVICEDecorateEnhence[] = "com.yy.decorateenhence";
__declspec(selectany) extern const char KSERVICEDecorateFilter[] = "com.yy.decoratefilter";
__declspec(selectany) extern const char KSERVICEDecorateLyric[] = "com.yy.decoratelyric";
__declspec(selectany) extern const char KSERVICEDecorateStretch[] = "com.yy.decoratestretch";
__declspec(selectany) extern const char KSERVICEDecorateText[] = "com.yy.decoratetext";

//��Ƶ����Ļ��׽����
__declspec(selectany) extern const char KSERVICEVideoPlayer[] = "com.yy.videoplayer";
__declspec(selectany) extern const char KSERVICEScreenCapture[] = "com.yy.screencapture";

/*
ͨ��root->getService��ȡ����ӿ�
*/
__declspec(selectany) extern const char KServiceAntiCom[] = "com.yy.anticom";
__declspec(selectany) extern const char KServiceInnerAntiCom[] = "com.yy.inneranticom";
/*
Ӧ�ú����½ӿ�
*/
__declspec(selectany) extern const char KSERVICEAddonBoxEx[] = "com.yy.channelserviceex";

//vip����
__declspec(selectany) extern const char KSERVICEYYVip[] = "com.yy.vipservice";

__declspec(selectany) extern const char KCLSIDYYStatisticInfoDownloader[] = "com.yy.statisticinfodownloader";	//ʵ������һ��checkpoint���͵����
