#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yygroup/yygroupchatwidget_i.h"
#include "yycomponent/yygroup/yydiscussionwidget_i.h"

/*
**可通过getServiceInterface<IYYGroupChatFrameManager>(KSERVICEGroup);
*/
DWDEFINE_INTERFACE(IYYGroupChatFrameManager) : public IDWQComponent
{
	/**
	* @brief . 按群group id和folder id查找与它聊天框的组件接口
	* @param . gid 群id
	* @param . fid 组id
	* @return . IYYGroupChatWidgetPtr 群的聊天框的wrap组件接口
	*/
	virtual IYYGroupChatWidgetPtr findGroupChatWidget(quint32 gid, quint32 fid) = 0;

	/**
	* @brief . 按讨论组discussion id查找与它聊天框的组件接口
	* @param . did 讨论组id
	* @return . IYYDiscussionWidgetPtr 讨论组的聊天框的wrap组件接口
	*/
	virtual IYYDiscussionWidgetPtr findDiscussionWidget(quint32 did) = 0;
    
	/**
	signals:
		void imGroupWidgetCreated(quint32 gid, quint32 folderid);
		void imDiscussionWidgetCreated(quint32 did);
	*/
};
