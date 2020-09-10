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
//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IPerformanceWarningReceiver

{
	virtual ~IPerformanceWarningReceiver() {}
	//���ж������� ���ж����ʴ���5%ʱ���������˾���
	//param isWarning  [in] true:���� false:��ʾ��������
	//param lossRate   [in] ���ж����� ��λΪǧ��֮һ
	virtual void onLinkLossWarning(bool isWarning,unsigned lossRate) = 0;

	//CPU���ؾ��� ������ʱ�䳬���趨��ÿ֡����ʱ��ʱ���������˾���
	//param isWarning     [in] true:���� false:��ʾ��������
	//param avgCodeTime   [in] ƽ��ÿ֡����ʱ�� ��λΪ����
	virtual void onCPUOverLoadWarning(bool isWarning,unsigned avgCodeTime) =0;
};

//
//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IH264RawDataTranscodeStatusReceiver
{
	virtual ~IH264RawDataTranscodeStatusReceiver() {}
	//����ת��/ת��ģʽ�л�֪ͨ��h264�����ݴ���ģʽʱ����ԭʼ�������ʹ��󣬿��ܻ���������ת�룬��ʱͨ���˽ӿ�֪ͨ��
	//param isTranscoding  [in] true:����ת��ģʽ false:ֱ��ת��ģʽ
	virtual void onTranscodeStatusChange(bool isTranscoding) = 0;
};

//
//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IVideoStreamInputTypeStatusReceiver
{
	virtual ~IVideoStreamInputTypeStatusReceiver() {}
	//��Ƶ��ת��/ת���ģʽ�л�֪ͨ
	//param uid [in] ��ǰ������uid
	//param inputTypeStatus  [in]  1:����ת��ģʽ  0:ֱ��ת��ģʽ  other:���޶��壬����չ
	virtual void onTranscodeStatus(unsigned uid, unsigned inputTypeStatus) = 0;
};
///

//
//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IBitmapSourceReleaseReceiver
{
	virtual ~IBitmapSourceReleaseReceiver() {}
	//�ڴ���Դ�ͷ�֪ͨ
	//param rcHandle  [in] �ͷŵ���Դ���
	virtual void onBitmapReSourceRelease(unsigned rcHandle) = 0;
};
//
//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IRawVideoStreamDataReceiver
{
	virtual ~IRawVideoStreamDataReceiver() {}
	//��AVCDecoderConfigurationRecord����֪ͨ
	virtual void OnNewAVCDecoderConfigurationRecord(unsigned uid,const unsigned char* pAVCDecoderConfigurationRecord, unsigned len) = 0;
	virtual void OnFrameData(unsigned uid,const unsigned char* pFramedata, unsigned len, unsigned pts, unsigned dts, bool isKeyFrame) = 0;
};
//
//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IVideoRenderCallback
{
	virtual ~IVideoRenderCallback() {}

	//DX��Ⱦ״̬
	//param hWnd   [in] ��Ⱦ����󶨵Ĵ��ھ�����������ֲ�ͬ����Ⱦ����
	//param status [in] ״ֵ̬
	//param width  [in] ���ڿ��
	//param height [in] ���ڸ߶�
	//param format [in] ��Ⱦʹ�õ����ظ�ʽ��0-NONE, 1-YV12, 2-I420, 3-RGB
	virtual void onStatus(HWND hWnd, RenderStatus status, int width, int height, int format) = 0;

	//��Ⱦǰ����
	//param hWnd   [in] ��Ⱦ����󶨵Ĵ��ھ�����������ֲ�ͬ����Ⱦ����
	//param data   [in] ��Ƶ����
	//param linesize [in] ÿ���ֽ���
	//param width  [in] ��Ƶ���
	//param height [in] ��Ƶ�߶�
	//param format [in] ��Ⱦʹ�õ����ظ�ʽ��0-NONE, 1-YV12, 2-I420, 3-RGB
	//����ֵ Լ����Ĭ�Ϸ���0����Ӧ�ò㲻�ܴ���YUV�ͷ���-1���˺�SDK��ǿ��ʹ��RGBģʽ
	virtual int onRenderBefore(HWND hWnd, unsigned char *data[4], int linesize[4], int width, int height, int format) = 0;

	//DX��Ⱦʱ�ص���Ӧ�ò����һЩҵ��Ԫ��
	//param hWnd   [in] ��Ⱦ����󶨵Ĵ��ھ�����������ֲ�ͬ����Ⱦ����
	//param hDC    [in] Ӧ�ò����ʱ��Ŀ��DC��������ϲ�ҪReleaseDC
	//param width  [in] ��������Ŀ��
	//param height [in] ��������ĸ߶�
	virtual void onRenderPaint(HWND hWnd, HDC hDC, int width, int height) = 0;
};


