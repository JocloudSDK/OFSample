#pragma once

#include <stack>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <locale>
#include <vector>
#include <dshow.h>
#include "ICameraBase.h"
#pragma comment(lib,"Strmiids.lib")

#define BLUE    0x0001
#define GREEN   0x0002
#define RED     0x0004
#define GRAY    0x0007

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = nullptr; } }
#endif

#ifndef loge
#define loge    printf
#define logi    printf
#endif
enum CAPTURE_TYPE
{
	CAPTURE_CAMERA,
	CAPTURE_FILE
};

using namespace std;

class CCameraDS : public ICameraBase
{

public:
	CCameraDS();
	~CCameraDS();

	virtual void initCamera(int, int, int);
	virtual bool isInit();
	virtual void closeCamera();
	virtual void connectCamera();
	virtual void playCamera();
	virtual void stopCamera();
	virtual bool isPlaying();
	virtual int getDeviceNameList(vector<string> &vNameList);
	virtual cv::Mat getCameraData();
	virtual BOOL Run();
	virtual bool GetCameraResolution(uint32_t &nWidth, uint32_t &nHeight);
	virtual bool SetCameraResolution(uint32_t nWidth, uint32_t nHeight);

	bool openFileDlg();
	void restartCamera();

	int getStatus() { return status; }
	void setFilePath(std::string path);
	void setCaptureType(int _type);
	void setCaptureCameraID(int _id);
	cv::Mat getFrame();
	void _FreeMediaType(AM_MEDIA_TYPE & mt);
	HRESULT CamCaps(IBaseFilter * pBaseFilter);
	void process_filter(IBaseFilter * pBaseFilter);
	int enum_devices();
	int getDeviceList();
	//std::vector<std::string> getDeviceNameList();
	void init();

	std::vector<std::string> deviceList;
	cv::Size getCameraResolution();
	void QueryFrame();
private:
	float w, h;
	typedef enum {
		STATUS_UN_INIT,
		STATUS_NAMA_UN_INIT,
		STATUS_INIT,
		STATUS_PLAYING,
		STATUS_STOP,
		STATUS_NO_CAMERA,
		STATUS_ERROR
	}PlayStatus;
	
	std::string m_filepath;
	PlayStatus status;
	bool m_isCameraInited = false;
	int m_capture_type;
	int m_capture_camera_id;
	int frame_id;
	int frameCount;
public:
	int rs_width;
	int rs_height;
	bool useDefaultFrame;
	cv::VideoCapture mCapture;
	//cv::Mat frame;
	cv::Mat resize_frame;
	HANDLE hThread;
};

