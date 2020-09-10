#pragma once
#include <map>
#include <set>

namespace VideoSdk
{
enum PublishStatus
{
	PUBLISH_SUCCESS = 0,
	PUBLISH_FAILED = 1,
	PUBLISHING_ERROR = 2,
};

enum CameraStatus
{
	CAMERA_READY = 0,
	CAMERA_NOT_READY = 1,
	CAMERA_NOT_SUPPORT = 2,
	/*CAMERA_BAD_INDEX = 3,
	CAMERA_PLUG_OUT =4,
	CAMERA_FREEZE_AND_RECONNECTED = 5,
	CAMERA_FREEZE_AND_RECONNECT_FAILD = 6,*/
};

enum SubcribeStatus
{
	SUBCRIBE_SUCCESS = 0,
	SUBCRIBE_FAILED = 1,
	SUBCRIBEING_ERROR = 2,
};

enum StreamStatus
{
	START_STREAM = 0,
	STOP_STREAM = 1,
};

enum RenderStatus
{
	RENDER_SUCCESSED = 0,
	RENDER_FAILED = 1,
	RENDER_EXIT = 2,
};

enum RecordStatus
{
	START_RECORD = 0,
	STOP_RECORD = 1,
	ERROR_RECORD = 2,
	ERROR_DISKFULL = 3,
	ERROR_CREATE_FILE = 4,
};

enum RecordMode
{
	RECORD_BY_UID = 0,
	RECORD_BY_SID,
	RECORD_BY_OLD,
};

enum StaticsPropsId
{
	CODE_RATE_SID = 0,
	FRAME_RATE_SID = 1,
	RTT_SID = 2,
	UP_LINK_FLOW_SID = 3,
	UP_LINK_LOSS_RATE_SID = 4,
	AVG_CODE_TIME_SID = 5,
	CODE_TIME_OVERLOAD_RATIO_SID = 6,
	REAL_CODE_RATE_SID = 7,
	REAL_FRAME_RATE_SID = 8,
	PUBLISHER_LOSS_NOTIFY = 9,
};

enum VideoCodeRate
{
	LOW_DEFINITION = 0,
	HIGH_DEFINITION = 1,
};

enum VideoCodeRateEx
{
	CODE_RATE_100KPS =0,
	CODE_RATE_150KPS =1,
	CODE_RATE_200KPS =2,
	CODE_RATE_300KPS =3,
	CODE_RATE_400KPS =4,
	CODE_RATE_500KPS =5,
	CODE_RATE_800KPS =6,
	CODE_RATE_1000KPS=7,
	CODE_RATE_1500KPS=8,
	CODE_RATE_2000KPS=9,
	CODE_RATE_2500KPS=10,
	CODE_RATE_1200KPS=11,
	CODE_RATE_3000KPS=12,
	CODE_RATE_4000KPS=13,
	CODE_RATE_5000KPS=14,
	CODE_RATE_6000KPS=15,
	CODE_RATE_7000KPS=16,
	CODE_RATE_8000KPS=17,
	CODE_RATE_9000KPS=18,
	CODE_RATE_10000KPS=19,
};

enum VideoFrameRate
{
	FRAME_RATE_8FPS = 0,
	FRAME_RATE_10FPS =1,
	FRAME_RATE_12FPS =2,
	FRAME_RATE_15FPS =3,
	FRAME_RATE_24FPS =4,
	FRAME_RATE_1FPS  =5,
	FRAME_RATE_20FPS =6,
	FRAME_RATE_30FPS =7,
	FRAME_RATE_5FPS  =8,
	FRAME_RATE_16FPS =9,
	FRAME_RATE_25FPS =10,
	FRAME_RATE_50FPS =11,
	FRAME_RATE_60FPS =12,
	FRAME_RATE_120FPS =13,
	FRAME_RATE_240FPS =14,
	//��ö���ѱ���������ʹ�� USER_DEFINE_FRAME_RATE (�벼��)
};

enum VideoRatio
{
	RATIO_TRY_640x480 = 0,
	RATIO_TRY_800x600 = 1,
	RATIO_TRY_320x240 = 2,
	RATIO_TRY_240x180 = 3,
	RATIO_TRY_120x90 = 4,
	RATIO_TRY_720x405 = 5,
	RATIO_TRY_480x360 = 6,
	RATIO_TRY_1024x768 = 7,
	RATIO_TRY_1024x576 = 8,
	RATIO_TRY_1280x720 = 9,
	RATIO_TRY_400x300 = 10,
	RATIO_TRY_1440x900 = 11,
	RATIO_TRY_1920x1080 = 12,
	RATIO_TRY_3840x2160 = 13,
	RATIO_TRY_7680x4320 = 14
	//��ö���ѱ���������ʹ�� FORCE_USER_DEFINE_RESOLUTION_RATIO (�벼��)
};

enum EncCodecID
{
	ENC_CODEC_VP8 = 0,
	ENC_CODEC_VP9 = 1,
	ENC_CODEC_H264 = 2,  // libx264 
	ENC_CODEC_H265 = 3,  // libx265
	ENC_CODEC_H264_INTEL_QUICKSYNC = 4, // intel hardware H264 encoder
	ENC_CODEC_H265_INTEL_QUICKSYNC = 5, // intel hardware HEVC encoder
	ENC_CODEC_H264_NVIDIA_NVENC = 6, // nVidia hardware H264 encoder
	ENC_CODEC_H265_NVIDIA_NVENC = 7,  // nVidia hardware HEVC encoder
	ENC_CODEC_QY_H265 = 8,  // QY HEVC encoder
	ENC_CODEC_VRVIU_H264 = 9, //vrviu h264
	ENC_CODEC_UNKNOW = 100,
};

enum DecCodeID
{
	DEC_CODEC_FFMPEG		= 1 << 0,	//֧��H265,H264����
	DEC_CODEC_ITMHEVC_HEVC	= 1 << 1,	//Ŀǰֻ����H265����
	DEC_CODEC_NVIDIA_AVC    = 1 << 2,	//H264 NVIDIAӲ��
	DEC_CODEC_INTEL_AVC     = 1 << 3,	//H264 INTELӲ��
	DEC_CODEC_NVIDIA_HEVC   = 1 << 4,	//H265 NVIDIAӲ��
	DEC_CODEC_INTEL_HEVC    = 1 << 5,	//H265 INTELӲ��
};

enum VideoEncodecType
{
	ENCODE_UNKNOW	= 0,		// δ֪��������
	ENCODE_H264		= 1,		// H264����
	ENCODE_H265		= 2,		// H265����
	ENCODE_VP8		= 3,		// VP8����
	ENCODE_VP9		= 4,		// VP9����
};

enum D3DFMT
{
	R8G8B8               = 20,
	A8R8G8B8             = 21,
	X8R8G8B8             = 22,
	R5G6B5               = 23,
	X1R5G5B5             = 24,
	A1R5G5B5             = 25,
	A4R4G4B4             = 26,
	R3G3B2               = 27,
	A8                   = 28,
	A8R3G3B2             = 29,
	X4R4G4B4             = 30,
	A2B10G10R10          = 31,
	G16R16               = 34,
};

//cur key range[0 - 40]
enum PluginConfig
{
	//pluginConfig
	PLAY_DELAY_PID = 0,							//�����ӳ� ���õ�λΪ����
	DISPLAY_AUDIO_WAVE_PID = 1,					//�ڻ�����ʾ��Ƶ����
	SYNC_ALL_MC_LIST_AUDIO = 10,				//�Ƿ�ͬ������������uid����
	TOTAL_BIT_RATE_PID = 12,					//��ǰ��Ƶ����Ƶ������
	IGNORE_YY_MIC_LIST_MODE = 13,				//�Ƿ����YY��������ȫ�ɲ����������Ȩ��
	PLUGIN_P2P_SWITCH_PID = 14,					//����Ƿ���P2P
	SMOOTH_VIDEO_UPLOAD = 15,					//ƽ���������������������д����ӳ٣�
	PUBLISHER_NUM = 16,							//����ͬʱ��������
	QUALITY_DELAY_MODE_PID = 18,				//����/��ʱģʽѡ�� ����ֵ�ο�enum QualityDelayBalanceMode
	FORMER_ANCHOR_NUM = 20,						//ָ������ǰFORMERANCHORNUMλ�ر�p2p��������������Ƶ��ʱ�ͣ�ͬʱ���ⲿ����Ƶ��ʱ��
	FAST_ACCESS = 21,							//�Ƿ�֧�ֿ��ٽ��룬1Ĭ��Ϊ֧�� 
	FAKE_APPID = 30,							//ָ��appId��ָ�����û�����ʹ�ø�appId��������Ƶ
	PLUGIN_TYPE = 31,							//ָ��������ͣ��ò����������ڲ��ı�appid��ǰ���°Ѳ�ͬ���͵Ĳ��ͳ���������ֿ��������Ϳɼ�enum PluginType
	SYNC_ALL_AUDIO_NOT_PUBLISHER_PID=35,		//�Ƿ���������������Ƶ֮���������Ƶ���͵�vsdk����ͬ����0Ĭ�ϲ�����
	MULTI_CODE_RATE_LEVEL = 36,					//ָ�������ʷ����е����ʵ�λ��Ĭ�ϵ�λΪ0
	OPEN_MULTICODERATE = 37,					//�Ƿ������������ܣ�0Ĭ�ϲ�����,1����
	RGB_OUTPUT_FORMAT  =38,                     //0:bgr(rgb24) 1:bgra(rgb32)
	RGB_OUTPUT_FLIP = 43,                       //�����ͼ�������Ƿ���Ҫ���·�תΪ�ߵ���ͼ��, 0 ����Ҫ��ת��1��Ҫ��ת
	FLV_OVER_HTTP_MODE = 39,					//�Ƿ�ʹ��http����flvģʽ
	YCLOUD_VIDEO_MODE = 41,						//ģʽ
	YCLOUD_IS_1V1_MODE = 42,					//�Ƿ�1v1ģʽ
	MULTI_OPTION_SWITCH = 65,					//����㿪��ѡ�����Ӧ�ò����ã���ӦParaDef.h�е�MULTI_TEST_SWITCH
	USE_RTMP_PUBLISH = 66,						//�Ƿ�ʹ��rtmp��ʽ����flv������Ƶ��0Ĭ�ϲ�����,1����
	DELAY_SEND_TIME = 67,						//�ӳٷ�������Ƶʱ�䣬��λms
	PLUGIN_DECODE_AUDIO_VIDEO = 68,				//�Ƿ��������Ƶ����ǰ���ݸ�ģ����н���
	SUPPORT_FLV_FAST_ACCESS = 69,				//�Ƿ�֧��FLV���ٽ��빦��, 0 ��֧�֣�1֧�֡�Ĭ��֧�֡�
	
