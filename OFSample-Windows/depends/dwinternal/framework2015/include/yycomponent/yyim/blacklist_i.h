#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

struct IPropBagWrapper;

/*
use getObject function, have to release the smart pointer when component onUnload
IYYComImBlackListPtr black = root->getServiceInterface<IYYComImBlackList>(KSERVICEImGroup)
*/
DWDEFINE_INTERFACE(IYYComImBlackList) : public IUnk
{
	/**
	* @brief . 加入到黑名单里面
	* @param . uid 用户uid
	* @return. 无
	*/
	virtual void asyncAddBlackUser(UINT32 uid) = 0;
	/**
	* @brief . 移出黑名单
	* @param . uid 用户uid
	* @param . fMoveToBuddies true表示用户会申请为好友(为了兼容加好友流程，通常不会使用)，false表示只是移出黑名单
	* @return. 无
	*/
	virtual void asyncRemoveBlackUser(UINT32 uid, bool fMoveToBuddies) = 0;

	/**
	* @brief . 该联系人是否在黑名单中 
	* @param . uid 用户uid
	* @return. 无
	*/
	virtual bool isBlackUser(UINT32 uid) = 0;
	/**
	* @brief . 先产生一个removeall的事件，然后再发addblack的事件，目的是通知界面刷新。
	* @param . 无
	* @return. 无
	*/
	virtual void refresh(void) = 0;

	/**
	* @brief . 得到所有的黑名单联系人账号列表
	* @param . uids 用户列表
	* @return. 无
	*/
	virtual void getAllBlackUsers(std::list<UINT32>& uids) = 0;

	/**
	* @brief . 加入了黑名单成功通知事件
	* @param . uids 用户列表
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_ADD_BLACK_USERS;
	virtual _SIG_ADD_BLACK_USERS* getAddBlackUsersSig() = 0;

	/**
	* @brief . 删除了黑名单成功通知事件
	* @param . uids 用户列表
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_REMOVE_BLACK_USERS;
	virtual _SIG_REMOVE_BLACK_USERS* getRemoveBlackUsersSig() = 0;

	/**
	* @brief . 删除了所有黑名单成功通知事件
	* @param . 无
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_REMOVE_ALL;
	virtual _SIG_REMOVE_ALL* getRemoveAllSig() = 0;

	/**
	* @brief . 获取黑名单内容通知事件
	* @param . buddyID 用户uid
	* @param . objUInfo 用户信息
	* @return. 事件
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 buddyID, comptr<IPropBagWrapper> objUInfo)>::type _SIG_USERINFO;
	virtual _SIG_USERINFO* getUserInfoSig() = 0;
};