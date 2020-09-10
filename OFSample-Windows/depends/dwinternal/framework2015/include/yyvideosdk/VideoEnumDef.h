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
	//该枚举已被废弃，请使用 USER_DEFINE_FRAME_RATE (冯布洲)
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
	//该枚举已被废弃，请使用 FORCE_USER_DEFINE_RESOLUTION_RATIO (冯布洲)
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
	DEC_CODEC_FFMPEG		= 1 << 0,	//支持H265,H264两种
	DEC_CODEC_ITMHEVC_HEVC	= 1 << 1,	//目前只用于H265解码
	DEC_CODEC_NVIDIA_AVC    = 1 << 2,	//H264 NVIDIA硬解
	DEC_CODEC_INTEL_AVC     = 1 << 3,	//H264 INTEL硬解
	DEC_CODEC_NVIDIA_HEVC   = 1 << 4,	//H265 NVIDIA硬解
	DEC_CODEC_INTEL_HEVC    = 1 << 5,	//H265 INTEL硬解
};

enum VideoEncodecType
{
	ENCODE_UNKNOW	= 0,		// 未知编码类型
	ENCODE_H264		= 1,		// H264编码
	ENCODE_H265		= 2,		// H265编码
	ENCODE_VP8		= 3,		// VP8编码
	ENCODE_VP9		= 4,		// VP9编码
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
	PLAY_DELAY_PID = 0,							//播放延迟 设置单位为毫秒
	DISPLAY_AUDIO_WAVE_PID = 1,					//在画面显示音频波形
	SYNC_ALL_MC_LIST_AUDIO = 10,				//是否同步麦序上所有uid声音
	TOTAL_BIT_RATE_PID = 12,					//当前子频道视频总码率
	IGNORE_YY_MIC_LIST_MODE = 13,				//是否忽略YY的麦序，完全由插件控制主播权限
	PLUGIN_P2P_SWITCH_PID = 14,					//插件是否开启P2P
	SMOOTH_VIDEO_UPLOAD = 15,					//平滑发送主播（会增加上行传输延迟）
	PUBLISHER_NUM = 16,							//设置同时主播人数
	QUALITY_DELAY_MODE_PID = 18,				//画质/延时模式选择 可用值参考enum QualityDelayBalanceMode
	FORMER_ANCHOR_NUM = 20,						//指定麦序前FORMERANCHORNUM位关闭p2p，并且其间的音视频延时低，同时与外部音视频延时高
	FAST_ACCESS = 21,							//是否支持快速接入，1默认为支持 
	FAKE_APPID = 30,							//指定appId，指定后，用户可以使用该appId来主播视频
	PLUGIN_TYPE = 31,							//指定插件类型，该参数可用于在不改变appid的前提下把不同类型的插件统计数据区分开来，类型可见enum PluginType
	SYNC_ALL_AUDIO_NOT_PUBLISHER_PID=35,		//是否开启将除了主播音频之外的所有音频传送到vsdk进行同步，0默认不开启
	MULTI_CODE_RATE_LEVEL = 36,					//指定多码率方案中的码率档位，默认档位为0
	OPEN_MULTICODERATE = 37,					//是否开启多码流功能，0默认不开启,1开启
	RGB_OUTPUT_FORMAT  =38,                     //0:bgr(rgb24) 1:bgra(rgb32)
	RGB_OUTPUT_FLIP = 43,                       //输出的图像数据是否需要上下翻转为颠倒的图像, 0 不需要翻转，1需要翻转
	FLV_OVER_HTTP_MODE = 39,					//是否使用http传输flv模式
	YCLOUD_VIDEO_MODE = 41,						//模式
	YCLOUD_IS_1V1_MODE = 42,					//是否1v1模式
	MULTI_OPTION_SWITCH = 65,					//传输层开关选项可由应用层设置，对应ParaDef.h中的MULTI_TEST_SWITCH
	USE_RTMP_PUBLISH = 66,						//是否使用rtmp方式上行flv的音视频，0默认不开启,1开启
	DELAY_SEND_TIME = 67,						//延迟发送音视频时间，单位ms
	PLUGIN_DECODE_AUDIO_VIDEO = 68,				//是否输出音视频解码前数据给模板进行解码
	SUPPORT_FLV_FAST_ACCESS = 69,				//是否支持FLV快速接入功能, 0 不支持，1支持。默认支持。
	