	//publishConfig
	FRAME_RATE_PID = 2,							//֡�� ����ֵ�ο�enum VideoFrameRate
	BITRATE_PID = 3,							//���� (HIGH_DEFINITIONģʽ����Ч) ����ֵ�ο�enum VideoCodeRateEx
	RESOLUTION_PID = 4,							//�ֱ��� ����ֵ�ο�enum VideoRatio
	TRY_USING_2_THREAD_PID = 5,					//�Ƿ���˫�̱߳��루��������������ӳ٣�
	ENABLE_STATIC_IMAGE_MODE_PID = 6,			//�Ƿ���Ծ�̬�����Ż�(��HIGH_DEFINITIONģʽ����Ч)
	CUR_BITRATE_LD_PID = 7,						//��ǰ����   (��LOW_DEFINITIONģʽ����Ч,��bitΪ��λ)
	MIN_BITRATE_LD_PID = 8,						//������Сֵ (��LOW_DEFINITIONģʽ����Ч,��bitΪ��λ)
	MAX_BITRATE_LD_PID = 9,						//�������ֵ (��LOW_DEFINITIONģʽ����Ч,��bitΪ��λ)
	CUT_PICTURE_RATIO = 11,						//����ǰ�и���С��32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶ȡ�
	                                            //����320*240�����ʾΪ:(unsigned)(320 << 16 | 240)
	                                            //����LOW_DEFINITIONģʽ��ENC_PICTURE_RATIO_PID��ӦvalueΪRESOLUTION_RATIO_DEPEND_ON_CUT_PICTURE_RATIO������
	ENC_PICTURE_RATIO_PID = 17,					//���뻭����� LOW_DEFINITIONģʽ����ֵ��ΪRESOLUTION_RATIO_DEPEND_ON_CUT_PICTURE_RATIOʱ������
												
