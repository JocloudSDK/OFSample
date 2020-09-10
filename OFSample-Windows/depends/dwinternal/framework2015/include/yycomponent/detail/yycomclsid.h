#pragma once
#include "dwbase/yycomclsid.h"

/*
大公告与linkd的通信
*/
__declspec(selectany) extern const char KSERVICEYYBigBulletinLinkd[] = "com.yy.yybigbulletinlink";	
__declspec(selectany) extern const char KSERVICEOPADDONINFO4TICKET[]	= "com.yy.opaddoninfo4ticket";

/*
UI 层 内置组件
*/

__declspec(selectany) extern const char KCOMPONENTYYChannelTicket[]="com.yy.channelticket";
__declspec(selectany) extern const char KCOMPONENTYYChannelApp[] = "com.yy.channelapp";
__declspec(selectany) extern const char KCOMPONENTYYChargeTicket[] = "com.yy.chargeticket";
__declspec(selectany) extern const char KCOMPONENTYYPrivilegeTicket[] = "com.yy.privilegeticket";
__declspec(selectany) extern const char KCOMPONENTYYGameBoxTicket[] = "com.yy.gameboxticket";
__declspec(selectany) extern const char KCOMPONENTYYChatTicket[]	= "com.yy.chatticket";
__declspec(selectany) extern const char KCOMPONENTYYChannelLogic[] = "com.yy.channelLogic";
__declspec(selectany) extern const char KCOMPONENTYYIm[] = "com.yy.im";
__declspec(selectany) extern const char KCOMPONENTYYGroup[] = "com.yy.group";
__declspec(selectany) extern const char KCOMPONENTYYImGroupApp[] = "com.yy.imgroupapp";
__declspec(selectany) extern const char KCOMPONENTYYHistoryMessage[] = "com.yy.historymessage";
__declspec(selectany) extern const char KCOMPONENTYYStatistics[] = "com.yy.statistics";
__declspec(selectany) extern const char KCOMPONENTYYDefaultTemplate[] = "com.yy.defaulttemplate";
__declspec(selectany) extern const char KCOMPONENTYYDefaultTemplateUI[] = "com.yy.defaulttemplateui";
__declspec(selectany) extern const char KCOMPONENTYYMiniLiveTemplateUI[] = "com.yy.wonderworldui";
__declspec(selectany) extern const char KCOMPONENTTemplateHandler[] = "com.yy.templatehandler";
__declspec(selectany) extern const char KCOMPONENTTobeJoinChannelUI[] = "com.yy.tobejoinchannelui";
__declspec(selectany) extern const char KCOMPONENTYYWonderWorldTicket[] = "com.yy.wonderworldticket";
__declspec(selectany) extern const char KCOMPONENTYYGroupTicket[] = "com.yy.groupticket";
__declspec(selectany) extern const char KCOMPONENTYYFunnyRoom[] = "com.yy.funnyroom"; //已下线
__declspec(selectany) extern const char KCOMPONENTYYFunnyRoomUI[] = "com.yy.funnyroomui"; //已下线
__declspec(selectany) extern const char KCOMPONENTYYFunnyRoomTicket[] = "com.yy.funnyroomticket"; //已下线
__declspec(selectany) extern const char KCOMPONENTYYNewsTicket[] = "com.yy.yynewsticket";
__declspec(selectany) extern const char KCOMPONENTYYDianhuTemplate[] = "com.yy.dianhutemp";//8.49版本下线
__declspec(selectany) extern const char KCOMPONENTYYPcentroomTemplate[] = "com.yy.pcentroom";//8.52版本下线
__declspec(selectany) extern const char KCOMPONENTYYWtabTicket[] = "com.yy.wtabticket";//同KCOMPONENTYYWonderWorldTicket

