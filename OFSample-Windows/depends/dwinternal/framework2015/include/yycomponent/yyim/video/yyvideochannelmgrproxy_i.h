#pragma once
#include "videoinfo.h"
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

typedef xsignals::signal< void(unsigned short,bool bReady) > _def_chan_ready;
typedef xsignals::signal< void() > _def_punch_ready;
typedef xsignals::signal< void() > _def_server_ready;
typedef xsignals::signal< void(ImP2PVideoInfo::videoResolution resolution) > _def_change_resoltuion;
typedef xsignals::signal < void (unsigned int) > _def_test_channel_rtt; 
typedef xsignals::signal< void (unsigned short) > _def_change_channel;
typedef xsignals::signal< void(short serverNum, bool bPunch,ImP2PVideoInfo::eVideoChanMethod,short serverhots) > _def_channelinfo_report;
typedef xsignals::signal<void (const std::string &decodeData)> _SIG_VIDEO_DECODE_DATA;

DWDEFINE_INTERFACE(IImVideoChannelMgrProxy) : IDWComponent
{

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initVideoQuality(ImP2PVideoInfo::eVideoQuality quality) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void SetVideoGuid(const std::string &guid) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initCaptureResolution(const std::vector<ImP2PVideoInfo::videoResolution> &vecCaptureResolution)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void FlushTransmitSendQueue(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool SwitchMainChan(unsigned short wMainChanNum) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool IsMainChannelReady() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setBandWidthDetectTimes(int count,bool bInit) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setSenderResolution(ImP2PVideoInfo::videoResolution senderResolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void clearChannel() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ImP2PVideoInfo::reoprtChannelInfo reportChannelInfo(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void bizLostFrame() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int getFrameRateByResolution(ImP2PVideoInfo::videoResolution resolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setInFullScreenMode(bool bFullScreen) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int bizCpuUsage()  = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool bizFullScreenMode() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setMaxSenderVideoResolution(ImP2PVideoInfo::videoResolution resolution) = 0;
};