	//publishConfig
	FRAME_RATE_PID = 2,							//帧率 可用值参考enum VideoFrameRate
	BITRATE_PID = 3,							//码率 (HIGH_DEFINITION模式下生效) 可用值参考enum VideoCodeRateEx
	RESOLUTION_PID = 4,							//分辨率 可用值参考enum VideoRatio
	TRY_USING_2_THREAD_PID = 5,					//是否开启双线程编码（开启会增大编码延迟）
	ENABLE_STATIC_IMAGE_MODE_PID = 6,			//是否针对静态画面优化(仅HIGH_DEFINITION模式下生效)
	CUR_BITRATE_LD_PID = 7,						//当前码率   (仅LOW_DEFINITION模式下生效,以bit为单位)
	MIN_BITRATE_LD_PID = 8,						//码率最小值 (仅LOW_DEFINITION模式下生效,以bit为单位)
	MAX_BITRATE_LD_PID = 9,						//码率最大值 (仅LOW_DEFINITION模式下生效,以bit为单位)
	CUT_PICTURE_RATIO = 11,						//编码前切割画面大小，32位整数，高16位表示宽度，低16位表示高度。
	                                            //例如320*240，则表示为:(unsigned)(320 << 16 | 240)
	                                            //仅当LOW_DEFINITION模式下ENC_PICTURE_RATIO_PID对应value为RESOLUTION_RATIO_DEPEND_ON_CUT_PICTURE_RATIO起作用
	ENC_PICTURE_RATIO_PID = 17,					//编码画面比例 LOW_DEFINITION模式下其值不为RESOLUTION_RATIO_DEPEND_ON_CUT_PICTURE_RATIO时起作用
												
	VIRTUAL_CAM_MASK_OFF_PID = 19,				//屏蔽常见虚拟视频软件
	LONG_TERM_REF_FRAME_PID = 22,				//是否开启长效参考帧编码，开启后画质略微上升，丢包画面卡顿恢复快，但是会明显减慢web端的接入速度，默认关闭
	FORCE_CONSTANT_FRAMERATE_PID = 23,			//在外部输入画面不稳定的情况下，是否通过插入相同帧的方式达到帧率稳定的效果，LOW_DEFINITION默认FALSE，HIGH_DEFINITION默认TRUE
	SELF_ADAPTIVE_ENCODE_STRENGTH_PID = 24,		//是否开启编码强度自适应功能，LOW_DEFINITION默认开启，HIGH_DEFINITION默认关闭
	HIGH_ENCODE_STRENGTH_PID = 25,				//是否开启高强度编码功能，对cpu要求较高，建议分辨率较低的时候使用，LOW_DEFINITION默认开启，HIGH_DEFINITION默认关闭
	LONG_GOP_SIZE_PID = 26,						//是否使用较长的I帧间隔（仅当LONG_TERM_REF_FRAME_PID为FALSE时使用），使用可提高画质，但影响错误恢复速度，LOW_DEFINITION默认FALSE，HIGH_DEFINITION默认TRUE
	MOVIE_TUNE_PID = 27,						//是否采用为电影画面优化的编码配置，LOW_DEFINITION默认FALSE，HIGH_DEFINITION默认TRUE
	RESOLUTION_ADJUST_MODE_PID = 28,			//输入分辨率与预设舞台分辨率长宽比不一致时的调整策略 可用值参考enum ResolutionAdjustMode，LOW_DEFINITION默认ADJUST_CUT_TO_ARENA_RATIO，HIGH_DEFINITION默认ADJUST_STRETCH_KEEP_SRC_RATIO
	SELF_ADAPTIVE_ENCODE_RESOLUTION_PID = 29,	//是否根据实际输出画面的画质（psnr）动态调整编码分辨率的大小，默认开启
	NEED_PREVIEW_CALLBACK = 32,					//是否需要将主播自身预览画面回调，默认为TRUE
	VBR_MODE_PID = 33,							//是否开启vbr模式，启动时，编码码率将在预设码率的0.3到1.7倍之间浮动，默认FALSE
	LOW_CPU_COST_TUNE_PID = 34,					//是否采用低cpu消耗编码配置，默认FALSE.若设为TRUE，则编码器始终以低cpu消耗方式工作（SELF_ADAPTIVE_ENCODE_STRENGTH_PID的设置将无效）
	GOP_SIZE = 40,								//设置gop大小。(注：该设置在设置以下选项时无效。LONG_TERM_REF_FRAME_PID， LONG_GOP_SIZE_PID， LOW_CPU_COST_TUNE_PID， QUALITY_DELAY_MODE_PID = HIGHT_QUALITY_MODE)