	VIRTUAL_CAM_MASK_OFF_PID = 19,				//���γ���������Ƶ���
	LONG_TERM_REF_FRAME_PID = 22,				//�Ƿ�����Ч�ο�֡���룬����������΢�������������濨�ٻָ��죬���ǻ����Լ���web�˵Ľ����ٶȣ�Ĭ�Ϲر�
	FORCE_CONSTANT_FRAMERATE_PID = 23,			//���ⲿ���뻭�治�ȶ�������£��Ƿ�ͨ��������ͬ֡�ķ�ʽ�ﵽ֡���ȶ���Ч����LOW_DEFINITIONĬ��FALSE��HIGH_DEFINITIONĬ��TRUE
	SELF_ADAPTIVE_ENCODE_STRENGTH_PID = 24,		//�Ƿ�������ǿ������Ӧ���ܣ�LOW_DEFINITIONĬ�Ͽ�����HIGH_DEFINITIONĬ�Ϲر�
	HIGH_ENCODE_STRENGTH_PID = 25,				//�Ƿ�����ǿ�ȱ��빦�ܣ���cpuҪ��ϸߣ�����ֱ��ʽϵ͵�ʱ��ʹ�ã�LOW_DEFINITIONĬ�Ͽ�����HIGH_DEFINITIONĬ�Ϲر�
	LONG_GOP_SIZE_PID = 26,						//�Ƿ�ʹ�ýϳ���I֡���������LONG_TERM_REF_FRAME_PIDΪFALSEʱʹ�ã���ʹ�ÿ���߻��ʣ���Ӱ�����ָ��ٶȣ�LOW_DEFINITIONĬ��FALSE��HIGH_DEFINITIONĬ��TRUE
	MOVIE_TUNE_PID = 27,						//�Ƿ����Ϊ��Ӱ�����Ż��ı������ã�LOW_DEFINITIONĬ��FALSE��HIGH_DEFINITIONĬ��TRUE
	RESOLUTION_ADJUST_MODE_PID = 28,			//����ֱ�����Ԥ����̨�ֱ��ʳ���Ȳ�һ��ʱ�ĵ������� ����ֵ�ο�enum ResolutionAdjustMode��LOW_DEFINITIONĬ��ADJUST_CUT_TO_ARENA_RATIO��HIGH_DEFINITIONĬ��ADJUST_STRETCH_KEEP_SRC_RATIO
	SELF_ADAPTIVE_ENCODE_RESOLUTION_PID = 29,	//�Ƿ����ʵ���������Ļ��ʣ�psnr����̬��������ֱ��ʵĴ�С��Ĭ�Ͽ���
	NEED_PREVIEW_CALLBACK = 32,					//�Ƿ���Ҫ����������Ԥ������ص���Ĭ��ΪTRUE
	VBR_MODE_PID = 33,							//�Ƿ���vbrģʽ������ʱ���������ʽ���Ԥ�����ʵ�0.3��1.7��֮�両����Ĭ��FALSE
	LOW_CPU_COST_TUNE_PID = 34,					//�Ƿ���õ�cpu���ı������ã�Ĭ��FALSE.����ΪTRUE���������ʼ���Ե�cpu���ķ�ʽ������SELF_ADAPTIVE_ENCODE_STRENGTH_PID�����ý���Ч��
	GOP_SIZE = 40,								//����gop��С��(ע������������������ѡ��ʱ��Ч��LONG_TERM_REF_FRAME_PID�� LONG_GOP_SIZE_PID�� LOW_CPU_COST_TUNE_PID�� QUALITY_DELAY_MODE_PID = HIGHT_QUALITY_MODE)

	//ע�⣺������ѡ������ӵ�pluginConfig(ȫ��)����publishConfig(publisher���)��β����
	ENC_CODEC_ID = 70,                          //ѡ����ʵ�����(264��265��other)
	USER_DEFINE_FRAME_RATE = 71,				//�Զ���֡�� fps
	USER_DEFINE_BIT_RATE = 72,				//�Զ������� in kbps 
	USER_DEFINE_RESOLUTION_RATIO = 73,			//�Զ���ֱ��� in format:  width << 16 | height; �ѷ��������Ƽ�ʹ��

	ENCODING_THREAD_COUNT_PID = 74,
	ENCODING_PARAM_STRING = 75,		// used by codec extra parameter 
	OPT_FPS_UPON_INPUT_FPS_TUNE_PID = 76,       //��������֡������������Ԥ��֡�ʣ�Ĭ��FALSE.����ΪTRUE������������������֡��������Ԥ������֡��

	FORCE_USER_DEFINE_RESOLUTION_RATIO = 77,	//ǿ��ʹ���û��Զ���ķֱ��ʣ�����ֱ��ʣ� in format:  width << 16 | height; ��Ƚ���4�ֽڶ�����Ż�����

	SET_VIP_USER = 78,							//�����û��Ƿ�VIP
	ABSOLUTE_BIND_HWND_WITH_MIC_ORDER = 79,		//����ʹ��������Ƿ���԰󶨡����������4�ˣ��谴���������λ�ð󶨵�4�����ھ������ʱ������������Ϊ2�ˣ���
												//����ΪFALSEʱ��ֻ���2�����ڣ�����2�����ڸ��ݴ���λ�����հף�������ʾ�л���ģ���û�л���ķŵ�����
												//����ΪTRUEʱ��ֻ���2�����ڣ�����2�����ڸ�������λ�����հ�
												//����ģ�彨������ΪFALSE
												//������Ƶ��������20������ֱ����������ΪTRUE
												//�˲�����SDK�ڲ���Ĭ��ֵ��TRUE
	CAPTURE_CARD_MODE = 80,						//�ɼ��豸�Ƿ��ǲɼ���������ǲɼ�����ֵΪTRUE������ֵΪFALSE������YY���¡�USB����ͷ��WebCamera�ȣ���Ĭ��ֵ��FALSE
	AUTO_SUBSCRIBE_STREAM = 81,					//�Ƿ��Ƶ����Ĭ�϶�����Ƶ�� 1 -> �� 0 -> ��(Ĭ��)
	PREPROCESS_SHOWD = 82,						//���RGB����ʱ���Ƿ�Ԥ����showd��Ĭ��ֵ��TRUE

