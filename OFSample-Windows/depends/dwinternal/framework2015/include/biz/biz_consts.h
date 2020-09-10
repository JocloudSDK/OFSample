
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//info flags

__declspec(selectany) extern const UINT32 INFOFLAG_MINI		= 0xFF000000;
__declspec(selectany) extern const UINT32 INFOFLAG_SIMPLE	= 0xFF0000FF;
__declspec(selectany) extern const UINT32 INFOFLAG_DETAIL	= 0xFF00FFFF;
__declspec(selectany) extern const UINT32 INFOFLAG_ALL		= 0xFFFFFFFF;







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//prop names
 
//msghistory 
__declspec(selectany) extern const LPCWSTR SYSTEM_MSG_ID					= L"YYSYS_@U";
__declspec(selectany) extern const LPCWSTR SYSTEM_MSG_NICK				= L"ϵͳ��Ϣ";
__declspec(selectany) extern const LPCWSTR LOCAL_MESSAGE_GUID			= L"999999"; 
__declspec(selectany) extern const LPCWSTR RECOMMEND_MSG_ID				= L"YYRECC_@U";
#define MultiTalkToId				(unsigned int)-1
#define SERVER_MultiTLAK_INT		2147483647  //�������˵Ķ��˻Ự��touID�ķ���ֵ
#define PAGE_SIZE					60

//////////////////////////////////////////////////////////////////////////
__declspec(selectany) extern const UINT32 KLargeGroupMember				= 5000;
__declspec(selectany) extern const UINT32 KLargeGroupMemberStatusLimit	= 100;
__declspec(selectany) extern const UINT32 KLargeGroupMemberSearchLimit  = 500;

//user prop
//mini
__declspec(selectany) extern const LPCWSTR UPROP_Passport = L"passport";
__declspec(selectany) extern const LPCWSTR UPROP_Account = L"account"; //this is email
__declspec(selectany) extern const LPCWSTR UPROP_Uid = L"uid"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_Imid = L"imid"; // uint32_t m_uImId;//yy�̺ţ�û�о���-1
__declspec(selectany) extern const LPCWSTR UPROP_Nick = L"nick";
// 
__declspec(selectany) extern const LPCWSTR UPROP_ArtistName = L"artistname";// ����
//simple
__declspec(selectany) extern const LPCWSTR UPROP_Gender = L"gender"; //GENDER
__declspec(selectany) extern const LPCWSTR UPROP_Sign = L"sign";
__declspec(selectany) extern const LPCWSTR UPROP_Remark = L"remark"; // ��ע
__declspec(selectany) extern const LPCWSTR UPROP_Jifen = L"jifen"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_JifenInChannel = L"jifenInChannel"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_Level = L"level"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_RoleInChannel = L"roleInChannel"; //ROLE_IN_CHANNEL
__declspec(selectany) extern const LPCWSTR UPROP_EnableTextInChannel = L"enableTextInChannel"; //bool
__declspec(selectany) extern const LPCWSTR UPROP_EnableVoiceInChannel = L"enableVoiceInChannel"; //bool
__declspec(selectany) extern const LPCWSTR UPROP_ImStatus = L"imStatus"; //IM_STATUS
__declspec(selectany) extern const LPCWSTR UPROP_HeadPhotoIndex = L"headPhotoIndex"; // uint32_t m_uHeadPhoto;//ͷ��,û�о���-1
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL = L"customHeadPhotoURL"; //60*60ͷ��Url
__declspec(selectany) extern const LPCWSTR UPROP_CustomLocalHeadPhotoURL = L"customLocalHeadPhotoURL"; //���ص�ͷ��·��
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL_100_100 = L"customHeadPhotoURL_100_100"; //100*100ͷ��Url
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL_144_144 = L"customHeadPhotoURL_144_144"; //140*140ͷ��Url
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL_640_640 = L"customHeadPhotoURL_640_640"; //640*640ͷ��Url
__declspec(selectany) extern const LPCWSTR UPROP_BIRTHDAY = L"birthday";	// uint32_t m_uBirthDay;
__declspec(selectany) extern const LPCWSTR UPROP_PROVINCE = L"PROVINCE";	// uint32_t m_uProvince;
__declspec(selectany) extern const LPCWSTR UPROP_CITY = L"CITY";			// uint32_t m_uCity;
__declspec(selectany) extern const LPCWSTR UPROP_VERSION = L"VERSION";		// uint32_t m_uVersion;
__declspec(selectany) extern const LPCWSTR UPROP_INTRO = L"INTRO";			// UniString m_strIntro;
__declspec(selectany) extern const LPCWSTR UPROP_AREA = L"AREA";			// uint32_t m_uArea;
__declspec(selectany) extern const LPCWSTR UPROP_DEVICE = L"DEVICE";		// uint32_t 0:PC,1�ֻ��豸;
__declspec(selectany) extern const LPCWSTR UPROP_DEVICE_PHONE = L"DEVICE_PHONE";		// uint32_t 0:PC,1�ֻ��豸;
__declspec(selectany) extern const LPCWSTR UPROP_DEVICE_APP = L"DEVICE_APP"; // IMAPPLICATIONTYPE
__declspec(selectany) extern const LPCWSTR UPROP_CORPS = L"CORPS";		// ������
__declspec(selectany) extern const LPCWSTR UPROP_CORPS_POSITION = L"CORPSPOSITION";	//����ְλ

// ��˿���ԣ���App�õ�
__declspec(selectany) extern const LPCWSTR UPROP_FANS_UID = L"uid";
__declspec(selectany) extern const LPCWSTR UPROP_FANS_Nick = L"nick";
__declspec(selectany) extern const LPCWSTR UPROP_FANS_Photo = L"photo";
__declspec(selectany) extern const LPCWSTR UPROP_FANS_vip = L"vip";

//�ָ���������
__declspec(selectany) extern const LPCWSTR UPROP_OPERATOR_UID = L"operatorUid";
__declspec(selectany) extern const LPCWSTR UPROP_OPERATION_TIME = L"operationTime";