	//注意：新增的选项请添加到pluginConfig(全局)或者publishConfig(publisher相关)的尾部。
	ENC_CODEC_ID = 70,                          //选择合适的码流(264、265、other)
	USER_DEFINE_FRAME_RATE = 71,				//自定义帧率 fps
	USER_DEFINE_BIT_RATE = 72,				//自定义码率 in kbps 
	USER_DEFINE_RESOLUTION_RATIO = 73,			//自定义分辨率 in format:  width << 16 | height; 已废弃，不推荐使用

	ENCODING_THREAD_COUNT_PID = 74,
	ENCODING_PARAM_STRING = 75,		// used by codec extra parameter 
	OPT_FPS_UPON_INPUT_FPS_TUNE_PID = 76,       //根据输入帧率来调整编码预设帧率，默认FALSE.若设为TRUE，则编码器会根据输入帧率来调整预设的输出帧率

	FORCE_USER_DEFINE_RESOLUTION_RATIO = 77,	//强制使用用户自定义的分辨率（任意分辨率） in format:  width << 16 | height; 宽度建议4字节对齐可优化性能

	SET_VIP_USER = 78,							//设置用户是否VIP
	ABSOLUTE_BIND_HWND_WITH_MIC_ORDER = 79,		//麦序和窗口序列是否绝对绑定。如果麦序有4人，需按照麦序绝对位置绑定到4个窗口句柄，此时开播的人数若为2人，则：
												//设置为FALSE时，只填充2个窗口，另外2个窗口根据窗口位置留空白，优先显示有画面的，把没有画面的放到后面
												//设置为TRUE时，只填充2个窗口，另外2个窗口根据麦序位置留空白
												//娱乐模板建议设置为FALSE
												//多人视频场景比如20人连麦直播场景设置为TRUE
												//此参数在SDK内部的默认值是TRUE
	CAPTURE_CARD_MODE = 80,						//采集设备是否是采集卡，如果是采集卡赋值为TRUE，否则赋值为FALSE（比如YY伴侣、USB摄像头、WebCamera等）。默认值是FALSE
	AUTO_SUBSCRIBE_STREAM = 81,					//是否进频道就默认订阅视频流 1 -> 是 0 -> 否(默认)
	PREPROCESS_SHOWD = 82,						//输出RGB数据时，是否预处理showd。默认值是TRUE

	DISABLE_OUTPUT_RGB = 83,                    //是否屏蔽RGB数据输出，默认值为0表示允许RGB输出。取值为1时表示屏蔽RGB输出
	//HARDWARE_DECODER_ID = 84,                   //启用硬解的解码器ID。默认值为0表示不启用硬解。取值参考enum HW_CODEC_PROVIDER，目前只支持HW_CODEC_NVIDIA_H264

	/*encode param string*/
	AVC_ENCODING_PARAM_STRING = 85,				//h264 software encode param string
	INTEL_AVC_ENCODING_PARAM_STRING = 86,		//h264 intel hardware encode param string
	NVIDIA_AVC_ENCODING_PARAM_STRING = 87,		//h264 nvidia hardware encode param string
	
	HEVC_ENCODING_PARAM_STRING = 88,			//h265 software encode param string 
	INTEL_HEVC_ENCODING_PARAM_STRING = 89,		//h265 intel hardware encode param string
	NVIDIA_HEVC_ENCODING_PARAM_STRING = 90,		//h265 nvidia hardware encode param string
	QY_HEVC_ENCODING_PARAM_STRING = 91,			//QY 265 hardware encode param string

	STREAM_DEC_CODEC_ID = 92,							//解码器id,可参考enum DecCodeID,多个解码器采用或(|)连接,例如:DEC_CODEC_FFMPEG |DEC_CODEC_ITMHEVC_HEVC
	UPDATE_META_DATA_NOW = 93,					//是否立即广播MetaData，uint32值: =0 不广播, >0 广播。 默认不广播

