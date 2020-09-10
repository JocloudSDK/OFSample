#pragma once
#include "videocapture_i.h"
#include "yyvideochannelmgrproxy_i.h"

DWDEFINE_INTERFACE(IVideoJitterBuffer) : public IDWComponent
{
public:
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT initPeerVideoSetting(IVideoDeviceSetting::VideoResolution peerResolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInit(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initAudio(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void decodePacket(char *buffer,int length) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void waittingForNextIframe(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setVideoPlayBufferFrameCount(int bufferFrameCount) = 0;
	//返回值true 代表有效的,false 代表丢帧
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool currentFrameType(int &frameType) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getAvgVideoFrameDelay()  = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isVideoPackage() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setChannelMgr(IImVideoChannelMgrProxyPtr channelMgr) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setFirstFrameFlag( void ) = 0;

	enum ChangeResolutionType
	{
		change_higher,
		change_lower
	};
};

DWDEFINE_INTERFACE(IVideoJitterBuffer2) :public IVideoJitterBuffer
{
	virtual void unInitAudio(void) = 0;
	virtual void setVideoFlag(ImP2PVideoInfo::eVideoCustomValue value) = 0;
};

typedef xsignals::signal<void ( ) > _def_lost_frame;
typedef xsignals::signal<void () > _def_first_buffercoming;
typedef xsignals::signal<void (UINT32) > _def_detect_delay;
typedef xsignals::signal<void(IVideoDeviceSetting::VideoResolution)> _def_resolution_change;
typedef xsignals::signal< void(unsigned char *buffer,int length) > _def_draw_image;
typedef xsignals::signal< void(const QByteArray&) > _def_receive_voice; 
