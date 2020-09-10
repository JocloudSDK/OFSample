#pragma once
#include <string>
#include "videocapture_i.h"


typedef xsignals::signal<void (const std::string &encodeData,int frametype) > _def_packet_buffer;

DWDEFINE_INTERFACE(IVideoPacketBuffer) : public IDWComponent
{
public:
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initMyVideoResolution(IVideoDeviceSetting::VideoResolution resolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setHasVideo(bool bHaveVideo) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void packageVideo(char *buffer,int len,int frametype,unsigned char simpaleTime) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void packageAudio(const std::string &audiobuffer) = 0;
};