
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
__declspec(selectany) extern const LPCWSTR SYSTEM_MSG_NICK				= L"系统消息";
__declspec(selectany) extern const LPCWSTR LOCAL_MESSAGE_GUID			= L"999999"; 
__declspec(selectany) extern const LPCWSTR RECOMMEND_MSG_ID				= L"YYRECC_@U";
#define MultiTalkToId				(unsigned int)-1
#define SERVER_MultiTLAK_INT		2147483647  //服务器端的多人会话的touID的返回值
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
__declspec(selectany) extern const LPCWSTR UPROP_Imid = L"imid"; // uint32_t m_uImId;//yy短号，没有就是-1
__declspec(selectany) extern const LPCWSTR UPROP_Nick = L"nick";
// 
__declspec(selectany) extern const LPCWSTR UPROP_ArtistName = L"artistname";// 艺名
//simple
__declspec(selectany) extern const LPCWSTR UPROP_Gender = L"gender"; //GENDER
__declspec(selectany) extern const LPCWSTR UPROP_Sign = L"sign";
__declspec(selectany) extern const LPCWSTR UPROP_Remark = L"remark"; // 备注
__declspec(selectany) extern const LPCWSTR UPROP_Jifen = L"jifen"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_JifenInChannel = L"jifenInChannel"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_Level = L"level"; //UINT32
__declspec(selectany) extern const LPCWSTR UPROP_RoleInChannel = L"roleInChannel"; //ROLE_IN_CHANNEL
__declspec(selectany) extern const LPCWSTR UPROP_EnableTextInChannel = L"enableTextInChannel"; //bool
__declspec(selectany) extern const LPCWSTR UPROP_EnableVoiceInChannel = L"enableVoiceInChannel"; //bool
__declspec(selectany) extern const LPCWSTR UPROP_ImStatus = L"imStatus"; //IM_STATUS
__declspec(selectany) extern const LPCWSTR UPROP_HeadPhotoIndex = L"headPhotoIndex"; // uint32_t m_uHeadPhoto;//头像,没有就是-1
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL = L"customHeadPhotoURL"; //60*60头像Url
__declspec(selectany) extern const LPCWSTR UPROP_CustomLocalHeadPhotoURL = L"customLocalHeadPhotoURL"; //本地的头像路径
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL_100_100 = L"customHeadPhotoURL_100_100"; //100*100头像Url
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL_144_144 = L"customHeadPhotoURL_144_144"; //140*140头像Url
__declspec(selectany) extern const LPCWSTR UPROP_CustomHeadPhotoURL_640_640 = L"customHeadPhotoURL_640_640"; //640*640头像Url
__declspec(selectany) extern const LPCWSTR UPROP_BIRTHDAY = L"birthday";	// uint32_t m_uBirthDay;
__declspec(selectany) extern const LPCWSTR UPROP_PROVINCE = L"PROVINCE";	// uint32_t m_uProvince;
__declspec(selectany) extern const LPCWSTR UPROP_CITY = L"CITY";			// uint32_t m_uCity;
__declspec(selectany) extern const LPCWSTR UPROP_VERSION = L"VERSION";		// uint32_t m_uVersion;
__declspec(selectany) extern const LPCWSTR UPROP_INTRO = L"INTRO";			// UniString m_strIntro;
__declspec(selectany) extern const LPCWSTR UPROP_AREA = L"AREA";			// uint32_t m_uArea;
__declspec(selectany) extern const LPCWSTR UPROP_DEVICE = L"DEVICE";		// uint32_t 0:PC,1手机设备;
__declspec(selectany) extern const LPCWSTR UPROP_DEVICE_PHONE = L"DEVICE_PHONE";		// uint32_t 0:PC,1手机设备;
__declspec(selectany) extern const LPCWSTR UPROP_DEVICE_APP = L"DEVICE_APP"; // IMAPPLICATIONTYPE
__declspec(selectany) extern const LPCWSTR UPROP_CORPS = L"CORPS";		// 军团名
__declspec(selectany) extern const LPCWSTR UPROP_CORPS_POSITION = L"CORPSPOSITION";	//军团职位