//�ⲿ֪ͨ�ӿڣ��ɵ��ó���ʵ�� [callback]
//
struct IVideoWatcher
{
	virtual ~IVideoWatcher() {}

	//����״̬�ı䣨���ڶ�����Ƶ��
	//param uid          [in] �����ı��uid
	//param data         [in] ��Ƶλͼ����
	//param length       [in] ���ݳ���
	//param bitmapHeader [in] λͼ��Ϣͷ
	virtual void onPlay(const unsigned uid, const unsigned char* data, unsigned length,const BITMAPINFOHEADER *bitmapHeader) = 0;

	//����ͷ״̬֪ͨ
	//param status [in] ����ͷ״̬��������enum CameraStatus
		//case CAMERA_READY��                ����ͷ����
		//case CAMERA_NOT_READY��            ����ͷ�ݲ����ã�����ĳ���ռ�ã�
		//case CAMERA_NOT_SUPPORT��          ����ͷ�޷�֧��
	virtual void onCameraStatus(unsigned status) = 0;

	//���ؿ��õ�����ͷ��id�����û�п��õģ�index����-1
	virtual void onAvaliableCamera(unsigned index) = 0;
	
	//����ʱ��״̬֪ͨ
	//param status [in] ����״̬��������enum PublishStatus
		//case PUBLISH_SUCCESS:		�ɹ��ϴ���Ƶ
		//case PUBLISH_FAILED:		�ϴ���Ƶʧ��
		//case PUBLISHING_ERROR:	�ϴ���Ƶ�ɹ��󣬷�����Ƶ���ݹ����У����ִ���
	virtual void onPublishStatus(unsigned status) = 0;

	//������Ƶʱ��״̬֪ͨ�����ڶ�����Ƶ��
		//param uid    [in] ��ǰ������uid
		//param status [in] ����״̬��������enum SubcribeStatus
		//case SUBSCRIBE_SUCCESS:	�ɹ�������Ƶ
		//case SUBCRIBE_FAILED:		������Ƶʧ��
		//case SUBCRIBEING_ERROR:	������Ƶ�ɹ��󣬽�����Ƶ���ݹ����У����ִ���
	virtual void onSubcribeStatus(unsigned uid, unsigned status) = 0;

	//��ǰ��Ƶ��״̬֪ͨ�����ڶ�����Ƶ��
	//param uid    [in] ��ǰ������uid
	//param status [in] ��ǰ��Ƶ��״̬��������enum StreamStatus
		//case START_STREAM:		�����û�����������Ƶ�����յ���֪ͨʱ�����Կ�ʼ������Ƶ
		//case STOP_STREAM:			�����û�������Ƶ���������յ���֪ͨʱ����Ҫ������Ƶ����
	virtual void onStreamStatus(unsigned uid, unsigned status) = 0;

	//����ı�֪ͨ���������Լ���ά�����򣬿��Ժ��Դ���Ϣ֪ͨ
	//param bTop [in] �Լ��Ƿ�������ֱ���������ϣ�true���ǣ�false����.
	//���Լ���������ֱ����������ʱ�����Կ�ʼ������Ƶ���������Լ����뿪����ֱ�������򣬶�����������ʱ����Ҫ��������
	virtual void onMyTopQueue(bool bTop) = 0;

	//Ҫ������ת��֪ͨ
	//VideoSdkͨ���ýӿڽ�����͸����YY��Protocolģ�飬���������test/sdkLoader/YYCallBack.cpp�е�YYCallBack::sendVideoAppData����
	//param data [in] �跢�͵����ݵ�ַ
	//param len  [in] �跢�͵����ݳ���
	virtual void sendVideoAppData(const char* data, unsigned len) = 0;

	//¼��״̬֪ͨ
	//param status [in] ¼��״̬��������enum RecordStatus
		//case START_RECORD:	¼�������ɹ�
		//case STOP_RECORD:		ֹͣ¼��
		//case ERROR_RECORD:	¼���г��ִ���
    virtual void onRecordStatus(unsigned status) = 0;

	//¼�ƽ���֪ͨ
	//param sec   [in] ��¼��ʱ��(��)
    //param bytes [in] ��¼���ֽ�
    virtual void onRecordProgress(unsigned int sec, __int64 bytes) = 0;

	//�����б�֪ͨ
		//param uids [in] ��ǰ�������б�
	virtual void onGetVideoUidsFromYY(std::vector<unsigned>& uids) = 0;

	//videoSdk��YY�������֪ͨ
	virtual void onYYConnected() = 0;

