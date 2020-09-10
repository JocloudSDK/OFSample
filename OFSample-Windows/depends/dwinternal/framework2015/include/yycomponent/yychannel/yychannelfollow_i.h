#pragma once

#include "dwbase/dwcomex.h"

/**
* @brief 关注接口.
*/
DWDEFINE_INTERFACE(IYYChannelFollow) : public IDWQComponent
{
/**
   * @brief 请求关注数.
   * @param uid，用户uid.
   */
	virtual void requestFollowNumber(quint32 uid) = 0;
/**
   * @brief 请求多个关注数.
   * @param uids，用户uid列表.
   */
	virtual void requestMultiFollowNumber(const QSet<quint32>& uids) = 0;

/**
   * @brief 是否关注这个用户.
   * @param uid，要关注的用户.
   * @param isFollow，true，关注该用户，false，取消关注.
   */
	virtual void requestFollow(quint32 uid, bool isFollow) = 0;

	/**
	* @brief 设置关注状态.
	* @param uid，关注的用户.
	* @param isFollow，true，关注该用户，false，取消关注.
	*/
	virtual void setFollowState(quint32 uid, bool isFollow) = 0;

// signals:
// 	void followInfoInited(quint32 uid, quint32 followCount, bool followedByMe);
// 	void numberUpdated(quint32 uid, quint32 followCount);
//		void unfollowDataArrived(int resCode, quint32 tuid);
//		void followStateChanged(quint32 tuid, bool isFollow);
};