// 粉丝属性，给App用的
__declspec(selectany) extern const LPCWSTR UPROP_FANS_UID = L"uid";
__declspec(selectany) extern const LPCWSTR UPROP_FANS_Nick = L"nick";
__declspec(selectany) extern const LPCWSTR UPROP_FANS_Photo = L"photo";
__declspec(selectany) extern const LPCWSTR UPROP_FANS_vip = L"vip";

//恢复好友属性
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
//vip信息

__declspec(selectany) extern const LPCWSTR UPROP_VipGrade = L"vipgrade";
__declspec(selectany) extern const LPCWSTR UPROP_VipType = L"viptype";
__declspec(selectany) extern const LPCWSTR UPROP_pdGrade = L"PD_grade";  // 0 非紫钻会员  1 ~ 7
__declspec(selectany) extern const LPCWSTR UPROP_pdType = L"PD_type";    // 0 紫钻月费  1 紫钻年费
__declspec(selectany) extern const LPCWSTR UPROP_pdUpgradeTime = L"PD_upgrade_time"; // 紫钻升级时间
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviNick = L"PD_privi_nick";     // true表示开启紫色昵称
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviSuper = L"PD_privi_super";   // true表示超级紫钻
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviExvip = L"PD_privi_exvip";   // true表示至尊会员
__declspec(selectany) extern const LPCWSTR UPROP_pdPriviSVip = L"PD_privi_svip";    // true表示超凡会员