	//�����ʵ�λ֪ͨ
	//param multiCodeRateLevelsVec[in] �����ʵ�λ
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


//IYYVideo ��Ƶsdk��Ҫapi�ӿ��࣬����Init��ͨ��IYYVideo* GetInstance()�������ʵ��ָ��
struct IYYVideo
{
	virtual ~IYYVideo() {}
	
	//ע��IVideoWatcher
	//param VideoWatcher [in] �ص���ָ��
	virtual void Register(IVideoWatcher* VideoWatcher) = 0;

	//��ע��IVideoWatcher
	//param VideoWatcher [in] �ص���ָ��
	virtual void Unregister(IVideoWatcher* VideoWatcher) = 0;

	//ע��IPerformanceWarningReceiver
	//param warningReceiver [in] �ص���ָ��
	virtual void RegisterPerformanceWarningReceiver(IPerformanceWarningReceiver* warningReceiver) = 0;

	//��ע��IPerformanceWarningReceiver
	//param warningReceiver [in] �ص���ָ��
	virtual void UnregisterPerformanceWarningReceiver(IPerformanceWarningReceiver* warningReceiver) = 0;

	//��������ʹ��videoSdk��YY��ʼ���֡����øĽӿ�ʱ����Ҫ����ǰ��
	//1. YY�����������͸��ͨ���Ѿ�������2.��Ϊ����͸�������IVideoWatcher�Ѿ�ע�ᵽIYYVideo
	//��������ӿ�֮�󣬻��յ�sdk��onYYConnected�����ص���˵��videoSdk��YY������ɡ�
	virtual void connect() = 0;

	//��ѯ�Ƿ�λ����λ����
	virtual bool IsTopQueue() const = 0;

	//�������ר�ýӿڡ���ѯ�Ƿ�����������Ƶ
	virtual bool IsPublishing() const = 0;

	//��ȡ����ͷ�б�
	//param camList [out] ����ͷ�б�ָ��
	//return ����ͷ����
	/*
	���ӣ�
	const char **list=NULL;
	int camNum = GetInstance()->GetCameraList(&list);
	for (int i=0;i<camNUM;++i)
	{
		cout<<list[i]<<endl;
	}
	//�������ڴ潫��UnInit������ʱ�ͷ�
	*/
	virtual int GetCameraList(const char ***camList) = 0;

	//�Ƿ�����YY������ƵΪ����������Ƶ
	virtual void SetPreferToYYVcam(bool bPreferToYYVcam = true) = 0;

	//�������õ�����ͷ
	//param number [in] ָ������ͷ�ı��
	//���ú󣬽����첽��IVideoWatcher::onCameraStatus֪ͨ
	virtual void SetCameraNumber(unsigned number) = 0;

	//���ÿ��õ�����ͷ
	//���ú󣬽����첽��IVideoWatcher::onAvaliableCamera֪ͨ
	virtual void SetAvaliableCamera() = 0;
	
	//�ͷŶԱ�������ͷռ��
	virtual void ReleaseCurrentCamera() = 0;
	
	//�ϴ���Ƶ����
	virtual void StartPublishVideo() = 0;
	
	//������Ƶ�ϴ�
	virtual void StopPublishVideo() = 0;

	//��ʼ������Ƶ�����ڶ�����Ƶ��
	    //param uid [in] ����uid
	virtual void StartSubscribeVideo(unsigned uid) = 0;

	//����������Ƶ�����ڶ�����Ƶ��
		//param uid [in] ����uid
	virtual void StopSubscribeVideo(unsigned uid) = 0;
	
	//��ʼ����ɼ�.����λͼģʽ��ָ���ֱ��ʣ��趨��sdk���������ͷ��׽���ܱ����Σ�
	virtual bool StartDesktopCapture(int width, int height, D3DFMT fmt) = 0;
	
	//����ѹ����λͼ����
	virtual void setBitmapBits(unsigned char *buffer, unsigned len) = 0;

	//��������ɼ�
	virtual void StopDesktopCapture() = 0;
	
	//������Ƶ¼��
	virtual void StartVdieoRecord(const char *szfilename) = 0;
	
	//������Ƶ¼��
	virtual void StopVideoRecord() = 0;
	
	//������Ƶ�������ÿ�������Ƶ������Ҫ���ô˺��������ȵ���SetCameraNumber��
	//���PopVideoConfigDlgû�е��ã���ʹ��Ĭ�ϲ�������
	virtual bool PopVideoConfigDlg(unsigned parentHwnd) = 0;
	
	//����ͳ����Id����ȡ��Ƶ����ͳ������
	//param key [in] ͳ����Id��������enum StaticsPropsId���� 
	virtual unsigned GetStaticsProps(unsigned key) = 0;