//detail
__declspec(selectany) extern const LPCWSTR UPROP_Intro = L"intro";
__declspec(selectany) extern const LPCWSTR UPROP_Birthday = L"birtyday"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_Age = L"age"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_CountryId = L"countryId"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_ProvinceId = L"provinceId"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_CityId = L"cityId"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_IsNewUser = L"isNewUser";
//vip��Ϣ

__declspec(selectany) extern const LPCWSTR UPROP_VipGrade = L"vipgrade";
__declspec(selectany) extern const LPCWSTR UPROP_VipType = L"viptype";
__declspec(selectany) extern const LPCWSTR UPROP_pdGrade = L"PD_grade";  // 0 �������Ա  1 ~ 7
__declspec(selectany) extern const LPCWSTR UPROP_pdType = L"PD_type";    // 0 �����·�  1 �������
__declspec(selectany) extern const LPCWSTR UPROP_pdUpgradeTime = L"PD_upgrade_time"; // ��������ʱ��
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviNick = L"PD_privi_nick";     // true��ʾ������ɫ�ǳ�
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviSuper = L"PD_privi_super";   // true��ʾ��������
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviExvip = L"PD_privi_exvip";   // true��ʾ�����Ա
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviSVip = L"PD_privi_svip";    // true��ʾ������Ա

//�û���չ��Ϣ
__declspec(selectany) extern const LPCWSTR UPROP_PID = L"pid";
__declspec(selectany) extern const LPCWSTR UPROP_VIP_REDNAME = L"vipRedName";
__declspec(selectany) extern const LPCWSTR UPROP_VIP_FACE = L"vipFace";
__declspec(selectany) extern const LPCWSTR UPROP_VIP_LEVEL = L"vipLevel";
__declspec(selectany) extern const LPCWSTR UPROP_ACTIVE = L"active";
__declspec(selectany) extern const LPCWSTR UPROP_GAMMING= L"gamming";
__declspec(selectany) extern const LPCWSTR UPROP_IsVip= L"isVip";
__declspec(selectany) extern const LPCWSTR UPROP_lianghao = L"vipLiangHao";
__declspec(selectany) extern const LPCWSTR UPROP_ExtraInfo = L"extraInfo";

//channel prop
//join info
__declspec(selectany) extern const LPCWSTR CPROP_Displayid = L"displayid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_UserData = L"user_data";  // string
__declspec(selectany) extern const LPCWSTR CPROP_ShowTop = L"ch_show_top"; // bool
__declspec(selectany) extern const LPCWSTR CPROP_JoinStyle = L"ch_join_style"; // int
__declspec(selectany) extern const LPCWSTR CPROP_ActiveWindow = L"ch_active_window"; // bool

