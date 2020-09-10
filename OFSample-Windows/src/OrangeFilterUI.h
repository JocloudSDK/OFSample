#pragma once
#ifndef ORANGEFILTERUI_H
#define ORANGEFILTERUI_H
#include "Camera.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "effectmanager.h"

#define OFCAMERA_VERSION L"1.4.5"

class CCameraDS;
class OrangeFilterRender;
class EffcctManager;

class OrangeFilterUI
{
public:
    OrangeFilterUI();
    ~OrangeFilterUI();
    bool OrangeFilterUICreate(int width, int height);
    void RendererRun();
private:
    void ShowDemoTitleImage();
    void ShowCameraListWindow();
    void ShowStickerWindow();
	void ShowSettingWindow(bool &bOpen);
    void ShowBeautyTabs();
    void ShowDebugInfoWindow();
    void ShowItemSelectWindow();
    void RenderFrame();
	size_t GetSelectFilterParamType(size_t nBase);
    GLFWwindow* m_window;
	OrangeFilterRender * m_orangeFilterRender;
    //std::shared_ptr<CCameraDS> m_capture;
	ICameraBase *m_pCameraBase;
    int m_selectedCamera;
    bool m_isInitOrangeFilterSuccess;
    int m_bundleCategory;
    bool m_showItemSelectWindow;
    bool m_showDegubInfoWindow;
    std::vector<EffectCategory> m_categoryBundles;
    int m_curRenderItemUIID;
    std::string m_curRenderItemName;
    std::string m_runPath;
    double m_lastTime;
	int m_nSelectFilterNum;
	string m_strSelectEffectsPath;
	float m_fFilterIntensity;

    //
    // DebugInfo
    //
    uint32_t m_FPS;
    uint32_t m_resolutionWidth;
    uint32_t m_resolutionHeight;
    float m_renderTime;
};

class CameraFactory {
public:
	virtual ICameraBase* CreateCameraMode() = 0;
};

class OpencvCameraFactory : public CameraFactory
{
public:
	ICameraBase* CreateCameraMode()
	{
		return new CCameraDS();
	}
};
#endif


