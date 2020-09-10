#pragma once
#define VIDEO_SDK_API __declspec(dllexport)

#define VIDEO_SDK_VERSION 100017

#include "videoEnumDef.h"
#include <map>
#include <set>
#include <vector>

EXTERN_C
{

typedef void (*YUV_CALLBACK_FUNCTION)(char * pBuf, int len, int width, int height);

namespace VideoSdk
{

struct IVsdkEventCallback
{
	virtual ~IVsdkEventCallback() {}
	virtual void onVsdkEvent(ETVsdkEvent* ev) = 0;
};

//
//外部通知接口，由调用程序实现 [callback]
//
struct IPerformanceWarningReceiver

{
	virtual ~IPerformanceWarningReceiver() {}
	//上行丢包警告 上行丢包率大于5%时，将触发此警告
	//param isWarning  [in] true:报警 false:提示警报消除
	//param lossRate   [in] 上行丢包率 单位为千分之一
	virtual void onLinkLossWarning(bool isWarning,unsigned lossRate) = 0;

	//CPU过载警告 当编码时间超过设定的每帧播放时间时，将触发此警告
	//param isWarning     [in] true:报警 false:提示警报消除
	//param avgCodeTime   [in] 平均每帧编码时间 单位为毫秒
	virtual void onCPUOverLoadWarning(bool isWarning,unsigned avgCodeTime) =0;
};

//
//外部通知接口，由调用程序实现 [callback]
//
struct IH264RawDataTranscodeStatusReceiver
{
	virtual ~IH264RawDataTranscodeStatusReceiver() {}
	//主播转发/转码模式切换通知（h264裸数据传输模式时，若原始数据码率过大，可能会引发二次转码，届时通过此接口通知）
	//param isTranscoding  [in] true:二次转码模式 false:直接转发模式
	virtual void onTranscodeStatusChange(bool isTranscoding) = 0;
};

//
//外部通知接口，由调用程序实现 [callback]
//
struct IVideoStreamInputTypeStatusReceiver
{
	virtual ~IVideoStreamInputTypeStatusReceiver() {}
	//视频流转发/转码等模式切换通知
	//param uid [in] 当前流主播uid
	//param inputTypeStatus  [in]  1:二次转码模式  0:直接转发模式  other:暂无定义，待扩展
	virtual void onTranscodeStatus(unsigned uid, unsigned inputTypeStatus) = 0;
};
///

//
//外部通知接口，由调用程序实现 [callback]
//
struct IBitmapSourceReleaseReceiver
{
	virtual ~IBitmapSourceReleaseReceiver() {}
	//内存资源释放通知
	//param rcHandle  [in] 释放的资源句柄
	virtual void onBitmapReSourceRelease(unsigned rcHandle) = 0;
};
//
//外部通知接口，由调用程序实现 [callback]
//
struct IRawVideoStreamDataReceiver
{
	virtual ~IRawVideoStreamDataReceiver() {}
	//新AVCDecoderConfigurationRecord到来通知
	virtual void OnNewAVCDecoderConfigurationRecord(unsigned uid,const unsigned char* pAVCDecoderConfigurationRecord, unsigned len) = 0;
	virtual void OnFrameData(unsigned uid,const unsigned char* pFramedata, unsigned len, unsigned pts, unsigned dts, bool isKeyFrame) = 0;
};
//
//外部通知接口，由调用程序实现 [callback]
//
struct IVideoRenderCallback
{
	virtual ~IVideoRenderCallback() {}

	//DX渲染状态
	//param hWnd   [in] 渲染对象绑定的窗口句柄，用于区分不同的渲染对象
	//param status [in] 状态值
	//param width  [in] 窗口宽度
	//param height [in] 窗口高度
	//param format [in] 渲染使用的像素格式：0-NONE, 1-YV12, 2-I420, 3-RGB
	virtual void onStatus(HWND hWnd, RenderStatus status, int width, int height, int format) = 0;

	//渲染前处理
	//param hWnd   [in] 渲染对象绑定的窗口句柄，用于区分不同的渲染对象
	//param data   [in] 视频数据
	//param linesize [in] 每行字节数
	//param width  [in] 视频宽度
	//param height [in] 视频高度
	//param format [in] 渲染使用的像素格式：0-NONE, 1-YV12, 2-I420, 3-RGB
	//返回值 约定：默认返回0，当应用层不能处理YUV就返回-1，此后SDK将强制使用RGB模式
	virtual int onRenderBefore(HWND hWnd, unsigned char *data[4], int linesize[4], int width, int height, int format) = 0;

	//DX渲染时回调给应用层绘制一些业务元素
	//param hWnd   [in] 渲染对象绑定的窗口句柄，用于区分不同的渲染对象
	//param hDC    [in] 应用层绘制时的目标DC，绘制完毕不要ReleaseDC
	//param width  [in] 绘制区域的宽度
	//param height [in] 绘制区域的高度
	virtual void onRenderPaint(HWND hWnd, HDC hDC, int width, int height) = 0;
};


//外部通知接口，由调用程序实现 [callback]
//
struct IVideoWatcher
{
	virtual ~IVideoWatcher() {}

	//播放状态改变（用于多人视频）
	//param uid          [in] 发生改变的uid
	//param data         [in] 视频位图数据
	//param length       [in] 数据长度
	//param bitmapHeader [in] 位图信息头
	virtual void onPlay(const unsigned uid, const unsigned char* data, unsigned length,const BITMAPINFOHEADER *bitmapHeader) = 0;

	//摄像头状态通知
	//param status [in] 摄像头状态，定义在enum CameraStatus
		//case CAMERA_READY：                摄像头就绪
		//case CAMERA_NOT_READY：            摄像头暂不可用（被别的程序占用）
		//case CAMERA_NOT_SUPPORT：          摄像头无法支持
	virtual void onCameraStatus(unsigned status) = 0;

	//返回可用的摄像头的id，如果没有可用的，index等于-1
	virtual void onAvaliableCamera(unsigned index) = 0;
	
	//主播时，状态通知
	//param status [in] 主播状态，定义在enum PublishStatus
		//case PUBLISH_SUCCESS:		成功上传视频
		//case PUBLISH_FAILED:		上传视频失败
		//case PUBLISHING_ERROR:	上传视频成功后，发送视频数据过程中，出现错误
	virtual void onPublishStatus(unsigned status) = 0;

	//订阅视频时，状态通知（用于多人视频）
		//param uid    [in] 当前流主播uid
		//param status [in] 订阅状态，定义在enum SubcribeStatus
		//case SUBSCRIBE_SUCCESS:	成功订阅视频
		//case SUBCRIBE_FAILED:		订阅视频失败
		//case SUBCRIBEING_ERROR:	点阅视频成功后，接收视频数据过程中，出现错误
	virtual void onSubcribeStatus(unsigned uid, unsigned status) = 0;

	//当前视频流状态通知（用于多人视频）
	//param uid    [in] 当前流主播uid
	//param status [in] 当前视频流状态，定义在enum StreamStatus
		//case START_STREAM:		其他用户正在主播视频，当收到此通知时，可以开始订阅视频
		//case STOP_STREAM:			其他用户结束视频主播，当收到次通知时，需要结束视频订阅
	virtual void onStreamStatus(unsigned uid, unsigned status) = 0;

	//麦序改变通知。如果插件自己有维护麦序，可以忽略此消息通知
	//param bTop [in] 自己是否处在允许直播的麦序上，true：是，false：否.
	//当自己处在允许直播的麦序上时，可以开始主播视频。但发现自己已离开允许直播的麦序，而且正在主播时，需要结束主播
	virtual void onMyTopQueue(bool bTop) = 0;

	//要求数据转发通知
	//VideoSdk通过该接口将数据透传至YY的Protocol模块，例子请参阅test/sdkLoader/YYCallBack.cpp中的YYCallBack::sendVideoAppData函数
	//param data [in] 需发送的数据地址
	//param len  [in] 需发送的数据长度
	virtual void sendVideoAppData(const char* data, unsigned len) = 0;

	//录制状态通知
	//param status [in] 录制状态，定义在enum RecordStatus
		//case START_RECORD:	录制启动成功
		//case STOP_RECORD:		停止录制
		//case ERROR_RECORD:	录制中出现错误
    virtual void onRecordStatus(unsigned status) = 0;

	//录制进度通知
	//param sec   [in] 已录制时长(秒)
    //param bytes [in] 已录制字节
    virtual void onRecordProgress(unsigned int sec, __int64 bytes) = 0;

	//麦序列表通知
		//param uids [in] 当前的麦序列表
	virtual void onGetVideoUidsFromYY(std::vector<unsigned>& uids) = 0;

	//videoSdk跟YY握手完成通知
	virtual void onYYConnected() = 0;

	//多码率档位通知
	//param multiCodeRateLevelsVec[in] 多码率档位
	virtual void onRecvMultiCodeRateLevels(const std::vector<unsigned>& multiCodeRateLevelsVec) = 0;

};

typedef struct _tagLogoInfo
{
	unsigned uid;

	int posX;
	int posY;

	int nWidth;
	int nHeight;

	int nDataLen;
	unsigned char* pBmpData;
	
	_tagLogoInfo()
	{
		clear();
	}

	void clear()
	{
		uid = 0;
		posY = 0;
		posY = 0;
		nWidth = 0;
		nHeight = 0;

		nDataLen = 0;
		
		pBmpData = NULL;
	}
	
}LogoInfo;
typedef std::vector<_tagLogoInfo> VEC_LOGOINFO;
typedef VEC_LOGOINFO::iterator VEC_LOGOINFO_ITERATOR;


//IYYVideo 视频sdk主要api接口类，调用Init后通过IYYVideo* GetInstance()函数获得实例指针
struct IYYVideo
{
	virtual ~IYYVideo() {}
	
	//注册IVideoWatcher
	//param VideoWatcher [in] 回调类指针
	virtual void Register(IVideoWatcher* VideoWatcher) = 0;

	//反注册IVideoWatcher
	//param VideoWatcher [in] 回调类指针
	virtual void Unregister(IVideoWatcher* VideoWatcher) = 0;

	//注册IPerformanceWarningReceiver
	//param warningReceiver [in] 回调类指针
	virtual void RegisterPerformanceWarningReceiver(IPerformanceWarningReceiver* warningReceiver) = 0;

	//反注册IPerformanceWarningReceiver
	//param warningReceiver [in] 回调类指针
	virtual void UnregisterPerformanceWarningReceiver(IPerformanceWarningReceiver* warningReceiver) = 0;

	//调用用来使得videoSdk跟YY开始握手。调用改接口时，需要两个前提
	//1. YY跟插件的数据透传通道已经建立；2.作为数据透传代理的IVideoWatcher已经注册到IYYVideo
	//调用这个接口之后，会收到sdk的onYYConnected（）回调，说明videoSdk跟YY握手完成。
	virtual void connect() = 0;

	//查询是否位于首位麦序
	virtual bool IsTopQueue() const = 0;

	//桌面分享专用接口。查询是否正在主播视频
	virtual bool IsPublishing() const = 0;

	//获取摄像头列表
	//param camList [out] 摄像头列表指针
	//return 摄像头数量
	/*
	例子：
	const char **list=NULL;
	int camNum = GetInstance()->GetCameraList(&list);
	for (int i=0;i<camNUM;++i)
	{
		cout<<list[i]<<endl;
	}
	//所分配内存将在UnInit被调用时释放
	*/
	virtual int GetCameraList(const char ***camList) = 0;

	//是否设置YY虚拟视频为优先虚拟视频
	virtual void SetPreferToYYVcam(bool bPreferToYYVcam = true) = 0;

	//设置所用的摄像头
	//param number [in] 指定摄像头的编号
	//调用后，将有异步的IVideoWatcher::onCameraStatus通知
	virtual void SetCameraNumber(unsigned number) = 0;

	//设置可用的摄像头
	//调用后，将有异步的IVideoWatcher::onAvaliableCamera通知
	virtual void SetAvaliableCamera() = 0;
	
	//释放对本地摄像头占用
	virtual void ReleaseCurrentCamera() = 0;
	
	//上传视频数据
	virtual void StartPublishVideo() = 0;
	
	//结束视频上传
	virtual void StopPublishVideo() = 0;

	//开始订阅视频（用于多人视频）
	    //param uid [in] 主播uid
	virtual void StartSubscribeVideo(unsigned uid) = 0;

	//结束订阅视频（用于多人视频）
		//param uid [in] 主播uid
	virtual void StopSubscribeVideo(unsigned uid) = 0;
	
	//开始桌面采集.设置位图模式，指定分辨率（设定后，sdk自身的摄像头捕捉功能被屏蔽）
	virtual bool StartDesktopCapture(int width, int height, D3DFMT fmt) = 0;
	
	//填充待压缩的位图数据
	virtual void setBitmapBits(unsigned char *buffer, unsigned len) = 0;

	//结束桌面采集
	virtual void StopDesktopCapture() = 0;
	
	//启动视频录制
	virtual void StartVdieoRecord(const char *szfilename) = 0;
	
	//结束视频录制
	virtual void StopVideoRecord() = 0;
	
	//弹出视频属性设置框，设置视频参数。要调用此函数，得先调用SetCameraNumber。
	//如果PopVideoConfigDlg没有调用，则使用默认参数配置
	virtual bool PopVideoConfigDlg(unsigned parentHwnd) = 0;
	
	//根据统计项Id，获取视频质量统计数据
	//param key [in] 统计项Id，定义在enum StaticsPropsId里面 
	virtual unsigned GetStaticsProps(unsigned key) = 0;

	//YY的protocol得到透传数据后，请调用此函数转发至videosdk.dll
	//例子请参阅test/sdkLoader/YYCallBack.cpp中的YYCallBack::OnRecv函数
	//param data [in] 从应用服务器得到的数据地址
	//param len [in] 数据长度
	virtual void recvVideoAppData(const char* data, unsigned len) = 0;

	//获取当前publisher的BITMAPINFOHEADER, 成功则返回true, 失败则返回false
	virtual bool getBmpInfoHead(unsigned publisherUid, BITMAPINFOHEADER& bih) = 0;

	//改变输出画面大小（设置后生效，直到主播端改变源分辨率为止）
		//param uid     [in] 要改变的流的主播uid
		//param width   [in] 期待的宽度
		//param height  [in] 期待的高度
	//此接口已被标记为【废弃】，如需缩放视频，请使用下面的高性能缩放接口：ConvertVideo
	virtual void ResetFrameSize(unsigned uid, int width,int height) =0;

	//设置水印
	//暂时只支持RGB32格式数据(顺序为BGRA)
	//param pBmpData [in] 水印图片RGB数据(带alpha通道)
	//param nDataLen [in] 图片数据长度(必须等于nWidth*nHeight*4)
	//param nWidth   [in] 水印宽度
	//param nHeight  [in] 水印高度
	virtual void SetLogoBmp(unsigned char* pBmpData, int nDataLen, int nWidth, int nHeight) = 0;

	//设置编码搜索区域档次
	//param level [in] 搜索档次，范围在0-8
	virtual void setCodeSearchRangeLevel(unsigned level) = 0;

	//param configMap[in] 参数设置列表, 可用的key请参照enum PluginConfig 
	//请确保在得到IVideoWatcher::onYYConnected回调后再执行此操作，否则设置可能不成功
	virtual void setPluginConfig(const std::map<unsigned char, unsigned>& configMap) = 0;

	//获取音频波形数据
	virtual void getAudioWaveData(std::vector<unsigned>& waveData) = 0;

	//设置主播状态，打开摄像头前设置isPublisher = true用来预览画面；结束主播后，设置isPublisher = false
	virtual void setPublishStatus(bool isPublisher) = 0;

	//获取主播uid集合。如果返回uidsSet为空，表示此时没有人主播
	virtual void getPublisherUids(std::set<unsigned>& uidsSet) = 0;

	//获取指定主播的流属性, attributeMap的key参见enum StreamAttribute
	virtual void getStreamAttributes(unsigned publisherUid, std::map<unsigned char, unsigned>& attributeMap) = 0;

	//由主播方设置的视频增强参数
	//param isSupportEnhance [in] 是否打开视频增强功能
	//param noise [in] 范围为1-100,设置去噪值，越大视频越平滑
	//param sharp [in] 范围为1-100,设置视频清晰度,越大视频越清晰
	virtual void setVideoEnhanceConfig(bool isOpenVideoEnhance, unsigned noise, unsigned sharp) = 0;

	//重绘播放窗口
	virtual void Paint() = 0;

	//设定播放窗口句柄。日期2016-09-22后可能会停止维护这个接口。请用setRenderHwndMap代替
		//param playWindowHandle [in] 播放窗口句柄(请传入有效窗口句柄或NULL，若为NULL则sdk不负责渲染视频)
	virtual void setPlayHwnd(std::vector<HWND> &playWindowHandle) = 0;

	//获取主播从开播到现在所经过的时长
		//param publisherUid[in] 要获取播放时长的主播uid
		//return 主播从开播到当前所经过时长，单位为ms；如果返回-1，则表示当前主播没有开播
	virtual unsigned getPublishTime(unsigned publisherUid) = 0;

	//开始服务器视频录制(旧接口，新开发的请用startServerRecord(std::set<unsigned>& recordUidSet, std::string& programId)这个接口)
		//param programId[in] 插件生成的节目id
	virtual void startServerRecord(std::string programId) = 0;


	//结束服务器视频录制
	virtual void stopServerRecord() = 0;

	//设置水平翻转播放画面 若传入uid非直播中，则返回false
	virtual bool setHorizintalFlip(const unsigned uid, const bool isFlip) = 0;

	//获知某主播画面是否处于水平翻转状态 若传入uid非直播中，则返回false
	virtual bool getHorizintalFlip(const unsigned uid, bool &isFlip) = 0;

	//让设置主播额外的meta信息
	virtual void setExtraMetaData(const std::map<unsigned char, unsigned>& metas) = 0;

	//获取主播额外的meta信息
		// param uid[in].要获取的主播uid
		// param metas[out].返回的该主播的meta信息
	virtual void getExtraMetaData(unsigned uid, std::map<unsigned char, unsigned>& metas) = 0;


	//获知用户在频道内看到视频的实际时间，不是在频道内的时间
	//return 用户在当前频道内实际收看到视频的时间，单位为S;如果返回-1，则表示当前没有视频播放。
	virtual unsigned getViewerTime() = 0;

	//开始服务器视频录制
	//param recordUidSet[in] 要录制的主播uid集(目前暂不支持同时填多个主播uid)
	//param programId[in] 插件生成的节目id
	virtual void startServerRecord(std::set<unsigned>& recordUidSet, std::string& programId) = 0;

	//开启h264裸数据传输模式
	//param AVCDecoderConfigurationRecord[in] AVCDecoderConfigurationRecord中包含h264解码必须的sps和pps信息
	//param len[in] AVCDecoderConfigurationRecord 长度
	//param bitRate[in] 预计输入码率（建议尽量输入正确码率，但不强求精确）
	//param maxTolerateBitRate[in] 最大容忍码率（若实际输入裸数据码率高于此值，则vsdk执行二次编码，将实际码率维持在最大容忍码率范围之内）
	virtual bool StartRawH264VideoTransmit(unsigned char* AVCDecoderConfigurationRecord, unsigned len, unsigned bitRate, unsigned maxTolerateBitRate=1500000) = 0;

	//结束h264裸数据传输模式
	virtual void StopRawH264VideoTransmit() = 0;

	//同步采集时间戳
	virtual void syncCapTimeStamp(unsigned ptsBase/*now*/) = 0;

	//填充原始打包格式的H264帧数据
	virtual void setRawH264VideoData(unsigned char *rawH264VideoNalu, unsigned len, unsigned pts, unsigned dts, bool isKeyFrame) = 0;

	//获取直播时的streamId, 该接口应在收到VideoSdk::PUBLISH_SUCCESS回调后调用，否则该接口可能返回-1
	virtual unsigned long long getMyStreamId() = 0;

	//重设裸数据传输模式的最大容忍码率
	virtual void ResetmaxTolerateBitRate(unsigned maxTolerateBitRate) = 0;

	//注册IH264RawDataTranscodeStatusReceiver
	//param videoRawDataTranscodeStatusReceiver [in] 回调类指针
	virtual void RegisterH264RawDataTranscodeStatusReceiver(IH264RawDataTranscodeStatusReceiver* videoRawDataTranscodeStatusReceiver) = 0;

	//反注册IH264RawDataTranscodeStatusReceiver
	virtual void UnregisterH264RawDataTranscodeStatusReceiver() = 0;

	//注册IVideoStreamTypeChangeReceiver
	//param videoStreamTypeChangeReceiver [in] 回调类指针
	virtual void RegisterVideoStreamTypeChangeReceiver(IVideoStreamInputTypeStatusReceiver* videoStreamTypeChangeReceiver) = 0;

	//反注册IVideoStreamTypeChangeReceiver
	virtual void UnregisterVideoStreamTypeChangeReceiver() = 0;

	//根据主播uid获取相应的streamId, 当找不到相应的主播uid时，则返回-1
	//param publisherUid [in] 主播uid
	virtual unsigned long long getPublisherStreamId(unsigned publisherUid) = 0;

	//选择IBitmapSourceReleaseReceiver
	//return 旧的IBitmapSourceReleaseReceiver指针
	//param bitmapSourceReleaseReceiver [in] 回调类指针
	virtual IBitmapSourceReleaseReceiver *SelectBitmapSourceReleaseReceiver(IBitmapSourceReleaseReceiver* bitmapSourceReleaseReceiver) = 0;

	//设置填充位图数据,
	//传入的buffer将被vsdk占用，直到vsdk异步调用IBitmapSourceReleaseReceiver::onBitmapReSourceRelease（rcHandle）为止
	//return 此帧是否被vsdk处理
	        //如vsdk认为此时应该丢帧，将不会对传进的buffer进行处理，函数将直接返回false，并立即有异步的release回调
	        //返回true表明，vsdk将此帧加入编码前处理缓冲，当编码前处理完毕后，不需要占用buffer时，会有异步的release回调
	//param buffer   [in]  图片数据指针
	//param len      [in]  图片数据长度
	//param rcHandle [in]  资源句柄，vsdk回调用与本短内存对应的release时，会以本rcHandle作为参数
	virtual bool SetBitmapBitsInPlace(unsigned char *buffer, unsigned len, unsigned rcHandle) = 0;

	//选择IRawVideoStreamDataReceiver
	//return 旧的IRawVideoStreamDataReceiver指针
	//param IRawVideoStreamDataReceiver [in] 回调类指针
	virtual IRawVideoStreamDataReceiver *SelectVideoStreamTypeChangeReceiver(IRawVideoStreamDataReceiver* pRawVideoStreamDataReceiver) = 0;

	//根据uid来获取一些用字符表示的信息, userInfoMap的key参见enum UserStrInfo
	virtual void getUserStrInfo(unsigned publisherUid, std::map<unsigned char, std::string>& userInfoMap) = 0;


	//**********************************************************************************************************************************************
	//新接口, 支持一个主播上传多个流
	//**********************************************************************************************************************************************
	//事件回调接口，具体事件请看VsdkEvent.h
	virtual void setCallback(IVsdkEventCallback* cb) = 0;
	//Vsdk调用接口，具体方法请看VsdkInvokerEvent.h
	virtual void invoke(ETVsdkInvoke* evt) = 0;


	//设置垂直翻转画面, 若传入uid非直播中，则返回false
	virtual bool setVerticalFlip(const unsigned uid, const bool isFlip) = 0;

	//获知某主播画面是否处于垂直翻转状态 若传入uid非直播中，则返回false
	virtual bool getVerticalFlip(const unsigned uid, bool &isFlip) = 0;

	//设置填充位图数据,
	//传入的buffer将被vsdk占用，直到vsdk异步调用IBitmapSourceReleaseReceiver::onBitmapReSourceRelease（rcHandle）为止
	//return 此帧是否被vsdk处理
	//如vsdk认为此时应该丢帧，将不会对传进的buffer进行处理，函数将直接返回false，并立即有异步的release回调
	//返回true表明，vsdk将此帧加入编码前处理缓冲，当编码前处理完毕后，不需要占用buffer时，会有异步的release回调
	//param buffer   [in]  图片数据指针
	//param len      [in]  图片数据长度
	//param rcHandle [in]  资源句柄，vsdk回调用与本短内存对应的release时，会以本rcHandle作为参数
	//param captrueTimestamp [in]  采集时间戳，请传入开机至采集时刻所经历的毫秒数
	virtual bool SetBitmapBitsInPlace(unsigned char *buffer, unsigned len, unsigned rcHandle , unsigned captrueTimestamp) = 0;

	//开始获取FLV视频流
		//param uid       [in] 主播uid
		//param publishId [in] 主播标识
		//param proxyType [in] 服务器标识, 见enum FlvOverHttpProxyType
		//param url       [in] 用于获取流的url
	virtual void StartFlvOverHttpVideo(unsigned uid, unsigned publishId, unsigned proxyType, unsigned codeRate, std::string& url) = 0;


	//结束获取FLV视频流
		//param uid       [in] 主播uid
		//param publishId [in] 主播标识
	virtual void StopFlvOverHttpVideo(unsigned uid, unsigned publishId) = 0;

	//业务方开始使用rtmp上行音视频
	//param cdn       [in] Key为CDN地址，Value为鉴权码
	//param  start    [in] 推流or断流，1-开始, 0-断开;
	virtual void NotifyRtmpStream(std::map<std::string, std::string>& cdn, unsigned start) = 0;

	//**********************************************************************************************************************************************
	//新接口, 支持主播使用Rtmp上行音视频
	//**********************************************************************************************************************************************
	//开始使用rtmp上行音视频
		//param proxyType [in] rtmp服务器来源，见enum RTMPServer
		//param url       [in] 用于推rtmp流的服务器url
		//param playPath  [in] 用于推rtmp流的playPath
	virtual void StartRtmpPublish(unsigned proxyType, std::string& url, std::string& playPath) = 0;
	//结束rtmp开播
	virtual void StopRtmpPublish() = 0;

    //开启h265裸数据传输模式
    //param HEVCDecoderConfigurationRecord[in] AVCDecoderConfigurationRecord中包含h265解码必须的sps和pps信息
    //param len[in] AVCDecoderConfigurationRecord 长度
    //param bitRate[in] 预计输入码率（建议尽量输入正确码率，但不强求精确）
    //param maxTolerateBitRate[in] 最大容忍码率（若实际输入裸数据码率高于此值，则vsdk执行二次编码，将实际码率维持在最大容忍码率范围之内）
    virtual bool StartRawH265VideoTransmit(unsigned char* HEVCDecoderConfigurationRecord, unsigned len, unsigned bitRate, unsigned maxTolerateBitRate=1500000) = 0;

    //结束h264裸数据传输模式
    virtual void StopRawH265VideoTransmit() = 0;

    //填充原始打包格式的H264帧数据
	//parama frameType 暂时和下面的 enum 值对应：
	//                enum ENCODED_FRAME_TYPE
	//                {
	//	                  ERR_Frame = 0,
	//           		  P_Frame = 1,
	//	                  I_Frame = 2,
	//		              IDR_Frame = 3,
	//		              B_Frame = 4,
	//                };
	virtual void setRawH265VideoData(unsigned char *rawH264VideoNalu, unsigned len, unsigned pts, unsigned dts, int frameType) = 0;

	//业务方解码音视频时，每秒由模板通知vsdk上层解码音视频情况(不再使用)
		//param publisherUid               [in] 音视频所属主播uid
		//param publishId                  [in] 音视频所属主播publishId
		//param audioFrameIdToPlayStamp    [in] key：音频帧序号 value：TimeGetTime获取到播放时间戳
		//param videoFrameIdToRenderStamp  [in] key：视频频帧序号 value：TimeGetTime获取到渲染时间戳
	virtual void setPluginDecodeAudioVideoStat(unsigned publisherUid, unsigned publishId, const std::map<unsigned, unsigned>& audioFrameIdToPlayStamp,
		std::map<unsigned, unsigned>& videoFrameIdToRenderStamp) = 0;

	//业务方解码视频时，渲染前设置视频数据给vsdk，用于反低俗(不再使用)
		//param publisherUid               [in] 视频所属主播uid
		//param publishId                  [in] 视频所属主播publishId
		//param data                       [in] 图片数据
		//param bitmapHeader               [in] 位图文件头
	virtual void setPluginPlayVideoFrame(unsigned publisherUid, unsigned publishId, const unsigned char* data, const BITMAPINFOHEADER &bitmapHeader) = 0;

	//填充待压缩的位图数据
	virtual void setBitmapBitsWithTimestamp(unsigned char *buffer, unsigned len, unsigned captureTime) = 0;

	//设置水印
	//暂时只支持RGB32格式数据(顺序为BGRA)
	//param pBmpData [in] 水印图片RGB数据(带alpha通道)
	//param nDataLen [in] 图片数据长度(必须等于nWidth*nHeight*4)
	//param nWidth   [in] 水印宽度
	//param nHeight  [in] 水印高度
	//param x        [in] 水印图片右上角与视频右上角的横坐标差值
	//param y        [in] 水印图片右上角与视频右上角的纵坐标差值
	virtual void SetLogoBmpWithPosition(unsigned char* pBmpData, int nDataLen, int nWidth, int nHeight, int posX, int posY) = 0;

	//param configMap[in] 参数设置列表, 可用的key请参照enum PluginConfig 
	//请确保在得到IVideoWatcher::onYYConnected回调后再执行此操作，否则设置可能不成功
	//用于传递字符串参数
	virtual void setPluginConfigString(const std::map<unsigned char, const char*>& configMap) = 0;

	//编码前YUV数据回调
	virtual void setEncodeYuvCallback(YUV_CALLBACK_FUNCTION pCallbackFunction) = 0;

	//设置指定窗口的缩放模式，此接口不能在setPlayHwnd或setRenderHwndMap之前调用，可以播放时按需实时调用
	//param hWnd [in] 必须是setPlayHwnd或setRenderHwndMap接口设置过的，否则调用会被忽略
	//param mode [in] 缩放模式：0-填满目标窗口，不留黑边，裁掉超出的；1-在目标窗口内部做自适应，留黑边；2-仅进行DX兼容性检测，不绘制
	virtual void setVideoRenderScaleMode(HWND hWnd, int mode) = 0;

	//设置渲染回调
	virtual void setVideoRenderCallback(IVideoRenderCallback *cb) = 0;

	//设置UID和渲染窗口句柄的绑定关系。这个接口不能和setPlayHwnd同时使用
	virtual void setRenderHwndMap(std::map<unsigned, HWND>& hwndMap) = 0;

	//视频格式转换。退出程序时可传入NULL则清空内部上下文。当格式信息(宽高或格式)发生变化时需先执行传入NULL清空原有上下文。
	/*  一个简单的例子代码
		dst_buffer = malloc(dst_width * dst_height * 3); //分配一次后可以重复使用，目标分辨率改变时释放后重新分配

		VideoConvertParam param;

		param.srcW = src_width;
		param.srcH = src_height;
		param.srcFormat = eAV_PIX_FMT_BGR24;
		param.srcStride[0] = src_width * 3;
		param.src[0] = src_buffer;

		param.dstW = dst_width;
		param.dstH = dst_height;
		param.dstFormat = eAV_PIX_FMT_BGR24;
		param.dstStride[0] = dst_width * 3;
		param.dst[0] = dst_buffer;

		ConvertVideo(&param);

		缩放之后的数据就在 dst_buffer 里面
	*/
	virtual bool ConvertVideo(VideoConvertParam *vcp) = 0;

	//业务"贴纸"功能回调
	//开播前 setPluginConfig 设置CAMERA_DATA_CALLBACK标记
	//开播后 sdk会 在 virtual void onVsdkEvent(ETVsdkEvent* ev); 回调里面的 ET_CAMERA_DATA 返回摄像头数据给业务
	//业务处理完成后,把视频数据塞入sdk编码即可
	//format 目前只支持eAV_PIX_FMT_RGBA 和eAV_PIX_FMT_RGB24
	virtual void cameraDataCallback( unsigned char* buffer, unsigned bufLen,int w,int h, unsigned mediaTimeStamp, unsigned pushTimeStamp,bool hasPreprocessed,VideoPixelFormat format ) = 0;

	//设置水印【已废弃。请使用ETSetWaterMarkList】
	//暂时只支持RGB32格式数据(顺序为BGRA)
	//param pBmpData [in] 水印图片RGB数据(带alpha通道)
	//param nDataLen [in] 图片数据长度(必须等于nWidth*nHeight*4)
	//param nWidth   [in] 水印宽度
	//param nHeight  [in] 水印高度
	//param x        [in] 水印图片右上角与视频右上角的横坐标差值
	//param y        [in] 水印图片右上角与视频右上角的纵坐标差值
	virtual void SetLogoBmpWithPositionFromVector( VEC_LOGOINFO& vecLogoInfo ) = 0;

	//重要：为保持二进制兼容性，不要在这个接口定义中修改或添加新的接口。新接口全部通过ETVsdkInvoke继承定义 @冯布洲

};// end of IYYVideo

};// end of nameSpace


//导出函数
//初始化dll
//param codeRate [in] 支持普清跟高清两种码率，具体值定义在enum VideoCodeRate。
//	case LOW_DEFINITION  表示普清码率。
//	case HIGH_DEFINITION 表示高清码率。
//param sdkVer [in] 插件所用的videoSdk版本号。如果传入的sdkVer跟宏VIDEO_SDK_VERSION不相等，返回false
//初始化成功，返回true，否则返回false
VIDEO_SDK_API bool InitVideoService(unsigned codeRate, unsigned sdkVer);

//反初始化dll
VIDEO_SDK_API void UnInitVideoService();

//获取IYYVideo指针
VIDEO_SDK_API VideoSdk::IYYVideo* GetInstance();

};