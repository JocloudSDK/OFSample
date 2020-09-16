#include "Camera.h"
#include "stdio.h"
#include "../src/log/loggerExt.h"
extern int get_fps();

char tmp[] = "IMG";
LPCWSTR name = (WCHAR *)tmp;
HANDLE hMutex = CreateMutex(NULL, false, name);
CCameraDS::CCameraDS()
{
	init();
}

CCameraDS::~CCameraDS()
{
	status = STATUS_NO_CAMERA;
	if (mCapture.isOpened())
	{		
		mCapture.release();
	}
}

void CCameraDS::init() {	
	m_isCameraInited = false;
	useDefaultFrame = false;
	m_filepath = "";
	rs_width = 640;
	rs_height = 480;
	status = STATUS_UN_INIT;
	m_capture_camera_id = 0;
	frame_id = 0;
	frameCount = 0;
	getDeviceList();
}

void CCameraDS::playCamera()
{
	if (!m_isCameraInited) {
		initCamera(rs_width, rs_height, 0);
	}

	if (status == STATUS_PLAYING)return;
	if (status == STATUS_STOP || status == STATUS_INIT) {

		status = STATUS_PLAYING;
		return;
	}
}

void CCameraDS::stopCamera()
{
	if (status == STATUS_PLAYING) {
		status = STATUS_STOP;
	}
}


#define  CAP_DELAY 10
BOOL CCameraDS::Run()
{
	//WaitForSingleObject(hMutex, INFINITE);
	if (status != STATUS_NO_CAMERA && mCapture.grab() && mCapture.isOpened())
	{
		mCapture.retrieve(this->frameData);
		char key = (char)cv::waitKey(CAP_DELAY);
	}
	//ReleaseMutex(hMutex);
	if (this->getStatus() == 5)//STATUS_PLAYING
	{
		FALSE;
	}
	return TRUE;
}
/*
DWORD WINAPI TL_FRAME(LPVOID lpParamter)
{
	CCameraDS* cc = (CCameraDS*)lpParamter;	
	while (cc->mCapture.grab())
	{
		WaitForSingleObject(hMutex, INFINITE);
		if (cc->mCapture.isOpened())
		{			
			cc->mCapture.retrieve(cc->frame);
			char key = (char)cv::waitKey(CAP_DELAY);
		}
		ReleaseMutex(hMutex);
		if (cc->getStatus() == 5)//STATUS_PLAYING
		{
			break;
		}
	}
	//TODO v140xp
	return 0;
}*/

cv::Size CCameraDS::getCameraResolution()
{
	//mCapture.set(cv::CAP_PROP_FRAME_WIDTH, 10000);
	//mCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 10000);
	int w = (int)mCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	int h = (int)mCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
	return cv::Size(w, h);
}

void CCameraDS::QueryFrame() {
	try {
		if (status == STATUS_PLAYING) {
			if (m_capture_type == CAPTURE_FILE)
			{				
				mCapture.retrieve(frameData);
				if (frameData.empty())mCapture.retrieve(frameData);
				frame_id++;
				//if (frame_id == frameCount - 1)
				//{
				//	cvSetCaptureProperty(mCapture, CV_CAP_PROP_POS_FRAMES, 0);
				//	frame_id = 0;
				//}
			}
		}
	}
	catch (...) {
		loge(" CCameraDS::QueryFrame error\nd");
	}
}

void CCameraDS::setFilePath(std::string path)
{
	m_filepath = path;
}

void CCameraDS::setCaptureType(int _type)
{
	m_capture_type = _type;
}

void CCameraDS::setCaptureCameraID(int _id)
{
	m_capture_camera_id = _id;
}

cv::Mat CCameraDS::getCameraData()
{
	cv::Mat result;
	cv::flip(frameData, result, 1);  //ˮƽ������
	return result;
}

bool CCameraDS::GetCameraResolution(uint32_t &nWidth, uint32_t &nHeight)
{
	if (!mCapture.isOpened())return false;
	nWidth = (int)mCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	nHeight = (int)mCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
	base::Logd("SOUIEngine", base::Log("GetCameraResolution ").setMessage("maxRes width[%d], height[%d]", nWidth, nHeight));
	return true;
}

