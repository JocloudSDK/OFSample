#pragma once
#include "sessiontables.h"
#include "biz/biz_types.h"
#include "yychannelmarshall_i.h"
#include "yycomponent/yycommon/yymobileconst.h"
#include "yycomponent/yychannel/yychannelinfo_i.h"
#include "yycomponent/detail/specialwordfilter.h"

#ifdef DW_COMPONENT_ENV
#undef DW_COMPONENT_ENV
#define DW_COMPONENT_ENV_DEFINED
#endif//DW_COMPONENT_ENV
#include "dwbase/dwcommgr_i.h"
class YYChannelUInfo
{
public:
	YYChannelUInfo( Data::IRowPtr spRow ) : m_spUInfoRow(spRow)
	{
	}
/**
   * @brief .
   * @param .
   * @return .
   */
	bool isValid() const
	{
		return m_spUInfoRow != NULL;
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getUid()
	{
		return m_spUInfoRow->getDword(Data::Uinfo::dwUid);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	LPCWSTR getNick()
	{
		return m_spUInfoRow->hasData(Data::Uinfo::strNickname) ? m_spUInfoRow->getStr(Data::Uinfo::strNickname) : L"";
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	GENDER getGender()
	{
		return (GENDER)m_spUInfoRow->getByte(Data::Uinfo::btSex);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	ROLE_IN_CHANNEL getRole()
	{
		return (ROLE_IN_CHANNEL)m_spUInfoRow->getDword(Data::Uinfo::dwRole);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	bool isIgnoreVoice()
	{
		return m_spUInfoRow->getBool(Data::Uinfo::bIsIgnoreVoice);
	}

	UINT32 getSpeakVolume()
	{
		return m_spUInfoRow->hasProp(Data::Uinfo::bIsIgnoreVoice) ? m_spUInfoRow->getPropDword(Data::Uinfo::bIsIgnoreVoice) : 50;
	}

	void setSpeakVolume(UINT32 volume)
	{
		m_spUInfoRow->setPropDword(Data::Uinfo::bIsIgnoreVoice, volume);
	}

	UINT32 getRank()
	{
		return m_spUInfoRow->hasData(Data::Uinfo::dwRank) ? m_spUInfoRow->getDword(Data::Uinfo::dwRank) : 1;
	}

	bool isOrderValid()
	{
		//新增人员默认为true
		return m_spUInfoRow->hasData(Data::Uinfo::bVisual) ? (m_spUInfoRow->getBool(Data::Uinfo::bVisual) != FALSE) : true;
	}
/**
   * @brief .
   * @param .
   * @return .
   */
	bool isIgnoreText()
	{
		return m_spUInfoRow->getBool(Data::Uinfo::bIsIgnoreText);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	LPCWSTR getSign()
	{
		return m_spUInfoRow->hasData(Data::Uinfo::strSign) ? m_spUInfoRow->getStr(Data::Uinfo::strSign) : L"";
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getJifen()
	{
		return  m_spUInfoRow->getDword(Data::Uinfo::dwUinfoJifen);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getLevel()
	{
		UINT32 jifen = m_spUInfoRow->getDword(Data::Uinfo::dwUinfoJifen);
		return (UINT32) ((sqrt((double)jifen * 16 + 1) - 1) / 2 + 1);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getJifenInChannel()
	{
		return m_spUInfoRow->getDword(Data::Uinfo::dwSmemberJifen);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	bool isSettled()
	{
		return m_spUInfoRow->getBool(Data::Uinfo::bIsSettle) ? true : false;
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	bool isBindPhone()
	{
		return m_spUInfoRow->getBool(Data::Uinfo::bIsBindMobile);
	}

	bool hasPrivilege( UINT16 specInfoType )
	{
		switch(specInfoType)
		{
		case emPT_Channel_Red_Name://VIP红名标识
			if(m_spUInfoRow->hasData(Data::Uinfo::bVipChannelRedName) && m_spUInfoRow->getBool(Data::Uinfo::bVipChannelRedName))
				return true;
			return false;
		case emPT_Channel_Vip_Face://VIP魔法表情
			if(m_spUInfoRow->hasData(Data::Uinfo::bVipFace) && m_spUInfoRow->getBool(Data::Uinfo::bVipFace))
				return true;
			return false;
		case emPT_Channel_Top://Vip排名靠前.
			return m_spUInfoRow->hasData(Data::Uinfo::dwVipChannelTop) != FALSE;
        case emPT_Channel_Purple_Name:
            return m_spUInfoRow->hasData(Data::Uinfo::bVipChannelPurpleName) && m_spUInfoRow->getBool(Data::Uinfo::bVipChannelPurpleName);
		}
		return false;
	}

	bool enableTextInChannel()
	{
		if(isIgnoreText())
		{
			return false;
		}

		//该用户被禁止打字
		if(m_spUInfoRow->getBool(Data::Uinfo::bIsTextDisable))
		{
			return false;
		}

		IYYChannelInfomationPtr info = comMgr()->getServiceInterface<IYYChannelInfomation>(KSERVICEChannel);
		if(!info)
		{
			return true;
		}
		Data::IRowPtr spChannelRow = info->getChannelRow(m_spUInfoRow->getDword(Data::Uinfo::dwPid));
		if(!spChannelRow)
		{
			return true;
		}

		//所有用户禁止打字
		if(spChannelRow->getBool(Data::ChannelData::bIsAllTextDisable) && getRole() < ROLE_IN_CHANNEL_CMANAGER)
		{
			return false;
		}

		//游客禁止打字
		if(spChannelRow->getBool(Data::ChannelData::bIsGuestTextDisable) && getRole() <= ROLE_IN_CHANNEL_NORMAL)
		{
			return false;
		}

		return true;
	}

	bool enableVoiceInChannel()
	{
		if(isIgnoreVoice())
		{
			return false;
		}

		//该用户被禁止说话
		if(m_spUInfoRow->getBool(Data::Uinfo::bIsVoiceDisable))
		{
			return false;
		}

		IYYChannelInfomationPtr info = comMgr()->getServiceInterface<IYYChannelInfomation>(KSERVICEChannel);
		if (!info)
		{
			return false;
		}

		Data::IRowPtr spChannelRow = info->getChannelRow(m_spUInfoRow->getDword(Data::Uinfo::dwPid));
		if(!spChannelRow)
		{
			return true;
		}

		if(getRole() <= ROLE_IN_CHANNEL_NORMAL && 
			spChannelRow->getByte(Data::ChannelData::btStyle) == CHANNEL_STYLE_FREE &&
			spChannelRow->getBool(Data::ChannelData::bGuestVoice) )
		{
			return false;
		}

		return true;
	}

/**
   * @brief .
   * @param .
   * @return .
   */
    bool isPurpleDiamond()
    {
        return m_spUInfoRow->getBool(Data::Uinfo::bPurpleDiamond);
    }

    bool isSuperVip()
    {
        return m_spUInfoRow->getBool(Data::Uinfo::bSuperVipName);
    }

	bool isVIP()
	{
		return m_spUInfoRow->getBool(Data::Uinfo::bIsVip) ? true : false;
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getVipLevel()
	{
		return m_spUInfoRow->getByte(Data::Uinfo::btVipLevel);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	BOOL isVipFace()
	{
		return m_spUInfoRow->getBool(Data::Uinfo::bVipFace);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	DWORD getVipChannelTop()
	{
		return m_spUInfoRow->getDword(Data::Uinfo::dwVipChannelTop);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getYYNumber()
	{
		return m_spUInfoRow->getDword(Data::Uinfo::dwYYId);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	UINT32 getPid()
	{
		return m_spUInfoRow->getDword(Data::Uinfo::dwPid); 
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	bool hasFullInfo()
	{ 
		if( m_spUInfoRow->hasData(Data::Uinfo::dwPid) &&
			m_spUInfoRow->hasData(Data::Uinfo::strNickname))
		{
			return true;
		}

		return false; 
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	LPCWSTR getCorps()
	{
		return m_spUInfoRow->hasData(Data::Uinfo::strCorpName) ? m_spUInfoRow->getStr(Data::Uinfo::strCorpName) : L"";
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	LPCWSTR getCorpsPosition()
	{
		return m_spUInfoRow->hasData(Data::Uinfo::strCorpRoler) ? m_spUInfoRow->getStr(Data::Uinfo::strCorpRoler) : L"";
	}

	UINT32 getTestIcon()
	{
		return m_spUInfoRow->getDword(Data::Uinfo::dwTestIcon);
	}

	std::wstring getDisplayNick()
	{
		if(!m_spUInfoRow->hasData(Data::Uinfo::strNickname))
		{
			return L"";
		}

		LPCWSTR str = m_spUInfoRow->getStr(Data::Uinfo::strNickname);
		if(specialwordfilter_helper::isNeedFilter(str))
		{
			std::wstring nick = str;
			specialwordfilter_helper::filterNick(nick);
			return nick;
		}

		return str;
	}

	bool hasExtentProps()
	{
		DWORD len = 0;
		const char* pBytes =  (const char*)m_spUInfoRow->getBlob(Data::Uinfo::strPluginExtent, &len);
		return len > 0 && pBytes != NULL;
	}

	LPCWSTR getUrl()
	{
		return m_spUInfoRow->getStr(Data::Uinfo::strBasicCardURL);
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	std::string getExtentProp(IYYChannelMarshalPtr spMarshal, int key )
	{
		DW_ASSERT(spMarshal);

		if ( !m_props.empty() )
		{
			return findKey(key);
		}
		if( !spMarshal )
		{
			return std::string();
		}

		DWORD len = 0;
		const char* pByte =(const char*) m_spUInfoRow->getBlob(Data::Uinfo::strPluginExtent, &len);
		if ( pByte )
		{
/**
   * @brief .
   * @param .
   * @return .
   */
			std::string buff( pByte, len );
			spMarshal->unmarshal(buff, m_props);

			return findKey(key);
		}
		return std::string();
	}

/**
   * @brief .
   * @param .
   * @return .
   */
	LPCWSTR strBasicCardURL()
	{
		return m_spUInfoRow->hasData(Data::Uinfo::strBasicCardURL) ? m_spUInfoRow->getStr(Data::Uinfo::strBasicCardURL) : L"";
	}

    bool isVoiceDisabled()
    {
        //该用户被禁止说话
        return m_spUInfoRow->getBool(Data::Uinfo::bIsVoiceDisable);
    }

    bool isDataValid(const NAME_ID& name) const
    {
        return (m_spUInfoRow && m_spUInfoRow->hasData(name));
    }
    bool isLevelValid() const
    {
        return isDataValid(Data::Uinfo::dwUinfoJifen);
    }

    bool isWebyyUser() const
    {
        int activityId = 0;
        if (m_spUInfoRow && m_spUInfoRow->hasData(Data::Uinfo::dwPid) && m_spUInfoRow->hasData(Data::Uinfo::strNickname))
        {
            activityId = LOWORD(m_spUInfoRow->getDword(Data::Uinfo::dwActive));
        }

        return  activityId == MobileConst::mbWebYY;
    }

    bool isPKWebyyUser() const
    {
        int activityId = 0;
        if (m_spUInfoRow && m_spUInfoRow->hasData(Data::Uinfo::dwPid) && m_spUInfoRow->hasData(Data::Uinfo::strNickname))
        {
            activityId = LOWORD(m_spUInfoRow->getDword(Data::Uinfo::dwActive));
        }

        return  activityId == MobileConst::mbPKWebYY;
    }


	bool isAnonymousUser() const
	{
		if (m_spUInfoRow && m_spUInfoRow->hasData(Data::Uinfo::bAnnoyUid))
		{
			return m_spUInfoRow->getBool(Data::Uinfo::bAnnoyUid);
		}
		return false;
	}

	UINT32 getHugeChannelSubAnnoyUserCount() const
	{
		if (m_spUInfoRow && m_spUInfoRow->hasData(Data::Uinfo::dwHugeChannelSubAnnoyUserCount))
		{
			return m_spUInfoRow->getDword(Data::Uinfo::dwHugeChannelSubAnnoyUserCount);
		}
		return 0;
	}

private:
/**
   * @brief .
   * @param .
   * @return .
   */
	std::string findKey(int key)
	{
		std::map<UINT16, std::string>::iterator itFind = m_props.find(key);
		if ( itFind != m_props.end() )
		{
			return itFind->second;
		}

		return std::string();
	}

private:
	Data::IRowPtr	 m_spUInfoRow;
	std::map<UINT16, std::string> m_props;
};

#ifdef DW_COMPONENT_ENV_DEFINED
#define DW_COMPONENT_ENV
#endif	//DW_COMPONENT_ENV_DEFINED