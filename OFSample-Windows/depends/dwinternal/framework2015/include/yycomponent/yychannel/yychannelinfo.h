#pragma once

#include "dwbase/IData.h"
#include "SessionTables.h"
#include "biz/biz_types.h"


/**
* @brief .��ȡƵ����Ϣ������
*/
class YYChannelInfoHelper
{
public:
	YYChannelInfoHelper( Data::IRowPtr spChannelRow )
	{
		m_spChannelRow = spChannelRow;
	}

	bool isValid() const
	{
		return m_spChannelRow != NULL;
	}

/**
   * @brief ��ȡƵ��id.
   * @return Ƶ��i.
   */
	UINT32 getSid()
	{
		return  m_spChannelRow->getDword(Data::ChannelData::dwSid);
	}

/**
   * @brief ��ȡƵ������.
   * @return Ƶ������.
   */
	LPCWSTR getChannelName()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strName);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	bool isRoleChangeNotice()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bRolerChanged);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getOrder()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwSubChannelOrder);
	}

/**
   * @brief ��Ƶ���Ƿ����ý�������.
   * @return ���������뷵��true�����򷵻�false.
   */
	bool hasPassword()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bHasPasswd);
	}

/**
   * @brief ��ȡƵ��ģʽ.
   * @return Ƶ��ģʽ.
   */
	std::wstring getChannelStyle()
	{
		CHANNEL_STYLE channel = (CHANNEL_STYLE)m_spChannelRow->getByte(Data::ChannelData::btStyle);
		return stringizing::ToString(channel);
	}

/**
   * @brief ��ȡƵ�����֣�����Ƶ����Ч.
   * @return Ƶ������.
   */
	UINT64 getJiFen()
	{
		return m_spChannelRow->getDword64(Data::ChannelData::dw64JiFen);
	}

/**
   * @brief ��ȡƵ��logo����ֵ.
   * @return Ƶ��logo����ֵ.
   */
	UINT32 getGuildLogoIndex()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuildLogo);
	}

/**
   * @brief ��ȡƵ���Զ���logoURL.
   * @return Ƶ���Զ���logoURL.
   */
	LPCWSTR getGuildCustomLogoURL()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strGuildLogoUrl);
	}

/**
   * @brief Ƶ���Ƿ���������.
   * @return Ƶ���Ƿ���������.
   */
	bool isLimit()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsLimit);
	}

/**
   * @brief ��ȡƵ����������.
   * @return Ƶ����������.
   */
	std::wstring getGuildType()
	{
		GUILD_TYPE guild = (GUILD_TYPE)m_spChannelRow->getByte(Data::ChannelData::btType);
		return stringizing::ToString(guild);
	}

/**
   * @brief ��ȡƵ������������Ϸ.
   * @return Ƶ������������Ϸ.
   */
	LPCWSTR getGuildMainGame()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strTypeString);
	}

/**
   * @brief Ƶ���Ƿ���οͿ���.
   * @return Ƶ���Ƿ���οͿ���.
   */
	bool isPub()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsPub);
	}

/**
   * @brief ��ȡ��������.
   * @return ��������.
   */
	LPCWSTR getBulletin()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strBulletin);
	}

/**
   * @brief ��ȡ.
   * @return �������ʱ��.
   */
	UINT32 getBulletinTimeStamp()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwBulletinTimeStamp);
	}

/**
   * @brief ��ȡƵ����Ա��������.
   * @return Ƶ����Ա��������.
   */
	UINT32 getMemberCountLimit()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwMemberLimit);
	}

/**
   * @brief ��ȡƵ������.
   * @return Ƶ������.
   */
	UINT32 getPaiMing()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwPaiMing);
	}

/**
   * @brief ��ȡƵ�����.
   * @return Ƶ�����.
   */
	LPCWSTR getIntro()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strIntroduce);
	}

/**
   * @brief ��ȡ����ʱ��.
   * @return ����ʱ��.
   */
	UINT32 getMaixuTime()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwMicroTime);
	}

/**
   * @brief Ƶ���Ƿ����÷��������ٶ�����.
   * @return �Ƿ����÷��������ٶ�����.
   */
	bool isLimitText()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsTxtLimit);
	}

/**
   * @brief ��ȡ�������ּ��.
   * @return �������ּ��.
   */
	UINT32 getTextLimitTime()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwTxtLimitTime);
	}

/**
   * @brief ��ȡ�ο������������ı�����.
   * @return �ο������������ı�����.
   */
	UINT32 getGuestMaxTextLen()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuestMaxTxtLen);
	}

/**
   * @brief �ο��Ƿ��ܹ�����.
   * @return ���Ƿ��ܹ�����.
   */
	UINT32 canGuestJoinMaixu()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuestJoinMaiXu) != 0;
	}

/**
   * @brief �οͽ���Ƶ������ʱ�����Է�������.
   * @return �οͽ���Ƶ������ʱ�����Է�������.
   */
	UINT32 getGuestWaitTime()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuestWaitTime);
	}

/**
   * @brief Ƶ���Ƿ������ο�����.
   * @return Ƶ���Ƿ������ο�����.
   */
	bool isGuestLimitEnabled()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestEnableLimit);
	}

/**
   * @brief �������������Ƿ�ֻ����ο�.
   * @return �������������Ƿ�ֻ����ο�.
   */
	bool isLimitTextOnlyForGuest()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestTxtSpeed);
	}

/**
   * @brief �ο��Ƿ��ܹ�����.
   * @return�ο��Ƿ��ܹ����� .
   */
	bool isLimitGuestJoin()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestAccessLimit);
	}