//mini
__declspec(selectany) extern const LPCWSTR CPROP_Sid = L"sid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_Ssid = L"ssid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_Name = L"name";
//simple
__declspec(selectany) extern const LPCWSTR CPROP_Order = L"order"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_HasPassword = L"hasPassword"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_ChannelStyle = L"channelStyle"; //CHANNEL_STYLE
__declspec(selectany) extern const LPCWSTR CPROP_Jifen = L"jifen"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_GuildLogoIndex = L"guildLogoIndex"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_GuildCustomLogoURL = L"guildCustomLogoURL";
__declspec(selectany) extern const LPCWSTR CPROP_Area = L"area";
//detail
__declspec(selectany) extern const LPCWSTR CPROP_GuildType = L"guildType"; //GUILD_TYPE
__declspec(selectany) extern const LPCWSTR CPROP_GuildMainGame = L"guildMainGame";//
__declspec(selectany) extern const LPCWSTR CPROP_GuildGameServer = L"guildGameServer";
__declspec(selectany) extern const LPCWSTR CPROP_IsLimit = L"isLimit"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_IsPub = L"isPub"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_HideShare = L"hideShare";  // bool
__declspec(selectany) extern const LPCWSTR CPROP_VoiceQuality = L"voiceQuality"; //VOICE_QUALITY
__declspec(selectany) extern const LPCWSTR CPROP_Bulletin = L"bulletin";
__declspec(selectany) extern const LPCWSTR CPROP_BulletinTimeStamp = L"bulletinTimeStamp";
__declspec(selectany) extern const LPCWSTR CPROP_MemberCountLimit = L"memberCountLimit"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_PaiMing = L"paiMing"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_Intro = L"intro";
__declspec(selectany) extern const LPCWSTR CPROP_MaixuTime = L"maixuTime"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsLimitText = L"isLimitText"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_TextLimitTime = L"textLimitText"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_GuestMaxTextLen = L"guestMaxTextLen"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsAuthenticated = L"authenticated"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_BuySessionId = L"channelBuySessId"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_CreditValue = L"creditValue"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_CanGuestJoinMaixu = L"canGuestJoinMaixu"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_GuestWaitTime = L"guestWaitTime"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsGuestLimitEnabled = L"isGuestLimitEnabled"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_IsLimitTextOnlyForGuest = L"isLimitTextOnlyForGuest"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_CanCurSubChannelSendImage = L"canCurSubChannelSendImage"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_IsRoleChangeNotice = L"isRoleChangeNotice"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_IsShowBroadcast = L"isShowBroadcast"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_IsLimitGuestJoin = L"isLimitGuestJoin"; // bool
__declspec(selectany) extern const LPCWSTR CPROP_CanGuestVoiceChat = L"canGuestVoiceChat"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_CanSendUrlText = L"canSendUrlText"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_CanMemberSendUrlText = L"canMemberSendUrlText"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_MobilePhoneBinding = L"mobilePhoneBinding";//bool
__declspec(selectany) extern const LPCWSTR CPROP_CanPrivateChat = L"canPrivateChat";//bool
__declspec(selectany) extern const LPCWSTR CPROP_CanGuestAuth = L"canGuestAuth";//bool
__declspec(selectany) extern const LPCWSTR CPROP_IsLimitSeeNAddFriends = L"isLimitSeeNAddFriends";//bool
__declspec(selectany) extern const LPCWSTR CPROP_AutoJieDai = L"autoJieDai";//bool
__declspec(selectany) extern const LPCWSTR CPROP_SayJieDai = L"sayJieDai";//text
__declspec(selectany) extern const LPCWSTR CPROP_EnrollmentJifenLimit = L"enrollmentJifenLimit"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_EnrollmentNotice = L"enrollmentNotice";
__declspec(selectany) extern const LPCWSTR CPROP_CanGuestAccessTop = L"canGuestAccessTop"; //bool
__declspec(selectany) extern const LPCWSTR CPROP_IspType = L"ispType"; //ISP_TYPE
__declspec(selectany) extern const LPCWSTR CPROP_templateId = L"templateId"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_ComplaintInfo = L"complaintInfo";//string
__declspec(selectany) extern const LPCWSTR CPROP_SessionCardPrefix = L"sessionCardPrefix";
__declspec(selectany) extern const LPCWSTR CPROP_SessionCardSuffix = L"sessionCardSuffix";
__declspec(selectany) extern const LPCWSTR CPROP_RemarkName = L"remarkName";//string
__declspec(selectany) extern const LPCWSTR CPROP_CollectedType = L"collectedType";//UINT32
__declspec(selectany) extern const LPCWSTR CPROP_HideLiveRecommend = L"hideLiveRecommendVisible"; // bool
__declspec(selectany) extern const LPCWSTR CPROP_BroadcastLimit = L"broadcastLimit"; // bool
__declspec(selectany) extern const LPCWSTR CPROP_BroadcastLimitValue = L"broadcastLimitValue"; // uint32
__declspec(selectany) extern const LPCWSTR CPROP_GuestPrivateChatWaitTime = L"guestPrivateChatWaitTime"; //UINT32 �οͽ���ƴ��������˽�ĵȴ�ʱ�䣨topChannel��
__declspec(selectany) extern const LPCWSTR CPROP_MgrRejectGuestPrivateChat = L"bMgrRejectGuestPrivateChat";//�������Ϲ���ԱĬ�ϲ�����˽�ģ�topChannel��
__declspec(selectany) extern const LPCWSTR CPROP_IsHideVisitorTextNWaitTime = L"bIsHidVisitorTextLimitation";//bool
//only for channelList/qChannelList/catalogChannelList/...
__declspec(selectany) extern const LPCWSTR CPROP_Asid = L"asid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_TopSid = L"topSid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_Password = L"password";
__declspec(selectany) extern const LPCWSTR CPROP_Hello = L"hello";
__declspec(selectany) extern const LPCWSTR CPROP_OnlineUserCount = L"onlineUserCount"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsEnableBubble = L"isEnableBubble";//bool��8.49�汾���߹���
__declspec(selectany) extern const LPCWSTR GPROP_AutoDiversionSetting = L"autoDiversionSetting"; //UINT32
__declspec(selectany) extern const LPCWSTR GPROP_AutoBroadcastSetting = L"autoBroadcastSetting"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsPinTop = L"isPinTop"; //Ƶ������б��У��Ƿ��ö�, bool
__declspec(selectany) extern const LPCWSTR CPROP_IsGifLogo = L"isGifLogo"; //Ƶ������б��У��Ƿ�gifͷ��, bool
//medal info
__declspec(selectany) extern const LPCWSTR CPROP_IsMedalVioletGolenSession = L"isMedalVioletGolenSession";
__declspec(selectany) extern const LPCWSTR CPROP_MedalVioletLogoUrl = L"medalVioletLogoUrl";
__declspec(selectany) extern const LPCWSTR CPROP_MedalName = L"medalName";
__declspec(selectany) extern const LPCWSTR CPROP_MedalDescription = L"medalDescription";
__declspec(selectany) extern const LPCWSTR CPROP_MedalLinkUrl = L"medalLinkUrl";
__declspec(selectany) extern const LPCWSTR CPROP_MedalID = L"medalID";
__declspec(selectany) extern const LPCWSTR CPROP_MedalGrade = L"medalGrade";

//channel error info
__declspec(selectany) extern const LPCWSTR CPROP_UnfreezenTime = L"unfreezenTime";
__declspec(selectany) extern const LPCWSTR CPROP_UnlockTime = L"unlockTime";
__declspec(selectany) extern const LPCWSTR CPROP_ErrorCode = L"errorCode";
__declspec(selectany) extern const LPCWSTR CPROP_RetryTime = L"retryTime";
__declspec(selectany) extern const LPCWSTR CPROP_ServerBusyErrCode = L"serverBusyCode";
__declspec(selectany) extern const LPCWSTR CPROP_ErrorHandled = L"ErrorHandled";    // bool
__declspec(selectany) extern const LPCWSTR CPROP_ErrInfo = L"ErrInfo"; //string

// channel chat text result extra info
__declspec(selectany) extern const LPCWSTR CPROP_AccessWaittingTime     = L"1"; // ��Ƶ������Ҫ�ȴ�N�뷢��
__declspec(selectany) extern const LPCWSTR CPROP_IntervalLeftTime       = L"2"; //���´η��Ի���Ҫ�ȴ�N��
__declspec(selectany) extern const LPCWSTR CPROP_TextLimitLength        = L"3"; //�ı�������಻����N����
__declspec(selectany) extern const LPCWSTR CPROP_FilterLimitReason      = L"4"; //������ϵͳ����ԭ��
__declspec(selectany) extern const LPCWSTR CPROP_GlobalFilterRescode    = L"5"; //������ϵͳ���˷�����
__declspec(selectany) extern const LPCWSTR CPROP_FormatedErrorString    = L"formated_error_string";// string, ���ظ�ʽ���õĴ�������

__declspec(selectany) extern const LPCWSTR FPROP_FolderType = L"folderType"; //FOLDER_TYPE

//specify info, 
__declspec(selectany) extern const LPCWSTR CPROP_FrontEndIP = L"frontEndIP";
__declspec(selectany) extern const LPCWSTR CPROP_SegRttInfoKey = L"SegRttInfoKey";
__declspec(selectany) extern const LPCWSTR CPROP_SegRttInfoValue = L"SegRttInfoValue";
__declspec(selectany) extern const LPCWSTR CPROP_VoicePkgSrcIP = L"VoicePkgSrcIP";

