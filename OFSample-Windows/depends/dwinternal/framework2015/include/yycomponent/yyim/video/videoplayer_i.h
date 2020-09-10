#pragma once
#include "devicesetting_i.h"


DWDEFINE_INTERFACE(IVideoPlayer) : public IDWComponent
{
	enum VideoFormatType
	{
		Type_UnKnow,
		Type_YUY2,
		Type_RGB24,
		Type_YV12,
		Type_I420,
	};
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT setupDirectDraw(HWND hWnd,IVideoDeviceSetting::VideoResolution resolutionSrc, IVideoDeviceSetting::VideoResolution resolutionDest,VideoFormatType type) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT setupNormalDraw(HWND hWnd,IVideoDeviceSetting::VideoResolution resolutionSrc, IVideoDeviceSetting::VideoResolution resolutionDest,VideoFormatType type) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual VideoFormatType getFormatType() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoDeviceSetting::VideoResolution currentRenderSize(void) = 0;
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
	virtual void clear(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void draw(BYTE *renderBuffer,long lBufferSize,int xDest,int yDest) = 0;
};
