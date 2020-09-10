#pragma once

#include "dwbase/IData.h"
#include "SessionTables.h"
#include "biz/biz_types.h"


/**
* @brief .读取频道信息辅助类
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
   * @brief 获取频道id.
   * @return 频道i.
   */
	UINT32 getSid()
	{
		return  m_spChannelRow->getDword(Data::ChannelData::dwSid);
	}

/**
   * @brief 获取频道名字.
   * @return 频道名字.
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
   * @brief 子频道是否设置进入密码.
   * @return 有设置密码返回true，否则返回false.
   */
	bool hasPassword()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bHasPasswd);
	}

/**
   * @brief 获取频道模式.
   * @return 频道模式.
   */
	std::wstring getChannelStyle()
	{
		CHANNEL_STYLE channel = (CHANNEL_STYLE)m_spChannelRow->getByte(Data::ChannelData::btStyle);
		return stringizing::ToString(channel);
	}

/**
   * @brief 获取频道积分，对子频道无效.
   * @return 频道积分.
   */
	UINT64 getJiFen()
	{
		return m_spChannelRow->getDword64(Data::ChannelData::dw64JiFen);
	}

/**
   * @brief 获取频道logo索引值.
   * @return 频道logo索引值.
   */
	UINT32 getGuildLogoIndex()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuildLogo);
	}

/**
   * @brief 获取频道自定义logoURL.
   * @return 频道自定义logoURL.
   */
	LPCWSTR getGuildCustomLogoURL()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strGuildLogoUrl);
	}

/**
   * @brief 频道是否启用限制.
   * @return 频道是否启用限制.
   */
	bool isLimit()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsLimit);
	}

/**
   * @brief 获取频道公会类型.
   * @return 频道公会类型.
   */
	std::wstring getGuildType()
	{
		GUILD_TYPE guild = (GUILD_TYPE)m_spChannelRow->getByte(Data::ChannelData::btType);
		return stringizing::ToString(guild);
	}

/**
   * @brief 获取频道公会主玩游戏.
   * @return 频道公会主玩游戏.
   */
	LPCWSTR getGuildMainGame()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strTypeString);
	}

/**
   * @brief 频道是否对游客开放.
   * @return 频道是否对游客开放.
   */
	bool isPub()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsPub);
	}

/**
   * @brief 获取公告内容.
   * @return 公告内容.
   */
	LPCWSTR getBulletin()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strBulletin);
	}

/**
   * @brief 获取.
   * @return 公告更新时间.
   */
	UINT32 getBulletinTimeStamp()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwBulletinTimeStamp);
	}

/**
   * @brief 获取频道会员人数限制.
   * @return 频道会员人数限制.
   */
	UINT32 getMemberCountLimit()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwMemberLimit);
	}

/**
   * @brief 获取频道排名.
   * @return 频道排名.
   */
	UINT32 getPaiMing()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwPaiMing);
	}

/**
   * @brief 获取频道简介.
   * @return 频道简介.
   */
	LPCWSTR getIntro()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strIntroduce);
	}

/**
   * @brief 获取麦序时间.
   * @return 麦序时间.
   */
	UINT32 getMaixuTime()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwMicroTime);
	}

/**
   * @brief 频道是否启用发送文字速度限制.
   * @return 是否启用发送文字速度限制.
   */
	bool isLimitText()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bIsTxtLimit);
	}

/**
   * @brief 获取发送文字间隔.
   * @return 发送文字间隔.
   */
	UINT32 getTextLimitTime()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwTxtLimitTime);
	}

/**
   * @brief 获取游客最长可以输入的文本长度.
   * @return 游客最长可以输入的文本长度.
   */
	UINT32 getGuestMaxTextLen()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuestMaxTxtLen);
	}

/**
   * @brief 游客是否能够抢麦.
   * @return 客是否能够抢麦.
   */
	UINT32 canGuestJoinMaixu()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuestJoinMaiXu) != 0;
	}

/**
   * @brief 游客进入频道多少时间后可以发送文字.
   * @return 游客进入频道多少时间后可以发送文字.
   */
	UINT32 getGuestWaitTime()
	{
		return m_spChannelRow->getDword(Data::ChannelData::dwGuestWaitTime);
	}

/**
   * @brief 频道是否启用游客限制.
   * @return 频道是否启用游客限制.
   */
	bool isGuestLimitEnabled()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestEnableLimit);
	}

/**
   * @brief 发送文字限制是否只针对游客.
   * @return 发送文字限制是否只针对游客.
   */
	bool isLimitTextOnlyForGuest()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestTxtSpeed);
	}

/**
   * @brief 游客是否能够进入.
   * @return游客是否能够进入 .
   */
	bool isLimitGuestJoin()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestAccessLimit);
	}

/**
   * @brief 自由模式下游客是否能语音.
   * @return 自由模式下游客是否能语音.
   */
	bool canGuestVoiceChat()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestVoice);
	}

/**
   * @brief 游客是否能发送URL，飞机票.
   * @return 游客是否能发送URL，飞机票 .
   */
	bool canSendUrlText()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bSendUrlTxt);
	}

/**
   * @brief 会员是否能发送URL，飞机票.
   * @return 会员是否能发送URL，飞机票.
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
   * @brief 游客是否能够跳转到大厅.
   * @return 游客是否能够跳转到大厅.
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
   * @brief 游客之间是否能够私聊.
   * @return 游客之间是否能够私聊.
   */
	bool canPrivateChat()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bGuestSendMsgLimit);
	}

/**
   * @brief 频道名片前缀.
   * @return 频道名片前缀.
   */
	LPCWSTR getSessionCardPrefix()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strSessionCardPrefix);
	}

/**
   * @brief 频道名片后缀.
   * @return 频道名片后缀.
   */
	LPCWSTR getSessionCardSuffix()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strSessionCardSuffix);
	}

/**
   * @brief 频道是否自动接待.
   * @return 频道是否自动接待.
   */
	bool isAutoJieDai()
	{
		return m_spChannelRow->getBool(Data::ChannelData::bEnableReceptionConfig);
	}

/**
   * @brief 获取接待语.
   * @return 接待语.
   */
	LPCWSTR getSayJieDai()
	{
		return m_spChannelRow->getStr(Data::ChannelData::strReceptionPrompt);
	}

/**
   * @brief 频道发言是否要绑定手机.
   * @return 频道发言是否要绑定手机.
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
        return false; // 8.49版本下线功能
    }

private:
	YYChannelInfoHelper(const YYChannelInfoHelper&);
	YYChannelInfoHelper& operator=(const YYChannelInfoHelper&);
private:
	Data::IRowPtr m_spChannelRow;
};