#pragma once

#include "dwbase/dwcomex.h"
#include "qvector.h"
#include "duifw/duiwidget.h"

/**
* @brief Mini card infomation interface.
*/
struct IMiniCardInfo
{
	/**
	* @brief 用户uid.
	* @param .
	* @return.
	*/
	virtual quint32 uid() const = 0;

	/**
	* @brief 用户所在组的id，我的好友分组id=1，最近联系人分组-2.
	* @param .
	* @return .
	*/
	virtual quint32 foldId() const = 0;

	/**
	* @brief 用户等级.
	* @param .
	* @return .
	*/
	virtual int userLevel() const = 0;

	/**
	* @brief 用户昵称.
	* @param .
	* @return.
	*/
	virtual QString userNick() const = 0;

	/**
	* @brief 频道id, 返回0表示没在任何频道.
	* @param .
	* @return.
	*/
	virtual quint32 channelId() const = 0;

	/**
	* @brief 正在玩的游戏 空表示没有玩.
	* @return .
	*/
	virtual QString playGame() const = 0;
	/**
	* @brief 是否手机用户.
	* @return.
	*/
	virtual bool bMobile() const = 0; 

    /**
    * @brief 是否是vip用户.
    * @return .
    */
    virtual bool bVipMember() const = 0;  

    /**
    * @brief vip等级.
    * @return .
    */
    virtual quint32 vipGrade() const = 0;

    /**
    * @brief vip充值类型.
    * @return .
    */
    virtual quint32 vipType() const = 0;

	/**
    * @brief 性别，分别为male,female.
    * @return 可能为空串.
    */
    virtual QString gender() const = 0;

    /**
    * @brief 来源.
    * @return 见yyminicard_i.h中minicard::From.
    */
    virtual int from() const = 0;
};

DWDEFINE_INTERFACE(IYYMiniCardIndicatorHandle)
{
	virtual ~IYYMiniCardIndicatorHandle(){}

	/**
	* @brief handle名字（每个handle有一个名字，在接收click事件的时候用于区别）.
	* @return.
	*/
	virtual QString handleName() = 0;

	/**
	* @brief 是否要显示icon图标.
	* @param . 
	* @return true表示显示，否则不显示.
	*/
	virtual bool	isShowIcon(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief 灰色显示的图片，迷你资料卡上的业务图标有两种，一种是点亮的，一种是没点亮的，也就是灰色的，mini资料卡根据业务返回的是否展示这个人的业务图标来判断取点亮图标还是不点亮图标.
	* @param userInfo 迷你卡信息，具体请看IMiniCardInfo.
	* @return 图片的路径.
	*/
	virtual QString hideStatusPic(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief 亮色显示的图片.
	* @param userInfo 迷你卡信息，具体请看IMiniCardInfo.
	* @return 图片的路径.
	*/
	virtual QString showStatusPic(IMiniCardInfo *userInfo) = 0;	

	/**
	* @brief tooltip的显示内容.
	* @param .
	* @return. 
	*/
	virtual QString toolTipInfo(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief 返回自定义tooltip样式的widget.
	* @param userInfo 迷你卡信息，具体请看IMiniCardInfo.
	* @return .
	*/
	virtual DuiWidget *tooltipWidget(IMiniCardInfo *userInfo) = 0;

	/**
	* @brief 显示自定义的tooltip样式.
	* @param parentWidget 触发迷你卡的父Qwidget.
	* @param itemInfo 迷你卡信息，具体请看IMiniCardInfo. 
	* @param offset tooltip显示相对位置.
	* @return.
	*/
	virtual void showCustomTooltip(QWidget* parentWidget, IMiniCardInfo* itemInfo, const QPointF& offset) = 0;

	/**
	* @brief 预先拉取数据，适用于数据拉去较慢的应用.
	* @param . 
	* @return .
	*/
	virtual void preFetchData(QVector<quint32> userId) = 0;

	/**
	* @brief 获取排位，如果没显示则按注册顺序往后放置 后来加的接口放最后.
	* @param uid 用户uid.
	* @return 返回排行位置.
	*/
	virtual int   getPreferredPosition(quint32 uid) = 0;

	virtual QString showStatusText(IMiniCardInfo *userInfo) = 0;

	virtual QString expandData(IMiniCardInfo *userInfo) = 0;

};
