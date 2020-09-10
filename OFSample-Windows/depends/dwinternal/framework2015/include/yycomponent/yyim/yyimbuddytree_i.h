#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycommon/propbagwrapper_i.h"
#include "yyimbuddytreeiteminfoareahandler_i.h"

//内置的handler信息
const UINT32 KSignHandlerLevel				= 0xffff;
const UINT32 KAreaSharedHandlerLevel		= 0xfffe;
const UINT32 KPersonalCenterHandlerLevel	= 0xfffd;
const UINT32 KChannelInfoLevel				= 0x01;
const UINT32 KGameInfoLevel					= 0x01;
const UINT32 KComponentInfoLevel = 0x02;

const char KSignHandlerName[]				= "signhandler";
const char KChannelInfoHandlerName[]		= "channelindohandler";
const char KGameInfoHandlerName[]			= "gameinfohandler";
const char KAreaSharedHandlerName[]			= "areasharedhandler"; // 该handler暂未提供
const char KPersonalCenterHandlerName[]		= "personalcenterhandler";
//

/*
IYYImBuddyTreePtr pBuddyTree= root->getServiceInterface<IYYImBuddyTree>(KSERVICEImGroup);

*/

DWDEFINE_INTERFACE(IYYImBuddyTree) : public IDWQComponent
{
	/**
	* @brief . 好友列表是否初始化，存在访问
	* @param . 无
	* @return. 返回true，表示已经存在，否则不存在
	*/
	virtual bool isImBuddyTreeCreated() = 0;

	/**
	* @brief . 已废弃
	* @param .
	* @return .
	*/
	virtual bool registerInfoAreaHandle(IYYImBuddyTreeItemInfoAreaHandler *handler) = 0;
	/**
	* @brief . 已废弃
	* @param .
	* @return.
	*/
	virtual bool unregisterInfoAreaHandle(IYYImBuddyTreeItemInfoAreaHandler *handler) = 0;	// 删除指定uid的handler
	/**
	* @brief .已废弃
	* @param .
	* @return.
	*/
	virtual bool unregisterUserInfoAreaHandler( UINT32 uid ) = 0; 
	/**
	* @brief .已废弃
	* @param .
	* @return.
	*/
	virtual QMap<UINT32, QList<IYYImBuddyTreeItemInfoAreaHandler*>>* userInfoAreaHandlers( UINT32 uid ) = 0;	// 指定uid的所有handler

	/**
	* @brief .已废弃
	* @param .
	* @return.
	*/
	virtual QMap<UINT32,  QMap<UINT32,QList<IYYImBuddyTreeItemInfoAreaHandler*>>>* allUserInfoAreaHandlers() = 0;	// 所有用户的handler

	/**
	* @brief . 设置光标
	* @param . cursor 表示光标的对象，具体查QCursor文档
	* @return. 无
	*/
	virtual void setCursor( QCursor cursor ) = 0; 

	/**
	* @brief . 好友列表修改内容后，需要刷新才能看到最新的修改内容。
	* @param . 无
	* @return. 无
	*/
	virtual void updateTree() = 0; 

	/**
	* @brief . 插入目录
	* @param . folderId 目录Id
	* @param . folderId 目录名字
	* @param . folderPositionIndex 目录位置
	* @return. 无
	*/
	virtual void insertFolder(const UINT32 folderId, const QString title, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . 插入指定目录的联系人
	* @param . folderId 目录Id
	* @param . userId 用户uid
	* @return. 无
	*/
	virtual void appendBuddy(const UINT32 folderId, const UINT32 userId )	=	0;

	/**
	* @brief . 显示指定目录
	* @param . folderId 目录Id
	* @param . folderPositionIndex 目录位置
	* @return. 无
	*/
	virtual void showFolder(const UINT32 folderId, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . 隐藏目录
	* @param . folderId 目录Id
	* @return. 无
	*/
	virtual void hideFolder(const UINT32 folderId)	=	0;

	/**
	* @brief . 注册显示在好友列表的联系人签名处的显示model
	* @param . modelItem model实体
	* @return. 无
	*/
	virtual	void registerBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . 反注册显示在好友列表的联系人签名处的显示model
	* @param . modelItem model实体
	* @return. 无
	*/
	virtual void unregisterBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . 插入指定用户在指定目录的显示model，则信息会在指定的签名处显示出来。
	（具体显示还要看优先级，是否轮转等条件）
	* @param . folderId	目录Id
	* @param . userId 用户uid
	* @param . modelName	model名字
	* @return. 无
	*/
	virtual void appendBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . 删除指定用户在指定目录的显示model。
	* @param . folderId	目录Id
	* @param . userId 用户uid
	* @param . modelName model名字
	* @return. 无
	*/
	virtual void removeBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . 删除好友列表中指定目录的联系人
	* @param . folderId	目录Id
	* @param . userId	用户uid
	* @return. 无
	*/
	virtual void removeBuddy(const UINT32 folderId, const UINT32 userId) = 0;

	/**
	* @brief . 查找指一目录的QStandardItem
	* @param . folderId	目录Id
	* @return. QStandardItem，指定目录的数据信息。
	*/
	virtual QStandardItem* findFolderStandardItem( const UINT32 folderId ) = 0;

	/**
	* @brief . 迷你资料卡处显示的频道游戏信息
	* @param . 如果某项信息为空则填入空的QString，不在频道则channelID的值是0
	* @param . folderId	目录Id
	* @param . userId		用户uid
	* @param . gameName	游戏名字
	* @param . channelName 频道名字
	* @param . activityStr 活动参数，例如activityStr://sid=400000&asid=2080&childsid=456&aid=1234&name=十一国庆七天乐 精彩乐翻天&imageurl=iowqruiowqur&
	* @return. 无
	*/
	virtual void setMinicardAreaShowInfo(const UINT32 userId, const UINT32 folderId, const QString& gameName, const QString& channelName, const UINT32 channelID, const QString& activityStr) = 0;

	/**
	* @brief . 迷你资料卡处去除某个目录的联系人显示频道游戏信息
	* @param . folderId	目录Id
	* @param . userId		用户uid
	* @return. 无
	*/
	virtual void clearMinicardAreaShowInfo(const UINT32 userId, const UINT32 folderId) = 0;

	virtual void visibleUsers(std::vector<unsigned int> &uids) = 0;

	/**
	* @brief . 新消息闪动通知，指定分组folderId且其内的uid闪动
	* @param . folderId，目录Id
	* @param . uid，	用户uid
	* @return. 无
	*/
	virtual void addFlashUid(const UINT32 folderId, const UINT64 uid) = 0;

	/**
	* @brief . 新消息闪动清除，指定分组folderId且其内的uid清除
	* @param . folderId，目录Id
	* @param . uid，	用户uid
	* @return. 无
	*/
	virtual void removeFlashUid(const UINT32 folderId, const UINT64 uid) = 0;

	/*
	* @brief . 更新自己在好友列表及im聊天窗口的分享轮播信息
	* @param . isTurn，是否轮播
	* @param . iconUrl，图标地址
	* @param . name，播放名字
	* @param . jumpTicket，跳转的飞机票
	* @return. 无
	*/
	virtual void updateMyCurrentShareStatus(IPropBagWrapperPtr pbag) = 0;

	/*
	* @brief . 删掉我的所在频道信息字段
	* @param . keys，删掉的propbag字段key集
	* @return. 无
	*/
	virtual void removeMyCurrentShareStatus(std::vector<std::wstring> &keys) = 0;

	virtual IPropBagWrapperPtr getBuddyChannelInfo(UINT32 buddyID) = 0;

	/*
signal:
	void buddiesInfoChanged(const std::vector<UINT32> & uid, UINT32 flags);
	*/
};

DWDEFINE_INTERFACE(IYYImBuddyFolderItemProp) : public IUnk
{
	/**
	* @brief . 指定目录是否有展开
	* @param . folderId	目录Id
	* @return. 是否有展开目录
	*/
	virtual bool isExpanded(const UINT32 folderId) = 0;
};

DWDEFINE_INTERFACE(IYYImIndependentBuddyFolderItemProp) : public IUnk
{
	/**
	* @brief . 指定目录是否有展开(独立目录)
	* @param . folderId	目录Id
	* @return. 是否有展开目录
	*/
	virtual bool isExpanded(const UINT32 folderId) = 0;

	/**
	* @brief . 设置目录名字
	* @param . folderId 目录id
	* @param . folderName 目录名字
	* @return. 无
	*/
	virtual void setFolderName( const UINT32 folderId, const QString folderName )	=	0;
};

DWDEFINE_INTERFACE(IYYImIndepentendBuddyTree) : public IDWQComponent
{
	/**
	* @brief . 好友列表是否初始化，存在访问
	* @param . 无
	* @return. 返回true，表示已经存在，否则不存在
	*/
	virtual bool isImBuddyTreeCreated();

	/**
	* @brief . 设置光标
	* @param . cursor 表示光标的对象，具体查QCursor文档
	* @return. 无
	*/
	virtual void setCursor( QCursor cursor ) = 0; // 设置光标

	/**
	* @brief . 好友列表修改内容后，需要刷新才能看到最新的修改内容。
	* @param . 无
	* @return. 无
	*/
	virtual void updateTree() = 0;

	/**
	* @brief . 插入目录
	* @param . folderId 目录Id
	* @param . folderId 目录名字
	* @param . folderPositionIndex 目录位置
	* @return. 无
	*/
	virtual void insertFolder(const UINT32 folderId, const QString title, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . 显示指定目录
	* @param . folderId 目录Id
	* @param . folderPositionIndex 目录位置
	* @return. 无
	*/
	virtual void showFolder(const UINT32 folderId, const UINT32 folderPositionIndex)	=	0;

	/**
	* @brief . 隐藏目录
	* @param . folderId 目录Id
	* @return. 无
	*/
	virtual void hideFolder(const UINT32 folderId)	=	0;

	/**
	* @brief . 插入指定目录的联系人
	* @param . folderId 目录Id
	* @param . userId 用户uid
	* @return. 无
	*/
	virtual void appendBuddy(const UINT32 folderId, const UINT32 userId )	=	0;

	/**
	* @brief . 删除好友列表中指定目录的联系人
	* @param . folderId	目录Id
	* @param . userId 用户uid
	* @return. 无
	*/
	virtual void removeBuddy(const UINT32 folderId, const UINT32 userId) = 0;

	/**
	* @brief . 查找指一目录的QStandardItem
	* @param . folderId	目录Id
	* @return. QStandardItem，指定目录的数据信息。
	*/
	virtual QStandardItem* findFolderStandardItem( const UINT32 folderId ) = 0;

	/**
	* @brief . 注册显示在好友列表的联系人签名处的显示model
	* @param . modelItem model实体
	* @return. 无
	*/
	virtual	void registerBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . 反注册显示在好友列表的联系人签名处的显示model
	* @param . modelItem model实体
	* @return. 无
	*/
	virtual void unregisterBuddyTreeItemAreaModel(IYYImBuddyTreeItemInfoAreaModelPtr modelItem) = 0;

	/**
	* @brief . 插入指定用户在指定目录的显示model，则信息会在指定的签名处显示出来。
	（具体显示还要看优先级，是否轮转等条件）
	* @param . folderId	目录Id
	* @param . userId 用户uid
	* @param . modelName model名字
	* @return. 无
	*/
	virtual void appendBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . 删除指定用户在指定目录的显示model。
	* @param . folderId	目录Id
	* @param . userId 用户uid
	* @param . modelName model名字
	* @return. 无
	*/
	virtual void removeBuddyTreeItemShowInfo(const UINT32 userId, const UINT32 folderId, const QString modelName) = 0;

	/**
	* @brief . 指定目录按在线状态等信息排序 
	* @param . folderId	目录Id
	* @return. 无
	*/
	virtual void sortTreeItemData( const UINT32 folderid ) = 0;

	/**
	* @brief . 当用户选择只显示在线联系人时，它会根据指定联系人在线状态来决定是否显示它
	* @param . userId	用户uid
	* @return. 无
	*/
	virtual void updateOnlineOnlyStatus(  const UINT32 userId ) = 0;
	virtual void showOnlineMemberOnly() = 0;
};