	//YY��protocol�õ�͸�����ݺ�����ô˺���ת����videosdk.dll
	//���������test/sdkLoader/YYCallBack.cpp�е�YYCallBack::OnRecv����
	//param data [in] ��Ӧ�÷������õ������ݵ�ַ
	//param len [in] ���ݳ���
	virtual void recvVideoAppData(const char* data, unsigned len) = 0;

	//��ȡ��ǰpublisher��BITMAPINFOHEADER, �ɹ��򷵻�true, ʧ���򷵻�false
	virtual bool getBmpInfoHead(unsigned publisherUid, BITMAPINFOHEADER& bih) = 0;

	//�ı���������С�����ú���Ч��ֱ�������˸ı�Դ�ֱ���Ϊֹ��
		//param uid     [in] Ҫ�ı����������uid
		//param width   [in] �ڴ��Ŀ��
		//param height  [in] �ڴ��ĸ߶�
	//�˽ӿ��ѱ����Ϊ��������������������Ƶ����ʹ������ĸ��������Žӿڣ�ConvertVideo
	virtual void ResetFrameSize(unsigned uid, int width,int height) =0;

	//����ˮӡ
	//��ʱֻ֧��RGB32��ʽ����(˳��ΪBGRA)
	//param pBmpData [in] ˮӡͼƬRGB����(��alphaͨ��)
	//param nDataLen [in] ͼƬ���ݳ���(�������nWidth*nHeight*4)
	//param nWidth   [in] ˮӡ���
	//param nHeight  [in] ˮӡ�߶�
	virtual void SetLogoBmp(unsigned char* pBmpData, int nDataLen, int nWidth, int nHeight) = 0;

	//���ñ����������򵵴�
	//param level [in] �������Σ���Χ��0-8
	virtual void setCodeSearchRangeLevel(unsigned level) = 0;

	//param configMap[in] ���������б�, ���õ�key�����enum PluginConfig 
	//��ȷ���ڵõ�IVideoWatcher::onYYConnected�ص�����ִ�д˲������������ÿ��ܲ��ɹ�
	virtual void setPluginConfig(const std::map<unsigned char, unsigned>& configMap) = 0;

	//��ȡ��Ƶ��������
	virtual void getAudioWaveData(std::vector<unsigned>& waveData) = 0;

	//��������״̬��������ͷǰ����isPublisher = true����Ԥ�����棻��������������isPublisher = false
	virtual void setPublishStatus(bool isPublisher) = 0;

	//��ȡ����uid���ϡ��������uidsSetΪ�գ���ʾ��ʱû��������
	virtual void getPublisherUids(std::set<unsigned>& uidsSet) = 0;

	//��ȡָ��������������, attributeMap��key�μ�enum StreamAttribute
	virtual void getStreamAttributes(unsigned publisherUid, std::map<unsigned char, unsigned>& attributeMap) = 0;

	//�����������õ���Ƶ��ǿ����
	//param isSupportEnhance [in] �Ƿ����Ƶ��ǿ����
	//param noise [in] ��ΧΪ1-100,����ȥ��ֵ��Խ����ƵԽƽ��
	//param sharp [in] ��ΧΪ1-100,������Ƶ������,Խ����ƵԽ����
	virtual void setVideoEnhanceConfig(bool isOpenVideoEnhance, unsigned noise, unsigned sharp) = 0;

	//�ػ沥�Ŵ���
	virtual void Paint() = 0;

	//�趨���Ŵ��ھ��������2016-09-22����ܻ�ֹͣά������ӿڡ�����setRenderHwndMap����
		//param playWindowHandle [in] ���Ŵ��ھ��(�봫����Ч���ھ����NULL����ΪNULL��sdk��������Ⱦ��Ƶ)
	virtual void setPlayHwnd(std::vector<HWND> &playWindowHandle) = 0;

	//��ȡ�����ӿ�����������������ʱ��
		//param publisherUid[in] Ҫ��ȡ����ʱ��������uid
		//return �����ӿ�������ǰ������ʱ������λΪms���������-1�����ʾ��ǰ����û�п���
	virtual unsigned getPublishTime(unsigned publisherUid) = 0;

	//��ʼ��������Ƶ¼��(�ɽӿڣ��¿���������startServerRecord(std::set<unsigned>& recordUidSet, std::string& programId)����ӿ�)
		//param programId[in] ������ɵĽ�Ŀid
	virtual void startServerRecord(std::string programId) = 0;


	//������������Ƶ¼��
	virtual void stopServerRecord() = 0;