/*
提供给全频道定制模板使用的“带逻辑UI控件”
*/
__declspec(selectany) extern const char KSERVICEManagerWidget[] = "com.yy.managerwidget";
__declspec(selectany) extern const char KSERVICEMaixuWidget[] = "com.yy.maixuwidget";
__declspec(selectany) extern const char KSERVICEMessageEdit[] = "com.yy.messageedit";
__declspec(selectany) extern const char KSERVICEInputEdit[] = "com.yy.inputedit";
__declspec(selectany) extern const char KSERVICEUserTreeWidget[] = "com.yy.usertreewidget";
__declspec(selectany) extern const char KSERVICESearchWidget[] = "com.yy.searchwidget";
__declspec(selectany) extern const char KSERVICEHomePage[] = "com.yy.homepage";
__declspec(selectany) extern const char KSERVICEBulletin[] = "com.yy.bulletin";
__declspec(selectany) extern const char KSERVICESmileWidget[] = "com.yy.smilewidget";
__declspec(selectany) extern const char KSERVICENetStatusWidget[] = "com.yy.netstatuswidget";
__declspec(selectany) extern const char KSERVICEAudioSettingFrame[] = "com.yy.audiosettingframe";
__declspec(selectany) extern const char KSERVICEKaraokeSettingFrame[] = "com.yy.karaokesettingframe";
__declspec(selectany) extern const char KSERVICEPrivateChatAutoReplyFrame[] = "com.yy.privatechatautoreplyframe";
__declspec(selectany) extern const char KSERVICEChannelChatQuickReply[] = "com.yy.channelchatquickreply";
__declspec(selectany) extern const char KSERVICEInputTips[] = "com.yy.inputips";

//biz层服务，不对动态组件开放
	__declspec(selectany) extern const char KSERVICEYYBizDriver[] = "com.yy.bizdirver";
	__declspec(selectany) extern const char KSERVICEYYBizInnerDriver[] = "com.yy.bizinnerdirver";
	__declspec(selectany) extern const char KSERVICEYYBizPassport[] = "com.yy.bizpassport";
	__declspec(selectany) extern const char KSERVICEYYBizInnerPassport[] = "com.yy.bizinnerpassport";
	__declspec(selectany) extern const char KSERVICEYYBizKitFactory[] = "com.yy.bizkitfactory";

	__declspec(selectany) extern const char KCOMPONENTYYVoiceCall[] = "com.yy.voicecall";
	__declspec(selectany) extern const char KSERVICEYYVoiceCallMgr[] = "com.yy.voicecallmgr";

__declspec(selectany) extern const char KCOMPONENTBIZCore[] = "com.yy.bizcore";		//内置组件
__declspec(selectany) extern const char KCOMPONENTBIZChannel[] = "com.yy.bizchannel";	//内置组件，同时提供服务
__declspec(selectany) extern const char KCOMPONENTBIZIm[] = "com.yy.bizim";	//内置组件，同时提供服务

