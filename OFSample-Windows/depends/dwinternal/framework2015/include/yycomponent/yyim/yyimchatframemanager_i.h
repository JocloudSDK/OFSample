#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yyim/yyimchatwidget_i.h"

/*
 * 可通过 getServiceInterface<IYYImChatFrameManager>(KSERVICEImGroup)
*/
DWDEFINE_INTERFACE(IYYImChatFrameManager) : public IDWQComponent
{
/**
   * @brief . 按联系人uid查找与它聊天框的组件接口
   * @param . uid 用户uid
   * @return . IYYImChatWidgetPtr 用户的聊天框的wrap组件接口
   */
	virtual IYYImChatWidgetPtr findImWidget( quint32 uid ) = 0;
/**
   * @brief . 置顶的聊天框
   * @param . 无
   * @return. IYYImChatWidgetPtr 用户的聊天框的wrap组件接口
   */
	virtual IYYImChatWidgetPtr foregroundWnd( void ) = 0;
/**
   * @brief . 是否与这个联系人有打开聊天框
   * @param . uid 用户uid
   * @return. true 表示已经打开聊天框，否则没有打开
   */
	virtual bool isOpenChatWnd( quint32 uid ) =	0;
/**
   * @brief . 共打开了多少个聊天框
   * @param . 无
   * @return. 返回打开聊天框的总数
   */ 
	virtual int  chatWndCount() = 0;

    /*
    * 创建IM窗口，如果已经存在，直接返回
    */
    virtual IYYImChatWidgetPtr createImWidget(quint32 uid) = 0;

	/*
	* 创建IM窗口，如果已经存在，直接返回
	*/
	virtual QList<IYYImChatWidgetPtr> allImChatFrames() = 0;

    virtual QList<IYYImChatWidgetPtr> foregrounds() const = 0;

    /** 
	signals:
		void imWidgetCreated(quint32 user_id);
		void imWidgetDestoryed(quint32 user_id);
    */
};