//Login error info
__declspec(selectany) extern const LPCWSTR LPROP_LoginType = L"loginType";// uint32
__declspec(selectany) extern const LPCWSTR LPROP_StategyType = L"stategyType";// int32
__declspec(selectany) extern const LPCWSTR LPROP_BanType = L"banType";
__declspec(selectany) extern const LPCWSTR LPROP_BanTime = L"banTime";
__declspec(selectany) extern const LPCWSTR LPROP_BanReason = L"banReason";
__declspec(selectany) extern const LPCWSTR LPROP_UnbanTime = L"unbanTime";//string
__declspec(selectany) extern const LPCWSTR LPROP_FreezenType = L"freezenType";//KickType
__declspec(selectany) extern const LPCWSTR LPROP_UnfreezenTime = L"unfreezenTime";//time_t value
__declspec(selectany) extern const LPCWSTR LPROP_ReconnectWaitTime = L"reconnectWaitTime";//UINT32 second
__declspec(selectany) extern const LPCWSTR LPROP_RealLoginFail = L"realLoginFail";
__declspec(selectany) extern const LPCWSTR LPROP_LoginFailType = L"loginFailType";
__declspec(selectany) extern const LPCWSTR LPROP_LoginTimeout = L"loginTimeout";
__declspec(selectany) extern const LPCWSTR LPROP_ErrMsg = L"loginErrMsg"; // json data string
__declspec(selectany) extern const LPCWSTR LPROP_ErrCode = L"loginErrCode"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_BtnName = L"banButtonName"; // string
__declspec(selectany) extern const LPCWSTR LPROP_BtnClick = L"banButtonClick"; // string
__declspec(selectany) extern const LPCWSTR LPROP_BanVer = L"banVersion"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_PicId = L"picId"; // string
__declspec(selectany) extern const LPCWSTR LPROP_PicCode = L"picCode"; // string
__declspec(selectany) extern const LPCWSTR LPROP_VeriReason = L"verifyReason"; // string
__declspec(selectany) extern const LPCWSTR LPROP_VerResCode = L"verifyResCode"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_PicCodeContext = L"picCodeContext"; // std::vector<std::string>
__declspec(selectany) extern const LPCWSTR LPROP_PicCodeDescrip = L"picCodeDescription"; // string
__declspec(selectany) extern const LPCWSTR LPROP_ForceOut_Res = L"forceOutResason"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_ForceOut_Desc = L"forceOutDescription"; // string
__declspec(selectany) extern const LPCWSTR LPROP_RefreshSms_Res = L"refreshSmsRes"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_RefreshSms_Reason = L"refreshSmsReason"; // string
__declspec(selectany) extern const LPCWSTR LPROP_RefreshSms_Desc = L"refreshSmsDescription"; // string
__declspec(selectany) extern const LPCWSTR LPROP_RefreshSms_DescContent = L"refreshSmsDescriptionContent"; // string
__declspec(selectany) extern const LPCWSTR LPROP_PicCodePromptTitle = L"picCodePromptTitle"; // string
__declspec(selectany) extern const LPCWSTR LPROP_VerifyBox_Height = L"verifyBoxHeight"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_VerifyBox_Width = L"verifyBoxWidth"; // uint32
__declspec(selectany) extern const LPCWSTR LPROP_Kickoff_ChannelDel = L"kickoffByChannelDel"; // 0/1


// group search
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_UID = L"gsUid";				// uint32
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_IMID = L"gsImid";			// uint32
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_GID = L"gsGid";				// uint32
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_GCARD = L"gsGCard";			// wstring
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_GENDER = L"gsGender";		// uint32
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_INFOTYPE = L"gsInfoType";	// uint32
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_NICKNAME = L"gsNickName";	// wstring
__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_GNICK = L"gsGNick";			// wstring
//__declspec(selectany) extern const LPCWSTR GROUP_SEARCH_PROP_ = L"gs";