	DISABLE_OUTPUT_RGB = 83,                    //�Ƿ�����RGB���������Ĭ��ֵΪ0��ʾ����RGB�����ȡֵΪ1ʱ��ʾ����RGB���
	//HARDWARE_DECODER_ID = 84,                   //����Ӳ��Ľ�����ID��Ĭ��ֵΪ0��ʾ������Ӳ�⡣ȡֵ�ο�enum HW_CODEC_PROVIDER��Ŀǰֻ֧��HW_CODEC_NVIDIA_H264

	/*encode param string*/
	AVC_ENCODING_PARAM_STRING = 85,				//h264 software encode param string
	INTEL_AVC_ENCODING_PARAM_STRING = 86,		//h264 intel hardware encode param string
	NVIDIA_AVC_ENCODING_PARAM_STRING = 87,		//h264 nvidia hardware encode param string
	
	HEVC_ENCODING_PARAM_STRING = 88,			//h265 software encode param string 
	INTEL_HEVC_ENCODING_PARAM_STRING = 89,		//h265 intel hardware encode param string
	NVIDIA_HEVC_ENCODING_PARAM_STRING = 90,		//h265 nvidia hardware encode param string
	QY_HEVC_ENCODING_PARAM_STRING = 91,			//QY 265 hardware encode param string

	STREAM_DEC_CODEC_ID = 92,							//������id,�ɲο�enum DecCodeID,������������û�(|)����,����:DEC_CODEC_FFMPEG |DEC_CODEC_ITMHEVC_HEVC
	UPDATE_META_DATA_NOW = 93,					//�Ƿ������㲥MetaData��uint32ֵ: =0 ���㲥, >0 �㲥�� Ĭ�ϲ��㲥

	CAMERA_DATA_CALLBACK = 94,					//����ͷ���ݻص�
	DWX265_ISOLATED_PROCESS = 95,				//x265���ʱ���Ƿ�ʹ�ö������̷�ʽ��Ĭ��ΪFALSE
	
	FLV_FASTACCESS_DURATION = 96,				//FLVģʽ���ٽ���׶�ʱ��

	CAMERA_DATA_CALLBACK_FORMAT = 97,			//����ͷAR�ص�ʱ�������Ƶ��ʽeAV_PIX_FMT_RGB24��eAV_PIX_FMT_RGBA��Ĭ����eAV_PIX_FMT_RGB24
	CAMERA_CAPTURE_FORMAT = 98,					//����ͷ�ɼ�ʱ����Ƶ��ʽeAV_PIX_FMT_RGB24��eAV_PIX_FMT_RGBA��eAV_PIX_FMT_YUV420P��Ĭ����-1����ʾ��ǿ����������ͷ�Ĳɼ���ʽ
	DISABLE_CHANGE_ENCODER_ON_LOWDELAY = 99,	//����ʱģʽʱ��ֹ���ı��������ã�ȡֵ0��1��Ĭ��ֵ��0����ʾ����ʱģʽʱ�����ø��ı��������õ��߼�
	PID_RESERVED_100 = 100,						//����
	PID_RESERVED_200 = 200,						//����
};

enum ResolutionAdjustMode
{
	ADJUST_CUT_TO_ARENA_RATIO = 0,      //�ü�������̨һ�������֮���������С����̨�ֱ��ʴ�С
	ADJUST_STRETCH_KEEP_SRC_RATIO = 1, //���ֱ���ֱ�����Դ�ֱ���һ�£��Ŵ�/��С�����Ĳ�������̨�ֱ��ʵĳߴ�
};

enum StreamNotify
{
	STREAM_PAUSE = 0,
	DLL_QUIT = 1,
};

enum StreamAttribute
{
	STREAM_RESOLUTION = 0,					//�ֱ��ʣ� 32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶ȡ�
	STREAM_BIT_RATE = 1,					//����
	STREAM_FRAME_RATE = 2,					//֡��

	PUBLISHER_UP_LINK_LOSS_RATE = 3,		//������·������
	ENCODE_OVER_LOAD = 4,					//���������
	PROCESS_MAX_CPU = 5,					//���ڽ���CPUʹ����
	SYSTEM_MAX_CPU = 6,						//����ϵͳCPUʹ����
	UNACK_COUNT = 7,						//û���յ�ack�İ���
	PUBLISHER_RTT = 8,						//����rtt
	PUBLISHER_RTT_DELTA = 9,				//����rtt����

	//ʵʱ������
	REAL_STREAM_BIT_RATE = 10,				//ʵʱ����
	REAL_STREAM_FRAME_RATE = 11,			//ʵʱ֡��
	REAL_STREAM_RATIO = 12,					//ʵʱ�ֱ���

	CPU_MHZ = 13,							//CPU��Ƶ,��λ��Mhz
	CPU_CORE_COUNT = 14,					//CPU������
	SYSTEM_TOTAL_MEMORY = 15,				//ϵͳ���ڴ��С����λ��Kb
	SYSTEM_USE_MEMORY = 16,					//ϵͳ�����ڴ��С����λ��Kb
	PROCESS_USE_MEMORY = 17,				//�����������ڴ��С����λ��Kb
	VIWER_DOWN_LINK_LOSS_RATE = 18,			//����������·������%
	PUBLISHER_PSNR = 19,					//����psnr
	SEND_AUDIO_COUNT = 20,					//������Ƶ֡��
	PLAY_AUDIO_COUNT = 21,					//������Ƶ֡��
	AUDIO_SERIAL_LOSS_TIMES = 22,			//��������Ƶ֡����

    FRAME_RATE_NOT_SUFFICIENT_REASON =23,   //֡�ʲ���ԭ��
                                                //0��֡������  
                                                //1���������벻�㣨����ͷ���⣩   
                                                //2����Ƶ���ʵͣ�Ϊ�������ʶ�����֡��    
                                                //3���������������֡�ʲ���(���������Ϊcpu����,С���ֿ�����Ϊ����ʱ�������ȶ�)
                                                //4�����綪֡
	BITRATE_DECREASE_REASON = 24,			//�����½�ԭ��,enum BitRateDecreaseReason
	ENCODE_TYPE = 25,						//�������ͣ��� enum VideoEncodecType

