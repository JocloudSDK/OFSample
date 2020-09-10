#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/
DWDEFINE_INTERFACE(IDWVoiceMail):public IUnk
{
/**
   * @brief . 
   * @param .
   * @return .
   */
	virtual bool isRecording() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool start(const TCHAR* pOutFileName, bool bMobile ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stop(UINT32 uId) = 0;

	// 格式转换
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convert(const TCHAR* pSrcFileName, const TCHAR* pDstFileName) = 0;

	// 播放
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void playFile(TCHAR* pFileName, bool bMobile) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stopPlay() = 0;

	//暂停
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual  void pause() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual  void resume() = 0;

	// 信号
	typedef _def_boost_signals2_signal_type<void (int nType)>::type _SIG_VOICE_ERROR;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_ERROR *getVoiceErrorSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_VOICE_PALY_OVER;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_PALY_OVER *getVoicePalyOverSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32)>::type _SIG_VOICE_RECORD_OVER;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_RECORD_OVER *getVoiceRecordOverSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (int nValue)>::type _SIG_VOICE_INPUT_RECORD_VALUE;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_INPUT_RECORD_VALUE *getInputVoiceValueSignal() = 0;
};