	CAMERA_DATA_CALLBACK = 94,					//摄像头数据回调
	DWX265_ISOLATED_PROCESS = 95,				//x265软编时，是否使用独立进程方式。默认为FALSE
	
	FLV_FASTACCESS_DURATION = 96,				//FLV模式快速接入阶段时长

	CAMERA_DATA_CALLBACK_FORMAT = 97,			//摄像头AR回调时输出的视频格式eAV_PIX_FMT_RGB24或eAV_PIX_FMT_RGBA，默认是eAV_PIX_FMT_RGB24
	CAMERA_CAPTURE_FORMAT = 98,					//摄像头采集时的视频格式eAV_PIX_FMT_RGB24或eAV_PIX_FMT_RGBA或eAV_PIX_FMT_YUV420P，默认是-1，表示不强制设置摄像头的采集格式
	DISABLE_CHANGE_ENCODER_ON_LOWDELAY = 99,	//低延时模式时禁止更改编码器配置，取值0或1。默认值是0，表示低延时模式时会启用更改编码器配置的逻辑
	PID_RESERVED_100 = 100,						//保留
	PID_RESERVED_200 = 200,						//保留
};

enum ResolutionAdjustMode
{
	ADJUST_CUT_TO_ARENA_RATIO = 0,      //裁剪成与舞台一样长宽比之后，拉伸或缩小至舞台分辨率大小
	ADJUST_STRETCH_KEEP_SRC_RATIO = 1, //保持编码分辨率与源分辨率一致，放大/缩小至最大的不超过舞台分辨率的尺寸
};

enum StreamNotify
{
	STREAM_PAUSE = 0,
	DLL_QUIT = 1,
};

enum StreamAttribute
{
	STREAM_RESOLUTION = 0,					//分辨率， 32位整数，高16位表示宽度，低16位表示高度。
	STREAM_BIT_RATE = 1,					//码率
	STREAM_FRAME_RATE = 2,					//帧率

	PUBLISHER_UP_LINK_LOSS_RATE = 3,		//上行链路丢包率
	ENCODE_OVER_LOAD = 4,					//编码过载率
	PROCESS_MAX_CPU = 5,					//周期进程CPU使用率
	SYSTEM_MAX_CPU = 6,						//周期系统CPU使用率
	UNACK_COUNT = 7,						//没有收到ack的包数
	PUBLISHER_RTT = 8,						//主播rtt
	PUBLISHER_RTT_DELTA = 9,				//主播rtt抖动

	//实时流属性
	REAL_STREAM_BIT_RATE = 10,				//实时码率
	REAL_STREAM_FRAME_RATE = 11,			//实时帧率
	REAL_STREAM_RATIO = 12,					//实时分辨率

	CPU_MHZ = 13,							//CPU主频,单位：Mhz
	CPU_CORE_COUNT = 14,					//CPU核心数
	SYSTEM_TOTAL_MEMORY = 15,				//系统总内存大小，单位：Kb
	SYSTEM_USE_MEMORY = 16,					//系统已用内存大小，单位：Kb
	PROCESS_USE_MEMORY = 17,				//本进程已用内存大小，单位：Kb
	VIWER_DOWN_LINK_LOSS_RATE = 18,			//观众下行链路丢包率%
	PUBLISHER_PSNR = 19,					//主播psnr
	SEND_AUDIO_COUNT = 20,					//发送音频帧数
	PLAY_AUDIO_COUNT = 21,					//播放音频帧数
	AUDIO_SERIAL_LOSS_TIMES = 22,			//连续丢音频帧次数

    FRAME_RATE_NOT_SUFFICIENT_REASON =23,   //帧率不足原因
                                                //0：帧率正常  
                                                //1：主播输入不足（摄像头问题）   
                                                //2：视频画质低，为提升画质而牺牲帧率    
                                                //3：主播编码器输出帧率不足(大多数可能为cpu问题,小部分可能因为输入时间间隔不稳定)
                                                //4：网络丢帧
	BITRATE_DECREASE_REASON = 24,			//码率下降原因,enum BitRateDecreaseReason
	ENCODE_TYPE = 25,						//编码类型，见 enum VideoEncodecType