	ENCODER_INPUT_RESOLUTION     = 26,      //��������ƵԴ����ֱ���, 32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶�
	ENCODER_INPUT_FRAMERATE      = 27,      //��������ƵԴ����֡��
	ENCODER_OPTIMIZE_RESOLUTION  = 28,      //������������ı���ֱ���, 32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶�
	ENCODER_OPTIMIZE_FRAMERATE   = 29,      //������������ķֱ���, 32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶�
	ENCODER_OPTIMIZE_BITRATE     = 30,      //�����������������
	ENCODER_OUTPUT_RESOLUTION    = 31,      //������ʵ������ֱ���, 32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶�
	ENCODER_OUTPUT_FRAMERATE     = 32,      //������ʵ�����֡��
	ENCODER_OUTPUT_BITRATE       = 33,      //������ʵ���������

	DECODER_OUTPUT_RESOLUTION    = 34,      //������ʵ������ֱ���, 32λ��������16λ��ʾ��ȣ���16λ��ʾ�߶�
	DECODER_OUTPUT_FRAMERATE     = 35,      //������ʵ�����֡��
	DECODER_OUTPUT_BITRATE       = 36,      //������ʵ���������

};

enum UserStrInfo
{
	//����Ӳ����Ϣ
	CPU_NAME = 0,
	DISPLAY_CARD_NAME = 1,
	//������������Ϣ
	ENCODER_NAME = 2,    //����������
	ENCODER_VERSION = 3, //�������汾��
	//���ڶ˽�������Ϣ
	DECODER_NAME = 4,    //����������
	DECODER_VERSION = 5, //�������汾��
};

enum EncPictureRtion
{
	RESOLUTION_RATIO_DEPEND_ON_CUT_PICTURE_RATIO = 0,
	RESOLUTION_RATIO_4x3 = 1,
	RESOLUTION_RATIO_16x9 = 2,
	RESOLUTION_RATIO_8x9 = 3,
	RESOLUTION_RATIO_5x9 = 4,
	RESOLUTION_RATIO_2x3 = 5
};

enum QualityDelayBalanceMode
{
	LOW_DELAY_MODE = 0,     /*�ϵ��ӳ٣�3-6֡��������������һ����������ѡ��*/
	HIGHT_QUALITY_MODE = 1, /*�ýϸߵ��ӳ�(40֡������)����ȡͬ�������µĽϸ߻��ʣ�������������Ϸ����ֱ��ѡ��*/
	ZERO_LATENCY_MODE = 2,   /*�ޱ����ӳ٣����ʲ�����Խϴ�ʵʱ��������ã����ͳ�����YY��Ƹ����Ҫ�������ڸ߶Ȼ����ĳ���*/
	FULLHD_QUALITY_MODE = 3   /*ȫ����ģʽ������1080p 2500kbps��֧�����ò��б�����߳�����*/
};

enum ExtraMetaDataId
{
	// �ͻ�����100���ڵ��ֶ�
	PUBLISHER_CUR_BIT_RATE						= 7,
	PUBLISHER_MIN_BIT_RATE						= 8,
	PUBLISHER_MAX_BIT_RATE						= 9,
	PUBLISHER_TOTAL_BIT_RATE					= 10,
	PUBLISHER_RESOLUTION						= 11,
	PUBLISHER_FRAME_RATE						= 12,
	PUBLISHER_SUBSTREAM_NUM						= 13,
	PUBLISHER_MIN_PEER_NODE_NUM					= 14,
	PUBLISHER_MAX_PEER_NODE_NUM					= 15,
	PUBLISHER_START_PUBLISH_TIME				= 16,
	PUBLISHER_PUBLISHING_TIME					= 17,		// �����ͻ��˱���ʹ�ã�����ͬ�������������������ͻ���
	PUBLISHER_STREAM_ALIVE						= -1,
	PUBLISHER_VIDEOSDK_VER						= 31,
	PUBLISHER_CAP_SRC_RESELUTION_RATIO			= 32,
	PUBLISHER_CAP_ENC_INFO						= 33,
	PUBLISHER_CAP_ENC_INFO_EX					= 34,
	PUBLISHER_HIGH_DELAY						= 35,
	PUBLISHER_TOP_SID							= 36,
	PUBLISHER_CAP_ENC_INFO_PLUS					= 37,
	PUBLISHER_MY_MAX_BIT_RATE					= 38,
	PUBLISHER_APP_TYPE							= 39,
	PUBLISHER_PUBLISH_ID						= 40,
	PUBLISHER_RESEND_JITTER						= 41,
	PUBLISHER_CLIENT_TYPE						= 42,		// PC 0 web 1 ��׿ 2 ios 3
	PUBLISHER_SECRET_KEY						= 43,		// key for PStreamData3 check sum
	PUBLISER_MAX_PTS_DTS_DIFF					= 44,		// pts��dts֮������ֵ�������ж��Ƿ���ǰ����
	PUBLISER_MULTICODERATE_SEND_FRAME_JITTER	= 45,		// ������ת�����������ת��֡�������
	PUBLISHER_ENCODE_TYPE						= 46,		// ��������
	PUBLISHER_IS_TRANCODE_STREAM				= 47,		// ��������(0:Դ����;1:ת����;2:������)//PUBLISHER_CHANNEL_TYPE
	PUBLISHER_CHECK_INCORRECT_STREAM			= 48,		// �Ƿ���Ҫ�����ڵ�streamId
	PUBLISHER_IS_CROP_STREAM					= 49,		// �Ƿ��Ǻ�������(0:�Ǻ�����;1:���вü��ĺ�����)
	PUBLISHER_COPY_STREAM_FROM_CHANNELID		= 50,
	PUBLISHER_AUDIO_GROUP						= 51,		// ��Ƶ����Ӧ����Ƶ�ಥ��
	PUBLISHER_VIDEO_LINE_ID						= 52,		// ��Ƶ��·ID
	PUBLISHER_AUDIO_SHQ							= 53,		// �Ƿ񳬸����ʿ���
	PUBLISHER_METADATA_VER						= 54,		// ����metaData�汾�ţ���1��ʼ��ÿ�α仯+1
	PUBLISHER_SUPPORT_SPECIAL_RTMP				= 55,		// Я�������ʶ��RTMP���У��ܹ�ʹ����������ͬ��uid�ۿ�����Ƶ

