#pragma once

#include <tchar.h>
#include "dwbase/IData.h"

namespace Data
{
	DEFINE_NAMEID(viewMyUinfo);
	DEFINE_NAMEID(viewManagerList);
	DEFINE_NAMEID(viewManagerListEx);
	DEFINE_NAMEID(viewUserList);
	DEFINE_NAMEID(viewFullInfoUserList);
	DEFINE_NAMEID(viewMyChannelUserList);
	DEFINE_NAMEID(viewCurChannel);
	DEFINE_NAMEID(viewChannelList);
    DEFINE_NAMEID(viewLessInfoChannelList);
	DEFINE_NAMEID(tblManagerList);
	DEFINE_NAMEID(tblEchoUinfo);
	DEFINE_NAMEID(tblUinfo);
	DEFINE_NAMEID(tblUinfoCache);
    DEFINE_NAMEID(tblMicroList);
    DEFINE_NAMEID(tblChannelData);
	namespace Uinfo
	{
		DEFINE_NAMEID(dwUid);
		DEFINE_NAMEID(dwPid);  //当pid无数据时, UI即可忽略所有事件.
		DEFINE_NAMEID(dwUinfoJifen);
		DEFINE_NAMEID(dwSmemberJifen);
		DEFINE_NAMEID(dwGamming); 	//游戏尾灯: 
		DEFINE_NAMEID(dwActive); 	//活动尾灯
		DEFINE_NAMEID(bIsSettle);
		DEFINE_NAMEID(btSex);
		DEFINE_NAMEID(strNickname);
		DEFINE_NAMEID(strSign);
		DEFINE_NAMEID(dwYYId);
		DEFINE_NAMEID(dwCreateTime);
		DEFINE_NAMEID(strCorpName);
		DEFINE_NAMEID(strCorpRoler);
		DEFINE_NAMEID(bIsAdminDisplay);  //
		DEFINE_NAMEID(bIsIgnoreText); //true: 被忽略文字 
		DEFINE_NAMEID(bIsIgnoreVoice); //true: 被忽略说话
		DEFINE_NAMEID(bIsVoiceDisable);	//true: 被禁止说话
		DEFINE_NAMEID(bIsTextDisable); //true: 被禁止打字.
		DEFINE_NAMEID(bIsBindMobile);   //是否绑定手机. 
		DEFINE_NAMEID(dwRole);  //当用户role发生变化时, 此字段会改变, UI需要重新取得角色信息. 
		DEFINE_NAMEID(bIsVip);  //是否VIP
		DEFINE_NAMEID(dwVipJifen); 
		DEFINE_NAMEID(btVipLevel);
		DEFINE_NAMEID(bVipChannelRedName);	//VIP红名标识. 
		DEFINE_NAMEID(bVipFace);      //VIP魔法表情
		DEFINE_NAMEID(dwVipChannelTop);  //Vip排名靠前.
        DEFINE_NAMEID(dwVipLiangNumber); //vip靓号. 
		DEFINE_NAMEID(bVipChannelPurpleName); //vip紫色名
        DEFINE_NAMEID(bPurpleDiamond); //紫钻
        DEFINE_NAMEID(bPinkDiamond); //粉钻
        DEFINE_NAMEID(bSuperVipName); //至尊VIP(VIP 6 + 紫钻)
        DEFINE_NAMEID(bSuperPlDiamond); //超级紫钻 
        DEFINE_NAMEID(dwPlDiamondGrade); //紫钻等级    value: 1-7 
        DEFINE_NAMEID(dwPlDiamondType);  //紫钻付费类型    value: 0 月费， 1 年费
        DEFINE_NAMEID(dwPinkDiamondGrade); //粉钻等级    value: 1-7 
        DEFINE_NAMEID(dwPinkDiamondType);  //粉钻付费类型    value: 0 月费， 1 年费
        DEFINE_NAMEID(dwBrilliantVipGrade);  //超凡VIP等级
		DEFINE_NAMEID(bPropCompFlag); //数据对比用到的属性字段
		DEFINE_NAMEID(dwTestIcon);     //测试达人尾灯
		DEFINE_NAMEID(strPluginExtent);  //插件拓展属性，实际内容为用户拓展属性，由上层再去解其关心业务
		DEFINE_NAMEID(strBasicCardURL);  //麦上基础名片
		DEFINE_NAMEID(blobBasicCardData); //(新)上麦名片,存二进制字节流,由客户端解析(可能有多个URL)
		DEFINE_NAMEID(btQueryRetryNum);  //向服务器拉取重试次数. 
		DEFINE_NAMEID(dwRank);       //用户排名. 
		DEFINE_NAMEID(bVisual);      //大频道模式下，用户是否显示. 
		DEFINE_NAMEID(dwSortValue);			//频道排名 
		DEFINE_NAMEID(dwSortTipsType);      //排名规则
        DEFINE_NAMEID(strArtistname);       //艺名
		DEFINE_NAMEID(bAnnoyUid);           //匿名用户
		DEFINE_NAMEID(dwHugeChannelSubAnnoyUserCount);   //大频道中子频道匿名总人数，大频道专用
		DEFINE_NAMEID(dwTermType);			//终端
	}