	ENCODER_INPUT_RESOLUTION     = 26,      //编码器视频源输入分辨率, 32位整数，高16位表示宽度，低16位表示高度
	ENCODER_INPUT_FRAMERATE      = 27,      //编码器视频源输入帧率
	ENCODER_OPTIMIZE_RESOLUTION  = 28,      //编码器调整后的编码分辨率, 32位整数，高16位表示宽度，低16位表示高度
	ENCODER_OPTIMIZE_FRAMERATE   = 29,      //编码器调整后的分辨率, 32位整数，高16位表示宽度，低16位表示高度
	ENCODER_OPTIMIZE_BITRATE     = 30,      //编码器调整后的码率
	ENCODER_OUTPUT_RESOLUTION    = 31,      //编码器实际输出分辨率, 32位整数，高16位表示宽度，低16位表示高度
	ENCODER_OUTPUT_FRAMERATE     = 32,      //编码器实际输出帧率
	ENCODER_OUTPUT_BITRATE       = 33,      //编码器实际输出码率

	DECODER_OUTPUT_RESOLUTION    = 34,      //解码器实际输出分辨率, 32位整数，高16位表示宽度，低16位表示高度
	DECODER_OUTPUT_FRAMERATE     = 35,      //解码器实际输出帧率
	DECODER_OUTPUT_BITRATE       = 36,      //解码器实际输出码率

};

enum UserStrInfo
{
	//主播硬件信息
	CPU_NAME = 0,
	DISPLAY_CARD_NAME = 1,
	//主播编码器信息
	ENCODER_NAME = 2,    //编码器类型
	ENCODER_VERSION = 3, //编码器版本号
	//观众端解码器信息
	DECODER_NAME = 4,    //解码器类型
	DECODER_VERSION = 5, //解码器版本号
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
	LOW_DELAY_MODE = 0,     /*较低延迟（3-6帧），常见场景：一般娱乐主播选用*/
	HIGHT_QUALITY_MODE = 1, /*用较高的延迟(40帧或以上)来换取同等码率下的较高画质，常见场景：游戏赛事直播选用*/
	ZERO_LATENCY_MODE = 2,   /*无编码延迟，码率波动相对较大，实时交互性最好，典型场景：YY招聘等需要主播观众高度互动的场合*/
	FULLHD_QUALITY_MODE = 3   /*全高清模式，至少1080p 2500kbps，支持设置并行编码的线程数量*/
};

enum ExtraMetaDataId
{
	// 客户端用100以内的字段
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
	PUBLISHER_PUBLISHING_TIME					= 17,		// 仅仅客户端本地使用，不会同步给服务器或者其他客户端
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
	PUBLISHER_CLIENT_TYPE						= 42,		// PC 0 web 1 安卓 2 ios 3
	PUBLISHER_SECRET_KEY						= 43,		// key for PStreamData3 check sum
	PUBLISER_MAX_PTS_DTS_DIFF					= 44,		// pts和dts之间最大差值，用于判断是否提前解码
	PUBLISER_MULTICODERATE_SEND_FRAME_JITTER	= 45,		// 周期内转码服务器发送转码帧的最大间隔
	PUBLISHER_ENCODE_TYPE						= 46,		// 编码类型
	PUBLISHER_IS_TRANCODE_STREAM				= 47,		// 码流类型(0:源码流;1:转码流;2:复制流)//PUBLISHER_CHANNEL_TYPE
	PUBLISHER_CHECK_INCORRECT_STREAM			= 48,		// 是否需要检查过期的streamId
	PUBLISHER_IS_CROP_STREAM					= 49,		// 是否是横竖屏流(0:非横竖屏;1:居中裁剪的横竖屏)
	PUBLISHER_COPY_STREAM_FROM_CHANNELID		= 50,
	PUBLISHER_AUDIO_GROUP						= 51,		// 视频流对应的音频多播组
	PUBLISHER_VIDEO_LINE_ID						= 52,		// 视频线路ID
	PUBLISHER_AUDIO_SHQ							= 53,		// 是否超高音质开播
	PUBLISHER_METADATA_VER						= 54,		// 主播metaData版本号，从1开始，每次变化+1
	PUBLISHER_SUPPORT_SPECIAL_RTMP				= 55,		// 携带特殊标识的RTMP上行，能够使用与主播相同的uid观看音视频

