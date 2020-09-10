#pragma once
#include "dwbase/dwcomex.h"

/**
  * @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE( IYYChannelKaraokeScorePlayer ) : public IUnk
{
public:

	enum ReverbEffect
	{
		Normal		= 0xffffffff,	// 原声
		Ktv			= 0,			// KTV
		Hall		= 1,			// 大厅
		Theatre		= 2,			// 小剧场
		Odeum		= 5,			// 音乐厅
		Gymnasium	= 6,			// 体育馆
	};

/**
   * @brief 启动播放唱K评分模块.
   * @param scoreDllPath 评分dll路径.
   */
	virtual void startKaraokeScorePlayer( const std::wstring& scoreDllPath ) = 0;

/**
   * @brief 播放音频文件.
   * @param filePath 音频文件路径.
   * @param loop 循环播放标记.
   */
	virtual bool karaokePlay( const std::wstring& filePath, bool loop ) = 0;

/**
   * @brief 暂停.
   */
	virtual void karaokePause() = 0;

/**
   * @brief 继续.
   */
	virtual void karaokeResume() = 0;

/**
   * @brief 结束.
   */
	virtual void karaokeStop() = 0;
	
/**
   * @brief 设置音量.
   * @param vol 音量值0~100.
   */
	virtual void karaokeSetVolume( int vol ) = 0;

/**
   * @brief 获取音量.
   * @return 音量值0~100.
   */
	virtual int karaokeGetVolume() = 0;

/**
   * @brief 获取正在播放的音频文件播放时长.
   * @return 正在播放的音频文件播放时长ms.
   */
	virtual int getKaraokeFileTime() = 0;

/**
   * @brief 根据文件路径获取音频文件播放时长.
   * @param filePath 音频文件路径.
   * @return 音频文件播放时长ms.
   */
	virtual int getKaraokeFileTime2( const std::wstring& filePath ) = 0;

/**
   * @brief 获取正在播放的音频文件播放时间.
   * @return 正在播放的音频文件播放时间ms.
   */
	virtual int getKaraokeCurTime() = 0;

/**
   * @brief 打开音频特征文件.
   * @param filePath 特征文件路径.
   */
	virtual void openKaraokeRefFile( const std::wstring& filePath ) = 0;

/**
   * @brief 获取基准评分.
   * @param timeParts 歌词时间片.
   * @return pitches 基准评分.
   */
	virtual void SetScoreTimeRange( const std::vector< TimePart >& timeParts, std::vector< UINT32 >& pitches ) = 0;

/**
   * @brief 停止播放音频.
   */
	virtual void stopKaraokeScorePlayer() = 0;

/**
   * @brief 应用特效.
   * @param reverb 特效值.
   */
	virtual void applyReverbSetting( UINT32 reverb ) = 0;

	typedef _def_boost_signals2_signal_type<void ( UINT32 score )>::type	_SIG_CALC_KARAOKE_SCORE;
	typedef _def_boost_signals2_signal_type<void ( UINT32 pitch )>::type	_SIG_CALC_PITCH;
	typedef _def_boost_signals2_signal_type<void ()>::type					_SIG_FINISH;

/**
   * @brief 获取评分信号.
   * @return 评分信号.
   */
	virtual _SIG_CALC_KARAOKE_SCORE* getCalcKaraokeScoreSignal() = 0;

/**
   * @brief 获取级别信号.
   * @return 级别信号.
   */
	virtual _SIG_CALC_PITCH* getCalcPitchSignal() = 0;

/**
   * @brief 获取结束信号.
   * @return 结束信号.
   */
	virtual _SIG_FINISH* getFinishSignal() = 0;

/**
   * @brief 麦克风回放启用禁用.
   */
	virtual void setRealTimePlayback( bool enable ) = 0;

/**
   * @brief 打开音频特征文件.
   * @param filePath 特征文件路径.
   * @return true 成功,false 失败.
   */
	virtual bool openKaraokeRefFile2( const std::wstring& filePath ) = 0;
};