	//����ˮƽ��ת���Ż��� ������uid��ֱ���У��򷵻�false
	virtual bool setHorizintalFlip(const unsigned uid, const bool isFlip) = 0;

	//��֪ĳ���������Ƿ���ˮƽ��ת״̬ ������uid��ֱ���У��򷵻�false
	virtual bool getHorizintalFlip(const unsigned uid, bool &isFlip) = 0;

	//���������������meta��Ϣ
	virtual void setExtraMetaData(const std::map<unsigned char, unsigned>& metas) = 0;

	//��ȡ���������meta��Ϣ
		// param uid[in].Ҫ��ȡ������uid
		// param metas[out].���صĸ�������meta��Ϣ
	virtual void getExtraMetaData(unsigned uid, std::map<unsigned char, unsigned>& metas) = 0;


	//��֪�û���Ƶ���ڿ�����Ƶ��ʵ��ʱ�䣬������Ƶ���ڵ�ʱ��
	//return �û��ڵ�ǰƵ����ʵ���տ�����Ƶ��ʱ�䣬��λΪS;�������-1�����ʾ��ǰû����Ƶ���š�
	virtual unsigned getViewerTime() = 0;

	//��ʼ��������Ƶ¼��
	//param recordUidSet[in] Ҫ¼�Ƶ�����uid��(Ŀǰ�ݲ�֧��ͬʱ��������uid)
	//param programId[in] ������ɵĽ�Ŀid
	virtual void startServerRecord(std::set<unsigned>& recordUidSet, std::string& programId) = 0;

	//����h264�����ݴ���ģʽ
	//param AVCDecoderConfigurationRecord[in] AVCDecoderConfigurationRecord�а���h264��������sps��pps��Ϣ
	//param len[in] AVCDecoderConfigurationRecord ����
	//param bitRate[in] Ԥ���������ʣ����龡��������ȷ���ʣ�����ǿ��ȷ��
	//param maxTolerateBitRate[in] ����������ʣ���ʵ���������������ʸ��ڴ�ֵ����vsdkִ�ж��α��룬��ʵ������ά��������������ʷ�Χ֮�ڣ�
	virtual bool StartRawH264VideoTransmit(unsigned char* AVCDecoderConfigurationRecord, unsigned len, unsigned bitRate, unsigned maxTolerateBitRate=1500000) = 0;

	//����h264�����ݴ���ģʽ
	virtual void StopRawH264VideoTransmit() = 0;

	//ͬ���ɼ�ʱ���
	virtual void syncCapTimeStamp(unsigned ptsBase/*now*/) = 0;

	//���ԭʼ�����ʽ��H264֡����
	virtual void setRawH264VideoData(unsigned char *rawH264VideoNalu, unsigned len, unsigned pts, unsigned dts, bool isKeyFrame) = 0;

	//��ȡֱ��ʱ��streamId, �ýӿ�Ӧ���յ�VideoSdk::PUBLISH_SUCCESS�ص�����ã�����ýӿڿ��ܷ���-1
	virtual unsigned long long getMyStreamId() = 0;

	//���������ݴ���ģʽ�������������
	virtual void ResetmaxTolerateBitRate(unsigned maxTolerateBitRate) = 0;

	//ע��IH264RawDataTranscodeStatusReceiver
	//param videoRawDataTranscodeStatusReceiver [in] �ص���ָ��
	virtual void RegisterH264RawDataTranscodeStatusReceiver(IH264RawDataTranscodeStatusReceiver* videoRawDataTranscodeStatusReceiver) = 0;

	//��ע��IH264RawDataTranscodeStatusReceiver
	virtual void UnregisterH264RawDataTranscodeStatusReceiver() = 0;

	//ע��IVideoStreamTypeChangeReceiver
	//param videoStreamTypeChangeReceiver [in] �ص���ָ��
	virtual void RegisterVideoStreamTypeChangeReceiver(IVideoStreamInputTypeStatusReceiver* videoStreamTypeChangeReceiver) = 0;

	//��ע��IVideoStreamTypeChangeReceiver
	virtual void UnregisterVideoStreamTypeChangeReceiver() = 0;

	//��������uid��ȡ��Ӧ��streamId, ���Ҳ�����Ӧ������uidʱ���򷵻�-1
	//param publisherUid [in] ����uid
	virtual unsigned long long getPublisherStreamId(unsigned publisherUid) = 0;

	//ѡ��IBitmapSourceReleaseReceiver
	//return �ɵ�IBitmapSourceReleaseReceiverָ��
	//param bitmapSourceReleaseReceiver [in] �ص���ָ��
	virtual IBitmapSourceReleaseReceiver *SelectBitmapSourceReleaseReceiver(IBitmapSourceReleaseReceiver* bitmapSourceReleaseReceiver) = 0;