//guild prop
__declspec(selectany) extern const LPCWSTR GUILDPROP_ChannelSid = CPROP_Sid;
__declspec(selectany) extern const LPCWSTR GUILDPROP_ChannelAsid = CPROP_Asid;
__declspec(selectany) extern const LPCWSTR GUILDPROP_ChannelName = CPROP_Name;
__declspec(selectany) extern const LPCWSTR GUILDPROP_UserGender = UPROP_Gender;
__declspec(selectany) extern const LPCWSTR GUILDPROP_UserRoleInChannel = UPROP_RoleInChannel;
__declspec(selectany) extern const LPCWSTR GUILDPROP_UserJifenInChannel = UPROP_JifenInChannel;
__declspec(selectany) extern const LPCWSTR GUILDPROP_IsSettled = L"isSettled"; //bool
__declspec(selectany) extern const LPCWSTR GUILDPROP_ClearJifenTime = L"clearJifenTime"; //UINT32
__declspec(selectany) extern const LPCWSTR GUILDPROP_QuitTime = L"quitTime"; //UINT32
__declspec(selectany) extern const LPCWSTR GUILDPROP_JoinReason = L"joinReason";
__declspec(selectany) extern const LPCWSTR GUILDPROP_IsInitiativeQuit = L"isInitiativeQuit";
__declspec(selectany) extern const LPCWSTR GUILDPROP_AdminUid = L"adminUid";
__declspec(selectany) extern const LPCWSTR GUILDPROP_AdminNick = L"adminNick";
__declspec(selectany) extern const LPCWSTR GUILDPROP_NetStatus = L"netStatus";
__declspec(selectany) extern const LPCWSTR GUILDPROP_OwVip = L"owvip";
//group prop
//simple
__declspec(selectany) extern const LPCWSTR GPROP_Gid = L"gid"; //UINT32
__declspec(selectany) extern const LPCWSTR GPROP_GName = L"gname";	// Ⱥ��Ƭ�ǳ�
__declspec(selectany) extern const LPCWSTR GPROP_Name = L"name";		// �û��ǳ�
__declspec(selectany) extern const LPCWSTR GPROP_GuildLogoIndex = L"guildLogoIndex"; //Ⱥͷ������ UINT32
__declspec(selectany) extern const LPCWSTR GPROP_ModifyGroupInfoUserId = L"modifyGroupInfoUserId"; //�޸�Ⱥ��Ϣ�Ĳ����û� UINT32
__declspec(selectany) extern const LPCWSTR GPROP_OwName = L"gOwnerName"; // Ⱥӵ����
__declspec(selectany) extern const LPCWSTR GPROP_CreateTime = L"gCreateTime";   // ����ʱ��
//detail
__declspec(selectany) extern const LPCWSTR GPROP_GuildAuthMod = L"guildAuthMod"; // Ⱥ����֤ģʽ BizGroupFolderAuthMode ����
__declspec(selectany) extern const LPCWSTR GPROP_GuildAliasId = L"guildAliasId"; //�����Ⱥ���� UINT32
__declspec(selectany) extern const LPCWSTR GPROP_GuildAllowAdhocChat = L"guildAllowAdhocChat"; // bool
__declspec(selectany) extern const LPCWSTR GPROP_GuildIsPrivate = L"guildIsPrivate"; // bool
__declspec(selectany) extern const LPCWSTR GPROP_GuildCategory = L"guildCategory"; // UINT16
__declspec(selectany) extern const LPCWSTR GPROP_GuildSubCategory = L"guildSubCategory"; // UINT16
__declspec(selectany) extern const LPCWSTR GPROP_GuildGroupDesc = L"guildGroupDesc"; // string
__declspec(selectany) extern const LPCWSTR GPROP_GuildGroupBulletin = L"guildGroupBulletin"; // string
__declspec(selectany) extern const LPCWSTR GPROP_GuildCustomLogoURL = L"guildCustomLogoURL";
__declspec(selectany) extern const LPCWSTR GPROP_GuildTopicMod = L"guildTopicMod"; // BizGroupTopicMode����
__declspec(selectany) extern const LPCWSTR GPROP_GuildBindingAliasId = L"bindingAliasId";    // Ƶ���̺�
__declspec(selectany) extern const LPCWSTR GPROP_GuildBindingType = L"bindingType";
__declspec(selectany) extern const LPCWSTR GPROP_GuildBindingAppId = L"bindingAppId";    // typeΪ0ʱ��Ϊ����
__declspec(selectany) extern const LPCWSTR GPROP_GuildAliasName = L"guildAliasName";     // Ⱥ��ע
__declspec(selectany) extern const LPCWSTR GPROP_GuildDiscussionType = L"guildDiscussionType";     // ����������

//folder prop
//simple
__declspec(selectany) extern const LPCWSTR FPROP_Fid = L"fid"; //UINT32
__declspec(selectany) extern const LPCWSTR FPROP_Name = L"folderName";  // string
__declspec(selectany) extern const LPCWSTR FPROP_AuthMod = L"folderAuthMod";  // byte
//detail
__declspec(selectany) extern const LPCWSTR FPROP_FolderDesc = L"folderDesc"; //FOLDER_Desc
__declspec(selectany) extern const LPCWSTR FPROP_FolderBulletin = L"folderBulletin"; //FOLDER_Bulletin
__declspec(selectany) extern const LPCWSTR FPROP_FolderTopicMod = L"foldTopicMod"; // BizGroupTopicMode����

// Ⱥ��Աͷ�����϶���
__declspec(selectany) extern const LPCWSTR MEMBER_HEAD_LOGOINDEX = L"memberHeadLogoIndex"; //UINT32 ͷ������ID
__declspec(selectany) extern const LPCWSTR MEMBER_HEAD_LOGOURL = L"memberHeadLogoURL"; //string ͷ�����ص�ַ

// Ⱥ������ɫ
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_ROLE = L"memberRole"; //UINT8 ����Ա��ɫ

// group member status
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_STATUS = L"memberStatus"; //UINT8 online status
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_BEBANNED = L"memberBeBanned"; //UINT8 be banned to say
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_LATEST_REPLAYTIME = L"memberLatestReplyTime"; //UINT8
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_IMID = L"memberIMID"; //UINT32
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_IMVIP = L"memberIMVIP"; //UINT32
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_IMVIPTYPE = L"memberIMVIPTYPE"; //UINT32

//dev prop
__declspec(selectany) extern const LPCWSTR DEVPROP_Guid = L"guid"; //GUID
__declspec(selectany) extern const LPCWSTR DEVPROP_Name = L"name";
__declspec(selectany) extern const LPCWSTR DEVPROP_Desc = L"desc";

//player prop
__declspec(selectany) extern const LPCWSTR PLAYERPROP_Path = L"path";
__declspec(selectany) extern const LPCWSTR PLAYERPROP_Name = L"name";

//reverb prop
__declspec(selectany) extern const LPCWSTR REVERBPROP_Enable = L"reverbEnable";
__declspec(selectany) extern const LPCWSTR REVERBPROP_Preset = L"reverbPreset";
__declspec(selectany) extern const LPCWSTR REVERBPROP_Volume = L"reverbVolume";
__declspec(selectany) extern const LPCWSTR REVERBPROP_InputVolume = L"reverbInputVolume";

//voiceChange prop
__declspec(selectany) extern const LPCWSTR VOICECHANGEPROP_Enable = L"voiceChangeEnable";
__declspec(selectany) extern const LPCWSTR VOICECHANGEPROP_Preset = L"voiceChangePreset";
__declspec(selectany) extern const LPCWSTR VOICECHANGEPROP_Auto = L"voiceChangeYYAnchorAuto";
__declspec(selectany) extern const LPCWSTR VOICECHANGEPROP_IntputGuid = L"voiceChangeInPutGuid";

__declspec(selectany) extern const LPCWSTR VOICECHANGEPROP_ExquisiteEnable = L"voiceChangeExquisteEnable";
__declspec(selectany) extern const LPCWSTR VOICECHANGEPROP_Semitone = L"voiceChangeSemitone";