	// [80, 95] 预留给应用层设置extraMetaData。然后查看各端ID占用情况：http://doc.ys.m.yy.com:8090/pages/viewpage.action?pageId=3060555 （新用户需要先注册）
	PUBLISHER_APP_RESERVE1						= 80,		// 开播清晰度模式
	PUBLISHER_APP_RESERVE2						= 81,		// 手Y和PC的区别标识
	PUBLISHER_APP_RESERVE3						= 82,		// PC投屏
	PUBLISHER_APP_RESERVE4						= 83,		// 跨频道连麦
	PUBLISHER_APP_RESERVE5						= 84,		// 双手机开播
	PUBLISHER_APP_RESERVE6						= 85,		// 外设开播
	PUBLISHER_APP_RESERVE7						= 86,		// 视频流玩法标识，比如欢乐篮球、吃货脸萌等
	PUBLISHER_APP_RESERVE8						= 87,		// 是否需要将3D画面转为2D
	PUBLISHER_APP_RESERVE9						= 88,		// 抓娃娃
	PUBLISHER_APP_RESERVE10						= 89,		// 多人连麦
	PUBLISHER_APP_RESERVE11						= 90,		// APP设置预留11
	PUBLISHER_APP_RESERVE12						= 91,		// 3人竞技场
	PUBLISHER_APP_RESERVE13						= 92,		// 双摄
	PUBLISHER_APP_RESERVE14						= 93,		// 电影开播
	PUBLISHER_APP_RESERVE15						= 94,		// APP设置预留15
	PUBLISHER_APP_RESERVE16						= 95,		// APP设置预留16

	PUBLISHER_RESERVE_META7						= 96,		// 预留7
	PUBLISHER_RESERVE_META6						= 97,		// VR
	PUBLISHER_RESERVE_META5						= 98,		// 预留5
	PUBLISHER_RESERVE_META4						= 99,		// PC全屏按钮

	PUBLISHER_MC_POS							= 100,		// value = 0（首麦），value = 1（被连麦）
	PUBLISHER_CODERATE_TYPE						= 101,		// value = 0（兼容旧版本），value = 1（给快男pc端看的），value = 2（给快男web/mobile看的），value = 3（给快男所有平台看的）
	PUBLISHER_RESERVE_META1						= 102,		// 主播开播的横竖形状   vale = 0 （横）  value = 1（竖）
	PUBLISHER_RESERVE_META2						= 103,		// 手Y竖屏观看时的形状  vale = 0 （原画带黑边）   value = 1（铺满）
	PUBLISHER_RESERVE_META3						= 104,		// 手Y横屏观看时的形状  vale = 0 （原画带黑边）   value = 1（铺满）