	// [80, 95] Ԥ����Ӧ�ò�����extraMetaData��Ȼ��鿴����IDռ�������http://doc.ys.m.yy.com:8090/pages/viewpage.action?pageId=3060555 �����û���Ҫ��ע�ᣩ
	PUBLISHER_APP_RESERVE1						= 80,		// ����������ģʽ
	PUBLISHER_APP_RESERVE2						= 81,		// ��Y��PC�������ʶ
	PUBLISHER_APP_RESERVE3						= 82,		// PCͶ��
	PUBLISHER_APP_RESERVE4						= 83,		// ��Ƶ������
	PUBLISHER_APP_RESERVE5						= 84,		// ˫�ֻ�����
	PUBLISHER_APP_RESERVE6						= 85,		// ���迪��
	PUBLISHER_APP_RESERVE7						= 86,		// ��Ƶ���淨��ʶ�����绶�����򡢳Ի����ȵ�
	PUBLISHER_APP_RESERVE8						= 87,		// �Ƿ���Ҫ��3D����תΪ2D
	PUBLISHER_APP_RESERVE9						= 88,		// ץ����
	PUBLISHER_APP_RESERVE10						= 89,		// ��������
	PUBLISHER_APP_RESERVE11						= 90,		// APP����Ԥ��11
	PUBLISHER_APP_RESERVE12						= 91,		// 3�˾�����
	PUBLISHER_APP_RESERVE13						= 92,		// ˫��
	PUBLISHER_APP_RESERVE14						= 93,		// ��Ӱ����
	PUBLISHER_APP_RESERVE15						= 94,		// APP����Ԥ��15
	PUBLISHER_APP_RESERVE16						= 95,		// APP����Ԥ��16

	PUBLISHER_RESERVE_META7						= 96,		// Ԥ��7
	PUBLISHER_RESERVE_META6						= 97,		// VR
	PUBLISHER_RESERVE_META5						= 98,		// Ԥ��5
	PUBLISHER_RESERVE_META4						= 99,		// PCȫ����ť

	PUBLISHER_MC_POS							= 100,		// value = 0�����󣩣�value = 1��������
	PUBLISHER_CODERATE_TYPE						= 101,		// value = 0�����ݾɰ汾����value = 1��������pc�˿��ģ���value = 2��������web/mobile���ģ���value = 3������������ƽ̨���ģ�
	PUBLISHER_RESERVE_META1						= 102,		// ���������ĺ�����״   vale = 0 ���ᣩ  value = 1������
	PUBLISHER_RESERVE_META2						= 103,		// ��Y�����ۿ�ʱ����״  vale = 0 ��ԭ�����ڱߣ�   value = 1��������
	PUBLISHER_RESERVE_META3						= 104,		// ��Y�����ۿ�ʱ����״  vale = 0 ��ԭ�����ڱߣ�   value = 1��������

