#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
// Ƶ����ֹ����.

/**
* @brief .�û��Ƿ��ܹ��������֣�˵���ӿ�
*/
DWDEFINE_INTERFACE(IChannelDisableSpeak) : public IDWQComponent
{
/**
   * @brief �ж�ĳ���û��Ƿ���Է����ļ�.
   * @param Uid���û�id.
   * @return ���Է��ͣ�����true�����򷵻�false.
   */
	virtual bool isTextEnable( UINT32 uid ) =0;
/**
   * @brief �ж�ĳ���û��Ƿ���Է���.
   * @param uid���û�id.
   * @return ���Է��ԣ�����true�����򷵻�false.
   */
	virtual bool isVoiceEnable( UINT32 uid ) = 0;
/**
   * @brief �ж��ܷ��ֹĳ���û���������.
   * @param uid���û�id.
   * @return ��Ȩ�޷���true�����򷵻�false.
   */
	virtual bool canDisableText( UINT32 uid ) =0;
/**
   * @brief �ж��ܷ��ֹĳ���û�����.
   * @param uid���û�id.
   * @return ��Ȩ�޷���true�����򷵻�false.
   */
	virtual bool canDisableVoice( UINT32 uid ) =0;

/**
   * @brief ����ĳ���û��Ƿ���Է�������.
   * @param uid���û�id��enable���Ƿ�����.
   */
	virtual void asyncEnableText( UINT32 uid, bool enable ) = 0;
/**
   * @brief ����ĳ���û��Ƿ���Է���.
   * @param uid���û�id��enable���Ƿ�����.
   */
	virtual void asyncEnableVoice( UINT32 uid, bool enable ) = 0;

/*
signals:
    void textStatusChanged( UINT32 admin, UINT32 uid, bool isEnable );
    void voiceStatusChanged( UINT32 admin, UINT32 uid, bool isEnable );
*/
};
