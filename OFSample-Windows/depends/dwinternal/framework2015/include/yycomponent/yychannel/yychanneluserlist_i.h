#pragma once
#include "dwbase/IData.h"
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponent/propBag_i.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/

enum NOTIFY_TYPE
{
	NOTIFY_DATA_INS = 0,
	NOTIFY_DATA_DEL = 1,
	NOTIFY_DATA_CHANGED = 2,
	NOTIFY_DATA_RESET = 3,
};

DWDEFINE_INTERFACE(IYYChannelUserList) : public IUnk
{
/**
   * @brief .��ȡƵ��������
   * @param .��
   * @return .Ƶ��������
   */
	virtual int userCount() = 0;

/**
   * @brief .�����û�����
   * @param .uid���������û�uid;fromSid,Դ��Ƶ��;toSid,Ŀ����Ƶ��
   * @return .��ʱ����
   */
	virtual int asyncMoveUser(UINT32 uid, UINT32 fromSid, UINT32 toSid ) = 0;

/**
   * @brief .��һ����Ƶ�����˵��ȵ���һ����Ƶ��
   * @param .fromSid,Դ��Ƶ����toSid,Ŀ����Ƶ��
   * @return .��ʱ����
   */
	virtual int asyncMoveUsers(UINT32 fromSid, UINT32 toSid ) = 0;
	
/**
	* @brief .��ȡ�û����ڵ���Ƶ��
	* @param .uid���û�uid
	* @return .�û����ڵ���Ƶ�����������0���û��Ѿ��뿪Ƶ��
	*/
	virtual UINT32 currentChannelId( UINT32 uid ) = 0;

/**
   * @brief .��ȡ�û���Ƶ����ȫ����Ϣ
   * @param .uid,�û�uid
   * @return .ָ���û�ȫ����Ϣ���������ĵ�ָ�루ͨ��sessiontables.h������ֶλ�ȡ��Ҫ����Ϣ��
   */
	virtual Data::IRowPtr getUserInfo( UINT32 uid ) = 0;

	// ��Ƶ���û������ܴ󣬽���ʹ������ź�.
	typedef xsignals::signal<void (Data::IRowArrayPtr)> USERS_SIGNAL;
	virtual USERS_SIGNAL* getUsersAddSignal() = 0;
	virtual USERS_SIGNAL* getUsersRemoveSignal() = 0;
	typedef xsignals::signal<void (Data::IRowArrayPtr, Data::IRowArrayPtr)> USERS_CHANGE_SIGNAL;
	virtual USERS_CHANGE_SIGNAL* getUsersChangeSignal() = 0;

	typedef xsignals::signal<void (UINT32, UINT32, const std::vector<UINT32>& )> SIG_USER_MOVED;
	virtual SIG_USER_MOVED* getUserMovedSignal() = 0;

/**
   * @brief .��ȡĳ����Ƶ��������
   * @param .subChannelId����Ƶ��id
   * @return .��Ƶ���������������һ����Ƶ��������������������Ƶ��������
   */
	virtual int subChannelUserCount(UINT32 subChannelId) = 0;


/**
   * @brief .��ȡ���׽ڵ�
   * @param .xid��Ƶ��id�����û�id��isUser������ǰ���xid�Ƿ����û�id
   * @return .���׽ڵ�sid���������0���Ҵ�������û�id������û��Ѿ��뿪Ƶ�����������������������쳣
   */
	virtual UINT32 getParentSidOf(UINT32 xid, bool isUser) = 0;


	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_ALL_USERS_LOADED;
/**
	* @brief .��ȡ�����û���Ϣ��������ź�
	* @param .��
	* @return .�����û���Ϣ��������ź�,ע������ֻ�ǲ�ѯ�û���Ϣ�����󶼷���ȥ�ˣ����ǲ�һ�����յ������û�����Ϣ
	*/
	virtual _SIG_ALL_USERS_LOADED* getAllUsersLoadedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_SELF_INFO_COMPLETED;
/**
	* @brief .��ȡ�Լ���Ϣ��������ź�
	* @param .��
	* @return .�Լ���Ϣ��������ź�
	*/
	virtual _SIG_SELF_INFO_COMPLETED* getSelfInfoCompletedSignal() = 0;


	typedef _def_boost_signals2_signal_type<void (Data::IRowArrayPtr pRowArray) >::type _SIG_MYINFO_CHANGED;
/**
	* @brief .��ȡ�Լ���Ϣ�ı��ź�
	* @param .��
	* @return .�Լ���Ϣ�ı��ź�
	*/
	virtual _SIG_MYINFO_CHANGED* getMyInfoChanged() = 0;

/**
   * @brief .��λĳ���û���������û���Ƶ�������ڿɼ�
   * @param .uid���û�id
   * @return .��
   */
	virtual void locateUser(quint32 uid) = 0;

/**
   * @brief .����ĳ���û��ڹ������ı���Ϣ�Ƿ����
   * @param .uid���û�id��isIgnore���Ƿ����
   * @return .��
   */
	virtual void setIgnoreText(quint32 uid, bool isIgnore) = 0;

/**
   * @brief .����ĳ���û���˵�������Ƿ����
   * @param .uid���û�id��isIgnore���Ƿ����
   * @return .��
   */
	virtual void setIgnoreVoice(quint32 uid, bool isIgnore) = 0;

/**
   * @brief .����һ���û��������һ���û�����ͬһ����Ƶ�������ߵ����߶�ĳ���û�û�е���Ȩ�ޣ���������ȳɹ�
   * @param .uids���û�id�б�fromSid��Դ��Ƶ����toSid��Ŀ����Ƶ��
   * @return .��
   */	
	virtual void asyncMoveMultiUsers(const QVector<quint32>& uids, UINT32 fromSid,  UINT32 toSid ) = 0;

/**
	* @brief .��ȡĳ����Ƶ���ĳ�Ա�б�
	* @param .sid����Ƶ��id
	* @return .��Ƶ���ĳ�Աuid�б�
	*/	
	virtual QList<quint32> getUserList(quint32 sid) = 0;


	typedef xsignals::signal<void (NOTIFY_TYPE, Data::IRowArrayPtr , Data::IRowArrayPtr)> _SIG_CURRENT_USER_PID_CHANGE;
	virtual _SIG_CURRENT_USER_PID_CHANGE* getCurrentUserPidChange() = 0;

	virtual void getDisplayUnick(UINT32 uid, QString &nick) = 0;

	virtual ROLE_IN_CHANNEL getUserRole(UINT32 uid, UINT32 specSid) = 0;

	typedef _def_boost_signals2_signal_type<void (DATA_NOTIFY_TYPE dataNotifyType, Data::IRowArrayPtr spNewRowArray, Data::IRowArrayPtr spOldRowArray)>::type	_SIG_DATA_NOTIFY;
	virtual _SIG_DATA_NOTIFY* getSigUserTable(const NAME_IDS* fieldFilters) = 0;

	virtual Data::ITablePtr getUserTable() = 0;
	virtual Data::IRowPtr getUserRow(UINT32 uid, USERROW_LOC_TYPE rowLocType) = 0;
	virtual Data::IRowPtr getMyChannelInfo(USERROW_LOC_TYPE rowLocType ) = 0;
	virtual IPropBagPtr getUserBasicInfoPropBag(UINT32 uid) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type	_SIG_REQUEST_LOCATEUSER;
	virtual _SIG_REQUEST_LOCATEUSER* getRequestLocateuserSignal() = 0;

/**
	* @brief .��ȡĳ����Ƶ��������ǰnλ�ĳ�Ա�б�
	* @param .sid����Ƶ��id
	* @param .count����Ҫ��ȡ�ĳ�Ա��
	* @return .��Ƶ��ǰnλ�ĳ�Աuid�б�
	*/
	virtual QList<quint32> getUserTopList(quint32 uid, int count) = 0;

	/**
	* @brief .����+�������û���������
	* @param .count�������û���
	* @param .topSid������Ƶ����
	* @param .fromSid��ԴƵ����
	* @param .toSid��Ŀ��Ƶ����
	* @param .uids���������û�uids(�����)
	*/
	virtual void asyncMoveAnonyUsers(UINT32 nCount, UINT32 topSid, UINT32 fromSid, UINT32 toSid, const std::set<UINT32> &uids)=0;

	/**
   * @brief .����ĳ���û���˵��������С
   * @param .uid���û�id��nVolume��0-200 �û���ʼֵΪ100
   * @return .��
   */
	virtual void setSpeakVolume(quint32 uid, quint32 nVolume) = 0;
};
