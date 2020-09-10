#pragma once

#include "dwbase/dwcomex.h"

//共享模式可以继承IYYImBuddyTreeItemInfoAreaHandler接口实现一个handler
//-作为内置handler然后提供一些接口插入共享handler的接口


DWDEFINE_INTERFACE(IYYImBuddyTreeItemInfoAreaHandler)
{
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual UINT32 level() = 0; //优先级
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool bInTurn() = 0; //该level等级是否轮流显示
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual UINT32 uid() = 0; // 需要显示这个消息的好友uid
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual QString handleName() = 0; //唯一标识这个handle的字符串
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool offlineShow() = 0; //离线或者隐身的时候是否显示

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF infoAreaRect ) = 0;

	/*tips:
	下面的事件范围是整个item，所以handler实现这几个函数的时候需要做判断是否真正点中了属于自己paint自己的区域
	*/
	virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event, QRectF itemRect ) = 0;
};

DWDEFINE_INTERFACE(IYYImBuddyTreeItemInfoAreaModel) : public IUnk
{
	/**
	* @brief . 显示优先级，在同一个用户的签名处，同一高优化级归类一起显示
	* @param . 无
	* @return. 优先级别
	*/
	virtual UINT32	level() = 0; //优先级
	/**
	* @brief . 本显示签名处的model唯一名字
	* @param . 无
	* @return. 返回显示签名处的model名字
	*/
	virtual QString handleName() = 0; 
	/**
	* @brief . 本显示model可以控制在离线或者隐身的时候是否显示
	* @param . 无
	* @return. 返回true为显示内容，false为不显示
	*/
	virtual bool	offlineShow() = 0;
	/**
	* @brief . 该level等级是否轮流显示
	* @param . 无
	* @return. 返回true为轮转，false为不轮转
	*/
	virtual bool	bInTurn() = 0; 

	/**
	* @brief . 画本model的信息
	* @param . painter 画图的handle, QT用它来画图操作
	* @param . option  描述画图的一些参数，请查看Qt说明文档QStyleOptionGraphicsItem
	* @param . widget  画图的窗口类
	* @param . infoAreaRect 画图的区域
	* @return. 无
	*/
	virtual void	paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF infoAreaRect ) = 0;

	/**
	* @brief . 用户鼠标移入区域时响应
	* @param . event hoverenter时的事件，具体请查Qt文档中的QGraphicsSceneHoverEvent
	* @param . itemRect hoverenter时的响应的区域
	* @return. 无
	*/
	virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;

	/**
	* @brief . 用户鼠标在该区域内移动时响应
	* @param . event hovermove时的事件，具体请查Qt文档中的QGraphicsSceneHoverEvent
	* @param . itemRect hovermove时的响应的区域
	* @return. 无
	*/
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event, QRectF itemRect ) = 0;

	/**
	* @brief . 用户鼠标移出区域时响应
	* @param . event hoverleave时的事件，具体请查Qt文档中的QGraphicsSceneHoverEvent
	* @param . itemRect hoverleave时的响应的区域
	* @return. 无
	*/
	virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event, QRectF itemRect ) = 0;

	/**
	* @brief . 用户点击鼠标，压下动作时响应
	* @param .event	 具体请查Qt文档中的QGraphicsSceneHoverEvent
	* @param . itemRect 响应的区域
	* @return. 无
	*/
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;

	/**
	* @brief . 用户点击鼠标，松开动作时响应
	* @param . event	 具体请查Qt文档中的QGraphicsSceneHoverEvent
	* @param . itemRect 响应的区域
	* @return. 无
	*/
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event, QRectF itemRect ) = 0;

	/**
	* @brief . 用户双击鼠标时响应
	* @param . event	 具体请查Qt文档中的QGraphicsSceneHoverEvent
	* @param . itemRect 响应的区域
	* @return. 无
	*/
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event, QRectF itemRect ) = 0;

	/**
	* @brief . 需要显示这个消息的好友uid
	* @param . 无
	* @return. 用户uid
	*/
	virtual UINT32	uid() = 0; 

	/**
	* @brief . 好友列表当前uid所在的分组id
	* @param . 无
	* @return. 目录uid
	*/
	virtual UINT32	folderId() = 0;

	/**
	* @brief . 设置需要显示这个消息的好友uid
	* @param . 用户uid
	* @return. 无
	*/
	virtual void setUid( const UINT32 uUid ) = 0;

	/**
	* @brief . 设置好友列表当前uid所在的分组id
	* @param . 目录uid
	* @return. 无
	*/
	virtual void setFolderId( const UINT32 folderId ) = 0;
};