/** 
@file
@brief 
@version 
*/
#pragma once

//Log ������(filter)ͳһ�����ﶨ�壬���ڷ������
namespace LOG
{
	__declspec(selectany) extern const char KQT[]					= "QT";					//QT�ڲ�����
	__declspec(selectany) extern const char KLogin[]					= "Login";				//��¼
	__declspec(selectany) extern const char KChannelList[]			= "ChannelList";		//������Ƶ���б�
	__declspec(selectany) extern const char KChannel[]				= "Channel";			//Ƶ������
	__declspec(selectany) extern const char KChannelTime[]				= "ChannelTime";
	__declspec(selectany) extern const char KChannelAudio[]				= "ChannelAudio";
	__declspec(selectany) extern const char KMaixuList[]				= "MaixuList";			//�����б�
	__declspec(selectany) extern const char KChannelUserList[]		= "ChannelUserList";	//Ƶ���û��б�
	__declspec(selectany) extern const char KChannelEchoMsg[]		= "ChannelEchoMsg";		//Ƶ������ʾ��Ϣ
	__declspec(selectany) extern const char KYYRun[]					= "YYRun";				//yyrun
	__declspec(selectany) extern const char KDuiFw[]					= "DuiFw";
	__declspec(selectany) extern const char KMainframe[]				= "Mainframe";
	__declspec(selectany) extern const char KAddon[]					= "Addon";
	__declspec(selectany) extern const char KHttp[]					= "Http";
	__declspec(selectany) extern const char KCommonSlot[]			= "CommonSlot";
	__declspec(selectany) extern const char KYYGroup[]			    = "YYGroup"; // yygrou����
	__declspec(selectany) extern const char KYYIm[]			        = "YYIm"; // yyim����
	__declspec(selectany) extern const char KGroup[]					= "Group";
	__declspec(selectany) extern const char KIm[]					= "Im";
    __declspec(selectany) extern const char KUpAndDownLoad[]			= "UpAndDownLoad"; // �ϴ�����
	__declspec(selectany) extern const char KMsgHistroy[]			= "KMsgHistory"; // ��Ϣ��ʷ
	__declspec(selectany) extern const char KVoiceMsg[]				= "VoiceMsg"; //������Ϣ 
	__declspec(selectany) extern const char KImMsg[]					= "ImMsg"; //������Ϣ 
    __declspec(selectany) extern const char KSmile[]					= "Smile"; //���� 
	__declspec(selectany) extern const char KIPC[]					= "IPC";	//����WM_COPYDATA��IPC
	__declspec(selectany) extern const char KAxhost[]				= "YYAxhost";	//axhost���
	__declspec(selectany) extern const char KDuiEditor[]             = "DuiEditor";  //Ƥ���༭����DuiEditor
	__declspec(selectany) extern const char KUpdate[]				= "DWUpdate";	//dwupdate���
	__declspec(selectany) extern const char KIMApp[]				    = "YYIMApp";	//yyimgroupapp����
	__declspec(selectany) extern const char KChannelSendImage[]		= "ChannelSendImage"; //Ƶ����ͼƬ
	__declspec(selectany) extern const char KStatistic[]				= "YYStatistic";
    __declspec(selectany) extern const char KOpenplatform[]			= "OpenPlatform";	//����ƽ̨
	__declspec(selectany) extern const char KYYQustion[]				= "KYYQustion"; //YY��ʱ�ʴ�
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
	__declspec(selectany) extern const char KVipMsg[]				= "KVipMsg";//��Ա��Ϣ
	__declspec(selectany) extern const char KBangDaiMsg[]			= "KBangDaiMsg";//ʦͽ���֪ͨ��Ϣ
	__declspec(selectany) extern const char KDataReport[]			= "DataReport";	//�����ϱ�
	__declspec(selectany) extern const char KBizDriver[]				= "BizDriver";
	__declspec(selectany) extern const char KImp2pVideo[]			= "P2PVideo";//1��1��Ƶ
    __declspec(selectany) extern const char KComponent[]             = "Component";
    __declspec(selectany) extern const char KVoiceCall[]             = "VoiceCall"; //1v1 audio module
    __declspec(selectany) extern const char KFatalException[]        = "FatalException"; //Fatal Exception Report
    __declspec(selectany) extern const char KAudioEngine[]           = "AudioEngine";
	__declspec(selectany) extern const char KBrowserInstance[]		= "BrowserInstance";
	__declspec(selectany) extern const char KRegulatory[]		= "Regulatory"; //�����׾ٱ�
	__declspec(selectany) extern const char KPreloadExternal[]	= "PreloadExternal";
	__declspec(selectany) extern const char KConfigCenter[]	= "ConfigCenter";
	__declspec(selectany) extern const char KCrashTrace[] = "CrashTrace";
	__declspec(selectany) extern const char KChannelChat[] = "ChannelChat";//Ƶ������&˽��
};