//用户拓展信息
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
__declspec(selectany) extern const LPCWSTR CPROP_GuestPrivateChatWaitTime = L"guestPrivateChatWaitTime"; //UINT32 游客进入拼单后允许私聊等待时间（topChannel）
__declspec(selectany) extern const LPCWSTR CPROP_MgrRejectGuestPrivateChat = L"bMgrRejectGuestPrivateChat";//黄马及以上管理员默认不接受私聊（topChannel）
__declspec(selectany) extern const LPCWSTR CPROP_IsHideVisitorTextNWaitTime = L"bIsHidVisitorTextLimitation";//bool
//only for channelList/qChannelList/catalogChannelList/...
__declspec(selectany) extern const LPCWSTR CPROP_Asid = L"asid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_TopSid = L"topSid"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_Password = L"password";
__declspec(selectany) extern const LPCWSTR CPROP_Hello = L"hello";
__declspec(selectany) extern const LPCWSTR CPROP_OnlineUserCount = L"onlineUserCount"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsEnableBubble = L"isEnableBubble";//bool，8.49版本下线功能
__declspec(selectany) extern const LPCWSTR GPROP_AutoDiversionSetting = L"autoDiversionSetting"; //UINT32
__declspec(selectany) extern const LPCWSTR GPROP_AutoBroadcastSetting = L"autoBroadcastSetting"; //UINT32
__declspec(selectany) extern const LPCWSTR CPROP_IsPinTop = L"isPinTop"; //频道最近列表中，是否置顶, bool
__declspec(selectany) extern const LPCWSTR CPROP_IsGifLogo = L"isGifLogo"; //频道最近列表中，是否gif头像, bool
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
__declspec(selectany) extern const LPCWSTR CPROP_AccessWaittingTime     = L"1"; // 进频道还需要等待N秒发言
__declspec(selectany) extern const LPCWSTR CPROP_IntervalLeftTime       = L"2"; //到下次发言还需要等待N秒
__declspec(selectany) extern const LPCWSTR CPROP_TextLimitLength        = L"3"; //文本长度最多不超过N个字
__declspec(selectany) extern const LPCWSTR CPROP_FilterLimitReason      = L"4"; //反垃圾系统过滤原因
__declspec(selectany) extern const LPCWSTR CPROP_GlobalFilterRescode    = L"5"; //反垃圾系统过滤返回码
__declspec(selectany) extern const LPCWSTR CPROP_FormatedErrorString    = L"formated_error_string";// string, 返回格式化好的错误内容

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
__declspec(selectany) extern const LPCWSTR GPROP_GName = L"gname";	// 群名片昵称
__declspec(selectany) extern const LPCWSTR GPROP_Name = L"name";		// 用户昵称
__declspec(selectany) extern const LPCWSTR GPROP_GuildLogoIndex = L"guildLogoIndex"; //群头像索引 UINT32
__declspec(selectany) extern const LPCWSTR GPROP_ModifyGroupInfoUserId = L"modifyGroupInfoUserId"; //修改群信息的操作用户 UINT32
__declspec(selectany) extern const LPCWSTR GPROP_OwName = L"gOwnerName"; // 群拥有者
__declspec(selectany) extern const LPCWSTR GPROP_CreateTime = L"gCreateTime";   // 创建时间
//detail
__declspec(selectany) extern const LPCWSTR GPROP_GuildAuthMod = L"guildAuthMod"; // 群的验证模式 BizGroupFolderAuthMode 类型
__declspec(selectany) extern const LPCWSTR GPROP_GuildAliasId = L"guildAliasId"; //对外的群号码 UINT32
__declspec(selectany) extern const LPCWSTR GPROP_GuildAllowAdhocChat = L"guildAllowAdhocChat"; // bool
__declspec(selectany) extern const LPCWSTR GPROP_GuildIsPrivate = L"guildIsPrivate"; // bool
__declspec(selectany) extern const LPCWSTR GPROP_GuildCategory = L"guildCategory"; // UINT16
__declspec(selectany) extern const LPCWSTR GPROP_GuildSubCategory = L"guildSubCategory"; // UINT16
__declspec(selectany) extern const LPCWSTR GPROP_GuildGroupDesc = L"guildGroupDesc"; // string
__declspec(selectany) extern const LPCWSTR GPROP_GuildGroupBulletin = L"guildGroupBulletin"; // string
__declspec(selectany) extern const LPCWSTR GPROP_GuildCustomLogoURL = L"guildCustomLogoURL";
__declspec(selectany) extern const LPCWSTR GPROP_GuildTopicMod = L"guildTopicMod"; // BizGroupTopicMode类型
__declspec(selectany) extern const LPCWSTR GPROP_GuildBindingAliasId = L"bindingAliasId";    // 频道短号
__declspec(selectany) extern const LPCWSTR GPROP_GuildBindingType = L"bindingType";
__declspec(selectany) extern const LPCWSTR GPROP_GuildBindingAppId = L"bindingAppId";    // type为0时，为长号
__declspec(selectany) extern const LPCWSTR GPROP_GuildAliasName = L"guildAliasName";     // 群备注
__declspec(selectany) extern const LPCWSTR GPROP_GuildDiscussionType = L"guildDiscussionType";     // 讨论组属性

//folder prop
//simple
__declspec(selectany) extern const LPCWSTR FPROP_Fid = L"fid"; //UINT32
__declspec(selectany) extern const LPCWSTR FPROP_Name = L"folderName";  // string
__declspec(selectany) extern const LPCWSTR FPROP_AuthMod = L"folderAuthMod";  // byte
//detail
__declspec(selectany) extern const LPCWSTR FPROP_FolderDesc = L"folderDesc"; //FOLDER_Desc
__declspec(selectany) extern const LPCWSTR FPROP_FolderBulletin = L"folderBulletin"; //FOLDER_Bulletin
__declspec(selectany) extern const LPCWSTR FPROP_FolderTopicMod = L"foldTopicMod"; // BizGroupTopicMode类型

