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
	* @brief . Ĭ�Ϻ���Ŀ¼�����ǽ����ϡ��ҵĺ��ѡ�Ŀ¼
	* @param . ��
	* @return. ����Ŀ¼uid
	*/
	virtual unsigned int getDefaultFid() = 0;

	/**
	* @brief . ˢ�º����б�
	* @param . flag �����õ�
	* @return. ��
	*/
	virtual void refresh(unsigned int flag) = 0;

	//add/remove/move folder/buddy
	/**
	* @brief . ��������Ŀ¼
	* @param . folderName Ŀ¼����
	* @return. ��
	*/
	virtual void asyncCreateFolder(LPCWSTR folderName) = 0;
	/**
	* @brief . ������Ŀ¼�޸�����
	* @param . fid Ŀ¼id
	* @param . folderName ��Ŀ¼������
	* @return. ��
	*/
	virtual void asyncRenameFolder(unsigned int fid, LPCWSTR folderName) = 0;
	/**
	* @brief . ɾ��ָ���ĺ���Ŀ¼
	* @param . fid Ŀ¼id
	* @return. ��
	*/
	virtual void asyncDestroyFolder(unsigned int fid) = 0;

	/**
	* @brief . ��ָ������Ŀ¼�����Ӻ���
	* @param . fid Ŀ¼id
	* @param . uid �û�uid
	* @param . pNickName �û��ǳ�
	* @return .
	*/
	virtual void asyncAddBuddy(unsigned int fid, unsigned int uid, LPCWSTR pNickName) = 0;
	/**
	* @brief . ��ָ������Ŀ¼��ɾ������
	* @param . fid Ŀ¼id
	* @param . uid �û�uid
	* @param . bDelMe �Ƿ�ɾ���Լ�
	* @return. ��
	*/
	virtual void asyncRemoveBuddy(unsigned int fid, unsigned int uid,bool bDelMe ) = 0;
	/**
	* @brief . �ƶ����ѵ���һ������Ŀ¼
	* @param . fid Ŀ¼id
	* @param . uid �û�uid
	* @param . new_fid �µĺ���Ŀ¼id
	* @return. ��
	*/
	virtual void asyncMoveBuddy(unsigned int fid, unsigned int uid, unsigned int new_fid) = 0;

	/**
	* @brief . �޸ĺ��ѵı�ע
	* @param . id �û�uid
	* @param . remark ��ע
	* @return. ��
	*/
	virtual void asyncChangeRemark(unsigned int id, const std::wstring& remark) = 0;

	/**
	* @brief . �õ��û���Ϣ
	* @param . wID �û�uid
	* @param . bIsUpdate true��ʾǿ�ƴӷ���������ȡ,false�����ܴӻ���ȡ��
	* @return. ��
	*/
	virtual void GetUserInfo(unsigned int wID, bool bIsUpdate) = 0;
	/**
	* @brief . ����
	* @param . 
	* @return.
	*/
	virtual void GetUserStatus(std::vector<unsigned int> & uIDs) = 0;
	/**
	* @brief . �Ƿ��Ǻ���
	* @param . id �û�uid
	* @return. true ��ʾ����ϵ���Ǻ��ѹ�ϵ�������Ǻ��ѹ�ϵ
	*/
	virtual bool isBuddy(unsigned int id) = 0;
	/**
	* @brief . �õ�ָ����ϵ�˵ķ�������ͷ���ļ���  
	* @param . wID �û�uid
	* @param . url ͷ��url 
	* @return. ��
	*/
	virtual bool GetUserLogo(unsigned int wID, std::wstring & url) = 0;
	/**
	* @brief . �õ�ָ����ϵ�˵�����״̬
	* @param . id �û�uid
	* @return. ����״ֵ̬��IM_STATUS��
	*/
	virtual	unsigned int GetImstatus(unsigned int id) = 0;

	/**
	* @brief . ���Ӻ���Ŀ¼�ɹ����¼� 
	* @param . fids Ŀ¼id
	* @return. _SIG_ADD_FOLDERS�¼�
	*/
	typedef _def_boost_signals2_signal_type<void (const FIDS& fids)>::type	_SIG_ADD_FOLDERS;
	virtual _SIG_ADD_FOLDERS *getAddFoldersSignal() = 0;

	/**
	* @brief . ɾ������Ŀ¼�ɹ��¼�
	* @param . ��
	* @return. _SIG_REMOVE_FOLDERS
	*/
	typedef _def_boost_signals2_signal_type<void (const FIDS& fids)>::type	_SIG_REMOVE_FOLDERS;
	virtual _SIG_REMOVE_FOLDERS *getRemoveFoldersSignal() = 0;


	/**
	* @brief . ����һ�����ѳɹ����¼�
	* @param . fid Ŀ¼id
	* @param . uids һ���û�uid
	* @return. _SIG_ADD_BUDDIES
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, const UIDS& uids)>::type	_SIG_ADD_BUDDIES;
	virtual _SIG_ADD_BUDDIES *getAddBuddiesSignal() = 0;

	/**
	* @brief . ����һ�����ѳɹ����¼�
	* @param . fid Ŀ¼id
	* @param . uid �û�uid
	* @return. _SIG_ADD_BUDDY
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, unsigned int uid)>::type	_SIG_ADD_BUDDY;
	virtual _SIG_ADD_BUDDY *getAddBuddySignal() = 0;

	/**
	* @brief . ɾ��һ�����ѳɹ����¼�
	* @param . fid Ŀ¼uid
	* @param . uids һ���û���uid
	* @return. _SIG_REMOVE_BUDDIES
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, const UIDS& uids)>::type	_SIG_REMOVE_BUDDIES;
	virtual _SIG_REMOVE_BUDDIES *getRemoveBuddiesSignal() = 0;

	/**
	* @brief . �ƶ�һ�����ѵ���һĿ¼�ɹ����¼�
	* @param . fid Ŀ¼id
	* @param . uids һ�����ѵ�uid
	* @param . new_fid �ƶ����µ�Ŀ¼id
	* @return. _SIG_MOVE_BUDDIES
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, const UIDS& uids, unsigned int new_fid)>::type	_SIG_MOVE_BUDDIES;
	virtual _SIG_MOVE_BUDDIES *getMoveBuddiesSignal() = 0;

	/**
	* @brief . ����ͷ��仯���¼���
	* @param . uid ���ѵ�uid
	* @param . nPhotoIndex ͷ��index��Ϊ0��ʾ��ϵͳͷ�� photoPathֵ��Ч����ʾ���Զ���ͷ��url�����أ�����������ʾϵͳͷ��index 
	* @param . photoPath ϵͳͷ���url�� 
	* @return. _SIG_BUDDY_PHOTOPATH
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int uid, unsigned int nPhotoIndex, LPCWSTR photoPath)>::type	_SIG_BUDDY_PHOTOPATH;
	virtual _SIG_BUDDY_PHOTOPATH *getHeadPhotoChangeSignal() = 0;

	/**
	* @brief . ������Ϣ�仯֪ͨ�¼�
	* @param .
	uid �û�uid�б�
	flags ����״̬�ı䣬��const protocol::im::E_UI_LIST_CHANGED_OP
	* @return. _SIG_BUDDIES_INFO_CHANGED
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<unsigned int> & uid, unsigned int flags)>::type	_SIG_BUDDIES_INFO_CHANGED;
	virtual _SIG_BUDDIES_INFO_CHANGED *getBuddiesInfoChangedSignal() = 0;

	//sig: folder/buddy's info
	/**
	* @brief . ����Ŀ¼��Ϣ�仯֪ͨ�¼�
	* @param . 
	fid Ŀ¼id
	flags ��Ӧ protocol::im::E_UI_LIST_CHANGED_OP
	* @return. _SIG_FOLDER_INFO_CHANGED
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int fid, unsigned int flags)>::type	_SIG_FOLDER_INFO_CHANGED;
	virtual _SIG_FOLDER_INFO_CHANGED *getFolderInfoChangedSignal() = 0;

	// 
	/**
	* @brief . ����
	* @param .
	* @return .
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int flags)>::type	_SIG_ALL_CHANGED;
	virtual _SIG_ALL_CHANGED *getAllChangedSignal() = 0;

	/**
	* @brief . �û������б�ȫ��ɾ���¼�
	* @param . ��
	* @return. �� 
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_REMOVE_ALL;
	virtual _SIG_REMOVE_ALL *getRemoveAllSignal() = 0;
	// 
	//typedef _def_boost_signals2_signal_type<void (unsigned int buddyID, IPropBagPtr objUInfo)>::type	_SIG_USERINFO;
	
	/**
	* @brief . �����б���û���Ϣ�¼�
	* @param . buddyID �û���uid
	* @param . objUInfo  �û���Ϣproperty
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int buddyID, comptr<IPropBagWrapper> objUInfo)>::type	_SIG_USERINFO;
	virtual _SIG_USERINFO *getUserInfoSignal() = 0;

	
	/**
	* @brief . �����б���û�����״̬
	* @param . uids �û�uid�б� status ��Ӧ������״̬
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void (const std::vector<unsigned int> & uids, const std::vector<unsigned int> & status)>::type		_SIG_USERS_STATUS;
	virtual _SIG_USERS_STATUS *getUserStatusSignal() = 0;
	
	/**
	* @brief . ����
	* @param . uids uid�б�
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
	* @brief . vip activitiy�����ݷ����¼�
	* @param . 
	* @return.
	*/
	typedef _def_boost_signals2_signal_type<void ( const std::vector<unsigned int> & vec_activity)>::type _SIG_GETVIPACITIVITY_RES;
	virtual _SIG_GETVIPACITIVITY_RES *getVipActivityResSignal() = 0;

	/**
	* @brief . ��ȡvip activity
	* @param . uid �û�uid
	* @return. ��
	*/
	virtual void getVipActivity(unsigned int uid) = 0;
	
	/**
	* @brief . ȡ��vip activity �¼�����
	* @param . uid �û�uid
	* @param . ret activity����
	* @return.
	*/
	typedef _def_boost_signals2_signal_type<void (const UINT32 uid,const UINT32 ret)>::type _SIG_CANCELVIPACTIVITY_RES;
	virtual _SIG_CANCELVIPACTIVITY_RES *cancelVipActivityResSignal() = 0;
	/**
	* @brief . ȡ��vip activity
	* @param . uid �û�uid 
	* @param . m_vec_activity activity����
	* @return.
	*/
	virtual void cancleActivity(unsigned int uid,std::vector<unsigned int> m_vec_activity) = 0;

   
    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 vipGrade, UINT32 vipType)>::type	_SIG_BECOME_VIP_NOTIFY;
    virtual _SIG_BECOME_VIP_NOTIFY *getBecomeVipNotifySignal() = 0;

    typedef _def_boost_signals2_signal_type<void (UINT32 uid, UINT32 vipGrade, UINT32 vipType)>::type	_SIG_VIP_SINGLE_USER_INFO;
    virtual _SIG_VIP_SINGLE_USER_INFO *getVipSingleUserInfoSignal() = 0;

    /*
    *ȡuid�Ļ�Ա��Ϣ���Ǻ���
    */
    virtual void asyncGetNoBuddyVipInfo(UINT32 uid) = 0;
};

DWDEFINE_INTERFACE(IComBuddyList): public IUnk
{
    /**
	* @brief . �õ��û���Ϣ
	* @param . wID �û�uid
	* @param . bIsUpdate true��ʾǿ�ƴӷ���������ȡ,false�����ܴӻ���ȡ��
	* @return. ��
	*/
    virtual void ComGetUserInfo(unsigned int wID, bool bIsUpdate) = 0;

    /**
	* @brief . �����б���û���Ϣ�¼�
	* @param . buddyID �û���uid
	* @param . objUInfo  �û���Ϣproperty
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void (unsigned int buddyID, comptr<IPropBagWrapper> objUInfo)>::type	_SIG_COMUSERINFO;
	virtual _SIG_COMUSERINFO *ComGetUserInfoSignal() = 0;
};
