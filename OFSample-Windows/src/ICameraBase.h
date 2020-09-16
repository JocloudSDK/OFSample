#pragma once
/********************************
ICameraBase 获取摄像头数据基类
*********************************/

#include<Windows.h>
#include<vector>

class ICameraBase
{

public:
	ICameraBase()
	{
		bIsExit = false;
		hThread = NULL;
		
	}
	virtual ~ICameraBase()
	{
		bIsExit = true;
		if (hThread != NULL)
		{
			CloseHandle(hThread);
			hThread = NULL;
		}	
	}

	virtual void initCamera(int, int, int) = 0;
	virtual void closeCamera() = 0;
	virtual void connectCamera() = 0;
	virtual void playCamera() = 0;
	virtual void stopCamera() = 0;
	virtual bool isInit() = 0;
	virtual bool isPlaying() =0;
	virtual int getDeviceNameList(std::vector<std::string> &vNameList) = 0;
	virtual cv::Mat getCameraData() = 0;
	virtual bool GetCameraResolution(uint32_t &nWidth, uint32_t &nHeight) = 0;
	virtual bool SetCameraResolution(uint32_t nWidth, uint32_t nHeight) = 0;

protected:
	virtual BOOL Run() { return FALSE; };
	static DWORD WINAPI TL_FRAME(LPVOID lpParamter)
	{
		ICameraBase* pCameraBase = (ICameraBase*)lpParamter;
		while (!pCameraBase->bIsExit)
		{
			if (!pCameraBase->Run())
			{
				Sleep(1000);
				continue;
			}
			//pCameraBase->getCameraData();
		}
		return 0;
	}
	void CreateReadThread()
	{
		if(!hThread)
			hThread = CreateThread(NULL, 0, TL_FRAME, this, 0, NULL);
	}

protected:
	cv::Mat frameData;
	bool bIsExit;
private:
	HANDLE hThread;

};

