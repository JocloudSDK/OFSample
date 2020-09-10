#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yygroup/yygroupchatwidget_i.h"
#include "yycomponent/yygroup/yydiscussionwidget_i.h"

/*
**��ͨ��getServiceInterface<IYYGroupChatFrameManager>(KSERVICEGroup);
*/
DWDEFINE_INTERFACE(IYYGroupChatFrameManager) : public IDWQComponent
{
	/**
	* @brief . ��Ⱥgroup id��folder id������������������ӿ�
	* @param . gid Ⱥid
	* @param . fid ��id
	* @return . IYYGroupChatWidgetPtr Ⱥ��������wrap����ӿ�
	*/
	virtual IYYGroupChatWidgetPtr findGroupChatWidget(quint32 gid, quint32 fid) = 0;

	/**
	* @brief . ��������discussion id������������������ӿ�
	* @param . did ������id
	* @return . IYYDiscussionWidgetPtr �������������wrap����ӿ�
	*/
	virtual IYYDiscussionWidgetPtr findDiscussionWidget(quint32 did) = 0;
    
	/**
	signals:
		void imGroupWidgetCreated(quint32 gid, quint32 folderid);
		void imDiscussionWidgetCreated(quint32 did);
	*/
};