// 群成员头像资料定义
__declspec(selectany) extern const LPCWSTR MEMBER_HEAD_LOGOINDEX = L"memberHeadLogoIndex"; //UINT32 头像索引ID
__declspec(selectany) extern const LPCWSTR MEMBER_HEAD_LOGOURL = L"memberHeadLogoURL"; //string 头像下载地址

// 群组管理角色
__declspec(selectany) extern const LPCWSTR GFPROP_MEMBER_ROLE = L"memberRole"; //UINT8 管理员角色

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

// MsgCenter消息盒子
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGKIND = L"msgKind";		// msgKind: 消息类别，在MessageKind定义
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGSYS_SUBKIND = L"msgSysSubKind";		// msgSysSubKind: 消息类别，群系统消息中的子类型
__declspec(selectany) extern const LPCWSTR CENTERPROP_PEERID = L"msgpeerid";		// msgpeerid: 一对一聊天对方peerid
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGTYPE = L"msgType";		// msgType: 消息类别，在MessageKind定义
__declspec(selectany) extern const LPCWSTR CENTERPROP_NO_MSGBOX = L"no_msgbox";  // no_msgbox: 1表示不插入到消息盒子中
__declspec(selectany) extern const LPCWSTR CENTERPROP_MULGUID = L"msgguid";		// msgguid: 多人聊天Guid
__declspec(selectany) extern const LPCWSTR CENTERPROP_GROUPUID = L"msggroupid";	// msggroupid: 群id
__declspec(selectany) extern const LPCWSTR CENTERPROP_FOLDERUID = L"msgfolderid";// msgfolderid: 组id
__declspec(selectany) extern const LPCWSTR CENTERPROP_NICKNAME  = L"msgpeernickname"; //单人聊天昵称
__declspec(selectany) extern const LPCWSTR CENTERPROP_GENDER    = L"msgpeergender"; //性别
__declspec(selectany) extern const LPCWSTR CENTERPROP_GUILDNOTICEID = L"msgguildnotifyid";//公会通知
__declspec(selectany) extern const LPCWSTR CENTERPROP_CLEAR = L"clear";              // bool: true时表示清空所有消息
__declspec(selectany) extern const LPCWSTR CENTERPROP_REMOVED_COUNT = L"removed_count"; // uint32_t: 减少数量
__declspec(selectany) extern const LPCWSTR CENTERPROP_BUDDYVERIFY_EXPAND = L"buddyverifymsgexpand"; //添加好友验证消息expand字段.
__declspec(selectany) extern const LPCWSTR CENTERPROP_SEND_TIME = L"msgsendtime";//uint32_t:
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGBOX_FLAG = L"msgboxflag";//bool
__declspec(selectany) extern const LPCWSTR CENTERPROP_OPEN_FROM = L"msgopenfrom"; // uint32_t: 开聊天窗口来源
__declspec(selectany) extern const LPCWSTR CENTERPROP_INITIATIVE = L"msginitiative";//bool
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSG_ID = L"msgid";//uint32_t
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSG_RETRACT = L"msgRetract";//bool是否已经被撤回
__declspec(selectany) extern const LPCWSTR CENTERPROP_PAOSAOTUAN_MSG_TYPE = L"paoSaoTuanMsgType"; //uint32 跑骚团消息（1.红包，2.集结 集结令+心愿任务）
__declspec(selectany) extern const LPCWSTR CENTERPROP_PAOSAOTUAN_MSG=L"paoSaoTuanMsg"; //string 跑骚团消息（json）
__declspec(selectany) extern const LPCWSTR CENTERPROP_IS_SOMEONE_AT_ME=L"isSomeoneAtMe";// 是否有人@我 

