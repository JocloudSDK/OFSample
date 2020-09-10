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
		DEFINE_NAMEID(dwPid);  //��pid������ʱ, UI���ɺ��������¼�.
		DEFINE_NAMEID(dwUinfoJifen);
		DEFINE_NAMEID(dwSmemberJifen);
		DEFINE_NAMEID(dwGamming); 	//��Ϸβ��: 
		DEFINE_NAMEID(dwActive); 	//�β��
		DEFINE_NAMEID(bIsSettle);
		DEFINE_NAMEID(btSex);
		DEFINE_NAMEID(strNickname);
		DEFINE_NAMEID(strSign);
		DEFINE_NAMEID(dwYYId);
		DEFINE_NAMEID(dwCreateTime);
		DEFINE_NAMEID(strCorpName);
		DEFINE_NAMEID(strCorpRoler);
		DEFINE_NAMEID(bIsAdminDisplay);  //
		DEFINE_NAMEID(bIsIgnoreText); //true: ���������� 
		DEFINE_NAMEID(bIsIgnoreVoice); //true: ������˵��
		DEFINE_NAMEID(bIsVoiceDisable);	//true: ����ֹ˵��
		DEFINE_NAMEID(bIsTextDisable); //true: ����ֹ����.
		DEFINE_NAMEID(bIsBindMobile);   //�Ƿ���ֻ�. 
		DEFINE_NAMEID(dwRole);  //���û�role�����仯ʱ, ���ֶλ�ı�, UI��Ҫ����ȡ�ý�ɫ��Ϣ. 
		DEFINE_NAMEID(bIsVip);  //�Ƿ�VIP
		DEFINE_NAMEID(dwVipJifen); 
		DEFINE_NAMEID(btVipLevel);
		DEFINE_NAMEID(bVipChannelRedName);	//VIP������ʶ. 
		DEFINE_NAMEID(bVipFace);      //VIPħ������
		DEFINE_NAMEID(dwVipChannelTop);  //Vip������ǰ.
        DEFINE_NAMEID(dwVipLiangNumber); //vip����. 
		DEFINE_NAMEID(bVipChannelPurpleName); //vip��ɫ��
        DEFINE_NAMEID(bPurpleDiamond); //����
        DEFINE_NAMEID(bPinkDiamond); //����
        DEFINE_NAMEID(bSuperVipName); //����VIP(VIP 6 + ����)
        DEFINE_NAMEID(bSuperPlDiamond); //�������� 
        DEFINE_NAMEID(dwPlDiamondGrade); //����ȼ�    value: 1-7 
        DEFINE_NAMEID(dwPlDiamondType);  //���긶������    value: 0 �·ѣ� 1 ���
        DEFINE_NAMEID(dwPinkDiamondGrade); //����ȼ�    value: 1-7 
        DEFINE_NAMEID(dwPinkDiamondType);  //���긶������    value: 0 �·ѣ� 1 ���
        DEFINE_NAMEID(dwBrilliantVipGrade);  //����VIP�ȼ�
		DEFINE_NAMEID(bPropCompFlag); //���ݶԱ��õ��������ֶ�
		DEFINE_NAMEID(dwTestIcon);     //���Դ���β��
		DEFINE_NAMEID(strPluginExtent);  //�����չ���ԣ�ʵ������Ϊ�û���չ���ԣ����ϲ���ȥ�������ҵ��
		DEFINE_NAMEID(strBasicCardURL);  //���ϻ�����Ƭ
		DEFINE_NAMEID(blobBasicCardData); //(��)������Ƭ,��������ֽ���,�ɿͻ��˽���(�����ж��URL)
		DEFINE_NAMEID(btQueryRetryNum);  //���������ȡ���Դ���. 
		DEFINE_NAMEID(dwRank);       //�û�����. 
		DEFINE_NAMEID(bVisual);      //��Ƶ��ģʽ�£��û��Ƿ���ʾ. 
		DEFINE_NAMEID(dwSortValue);			//Ƶ������ 
		DEFINE_NAMEID(dwSortTipsType);      //��������
        DEFINE_NAMEID(strArtistname);       //����
		DEFINE_NAMEID(bAnnoyUid);           //�����û�
		DEFINE_NAMEID(dwHugeChannelSubAnnoyUserCount);   //��Ƶ������Ƶ����������������Ƶ��ר��
		DEFINE_NAMEID(dwTermType);			//�ն�
	}

	namespace ChannelData
	{
		DEFINE_NAMEID(dwSid);  //SIT_SID
		DEFINE_NAMEID(dwPid);  //SIT_PID
		DEFINE_NAMEID(dwSubChannelOrder);	//SIT_SUBCHANNEL_ORDER(N)
		DEFINE_NAMEID(strName); 	//SIT_NAME
		DEFINE_NAMEID(bHasPasswd); 	//SIT_BHASPASSWD
		DEFINE_NAMEID(bIsPub);  //SIT_ISPUB
		DEFINE_NAMEID(btStyle); 	//SIT_STYLEö�� 
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
		DEFINE_NAMEID(btCodecRate); //SIT_CODECRATEö��
		DEFINE_NAMEID(btIspType); 	//SIT_ISPTYPEö��
		DEFINE_NAMEID(bIsLimit); 	//SIT_ISLIMIT
		DEFINE_NAMEID(btType);  //SIT_TYPEö��
		DEFINE_NAMEID(strTypeString); //SIT_TYPESTRING
		DEFINE_NAMEID(strOwner); 	//SIT_OWNER
		DEFINE_NAMEID(dwApplyJifen); //SIT_APPLY_JIFEN
		DEFINE_NAMEID(strApplyAnnounce);	//SIT_APPLY_ANNOUNCE
		//////////////////////////////////////////////////////////////////////////////////
		DEFINE_NAMEID(bIsAllTextDisable);	//�б�
		DEFINE_NAMEID(bIsGuestTextDisable);//�б�
		DEFINE_NAMEID(dwMemberLimit); //SIT_MEMBERLIMIT
		DEFINE_NAMEID(bRolerChanged); //SIT_ROLER_CHANGED
		DEFINE_NAMEID(bGuestApplyNotify);	//SIT_GUEST_APPLY_NOTIFY
		DEFINE_NAMEID(bSendImage); 	//SIT_SEND_IMAGE
		DEFINE_NAMEID(strIntroduce); //SIT_INTRODUCE
		DEFINE_NAMEID(btVoiceQc); 	//SIT_VOICEQCö��
		DEFINE_NAMEID(dwGuestMaxTxtLen);	//SIT_GUEST_MAXTXTLEN
		DEFINE_NAMEID(dwGuestWaitTime);	//SIT_GUEST_WAITTIME
		DEFINE_NAMEID(dwGuestJoinMaiXu);	//SIT_GUEST_JOINMAIXU
		DEFINE_NAMEID(btMediaTransType);	//SIT_MEDIA_TRANFPORT_STYLEö��
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
		DEFINE_NAMEID(bGuestTxtBindMobile); //SIT_GUEST_TXT_BINDMOBILE, �ο���Ҫ���ֻ����ܴ���
		DEFINE_NAMEID(bSessionDoubleResend);//SIT_SESSION_DOUBLE_RESEND
		DEFINE_NAMEID(bCommandBroadcast);	//SIT_COMMANDBROADCAST
		DEFINE_NAMEID(bSessionChooseProxy);//SIT_SESSION_CHOOSE_PROXY
		DEFINE_NAMEID(btAssembleFrameNum); //SIT_ASSEMBLEFRAMENUM ������������֡��
		DEFINE_NAMEID(btAssembleQualityFrameNum); //SIT_ASSEMBLEQUALITYFRAMENUM ������������֡��
		DEFINE_NAMEID(btFrameTransMode);	//SIT_FRAMETRANSMODE  ����֡���䷽ʽ,0-��������,1-������
		DEFINE_NAMEID(btCriticalFrameThreshold); //SIT_CRITICALFRAMETHRESHOLD  ������֡��ֵ
		DEFINE_NAMEID(btServiceAccess);  //SIT_SERVICE_ACCESS
		DEFINE_NAMEID(bIsChargeCh);  //�Ƿ��շ�Ƶ��. 
		DEFINE_NAMEID(dwTemplateId);	//��Ƶ��ģ��
        DEFINE_NAMEID(dwGameZone);      //��Ϸ����.
		DEFINE_NAMEID(dwUserCount);  //��ǰ��Ƶ��UI��ʾ����
		DEFINE_NAMEID(dwInnerUserCount);  //��ǰ��Ƶ�����ж�����. 
		DEFINE_NAMEID(dwRealInnerUserCount);  //��ǰ��Ƶ������ʵ��������Ƶ���Ѷ��ĵ���Ƶ����Ϊ0����ͨƵ��ȫ��Ϊ0��
		DEFINE_NAMEID(bMemberSendUrlTxt); //��Ա����ģʽ
		DEFINE_NAMEID(dwVipgradeSendImage); //VIP��ͼ�ȼ�

		DEFINE_NAMEID(bAuthenticated);	//SIT_AUTHENTICATED��֤��־
		DEFINE_NAMEID(dwCreditValue); //SIT_CREDIT_VALUEƵ������
		DEFINE_NAMEID(bSubChannelSendPic); 	//SIT_SUBCHANNEL_SEND_PICTURE
        DEFINE_NAMEID(strDisbandApplyTime); ////����Ƶ����ɢ��ʱ��, "2013-12-31 12:30:31"
        DEFINE_NAMEID(dwDisbandStatus);     //Ƶ����ɢ��״̬��1 �ȴ���ɢ��0 ȡ����ɢ 
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
		DEFINE_NAMEID(dwSubChanCount);   //SIT_SUBCHAN_COUNT ��ǰƵ����������Ƶ����Ŀ
		//////////////////////////////////////////////////////////////////////////////////
		DEFINE_NAMEID(dwArea);         //UIT_AREA
		DEFINE_NAMEID(dwProvince);       //UIT_PROVINCE
		DEFINE_NAMEID(dwCity);         //UIT_CITY
		DEFINE_NAMEID(strCreateTime);     //SIT_CREATETIME
		DEFINE_NAMEID(strGuestControl);    //SIT_GUESTCONTROL
		DEFINE_NAMEID(strJieDai);       //SIT_JIEDAI
		DEFINE_NAMEID(dw64RawJifen);      //SIT_JIFENδ��60��ֵ.
		//�⼸���������ã� ֻ�Ƿ��������·������� ������Ҫ��Ƶ����Ϣmd5У�飬 �����������·�
		//�⼸������ʱ�� �����ȥ���⼸���ֶ�.
		//////////////////////////////////////////////////////////////////////////////////
	}

	DEFINE_NAMEID(fldChannelData);
	namespace ChannelFldSet
	{
		DEFINE_NAMEID(dwSid);  //����Ƶ��ID
		DEFINE_NAMEID(dwAliasId); 	//Ƶ����λID
		DEFINE_NAMEID(dwCurrentId); //��ǰƵ��ID  
		DEFINE_NAMEID(dwReceptionId); //�Ӵ�Ƶ��ID  
		DEFINE_NAMEID(dwSGroupId); 	//Session Group Id.
		DEFINE_NAMEID(bMgmtPasswd);
		DEFINE_NAMEID(btCodecRate); 
		DEFINE_NAMEID(dwMyUid);
		DEFINE_NAMEID(dwTotalUserCnt);
		DEFINE_NAMEID(bChatText2); //�Ƿ�֧�ֹ�������ChatText2��ʽ
		DEFINE_NAMEID(bHugeChannel);
        DEFINE_NAMEID(bUInfoTblShrink); //��Ƶ��ģʽ���Ƿ�֧��UInfo Table shrink...
        DEFINE_NAMEID(dwMPRecordIp); // ý��ǰ��¼��IP.
        DEFINE_NAMEID(dwMPRecordPort); //ý��ǰ��¼��Port
        DEFINE_NAMEID(dwUIJoinType);    //UI����Ƶ���������.
        DEFINE_NAMEID(bSPLoginMultiplexing); //֧������ǰ�ζ�·(>2)ͬʱ��½����. 
        DEFINE_NAMEID(bGotChannelInfo); //�Ƿ��Ѿ��Ŵӷ�������ȡ����Ƶ����Ϣ(���������ػ���)
        DEFINE_NAMEID(bGetMediaProxyFromLinkd); //�Ƿ�ͨ��Linkdȡ��ý��ǰ�˵�ַ. 
        DEFINE_NAMEID(bGetMediaProxyRetryBackoff);//�Ƿ����������ǰ���˱ܲ���
        DEFINE_NAMEID(bZipLoginMsg); //��¼��Կ��PLoginSignalProxy2��Ϣ�Ƿ�ѹ��
        DEFINE_NAMEID(bTcpNodelay); //��¼Tcp��Ϣ����No_delay
        DEFINE_NAMEID(bOptSpIpMultiplexSwitch);
        DEFINE_NAMEID(bOptSpIpMSSwitch);
		DEFINE_NAMEID(bUseRolerFromSvc); //д��session/service��һ�ߵĽ�ɫ����
        DEFINE_NAMEID(bChannelSvcV5);   //server V5 support
        DEFINE_NAMEID(bGetVideoProxyFromLinkd);
        DEFINE_NAMEID(bSendClient2GatewayMsg);
		DEFINE_NAMEID(bUseServiceVipInfo); //ʹ��service���vip��Ϣ
		DEFINE_NAMEID(bUseFakeUid);//�Ƿ���sdk��������uid
		DEFINE_NAMEID(bSuperChannels);//�Ƿ��ǳ�����Ƶ��
		DEFINE_NAMEID(bInsertSuperChannelsFinish);//������Ƶ����Ϣ�Ƿ�������
	}

	//��ʱ����serviceģ�����sessionģ�麯��. 
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

		//ҵ�񸽼�����. 
		DEFINE_NAMEID(dwSubUidsSeqNo);
	}
}