bool CCameraDS::SetCameraResolution(uint32_t nWidth, uint32_t nHeight)
{
	if (!mCapture.isOpened())return false;

	cv::Size maxRes = getCameraResolution();
	cv::Size neededRes = cv::Size(nWidth, nHeight);

	base::Logd("SOUIEngine", base::Log("getCameraResolution").setMessage("maxRes width[%d], height[%d]", maxRes.width, maxRes.height));
	if (maxRes.width < neededRes.width) {
		double aR = (double)maxRes.width / maxRes.height;
		int height = neededRes.width / aR;
		mCapture.set(cv::CAP_PROP_FRAME_WIDTH, neededRes.width);
		mCapture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
		base::Logd("SOUIEngine", base::Log("maxRes.width < neededRes.width").setMessage("maxRes height[%d], width[%d]", neededRes.width, height));
	}
	else if (maxRes.height < neededRes.height) {
		double aR = (double)maxRes.width / maxRes.height;
		mCapture.set(cv::CAP_PROP_FRAME_HEIGHT, neededRes.height);
		mCapture.set(cv::CAP_PROP_FRAME_WIDTH, neededRes.height*aR);
		base::Logd("SOUIEngine", base::Log("maxRes.height < neededRes.height").setMessage("maxRes height[%d], width[%d]", neededRes.height, neededRes.height*aR));
	}
	else {
		mCapture.set(cv::CAP_PROP_FRAME_HEIGHT, neededRes.height);
		mCapture.set(cv::CAP_PROP_FRAME_WIDTH, neededRes.width);
		base::Logd("SOUIEngine", base::Log("else ").setMessage("maxRes width[%d], height[%d]", neededRes.width,neededRes.height));
	}
	mCapture.set(cv::CAP_PROP_AUTOFOCUS, 0);
	mCapture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	return true;
}

void CCameraDS::initCamera(int width, int height,int camID) {
	logi("CCameraDS::initCamera\n");
	status = STATUS_NO_CAMERA;
	try {
		if (CAPTURE_FILE == m_capture_type)
		{
			if (m_filepath == "")
			{
				openFileDlg();
			}
			//mCapture = cvCaptureFromFile(m_filepath.c_str());
			mCapture.open(m_filepath.c_str());
			if (!mCapture.isOpened())
				throw std::runtime_error("Unable to open video source");
		}
		else
		{
			bool ret = mCapture.open(cv::CAP_DSHOW + camID);
			if (!mCapture.isOpened())
				throw std::runtime_error("Unable to open video source");
		}
	}
	catch (...) {
		loge("camera init error\n");
	}
}

void CCameraDS::closeCamera() {
	
	status = STATUS_NO_CAMERA;
	Sleep(1000);

	if (mCapture.isOpened())
	{
		mCapture.release();
	}
}

void CCameraDS::connectCamera()
{
	if (!mCapture.isOpened()) {
		loge("can not open camera!!!\n");
		status = STATUS_NO_CAMERA;
	}
	if (mCapture.isOpened())
	{
        //��ȡ��Ƶ��֡��
		double fps = 60;
        if (m_capture_type == CAPTURE_FILE)
		{
            //��ȡ��Ƶ��֡��
			fps = mCapture.get(cv::CAP_PROP_FPS); 
			frameCount = (int)mCapture.get(cv::CAP_PROP_FRAME_COUNT);
            //����ÿ֡���ŵ�ʱ��
			int vfps = (int)(1000 / fps);                                       
		}
		else
		{
			CreateReadThread();
		}

		status = STATUS_INIT;
		m_isCameraInited = true;
	}
}

bool CCameraDS::openFileDlg()
{
	//QString qstr_path = QFileDialog::getOpenFileName(0, tr("Select Video"), "", "allfiles(*.mp4;*.avi);");
	//if (qstr_path == "")return false;
	//m_filepath = qstr_path.toLocal8Bit();
	m_capture_type = CAPTURE_FILE;
	return true;
}

bool CCameraDS::isPlaying() {
	return status == STATUS_PLAYING;
}

void CCameraDS::restartCamera()
{
	closeCamera();
	initCamera(rs_width,rs_height,0);
}

cv::Mat CCameraDS::getFrame() {
	if (frameData.rows == 0&& frameData.cols ==0 )
	{
		static cv::Mat defaulfFrame = cv::imread("res/frame.png",cv::IMREAD_COLOR);
		if (!defaulfFrame.empty())
		{
			cv::resize(defaulfFrame, defaulfFrame, cv::Size(rs_width, rs_height));
			frameData = defaulfFrame;
			useDefaultFrame = true;
		}
	}
	return frameData;
}

#pragma region enum_camera


static void setcolor(unsigned int color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, color | FOREGROUND_INTENSITY);
}

void CCameraDS::_FreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0)
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL)
	{
		// pUnk should not be used.
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}