//rovee voicechange pro
__declspec(selectany) extern const LPCWSTR ROVEE_VOICECHANGEPROP_Enable = L"roveevoiceChangeEnable";
__declspec(selectany) extern const LPCWSTR ROVEE_VOICECHANGEPROP_Preset = L"roveevoiceChangePreset";
__declspec(selectany) extern const LPCWSTR ROVEE_VOICECHANGEPROP_Pitch = L"roveevoiceChangePitch";
__declspec(selectany) extern const LPCWSTR ROVEE_VOICECHANGEPROP_Formant = L"roveevoiceChangeFormant";

//microphone prop
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_AGC = L"AGC";
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_Denoise = L"denoise";
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_Boost = L"boost";
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_BoostKind = L"boostKind";
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_BoostScale = L"boostScale";
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_AEC = L"AEC";
__declspec(selectany) extern const LPCWSTR MICROPHONEPROP_VAD = L"VAD";

// MsgCenter��Ϣ����
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGKIND = L"msgKind";		// msgKind: ��Ϣ�����MessageKind����
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGSYS_SUBKIND = L"msgSysSubKind";		// msgSysSubKind: ��Ϣ���Ⱥϵͳ��Ϣ�е�������
__declspec(selectany) extern const LPCWSTR CENTERPROP_PEERID = L"msgpeerid";		// msgpeerid: һ��һ����Է�peerid
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGTYPE = L"msgType";		// msgType: ��Ϣ�����MessageKind����
__declspec(selectany) extern const LPCWSTR CENTERPROP_NO_MSGBOX = L"no_msgbox";  // no_msgbox: 1��ʾ�����뵽��Ϣ������
__declspec(selectany) extern const LPCWSTR CENTERPROP_MULGUID = L"msgguid";		// msgguid: ��������Guid
__declspec(selectany) extern const LPCWSTR CENTERPROP_GROUPUID = L"msggroupid";	// msggroupid: Ⱥid
__declspec(selectany) extern const LPCWSTR CENTERPROP_FOLDERUID = L"msgfolderid";// msgfolderid: ��id
__declspec(selectany) extern const LPCWSTR CENTERPROP_NICKNAME  = L"msgpeernickname"; //���������ǳ�
__declspec(selectany) extern const LPCWSTR CENTERPROP_GENDER    = L"msgpeergender"; //�Ա�
__declspec(selectany) extern const LPCWSTR CENTERPROP_GUILDNOTICEID = L"msgguildnotifyid";//����֪ͨ
__declspec(selectany) extern const LPCWSTR CENTERPROP_CLEAR = L"clear";              // bool: trueʱ��ʾ���������Ϣ
__declspec(selectany) extern const LPCWSTR CENTERPROP_REMOVED_COUNT = L"removed_count"; // uint32_t: ��������
__declspec(selectany) extern const LPCWSTR CENTERPROP_BUDDYVERIFY_EXPAND = L"buddyverifymsgexpand"; //��Ӻ�����֤��Ϣexpand�ֶ�.
__declspec(selectany) extern const LPCWSTR CENTERPROP_SEND_TIME = L"msgsendtime";//uint32_t:
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGBOX_FLAG = L"msgboxflag";//bool
__declspec(selectany) extern const LPCWSTR CENTERPROP_OPEN_FROM = L"msgopenfrom"; // uint32_t: �����촰����Դ
__declspec(selectany) extern const LPCWSTR CENTERPROP_INITIATIVE = L"msginitiative";//bool
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSG_ID = L"msgid";//uint32_t
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSG_RETRACT = L"msgRetract";//bool�Ƿ��Ѿ�������
__declspec(selectany) extern const LPCWSTR CENTERPROP_PAOSAOTUAN_MSG_TYPE = L"paoSaoTuanMsgType"; //uint32 ��ɧ����Ϣ��1.�����2.���� ������+��Ը����
__declspec(selectany) extern const LPCWSTR CENTERPROP_PAOSAOTUAN_MSG=L"paoSaoTuanMsg"; //string ��ɧ����Ϣ��json��
__declspec(selectany) extern const LPCWSTR CENTERPROP_IS_SOMEONE_AT_ME=L"isSomeoneAtMe";// �Ƿ�����@�� 

__declspec(selectany) extern const LPCWSTR CENTERPROP_WORLDID = L"msgworldid";		//msgworldid:����ID
__declspec(selectany) extern const LPCWSTR CENTERPROP_QUESTIONID = L"msgquestionid";//msgquestionid:����ID
__declspec(selectany) extern const LPCWSTR CENTERPROP_VIDEO_GUID = L"msgvideoguid";// msgvideoguid
__declspec(selectany) extern const LPCWSTR CENTERPROP_VIPMSG_ID = L"vipmsgid";//��ԱIm����ID
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGFLAG = L"msg_flag"; // ����MSG_FLAG
 //biz config
__declspec(selectany) extern const LPCWSTR BIZPROP_AUTOREPLY = L"msg_autoreply";			// �Զ��ظ�1, 0
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG = L"msg_setting";					// ��Ϣ�趨��������������һ
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG_FLASH = L"msg_flashicon";			// ����Ϣ���̴�����
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG_POPUP = L"msg_popup";				// ֱ�ӵ����Ի���
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG_SILENCE = L"msg_silence";			// ����ģʽ��ֻ��¼����Ϣ����
__declspec(selectany) extern const LPCWSTR BIZPROP_REJECTINVITE = L"msg_rejectinvite"; //�ܾ�����
 
//error
//__declspec(selectany) extern const LPCWSTR ERRPROP_

//audio statistic
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_AudioType = L"audioType";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_DelayTime = L"delayTime";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_SSID = L"SSID";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_SEQ  = L"seq";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_OLD_VAD = L"oldVad";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_NEW_VAD = L"newVad"; 
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_NO_VOICE = L"noVoiceTimes"; // uint32_t
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_CHECK = L"checkTimes";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_CHANNEL_TYPE = L"channelType";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_LOSS = L"loss";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_TOTAL = L"total";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_FramePlayTime = L"framePlayTime";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_FrameNum = L"frameNum";
__declspec(selectany) extern const LPCWSTR AUDIOSTATISTIC_IsSplit = L"IsSplitFrame";

