#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"
#include "biz/biz_types.h"
/**
  * @brief 主面板频道最近访问列表接口, 通过Root->getServiceInterface<IYYMainFrameRecentList>(KSERVICEMainframe)获取.
*/
//yycommonconst.h
//enum ChannelJoinStyle
//{
//	JoinNormalStyle = 0,
//	JoinWonderChannelStyle = 1, // 精彩世界频道
//	JoinWonderWordStyle = 2,
//	JoinFunnyRoomStyle = 3, // 微房频道
//	JoinMeStyle = 4, //me频道
//};


DWDEFINE_INTERFACE(IYYMainFrameRecentList) : public IUnk
{
	/**
	* @brief 添加一条最近访问记录项.如果项已经存在会更新信息，并且把item排在最前面.
	* @param 
	*/
	virtual bool addItem(quint32 sid, quint32 uid, const QString& name, quint32 asid, quint32 users, 
		quint32 logoIndex, const QString& logoUrl, bool isLimit, int joinType) = 0;

	/**
	* @brief 添加一条最近访问记录项.如果项已经存在会更新信息，并且把item排在最前面。记录不会存储到本地，需要业务组件自行存储
	* @param guid唯一标识
    * @param name显示的文字
    * @param logoUrl, 头像，http url 或者是 本地路径
    * @param jumpTicket, 双击时跳转的飞机票
    * @param propExt, 额外参数。
    *                 CPROP_OnlineUserCount表示在线人数
    *                 CPROP_Asid表示显示的ID
    *                 GPROP_CreateTime表示记录生成时间，将会按这个排序，0或不传入表示使用当前时间
	*/
	virtual bool addAppItem(const QString& guid, const QString& name, const QString& logoUrl, const QString &jumpTicket, IPropBagWrapperPtr propExt) = 0;

	/*
	signals:
	*/
	typedef _def_boost_signals2_signal_type<void()>::type _SIG_CHANNEL_RECENT_LIST_CLEAR;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual _SIG_CHANNEL_RECENT_LIST_CLEAR *getChannelRecentListClearSignal() = 0;
	
	/*
	* @signals:用户操作-删掉该最近列表项
	* @param:组件自定义的guid唯一值
	*/
	typedef _def_boost_signals2_signal_type<void(const QString&)>::type _SIG_REMOVE_FROM_RECENT_LIST;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual _SIG_REMOVE_FROM_RECENT_LIST *getRemoveFromRecentListSignal() = 0;
	/*
	* @signals:用户操作-触发组件项
	* @param:组件自定义的guid唯一值
	*/
	typedef _def_boost_signals2_signal_type<void(const QString&)>::type _SIG_TRIGGER_ITEM;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual _SIG_TRIGGER_ITEM *getTriggerItemSignal() = 0;
};