	//�������λͼ����,
	//�����buffer����vsdkռ�ã�ֱ��vsdk�첽����IBitmapSourceReleaseReceiver::onBitmapReSourceRelease��rcHandle��Ϊֹ
	//return ��֡�Ƿ�vsdk����
	        //��vsdk��Ϊ��ʱӦ�ö�֡��������Դ�����buffer���д���������ֱ�ӷ���false�����������첽��release�ص�
	        //����true������vsdk����֡�������ǰ�����壬������ǰ������Ϻ󣬲���Ҫռ��bufferʱ�������첽��release�ص�
	//param buffer   [in]  ͼƬ����ָ��
	//param len      [in]  ͼƬ���ݳ���
	//param rcHandle [in]  ��Դ�����vsdk�ص����뱾���ڴ��Ӧ��releaseʱ�����Ա�rcHandle��Ϊ����
	virtual bool SetBitmapBitsInPlace(unsigned char *buffer, unsigned len, unsigned rcHandle) = 0;

	//ѡ��IRawVideoStreamDataReceiver
	//return �ɵ�IRawVideoStreamDataReceiverָ��
	//param IRawVideoStreamDataReceiver [in] �ص���ָ��
	virtual IRawVideoStreamDataReceiver *SelectVideoStreamTypeChangeReceiver(IRawVideoStreamDataReceiver* pRawVideoStreamDataReceiver) = 0;

	//����uid����ȡһЩ���ַ���ʾ����Ϣ, userInfoMap��key�μ�enum UserStrInfo
	virtual void getUserStrInfo(unsigned publisherUid, std::map<unsigned char, std::string>& userInfoMap) = 0;


	//**********************************************************************************************************************************************
	//�½ӿ�, ֧��һ�������ϴ������
	//**********************************************************************************************************************************************
	//�¼��ص��ӿڣ������¼��뿴VsdkEvent.h
	virtual void setCallback(IVsdkEventCallback* cb) = 0;
	//Vsdk���ýӿڣ����巽���뿴VsdkInvokerEvent.h
	virtual void invoke(ETVsdkInvoke* evt) = 0;


	//���ô�ֱ��ת����, ������uid��ֱ���У��򷵻�false
	virtual bool setVerticalFlip(const unsigned uid, const bool isFlip) = 0;

	//��֪ĳ���������Ƿ��ڴ�ֱ��ת״̬ ������uid��ֱ���У��򷵻�false
	virtual bool getVerticalFlip(const unsigned uid, bool &isFlip) = 0;

	//�������λͼ����,
	//�����buffer����vsdkռ�ã�ֱ��vsdk�첽����IBitmapSourceReleaseReceiver::onBitmapReSourceRelease��rcHandle��Ϊֹ
	//return ��֡�Ƿ�vsdk����
	//��vsdk��Ϊ��ʱӦ�ö�֡��������Դ�����buffer���д���������ֱ�ӷ���false�����������첽��release�ص�
	//����true������vsdk����֡�������ǰ�����壬������ǰ������Ϻ󣬲���Ҫռ��bufferʱ�������첽��release�ص�
	//param buffer   [in]  ͼƬ����ָ��
	//param len      [in]  ͼƬ���ݳ���
	//param rcHandle [in]  ��Դ�����vsdk�ص����뱾���ڴ��Ӧ��releaseʱ�����Ա�rcHandle��Ϊ����
	//param captrueTimestamp [in]  �ɼ�ʱ������봫�뿪�����ɼ�ʱ���������ĺ�����
	virtual bool SetBitmapBitsInPlace(unsigned char *buffer, unsigned len, unsigned rcHandle , unsigned captrueTimestamp) = 0;

	//��ʼ��ȡFLV��Ƶ��
		//param uid       [in] ����uid
		//param publishId [in] ������ʶ
		//param proxyType [in] ��������ʶ, ��enum FlvOverHttpProxyType
		//param url       [in] ���ڻ�ȡ����url
	virtual void StartFlvOverHttpVideo(unsigned uid, unsigned publishId, unsigned proxyType, unsigned codeRate, std::string& url) = 0;


	//������ȡFLV��Ƶ��
		//param uid       [in] ����uid
		//param publishId [in] ������ʶ
	virtual void StopFlvOverHttpVideo(unsigned uid, unsigned publishId) = 0;

	//ҵ�񷽿�ʼʹ��rtmp��������Ƶ
	//param cdn       [in] KeyΪCDN��ַ��ValueΪ��Ȩ��
	//param  start    [in] ����or������1-��ʼ, 0-�Ͽ�;
	virtual void NotifyRtmpStream(std::map<std::string, std::string>& cdn, unsigned start) = 0;

