#pragma once

#include "dwbase/IData.h"
#include "detail/mainframeTables.h"

/**
* @brief 本地频道列表辅助类
*/
class YYLocalChannelListfoHelper
{
public:
	YYLocalChannelListfoHelper( Data::IRowPtr spChannelRow )
	{
		m_spChannelRow = spChannelRow;
	}

	bool isValid() const
	{
		return m_spChannelRow != NULL;
	}

	/**
	  * @brief 获取频道id
	  * @return UINT32 频道id
	  */
	UINT32 getSid() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwSid);
	}

	/**
	  * @brief 获得频道短号
	  * @return UINT32 频道短号
	  */
	UINT32 getAsid() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwAliasId);
	}

	/**
	  * @brief 获取频道名字.
	  * @return 频道名字.
	  */
	LPCWSTR getChannelName() const
	{
		return m_spChannelRow->getStr(Data::ChannelList::strName);
	}

	/**
	  * @brief 获取频道logo索引值.
	  * @return 频道logo索引值.
	  */
	UINT32 getGuildLogoIndex() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwGuildLogo);
	}

	/**
	  * @brief 获取频道自定义logoURL.
	  * @return 频道自定义logoURL.
	  */
	LPCWSTR getGuildCustomLogoURL() const
	{
		return m_spChannelRow->getStr(Data::ChannelList::strGuildLogoUrl);
	}

	/**
	  * @brief 取得频道当前在线人数
	  * @return UINT32 频道当前在线人数
	  */
	UINT32 getUsersCount() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwUserCount);
	}

	/**
	  * @brief 是否频道会员
	  * @return bool 
	  */
	bool bIsSettle() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsSettle);
	}

	/**
	  * @brief 在当前频道的角色
	  * @return UINT32 
	  */
	UINT32 getRole() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::dwRole);
	}

	/**
	  * @brief 频道是否启用限制.
	  * @return 频道是否启用限制.
	  */
	bool isLimit() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsLimit);
	}

	/**
	  * @brief 频道备注名
	  * @return LPCWSTR 
	  */
	LPCWSTR getRemark() const
	{
		return m_spChannelRow->getStr(Data::ChannelList::strRemark);
	}

	/**
	  * @brief 进入频道方式，普通频道/精彩世界
	  * @return UINT8 
	  */
	BYTE getJoinType() const
	{
		return m_spChannelRow->getByte(Data::ChannelList::btJoinType);
	}

	/**
	  * @brief 是否我的公会.
	  * @return 我的公会.
	  */
	bool isMyGuild() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsMyGuild);
	}

	/**
	  * @brief 是否在收藏的频道.
	  * @return 收藏频道.
	  */
	bool isMyFavorChannel() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsMyFavorChannel);
	}

	/**
	  * @brief 是否最近访问.
	  * @return 最近访问.
	  */
	bool isMyRecentChannel() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsMyRecentChannel);
	}

	/**
	  * @brief 最近访问时间，只作排序用，不准确
	  * @return UINT64 
	  */
	UINT64 lastVisit() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::dw64LastVisit);
	}

private:
	YYLocalChannelListfoHelper(const YYLocalChannelListfoHelper&);
	YYLocalChannelListfoHelper& operator=(const YYLocalChannelListfoHelper&);
private:
	Data::IRowPtr m_spChannelRow;
};