	namespace ChannelData
	{
		DEFINE_NAMEID(dwSid);  //SIT_SID
		DEFINE_NAMEID(dwPid);  //SIT_PID
		DEFINE_NAMEID(dwSubChannelOrder);	//SIT_SUBCHANNEL_ORDER(N)
		DEFINE_NAMEID(strName); 	//SIT_NAME
		DEFINE_NAMEID(bHasPasswd); 	//SIT_BHASPASSWD
		DEFINE_NAMEID(bIsPub);  //SIT_ISPUB
		DEFINE_NAMEID(btStyle); 	//SIT_STYLE枚举 
		DEFINE_NAMEID(dwMicroTime); //SIT_MICROTIME
		DEFINE_NAMEID(dwBulletinTimeStamp);//SIT_BULLETINTIMESTAMP
		DEFINE_NAMEID(bIsTxtLimit); //SIT_ISTXTLIMIT 
		DEFINE_NAMEID(dwTxtLimitTime); //SIT_TXTLIMITTIME 
		DEFINE_NAMEID(bGuestEnableLimit);	//SIT_GUEST_ENABLELIMIT
		DEFINE_NAMEID(bEnableReceptionConfig);//SIT_ENABLE_RECEPTION_CONFIG
		////////////////////////////////////////////////////////////////////////////////
		DEFINE_NAMEID(strBulletin); //SIT_BULLETIN
		DEFINE_NAMEID(dwGuildLogo); //SIT_GUILD_LOGO
		DEFINE_NAMEID(strGuildLogoUrl);	//SIT_GUILD_LOGO_URL
		DEFINE_NAMEID(bMedalVioletGolenSession);	//SIT_MEDAL_VIOLET_GOLD_SESSION
		DEFINE_NAMEID(strMedalVioletLogoUrl);	//SIT_MEDAL_VIOLET_LOGO_URL
		DEFINE_NAMEID(strMedalName);	//SIT_MEDAL_NAME
		DEFINE_NAMEID(strMedalDescription); //SIT_MEDAL_DESCRIPTION
		DEFINE_NAMEID(strMedalLinkUrl); //SIT_MEDAL_LINK_URL
		DEFINE_NAMEID(dwMedalID); //SIT_MEDAL_ID
		DEFINE_NAMEID(dw64JiFen); 	//SIT_JIFEN
		DEFINE_NAMEID(dwPaiMing); 	//SIT_PAIMING 
		DEFINE_NAMEID(btCodecRate); //SIT_CODECRATE枚举
		DEFINE_NAMEID(btIspType); 	//SIT_ISPTYPE枚举
		DEFINE_NAMEID(bIsLimit); 	//SIT_ISLIMIT
		DEFINE_NAMEID(btType);  //SIT_TYPE枚举
		DEFINE_NAMEID(strTypeString); //SIT_TYPESTRING
		DEFINE_NAMEID(strOwner); 	//SIT_OWNER
		DEFINE_NAMEID(dwApplyJifen); //SIT_APPLY_JIFEN
		DEFINE_NAMEID(strApplyAnnounce);	//SIT_APPLY_ANNOUNCE
		//////////////////////////////////////////////////////////////////////////////////
		DEFINE_NAMEID(bIsAllTextDisable);	//列表
		DEFINE_NAMEID(bIsGuestTextDisable);//列表
		DEFINE_NAMEID(dwMemberLimit); //SIT_MEMBERLIMIT
		DEFINE_NAMEID(bRolerChanged); //SIT_ROLER_CHANGED
		DEFINE_NAMEID(bGuestApplyNotify);	//SIT_GUEST_APPLY_NOTIFY
		DEFINE_NAMEID(bSendImage); 	//SIT_SEND_IMAGE
		DEFINE_NAMEID(strIntroduce); //SIT_INTRODUCE
		DEFINE_NAMEID(btVoiceQc); 	//SIT_VOICEQC枚举
		DEFINE_NAMEID(dwGuestMaxTxtLen);	//SIT_GUEST_MAXTXTLEN
		DEFINE_NAMEID(dwGuestWaitTime);	//SIT_GUEST_WAITTIME
		DEFINE_NAMEID(dwGuestJoinMaiXu);	//SIT_GUEST_JOINMAIXU
		DEFINE_NAMEID(btMediaTransType);	//SIT_MEDIA_TRANFPORT_STYLE枚举
		DEFINE_NAMEID(bGuestTxtSpeed); //SIT_GUEST_TXTSPEED 
		DEFINE_NAMEID(bGuestAuthEnable);	//SIT_GUEST_AUTH_ENABLE 
		DEFINE_NAMEID(bGuestSmemberViewAndAddLimit); //SIT_GUEST_SMEMBER_VIEW_AND_ADD_LIMIT 
		DEFINE_NAMEID(bGuestEnableWhisperToManagers); //SIT_GUEST_ENABLE_WHISPER_TO_MANAGERS 
		DEFINE_NAMEID(dwGuestWhisperToManagersWaitime); //SIT_GUEST_WHISPER_TO_MANAGERS_WAITIME 
		DEFINE_NAMEID(bGuestAccessLimit);	//SIT_GUEST_ACCESS_LIMIT 
		DEFINE_NAMEID(bGuestAccessTopLimit);//SIT_GUEST_ACCESS_TOP_LIMIT
		DEFINE_NAMEID(bGuestVoice); //SIT_GUEST_VOICE 
		DEFINE_NAMEID(bSendUrlTxt); //SIT_SEND_URL_TXT 
		DEFINE_NAMEID(dwUinfoDisplayControl);//SIT_UINFO_DISPLAY_CONTROL
		DEFINE_NAMEID(dwServiceStatus);	//SIT_SERVICE_STATUS
		DEFINE_NAMEID(bGuestSendMsgLimit);	//SIT_GUEST_SEND_MSG_LIMIT
		DEFINE_NAMEID(strReceptionPrompt);	//SIT_RECEPTION_PROMPT
		DEFINE_NAMEID(strSessionCardPrefix);//SIT_SESSION_CARD_PREFIX
		DEFINE_NAMEID(strSessionCardSuffix);//SIT_SESSION_CARD_SUFFIX
		DEFINE_NAMEID(bLarge);  //SIT_LARGE
		DEFINE_NAMEID(bChorus); 	//SIT_CHORUS
		DEFINE_NAMEID(bGuestTxtBindMobile); //SIT_GUEST_TXT_BINDMOBILE, 游客需要绑定手机才能打字
		DEFINE_NAMEID(bSessionDoubleResend);//SIT_SESSION_DOUBLE_RESEND
		DEFINE_NAMEID(bCommandBroadcast);	//SIT_COMMANDBROADCAST
		DEFINE_NAMEID(bSessionChooseProxy);//SIT_SESSION_CHOOSE_PROXY
		DEFINE_NAMEID(btAssembleFrameNum); //SIT_ASSEMBLEFRAMENUM 低音质语音包帧数
		DEFINE_NAMEID(btAssembleQualityFrameNum); //SIT_ASSEMBLEQUALITYFRAMENUM 高音质语音包帧数
		DEFINE_NAMEID(btFrameTransMode);	//SIT_FRAMETRANSMODE  语音帧传输方式,0-连续传输,1-交错传输
		DEFINE_NAMEID(btCriticalFrameThreshold); //SIT_CRITICALFRAMETHRESHOLD  高能量帧阀值
		DEFINE_NAMEID(btServiceAccess);  //SIT_SERVICE_ACCESS
		DEFINE_NAMEID(bIsChargeCh);  //是否收费频道. 
		DEFINE_NAMEID(dwTemplateId);	//子频道模板
        DEFINE_NAMEID(dwGameZone);      //游戏区服.
		DEFINE_NAMEID(dwUserCount);  //当前子频道UI显示人数
		DEFINE_NAMEID(dwInnerUserCount);  //当前子频道中有多少人. 
		DEFINE_NAMEID(dwRealInnerUserCount);  //当前子频道中真实人数（大频道已订阅的子频道不为0，普通频道全部为0）
		DEFINE_NAMEID(bMemberSendUrlTxt); //会员受限模式
		DEFINE_NAMEID(dwVipgradeSendImage); //VIP发图等级