	// 服务器使用字段，后续客户端使用字段需要小于100
	SERVER_TRANSCODE							= 105,		// 服务器转码
	SERVER_VALID_TRANSCODE						= 106,		// 由主播前端填写，转码成功（收到TC节点的转码数据）值为1
	SERVER_TRANSCODE_STOPPED					= 107,		// 转码停止，用于源码流通知转码流视频转码已停止
	SERVER_ORI_STREAM_ID_STAMP					= 108,		// 填在转码流的Notify中，记录原码流的StreamId的低32位，与转码流的uid可组成原码流的Streamid
	SERVER_TRANS_CODE_TYPE_3_SUPPORT			= 109,		// 原码流是否已转码1200K的码流，主要用于升级时的过渡，后面会删除
	SERVER_TRANS_CODE_ORI_STREAM_CODE_RATE		= 110,		// 原码UserGroup的totalCodeRate
	SERVER_TRANSCODE_TRANS_LEVEL				= 111,		// 由业务指定的转码的档位
	SERVER_H265_STREAM							= 112,		// 标识该流为H265流
	SERVER_FASTACCESS_MODE						= 113,		// 视频快速接入包的获取方式，1表示客户端自己拉 2表示服务器直接推
	SERVER_FASTACCESS_FIRST_SEQ					= 114,		// 快速接入包最小值，仅在SERVER_FASTACCESS_MODE为1时起作用[first, last]
	SERVER_FASTACCESS_LAST_SEQ					= 115,		// 快速接入包最大值，仅在SERVER_FASTACCESS_MODE为1时起作用[first, last]
	SERVER_FASTACCESS_FIRST_DTS					= 117,		// 快速接入最小帧对应的采集时间，-1表示无效
	SERVER_FASTACCESS_LAST_DTS					= 118,		// 快速接入最大帧对应的采集时间，-1表示无效
	SERVER_STREAM_TYPE							= 200,
	ORI_STREAM_TIMESTAMP						= 201,
	USER_RESEND_LIMIT							= 202,		// 用户维度的重传上限，由服务器填写通知客户端
	SERVER_TRANS_TASK_PRIORITY					= 204,		// 标记视频流的转码任务的优先级，0最低，数值越大优先级越高
	//-----转发流中记录此流的来源-----//
	SERVER_STREAM_ORI_SID						= 205,
	SERVER_STREAM_ORI_APPID						= 206,
	SERVER_STREAM_ORI_CHANNEL					= 207,
	SERVER_STREAM_CHANNEL_TYPE					= 208,	
	SERVER_AUDIO_FASTACCESS_FIRST_SEQ			= 209,		// 音频快速接入包最小值，仅在SERVER_FASTACCESS_MODE为1时起作用[min, max]
	SERVER_AUDIO_FASTACCESS_LAST_SEQ			= 210,		// 音频快速接入包最大值，仅在SERVER_FASTACCESS_MODE为1时起作用[min, max]
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
	BITRATE_NORMAL = 0, //码率正常
	DECREASE_FOR_NET = 1, //网络原因导致码率被降
	MUCH_SMALLER_THAN_SET = 2, //编码器实际码率小于设定给编码器码率的三分之二
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
	DOWNLINK_BANDWIDTH = 0, //单位：bit/s
	DOWNLINK_FROM_SERVER = 1, // 从服务器收到的流量
	VIDEO_WATCH_DELAY = 2,	// 视频加载时间
	VIDEO_TOTAL_DELAY = 3,	// 总延迟
	IS_MIC_USER = 4,		// 是否连麦用户
	VIDOE_RECV_FRAME_CNT = 5,	// 收到的视频帧个数
	VIDEO_NETWORK_LOSSFRAME_CNT = 6,	// 网络丢帧数
	VIDEO_DECODE_LOSSFRAME_CNT = 7,		// 解码丢帧数
	VIDEO_RENDER_LOSSFRAME_CNT = 8,		// 渲染丢帧数
	VIDEO_UPLINK_LOSS_RATE = 9,	// 上行丢包率, 百分率
	VIDEO_DOWNLINK_LOSS_RATE = 10,	// 下行丢包率, 百分率
	LINK_RTT = 11,	// RTT
};

enum DetectBandwidthResultEnum
{
	DETECT_SUCCESS = 0,
	FAIL_TO_CONNECT_DETECT_SERVER = 1,
	DETECT_TIME_OUT = 2,
};

//取值来源于ffmpeg
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
	FORWARD_SUCCESS = 0,	//转发成功
	FORWARD_FAILED = 1,		//转发失败
	FORWARD_FORBID = 2,		//在转发频道鉴权失败
	FORWARD_ERROR = 3,		//转发参数错误
	FORWARD_NOT_SUPPORT = 4,//不支持频道转发
};

enum StreamSourceType
{
	FROM_ORIGINAL = 0,		//原画档
	FROM_TRANSCODE = 1,		//转码档
	FROM_COPY = 2,			//复制档
};

// 业务音频传输状态回调
enum AudioTransmitStatus
{
	AUDIOTRANSMIT_STATUS_OK					= 0,		// 启动成功
	AUDIOTRANSMIT_STATUS_ASC_ERROR			= 1,		// audioSpecificConfig结构错误
	AUDIOTRANSMIT_STATUS_UNSUPPORT_CODEC	= 2,		// 不支持的音频格式
	AUDIOTRANSMIT_STATUS_DUPLICATE_START	= 3,		// 重复start
	AUDIOTRANSMIT_STATUS_NOT_START			= 4,		// 未开启音频传输模式
	AUDIOTRANSMIT_STATUS_UNKNOWN_CODEC		= 5,		// 未设置音质类型
	AUDIOTRANSMIT_STATUS_UNSUPPORT_TRANSMIT	= 7,		// sdk模式不支持音频传输功能
};

enum LiveStreamType
{
	LIVE_STREAM_TYPE_V = 1,		//视频流
	LIVE_STREAM_TYPE_A = 2,		//音频流
};

