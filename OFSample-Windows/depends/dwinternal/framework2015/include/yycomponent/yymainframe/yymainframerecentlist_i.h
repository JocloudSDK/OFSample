#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"
#include "biz/biz_types.h"
/**
  * @brief �����Ƶ����������б�ӿ�, ͨ��Root->getServiceInterface<IYYMainFrameRecentList>(KSERVICEMainframe)��ȡ.
*/
//yycommonconst.h
//enum ChannelJoinStyle
//{
//	JoinNormalStyle = 0,
//	JoinWonderChannelStyle = 1, // ��������Ƶ��
//	JoinWonderWordStyle = 2,
//	JoinFunnyRoomStyle = 3, // ΢��Ƶ��
//	JoinMeStyle = 4, //meƵ��
//};


DWDEFINE_INTERFACE(IYYMainFrameRecentList) : public IUnk
{
	/**
	* @brief ���һ��������ʼ�¼��.������Ѿ����ڻ������Ϣ�����Ұ�item������ǰ��.
	* @param 
	*/
	virtual bool addItem(quint32 sid, quint32 uid, const QString& name, quint32 asid, quint32 users, 
		quint32 logoIndex, const QString& logoUrl, bool isLimit, int joinType) = 0;

	/**
	* @brief ���һ��������ʼ�¼��.������Ѿ����ڻ������Ϣ�����Ұ�item������ǰ�档��¼����洢�����أ���Ҫҵ��������д洢
	* @param guidΨһ��ʶ
    * @param name��ʾ������
    * @param logoUrl, ͷ��http url ������ ����·��
    * @param jumpTicket, ˫��ʱ��ת�ķɻ�Ʊ
    * @param propExt, ���������
    *                 CPROP_OnlineUserCount��ʾ��������
    *                 CPROP_Asid��ʾ��ʾ��ID
    *                 GPROP_CreateTime��ʾ��¼����ʱ�䣬���ᰴ�������0�򲻴����ʾʹ�õ�ǰʱ��
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
	* @signals:�û�����-ɾ��������б���
	* @param:����Զ����guidΨһֵ
	*/
	typedef _def_boost_signals2_signal_type<void(const QString&)>::type _SIG_REMOVE_FROM_RECENT_LIST;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual _SIG_REMOVE_FROM_RECENT_LIST *getRemoveFromRecentListSignal() = 0;
	/*
	* @signals:�û�����-���������
	* @param:����Զ����guidΨһֵ
	*/
	typedef _def_boost_signals2_signal_type<void(const QString&)>::type _SIG_TRIGGER_ITEM;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual _SIG_TRIGGER_ITEM *getTriggerItemSignal() = 0;
};
