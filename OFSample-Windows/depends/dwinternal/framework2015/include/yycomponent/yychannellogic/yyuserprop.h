#pragma once
#include "sysbase\tool\stringparser.h"
#include "yycomponent\propBag_i.h"
#include "biz\biz_consts.h"
#include "yycomponent\yycommon\yymobileconst.h"

class ChannelUserProp
{
public:
	ChannelUserProp(const IPropBagPtr spPropBag )
		: m_propBag(spPropBag)
	{
	}
	quint32 uid()
	{
		return stringparser::Parse2<quint32>( m_propBag->get(UPROP_Uid), 0);
	}
	quint32 yyid()
	{
		return stringparser::Parse2<quint32>( m_propBag->get(UPROP_Imid), -1);
	}
	QString nick()
	{
		return QString::fromWCharArray( m_propBag->get(UPROP_Nick) );
	}

	int mobileTypeId()
	{
		return stringparser::Parse2<int>(m_propBag->get(UPROP_DEVICE_PHONE), -1);
	}

	ROLE_IN_CHANNEL getRole()
	{
		return static_cast<ROLE_IN_CHANNEL>(stringparser::Parse2(m_propBag->get(UPROP_RoleInChannel), (quint32)0));
	}

	GENDER getGender()
	{
		return static_cast<GENDER>(stringparser::Parse2(m_propBag->get(UPROP_Gender), (quint32)0));
	}

	UINT32 getJifen()
	{
		return static_cast<UINT32>(stringparser::Parse2(m_propBag->get(UPROP_Jifen), (quint32)0));
	}

	UINT32 getLevel()
	{
		return static_cast<UINT32>(stringparser::Parse2(m_propBag->get(UPROP_Level), (quint32)0));
	}

	bool vipNeedRedName()
	{
		return static_cast<bool>( stringparser::Parse2(m_propBag->get(UPROP_VIP_REDNAME), (bool)false ) );
	}

	quint32 getVipLevel()
	{
		return static_cast<UINT32>( stringparser::Parse2(m_propBag->get(UPROP_VIP_LEVEL), (UINT32)0) );
	}

	bool isWebyy()
	{
        return stringparser::Parse2(m_propBag->get(UPROP_ACTIVE), (UINT32)0) == MobileConst::mbWebYY;
	}

    bool isPKWebyy()
    {
        return stringparser::Parse2(m_propBag->get(UPROP_ACTIVE), (UINT32)0) == MobileConst::mbPKWebYY;
    }

	UINT32 lianghao()
	{
		return stringparser::Parse2(m_propBag->get(UPROP_lianghao), (UINT32)0);
	}

    bool isLimitSeeNAddFriend()
    {
        return stringparser::Parse2<bool>(m_propBag->get(CPROP_IsLimitSeeNAddFriends), false);
    }
private:
	IPropBagPtr m_propBag;
};
