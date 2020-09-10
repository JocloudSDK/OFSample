#pragma once
#include "videocapture_i.h"

struct AVFrame;

DWDEFINE_INTERFACE(IVideoJitterBufferPlayer) : public IDWComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT initVideo(HWND hWnd, IVideoDeviceSetting::VideoResolution videoresolution,
		IVideoDeviceSetting::VideoResolution resolutionDest) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual HRESULT initAudio(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInit() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInitVideo(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HWND renderWnd(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  getVolume() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setVolume(int vol) =0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool getMute() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setMute(bool mute) = 0;

	//以下函数为内部使用
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void drawLastFrame(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void playSound(const QList<QByteArray> &audioList) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void framePacket(AVFrame * picture, int width, int height ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual QByteArray decodeAudioPacket(const char * pbuf, int size) = 0;

	virtual unsigned char* yuvBuffer(void) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int yuvBufferLength(void) const = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void drawFrame(unsigned char * pbuf, int size) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void detectAudio(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoDeviceSetting::VideoResolution destResolution(void) const = 0;
};

DWDEFINE_INTERFACE(IVideoJitterBufferPlayer2) : public IVideoJitterBufferPlayer
{
	virtual void unInitAudio(void) = 0;
};