__declspec(selectany) extern const LPCWSTR CENTERPROP_WORLDID = L"msgworldid";		//msgworldid:世界ID
__declspec(selectany) extern const LPCWSTR CENTERPROP_QUESTIONID = L"msgquestionid";//msgquestionid:问题ID
__declspec(selectany) extern const LPCWSTR CENTERPROP_VIDEO_GUID = L"msgvideoguid";// msgvideoguid
__declspec(selectany) extern const LPCWSTR CENTERPROP_VIPMSG_ID = L"vipmsgid";//会员Im弹窗ID
__declspec(selectany) extern const LPCWSTR CENTERPROP_MSGFLAG = L"msg_flag"; // 参照MSG_FLAG
 //biz config
__declspec(selectany) extern const LPCWSTR BIZPROP_AUTOREPLY = L"msg_autoreply";			// 自动回复1, 0
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG = L"msg_setting";					// 消息设定，以下三项中其一
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG_FLASH = L"msg_flashicon";			// 在消息托盘处闪动
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG_POPUP = L"msg_popup";				// 直接弹出对话框
__declspec(selectany) extern const LPCWSTR BIZPROP_MSG_SILENCE = L"msg_silence";			// 安静模式，只记录在消息盒子
__declspec(selectany) extern const LPCWSTR BIZPROP_REJECTINVITE = L"msg_rejectinvite"; //拒绝邀请
 
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
__declspec(selectany) extern const LPCWSTR BUDDY_channel_uid = L"channel_uid";           // 好友UID
__declspec(selectany) extern const LPCWSTR BUDDY_channel_id = L"channel_id";             // 顶级频道号
__declspec(selectany) extern const LPCWSTR BUDDY_channel_sid = L"channel_sid";           // 频道短号
__declspec(selectany) extern const LPCWSTR BUDDY_channel_ssid = L"channel_ssid";         // 频道子频道ID
__declspec(selectany) extern const LPCWSTR BUDDY_channel_name = L"channel_name";         // 频道名
__declspec(selectany) extern const LPCWSTR BUDDY_channel_aid = L"channel_aid";           // 频道活动ID
__declspec(selectany) extern const LPCWSTR BUDDY_channel_asid = L"channel_asid";         // 活动子频道id
__declspec(selectany) extern const LPCWSTR BUDDY_channel_aname = L"channel_aname";       // 子频道活动名
__declspec(selectany) extern const LPCWSTR BUDDY_channel_aimage = L"channel_aimage";     // 频道活动海报
__declspec(selectany) extern const LPCWSTR BUDDY_channel_type = L"channel_type";         // 频道类型，通常的频道/精彩世界
__declspec(selectany) extern const LPCWSTR BUDDY_channel_invition_behavior = L"channel_invation_behavior";   // 被邀请的行为
__declspec(selectany) extern const LPCWSTR BUDDY_channel_kick_others = L"channel_kick_others";   // T其他端的所在的频道，值不重要。
__declspec(selectany) extern const LPCWSTR BUDDY_channel_client_type = L"channel_client_type";   // 客户端类型, EClientType
__declspec(selectany) extern const LPCWSTR BUDDY_channel_hidden = L"channel_hidden";     // 接收方是否隐藏。flase/true，默认false
__declspec(selectany) extern const LPCWSTR BUDDY_channel_icon = L"channel_icon";			// 小图标
__declspec(selectany) extern const LPCWSTR BUDDY_channel_from = L"channel_from";			// 组件id/频道模板id
__declspec(selectany) extern const LPCWSTR BUDDY_channel_teamname = L"channel_teamname"; //频道组队名
__declspec(selectany) extern const LPCWSTR BUDDY_channel_i_am_in_wonder_world = L"channel_i_am_in_wonder_world";	// 当前我在精彩世界标志
__declspec(selectany) extern const LPCWSTR BUDDY_channel_invite_title = L"channel_invite_title";	// 当前我在精彩世界标志
__declspec(selectany) extern const LPCWSTR BUDDY_channel_invite_userdata = L"channel_invite_userdata";	// 当前我在精彩世界标志
__declspec(selectany) extern const LPCWSTR BUDDY_channel_share_link_begin = L"channel_share_link_begin"; // 分享频道广播通知里“分享”超链接文字的起始位置
__declspec(selectany) extern const LPCWSTR BUDDY_channel_share_media = L"channel_share_media"; // 分享频道的媒体
__declspec(selectany) extern const LPCWSTR BUDDY_channel_share_setting = L"channel_share_setting"; // 分享频道广播通知里，显示设置提示
__declspec(selectany) extern const LPCWSTR BUDDY_component_jumpticket = L"component_jumpticket"; // 组件分享跳转飞机票，string
__declspec(selectany) extern const LPCWSTR BUDDY_component_isturn = L"component_isturn"; // 组件分享是否轮播，int
__declspec(selectany) extern const LPCWSTR BUDDY_component_infoname = L"component_infoname";   // 组件分享轮播的title
__declspec(selectany) extern const LPCWSTR BUDDY_component_iconurl = L"component_iconurl";	   // 组件分享轮播的小图标

