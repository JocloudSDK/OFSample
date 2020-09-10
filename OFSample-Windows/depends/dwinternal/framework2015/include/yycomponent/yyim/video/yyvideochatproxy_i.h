#pragma  once
#include "yyvideochannelmgrproxy_i.h"

typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo, ImP2PVideoInfo::P2PVideoAction act)> _SIG_VIDEO_INV_RES;
// ��˫��ͬ����Ƶ���״̬���ڼ���ܻ��η������ź�
typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo, ImP2PVideoInfo::P2PVideoStatus status,ImP2PVideoInfo::eExternReason reason)> _SIG_VIDEO_CONN_STATUS;

typedef xsignals::signal<void (unsigned int )> _SIG_VIDEO_STOP;
//��Ƶ�豸�β�
typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo, const ImP2PVideoInfo::P2PVideoCameraStatus enStatus)> _SIG_VIDEO_CAMERA_STATUS;
// ͨ�������е�ping
typedef xsignals::signal<void (const UINT32 nPing)> _SIG_VIDEO_PING_VALUE;

typedef xsignals::signal<void (const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo)> _SIG_ACCEPT_VIDEO_SESSION;

typedef xsignals::signal<void ()> _SIG_INIT_AUDIO_CAPTURE;

typedef xsignals::signal<void (int )> _SIG_AUDIO_SPEAKER_VOLUME;

typedef xsignals::signal<void (bool )> _SIG_AUDIO_SPEAKER_MUTE;

DWDEFINE_INTERFACE(IImVideoChatProxy) : public IDWComponent
{
	/**
	* @brief . ���ͷ�������Ƶ
	* @param . p2pVideoInfo ������Ƶ��Ϣ
	* @return. ��
	*/
	virtual const std::string VideoInvite(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION &p2pVideoInfo) = 0;
	/**
	* @brief . ���շ�������Ƶ
	* @param . p2pVideoInfo ������Ƶ��Ϣ
	* @return. ��
	*/
	virtual void VideoAccept( const ImP2PVideoInfo::P2P_VIDEO_INFORMATION &p2pVideoInfo ) = 0;

	/**
	* @brief . �ܾ���Ƶ
	* @param . ��
	* @return. ��
	*/
	virtual void VideoReject() = 0;

	/**
	* @brief . ��ֹ��Ƶ
	* @param . stopStatus ��ֹ������
	* @return. ��
	*/
	virtual void VideoStop(ImP2PVideoInfo::StopVideoStatus stopStatus) = 0;
	/**
	* @brief . 
	* @param .
	* @return .
	*/
	virtual bool TransmitCameraStat(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION myInfo,const ImP2PVideoInfo::P2PVideoCameraStatus &enStatus) = 0;

	/**
	* @brief . 
	* @param .
	* @return. 
	*/
	virtual ImP2PVideoInfo::P2PVideoStatus GetCurrentVideoStatus() = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void SendEncodeFrame(const std::string &encodeData,ImP2PVideoInfo::VideoFrameType eframetype,int ttl) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void VideoDecodeData(const std::string &decodeData) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void InsertHistory(const std::wstring& msg) = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual ImP2PVideoInfo::Status actState() = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void setActState(ImP2PVideoInfo::Status status) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void setPeerUnionSuppIndicate(const bool isPeerSuppUnionChan, const std::string &strGuid) = 0;//���ڵײ�1V1���ݴ���ͳһͨ���汾������,�����ڽ���.

	//����ĺ���������ת���߳�����ʼ��audio��audio��֧���̰߳�ȫ������ֻ�������ַǳ����������
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void InitAudioCapture(void) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void UninitAudioCapture(void) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void StartAudioCapture(void) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void StopAudioCapture(void) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void InitAudioRender(void) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void UnInitAudioRender(void) = 0;

	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void setVideoSpeakerVolume( int value ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void setVideoSpeakerMute(bool bMute) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void setVideoMicphoneVolume( int value ) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void setVideoMicphoneMute(bool bMute) = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual IImVideoChannelMgrProxyPtr getVideoChannelMgr() = 0;
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::string videoGuid() = 0;
};

DWDEFINE_INTERFACE(IImVideoChatProxy2) : public IImVideoChatProxy
{
	virtual void videoTransmitAction(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION &myInfo,ImP2PVideoInfo::eP2PUnionAppAction videoAction) = 0;
};

DWDEFINE_INTERFACE(IImVideoMagicalBoxProxy) : public IUnk
{
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual std::string VideoMagicalBoxInvite(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION &p2pVideoInfo ) = 0;
};