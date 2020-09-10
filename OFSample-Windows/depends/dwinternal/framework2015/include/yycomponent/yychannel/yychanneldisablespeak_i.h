#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
// 频道禁止发言.

/**
* @brief .用户是否能够发送文字，说话接口
*/
DWDEFINE_INTERFACE(IChannelDisableSpeak) : public IDWQComponent
{
/**
   * @brief 判断某个用户是否可以发送文件.
   * @param Uid，用户id.
   * @return 可以发送，返回true，否则返回false.
   */
	virtual bool isTextEnable( UINT32 uid ) =0;
/**
   * @brief 判断某个用户是否可以发言.
   * @param uid，用户id.
   * @return 可以发言，返回true，否则返回false.
   */
	virtual bool isVoiceEnable( UINT32 uid ) = 0;
/**
   * @brief 判断能否禁止某个用户发送文字.
   * @param uid，用户id.
   * @return 有权限返回true，否则返回false.
   */
	virtual bool canDisableText( UINT32 uid ) =0;
/**
   * @brief 判断能否禁止某个用户发言.
   * @param uid，用户id.
   * @return 有权限返回true，否则返回false.
   */
	virtual bool canDisableVoice( UINT32 uid ) =0;

/**
   * @brief 设置某个用户是否可以发送文字.
   * @param uid，用户id；enable，是否允许.
   */
	virtual void asyncEnableText( UINT32 uid, bool enable ) = 0;
/**
   * @brief 设置某个用户是否可以发言.
   * @param uid，用户id；enable，是否允许.
   */
	virtual void asyncEnableVoice( UINT32 uid, bool enable ) = 0;

/*
signals:
    void textStatusChanged( UINT32 admin, UINT32 uid, bool isEnable );
    void voiceStatusChanged( UINT32 admin, UINT32 uid, bool isEnable );
*/
};