// 多点登录
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_desc = L"desc";                   // 描述，如: YY for iPhone
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_type = L"type";                   // 类型，通常为MULTI_LOGIN_device_type和MULTI_LOGIN_system_type组合的值
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_device_type = L"device_type";     // 设备/终端类型，EClientType
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_system_type = L"system_type";     // 系统. ios/android ==, EMobileType
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_application_type = L"app_type";   // 应用. EAppType
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_status = L"status";               // IM_STATUS/protocol::im::E_IMSTATUS
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_last_time = L"last_login_time";   // 最近登录时间
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_ipaddr = L"ipaddr";               // ip地址
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_position = L"position";           // 地理位置
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_client_string = L"client_string"; // 终端标识字符串（比如"yy"、"yymand"）
//
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_channel_id = L"channel_id";       // 其他端所在频道的id
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_data1 = L"data1";                 // 保留1
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_data2 = L"data2";                 // 保留2
__declspec(selectany) extern const LPCWSTR MULTI_LOGIN_data3 = L"data3";                 // 保留3

// 语音消息
__declspec(selectany) extern const LPCWSTR VOICE_MESSAGE_url = L"__url";                 // 语音url
__declspec(selectany) extern const LPCWSTR VOICE_MESSAGE_time = L"time_length";          // 时间长度
__declspec(selectany) extern const LPCWSTR VOICE_MESSAGE_index = L"index";               // 索引

// url链接
__declspec(selectany) extern const LPCWSTR LINK_MESSAGE_url = L"lnk_url";                // 链接
__declspec(selectany) extern const LPCWSTR LINK_MESSAGE_desc = L"lnk_desc";              // 显示出来的文本

// @
__declspec(selectany) extern const LPCWSTR YYAT_MESSAGE_text = L"at_text";               // text
__declspec(selectany) extern const LPCWSTR YYAT_MESSAGE_imid = L"at_imid";               // imid
__declspec(selectany) extern const LPCWSTR YYAT_MESSAGE_mytext = L"at_mytext";           // 显示给自己看的text（比如备注）

// 用于IPorpertyPacker中的类型
__declspec(selectany) extern const LPCWSTR PROP_PACKER_voice = L"voice";                 // 语音
__declspec(selectany) extern const LPCWSTR PROP_PACKER_link = L"link";                   // url链接
__declspec(selectany) extern const LPCWSTR PROP_PACKER_yyat = L"yyat";                   // at
__declspec(selectany) extern const LPCWSTR PROP_PACKER_isolatedch = L"isolatedch";       // 频道进程隔离
__declspec(selectany) extern const LPCWSTR PROP_PACKER_channel_shader = L"channel_shader";   // 频道好友分享
__declspec(selectany) extern const LPCWSTR PROP_PACKER_channel_invite = L"channel_invite";   // 频道邀请