//growth system
__declspec(selectany) extern const LPCWSTR REPUTATION_UID	=L"ReputaionUID";
__declspec(selectany) extern const LPCTSTR REPUTATION_GRADE	=L"ReputationGrade";
__declspec(selectany) extern const LPCTSTR REPUTATION_VALUE	=L"ReputationValue";

//mp3info
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_title = L"title";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_artist = L"artist";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_album = L"album";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_year = L"year";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_comment = L"comment";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_track = L"track";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_genre = L"genre";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_fieldValue = L"fieldValue";
__declspec(selectany) extern const LPCWSTR AUDIOMP3INFO_imagePath = L"imagePath";

//hiido statistic
__declspec(selectany) extern const LPCWSTR REPORTHIIDO_udbLinkIP = L"udbEauthLinkIP";
__declspec(selectany) extern const LPCWSTR REPORTHIIDO_udbEauthAppid = L"udbEauthAppid";
__declspec(selectany) extern const LPCWSTR REPORTHIIDO_clientVer = L"clientVer";
__declspec(selectany) extern const LPCWSTR REPORTHIIDO_uid = L"uid";

// buddy channel info
__declspec(selectany) extern const LPCWSTR BUDDY_channel_uid = L"channel_uid";           // ����UID
__declspec(selectany) extern const LPCWSTR BUDDY_channel_id = L"channel_id";             // ����Ƶ����
__declspec(selectany) extern const LPCWSTR BUDDY_channel_sid = L"channel_sid";           // Ƶ���̺�
__declspec(selectany) extern const LPCWSTR BUDDY_channel_ssid = L"channel_ssid";         // Ƶ����Ƶ��ID
__declspec(selectany) extern const LPCWSTR BUDDY_channel_name = L"channel_name";         // Ƶ����
__declspec(selectany) extern const LPCWSTR BUDDY_channel_aid = L"channel_aid";           // Ƶ���ID
__declspec(selectany) extern const LPCWSTR BUDDY_channel_asid = L"channel_asid";         // ���Ƶ��id
__declspec(selectany) extern const LPCWSTR BUDDY_channel_aname = L"channel_aname";       // ��Ƶ�����
__declspec(selectany) extern const LPCWSTR BUDDY_channel_aimage = L"channel_aimage";     // Ƶ�������
__declspec(selectany) extern const LPCWSTR BUDDY_channel_type = L"channel_type";         // Ƶ�����ͣ�ͨ����Ƶ��/��������
__declspec(selectany) extern const LPCWSTR BUDDY_channel_invition_behavior = L"channel_invation_behavior";   // ���������Ϊ
__declspec(selectany) extern const LPCWSTR BUDDY_channel_kick_others = L"channel_kick_others";   // T�����˵����ڵ�Ƶ����ֵ����Ҫ��
__declspec(selectany) extern const LPCWSTR BUDDY_channel_client_type = L"channel_client_type";   // �ͻ�������, EClientType
__declspec(selectany) extern const LPCWSTR BUDDY_channel_hidden = L"channel_hidden";     // ���շ��Ƿ����ء�flase/true��Ĭ��false
__declspec(selectany) extern const LPCWSTR BUDDY_channel_icon = L"channel_icon";			// Сͼ��
__declspec(selectany) extern const LPCWSTR BUDDY_channel_from = L"channel_from";			// ���id/Ƶ��ģ��id
__declspec(selectany) extern const LPCWSTR BUDDY_channel_teamname = L"channel_teamname"; //Ƶ�������
__declspec(selectany) extern const LPCWSTR BUDDY_channel_i_am_in_wonder_world = L"channel_i_am_in_wonder_world";	// ��ǰ���ھ��������־
__declspec(selectany) extern const LPCWSTR BUDDY_channel_invite_title = L"channel_invite_title";	// ��ǰ���ھ��������־
__declspec(selectany) extern const LPCWSTR BUDDY_channel_invite_userdata = L"channel_invite_userdata";	// ��ǰ���ھ��������־
__declspec(selectany) extern const LPCWSTR BUDDY_channel_share_link_begin = L"channel_share_link_begin"; // ����Ƶ���㲥֪ͨ��������������ֵ���ʼλ��
__declspec(selectany) extern const LPCWSTR BUDDY_channel_share_media = L"channel_share_media"; // ����Ƶ����ý��
__declspec(selectany) extern const LPCWSTR BUDDY_channel_share_setting = L"channel_share_setting"; // ����Ƶ���㲥֪ͨ���ʾ������ʾ
__declspec(selectany) extern const LPCWSTR BUDDY_component_jumpticket = L"component_jumpticket"; // ���������ת�ɻ�Ʊ��string
__declspec(selectany) extern const LPCWSTR BUDDY_component_isturn = L"component_isturn"; // ��������Ƿ��ֲ���int
__declspec(selectany) extern const LPCWSTR BUDDY_component_infoname = L"component_infoname";   // ��������ֲ���title
__declspec(selectany) extern const LPCWSTR BUDDY_component_iconurl = L"component_iconurl";	   // ��������ֲ���Сͼ��

// ����¼
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_desc = L"desc";                   // ��������: YY for iPhone
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_type = L"type";                   // ���ͣ�ͨ��ΪMULTI_LOGIN_device_type��MULTI_LOGIN_system_type��ϵ�ֵ
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_device_type = L"device_type";     // �豸/�ն����ͣ�EClientType
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_system_type = L"system_type";     // ϵͳ. ios/android ==, EMobileType
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_application_type = L"app_type";   // Ӧ��. EAppType
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_status = L"status";               // IM_STATUS/protocol::im::E_IMSTATUS
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_last_time = L"last_login_time";   // �����¼ʱ��
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_ipaddr = L"ipaddr";               // ip��ַ
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_position = L"position";           // ����λ��
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_client_string = L"client_string"; // �ն˱�ʶ�ַ���������"yy"��"yymand"��
//
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_channel_id = L"channel_id";       // ����������Ƶ����id
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_data1 = L"data1";                 // ����1
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_data2 = L"data2";                 // ����2
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_data3 = L"data3";                 // ����3