		DEFINE_NAMEID(bAuthenticated);	//SIT_AUTHENTICATED认证标志
		DEFINE_NAMEID(dwCreditValue); //SIT_CREDIT_VALUE频道信用
		DEFINE_NAMEID(bSubChannelSendPic); 	//SIT_SUBCHANNEL_SEND_PICTURE
        DEFINE_NAMEID(strDisbandApplyTime); ////申请频道解散的时间, "2013-12-31 12:30:31"
        DEFINE_NAMEID(dwDisbandStatus);     //频道解散的状态，1 等待解散，0 取消解散 
		DEFINE_NAMEID(dwMedalGrade); //SIT_MEDAL_GRADE
		DEFINE_NAMEID(bBuySessIdFlag); //SIT_BUY_SESSID_FLAG
		DEFINE_NAMEID(bShowRecruitInfo); //SIT_SHOW_RECRUIT_INFO
		DEFINE_NAMEID(bHideRecommendedLayer); //SIT_HIDE_RECOMMENDED_LAYER
		DEFINE_NAMEID(bBrocastEnableLimit); //SIT_BROADCAST_ENABLELIMIT
		DEFINE_NAMEID(dwBroadcastInterval); //SIT_BROADCAST_INTERVAL
		DEFINE_NAMEID(bHideShareMsg); //SIT_HIDE_SHARE_MSG
		DEFINE_NAMEID(bEnableQipao); //SIT_ENABLE_QIPAO
		DEFINE_NAMEID(dwAutoJiedai); //SIT_AUTO_JIEDAI
		DEFINE_NAMEID(dwAutoBroadcast); //SIT_AUTO_BROADCAST
		DEFINE_NAMEID(dwSubChanCount);   //SIT_SUBCHAN_COUNT 当前频道包含的子频道数目
		//////////////////////////////////////////////////////////////////////////////////
		DEFINE_NAMEID(dwArea);         //UIT_AREA
		DEFINE_NAMEID(dwProvince);       //UIT_PROVINCE
		DEFINE_NAMEID(dwCity);         //UIT_CITY
		DEFINE_NAMEID(strCreateTime);     //SIT_CREATETIME
		DEFINE_NAMEID(strGuestControl);    //SIT_GUESTCONTROL
		DEFINE_NAMEID(strJieDai);       //SIT_JIEDAI
		DEFINE_NAMEID(dw64RawJifen);      //SIT_JIFEN未除60的值.
		//这几个属性无用， 只是服务器会下发下来， 而本地要做频道信息md5校验， 当服务器不下发
		//这几个属性时， 则可以去掉这几个字段.
		//////////////////////////////////////////////////////////////////////////////////
	}

