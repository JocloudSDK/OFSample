#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(ISWSCaleHelper) : public IDWComponent 
{
	enum RgbType
	{
		RGB24,
		BGR24,
		RGB32,
		BGR32,
	};
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initYUV2ToRGB(int srcWidth,int srcHeight,int destWidth,int destHeight,int destBitcount,RgbType rgbType) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initRGBToYV12(int srcWidth,int srcHeight,int destWidth,int destHeight,int srcBitcount,RgbType rgbType) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initYV12ToRGB(int srcWidth,int srcHeight,int destWidth,int destHeight,int destBitcount,RgbType rgbType) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initYUV2ToYUV2(int srcWidth,int srcHeight,int destWidth,int destHeight) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initRGBToRGB(int srcWidth,int srcHeight,int destWidth,int destHeight,int srcBitcount,int destBitcount,RgbType srcRgbType,RgbType desRgbType) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initYV12ToYV12(int srcWidth,int srcHeight,int destWidth,int destHeight) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convertYUV2ToRGB(unsigned char *inBuffer, int bufferLen, unsigned char *& outBuffer,unsigned int &writeSize) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convertYV12ToRGB(unsigned char *inBuffer, int bufferLen,unsigned char *& outBuffer,unsigned int &writeSize) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convertRGBToYV12(unsigned char *inBuffer, int bufferLen,bool bNeedVerticalConvert,unsigned char *& outBuffer) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convertYV12ToYV12(unsigned char *inBuffer, int bufferLen,unsigned char *& outBuffer) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convertYUV2ToYUV2(unsigned char *inBuffer, int bufferLen,unsigned char *& outBuffer,unsigned int &writeSize) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool convertRGBToRGB(unsigned char *inBuffer, int bufferLen,bool bNeedVerticalConvert,unsigned char *& outBuffer,unsigned int &writeSize) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual void clear() = 0;

	virtual void initYUV2ToYV12(int srcWidth,int srcHeight,int destWidth,int destHeight) = 0;

	virtual bool convertYUV2ToYV12(unsigned char *inBuffer, int bufferLen,bool bNeedVerticalConvert,unsigned char *& outBuffer,unsigned int &writeSize) = 0;
};