//频道公屏靓号皇族图标信息
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_dengji = L"dengji";        //靓号皇族图标等级
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_iconPath = L"iconPath";    //靓号皇族图标路径
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_tipsUrl = L"tipsUrl";      //靓号皇族图标tips
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_width = L"width";          //靓号皇族图标tips宽度
__declspec(selectany) extern const LPCWSTR PROP_Nnmber_height = L"height";		  //靓号皇族图标等级tips高度

//进频道来源
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_create = L"main.createchannel";        //创建频道
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_search = L"main.search";				//搜索
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_sysmsg = L"main.systemmsg";			//活动提醒
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_normal = L"main.channeltab.normal";	//双击频道列表
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_activity = L"main.channeltab.activity";//双击频道列表下的活动
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_menu = L"main.mainmenu";				//频道列表右键菜单
__declspec(selectany) extern const LPCWSTR ENTRANCE_MAIN_wonder = L"main.wonderitem";			//主面板精彩世界下方入口
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_tablink = L"main.imtab.link";			//IM下频道链接
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_tabmenu = L"main.imtab.menu";			//IM下频道链接-右键进入
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_minicardlink = L"im.minicard.link";		//mini资料卡tips
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_chatlink = L"im.chatwnd.chatlink";		//聊天消息频道链接
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_bindgroup = L"bindgroup";				//绑定频道
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_chatview= L"im.chatwnd.view";			//群对应频道入口
__declspec(selectany) extern const LPCWSTR ENTRANCE_IM_chatinvite = L"im.chatwnd.invite";		//邀请进频道
__declspec(selectany) extern const LPCWSTR ENTRANCE_GUILD_link = L"guildminicard.link";			//公户迷离资料卡
__declspec(selectany) extern const LPCWSTR ENTRANCE_GUILD_infopage = L"guildinfopage";			//公会信息页
__declspec(selectany) extern const LPCWSTR ENTRANCE_CHANNEL_tour = L"channeltour";				//频道导航
__declspec(selectany) extern const LPCWSTR ENTRANCE_CHANNEL_homepage = L"homepage";				//频道
__declspec(selectany) extern const LPCWSTR ENTRANCE_CHANNEL_favormenu = L"favormenu";			//频道
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_channel = L"channl.ticket.channel";	//频道内点击飞机票
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_im = L"channel.ticket.im";			//IM聊天窗内点击飞机票
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_group = L"channel.ticket.imgroup";	//群组聊天窗内点击飞机票
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_popwnd = L"channel.ticket.popupwindow";//右下角弹窗点击飞机票
__declspec(selectany) extern const LPCWSTR ENTRANCE_TICKET_web = L"channel.ticket.web";			//网页飞机票
__declspec(selectany) extern const LPCWSTR ENTRANCE_WONDERWORLD = L"wonderworld";				//精彩世界
__declspec(selectany) extern const LPCWSTR ENTRANCE_WONDERWORLD_programe = L"wonderprogram";	//精彩世界
__declspec(selectany) extern const LPCWSTR ENTRANCE_WEB_IPC = L"ipcwidget";						//内嵌页
__declspec(selectany) extern const LPCWSTR ENTRANCE_WEB_FRAME = L"webframe";					//内嵌页
__declspec(selectany) extern const LPCWSTR ENTRANCE_KEFU = L"kefu";								//客服
__declspec(selectany) extern const LPCWSTR ENTRANCE_FINDCENTER = L"findcenter";					//频道查找
__declspec(selectany) extern const LPCWSTR ENTRANCE_DWOP = L"dwop";								//开放平台

namespace LOG
{
	__declspec(selectany) extern const char KSystemMsg[]		="KSystemMsg";
}

// 区分收藏的渠道用
enum ENUM_JOIN_TYPE
{
	KJoinUnknown = -1,	// 未知，行为按默认处理
	KJoinChannel,		// 频道
	KJoinWonderWorld	// 精彩世界
};