	DEFINE_NAMEID(fldChannelData);
	namespace ChannelFldSet
	{
		DEFINE_NAMEID(dwSid);  //顶级频道ID
		DEFINE_NAMEID(dwAliasId); 	//频道短位ID
		DEFINE_NAMEID(dwCurrentId); //当前频道ID  
		DEFINE_NAMEID(dwReceptionId); //接待频道ID  
		DEFINE_NAMEID(dwSGroupId); 	//Session Group Id.
		DEFINE_NAMEID(bMgmtPasswd);
		DEFINE_NAMEID(btCodecRate); 
		DEFINE_NAMEID(dwMyUid);
		DEFINE_NAMEID(dwTotalUserCnt);
		DEFINE_NAMEID(bChatText2); //是否支持公屏聊天ChatText2格式
		DEFINE_NAMEID(bHugeChannel);
        DEFINE_NAMEID(bUInfoTblShrink); //大频道模式下是否支持UInfo Table shrink...
        DEFINE_NAMEID(dwMPRecordIp); // 媒体前端录制IP.
        DEFINE_NAMEID(dwMPRecordPort); //媒体前端录制Port
        DEFINE_NAMEID(dwUIJoinType);    //UI进入频道入口种类.
        DEFINE_NAMEID(bSPLoginMultiplexing); //支持信令前段多路(>2)同时登陆策略. 
        DEFINE_NAMEID(bGotChannelInfo); //是否已经才从服务器上取到了频道信息(不包括本地缓存)
        DEFINE_NAMEID(bGetMediaProxyFromLinkd); //是否通过Linkd取得媒体前端地址. 
        DEFINE_NAMEID(bGetMediaProxyRetryBackoff);//是否打开重新请求前端退避策略
        DEFINE_NAMEID(bZipLoginMsg); //登录密钥，PLoginSignalProxy2消息是否压缩
        DEFINE_NAMEID(bTcpNodelay); //登录Tcp消息发送No_delay
        DEFINE_NAMEID(bOptSpIpMultiplexSwitch);
        DEFINE_NAMEID(bOptSpIpMSSwitch);
		DEFINE_NAMEID(bUseRolerFromSvc); //写入session/service哪一边的角色数据
        DEFINE_NAMEID(bChannelSvcV5);   //server V5 support
        DEFINE_NAMEID(bGetVideoProxyFromLinkd);
        DEFINE_NAMEID(bSendClient2GatewayMsg);
		DEFINE_NAMEID(bUseServiceVipInfo); //使用service侧的vip信息
		DEFINE_NAMEID(bUseFakeUid);//是否由sdk生成匿名uid
		DEFINE_NAMEID(bSuperChannels);//是否是超多子频道
		DEFINE_NAMEID(bInsertSuperChannelsFinish);//所有子频道信息是否插入完毕
	}

