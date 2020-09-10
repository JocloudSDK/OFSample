#pragma once
#include "VideoEnumDef.h"
#include <vector>

 namespace VideoSdk
 {

//ʹ�����̣�����һ������ID, createPublish -> setPublishPluginConfig -> setPublishStatus -> GetCameraList ->  SetCameraNumber -> StartPublishVideo


//////////////////////////////////////////////////////////////////////////
//app => VSDK (eventId: 10 ~ 9999)

//virtual void CreatePublish(unsigned publishId) = 0;
struct ETCreatePublish : public ETVsdkInvoke
{
	enum { evId = 10};
	ETCreatePublish(unsigned publishId_) 
		: publishId(publishId_)
	{
		m_eventId = evId; 
	}

	unsigned publishId;
};

//virtual void SetPublishPluginConfig(unsigned publishId, const std::map<unsigned char, unsigned>& configMap) = 0;
struct ETSetPublishPluginConfig : public ETVsdkInvoke
{
	enum { evId = 12};
	ETSetPublishPluginConfig(unsigned publishId_, const std::map<unsigned char, unsigned>& configMap_) 
		: publishId(publishId_)
		, configMap(configMap_)
	{
		m_eventId = evId;
	}

 	unsigned publishId;
 	const std::map<unsigned char, unsigned>& configMap;
};

// virtual int GetCameraList2(unsigned publishId, const char ***camList) = 0;
struct ETGetCameraList : public ETVsdkInvoke
{
	enum { evId = 14};
	ETGetCameraList(unsigned publishId_, const char ***camList_)
	: publishId(publishId_)
	, camList(camList_)
	, camNum(0)
	{
		m_eventId = evId; 
	}

	unsigned publishId;
	const char ***camList; /* out */
	int camNum; /* out */
};

//virtual void SetCameraNumber2(unsigned publishId, unsigned number) = 0;
struct ETSetCameraNumber : public ETVsdkInvoke
{
	enum { evId = 16};
	ETSetCameraNumber(unsigned publishId_, unsigned number_)
		: publishId(publishId_)
		, number(number_)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	unsigned number;
};

//virtual void ReleaseCurrentCamera2(unsigned publishId) = 0;
struct ETReleaseCurrentCamera : public ETVsdkInvoke
{
	enum { evId = 18};
	ETReleaseCurrentCamera(unsigned publishId_) 
		: publishId(publishId_)
	{ 
		m_eventId = evId; 
	}

	unsigned publishId;
};

//virtual void SetVideoEnhanceConfig2(unsigned publishId, bool isOpenVideoEnhance, unsigned noise, unsigned sharp) = 0;
struct ETSetVideoEnhanceConfig : public ETVsdkInvoke
{
	enum { evId = 20};
	ETSetVideoEnhanceConfig(unsigned publishId_, bool isOpenVideoEnhance_, unsigned noise_, unsigned sharp_) 
		: publishId(publishId_)
		, isOpenVideoEnhance(isOpenVideoEnhance_)
		, noise(noise_)
		, sharp(sharp_)
	{ 
		m_eventId = evId;
	}

	unsigned publishId;
	bool isOpenVideoEnhance;
	unsigned noise;
	unsigned sharp;
};

//virtual void SetPublishStatus2(unsigned publishId, bool isPublisher) = 0;
struct ETSetPublishStatus : public ETVsdkInvoke
{
	enum { evId = 22};
	ETSetPublishStatus(unsigned publishId_, bool isPublisher_)
		: publishId(publishId_)
		, isPublisher(isPublisher_)
	{
		m_eventId = evId; 
	}

	unsigned publishId;
	bool isPublisher;
};

//virtual void StartPublishVideo2(unsigned publishId) = 0;
struct ETStartPublishVideo : public ETVsdkInvoke
{
	enum { evId = 24};
	ETStartPublishVideo(unsigned publishId_)
		: publishId(publishId_)
	{ 
		m_eventId = evId; 
	}

	unsigned publishId;
};

//virtual void StopPublishVideo2(unsigned publishId) = 0;
struct ETStopPublishVideo : public ETVsdkInvoke
{
	enum { evId = 26};
	ETStopPublishVideo(unsigned publishId_)
		: publishId(publishId_)
	{
		m_eventId = evId;
	}

	unsigned publishId;
};

//virtual bool StartDesktopCapture2(unsigned publishId, int width, int height, D3DFMT fmt) = 0;
struct ETStartDesktopCapture : public ETVsdkInvoke
{
	enum { evId = 28};
	ETStartDesktopCapture(unsigned publishId_, int width_, int height_, D3DFMT fmt_)
		: publishId(publishId_)
		, width(width_)
		, height(height_)
		, fmt(fmt_)
		, result(false)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	int width;
	int height;
	D3DFMT fmt;
	bool result; /* out */
};

//  virtual void StopDesktopCapture2(unsigned publishId) = 0;
struct ETStopDesktopCapture : public ETVsdkInvoke
{
	enum { evId = 30};
	ETStopDesktopCapture(unsigned publishId_)
		: publishId(publishId_)
	{ 
		m_eventId = evId;
	}

	unsigned publishId;
};


//  virtual void StartSubscribeVideo2(unsigned uid, unsigned publishId) = 0;
struct ETStartSubscribeVideo : public ETVsdkInvoke
{
	enum { evId = 32};
	ETStartSubscribeVideo(unsigned publishId_, unsigned uid_)
		: publishId(publishId_)
		, uid(uid_)
	{
		m_eventId = evId; 
	}

	unsigned uid;
	unsigned publishId;
};

//  virtual void StopSubscribeVideo2(unsigned uid, unsigned publishId) = 0;
struct ETStopSubscribeVideo : public ETVsdkInvoke
{
	enum { evId = 34};
	ETStopSubscribeVideo(unsigned publishId_, unsigned uid_) 
		: publishId(publishId_)
		, uid(uid_)
	{
		m_eventId = evId;
	}

	unsigned uid;
	unsigned publishId;
};

//  virtual void ResetFrameSize2(unsigned uid, unsigned publishId, int width,int height) = 0;
struct ETResetFrameSize : public ETVsdkInvoke
{
	enum { evId = 36};
	ETResetFrameSize(unsigned publishId_, unsigned uid_, int width_, int height_) 
		: publishId(publishId_)
		, uid(uid_)
		, width(width_)
		, height(height_)
	{
		m_eventId = evId;
	}

	unsigned uid;
	unsigned publishId;
	int width;
	int height;
};


//  virtual void SetBitmapBits2(unsigned publishId, unsigned char *buffer, unsigned len) = 0;
struct ETSetBitmapBits : public ETVsdkInvoke
{
	enum { evId = 38};
	ETSetBitmapBits(unsigned publishId_, unsigned char* buffer_, unsigned len_)
		: publishId(publishId_)
		, buffer(buffer_)
		, len(len_)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	unsigned char *buffer;
	unsigned len;
};

//  virtual bool SetBitmapBitsInPlace2(unsigned publishId, unsigned char *buffer, unsigned len, unsigned rcHandle) = 0;
struct ETSetBitmapBitsInPlace : public ETVsdkInvoke
{
	enum { evId = 40};
	ETSetBitmapBitsInPlace(unsigned publishId_, unsigned char* buffer_, unsigned len_, unsigned rcHandle_)
		: publishId(publishId_)
		, buffer(buffer_)
		, len(len_)
		, rcHandle(rcHandle_)
		, result(false)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	unsigned char *buffer;
	unsigned len;
	unsigned rcHandle;
	bool result; /* out */
};

//  virtual unsigned long long GetMyStreamId2(unsigned publishId) = 0;
struct ETGetMyStreamId : public ETVsdkInvoke
{
	enum { evId = 42};
	ETGetMyStreamId(unsigned publishId_)
		: publishId(publishId_)
		, streamId(0)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	unsigned long long streamId; /* out */
};

//  virtual unsigned long long GetPublisherStreamId2(unsigned publisherUid, unsigned publishId) = 0;
struct ETGetPublisherStreamId : public ETVsdkInvoke
{
	enum { evId = 44};
	ETGetPublisherStreamId(unsigned publishId_, unsigned uid_) 
		: publishId(publishId_)
		, uid(uid_)
		, streamId(0)
	{
		m_eventId = evId;
	}

	unsigned uid;
	unsigned publishId;
	unsigned long long streamId; /* out */
};

//  virtual unsigned GetStaticsProps2(unsigned publishId, unsigned key) = 0;
struct ETGetStaticsProps : public ETVsdkInvoke
{
	enum { evId = 46};
	ETGetStaticsProps(unsigned publishId_, unsigned key_)
		: publishId(publishId_)
		, key(key_)
		, result(false)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	unsigned key;
	unsigned result; /* out */
};

//  virtual bool IsPublishing2(unsigned publishId) const = 0;
struct ETIsPublishing : public ETVsdkInvoke
{
	enum { evId = 48};
	ETIsPublishing(unsigned publishId_)
		: publishId(publishId_)
		, isPublishing(false)
	{
		m_eventId = evId; 
	}

	unsigned publishId;
	bool isPublishing; /* out */
};

//////////////////////////////////////////////////////////////////////////
//��publishId��صĽӿ�

//virtual void connect() = 0;
struct ETConnect : public ETVsdkInvoke
{
	enum {evId = 100};
	ETConnect() { m_eventId = evId; };
};

//virtual void recvVideoAppData(const char* data, unsigned len) = 0;
struct ETRecvVideoAppData : public ETVsdkInvoke
{
	enum {evId = 101};
	ETRecvVideoAppData(const char* data_, unsigned len_)
		: data(data_)
		, len(len_)
	{
		m_eventId = evId;
	};

	const char* data;
	unsigned len;
};

//virtual void setExtraMetaData(const std::map<unsigned char, unsigned>& metas) = 0;
struct ETSetExtraMetaData : public ETVsdkInvoke
{
	enum {evId = 102};
	ETSetExtraMetaData(const std::map<unsigned char, unsigned>& metas_)
		: metas(metas_)
	{
		m_eventId = evId;
	};

	const std::map<unsigned char, unsigned>& metas;
};

//virtual void getExtraMetaData(unsigned uid, std::map<unsigned char, unsigned>& metas) = 0;
struct ETGetExtraMetaData : public ETVsdkInvoke
{
	enum {evId = 103};
	ETGetExtraMetaData(unsigned uid_, std::map<unsigned char, unsigned>& metas_)
		: uid(uid_)
		, metas(metas_)
	{
		m_eventId = evId;
	};

	unsigned uid;
	std::map<unsigned char, unsigned>& metas;
};

//virtual void setPluginConfig(const std::map<unsigned char, unsigned>& configMap) = 0;
struct ETSetPluginConfig : public ETVsdkInvoke
{
	enum { evId = 104 };
	ETSetPluginConfig(const std::map<unsigned char, unsigned>& configMap_)
		: configMap(configMap_)
	{
		m_eventId = evId;
	}

	const std::map<unsigned char, unsigned>& configMap;
};

struct ETGetPluginConfig : public ETVsdkInvoke
{
	enum { evId = 105 };
	ETGetPluginConfig()
	{ 
		m_eventId = evId;
	}

	std::map<unsigned char, unsigned> configMap;
};

struct ETGetPublishPluginConfig : public ETVsdkInvoke
{
	enum { evId = 13 };
	ETGetPublishPluginConfig(unsigned publishId_)
		: publishId(publishId_)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	std::map<unsigned char, unsigned> configMap;
};

//virtual void getPublisherUids(std::set<unsigned>& uidsSet) = 0;
struct ETGetPublisherUids : public ETVsdkInvoke
{
	enum { evId = 106 };
	ETGetPublisherUids(unsigned publishId_)
		: publishId(publishId_)
	{
		m_eventId = evId;
	}
	
	unsigned publishId;
	std::set<unsigned> uidsSet;
};

//�Ƿ�����YY������ƵΪ����������Ƶ
//virtual void SetPreferToYYVcam(bool bPreferToYYVcam = true) = 0;
struct ETSetPreferToYYVcam : public ETVsdkInvoke
{
	enum { evId = 108 };
	ETSetPreferToYYVcam(bool bPreferToYYVcam_ = true)
		: bPreferToYYVcam(bPreferToYYVcam_)
	{
		m_eventId = evId;
	}

	bool bPreferToYYVcam;
};

struct ETGetUserInfo : public ETVsdkInvoke
{
	enum { evId = 109 };
	ETGetUserInfo()
	: isp(0)
	{
		m_eventId = evId;
	}

	unsigned isp;
};

//virtual void StartRtmpPublish(unsigned proxyType, std::string& url, std::string& playPath) = 0;
struct ETStartRtmpPublish : public ETVsdkInvoke
{
	enum { evId = 110 };
	ETStartRtmpPublish(unsigned publishId_, unsigned proxyType_, std::string url_, std::string playPath_)
		: publishId(publishId_), proxyType(proxyType_), url(url_), playPath(playPath_)
	{
		m_eventId = evId;
	}

	unsigned publishId;
	unsigned proxyType;
	std::string url;
	std::string playPath;
};

//virtual void StopRtmpPublish() = 0;
struct ETStopRtmpPublish : public ETVsdkInvoke
{
	enum { evId = 111 };
	ETStopRtmpPublish()
	{
		m_eventId = evId;
	}
};

//����ʹ��
//virtual void setPluginDecodeAudioVideoStat(unsigned publisherUid, unsigned publishId, const std::map<unsigned, unsigned>& audioFrameIdToPlayStamp,
//											std::map<unsigned, unsigned>& videoFrameIdToRenderStamp) = 0;
struct ETPluginDecodeAudioVideoStat : public ETVsdkInvoke
{
	enum { evId = 112 };
	ETPluginDecodeAudioVideoStat()
		: m_publisherUid(0)
		, m_publishId((unsigned) -1)
	{
		m_eventId = evId;
	}

	unsigned m_publisherUid;									//����Ƶ��������uid
	unsigned m_publishId;										//����Ƶ��������publishId
	std::map<unsigned, unsigned> m_audioFrameIdToPlayStamp;		//key����Ƶ֡��� value��TimeGetTime��ȡ������ʱ���
	std::map<unsigned, unsigned> m_videoFrameIdToRenderStamp;	//key����ƵƵ֡��� value��TimeGetTime��ȡ����Ⱦʱ���
};

//����ʹ��
//virtual void setPluginPlayVideoFrame(unsigned publisherUid, unsigned publishId, const unsigned char* data, const BITMAPINFOHEADER &bitmapHeader) = 0;
struct ETPluginPlayVideoFrame : public ETVsdkInvoke
{
	enum { evId = 113 };
	ETPluginPlayVideoFrame()
		: m_publisherUid(0)
		, m_publishId((unsigned) -1)
	{
		m_eventId = evId;
	}

	unsigned m_publisherUid;
	unsigned m_publishId;
	unsigned char* m_data;
	BITMAPINFOHEADER m_bitmapHeader;
};

struct ETStartServerRecord : public ETVsdkInvoke
{
	enum { evId = 200 };
	ETStartServerRecord(RecordMode mode, std::set<unsigned> recordUidSet, const std::string& userMark)
		: m_mode(mode)
		, m_recordUidSet(recordUidSet)
		, m_mark(userMark)
		, m_programId("")
	{
		m_eventId = evId;
	}
	
	ETStartServerRecord(RecordMode mode, std::set<unsigned> recordUidSet, const std::string& userMark,const std::string& programId)
		: m_mode(mode)
		, m_recordUidSet(recordUidSet)
		, m_mark(userMark)
		, m_programId(programId)
	{
		m_eventId = evId;
	}

	RecordMode m_mode;
	std::set<unsigned> m_recordUidSet;
	std::string m_mark;
	std::string m_programId;
};

struct ETStartDetectBandWidth : public ETVsdkInvoke
{
	enum { evId = 201};
	ETStartDetectBandWidth()
	{
		m_eventId = evId;
	}
};

struct ETCancelDetectBandWidth : public ETVsdkInvoke
{
	enum { evId = 202};
	ETCancelDetectBandWidth()
	{
		m_eventId = evId;
	}
};

struct ETSetBroadcastGroup : public ETVsdkInvoke	//����Ҫ���Ĺ㲥��
{
	enum { evId = 203};
	ETSetBroadcastGroup(unsigned speakerAppId, unsigned channelId)
	{
		m_eventId = evId;
		m_channelId = channelId;
		m_speakerAppId = speakerAppId;
	}
	unsigned m_channelId;				//[in] Ҫ���õ�channelId,���õ�channelId�ο�ETAudienceStreamConfig�Ļص�֪ͨ
	unsigned m_speakerAppId;			//[in] Ҫ�ۿ�������APPID,�ο�ETAudienceStreamConfig�Ļص�֪ͨ
};

// ת������
struct ETSetTranscodeConifg : public ETVsdkInvoke
{
	enum { evId = 204};

	ETSetTranscodeConifg(unsigned channelId, unsigned publishId, unsigned frameRate, unsigned codeRate, 
		unsigned width, unsigned height, unsigned cropStream, std::map<unsigned short, unsigned>& metaDatas)
		: m_channelId(channelId)
		, m_publishId(publishId)
		, m_frameRate(frameRate)
		, m_codeRate(codeRate)
		, m_width(width)
		, m_height(height)
		, m_cropStream(cropStream)
	{
		m_eventId = evId;
		m_metaDatas.clear();
		m_metaDatas.insert(metaDatas.begin(), metaDatas.end());
	}

	unsigned m_channelId;		// ��Ҫ���͵�Ŀ��channelId
	unsigned m_publishId;
	unsigned m_frameRate;		// ֡��
	unsigned m_codeRate;		// ���ʣ���λbit/s
	unsigned m_width;			// �ֱ��ʣ���
	unsigned m_height;			// �ֱ��ʣ���
	unsigned m_cropStream;		// �Ƿ��Ǻ�������(0:�Ǻ�����;1:���вü��ĺ�����)
	std::map<unsigned short, unsigned> m_metaDatas;
};

// CDN����
struct ETSetCDNConfig : public ETVsdkInvoke
{
	enum { evId = 205};

	ETSetCDNConfig()
		: channelId(0)
		, publishId(0)
	{
		m_eventId = evId;
	}

	unsigned channelId;
	unsigned publishId;
	std::map<std::string, std::string> configs;
};

// �컭����
struct ETSetMixedVideoConfig : public ETVsdkInvoke
{
	enum { evId = 206};

	ETSetMixedVideoConfig()
		: channelId(0)
		, publishId(0)
		, frameRate(0)
		, codeRate(0)
		, height(0)
		, width(0)
		, encodeType(0)
	{
		m_eventId = evId;
	}

	unsigned channelId;
	unsigned publishId;
	unsigned frameRate;		// ֡��
	unsigned codeRate;		// ���ʣ�Ϊ0��ʾ���컭
	unsigned height;		// �ֱ��ʣ���
	unsigned width;			// �ֱ��ʣ���
	unsigned encodeType;	// ��������
};

// �ӽ�����
struct ETSetPerspectiveConfig : public ETVsdkInvoke
{
	enum { evId = 207};

	ETSetPerspectiveConfig()
		: channelId(0)
		, publishId(0)
		, id(0)
	{
		m_eventId = evId;
	}

	unsigned channelId;
	unsigned publishId;
	unsigned id; //�ӽǵ�id
};

// ��������
struct ETSetEncodingConfig : public ETVsdkInvoke
{
	enum { evId = 208};

	ETSetEncodingConfig()
		: channelId(0)
		, publishId(0)
		, encodingBitmap(0)
	{
		m_eventId = evId;
	}

	unsigned channelId;
	unsigned publishId;
	unsigned encodingBitmap; //0��ʾ���λ��ÿһλ��Ӧһ�ֱ��룬0:h264 ,1:h265, 3:vp8 ��, 
};

// ����������
struct ETSetCopyStreamConfig : public ETVsdkInvoke
{
	enum { evId = 209};

	ETSetCopyStreamConfig(unsigned channelId, unsigned publishId, unsigned fromchannelId, std::map<unsigned short, unsigned>& metaDatas)
		: m_channelId(channelId)
		, m_publishId(publishId)
		, m_fromChannelId(fromchannelId)
	{
		m_eventId = evId;
		m_metaDatas.clear();
		m_metaDatas.insert(metaDatas.begin(), metaDatas.end());
	}

	unsigned m_channelId;		// ��Ҫ���͵�Ŀ��channelId
	unsigned m_publishId;
	unsigned m_fromChannelId;	// ���Ƶ���Դ��channelId
	std::map<unsigned short, unsigned> m_metaDatas;
};

struct ETGetEncoderList : public ETVsdkInvoke
{
	enum { evId = 300};
	ETGetEncoderList(std::vector<int>& encoderList_)
		: encoderList(encoderList_)
	{
		m_eventId = evId; 
	}

	unsigned publishId;
	std::vector<int>& encoderList; /* out, using VideoSdk::EncCodecID for encoder reference*/
};

struct ETSetOriginalChannel : public ETVsdkInvoke
{
	enum { evId = 301};

	ETSetOriginalChannel(unsigned channelId, unsigned publishId)
		: m_channelId(channelId)
		, m_publishId(publishId)
	{
		m_eventId = evId;
	}

	unsigned m_channelId;
	unsigned m_publishId;
};

struct ETDeleteChannelConfig : public ETVsdkInvoke
{
	enum { evId = 302};

	ETDeleteChannelConfig(unsigned channelId, unsigned publishId)
		: m_channelId(channelId)
		, m_publishId(publishId)
	{
		m_eventId = evId;
	}

	unsigned m_channelId;
	unsigned m_publishId;
};

struct ETEnableDxRender : public ETVsdkInvoke
{
	enum { evId = 303};

	ETEnableDxRender()
	{
		m_eventId = evId;
		m_enable = false;
	}

	bool m_enable;
};

struct ETSetSimpleForwardConfig : public ETVsdkInvoke	//���ü򵥵�Ƶ��ת��(Ŀ��Ƶ���뵱ǰ��½��Ƶ��ʹ����ͬ�Ŀ�������)
{
	enum { evId = 304};

	ETSetSimpleForwardConfig(unsigned publishId, unsigned targetSid, unsigned targetTopSid)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_targetTopSid(targetTopSid)
	{
		m_eventId = evId;
	}

	unsigned m_publishId;
	unsigned m_targetSid;		//Ŀ��Ƶ������Ƶ����
	unsigned m_targetTopSid;	//Ŀ��Ƶ���Ķ���Ƶ����
};

struct ETDeleteForwardConfig : public ETVsdkInvoke
{
	enum { evId = 305};

	ETDeleteForwardConfig(unsigned publishId, unsigned targetSid)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
	{
		m_eventId = evId;
	}

	unsigned m_publishId;
	unsigned m_targetSid;		//Ŀ��Ƶ������Ƶ����
};

struct ETSetCustomForwardCfg : public ETVsdkInvoke
{
	enum { evId = 306};

	ETSetCustomForwardCfg(unsigned publishId, unsigned targetSid, unsigned targetTopSid, 
		unsigned targetChannelId, unsigned targetAppid, unsigned targetPublishId)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_targetTopSid(targetTopSid)
		, m_targetChannelId(targetChannelId)
		, m_targetAppid(targetAppid)
		, m_targetPublishId(targetPublishId)
	{
		m_eventId = evId;
		m_metaDatas.clear();
	}

	ETSetCustomForwardCfg(unsigned publishId, unsigned targetSid, unsigned targetTopSid, 
		unsigned targetChannelId, unsigned targetAppid, unsigned targetPublishId,
		std::map<unsigned short, unsigned>& metaDatas)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_targetTopSid(targetTopSid)
		, m_targetChannelId(targetChannelId)
		, m_targetAppid(targetAppid)
		, m_targetPublishId(targetPublishId)
	{
		m_eventId = evId;
		m_metaDatas.insert(metaDatas.begin(), metaDatas.end());
	}

	unsigned m_publishId;
	unsigned m_targetSid;
	unsigned m_targetTopSid;
	unsigned m_targetChannelId;
	unsigned m_targetAppid;
	unsigned m_targetPublishId;
	std::map<unsigned short, unsigned> m_metaDatas;
};

struct ETSetForwardTranscodeCfg	: public ETVsdkInvoke
{
	enum { evId = 307};

	ETSetForwardTranscodeCfg(unsigned publishId, unsigned targetSid, unsigned channelId, unsigned frameRate,
		unsigned codeRate, unsigned height, unsigned width, unsigned cropStream, std::map<unsigned short, unsigned>& metaDatas)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_channelId(channelId)
		, m_frameRate(frameRate)
		, m_codeRate(codeRate)
		, m_height(height)
		, m_width(width)
		, m_cropStream(cropStream)
	{
		m_eventId = evId;
		m_metaDatas.clear();
		m_metaDatas.insert(metaDatas.begin(), metaDatas.end());
	}

	unsigned m_publishId;
	unsigned m_targetSid;
	unsigned m_channelId;
	unsigned m_frameRate;		// ֡��
	unsigned m_codeRate;		// ����
	unsigned m_height;			// �ֱ��ʣ���
	unsigned m_width;			// �ֱ��ʣ���
	unsigned m_cropStream;		// �Ƿ��Ǻ�������(0:�Ǻ�����;1:���вü��ĺ�����)
	std::map<unsigned short, unsigned> m_metaDatas;
};

struct ETSetForwardCopyStreamCfg : public ETVsdkInvoke
{
	enum { evId = 308};

	ETSetForwardCopyStreamCfg(unsigned publishId, unsigned targetSid, unsigned channelId,
		unsigned fromChannelId, std::map<unsigned short, unsigned>& metaDatas)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_channelId(channelId)
		, m_fromChannelId(fromChannelId)
	{
		m_eventId = evId;
		m_metaDatas.clear();
		m_metaDatas.insert(metaDatas.begin(), metaDatas.end());
	}

	unsigned m_publishId;
	unsigned m_targetSid;
	unsigned m_channelId;		// ��Ҫ���͵�Ŀ��channelId
	unsigned m_fromChannelId;	// ���Ƶ���Դ��channelId
	std::map<unsigned short, unsigned> m_metaDatas;
};

struct ETDeleteForwardChannelCfg : public ETVsdkInvoke
{
	enum { evId = 309};

	ETDeleteForwardChannelCfg(unsigned publishId, unsigned targetSid, unsigned channelId)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_channelId(channelId)
	{
		m_eventId = evId;
	}

	unsigned m_publishId;
	unsigned m_targetSid;
	unsigned m_channelId;
};

struct ETNotifyRenderInfo : public ETVsdkInvoke
{
	enum { evId = 310};
	ETNotifyRenderInfo()
	{
		m_eventId = evId;
	}
	// (uid << 32 | publishId) -> FrameRenderInfo
	std::map<unsigned long long, std::vector<FrameRenderInfo> > m_renderInfoMap;
}; 

struct ETGetDecoderList : public ETVsdkInvoke
{
	enum { evId = 311};
	ETGetDecoderList(std::vector<int>& decoderList_)
		: decoderList(decoderList_)
	{
		m_eventId = evId; 
	}
	unsigned publishId;
	std::vector<int>& decoderList; /* out, using VideoSdk::DecCodecID for encoder reference*/
};

// �ص��Ƿ񳬸����ʿ���(SHQ: Super High Quality)
struct ETNotifyAudioSHQMode : public ETVsdkEvent
{
	enum { evId = 312};
	ETNotifyAudioSHQMode()
		: m_bSHQMode(false)
	{
		m_eventId = evId;
	}

	bool		m_bSHQMode;
};

struct ETSetWaterMarkList : public ETVsdkEvent
{
	enum { evId = 313};

	typedef struct WaterMark {
		unsigned uid;

		int posX;
		int posY;

		unsigned width;
		unsigned height;

		unsigned bgraSize;
		unsigned char *bgra;

		bool bOnlyForRecorder;

		unsigned char reserved[32];

		WaterMark() {
			memset(this, 0, sizeof(*this));
			bOnlyForRecorder = true;
		}
	} WaterMark;

	ETSetWaterMarkList(std::vector<ETSetWaterMarkList::WaterMark> &wmList)
		: watermarkList(wmList)
	{
		m_eventId = evId;
	}

	std::vector<ETSetWaterMarkList::WaterMark> &watermarkList;
};

struct ETSetMixStreamConfig : public ETVsdkEvent
{
	enum { evId = 314};
	ETSetMixStreamConfig(unsigned width, unsigned height, unsigned bitrate,
		unsigned fps, const std::map<unsigned, VideoSdk::Layout>& streamLayout)
		: m_width(width)
		, m_height(height)
		, m_bitrate(bitrate)
		, m_fps(fps)
	{
		m_eventId = evId;
		m_streamLayout.clear();
		m_streamLayout.insert(streamLayout.begin(), streamLayout.end());
	}
	unsigned m_width;
	unsigned m_height;
	unsigned m_bitrate;
	unsigned m_fps;
	std::map<unsigned, VideoSdk::Layout> m_streamLayout;
};

struct ETSetModifyConfigList : public ETVsdkEvent
{
	enum { evId = 315};

	typedef struct Config {
		int maxfps;
		int minfps;
		int maxrate;
		int minrate;
		int currate;
		int width;
		int height;
		int encodeid;
		char encodeparam[1024];
		unsigned char reserved[32];

		Config() {
			memset(this, 0, sizeof(*this));
		}
	} Config;

	ETSetModifyConfigList(unsigned _delay, std::vector<ETSetModifyConfigList::Config> &confList)
		: configList(confList)
		, delay(_delay)
		, trans_alarmrate(0)
		, trans_minrate(0)
	{
		m_eventId = evId;
	}

	unsigned delay;
	unsigned trans_alarmrate; //�����澯����
	unsigned trans_minrate;
	std::vector<ETSetModifyConfigList::Config> &configList;
};

struct ETSetAudioForwardConfig : public ETVsdkInvoke	//���ô���Ƶ��Ƶ��ת��
{
	//ע�����������˴���Ƶ��Ƶ��ת����ͬһpublish����Ƶת������ֹͣ��������ʹ������Ƶת�������ٴε�������Ƶת���ӿ�
	enum { evId = 316};

	ETSetAudioForwardConfig(unsigned publishId, unsigned appid, unsigned targetSid, unsigned targetTopSid)
		: m_publishId(publishId)
		, m_appid(appid)
		, m_targetSid(targetSid)
		, m_targetTopSid(targetTopSid)
	{
		m_eventId = evId;
	}

	unsigned m_publishId;
	unsigned m_appid;
	unsigned m_targetSid;		//Ŀ��Ƶ������Ƶ����
	unsigned m_targetTopSid;	//Ŀ��Ƶ���Ķ���Ƶ����
};

struct ETDeleteAudioForwardConfig : public ETVsdkInvoke
{
	enum { evId = 317};

	ETDeleteAudioForwardConfig(unsigned targetSid)
		: m_targetSid(targetSid)
	{
		m_eventId = evId;
	}

	unsigned m_targetSid;		//Ŀ��Ƶ������Ƶ����
};

//������Ƶ���ݴ���ģʽ
struct ETStartAudioTransmit : public ETVsdkInvoke
{
	enum { evId = 318};

	ETStartAudioTransmit()
	{
		m_eventId = evId;
	}
};

//������Ƶ���ݴ���ģʽ
struct ETStopAudioTransmit : public ETVsdkInvoke
{
	enum { evId = 319};

	ETStopAudioTransmit()
	{
		m_eventId = evId;
	}
};

//������ƵaudioSpecificConfig, ����ǰ�ȵ���ETStartAudioTransmit������Ƶ���书��
//[param] audioSpecificConfig: audioSpecificConfig�ṹ
//[param] len: audioSpecificConfig ����
struct ETSetAudioSpecificConfig : public ETVsdkInvoke
{
	enum { evId = 320};

	ETSetAudioSpecificConfig(const unsigned char* audioSpecificConfig, unsigned len)
		: m_audioSpecificConfig(audioSpecificConfig)
		, m_len(len)
	{
		m_eventId = evId;
	}

	const unsigned char* m_audioSpecificConfig;
	unsigned m_len;
};

//�����Ƶ֡����
//[param] audioData: ��Ƶ��������
//[param] len: ��Ƶ�������ݳ���
//[param] dts: ��Ƶdts
struct ETSetAudioData : public ETVsdkInvoke
{
	enum { evId = 321};

	ETSetAudioData(const unsigned char* audioData, unsigned len, unsigned dts)
		: m_audioData(audioData)
		, m_len(len)
		, m_dts(dts)
	{
		m_eventId = evId;
	}

	const unsigned char* m_audioData;
	unsigned m_len;
	unsigned m_dts;
};


//����ҵ����Ϣ��ֱ��ϵͳ��
struct ETLiveSetAppInfo : public ETVsdkInvoke
{
	enum { evId = 330};
	ETLiveSetAppInfo(unsigned appid, unsigned sceneId)
		:  m_appid(appid)
		, m_sceneId(sceneId)
	{
		m_eventId = evId;
	}

	unsigned m_appid;		//ҵ��ID,ÿ��ҵ���µ�streamNamesҪ��֤Ψһ
	unsigned m_sceneId;		//����id������ҵ���µ�ϸ�֣�������ҵ���Զ��壬����ϵͳ����ͳ��
};

//����ֱ��ϵͳ���ĵ�����Ϣ
//appIdToAnchorStreams:key:appid;value:������ [��Ϊ�������ĵ�������]
//appIdToAnchorGroups:key:appid;value:Ⱥ������ [��Ϊ�������ĵ�Ⱥ������]
//appIdToAudienceStreams:key:appid;value:������ [��Ϊ���ڶ��ĵ�������]
//appIdToAudienceGroups:key:appid;value:Ⱥ������ [��Ϊ���ڶ��ĵ�Ⱥ������]
struct ETLiveSetSubscribeInfo : public ETVsdkInvoke
{
	enum { evId = 331};
	ETLiveSetSubscribeInfo()
	{
		m_eventId = evId;
	}

	ETLiveSetSubscribeInfo(std::map<unsigned, std::set<LiveStreamInfo> >& appIdToAnchorStreams,
		std::map<unsigned, std::set<VideoSdk::LiveLoginGroupInfo> > appIdToAnchorGroups,
		std::map<unsigned, std::set<VideoSdk::LiveStreamInfo> > appIdToAudienceStreams,
		std::map<unsigned, std::set<VideoSdk::LiveLoginGroupInfo> > appIdToAudienceGroups)
	{
		m_eventId = evId;
		m_appIdToAnchorStreams.insert(appIdToAnchorStreams.begin(), appIdToAnchorStreams.end());
		m_appIdToAnchorGroups.insert(appIdToAnchorGroups.begin(), appIdToAnchorGroups.end());
		m_appIdToAudienceStreams.insert(appIdToAudienceStreams.begin(), appIdToAudienceStreams.end());
		m_appIdToAudienceGroups.insert(appIdToAudienceGroups.begin(), appIdToAudienceGroups.end());
	}

	std::map<unsigned, std::set<VideoSdk::LiveStreamInfo> > m_appIdToAnchorStreams;
	std::map<unsigned, std::set<VideoSdk::LiveLoginGroupInfo> > m_appIdToAnchorGroups;
	std::map<unsigned, std::set<VideoSdk::LiveStreamInfo> > m_appIdToAudienceStreams;
	std::map<unsigned, std::set<VideoSdk::LiveLoginGroupInfo> > m_appIdToAudienceGroups;
};

//�����Ƿ�ǿ��Ϊ���� �ýӿ��ѷ�������ʹ��ETLiveSwitchAnchorSystem����ӿ�
struct ETLiveForceToPublisher : public ETVsdkInvoke
{
	enum { evId = 332};
	ETLiveForceToPublisher(bool isForce)
		: m_isForce(isForce)
	{
		m_eventId = evId;
	}

	bool m_isForce;	//true:ǿ�ƽ�����ϵͳ��false:��VSDKȷ���Ƿ������ϵͳ(Ĭ��)
};

//��Ƶ���� (�滻ETStartPublishVideo)
struct ETLiveStartPublishVideo : public ETVsdkInvoke
{
	enum { evId = 333};
	ETLiveStartPublishVideo(std::string& publishStreamName, bool isPublishAsStreamName,
		std::set<std::string>& groupNames, std::map<unsigned, unsigned>& configMap)
		: m_publishStreamName(publishStreamName)
		, m_isPublishAsStreamName(isPublishAsStreamName)
	{
		m_eventId = evId;
		m_groupNames.insert(groupNames.begin(), groupNames.end());
		m_configMap.insert(configMap.begin(), configMap.end());
	}

	std::string m_publishStreamName; 				//����������
	bool m_isPublishAsStreamName;		   			//�Ƿ���Ҫ���͵Ķ�Ӧ����������
	std::set<std::string> m_groupNames; 			//����������Ⱥ����
	std::map<unsigned, unsigned> m_configMap;		//�����������ò���(key:��enum LiveStreamCfgKey)
};

//������Ƶ���� ��ֱ��ϵͳ��
struct ETLiveStartPublishAudio : public ETVsdkInvoke
{
	enum { evId = 334};
	ETLiveStartPublishAudio(std::string& publishStreamName, bool isPublishAsStreamName,
		std::set<std::string>& groupNames, std::map<unsigned, unsigned> configMap)
		: m_publishStreamName(publishStreamName)
		, m_isPublishAsStreamName(isPublishAsStreamName)
	{
		m_eventId = evId;
		m_groupNames.insert(groupNames.begin(), groupNames.end());
		m_configMap.insert(configMap.begin(), configMap.end());
	}

	std::string m_publishStreamName; 			//����������
	bool m_isPublishAsStreamName;			 	//�Ƿ���Ҫ���͵Ķ�Ӧ����������
	std::set<std::string> m_groupNames; 		//����������Ⱥ����
	std::map<unsigned, unsigned> m_configMap;	//�����������ò���(key:��enum LiveStreamCfgKey)
};

//���÷��������Ƶ���ز���
struct ETLiveSetPublishConfig : public ETVsdkInvoke
{
	enum { evId = 335};
	ETLiveSetPublishConfig(std::string& publishStreamName, std::map<unsigned, unsigned>& configMap)
		: m_publishStreamName(publishStreamName)
	{
		m_eventId = evId;
		m_configMap.insert(configMap.begin(), configMap.end());
	}

	std::string m_publishStreamName;			//����������
	std::map<unsigned, unsigned> m_configMap;	//�����������ò���(key:��enum LiveStreamCfgKey)
};

//ͣ��(������Ƶ����Ƶ)
struct ETLiveStopPublish : public ETVsdkInvoke
{
	enum { evId = 336};
	ETLiveStopPublish(std::string& publishStreamName)
		: m_publishStreamName(publishStreamName)
	{
		m_eventId = evId;
	}

	std::string m_publishStreamName; 		//����������
};

//�л�ֱ��ϵͳ
struct ETSwitchToLiveSystem : public ETVsdkInvoke
{
	enum { evId = 337};
	ETSwitchToLiveSystem(bool bLiveSystem)
		: m_bLiveSystem(bLiveSystem)
	{
		m_eventId = evId;
	}

	bool m_bLiveSystem;		//true:�л�ֱ��ϵͳ; false:�л��ؾ�ϵͳ
};

//��ȡvideoSdk��transSdk�汾��
struct ETLiveGetSdkVersion : public ETVsdkInvoke
{
	enum { evId = 338};
	ETLiveGetSdkVersion(unsigned& vsdkVersion, unsigned& transVersion)
		: m_vsdkVersion(vsdkVersion)
		, m_transVersion(transVersion)
	{
		m_eventId = evId;
	}

	unsigned& m_vsdkVersion;
	unsigned& m_transVersion;
};

//��ѯ����·
struct ETLiveGetStreamLineInfo : public ETVsdkInvoke
{
	enum { evId = 339};
	ETLiveGetStreamLineInfo(std::map<unsigned, std::set<VideoSdk::LiveStreamInfo>>& appIdToStreams)
	{
		m_eventId = evId;
		m_appIdToStreams.insert(appIdToStreams.begin(), appIdToStreams.end());
	}
	std::map<unsigned, std::set<VideoSdk::LiveStreamInfo>> m_appIdToStreams;
};

struct ETSetYUVData : public ETVsdkInvoke
{
	enum { evId = 340};
	ETSetYUVData()
	{
		m_eventId = evId;
		publishId = 0;
		width = 0;
		height = 0;
	}

	unsigned publishId;
	int width;
	int height;
	const unsigned char *yuv[3];
	int linesize[3];
	unsigned timestamp;
	bool hasTimestamp;
};

//��ֱ��ϵͳ���л�Ϊ����ϵͳ/����ϵͳ������ʹ��˵�����£�
// 1.ϵͳ��ʼ��Ĭ��Ϊ����ϵͳ���Ӿ�ϵͳ�л�����ϵͳ��ʱ�򣬻�������Ϊ����ϵͳ
// 2.���ÿ����ӿ�ʱ������ǹ���ϵͳ�����ش��󣬾ܾ�������
//	(�ڽӿڵ���ʱ���Ͼ�Ҫ�������ó�����ϵͳ���ٵ��ÿ���)
// 3.�ڿ���������£��л�Ϊ����ϵͳ����ᵼ��ֹͣ���������ص�ͣ��֪ͨ��ģ��
struct ETLiveSwitchAnchorSystem : public ETVsdkInvoke
{
	enum { evId = 341};
	ETLiveSwitchAnchorSystem(bool isAnchorSystem)
		: m_isAnchorSystem(isAnchorSystem)
	{
		m_eventId = evId;
	}

	bool m_isAnchorSystem;		//true:������ϵͳ��false:������ϵͳ
};

//�״ζ���ǰ����avp��ַ���������ٳ���Ƶ����
struct ETLiveSetQuickShowVideoAvpAddr : public ETVsdkInvoke
{
	enum { evId = 342};

	ETLiveSetQuickShowVideoAvpAddr(std::string data, unsigned getAvpCostTime)
	{
		m_eventId = evId;
		m_data.assign(data);
		m_getAvpCostTime = getAvpCostTime;
	}

	std::string m_data;
	unsigned m_getAvpCostTime;
};

struct ETSetSrcData :  public ETVsdkEvent
{
	enum { evId = 343};

	enum {
		DATA_TYPE_I420,
		DATA_TYPE_NV12,
		DATA_TYPE_BGR24,
		DATA_TYPE_BGRA,
		DATA_TYPE_D3D9TEXTURE,
		DATA_TYPE_D3D11TEXTURE,
	};
	ETSetSrcData()
	{
		memset(this, 0, sizeof(*this));
		m_eventId = evId;
	}

	unsigned publishId;
	int dataType;
	int width;
	int height;
	unsigned timestamp;

	union {
		struct {
			const unsigned char *yuv[3];
			int linesize[3];
		} sysmem; //ϵͳ�ڴ�

		struct {
			HANDLE handle; //��֧��NV12
		} gpumem; //�Դ�

		char placeholder[128];
	};

	char reserve[64];
};

struct ETSetSwitchMap : public ETVsdkInvoke
{
	enum { evId = 344};
	ETSetSwitchMap(unsigned pubId, std::map<std::string, std::string>& switchMap)
		: m_publishId(pubId)
	{
		m_eventId = evId;
		m_switchMap.clear();
		m_switchMap.insert(switchMap.begin(), switchMap.end());
	}

	unsigned m_publishId;
	std::map<std::string, std::string> m_switchMap;
};

struct ETEnableCheckEncoderOutput : public ETVsdkInvoke
{
	enum { evId = 345};

	ETEnableCheckEncoderOutput()
		: publishId(0)
	{
		m_eventId = evId;
		checkInterval = 1000;
		notifyInterval = 5000;
		memset(reserve, 0, sizeof(reserve));
	}

	unsigned publishId;
	unsigned checkInterval; //���������λ���룬����С��500���룬Ϊ�˱���ż�����ٵ����󱨣�����ʹ��1000�����������ֵ
	unsigned notifyInterval; //������ʾ�ļ������λ���룬Ĭ��5�룬�������Ϊ0��ʾ����Ҫ���
	char reserve[64];
};

//����ϵͳ��,ҵ�����Դ����streamName��sdk��
struct ETLiveNotifySrcStreamName : public ETVsdkInvoke
{
	enum {evId = 346};
	ETLiveNotifySrcStreamName(unsigned appid,
		std::map<std::string, std::map<unsigned, std::string>>& streamConf)
		: m_appid(appid)
	{
		m_eventId = evId;
		m_streamConf.clear();
		m_streamConf.insert(streamConf.begin(), streamConf.end());
	}
	unsigned m_appid;
	std::map<std::string, std::map<unsigned, std::string>/*enum LiveSrcStreamShowdConfig*/> m_streamConf;
};

}