	//**********************************************************************************************************************************************
	//�½ӿ�, ֧������ʹ��Rtmp��������Ƶ
	//**********************************************************************************************************************************************
	//��ʼʹ��rtmp��������Ƶ
		//param proxyType [in] rtmp��������Դ����enum RTMPServer
		//param url       [in] ������rtmp���ķ�����url
		//param playPath  [in] ������rtmp����playPath
	virtual void StartRtmpPublish(unsigned proxyType, std::string& url, std::string& playPath) = 0;
	//����rtmp����
	virtual void StopRtmpPublish() = 0;

    //����h265�����ݴ���ģʽ
    //param HEVCDecoderConfigurationRecord[in] AVCDecoderConfigurationRecord�а���h265��������sps��pps��Ϣ
    //param len[in] AVCDecoderConfigurationRecord ����
    //param bitRate[in] Ԥ���������ʣ����龡��������ȷ���ʣ�����ǿ��ȷ��
    //param maxTolerateBitRate[in] ����������ʣ���ʵ���������������ʸ��ڴ�ֵ����vsdkִ�ж��α��룬��ʵ������ά��������������ʷ�Χ֮�ڣ�
    virtual bool StartRawH265VideoTransmit(unsigned char* HEVCDecoderConfigurationRecord, unsigned len, unsigned bitRate, unsigned maxTolerateBitRate=1500000) = 0;

    //����h264�����ݴ���ģʽ
    virtual void StopRawH265VideoTransmit() = 0;

    //���ԭʼ�����ʽ��H264֡����
	//parama frameType ��ʱ������� enum ֵ��Ӧ��
	//                enum ENCODED_FRAME_TYPE
	//                {
	//	                  ERR_Frame = 0,
	//           		  P_Frame = 1,
	//	                  I_Frame = 2,
	//		              IDR_Frame = 3,
	//		              B_Frame = 4,
	//                };
	virtual void setRawH265VideoData(unsigned char *rawH264VideoNalu, unsigned len, unsigned pts, unsigned dts, int frameType) = 0;

	//ҵ�񷽽�������Ƶʱ��ÿ����ģ��֪ͨvsdk�ϲ��������Ƶ���(����ʹ��)
		//param publisherUid               [in] ����Ƶ��������uid
		//param publishId                  [in] ����Ƶ��������publishId
		//param audioFrameIdToPlayStamp    [in] key����Ƶ֡��� value��TimeGetTime��ȡ������ʱ���
		//param videoFrameIdToRenderStamp  [in] key����ƵƵ֡��� value��TimeGetTime��ȡ����Ⱦʱ���
	virtual void setPluginDecodeAudioVideoStat(unsigned publisherUid, unsigned publishId, const std::map<unsigned, unsigned>& audioFrameIdToPlayStamp,
		std::map<unsigned, unsigned>& videoFrameIdToRenderStamp) = 0;

	//ҵ�񷽽�����Ƶʱ����Ⱦǰ������Ƶ���ݸ�vsdk�����ڷ�����(����ʹ��)
		//param publisherUid               [in] ��Ƶ��������uid
		//param publishId                  [in] ��Ƶ��������publishId
		//param data                       [in] ͼƬ����
		//param bitmapHeader               [in] λͼ�ļ�ͷ
	virtual void setPluginPlayVideoFrame(unsigned publisherUid, unsigned publishId, const unsigned char* data, const BITMAPINFOHEADER &bitmapHeader) = 0;

	//����ѹ����λͼ����
	virtual void setBitmapBitsWithTimestamp(unsigned char *buffer, unsigned len, unsigned captureTime) = 0;

	//����ˮӡ
	//��ʱֻ֧��RGB32��ʽ����(˳��ΪBGRA)
	//param pBmpData [in] ˮӡͼƬRGB����(��alphaͨ��)
	//param nDataLen [in] ͼƬ���ݳ���(�������nWidth*nHeight*4)
	//param nWidth   [in] ˮӡ���
	//param nHeight  [in] ˮӡ�߶�
	//param x        [in] ˮӡͼƬ���Ͻ�����Ƶ���Ͻǵĺ������ֵ
	//param y        [in] ˮӡͼƬ���Ͻ�����Ƶ���Ͻǵ��������ֵ
	virtual void SetLogoBmpWithPosition(unsigned char* pBmpData, int nDataLen, int nWidth, int nHeight, int posX, int posY) = 0;