/***更新该枚举类时，需要保证pc、mob一致***/
enum LiveStreamCfgKey
{
	STREAM_CONFIG_KEY_PUBLISHID					= 1,	//publishId(兼容旧系统使用)
	STREAM_CONFIG_KEY_SPEAKER_UID				= 2,	//流uid
	STREAM_CONFIG_KEY_STREAM_TYPE				= 3,	//参考:enum LiveStreamType
	STREAM_CONFIG_KEY_CODE_RATE					= 4,	//码率,单位:kbps
	STREAM_CONFIG_KEY_ENCODE_TYPE				= 5,	//
	STREMA_CONFIG_KEY_AUDIO_CHANNELS			= 6,
	STREAM_CONFIG_KEY_AUDIO_SAMPLERATE			= 7,
	STREAM_CONFIG_KEY_PUBLISH_STREAM_PROVIDER	= 8,	//mob--LiveStreamProvider, 发布的流从 yysdk采集还是手Y自己采集的
	STREAM_CONFIG_KEY_SID_BROADCAST_AUDIO		= 9,	//频道内音频广播标记，0=false,非0=true;
	STREAM_CONFIG_KEY_PUBLISHER_MC_POS			= 10,	//mob--标识是首麦还是被连麦
	STREAM_CONFIG_KEY_FRAME_RATE				= 11,	//业务设置的帧率
	STREAM_CONFIG_KEY_RESOLUTION_WIDTH			= 12,
	STREAM_CONFIG_KEY_RESOLUTION_HEIGHT			= 13,
	STREAM_CONFIG_KEY_STREAM_LINE				= 14,	//线路
	STREAM_CONFIG_KEY_STREAM_LINE_GROUP			= 15,	//模板请求线路信息时需要指定
	STREAM_CONFIG_KEY_STREAM_A_V_PAIR_ID		= 16,	//音频和视频配对的pairId
};

//返回给模板的线路信息的来源类型
enum LiveResLineInfoType
{
	FROM_SUBSCRIBE_INVAILD_LINE = -1,	//在已经订阅了线路时 业务仍然订阅无效线路-1
	FROM_INQUIRY_RES = 0,				//查询线路请求的res中的线路信息返回
	FROM_SERVER_NOTIFY = 1,				//server主动通知的线路信息
	FROM_FETCH_AVP_RES = 2,				//获取前端地址的res中返回的线路信息
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
	std::map<unsigned, unsigned> configs;	//key:见enum LiveStreamCfgKey
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
	std::map<unsigned, unsigned> configs;	//key:见enum LiveStreamCfgKey
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

enum LivePublishStatus	//开播状态通知
{
	LIVE_PUBLISH_SUCCESS		= 0,		//发布成功
	LIVE_PUBLISH_PARAM_ERROR	= 1,		//参数错误
	LIVE_PUBLISH_FAILED			= 2, 		//发布失败
	LIVE_STATUS_AUTH_FAIL		= 3,		//鉴权失败
	LIVE_PUBLISH_STOP			= 4,		//发布停止
	LIVE_PUBLISH_NOT_ANCHOR_SYS	= 5,		//不在主播系统,不允许开播
};

enum LiveSubcribeStatus	//订阅状态通知
{
	LIVE_SUBCRIBE_SUCCESS		= 0,		//订阅成功
	LIVE_SUBCRIBE_PARAM_ERROR	= 1,		//参数错误
	LIVE_SUBCRIBE_FAILED		= 2,		//订阅失败
	LIVE_SUBCRIBE_CANCEL		= 3,		//订阅取消
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
	const wchar_t* (*showdtext)(void *obj, wchar_t *textbuf, int maxlen); //maxlen最大文字的个数，与bufsize的关系是：bufsize = maxlen * sizeof(wchar_t)

	int yuv_width;
	int yuv_height;

	unsigned int pts;

	unsigned char reserved[128];

	VideoRenderInfo() {
		memset(this, 0, sizeof(*this));
	}
};

// (1) 如果在模板单进程版本中，renderDelay填-1，同时renderStamp填该帧的渲染时刻(timeGetTime)
// (2) 如果在模板多进程版本中，renderDelay填实际渲染耗时, renderStamp可忽略
// 注：模板单进程版本，即接收vsdk帧和渲染帧在同一个进程中
// 注：两种情况下 renderStamp都是正常赋值的，已和模板确定
struct FrameRenderInfo
{
	unsigned renderDelay;	// 渲染耗时;在模板多进程版本中, 模板自己算出个渲染耗时
							//			 在模板单进程版本中, 该值填-1	
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