	//暂时用作service模块调用session模块函数. 
	DEFINE_NAMEID(tblSimpleOperator);
	namespace SimpleOperator
	{
		DEFINE_NAMEID(dwOperName);
		DEFINE_NAMEID(dwParam1);
		DEFINE_NAMEID(dwParam2);
		DEFINE_NAMEID(dwParam3);
		DEFINE_NAMEID(dwParam4);
		DEFINE_NAMEID(dwParam5);
		DEFINE_NAMEID(dwParam6);
		DEFINE_NAMEID(bParam1);
		DEFINE_NAMEID(bParam2);
		DEFINE_NAMEID(bTrigger);

		const DWORD FUNC_AFTER_LOADME   = 1; //No parameter
		const DWORD FUNC_CHANGE_MYFOLDER	  = 2; //dwParam1: toChannel;
		const DWORD FUNC_MIC_ONCHANGE   = 3; //dwParam1: operator(who); dwParam2: pid
		const DWORD FUNC_CHNL_CHANGE   = 4; //bParam1: sinoCacheMd5EQ, bParam2: bLoadCache
		const DWORD FUNC_ECHO_BULLETIN_CHANGE  = 5; //dwParam1: Updator
		const DWORD FUNC_SVC_SINFO_STATICS   = 6; //dwParam1: loginServdbExp, dwParam2: loginServSinfoExp, dwParam3: getServSinfoExp, dwParam4: sinfoSize, bParam1: bCacheMD5EQ
		const DWORD FUNC_ON_SINFO_SERVCACHE   = 7; //No Parameter
	}

	DEFINE_NAMEID(tblVipPrivilege);
	namespace VipPrivilege
	{
		DEFINE_NAMEID(wPrivilegeKey);
		DEFINE_NAMEID(strPrivilegeVal);
	}

	DEFINE_NAMEID(tblUinfoTemp);
	namespace UinfoTemp
	{
		DEFINE_NAMEID(dwUid);
		DEFINE_NAMEID(btSex);
		DEFINE_NAMEID(strSign);
		DEFINE_NAMEID(strNickname);
		DEFINE_NAMEID(dwYYId);
	}

	DEFINE_NAMEID(tblCompareTemp);
	namespace CompareTemp
	{
		DEFINE_NAMEID(dwTableKey);	
		DEFINE_NAMEID(tblUinfoTemp);
		DEFINE_NAMEID(tblChannelData);

		const DWORD	COMP_TABLE_KEY = 1;
	}

	DEFINE_NAMEID(tblManagers);
	namespace Managers
	{
		DEFINE_NAMEID(dwSid);
		DEFINE_NAMEID(dwUid);	
		DEFINE_NAMEID(dwRole);
		DEFINE_NAMEID(btGender);
		DEFINE_NAMEID(strNick);	
	}

	DEFINE_NAMEID(tblSessLinks);
	namespace SessLinks
	{
		DEFINE_NAMEID(dwConnId);
		DEFINE_NAMEID(dwConnType);
		DEFINE_NAMEID(bActive);
		DEFINE_NAMEID(dwIp);
		DEFINE_NAMEID(dwIsp);
		DEFINE_NAMEID(dwAreaType);
		DEFINE_NAMEID(dwConnTime);
		DEFINE_NAMEID(dwAvgRtt);
		DEFINE_NAMEID(dwLastRtt);
		DEFINE_NAMEID(dwErrTimes);

		//业务附加属性. 
		DEFINE_NAMEID(dwSubUidsSeqNo);
	}
}