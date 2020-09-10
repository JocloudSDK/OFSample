#pragma once

#include "dwbase/dwcomex.h"

/**
* @brief ��ע�ӿ�.
*/
DWDEFINE_INTERFACE(IYYChannelFollow) : public IDWQComponent
{
/**
   * @brief �����ע��.
   * @param uid���û�uid.
   */
	virtual void requestFollowNumber(quint32 uid) = 0;
/**
   * @brief ��������ע��.
   * @param uids���û�uid�б�.
   */
	virtual void requestMultiFollowNumber(const QSet<quint32>& uids) = 0;

/**
   * @brief �Ƿ��ע����û�.
   * @param uid��Ҫ��ע���û�.
   * @param isFollow��true����ע���û���false��ȡ����ע.
   */
	virtual void requestFollow(quint32 uid, bool isFollow) = 0;

	/**
	* @brief ���ù�ע״̬.
	* @param uid����ע���û�.
	* @param isFollow��true����ע���û���false��ȡ����ע.
	*/
	virtual void setFollowState(quint32 uid, bool isFollow) = 0;

// signals:
// 	void followInfoInited(quint32 uid, quint32 followCount, bool followedByMe);
// 	void numberUpdated(quint32 uid, quint32 followCount);
//		void unfollowDataArrived(int resCode, quint32 tuid);
//		void followStateChanged(quint32 tuid, bool isFollow);
};