	//param configMap[in] ���������б�, ���õ�key�����enum PluginConfig 
	//��ȷ���ڵõ�IVideoWatcher::onYYConnected�ص�����ִ�д˲������������ÿ��ܲ��ɹ�
	//���ڴ����ַ�������
	virtual void setPluginConfigString(const std::map<unsigned char, const char*>& configMap) = 0;

	//����ǰYUV���ݻص�
	virtual void setEncodeYuvCallback(YUV_CALLBACK_FUNCTION pCallbackFunction) = 0;

	//����ָ�����ڵ�����ģʽ���˽ӿڲ�����setPlayHwnd��setRenderHwndMap֮ǰ���ã����Բ���ʱ����ʵʱ����
	//param hWnd [in] ������setPlayHwnd��setRenderHwndMap�ӿ����ù��ģ�������ûᱻ����
	//param mode [in] ����ģʽ��0-����Ŀ�괰�ڣ������ڱߣ��õ������ģ�1-��Ŀ�괰���ڲ�������Ӧ�����ڱߣ�2-������DX�����Լ�⣬������
	virtual void setVideoRenderScaleMode(HWND hWnd, int mode) = 0;

	//������Ⱦ�ص�
	virtual void setVideoRenderCallback(IVideoRenderCallback *cb) = 0;

	//����UID����Ⱦ���ھ���İ󶨹�ϵ������ӿڲ��ܺ�setPlayHwndͬʱʹ��
	virtual void setRenderHwndMap(std::map<unsigned, HWND>& hwndMap) = 0;

	//��Ƶ��ʽת�����˳�����ʱ�ɴ���NULL������ڲ������ġ�����ʽ��Ϣ(��߻��ʽ)�����仯ʱ����ִ�д���NULL���ԭ�������ġ�
	/*  һ���򵥵����Ӵ���
		dst_buffer = malloc(dst_width * dst_height * 3); //����һ�κ�����ظ�ʹ�ã�Ŀ��ֱ��ʸı�ʱ�ͷź����·���

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

		����֮������ݾ��� dst_buffer ����
	*/
	virtual bool ConvertVideo(VideoConvertParam *vcp) = 0;

	//ҵ��"��ֽ"���ܻص�
	//����ǰ setPluginConfig ����CAMERA_DATA_CALLBACK���
	//������ sdk�� �� virtual void onVsdkEvent(ETVsdkEvent* ev); �ص������ ET_CAMERA_DATA ��������ͷ���ݸ�ҵ��
	//ҵ������ɺ�,����Ƶ��������sdk���뼴��
	//format Ŀǰֻ֧��eAV_PIX_FMT_RGBA ��eAV_PIX_FMT_RGB24
	virtual void cameraDataCallback( unsigned char* buffer, unsigned bufLen,int w,int h, unsigned mediaTimeStamp, unsigned pushTimeStamp,bool hasPreprocessed,VideoPixelFormat format ) = 0;

	//����ˮӡ���ѷ�������ʹ��ETSetWaterMarkList��
	//��ʱֻ֧��RGB32��ʽ����(˳��ΪBGRA)
	//param pBmpData [in] ˮӡͼƬRGB����(��alphaͨ��)
	//param nDataLen [in] ͼƬ���ݳ���(�������nWidth*nHeight*4)
	//param nWidth   [in] ˮӡ���
	//param nHeight  [in] ˮӡ�߶�
	//param x        [in] ˮӡͼƬ���Ͻ�����Ƶ���Ͻǵĺ������ֵ
	//param y        [in] ˮӡͼƬ���Ͻ�����Ƶ���Ͻǵ��������ֵ
	virtual void SetLogoBmpWithPositionFromVector( VEC_LOGOINFO& vecLogoInfo ) = 0;

	//��Ҫ��Ϊ���ֶ����Ƽ����ԣ���Ҫ������ӿڶ������޸Ļ�����µĽӿڡ��½ӿ�ȫ��ͨ��ETVsdkInvoke�̳ж��� @�벼��

};// end of IYYVideo

};// end of nameSpace


//��������
//��ʼ��dll
//param codeRate [in] ֧������������������ʣ�����ֵ������enum VideoCodeRate��
//	case LOW_DEFINITION  ��ʾ�������ʡ�
//	case HIGH_DEFINITION ��ʾ�������ʡ�
//param sdkVer [in] ������õ�videoSdk�汾�š���������sdkVer����VIDEO_SDK_VERSION����ȣ�����false
//��ʼ���ɹ�������true�����򷵻�false
VIDEO_SDK_API bool InitVideoService(unsigned codeRate, unsigned sdkVer);

//����ʼ��dll
VIDEO_SDK_API void UnInitVideoService();

//��ȡIYYVideoָ��
VIDEO_SDK_API VideoSdk::IYYVideo* GetInstance();

};