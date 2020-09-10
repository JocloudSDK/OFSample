#pragma  once
#include "videocapture_i.h"
#include "yyvideochatmgrproxy_i.h"


DWDEFINE_INTERFACE(IYYP2PVideoMonitor) : public IDWComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initSession( unsigned int peerid,IImVideoChatMgrProxyPtr videoChatMgr,IImVideoChatProxyPtr videoChat) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setVideoGuid(const std::string &GUID) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void intVideoInvitation(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void acceptVideoChat(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo,HWND hMyWnd,HWND hPeerWnd) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initRenderWindow(HWND hMyWnd,HWND hPeerWnd) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool amIHaveCamera() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isPeerHaveCamera() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setPeerHaveVideoData(bool bHaveVideoData) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isPeerHaveVideoData() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setVideoChating(bool bVideoChating) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isVideoChating() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isSender() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initVideoReciver( IVideoDeviceSetting::VideoResolution resolution,QRect rcLarget ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInitVideo() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void resetVideoRenderSize(QRect rcLarget,QRect rcSmall,HWND smallView,HWND largeView,bool bInterchangeViewCheck ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void onVideoActionAccepted(const ImP2PVideoInfo::P2P_VIDEO_INFORMATION& vInfo);

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
	virtual bool isSpeakerMute() const  = 0;
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
	virtual bool isMicphoneMute() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setCanSendVideoData(bool bFlag) = 0;

	//开始监测2s内有没视频数据过来，有拆停止刷新背景
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void startDetectPeerHaveVideoData(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void resetVideoParam() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void startCameraAndAudio(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stopCameraAndAudio(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int videoDuartion(void) const = 0;
	
};

DWDEFINE_INTERFACE(IYYP2PVideoMonitor2) : public IYYP2PVideoMonitor
{
	virtual void sendPrepareVideoLinkByReceiver(IVideoDeviceSetting::VideoResolution resolution) = 0;
	virtual ImP2PVideoInfo::videoResolution getSenderInitialResolution( void ) = 0;
};

typedef xsignals::signal<void ()> _def_videodevice_conflict;
typedef xsignals::signal<void ()> _def_cannotfinddevice; 
typedef xsignals::signal<void (unsigned int )> _def_videoChatTimerAlert;
typedef xsignals::signal<void (unsigned int )> _def_testChannelRtt;
typedef xsignals::signal<void (bool )> _def_peerHasCamera;
typedef xsignals::signal<void (bool )> _def_meHasCamera;
typedef xsignals::signal<void (bool )> _def_eraseMyBackground;
typedef xsignals::signal<void (IVideoDeviceSetting::VideoResolution resolution) > _def_video_resolutionChanged;
typedef xsignals::signal< void(const QByteArray&)> _def_get_audio_data;
typedef xsignals::signal<void ()> _def_exist_videoinsetance;

