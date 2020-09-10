#pragma once
#include "VideoEnumDef.h"
#include <vector>

 namespace VideoSdk
 {

 enum VsdkCallBackId
 {
	ET_PUBLISH_STATUS							= 10000,
	ET_CAMERA_STATUS							= 10001,
	ET_CAMERA_AVAILABLE							= 10002,
	ET_PLAY										= 10003,
	ET_STREAM_STATUS							= 10004,
	ET_SUBSCRIBE_STATUS							= 10005,
	ET_RECORD_STATUS							= 10006,
	ET_RECORD_PROGRESS							= 10007,
	ET_MY_TOP_QUEUE								= 10008,
	ET_SEND_VIDEO_APP_DATA						= 10009,
	ET_GET_VIDEO_UIDS_FROM_YY					= 10010,
	
	ET_RECV_MULTI_CODE_RATE_LEVELS				= 10011,
	ET_CPU_OVER_LOAD_WARNING					= 10012,
	ET_LINK_LOSS_WARNING						= 10013,
	ET_TRANSCODE_STATUS_CHANGE					= 10014,
	ET_TRANSCODE_STATUS							= 10015,
	ET_BITMAP_RESOURCE_RELEASE					= 10016,
	ET_NEW_AVC_DECODER_CONFIGURATION_RECORD		= 10017,
	ET_FRAME_DATA								= 10018,
	ET_YY_CONNECTED								= 10019,
	ET_RECV_MULTI_CODERATE_RESULT				= 10020,
	ET_VIDEO_LINK_STATUS						= 10022,
	ET_VIDEO_META_DATA							= 10023,
	ET_PLUGIN_RTMP_PUBLISH_RESULT				= 10024,
	ET_USER_SERVER_INFO							= 10025,
	ET_VIDEO_CODE_RATE_LEVEL_SUGGEST			= 10026,
	ET_VIDEO_DECODE_SLOW						= 10027,
	ET_SERVER_RECORD_RES						= 10028,
	ET_ENCODED_VIDEO_DATA						= 10029,
	ET_ENCODED_AUDIO_DATA						= 10030,
	ET_ENCODER_ERROR                            = 10031,
	ET_CYCLE_STATISTICS_INFO					= 10032,
	ET_DETECT_BANDWIDTH_RESULT					= 10033,
	ET_AUDIENCE_STREAM_CONFIG					= 10034,	//����������֪ͨ
	ET_SID_CHANGE_NOTIFY						= 10035,	//SID�仯֪ͨ
	ET_FORWARD_STATUS							= 10036,	//ת��״̬֪ͨ
	ET_PLAY_EX									= 10037,
	ET_HARDWARE_DECODER_ABILITY					= 10038,
	ET_CAMERA_DATA								= 10039,
	ET_AUDIOT_TRANSMIT_STATUS					= 10040,
	ET_NOTIFY_PUBLISHER_LOSS					= 10041,

	//ֱ��ϵͳ
	ET_LIVE_AUDIENCE_SPEAKING					= 10050,
	ET_LIVE_SUBSCRIBE_STATUS					= 10051,
	ET_LIVE_PUBLISH_STATUS						= 10052,
	ET_LIVE_BRAODCAST_AUDIO						= 10053,
	ET_LIVE_NO_AUDIO_COLLECT					= 10054,
	ET_LIVE_STREAM_LINES						= 10055,
	ET_LIVE_MIX_UID_TO_STREAM					= 10056,
	ET_LIVE_IGNORE_VOICE						= 10057,

	ET_FLV_OVER_HTTP_STATUS						= 10100,
	ET_FLV_PROXY_ADDR							= 10101,
	ET_FLV_QUALITY								= 10102,
	ET_FLV_SYNC_STAMP							= 10103,

	ET_RTMP_PUBLISH_STATUS						= 10150,
	ET_RTMP_STATICS_CALLBACK					= 10151,

	ET_CHECK_ENCODER_OUTPUT_CALLBACK			= 10200,
 };


//////////////////////////////////////////////////////////////////////////
//VSDK => app�ص�
struct ETPublishStatus : public ETVsdkEvent
{
	ETPublishStatus()
	: m_publishId(0)
	, m_status(PUBLISH_SUCCESS)
	{
		m_eventId = ET_PUBLISH_STATUS;
	}

	unsigned m_publishId;
	PublishStatus m_status;
};

struct ETCameraStatus : public ETVsdkEvent
{
	ETCameraStatus()
	: m_publishId(0)
	, m_status(CAMERA_READY)
	{
		m_eventId = ET_CAMERA_STATUS;
	};

	unsigned m_publishId;
	CameraStatus m_status;
};

struct ETCameraAvaliable : public ETVsdkEvent
{
	ETCameraAvaliable()
	: m_publishId(0)
	, m_index(0)
	{
		m_eventId = ET_CAMERA_AVAILABLE;
	};

	unsigned m_publishId;
	unsigned m_index;
};

struct ETPlay : public ETVsdkEvent
{
	ETPlay()
	: m_publishId(0)
	, m_uid(0)
	, m_length(0)
	, m_data(NULL)
	, m_bitmapHeader(NULL)
	{
		m_eventId = ET_PLAY;
	};

	unsigned m_publishId;
	unsigned m_uid;
	unsigned m_length;
	const unsigned char* m_data;
	const BITMAPINFOHEADER* m_bitmapHeader;
};

struct ETPlayEx : public ETVsdkEvent
{
	ETPlayEx()
		: m_publishId(0)
	{
		m_eventId = ET_PLAY_EX;
	};

	unsigned m_publishId;
	VideoRenderInfo m_renderInfo;
};

struct ETHardwareDecoderAbility : public ETVsdkEvent
{
	ETHardwareDecoderAbility()
		: m_ability(0)
	{
		m_eventId = ET_HARDWARE_DECODER_ABILITY;
	};

	unsigned m_ability;
};

struct ETStreamStatus : public ETVsdkEvent
{
	ETStreamStatus()
	: m_uid(0)
	, m_publishId(0)
	, m_status(START_STREAM)
	{
		m_eventId = ET_STREAM_STATUS;
	};

	unsigned m_uid;
	unsigned m_publishId;
	StreamStatus m_status;
};

struct ETSubcribeStatus : public ETVsdkEvent
{
	ETSubcribeStatus()
	: m_uid(0)
	, m_publishId(0)
	, m_status(SUBCRIBE_SUCCESS)
	{
		m_eventId = ET_SUBSCRIBE_STATUS;
	};

	unsigned m_uid;
	unsigned m_publishId;
	SubcribeStatus m_status;
};

struct ETRecordStatus : public ETVsdkEvent
{
	ETRecordStatus()
	: m_status(0)
	{
		m_eventId = ET_RECORD_STATUS;
	};

	unsigned m_status;
};

struct ETRecordProgress : public ETVsdkEvent
{
	ETRecordProgress()
	: m_sec(0)
	, m_bytes(0)
	{
		m_eventId = ET_RECORD_PROGRESS;
	};

	unsigned int m_sec;
	__int64 m_bytes;
};

struct ETMyTopQueue : public ETVsdkEvent
{
	ETMyTopQueue()
	: m_isTop(false)
	{
		m_eventId = ET_MY_TOP_QUEUE;
	};

	bool m_isTop;
};

struct ETSendVideoAppData : public ETVsdkEvent
{
	ETSendVideoAppData()
	: m_data(NULL)
	, m_len(0)
	{
		m_eventId = ET_SEND_VIDEO_APP_DATA;
	};

	const char* m_data;
	unsigned m_len;
};

struct ETGetVideoUidsFromYY : public ETVsdkEvent
{
	ETGetVideoUidsFromYY(std::vector<unsigned>& vec)
		: m_uids(vec)
	{
		m_eventId = ET_GET_VIDEO_UIDS_FROM_YY;
	};

	std::vector<unsigned>& m_uids;
};

struct ETRecvMultiCodeRateLevels : public ETVsdkEvent
{
	ETRecvMultiCodeRateLevels(std::vector<unsigned>& vec)
		: m_multiCodeRateLevelsVec(vec)
	{
		m_eventId = ET_RECV_MULTI_CODE_RATE_LEVELS;
	};

	std::vector<unsigned>& m_multiCodeRateLevelsVec;
};

struct ETCPUOverLoadWarning : public ETVsdkEvent
{
	ETCPUOverLoadWarning()
	: m_isWarning(false)
	, m_avgCodeTime(0)
	, m_publishId(0)
	{
		m_eventId = ET_CPU_OVER_LOAD_WARNING;
	};

	bool m_isWarning;
	unsigned m_avgCodeTime;
	unsigned m_publishId;
};

struct ETLinkLossWarning : public ETVsdkEvent
{
	ETLinkLossWarning()
	: m_isWarning(false)
	, m_lossRate(0)
	, m_publishId(0)
	{
		m_eventId = ET_LINK_LOSS_WARNING;
	};

	bool m_isWarning;
	unsigned m_lossRate;
	unsigned m_publishId;
};

struct ETNotifyPublisherLoss : public ETVsdkEvent
{
	ETNotifyPublisherLoss()
	: m_lossResult(0)
	, m_publishId(0)
	, m_uid(0)
	{
		m_eventId = ET_NOTIFY_PUBLISHER_LOSS;
	}
	unsigned m_lossResult;	//1������ 0������
	unsigned m_publishId;
	unsigned m_uid;
};

//IH264RawDataTranscodeStatusReceiver
struct ETTranscodeStatusChange : public ETVsdkEvent
{
	ETTranscodeStatusChange()
	: m_publishId(0)
	, m_isTranscoding(false)
	{
		m_eventId = ET_TRANSCODE_STATUS_CHANGE;
	};

	unsigned m_publishId;
	bool m_isTranscoding;
};

//IVideoStreamInputTypeStatusReceiver
struct ETTranscodeStatus : public ETVsdkEvent
{
	ETTranscodeStatus()
	: m_uid(0)
	, m_publishId(0)
	, m_inputTypeStatus(0)
	{
		m_eventId = ET_TRANSCODE_STATUS;
	};

	unsigned m_uid;
	unsigned m_publishId;
	unsigned m_inputTypeStatus;
};

//IBitmapSourceReleaseReceiver
struct ETBitmapReSourceRelease : public ETVsdkEvent
{
	ETBitmapReSourceRelease()
	: m_rcHandle(0)
	, m_publishId(0)
	{
		m_eventId = ET_BITMAP_RESOURCE_RELEASE;
	};

	unsigned m_rcHandle;
	unsigned m_publishId;
};

//IRawVideoStreamDataReceiver
struct ETNewAVCDecoderConfigurationRecord : public ETVsdkEvent
{
	ETNewAVCDecoderConfigurationRecord()
	: m_uid(0)
	, m_publishId(0)
	, m_pAVCDecoderConfigurationRecord(NULL)
	, m_len(0)
	{
		m_eventId = ET_NEW_AVC_DECODER_CONFIGURATION_RECORD;
	};

	unsigned m_uid;
	unsigned m_publishId;
	const unsigned char* m_pAVCDecoderConfigurationRecord;
	unsigned m_len;
};

//IRawVideoStreamDataReceiver
struct ETFrameData : public ETVsdkEvent
{
	ETFrameData()
	: m_uid(0)
	, m_publishId(0)
	, m_pFramedata(NULL)
	, m_len(0)
	, m_pts(0)
	, m_dts(0)
	, m_isKeyFrame(false)
	{
		m_eventId = ET_FRAME_DATA;
	};

	unsigned m_uid;
	unsigned m_publishId;
	const unsigned char* m_pFramedata;
	unsigned m_len;
	unsigned m_pts;
	unsigned m_dts;
	bool m_isKeyFrame;
};

struct ETonYYConnected : public ETVsdkEvent
{
	ETonYYConnected()
	{
		m_eventId = ET_YY_CONNECTED;
	};
};

struct ETonMultiCodeRateResult : public ETVsdkEvent
{
	ETonMultiCodeRateResult()
		: m_bSuccess(false)
		, m_realCodeRateLevel((unsigned) -1)
	{
		m_eventId = ET_RECV_MULTI_CODERATE_RESULT;
	};

	bool m_bSuccess;
	unsigned m_realCodeRateLevel;
};

struct ETVideoLinkStatus: public ETVsdkEvent
{
	ETVideoLinkStatus(LinkStatus status, std::string& ip, unsigned short port)
		: m_status(status)
		, m_ip(ip)
		, m_port(port)
	{
		m_eventId = ET_VIDEO_LINK_STATUS;
	}
	LinkStatus m_status;
	std::string m_ip;
	unsigned short m_port;
};

//m_metaDatas
//	first: publishUid << 32 | publishId
//	second: metaDatas
struct ETVideoMetaData: public ETVsdkEvent
{
	ETVideoMetaData()
	{
		m_eventId = ET_VIDEO_META_DATA;
	}
	std::map<unsigned long long, std::map<unsigned char, unsigned> > m_metaDatas;
};

struct ETonFlvOverHttpStatus : public ETVsdkEvent
{
	ETonFlvOverHttpStatus(unsigned uid, unsigned publishId, unsigned status)
		: m_uid(uid)
		, m_publishId(publishId)
		, m_status(status)
	{
		m_eventId = ET_FLV_OVER_HTTP_STATUS;
	}

	unsigned m_uid;
	unsigned m_publishId;
	unsigned m_status;
};

struct ETonFlvProxyAddr: public ETVsdkEvent
{
	ETonFlvProxyAddr(unsigned uid, unsigned publishId, std::string& ip, unsigned short port)
		: m_uid(uid)
		, m_publishId(publishId)
		, m_ip(ip)
		, m_port(port)
	{
		m_eventId = ET_FLV_PROXY_ADDR;
	}

	unsigned m_uid;
	unsigned m_publishId;
	std::string m_ip;
	unsigned short m_port;
};

struct ETonFlvQuality : public ETVsdkEvent
{
	ETonFlvQuality(unsigned kaTimes, unsigned lossTimes, unsigned lossCount, unsigned linkLossTimes, unsigned lossReason)
		: m_kaTimes(kaTimes)
		, m_lossTimes(lossTimes)
		, m_lossCount(lossCount)
		, m_linkLossTimes(linkLossTimes)
		, m_lossReason(lossReason)
	{
		m_eventId = ET_FLV_QUALITY;
	}

	unsigned m_kaTimes;			//�йٿ��Ĵ���
	unsigned m_lossTimes;		//��֡����
	unsigned m_lossCount;		//��֡������
	unsigned m_linkLossTimes;	//�����Ĵ���
	unsigned m_lossReason;		//�йٿ�ԭ��
};

struct ETonFlvSyncStamp : public ETVsdkEvent
{
	ETonFlvSyncStamp(unsigned uid, unsigned publishId, unsigned frameId, unsigned syncStamp)
		: m_uid(uid)
		, m_publishId(publishId)
		, m_frameId(frameId)
		, m_syncStamp(syncStamp)
	{
		m_eventId = ET_FLV_SYNC_STAMP;
	}

	unsigned m_uid;
	unsigned m_publishId;
	unsigned m_frameId;
	unsigned m_syncStamp;
};

struct ETonPluginRTmpPulish : public ETVsdkEvent
{
	ETonPluginRTmpPulish(unsigned uid, unsigned appid, unsigned topSid, unsigned subSid, unsigned status, std::map<std::string, unsigned>& resCode)
		: m_uid(uid)
		, m_appid(appid)
		, m_topSid(topSid)
		, m_subSid(subSid)
		, m_status(status)
		, m_resCode(resCode)
	{
		m_eventId = ET_PLUGIN_RTMP_PUBLISH_RESULT;
	}

	unsigned m_uid; 
	unsigned m_appid; 
	unsigned m_topSid;
	unsigned m_subSid;
	unsigned m_status; //0 - OK; ��0 -���������쳣
	std::map<std::string, unsigned> m_resCode; //keyΪCDN��ַ��valueΪ����������������ؽ����0-�ɹ�����0-�����쳣������
};

struct ETUserServerInfo : public ETVsdkEvent
{
	ETUserServerInfo(std::string& ip, unsigned serverAreaType, unsigned myAreaType)
		: m_ip(ip)
		, m_serverAreaType(serverAreaType)
		, m_myAreaType(myAreaType)
	{
		m_eventId = ET_USER_SERVER_INFO;
	}

	std::string m_ip;
	unsigned m_serverAreaType;
	unsigned m_myAreaType;
};

struct ETCodeRateLevelSuggest : public ETVsdkEvent
{
	ETCodeRateLevelSuggest(unsigned appid, unsigned recvNum, unsigned recvRange, unsigned rtt, unsigned result)
		: m_appId(appid)
		, m_recvNum(recvNum)
		, m_recvRange(recvRange)
		, m_rtt(rtt)
		, m_result(result)
	{
		m_eventId = ET_VIDEO_CODE_RATE_LEVEL_SUGGEST;
	}

	unsigned m_appId;
	unsigned m_recvNum;
	unsigned m_recvRange;
	unsigned m_rtt;
	unsigned m_result;	// 0 �������ʣ�1������
};

struct ETVideoDecodeSlow : public ETVsdkEvent
{
	ETVideoDecodeSlow(unsigned uid, unsigned publishId, unsigned bitRate, unsigned frameRate, unsigned decodeFrameRate, unsigned width, unsigned height)
		: m_uid(uid)
		, m_publishId(publishId)
		, m_bitRate(bitRate)
		, m_frameRate(frameRate)
		, m_decodeFrameRate(decodeFrameRate)
		, m_width(width)
		, m_height(height)
	{
		m_eventId = ET_VIDEO_DECODE_SLOW;
	}

	unsigned m_uid;
	unsigned m_publishId;
	unsigned m_bitRate;
	unsigned m_frameRate;
	unsigned m_decodeFrameRate;
	unsigned m_width;
	unsigned m_height;
};

//֪ͨ�ϲ�rtmp�������
struct ETonRtmpPublishStatus
	: public ETVsdkEvent
{
	ETonRtmpPublishStatus(unsigned status)
		: m_status(status)
	{
		m_eventId = ET_RTMP_PUBLISH_STATUS;
	}

	unsigned m_status; //ȡֵ��enum RtmpPublishStatus
};

//֪ͨ�ϲ�rtmp���ͳ��,ÿ��ص�һ��
struct ETonRtmpStaticsCallback
	: public ETVsdkEvent
{
	ETonRtmpStaticsCallback(unsigned sendBytes, unsigned discardFrameNum, unsigned totalFrameNum,
		unsigned congestTimes, unsigned congestInterval)
		: m_sendBytes(sendBytes)
		, m_discardFrameNum(discardFrameNum)
		, m_totalFrameNum(totalFrameNum)
		, m_congestTimes(congestTimes)
		, m_congestInterval(congestInterval)
	{
		m_eventId = ET_RTMP_STATICS_CALLBACK;
	}

	unsigned m_sendBytes;//1s�ڷ�������������λ�ֽ�
	unsigned m_discardFrameNum;//1s�ڶ�֡��
	unsigned m_totalFrameNum;//1s����֡��,������Ƶ֡����Ƶ֡
	unsigned m_congestTimes;//1s��ӵ������
	unsigned m_congestInterval;//1s��ӵ����ʱ��,��λms
};

struct ETStartServerRecordRes : public ETVsdkEvent
{
	ETStartServerRecordRes(unsigned result, RecordMode mode, const std::string& recordId, const std::string& userMark)
		: m_result(result)
		, m_mode(mode)
		, m_recordId(recordId)
		, m_userMark(userMark)
	{
		m_eventId = ET_SERVER_RECORD_RES;
	}

	unsigned m_result;
	RecordMode m_mode;
	std::string m_recordId;
	std::string m_userMark;
};

//����ʹ��
struct ETEncodedVideoData : public ETVsdkEvent
{
	ETEncodedVideoData(unsigned uid, unsigned publishId, unsigned frameId, unsigned capStamp, bool isKeyFrame, std::string& videoData)
		: m_uid(uid)
		, m_publishId(publishId)
		, m_frameId(frameId)
		, m_capStamp(capStamp)
		, m_isKeyFrame(isKeyFrame) 
		, m_videoData(videoData)
	{
		m_eventId = ET_ENCODED_VIDEO_DATA;
	}

	unsigned m_uid; //��Ƶ֡��������
	unsigned m_publishId; //publishId
	unsigned m_frameId; //��Ƶ֡���
	unsigned m_capStamp; //��Ƶ�ɼ�ʱ���
	bool m_isKeyFrame; //�Ƿ���I֡
	std::string m_videoData; //flv��ʽ��Ƶ֡����
};

//����ʹ��
struct ETEncodedAudioData : public ETVsdkEvent
{
	ETEncodedAudioData(unsigned uid, unsigned frameId, unsigned codecType, unsigned ssrc, unsigned capStamp, std::string& audioData)
		: m_uid(uid)
		, m_frameId(frameId)
		, m_codecType(codecType)
		, m_ssrc(ssrc)
		, m_capStamp(capStamp)
		, m_audioData(audioData)
	{
		m_eventId = ET_ENCODED_AUDIO_DATA;
	}

	unsigned m_uid; //��Ƶ֡��������
	unsigned m_frameId; //��Ƶ֡���
	unsigned m_codecType; //1:�����ʣ�2�������� 25:128kb/s��������
	unsigned m_ssrc;
	unsigned m_capStamp; //��Ƶ�ɼ�ʱ���
	std::string m_audioData; //��Ƶ����
};

struct ETEncoderError : public ETVsdkEvent
{
	ETEncoderError(unsigned uid)
		: m_publishId(uid)
	{
		m_eventId = ET_ENCODER_ERROR;
	};

	unsigned m_publishId;
};

struct ETCycleStatisticsInfo : public ETVsdkEvent
{
	ETCycleStatisticsInfo()
	{
		m_eventId = ET_CYCLE_STATISTICS_INFO;
	}

	std::map<unsigned, unsigned> m_infoMap;
	std::map<unsigned long long, std::map<unsigned, unsigned> >	m_streamToInfoMap;	// (spkuid<<32|publishid)->(staticsItem->val)
};

struct ETDetectBandWidthResult : public ETVsdkEvent
{
	ETDetectBandWidthResult(unsigned bandwidth, unsigned result)
		: m_bandwidth(bandwidth)
		, m_result(result)
	{
		m_eventId = ET_DETECT_BANDWIDTH_RESULT;
	}

	unsigned m_bandwidth; //��λ��bit/s
	unsigned m_result; //�������enum DetectBandwidthResultEnum
};

struct ETChannelConfig
{
	bool hasVideo;			 //�Ƿ������Ƶ(true:������Ƶ;false:��������Ƶ)
	unsigned isTrancode;	 //�Ƿ�ת��(��:enum StreamSourceType�Ķ���)
	unsigned transCodeRate;	 //���ʡ���λ:bit/s��
	unsigned frameRate;		 //֡��
	unsigned resolution;	 //�ֱ��ʣ�width << 16 | height��
	unsigned cropStream;	 //�Ƿ��Ǻ�������(0:�Ǻ�����;1:���вü��ĺ�����)
	std::map<unsigned,unsigned> transMetaData;	//ת�����Ƶ������Ϣ
};

struct ETSpeakerStreamConfig	//����������Ϣ
{
	unsigned sid;		//��������Ƶ����
	unsigned appid;		//������appid
	std::map<unsigned, ETChannelConfig> channelConfigs;   //channelId=>ETChannelConfig
};

struct ETAudienceStreamConfig : public ETVsdkEvent	//��������֪ͨ
{
	ETAudienceStreamConfig(std::map<unsigned long long , ETSpeakerStreamConfig> &speakerStreamCfgs)
	{
		m_eventId = ET_AUDIENCE_STREAM_CONFIG;
		m_speakerStreamCfgs.insert(speakerStreamCfgs.begin(),speakerStreamCfgs.end());
	}
	std::map<unsigned long long, ETSpeakerStreamConfig> m_speakerStreamCfgs;   //configs(uint64_t[speakerUid+publishId])=>ETSpeakerStreamConfig
};

struct ETSidChangeNotify : public ETVsdkEvent	//sid�仯֪ͨ
{
	ETSidChangeNotify(unsigned newSid,unsigned oldSid)
		: m_newSid(newSid)
		, m_oldSid(oldSid)
	{
		m_eventId = ET_SID_CHANGE_NOTIFY;
	}
	unsigned m_newSid;		//�µ���Ƶ��id
	unsigned m_oldSid;		//ԭ�ȵ���Ƶ��id
};

struct ETForwardStatus : public ETVsdkEvent
{
	ETForwardStatus(unsigned publishId, unsigned targetSid, ForwardStatus status)
		: m_publishId(publishId)
		, m_targetSid(targetSid)
		, m_status(status)
	{
		m_eventId = ET_FORWARD_STATUS;
	}

	unsigned m_publishId;
	unsigned m_targetSid;
	ForwardStatus m_status;
};

struct ETCameraData : public ETVsdkEvent
{
	ETCameraData()
		:m_data( NULL ),
		m_len( 0 ),
		m_width( 0 ),
		m_height( 0 ),
		m_mediaTimeStamp( 0 ),
		m_pushTimeStamp( 0 ),
		m_hasPreprocessed( false ),
		m_pixelFormat(eAV_PIX_FMT_RGB24)
	{
		m_eventId = ET_CAMERA_DATA;
	};

	unsigned char* m_data;
	unsigned m_len;
	int m_width;
	int m_height;
	unsigned m_mediaTimeStamp;
	unsigned m_pushTimeStamp;
	bool m_hasPreprocessed;
	VideoPixelFormat m_pixelFormat;
};

// ҵ����Ƶ����״̬�ص�
struct ETAudioTransmitStatus : public ETVsdkEvent
{
	ETAudioTransmitStatus(unsigned status)
		: m_status(status)
	{
		m_eventId = ET_AUDIOT_TRANSMIT_STATUS;
	};

	unsigned m_status;
};

//����˵���ص����յ��ûص���Ҫ����ETLiveStartPublishVideo����������Ƶ��������
//���ѵ��õ�ETLiveStartPublishVideo���򲻻ص���֪ͨ
struct ETLiveAudienceSpeaking : public ETVsdkEvent
{
	ETLiveAudienceSpeaking()
	{
		m_eventId = ET_LIVE_AUDIENCE_SPEAKING;
	}
};

//(ֱ��ϵͳ)����״̬�ص�
struct ETLiveSubscribeStatus : public ETVsdkEvent
{
	ETLiveSubscribeStatus(unsigned appid, std::string streamName, LiveSubcribeStatus status)
		: m_appid(appid)
		, m_streamName(streamName)
		, m_status(status)
	{
		m_eventId = ET_LIVE_SUBSCRIBE_STATUS;
	}

	unsigned m_appid;
	std::string m_streamName;			//������
	LiveSubcribeStatus m_status;		//����״̬
};

//(ֱ��ϵͳ)����״̬�ص�
struct ETLivePublishStatus : public ETVsdkEvent
{
	ETLivePublishStatus(std::string streamName, LivePublishStatus status)
		: m_streamName(streamName)
		, m_status(status)
	{
		m_eventId = ET_LIVE_PUBLISH_STATUS;
	}

	std::string m_streamName;		//������
	LivePublishStatus m_status;		//����״̬
};

//�յ��˻ص�����Ҫͨ������ ETLiveStartPublishAudio������Ƶ���ڹ㲥��Ƶ��������
//�������ã��򲻻ص�
//���������Ƿ���Ҫ������ǰ���ã�����Ƶ��ʱ�����Խ���Ƶ���������㲥����ʱ
struct ETLiveBroadcastAudio : public ETVsdkEvent
{
	ETLiveBroadcastAudio()
	{
		m_eventId = ET_LIVE_BRAODCAST_AUDIO;
	}
};

//��Ƶ�����󣬳���interval��ʱ�䣬û�вɼ�����Ƶ
struct ETLiveNoAudioCollect : public ETVsdkEvent
{
	ETLiveNoAudioCollect()
	{
		m_eventId = ET_LIVE_NO_AUDIO_COLLECT;
	}

	int interval;
};

//�ص���ģ����·��Ϣ
struct ETLiveStreamLines: public ETVsdkEvent
{
	ETLiveStreamLines(unsigned lineSeq,
		std::map<unsigned, std::set<LiveStreamLineInfo>>& appidToStreamLines, unsigned from)
		: m_curLineSeq(lineSeq)
		, m_from(from)
	{
		m_eventId = ET_LIVE_STREAM_LINES;
		m_appidToStreamLines.insert(appidToStreamLines.begin(), appidToStreamLines.end());
	}
	unsigned m_curLineSeq;
	std::map<unsigned, std::set<LiveStreamLineInfo>> m_appidToStreamLines;
	unsigned m_from;
};

//(ֱ��ϵͳ)�ص������ƶ�Ӧ�Ļ���appid��uid
struct ETLiveMixUid2Stream : public ETVsdkEvent
{
	ETLiveMixUid2Stream(unsigned appid, unsigned uid, std::string& streamName)
		: m_appid(appid)
		, m_uid(uid)
	{
		m_eventId = ET_LIVE_MIX_UID_TO_STREAM;
		m_streamName.assign(streamName);
	}
	unsigned m_appid;
	unsigned m_uid;
	std::string m_streamName;
};

//�ص�������Ϣ(m_bIgnore:true:����;false:����)
struct ETLiveIgnoreVoice : public ETVsdkEvent
{
	ETLiveIgnoreVoice(unsigned uid, bool bIgnore) 
		: m_uid(uid)
		, m_bIgnore(bIgnore)
	{
		m_eventId = ET_LIVE_IGNORE_VOICE;
	}

	unsigned m_uid;
	bool m_bIgnore;
};

struct ETCheckEncoderOutputCallback : public ETVsdkEvent
{
	enum Status{
		ENC_STATUS_OK,
		ENC_STATUS_CAMERA_FAILED,
		ENC_STATUS_PREPROCESS_FAILED,
		ENC_STATUS_ENCODER_FAILED,
	};

	ETCheckEncoderOutputCallback(unsigned id, unsigned status) 
		: m_publishId(id)
		, m_status(status)
	{
		m_eventId = ET_CHECK_ENCODER_OUTPUT_CALLBACK;
	}

	unsigned m_publishId;
	unsigned m_status;
};

}