	// ������ʹ���ֶΣ������ͻ���ʹ���ֶ���ҪС��100
	SERVER_TRANSCODE							= 105,		// ������ת��
	SERVER_VALID_TRANSCODE						= 106,		// ������ǰ����д��ת��ɹ����յ�TC�ڵ��ת�����ݣ�ֵΪ1
	SERVER_TRANSCODE_STOPPED					= 107,		// ת��ֹͣ������Դ����֪ͨת������Ƶת����ֹͣ
	SERVER_ORI_STREAM_ID_STAMP					= 108,		// ����ת������Notify�У���¼ԭ������StreamId�ĵ�32λ����ת������uid�����ԭ������Streamid
	SERVER_TRANS_CODE_TYPE_3_SUPPORT			= 109,		// ԭ�����Ƿ���ת��1200K����������Ҫ��������ʱ�Ĺ��ɣ������ɾ��
	SERVER_TRANS_CODE_ORI_STREAM_CODE_RATE		= 110,		// ԭ��UserGroup��totalCodeRate
	SERVER_TRANSCODE_TRANS_LEVEL				= 111,		// ��ҵ��ָ����ת��ĵ�λ
	SERVER_H265_STREAM							= 112,		// ��ʶ����ΪH265��
	SERVER_FASTACCESS_MODE						= 113,		// ��Ƶ���ٽ�����Ļ�ȡ��ʽ��1��ʾ�ͻ����Լ��� 2��ʾ������ֱ����
	SERVER_FASTACCESS_FIRST_SEQ					= 114,		// ���ٽ������Сֵ������SERVER_FASTACCESS_MODEΪ1ʱ������[first, last]
	SERVER_FASTACCESS_LAST_SEQ					= 115,		// ���ٽ�������ֵ������SERVER_FASTACCESS_MODEΪ1ʱ������[first, last]
	SERVER_FASTACCESS_FIRST_DTS					= 117,		// ���ٽ�����С֡��Ӧ�Ĳɼ�ʱ�䣬-1��ʾ��Ч
	SERVER_FASTACCESS_LAST_DTS					= 118,		// ���ٽ������֡��Ӧ�Ĳɼ�ʱ�䣬-1��ʾ��Ч
	SERVER_STREAM_TYPE							= 200,
	ORI_STREAM_TIMESTAMP						= 201,
	USER_RESEND_LIMIT							= 202,		// �û�ά�ȵ��ش����ޣ��ɷ�������д֪ͨ�ͻ���
	SERVER_TRANS_TASK_PRIORITY					= 204,		// �����Ƶ����ת����������ȼ���0��ͣ���ֵԽ�����ȼ�Խ��
	//-----ת�����м�¼��������Դ-----//
	SERVER_STREAM_ORI_SID						= 205,
	SERVER_STREAM_ORI_APPID						= 206,
	SERVER_STREAM_ORI_CHANNEL					= 207,
	SERVER_STREAM_CHANNEL_TYPE					= 208,	
	SERVER_AUDIO_FASTACCESS_FIRST_SEQ			= 209,		// ��Ƶ���ٽ������Сֵ������SERVER_FASTACCESS_MODEΪ1ʱ������[min, max]
	SERVER_AUDIO_FASTACCESS_LAST_SEQ			= 210,		// ��Ƶ���ٽ�������ֵ������SERVER_FASTACCESS_MODEΪ1ʱ������[min, max]
};

enum PluginType
{
	NORMAL_TYPE = 0,
	EDUCATION_100 = 1,
	DIRECTOR_TOOL = 2,
};

enum SyncAudioType
{
	OLD_TYPE_SYNC_PUBLISHER_MICUSER=0,
	NEW_TYPE_SYNC_ALL_AUDIO_NOT_PUBLISHER=1,
	NEW_TYPE_SYNC_ALL_AUDIO=2,
};

enum LinkStatus
{
	LINK_CONNECTED = 0,
	LINK_LOSS = 1,
};

enum FlvOverHttpStatus
{
	NO_RESOURCE_IN_SERVER = 0,
	HTTP_LINK_ERROR = 1,
};

enum FlvOverHttpProxyType
{
	YY_STREAM = 0,
	WS_STREAM = 1,
	OTHER_STREAM = 10,
};

enum BitRateDecreaseReason
{
	BITRATE_NORMAL = 0, //��������
	DECREASE_FOR_NET = 1, //����ԭ�������ʱ���
	MUCH_SMALLER_THAN_SET = 2, //������ʵ������С���趨�����������ʵ�����֮��
};
enum RtmpPublishStatus
{
	RTMP_PUBLISH_SUCCESS = 0,
	RTMP_PUBLISH_FAILED = 1,
	RTMP_PUBLISHING_ERROR = 2,
	RTMP_RECONNECT_30SEC_TIMEOUT = 3,
};

enum RtmpSendAudioStatus
{
	RTMP_START_SEND_AUDIO = 0,
	RTMP_STOP_SEND_AUDIO = 1,
};

enum RTMPServer
{
	YY_RTMPServer = 0,
	WS_RTMPServer = 1,
};

enum CycleStatisticsKey
{
	DOWNLINK_BANDWIDTH = 0, //��λ��bit/s
	DOWNLINK_FROM_SERVER = 1, // �ӷ������յ�������
	VIDEO_WATCH_DELAY = 2,	// ��Ƶ����ʱ��
	VIDEO_TOTAL_DELAY = 3,	// ���ӳ�
	IS_MIC_USER = 4,		// �Ƿ������û�
	VIDOE_RECV_FRAME_CNT = 5,	// �յ�����Ƶ֡����
	VIDEO_NETWORK_LOSSFRAME_CNT = 6,	// ���綪֡��
	VIDEO_DECODE_LOSSFRAME_CNT = 7,		// ���붪֡��
	VIDEO_RENDER_LOSSFRAME_CNT = 8,		// ��Ⱦ��֡��
	VIDEO_UPLINK_LOSS_RATE = 9,	// ���ж�����, �ٷ���
	VIDEO_DOWNLINK_LOSS_RATE = 10,	// ���ж�����, �ٷ���
	LINK_RTT = 11,	// RTT
};

enum DetectBandwidthResultEnum
{
	DETECT_SUCCESS = 0,
	FAIL_TO_CONNECT_DETECT_SERVER = 1,
	DETECT_TIME_OUT = 2,
};

//ȡֵ��Դ��ffmpeg
enum VideoPixelFormat
{
	eAV_PIX_FMT_YUV420P = 0,
	eAV_PIX_FMT_YUYV422 = 1,
	eAV_PIX_FMT_RGB24 = 2,
	eAV_PIX_FMT_BGR24 = 3,
	eAV_PIX_FMT_YUVJ420P = 12,
	eAV_PIX_FMT_UYVY422 = 17,
	eAV_PIX_FMT_NV12 = 25,
	eAV_PIX_FMT_NV21 = 26,
	eAV_PIX_FMT_ARGB = 27,
	eAV_PIX_FMT_RGBA = 28,
	eAV_PIX_FMT_ABGR = 29,
	eAV_PIX_FMT_BGRA = 30,
	eAV_PIX_FMT_YVYU422 = 119,
};

enum ForwardStatus
{
	FORWARD_SUCCESS = 0,	//ת���ɹ�
	FORWARD_FAILED = 1,		//ת��ʧ��
	FORWARD_FORBID = 2,		//��ת��Ƶ����Ȩʧ��
	FORWARD_ERROR = 3,		//ת����������
	FORWARD_NOT_SUPPORT = 4,//��֧��Ƶ��ת��
};

enum StreamSourceType
{
	FROM_ORIGINAL = 0,		//ԭ����
	FROM_TRANSCODE = 1,		//ת�뵵
	FROM_COPY = 2,			//���Ƶ�
};

// ҵ����Ƶ����״̬�ص�
enum AudioTransmitStatus
{
	AUDIOTRANSMIT_STATUS_OK					= 0,		// �����ɹ�
	AUDIOTRANSMIT_STATUS_ASC_ERROR			= 1,		// audioSpecificConfig�ṹ����
	AUDIOTRANSMIT_STATUS_UNSUPPORT_CODEC	= 2,		// ��֧�ֵ���Ƶ��ʽ
	AUDIOTRANSMIT_STATUS_DUPLICATE_START	= 3,		// �ظ�start
	AUDIOTRANSMIT_STATUS_NOT_START			= 4,		// δ������Ƶ����ģʽ
	AUDIOTRANSMIT_STATUS_UNKNOWN_CODEC		= 5,		// δ������������
	AUDIOTRANSMIT_STATUS_UNSUPPORT_TRANSMIT	= 7,		// sdkģʽ��֧����Ƶ���书��
};

enum LiveStreamType
{
	LIVE_STREAM_TYPE_V = 1,		//��Ƶ��
	LIVE_STREAM_TYPE_A = 2,		//��Ƶ��
};

/***���¸�ö����ʱ����Ҫ��֤pc��mobһ��***/
enum LiveStreamCfgKey
{
	STREAM_CONFIG_KEY_PUBLISHID					= 1,	//publishId(���ݾ�ϵͳʹ��)
	STREAM_CONFIG_KEY_SPEAKER_UID				= 2,	//��uid
	STREAM_CONFIG_KEY_STREAM_TYPE				= 3,	//�ο�:enum LiveStreamType
	STREAM_CONFIG_KEY_CODE_RATE					= 4,	//����,��λ:kbps
	STREAM_CONFIG_KEY_ENCODE_TYPE				= 5,	//
	STREMA_CONFIG_KEY_AUDIO_CHANNELS			= 6,
	STREAM_CONFIG_KEY_AUDIO_SAMPLERATE			= 7,
	STREAM_CONFIG_KEY_PUBLISH_STREAM_PROVIDER	= 8,	//mob--LiveStreamProvider, ���������� yysdk�ɼ�������Y�Լ��ɼ���
	STREAM_CONFIG_KEY_SID_BROADCAST_AUDIO		= 9,	//Ƶ������Ƶ�㲥��ǣ�0=false,��0=true;
	STREAM_CONFIG_KEY_PUBLISHER_MC_POS			= 10,	//mob--��ʶ�������Ǳ�����
	STREAM_CONFIG_KEY_FRAME_RATE				= 11,	//ҵ�����õ�֡��
	STREAM_CONFIG_KEY_RESOLUTION_WIDTH			= 12,
	STREAM_CONFIG_KEY_RESOLUTION_HEIGHT			= 13,
	STREAM_CONFIG_KEY_STREAM_LINE				= 14,	//��·
	STREAM_CONFIG_KEY_STREAM_LINE_GROUP			= 15,	//ģ��������·��Ϣʱ��Ҫָ��
	STREAM_CONFIG_KEY_STREAM_A_V_PAIR_ID		= 16,	//��Ƶ����Ƶ��Ե�pairId
};

//���ظ�ģ�����·��Ϣ����Դ����
enum LiveResLineInfoType
{
	FROM_SUBSCRIBE_INVAILD_LINE = -1,	//���Ѿ���������·ʱ ҵ����Ȼ������Ч��·-1
	FROM_INQUIRY_RES = 0,				//��ѯ��·�����res�е���·��Ϣ����
	FROM_SERVER_NOTIFY = 1,				//server����֪ͨ����·��Ϣ
	FROM_FETCH_AVP_RES = 2,				//��ȡǰ�˵�ַ��res�з��ص���·��Ϣ
};

struct LiveStreamInfo
{
	LiveStreamInfo()
	{
	}

