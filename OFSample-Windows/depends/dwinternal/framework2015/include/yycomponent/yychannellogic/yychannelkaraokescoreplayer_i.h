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
		Normal		= 0xffffffff,	// ԭ��
		Ktv			= 0,			// KTV
		Hall		= 1,			// ����
		Theatre		= 2,			// С�糡
		Odeum		= 5,			// ������
		Gymnasium	= 6,			// ������
	};

/**
   * @brief �������ų�K����ģ��.
   * @param scoreDllPath ����dll·��.
   */
	virtual void startKaraokeScorePlayer( const std::wstring& scoreDllPath ) = 0;

/**
   * @brief ������Ƶ�ļ�.
   * @param filePath ��Ƶ�ļ�·��.
   * @param loop ѭ�����ű��.
   */
	virtual bool karaokePlay( const std::wstring& filePath, bool loop ) = 0;

/**
   * @brief ��ͣ.
   */
	virtual void karaokePause() = 0;

/**
   * @brief ����.
   */
	virtual void karaokeResume() = 0;

/**
   * @brief ����.
   */
	virtual void karaokeStop() = 0;
	
/**
   * @brief ��������.
   * @param vol ����ֵ0~100.
   */
	virtual void karaokeSetVolume( int vol ) = 0;

/**
   * @brief ��ȡ����.
   * @return ����ֵ0~100.
   */
	virtual int karaokeGetVolume() = 0;

/**
   * @brief ��ȡ���ڲ��ŵ���Ƶ�ļ�����ʱ��.
   * @return ���ڲ��ŵ���Ƶ�ļ�����ʱ��ms.
   */
	virtual int getKaraokeFileTime() = 0;

/**
   * @brief �����ļ�·����ȡ��Ƶ�ļ�����ʱ��.
   * @param filePath ��Ƶ�ļ�·��.
   * @return ��Ƶ�ļ�����ʱ��ms.
   */
	virtual int getKaraokeFileTime2( const std::wstring& filePath ) = 0;

/**
   * @brief ��ȡ���ڲ��ŵ���Ƶ�ļ�����ʱ��.
   * @return ���ڲ��ŵ���Ƶ�ļ�����ʱ��ms.
   */
	virtual int getKaraokeCurTime() = 0;

/**
   * @brief ����Ƶ�����ļ�.
   * @param filePath �����ļ�·��.
   */
	virtual void openKaraokeRefFile( const std::wstring& filePath ) = 0;

/**
   * @brief ��ȡ��׼����.
   * @param timeParts ���ʱ��Ƭ.
   * @return pitches ��׼����.
   */
	virtual void SetScoreTimeRange( const std::vector< TimePart >& timeParts, std::vector< UINT32 >& pitches ) = 0;

/**
   * @brief ֹͣ������Ƶ.
   */
	virtual void stopKaraokeScorePlayer() = 0;

/**
   * @brief Ӧ����Ч.
   * @param reverb ��Чֵ.
   */
	virtual void applyReverbSetting( UINT32 reverb ) = 0;

	typedef _def_boost_signals2_signal_type<void ( UINT32 score )>::type	_SIG_CALC_KARAOKE_SCORE;
	typedef _def_boost_signals2_signal_type<void ( UINT32 pitch )>::type	_SIG_CALC_PITCH;
	typedef _def_boost_signals2_signal_type<void ()>::type					_SIG_FINISH;

/**
   * @brief ��ȡ�����ź�.
   * @return �����ź�.
   */
	virtual _SIG_CALC_KARAOKE_SCORE* getCalcKaraokeScoreSignal() = 0;

/**
   * @brief ��ȡ�����ź�.
   * @return �����ź�.
   */
	virtual _SIG_CALC_PITCH* getCalcPitchSignal() = 0;

/**
   * @brief ��ȡ�����ź�.
   * @return �����ź�.
   */
	virtual _SIG_FINISH* getFinishSignal() = 0;

/**
   * @brief ��˷�ط����ý���.
   */
	virtual void setRealTimePlayback( bool enable ) = 0;

/**
   * @brief ����Ƶ�����ļ�.
   * @param filePath �����ļ�·��.
   * @return true �ɹ�,false ʧ��.
   */
	virtual bool openKaraokeRefFile2( const std::wstring& filePath ) = 0;
};