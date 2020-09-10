/** 
@file
@brief 
@version 
*/
#pragma once

//Log 过滤器(filter)统一在这里定义，便于分类管理
namespace LOG
{
	__declspec(selectany) extern const char KQT[]					= "QT";					//QT内部错误
	__declspec(selectany) extern const char KLogin[]					= "Login";				//登录
	__declspec(selectany) extern const char KChannelList[]			= "ChannelList";		//主界面频道列表
	__declspec(selectany) extern const char KChannel[]				= "Channel";			//频道功能
	__declspec(selectany) extern const char KChannelTime[]				= "ChannelTime";
	__declspec(selectany) extern const char KChannelAudio[]				= "ChannelAudio";
	__declspec(selectany) extern const char KMaixuList[]				= "MaixuList";			//麦序列表
	__declspec(selectany) extern const char KChannelUserList[]		= "ChannelUserList";	//频道用户列表
	__declspec(selectany) extern const char KChannelEchoMsg[]		= "ChannelEchoMsg";		//频道回显示消息
	__declspec(selectany) extern const char KYYRun[]					= "YYRun";				//yyrun
	__declspec(selectany) extern const char KDuiFw[]					= "DuiFw";
	__declspec(selectany) extern const char KMainframe[]				= "Mainframe";
	__declspec(selectany) extern const char KAddon[]					= "Addon";
	__declspec(selectany) extern const char KHttp[]					= "Http";
	__declspec(selectany) extern const char KCommonSlot[]			= "CommonSlot";
	__declspec(selectany) extern const char KYYGroup[]			    = "YYGroup"; // yygrou工程
	__declspec(selectany) extern const char KYYIm[]			        = "YYIm"; // yyim工程
	__declspec(selectany) extern const char KGroup[]					= "Group";
	__declspec(selectany) extern const char KIm[]					= "Im";
    __declspec(selectany) extern const char KUpAndDownLoad[]			= "UpAndDownLoad"; // 上传下载
	__declspec(selectany) extern const char KMsgHistroy[]			= "KMsgHistory"; // 消息历史
	__declspec(selectany) extern const char KVoiceMsg[]				= "VoiceMsg"; //语音消息 
	__declspec(selectany) extern const char KImMsg[]					= "ImMsg"; //语音消息 
    __declspec(selectany) extern const char KSmile[]					= "Smile"; //表情 
	__declspec(selectany) extern const char KIPC[]					= "IPC";	//基于WM_COPYDATA的IPC
	__declspec(selectany) extern const char KAxhost[]				= "YYAxhost";	//axhost相关
	__declspec(selectany) extern const char KDuiEditor[]             = "DuiEditor";  //皮肤编辑工具DuiEditor
	__declspec(selectany) extern const char KUpdate[]				= "DWUpdate";	//dwupdate相关
	__declspec(selectany) extern const char KIMApp[]				    = "YYIMApp";	//yyimgroupapp工程
	__declspec(selectany) extern const char KChannelSendImage[]		= "ChannelSendImage"; //频道发图片
	__declspec(selectany) extern const char KStatistic[]				= "YYStatistic";
    __declspec(selectany) extern const char KOpenplatform[]			= "OpenPlatform";	//开放平台
	__declspec(selectany) extern const char KYYQustion[]				= "KYYQustion"; //YY及时问答
	__declspec(selectany) extern const char KDataCenter[]			= "DataCenter";			//Data Center Module
	__declspec(selectany) extern const char KProtocolNet[]			= "ProtocolNet";		//Protocol Session Module
	__declspec(selectany) extern const char KProtocolSession[]		= "ProtocolSession";	//Protocol Session Module
	__declspec(selectany) extern const char KProtocolService[]		= "ProtocolService";	//Protocol Service Module
	__declspec(selectany) extern const char KProtocolLogin[]			= "ProtocolLogin";		//Protocol Login Module
    __declspec(selectany) extern const char KProtocolMgr[]           = "ProtocolMgr";        //Protocol Manager Module
	__declspec(selectany) extern const char KProtocolSInfo[]			= "ProtocolSInfo";
	__declspec(selectany) extern const char KProtocolAudio[]			= "ProtocolAudio";
	__declspec(selectany) extern const char KProtocolVideo[]			= "ProtocolVideo";
    __declspec(selectany) extern const char KProtocolBase[]          = "ProtocolBase";       //Protocol Base Module
	__declspec(selectany) extern const char KOfflineMsg[]			= "KOfflineMsg";
	__declspec(selectany) extern const char KVipMsg[]				= "KVipMsg";//会员消息
	__declspec(selectany) extern const char KBangDaiMsg[]			= "KBangDaiMsg";//师徒帮带通知消息
	__declspec(selectany) extern const char KDataReport[]			= "DataReport";	//数据上报
	__declspec(selectany) extern const char KBizDriver[]				= "BizDriver";
	__declspec(selectany) extern const char KImp2pVideo[]			= "P2PVideo";//1对1视频
    __declspec(selectany) extern const char KComponent[]             = "Component";
    __declspec(selectany) extern const char KVoiceCall[]             = "VoiceCall"; //1v1 audio module
    __declspec(selectany) extern const char KFatalException[]        = "FatalException"; //Fatal Exception Report
    __declspec(selectany) extern const char KAudioEngine[]           = "AudioEngine";
	__declspec(selectany) extern const char KBrowserInstance[]		= "BrowserInstance";
	__declspec(selectany) extern const char KRegulatory[]		= "Regulatory"; //反低俗举报
	__declspec(selectany) extern const char KPreloadExternal[]	= "PreloadExternal";
	__declspec(selectany) extern const char KConfigCenter[]	= "ConfigCenter";
	__declspec(selectany) extern const char KCrashTrace[] = "CrashTrace";
	__declspec(selectany) extern const char KChannelChat[] = "ChannelChat";//频道公屏&私聊
};
