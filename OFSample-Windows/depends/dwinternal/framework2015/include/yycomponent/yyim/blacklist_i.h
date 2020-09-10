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
	* @brief . ���뵽����������
	* @param . uid �û�uid
	* @return. ��
	*/
	virtual void asyncAddBlackUser(UINT32 uid) = 0;
	/**
	* @brief . �Ƴ�������
	* @param . uid �û�uid
	* @param . fMoveToBuddies true��ʾ�û�������Ϊ����(Ϊ�˼��ݼӺ������̣�ͨ������ʹ��)��false��ʾֻ���Ƴ�������
	* @return. ��
	*/
	virtual void asyncRemoveBlackUser(UINT32 uid, bool fMoveToBuddies) = 0;

	/**
	* @brief . ����ϵ���Ƿ��ں������� 
	* @param . uid �û�uid
	* @return. ��
	*/
	virtual bool isBlackUser(UINT32 uid) = 0;
	/**
	* @brief . �Ȳ���һ��removeall���¼���Ȼ���ٷ�addblack���¼���Ŀ����֪ͨ����ˢ�¡�
	* @param . ��
	* @return. ��
	*/
	virtual void refresh(void) = 0;

	/**
	* @brief . �õ����еĺ�������ϵ���˺��б�
	* @param . uids �û��б�
	* @return. ��
	*/
	virtual void getAllBlackUsers(std::list<UINT32>& uids) = 0;

	/**
	* @brief . �����˺������ɹ�֪ͨ�¼�
	* @param . uids �û��б�
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_ADD_BLACK_USERS;
	virtual _SIG_ADD_BLACK_USERS* getAddBlackUsersSig() = 0;

	/**
	* @brief . ɾ���˺������ɹ�֪ͨ�¼�
	* @param . uids �û��б�
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_REMOVE_BLACK_USERS;
	virtual _SIG_REMOVE_BLACK_USERS* getRemoveBlackUsersSig() = 0;

	/**
	* @brief . ɾ�������к������ɹ�֪ͨ�¼�
	* @param . ��
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_REMOVE_ALL;
	virtual _SIG_REMOVE_ALL* getRemoveAllSig() = 0;

	/**
	* @brief . ��ȡ����������֪ͨ�¼�
	* @param . buddyID �û�uid
	* @param . objUInfo �û���Ϣ
	* @return. �¼�
	*/
	typedef _def_boost_signals2_signal_type<void (UINT32 buddyID, comptr<IPropBagWrapper> objUInfo)>::type _SIG_USERINFO;
	virtual _SIG_USERINFO* getUserInfoSig() = 0;
};