// ������Ϣ
__declspec(selectany) extern const LPCWSTR VOICE_MESSAGE_url = L"__url";                 // ����url
__declspec(selectany) extern const LPCWSTR VOICE_MESSAGE_time = L"time_length";          // ʱ�䳤��
__declspec(selectany) extern const LPCWSTR VOICE_MESSAGE_index = L"index";               // ����

// url����
__declspec(selectany) extern const LPCWSTR LINK_MESSAGE_url = L"lnk_url";                // ����
__declspec(selectany) extern const LPCWSTR LINK_MESSAGE_desc = L"lnk_desc";              // ��ʾ�������ı�

// @
__declspec(selectany) extern const LPCWSTR YYAT_MESSAGE_text = L"at_text";               // text
__declspec(selectany) extern const LPCWSTR YYAT_MESSAGE_imid = L"at_imid";               // imid
__declspec(selectany) extern const LPCWSTR YYAT_MESSAGE_mytext = L"at_mytext";           // ��ʾ���Լ�����text�����籸ע��

// ����IPorpertyPacker�е�����
__declspec(selectany) extern const LPCWSTR PROP_PACKER_voice = L"voice";                 // ����
__declspec(selectany) extern const LPCWSTR PROP_PACKER_link = L"link";                   // url����
__declspec(selectany) extern const LPCWSTR PROP_PACKER_yyat = L"yyat";                   // at
__declspec(selectany) extern const LPCWSTR PROP_PACKER_isolatedch = L"isolatedch";       // Ƶ�����̸���
__declspec(selectany) extern const LPCWSTR PROP_PACKER_channel_shader = L"channel_shader";   // Ƶ�����ѷ���
__declspec(selectany) extern const LPCWSTR PROP_PACKER_channel_invite = L"channel_invite";   // Ƶ������

//Ƶ���������Ż���ͼ����Ϣ
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_dengji = L"dengji";        //���Ż���ͼ��ȼ�
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_iconPath = L"iconPath";    //���Ż���ͼ��·��
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_tipsUrl = L"tipsUrl";      //���Ż���ͼ��tips
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_width = L"width";          //���Ż���ͼ��tips���
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_height = L"height";		  //���Ż���ͼ��ȼ�tips�߶�

//��Ƶ����Դ
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_create = L"main.createchannel";        //����Ƶ��
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_search = L"main.search";				//����
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_sysmsg = L"main.systemmsg";			//�����
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_normal = L"main.channeltab.normal";	//˫��Ƶ���б�
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_activity = L"main.channeltab.activity";//˫��Ƶ���б��µĻ
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_menu = L"main.mainmenu";				//Ƶ���б��Ҽ��˵�
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_wonder = L"main.wonderitem";			//����徫�������·����
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_tablink = L"main.imtab.link";			//IM��Ƶ������
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_tabmenu = L"main.imtab.menu";			//IM��Ƶ������-�Ҽ�����
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_minicardlink = L"im.minicard.link";		//mini���Ͽ�tips
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_chatlink = L"im.chatwnd.chatlink";		//������ϢƵ������
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_bindgroup = L"bindgroup";				//��Ƶ��
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_chatview= L"im.chatwnd.view";			//Ⱥ��ӦƵ�����
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_chatinvite = L"im.chatwnd.invite";		//�����Ƶ��
__declspec(selectany) extern const LPCWSTR ENTRANCE_GUILD_link = L"guildminicard.link";			//�����������Ͽ�
__declspec(selectany) extern const LPCWSTR ENTRANCE_GUILD_infopage = L"guildinfopage";			//������Ϣҳ
__declspec(selectany) extern const LPCWSTR ENTRANCE_CHANNEL_tour = L"channeltour";				//Ƶ������
__declspec(selectany) extern const LPCWSTR ENTRANCE_CHANNEL_homepage = L"homepage";				//Ƶ��
__declspec(selectany) extern const LPCWSTR ENTRANCE_CHANNEL_favormenu = L"favormenu";			//Ƶ��
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_channel = L"channl.ticket.channel";	//Ƶ���ڵ���ɻ�Ʊ
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_im = L"channel.ticket.im";			//IM���촰�ڵ���ɻ�Ʊ
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_group = L"channel.ticket.imgroup";	//Ⱥ�����촰�ڵ���ɻ�Ʊ
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_popwnd = L"channel.ticket.popupwindow";//���½ǵ�������ɻ�Ʊ
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_web = L"channel.ticket.web";			//��ҳ�ɻ�Ʊ
__declspec(selectany) extern const LPCWSTR ENTRANCE_WONDERWORLD = L"wonderworld";				//��������
__declspec(selectany) extern const LPCWSTR ENTRANCE_WONDERWORLD_programe = L"wonderprogram";	//��������
__declspec(selectany) extern const LPCWSTR ENTRANCE_WEB_IPC = L"ipcwidget";						//��Ƕҳ
__declspec(selectany) extern const LPCWSTR ENTRANCE_WEB_FRAME = L"webframe";					//��Ƕҳ
__declspec(selectany) extern const LPCWSTR ENTRANCE_KEFU = L"kefu";								//�ͷ�
__declspec(selectany) extern const LPCWSTR ENTRANCE_FINDCENTER = L"findcenter";					//Ƶ������
__declspec(selectany) extern const LPCWSTR ENTRANCE_DWOP = L"dwop";								//����ƽ̨

namespace LOG
{
	__declspec(selectany) extern const char KSystemMsg[]		="KSystemMsg";
}

// �����ղص�������
enum ENUM_JOIN_TYPE
{
	KJoinUnknown = -1,	// δ֪����Ϊ��Ĭ�ϴ���
	KJoinChannel,		// Ƶ��
	KJoinWonderWorld	// ��������
};