	bool operator < (const LiveStreamInfo& dest) const
	{
		return streamName < dest.streamName;
	}

	std::string streamName;
	std::map<unsigned, unsigned> configs;	//key:��enum LiveStreamCfgKey
};

struct LiveStreamLineInfo
{
	LiveStreamLineInfo()
	{
	}

	bool operator < (const LiveStreamLineInfo& dest) const
	{
		return streaName < dest.streaName;
	}
	std::string streaName;
	std::set<unsigned> lines;
};

struct LiveLoginGroupInfo
{
	LiveLoginGroupInfo()
	{

	}

	bool operator < (const LiveLoginGroupInfo& dest) const
	{
		return loginGroup < dest.loginGroup;
	}

	std::string loginGroup;
	std::map<unsigned, unsigned> configs;	//key:��enum LiveStreamCfgKey
};

enum LiveSrcStreamShowdConfig
{
	SHOWD_LOCTION_X		= 0,
	SHOWD_LOCTION_Y		= 1,
	SHOWD_LOCTION_W		= 2,
	SHOWD_LOCTION_H		= 3,
	TOTAL_W				= 4,
	TOTAL_H				= 5,
	SHOWD_CONTENT		= 100,
};

enum LivePublishStatus	//����״̬֪ͨ
{
	LIVE_PUBLISH_SUCCESS		= 0,		//�����ɹ�
	LIVE_PUBLISH_PARAM_ERROR	= 1,		//��������
	LIVE_PUBLISH_FAILED			= 2, 		//����ʧ��
	LIVE_STATUS_AUTH_FAIL		= 3,		//��Ȩʧ��
	LIVE_PUBLISH_STOP			= 4,		//����ֹͣ
	LIVE_PUBLISH_NOT_ANCHOR_SYS	= 5,		//��������ϵͳ,��������
};

enum LiveSubcribeStatus	//����״̬֪ͨ
{
	LIVE_SUBCRIBE_SUCCESS		= 0,		//���ĳɹ�
	LIVE_SUBCRIBE_PARAM_ERROR	= 1,		//��������
	LIVE_SUBCRIBE_FAILED		= 2,		//����ʧ��
	LIVE_SUBCRIBE_CANCEL		= 3,		//����ȡ��
};

struct VideoConvertParam
{
	int srcW;
	int srcH;
	int dstW;
	int dstH;
	enum VideoPixelFormat srcFormat;
	enum VideoPixelFormat dstFormat;
	unsigned char *src[3];
	int srcStride[3];
	unsigned char *dst[3];
	int dstStride[3];
	int colorspace;
	int fullrange;
	int flag;
	void **ctx;
	unsigned char reserved[128];
	
	VideoConvertParam() {
		memset(this, 0, sizeof(*this));
	}
};

struct VideoRenderInfo
{
	unsigned uid;
	BITMAPINFOHEADER bih;
	const unsigned char *data;
	int lenght;
	const unsigned char *yuv[3];
	int linesize[3];
	int debug_source;

	void *showd_obj;
	void (*showd)(void *obj, HDC hDC);
	const wchar_t* (*showdtext)(void *obj, wchar_t *textbuf, int maxlen); //maxlen������ֵĸ�������bufsize�Ĺ�ϵ�ǣ�bufsize = maxlen * sizeof(wchar_t)

	int yuv_width;
	int yuv_height;

	unsigned int pts;

	unsigned char reserved[128];

	VideoRenderInfo() {
		memset(this, 0, sizeof(*this));
	}
};

// (1) �����ģ�嵥���̰汾�У�renderDelay��-1��ͬʱrenderStamp���֡����Ⱦʱ��(timeGetTime)
// (2) �����ģ�����̰汾�У�renderDelay��ʵ����Ⱦ��ʱ, renderStamp�ɺ���
// ע��ģ�嵥���̰汾��������vsdk֡����Ⱦ֡��ͬһ��������
// ע����������� renderStamp����������ֵ�ģ��Ѻ�ģ��ȷ��
struct FrameRenderInfo
{
	unsigned renderDelay;	// ��Ⱦ��ʱ;��ģ�����̰汾��, ģ���Լ��������Ⱦ��ʱ
							//			 ��ģ�嵥���̰汾��, ��ֵ��-1	
	unsigned pts;
	unsigned renderStamp;

	FrameRenderInfo()
	{
		renderDelay = -1;
		pts = 0;
		renderStamp = 0;
	}
};

struct Layout
{
	unsigned x;
	unsigned y;
	unsigned w;
	unsigned h;
};

struct ETVsdkEvent
{
	unsigned m_eventId;
	unsigned m_reserve;

	ETVsdkEvent(){ m_eventId = 0; m_reserve = 0;}
};

typedef ETVsdkEvent ETVsdkInvoke;

}