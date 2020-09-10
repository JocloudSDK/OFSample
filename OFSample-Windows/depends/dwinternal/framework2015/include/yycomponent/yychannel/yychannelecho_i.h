#pragma once
#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

/* use for getObject() */
/**
* @brief .Ƶ�����Խӿ�
*/
DWDEFINE_INTERFACE( IYYChannelEcho) : public IUnk
{
/**
* @brief .�źŶ���
	* @param .op�������룬��ϸ��channelEcho_i.h��CHANNEL_OP_ERR_CODE��res�������룬RES_EAUTHΪ���������ϸ��res_code.h
	*/
	typedef _def_boost_signals2_signal_type<void (int op, int res)>::type	_SIG_ECHO_OPERATION_RES;
	
/**
	* @brief .��ȡ�������֪ͨ�ź�
	*/
	virtual _SIG_ECHO_OPERATION_RES* getEchoOperationSignal() = 0;

    /**
* @brief .�źŶ���
	* @param .op�������룬��ϸ��channelEcho_i.h��CHANNEL_OP_ERR_CODE��res�������룬RES_EAUTHΪ���������ϸ��res_code.h
    * @param .spEchoInfo, ������Ϣ, biz_consts.hԤ���塣ĿǰCPROP_ErrorHandled��ʾ��Ӵ����signal���ͻ��˲��ٴ���
	*/
    typedef _def_boost_signals2_signal_type<void (int op, int res, IPropBagWrapperPtr spEchoInfo)>::type _SIG_ECHO_OPERATION_EX_RES;
    virtual _SIG_ECHO_OPERATION_EX_RES* getEchoOperationExSignal() = 0;

	/**
	* @brief .�źŶ���
	* @param .isme true��ʾ���Լ���T/��
	* @param .spEchoInfo, ������Ϣ, biz_consts.hԤ���塣
	*                     CPROP_Name Ƶ������
	*                     GUILDPROP_AdminNick �������ǳ�
	*                     GUILDPROP_AdminUid ������uid
	*                     LPROP_BanReason ����
	*                     UPROP_Nick ��T/����û��ǳ�
	*                     UPROP_Uid ��T/����û�uid
	*                     LPROP_BanTime ʱ�䣬��ʾ��x���Ӳ��ܽ���Ƶ����
	*/
	typedef _def_boost_signals2_signal_type<void (bool isme, IPropBagWrapperPtr spEchoInfo)>::type _SIG_ECHO_KICKOUT;
	virtual _SIG_ECHO_KICKOUT* getEchoKickoutSignal() = 0;

	/**
	* @brief .����/ȡ�������û�������Ƶ��������ʾ
	* @param .subscribe true:���ģ�false:ȡ������
	*/
	virtual void subscribeUserChangedPrompt(UINT32 subSid, bool subscribe) = 0;
};