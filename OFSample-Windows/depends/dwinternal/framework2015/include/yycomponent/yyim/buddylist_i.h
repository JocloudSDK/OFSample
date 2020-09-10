#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

struct IPropBagWrapper;

/*
IYYBuddyListPtr prop = root->getServiceInterface<IYYBuddyList>(KSERVICEImGroup);
*/
DWDEFINE_INTERFACE(IYYBuddyList):public IUnk
{
	/**
	* @brief . 默认好友目录，就是界面上“我的好友”目录
	* @param . 无
	* @return. 返回目录uid
	*/
	virtual unsigned int getDefaultFid() = 0;

	/**
	* @brief . 刷新好友列表
	* @param . flag 保留用的
	* @return. 无
	*/
	virtual void refresh(unsigned int flag) = 0;

	//add/remove/move folder/buddy
	/**
	* @brief . 创建好友目录
	* @param . folderName 目录名字
	* @return. 无
	*/
	virtual void asyncCreateFolder(LPCWSTR folderName) = 0;
	/**
	* @brief . 给好友目录修改名字
	* @param . fid 目录id
	* @param . folderName 该目录新名字
	* @return. 无
	*/
	virtual void asyncRenameFolder(unsigned int fid, LPCWSTR folderName) = 0;
	/**
	* @brief . 删除指定的好友目录
	* @param . fid 目录id
	* @return. 无
	*/
	virtual void asyncDestroyFolder(unsigned int fid) = 0;

	/**
	* @brief . 给指定好友目录上增加好友
	* @param . fid 目录id
	* @param . uid 用户uid
	* @param . pNickName 用户昵称
	* @return .
	*/
	virtual void asyncAddBuddy(unsigned int fid, unsigned int uid, LPCWSTR pNickName) = 0;
	/**
	* @brief . 从指定好友目录中删除好友
	* @param . fid 目录id
	* @param . uid 用户uid
	* @param . bDelMe 是否删除自己
	* @return. 无
	*/
	virtual void asyncRemoveBuddy(unsigned int fid, unsigned int uid,bool bDelMe ) = 0;
	/**
	* @brief . 移动好友到另一个好友目录
	* @param . fid 目录id
	* @param . uid 用户uid
	* @param . new_fid 新的好友目录id
	* @return. 无
	*/
	virtual void asyncMoveBuddy(unsigned int fid, unsigned int uid, unsigned int new_fid) = 0;

	/**
	* @brief . 修改好友的备注
	* @param . id 用户uid
	* @param . remark 备注
	* @return. 无
	*/
	virtual void asyncChangeRemark(unsigned int id, const std::wstring& remark) = 0;

	/**
	* @brief . 得到用户信息
	* @param . wID 用户uid
	* @param . bIsUpdate true表示强制从服务器上拉取,false则智能从缓存取。
	* @return. 无
	*/
	virtual void GetUserInfo(unsigned int wID, bool bIsUpdate) = 0;
	/**
	* @brief . 保留
	* @param . 
	* @return.
	*/
	virtual void GetUserStatus(std::vector<unsigned int> & uIDs) = 0;
	/**
	* @brief . 是否是好友
	* @param . id 用户uid
	* @return. true 表示与联系人是好友关系，否则不是好友关系
	*/
	virtual bool isBuddy(unsigned int id) = 0;
	/**
	* @brief . 得到指定联系人的服务器上头像文件。  
	* @param . wID 用户uid
	* @param . url 头像url 
	* @return. 无
	*/
	virtual bool GetUserLogo(unsigned int wID, std::wstring & url) = 0;
	/**
	* @brief . 得到指定联系人的在线状态
	* @param . id 用户uid
	* @return. 在线状态值（IM_STATUS）
	*/
	virtual	unsigned int GetImstatus(unsigned int id) = 0;

	/**
	* @brief . 增加好友目录成功的事件 
	* @param . fids 目录id
	* @return. _SIG_ADD_FOLDERS事件
	*/
	typedef _def_boost_signals2_signal_type<void (const FIDS& fids)>::type	_SIG_ADD_FOLDERS;
	virtual _SIG_ADD_FOLDERS *getAddFoldersSignal() = 0;

	/**
	* @brief . 删除好友目录成功事件
	* @param . 无
	* @return. _SIG_REMOVE_FOLDERS
	*/
	typedef _def_boost_signals2_signal_type<void (const FIDS& fids)>::type	_SIG_REMOVE_FOLDERS;
	virtual _SIG_REMOVE_FOLDERS *getRemoveFoldersSignal() = 0;


	/**
	* @brief . 增加一批好友成功的事件
	* @param . fid 目录id
	* @param . uids 一批用户uid
	* @return. _SIG_ADD_BUDDIES
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, const UIDS& uids)>::type	_SIG_ADD_BUDDIES;
	virtual _SIG_ADD_BUDDIES *getAddBuddiesSignal() = 0;

	/**
	* @brief . 增加一个好友成功的事件
	* @param . fid 目录id
	* @param . uid 用户uid
	* @return. _SIG_ADD_BUDDY
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, unsigned int uid)>::type	_SIG_ADD_BUDDY;
	virtual _SIG_ADD_BUDDY *getAddBuddySignal() = 0;

	/**
	* @brief . 删除一批好友成功的事件
	* @param . fid 目录uid
	* @param . uids 一批用户的uid
	* @return. _SIG_REMOVE_BUDDIES
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, const UIDS& uids)>::type	_SIG_REMOVE_BUDDIES;
	virtual _SIG_REMOVE_BUDDIES *getRemoveBuddiesSignal() = 0;

	/**
	* @brief . 移动一批好友到另一目录成功的事件
	* @param . fid 目录id
	* @param . uids 一批好友的uid
	* @param . new_fid 移动到新的目录id
	* @return. _SIG_MOVE_BUDDIES
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, const UIDS& uids, unsigned int new_fid)>::type	_SIG_MOVE_BUDDIES;
	virtual _SIG_MOVE_BUDDIES *getMoveBuddiesSignal() = 0;

	/**
	* @brief . 好友头像变化的事件。
	* @param . uid 好友的uid
	* @param . nPhotoIndex 头像index，为0表示非系统头像， photoPath值有效，表示有自定义头像url可下载，其它正数表示系统头像index 
	* @param . photoPath 系统头像的url。 
	* @return. _SIG_BUDDY_PHOTOPATH
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int uid, unsigned int nPhotoIndex, LPCWSTR photoPath)>::type	_SIG_BUDDY_PHOTOPATH;
	virtual _SIG_BUDDY_PHOTOPATH *getHeadPhotoChangeSignal() = 0;

	/**
	* @brief . 好友信息变化通知事件
	* @param .
	uid 用户uid列表
	flags 好友状态改变，是const protocol::im::E_UI_LIST_CHANGED_OP
	* @return. _SIG_BUDDIES_INFO_CHANGED
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<unsigned int> & uid, unsigned int flags)>::type	_SIG_BUDDIES_INFO_CHANGED;
	virtual _SIG_BUDDIES_INFO_CHANGED *getBuddiesInfoChangedSignal() = 0;

	//sig: folder/buddy's info
	/**
	* @brief . 好友目录信息变化通知事件
	* @param . 
	fid 目录id
	flags 对应 protocol::im::E_UI_LIST_CHANGED_OP
	* @return. _SIG_FOLDER_INFO_CHANGED
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, unsigned int flags)>::type	_SIG_FOLDER_INFO_CHANGED;
	virtual _SIG_FOLDER_INFO_CHANGED *getFolderInfoChangedSignal() = 0;

	// 
	/**
	* @brief . 保留
	* @param .
	* @return .
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int flags)>::type	_SIG_ALL_CHANGED;
	virtual _SIG_ALL_CHANGED *getAllChangedSignal() = 0;

	/**
	* @brief . 用户好友列表全部删除事件
	* @param . 无
	* @return. 无 
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_REMOVE_ALL;
	virtual _SIG_REMOVE_ALL *getRemoveAllSignal() = 0;
	// 
	//typedef _def_boost_signals2_signal_type<void (unsigned int buddyID, IPropBagPtr objUInfo)>::type	_SIG_USERINFO;
	
	/**
	* @brief . 好友列表的用户信息事件
	* @param . buddyID 用户的uid
	* @param . objUInfo  用户信息property
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int buddyID, comptr<IPropBagWrapper> objUInfo)>::type	_SIG_USERINFO;
	virtual _SIG_USERINFO *getUserInfoSignal() = 0;

	
	/**
	* @brief . 好友列表的用户在线状态
	* @param . uids 用户uid列表 status 对应的在线状态
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<unsigned int> & uids, const std::vector<unsigned int> & status)>::type		_SIG_USERS_STATUS;
	virtual _SIG_USERS_STATUS *getUserStatusSignal() = 0;
	
	/**
	* @brief . 保留
	* @param . uids uid列表
	* @return.
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type	_SIG_BECOME_BUDDIES_NOTIFY;
	virtual _SIG_BECOME_BUDDIES_NOTIFY *getBecomeBuddiesNotifySignal() = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type	_SIG_WANNA_BECOME_BUDDIES_NOTIFY;
	virtual _SIG_WANNA_BECOME_BUDDIES_NOTIFY *getWannaBecomeBuddiesNotifySignal() = 0;
	
	/**
	* @brief . vip activitiy的内容返回事件
	* @param . 
	* @return.
	*/
	typedef _def_boost_signals2_signal_type<void ( const std::vector<unsigned int> & vec_activity)>::type _SIG_GETVIPACITIVITY_RES;
	virtual _SIG_GETVIPACITIVITY_RES *getVipActivityResSignal() = 0;

	/**
	* @brief . 获取vip activity
	* @param . uid 用户uid
	* @return. 无
	*/
	virtual void getVipActivity(unsigned int uid) = 0;
	
	/**
	* @brief . 取消vip activity 事件返回
	* @param . uid 用户uid
	* @param . ret activity内容
	* @return.
	*/
	typedef _def_boost_signals2_signal_type<void (const UINT32 uid,const UINT32 ret)>::type _SIG_CANCELVIPACTIVITY_RES;
	virtual _SIG_CANCELVIPACTIVITY_RES *cancelVipActivityResSignal() = 0;
	/**
	* @brief . 取消vip activity
	* @param . uid 用户uid 
	* @param . m_vec_activity activity内容
	* @return.
	*/
	virtual void cancleActivity(unsigned int uid,std::vector<unsigned int> m_vec_activity) = 0;

   
    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 vipGrade, UINT32 vipType)>::type	_SIG_BECOME_VIP_NOTIFY;
    virtual _SIG_BECOME_VIP_NOTIFY *getBecomeVipNotifySignal() = 0;

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 vipGrade, UINT32 vipType)>::type	_SIG_VIP_SINGLE_USER_INFO;
    virtual _SIG_VIP_SINGLE_USER_INFO *getVipSingleUserInfoSignal() = 0;

    /*
    *取uid的会员信息，非好友
    */
    virtual void asyncGetNoBuddyVipInfo(UINT32 uid) = 0;
};

DWDEFINE_INTERFACE(IComBuddyList): public IUnk
{
    /**
	* @brief . 得到用户信息
	* @param . wID 用户uid
	* @param . bIsUpdate true表示强制从服务器上拉取,false则智能从缓存取。
	* @return. 无
	*/
    virtual void ComGetUserInfo(unsigned int wID, bool bIsUpdate) = 0;

    /**
	* @brief . 好友列表的用户信息事件
	* @param . buddyID 用户的uid
	* @param . objUInfo  用户信息property
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int buddyID, comptr<IPropBagWrapper> objUInfo)>::type	_SIG_COMUSERINFO;
	virtual _SIG_COMUSERINFO *ComGetUserInfoSignal() = 0;
};