/*
违反设计原则，被YY主动关心的组件的名称
*/
__declspec(selectany) extern const char KRESOURCEMagicSmile[] = "com.yy.MagicSmile";		//这个比较特殊，暂不处理

	/*
	不对外开放的内部服务名称
	*/
	__declspec(selectany) extern const char KSERVICEItemContainer[] = "com.yy.itemcontainer";				//Item容器，容纳DuiGridItem
	__declspec(selectany) extern const char KSERVICEButtonContainer[] = "com.yy.buttoncontainer";		//按钮容器，用于容纳DuiPushButton
	__declspec(selectany) extern const char KSERVICEYYAudioDeviceSetting[] = "com.yy.audiodevicesetting";		
	__declspec(selectany) extern const char KSERVICEYYAudioCapture[] = "com.yy.audiocapture";
	__declspec(selectany) extern const char KSERVICEYYAudioRender[] = "com.yy.audiorender";
	__declspec(selectany) extern const char KSERVICEYYVideoDeviceSetting[] = "com.yy.videodevicesetting";//该服务与YYim视频设置相关，现抽离视频设备部分到KSERVICEYYVideoDeviceConfig服务，给组件通用
	__declspec(selectany) extern const char KSERVICEYYVideoCapture[] = "com.yy.videocapture";
	__declspec(selectany) extern const char KSERVICEYYVideoRender[] ="com.yy.videorender";
	__declspec(selectany) extern const char KSERVICEYYVideoDecoder[] ="com.yy.videodecoder";
	__declspec(selectany) extern const char KSERVICEYYVideoEncoder[] ="com.yy.videoencoder";
	__declspec(selectany) extern const char KSERVICEYYVideoJitterBufferPlayer[] ="com.yy.videojitterbufferplayer";
	__declspec(selectany) extern const char KSERVICEYYVideoJitterBuffer[] ="com.yy.vodeojitterbuffer";
	__declspec(selectany) extern const char KSERVICEYYVideoPacketBuffer[] ="com.yy.videopacketbuffer";
	__declspec(selectany) extern const char KSERVICEYYVideoHandler[] = "com.yy.videohandler";
	__declspec(selectany) extern const char KSERVICEYYVideoSender[] = "com.yy.videosender";
	__declspec(selectany) extern const char KSERVICEYYVideoReceiver[] = "com.yy.videoreceiver";
	__declspec(selectany) extern const char KSERVICEYYVideoChatProxy[] = "com.yy.videochatproxy";
	__declspec(selectany) extern const char KSERVICEYYVideoChatMgrProxy[] = "com.yy.videochatmgrproxy";
	__declspec(selectany) extern const char KSERVICEYYVideoChannelMgrProxy[] = "com.yy.videochannelmgrproxy";
	__declspec(selectany) extern const char KSERVICEYYVideoSWSCaleHelper[] = "com.yy.videoswscalehelper";
	__declspec(selectany) extern const char KSERVICEYYHistorymsgComponent[] ="com.yy.yyhistorymsgcomponent";
	__declspec(selectany) extern const char KSERVICEYYImBuddyTreeItemGameInfoHandler[] ="com.yy.yyimbuddytreeitemgameinfohandler";

	__declspec(selectany) extern const char KSERVICEYYWeiDeng[] = "com.yy.weideng";	//这是频道的一个服务，被channelservice聚合
	__declspec(selectany) extern const char KSERVICEYYOnAirCard[] = "com.yy.onaircard";	//频道的一个服务，被channelservice聚合
	__declspec(selectany) extern const char KSERVICEYYChannelFollow[] = "com.yy.channelfollow";	//频道的一个服务，被channelservice聚合
	__declspec(selectany) extern const char KSERVICEYYChannelFlower[] = "com.yy.channelflower";	//频道的一个服务，被channelservice聚合
	__declspec(selectany) extern const char KSERVICEYYGameQaButton[] = "com.yy.gameqabutton";	//imgroup的一个服务，但是在yggroup中
	__declspec(selectany) extern const char KSERVICEYYVideoRegulatory[] = "com.yy.videoregulatory";	//频道的一个服务，被channelservice聚合
    __declspec(selectany) extern const char KCOMPONENTVideoRegulatoryAnchorLiveLimit[] = "com.yy.videoregulatoryanchorlivelimit";// checkpoint类型的组件
    __declspec(selectany) extern const char KSERVICEYYChannelEnvironment[] = "com.yy.channelenvironment";
	__declspec(selectany) extern const char KSERVICEYYVideoDeviceConfig[] = "com.yy.videodeviceconfig";

		/*
	开放平台默认模版接口
	*/
	__declspec(selectany) extern const char KSERVICEOpenPlatformTemplate[] = "com.yy.openplatformtemplate";
	__declspec(selectany) extern const char KSERVICEOpenPlatformTemplateMgr[] = "com.yy.openplatformtemplatemgr";
	const char KSERVICEOpenPlatformExternalComponent[] = "com.yy.opexternalcomponent";

	//exe组件proxy
	__declspec(selectany) extern const char KCOMPONENTExeComponentProxy[] = "com.yy.execomponentproxy";