HRESULT CCameraDS::CamCaps(IBaseFilter *pBaseFilter)
{
	HRESULT hr = 0;
	std::vector<IPin*> pins;
	IEnumPins *EnumPins;
	pBaseFilter->EnumPins(&EnumPins);
	pins.clear();
	for (;;)
	{
		IPin *pin;
		hr = EnumPins->Next(1, &pin, NULL);
		if (hr != S_OK) { break; }
		pins.push_back(pin);
		pin->Release();
	}
	EnumPins->Release();

	//printf("Device pins number: %d\n", pins.size());

	PIN_INFO pInfo;
	for (unsigned int i = 0; i < pins.size(); i++)
	{
		pins[i]->QueryPinInfo(&pInfo);

		setcolor(RED);

		if (pInfo.dir == 0)
		{
			//wprintf(L"Pin name: %s (���ӧ��)\n", pInfo.achName);
		}

		if (pInfo.dir == 1)
		{
			//wprintf(L"Pin name: %s (�������)\n", pInfo.achName);
		}

		IEnumMediaTypes *emt = NULL;
		pins[i]->EnumMediaTypes(&emt);

		AM_MEDIA_TYPE *pmt;

		std::vector<SIZE> modes;
		setcolor(GRAY);
		//wprintf(L"Avialable resolutions.\n", pInfo.achName);
		for (;;)
		{
			hr = emt->Next(1, &pmt, NULL);
			if (hr != S_OK) { break; }

			if ((pmt->formattype == FORMAT_VideoInfo) &&
				//(pmt->subtype == MEDIASUBTYPE_RGB24) &&
				(pmt->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
				(pmt->pbFormat != NULL))
			{
				VIDEOINFOHEADER *pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;
				SIZE s;
				// Get frame size
				s.cy = pVIH->bmiHeader.biHeight;
				s.cx = pVIH->bmiHeader.biWidth;
				// ���ڧ��֧ۧ�
				unsigned int bitrate = pVIH->dwBitRate;
				modes.push_back(s);
				// Bits per pixel
				unsigned int bitcount = pVIH->bmiHeader.biBitCount;
				REFERENCE_TIME t = pVIH->AvgTimePerFrame; // blocks (100ns) per frame
				int FPS =(int) floor(10000000.0 / static_cast<double>(t));
				//printf("Size: x=%d\ty=%d\tFPS: %d\t bitrate: %ld\tbit/pixel:%ld\n", s.cx, s.cy, FPS, bitrate, bitcount);
			}
			_FreeMediaType(*pmt);
		}
		//----------------------------------------------------
		// 
		// 
		// 
		//----------------------------------------------------
		modes.clear();
		emt->Release();
	}

	pins.clear();

	return S_OK;
}

/*
* Do something with the filter. In this sample we just test the pan/tilt properties.
*/
void CCameraDS::process_filter(IBaseFilter *pBaseFilter)
{
	CamCaps(pBaseFilter);
}


/*
* Enumerate all video devices
*
* See also:
*
* Using the System Device Enumerator:
*     http://msdn2.microsoft.com/en-us/library/ms787871.aspx
*/
int CCameraDS::enum_devices()
{
	HRESULT hr;
	setcolor(GRAY);
	printf("Enumeraring videoinput devices ...\n");

	// Create the System Device Enumerator.
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
	{
		fprintf(stderr, "Error. Can't create enumerator.\n");
		return hr;
	}

	// Obtain a class enumerator for the video input device category.
	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

	if (hr == S_OK)
	{
		// Enumerate the monikers.
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
				(void **)&pPropBag);
			if (SUCCEEDED(hr))
			{
				// To create an instance of the filter, do the following:
				IBaseFilter *pFilter;
				hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,
					(void**)&pFilter);
				if (!pFilter)
				{
					printf("Warning! Create an instance of the filter failed! \n");
					setcolor(GRAY);
					pMoniker->Release();
					break;
				}
				// To retrieve the filter's friendly name, do the following:
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if (SUCCEEDED(hr))
				{
					// Display the name in your UI somehow.
					setcolor(GREEN);
					std::string str;
					int len = WideCharToMultiByte(CP_ACP, 0, varName.bstrVal, wcslen(varName.bstrVal), NULL, 0, NULL, NULL);
					char    *m_char = new char[len + 1];
					WideCharToMultiByte(CP_ACP, 0, varName.bstrVal, wcslen(varName.bstrVal), m_char, len, NULL, NULL);
					m_char[len] = '\0';
					str = m_char;
					deviceList.push_back(str);
					//wprintf(L"------------------> %s <------------------\n", varName.bstrVal);
				}
				VariantClear(&varName);
				//Remember to release pFilter later.
				pPropBag->Release();
			}
			pMoniker->Release();
		}
		pEnumCat->Release();
	}
	setcolor(GRAY);
	pSysDevEnum->Release();

	return 0;
}

int CCameraDS::getDeviceList()
{
	//setlocale(LC_ALL, "Russian");
	int result;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	result = enum_devices();

	CoUninitialize();
	//getchar();
	return result;
}
bool CCameraDS::isInit()
{ 
	return m_isCameraInited; 
}

int CCameraDS::getDeviceNameList(vector<string> &vNameList)
{
	vNameList = deviceList;
	return 0;
}

#pragma endregion enum_camera