#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yychannel/yyproperty.h"

typedef  QMap<int,QVariant> JoinInfoType;
DWBIND_INTERFACE(JoinInfoType, DWMAKE_INTERFACE_UUID_NAME(JoinInfoType))

DWDEFINE_INTERFACE(IYYChannelLogic):public IUnk
{
	enum JoinInfo
	{
		KSid = 0,
		KSubId = 1,
		KDisplayId = 2,
		KUserData = 3,
		KJoinStyle = 4,
		KNeedShowTop = 5,
		KNeedActive = 6,
        KJoinReportInfo = 7,
		KEntrance = 8,
	};

	enum ChannelStyleType
	{
		NormalStyle = 1,
		FullCustomeStyle = 2,
		FullControlStyle = 3
	};

	typedef yyproperty::property<ChannelStyleType> StylePropType; 

	virtual JoinInfoType getJoinInfo() = 0;
	virtual quint32 currentTemplateId() = 0;
	virtual StylePropType& pro_Style() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_JOININFO_CHANGED;
	virtual _SIG_JOININFO_CHANGED* getSigJoinInfoChanged() = 0;
};

class JoinInfoHelper
{
public:
	JoinInfoHelper( JoinInfoType& joinInfo )
		:m_joinInfo(joinInfo)
	{

	}

	void setSid( quint32 sid )
	{
		m_joinInfo[IYYChannelLogic::KSid] = sid;
	}

	quint32 sid() const
	{
		return m_joinInfo[IYYChannelLogic::KSid].toUInt();
	}

	void setSubId( quint32 subId )
	{
		m_joinInfo[IYYChannelLogic::KSubId] = subId;
	}

	quint32 subId() const
	{
		return m_joinInfo[IYYChannelLogic::KSubId].toUInt();
	}

	void setDisplayId( quint32 displayId )
	{
		m_joinInfo[IYYChannelLogic::KDisplayId] = displayId;
	}

	quint32 displayId() const
	{
		return m_joinInfo[IYYChannelLogic::KDisplayId].toUInt();

	}
	void setJoinType( int type )
	{
		m_joinInfo[IYYChannelLogic::KJoinStyle] = type;
	}
	int joinType() const
	{
		return m_joinInfo[IYYChannelLogic::KJoinStyle].toInt();
	}
    bool isJoinTypeValid() const
    {
        return m_joinInfo.contains(IYYChannelLogic::KJoinStyle);
    }
	void setUserData( const QString& userData )
	{
		m_joinInfo[IYYChannelLogic::KUserData] = userData;
	}
	QString userData() const
	{
		return m_joinInfo[IYYChannelLogic::KUserData].toString();
	}
	void setShowTop( bool top )
	{
		m_joinInfo[IYYChannelLogic::KNeedShowTop] = top;
	}
	bool needShowTop() const
	{
		return m_joinInfo[IYYChannelLogic::KNeedShowTop].toBool();
	}
	void setActive( bool active )
	{
		m_joinInfo[IYYChannelLogic::KNeedActive] = active;
	}
	bool needActive() const
	{
		return m_joinInfo[IYYChannelLogic::KNeedActive].toBool();
	}
    void setJoinReportInfo(const QString& info)
    {
        m_joinInfo[IYYChannelLogic::KJoinReportInfo] = info;
    }
    QString joinReportInfo() const
    {
        return m_joinInfo[IYYChannelLogic::KJoinReportInfo].toString();
    }

	void setEntrance(const QString& strEntrance) 
	{
		m_joinInfo[IYYChannelLogic::KEntrance] = strEntrance;
	}
	
	QString getEntrance() const
	{
		return m_joinInfo[IYYChannelLogic::KEntrance].toString();
	}

private:
	JoinInfoType& m_joinInfo;
};