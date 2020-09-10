#pragma once

#include "dwbase/IData.h"
#include "detail/mainframeTables.h"

/**
* @brief ����Ƶ���б�����
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
	  * @brief ��ȡƵ��id
	  * @return UINT32 Ƶ��id
	  */
	UINT32 getSid() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwSid);
	}

	/**
	  * @brief ���Ƶ���̺�
	  * @return UINT32 Ƶ���̺�
	  */
	UINT32 getAsid() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwAliasId);
	}

	/**
	  * @brief ��ȡƵ������.
	  * @return Ƶ������.
	  */
	LPCWSTR getChannelName() const
	{
		return m_spChannelRow->getStr(Data::ChannelList::strName);
	}

	/**
	  * @brief ��ȡƵ��logo����ֵ.
	  * @return Ƶ��logo����ֵ.
	  */
	UINT32 getGuildLogoIndex() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwGuildLogo);
	}

	/**
	  * @brief ��ȡƵ���Զ���logoURL.
	  * @return Ƶ���Զ���logoURL.
	  */
	LPCWSTR getGuildCustomLogoURL() const
	{
		return m_spChannelRow->getStr(Data::ChannelList::strGuildLogoUrl);
	}

	/**
	  * @brief ȡ��Ƶ����ǰ��������
	  * @return UINT32 Ƶ����ǰ��������
	  */
	UINT32 getUsersCount() const
	{
		return m_spChannelRow->getDword(Data::ChannelList::dwUserCount);
	}

	/**
	  * @brief �Ƿ�Ƶ����Ա
	  * @return bool 
	  */
	bool bIsSettle() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsSettle);
	}

	/**
	  * @brief �ڵ�ǰƵ���Ľ�ɫ
	  * @return UINT32 
	  */
	UINT32 getRole() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::dwRole);
	}

	/**
	  * @brief Ƶ���Ƿ���������.
	  * @return Ƶ���Ƿ���������.
	  */
	bool isLimit() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsLimit);
	}

	/**
	  * @brief Ƶ����ע��
	  * @return LPCWSTR 
	  */
	LPCWSTR getRemark() const
	{
		return m_spChannelRow->getStr(Data::ChannelList::strRemark);
	}

	/**
	  * @brief ����Ƶ����ʽ����ͨƵ��/��������
	  * @return UINT8 
	  */
	BYTE getJoinType() const
	{
		return m_spChannelRow->getByte(Data::ChannelList::btJoinType);
	}

	/**
	  * @brief �Ƿ��ҵĹ���.
	  * @return �ҵĹ���.
	  */
	bool isMyGuild() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsMyGuild);
	}

	/**
	  * @brief �Ƿ����ղص�Ƶ��.
	  * @return �ղ�Ƶ��.
	  */
	bool isMyFavorChannel() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsMyFavorChannel);
	}

	/**
	  * @brief �Ƿ��������.
	  * @return �������.
	  */
	bool isMyRecentChannel() const
	{
		return m_spChannelRow->getBool(Data::ChannelList::bIsMyRecentChannel);
	}

	/**
	  * @brief �������ʱ�䣬ֻ�������ã���׼ȷ
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