/**
   * @brief ����ģʽ���ο��Ƿ�������.
   * @return ����ģʽ���ο��Ƿ�������.
   */
	bool canGuestVoiceChat()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestVoice);
	}

/**
   * @brief �ο��Ƿ��ܷ���URL���ɻ�Ʊ.
   * @return �ο��Ƿ��ܷ���URL���ɻ�Ʊ .
   */
	bool canSendUrlText()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bSendUrlTxt);
	}

/**
   * @brief ��Ա�Ƿ��ܷ���URL���ɻ�Ʊ.
   * @return ��Ա�Ƿ��ܷ���URL���ɻ�Ʊ.
   */
	bool canMemberSendUrlText()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bMemberSendUrlTxt);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getEnrollmentJifenLimit()
	{	
		return m_spChannelRow->getDword(Data::ChannelData::dwApplyJifen);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	LPCWSTR getEnrollmentNotice()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strApplyAnnounce);
	}

/**
   * @brief �ο��Ƿ��ܹ���ת������.
   * @return �ο��Ƿ��ܹ���ת������.
   */
	bool canGuestAccessTop()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestAccessTopLimit);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	std::wstring getIspType()
	{
		ISP_TYPE isp = (ISP_TYPE)m_spChannelRow->getByte(Data::ChannelData::btIspType);
		return stringizing::ToString(isp);
	}

/**
   * @brief �ο�֮���Ƿ��ܹ�˽��.
   * @return �ο�֮���Ƿ��ܹ�˽��.
   */
	bool canPrivateChat()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestSendMsgLimit);
	}

/**
   * @brief Ƶ����Ƭǰ׺.
   * @return Ƶ����Ƭǰ׺.
   */
	LPCWSTR getSessionCardPrefix()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strSessionCardPrefix);
	}

/**
   * @brief Ƶ����Ƭ��׺.
   * @return Ƶ����Ƭ��׺.
   */
	LPCWSTR getSessionCardSuffix()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strSessionCardSuffix);
	}

/**
   * @brief Ƶ���Ƿ��Զ��Ӵ�.
   * @return Ƶ���Ƿ��Զ��Ӵ�.
   */
	bool isAutoJieDai()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bEnableReceptionConfig);
	}

/**
   * @brief ��ȡ�Ӵ���.
   * @return �Ӵ���.
   */
	LPCWSTR getSayJieDai()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strReceptionPrompt);
	}

/**
   * @brief Ƶ�������Ƿ�Ҫ���ֻ�.
   * @return Ƶ�������Ƿ�Ҫ���ֻ�.
   */
	bool isMobilePhoneBinding()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestTxtBindMobile);
	}
	UINT32 getOnlineUsersCount()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwUserCount);
	}
	UINT32 getUsersCount()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwInnerUserCount);
	}
	bool isHugeChannel()
	{
		return false;
	}
	bool isChargeChannel()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsChargeCh) != FALSE;
	}
	UINT32 getTemplateId()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwTemplateId);
	}
	UINT32 getArea()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwArea);
	}
	bool isMedalVioletGuild()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bMedalVioletGolenSession) != FALSE;
	}
	LPCWSTR getMedalVioletLogoUrl()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strMedalVioletLogoUrl);
	}

	LPCWSTR getMedalName()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strMedalName);
	}
	LPCWSTR getMedalDescription()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strMedalDescription);
	}
	LPCWSTR getMedalLinkUrl()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strMedalLinkUrl);
	}
	UINT32 getMedalID()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwMedalID);
	}
	UINT32 getMedalGrade()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwMedalGrade);
	}
	UINT32 getVipgradeSendImage()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwVipgradeSendImage);
	}

    bool isLiveRecommendHidden() const
    {
        return m_spChannelRow->getBool(Data::ChannelData::bHideRecommendedLayer) != FALSE;
    }

	bool getBuySessIdFlag() const
	{
		return m_spChannelRow->getBool(Data::ChannelData::bBuySessIdFlag) != FALSE;
	}

	UINT32 getCreditValue() const
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwCreditValue);
	}

	bool isAuthenticated() const
	{
		return m_spChannelRow->getBool(Data::ChannelData::bAuthenticated) != FALSE;
	}

	UINT32 getGameServer() const
	{
		return m_spChannelRow->hasData(Data::ChannelData::dwGameZone) ?
			m_spChannelRow->getDword(Data::ChannelData::dwGameZone) : -1;
	}

	bool canCurSubChannelSendImage()
	{
		if (!m_spChannelRow->hasData(Data::ChannelData::bSubChannelSendPic))
		{
			return true;
		}
		else
		{
			return m_spChannelRow->getBool(Data::ChannelData::bSubChannelSendPic) != FALSE;
		}
	}

	bool isShowRecruitInfo()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bShowRecruitInfo) != FALSE;
	}

    bool isBroadcastLimitEnabled() const
    {
        return m_spChannelRow->getBool(Data::ChannelData::bBrocastEnableLimit) != FALSE;
    }

    UINT32 getBroadcastLimitInterval() const
    {
        return m_spChannelRow->getDword(Data::ChannelData::dwBroadcastInterval);
    }

    bool isEnableChattingBubble() const
    {
        return false; // 8.49�汾���߹���
    }

private:
	YYChannelInfoHelper(const YYChannelInfoHelper&);
	YYChannelInfoHelper& operator=(const YYChannelInfoHelper&);
private:
	Data::IRowPtr m_spChannelRow;
};