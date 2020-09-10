#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


#define WAVE_ANSWER_QUESTION_1			1	//答题1
#define WAVE_ANSWER_QUESTION_2			2	//答题2
#define WAVE_ANSWER_QUESTION_3			3	//答题3
#define WAVE_ANSWER_QUESTION_4			4	//答题4
#define WAVE_JOIN						5	//进入频道
#define WAVE_LEAVE						6	//离开频道
#define WAVE_NOTICE						7	//被管理员踢出房间/被管理员禁止说话/进入子频道失败
#define WAVE_TUOREN						8	//被管理员拖动到其他频道
#define WAVE_EMERGENCY					9	//后台发布紧急情况
#define WAVE_IM_MESSAGE					10	//IM好友消息
#define WAVE_IM_SYSTEM					11	//IM系统消息
#define WAVE_PRIVATE_CHAT				12	//私聊消息
#define WAVE_GROUP_MSG					13  //群消息声音
#define WAVE_GROUP_SYSMSG				14  //群系统消息声音
#define WAVE_IM_VOICE					15  //语音邀请声音
#define	WAVE_ACTIVITY_WND_FIRST_SHOWN	16	//活动提醒弹窗弹出
#define WAVE_ACTIVITY_BEGIN				17	//活动开始时提醒

#define WAVE_CMD_JOIN					18	//进入指挥通话频道
#define WAVE_CMD_LEAVE					19	//离开指挥通话频道
#define WAVE_CMD_START_SPEAK			20	//指挥通话开始有人说话时

#define WAVE_UNMUTE_MICROPHONE			21	//使用热键打开频道麦克风时播放声音

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYWaveEngine): public IUnk
{

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool playWave(const std::wstring &fileName, bool bStopCur /*= false*/) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void playWave(int id, bool bStopCur /*= false*/) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void enableWave(bool bEnable) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool playMp3(const std::wstring &fileName, UINT32 fromPeerUid /*=0*/ ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stopPlay(UINT32 fromPeerId /*=0*/ ) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void uninit() = 0;
};