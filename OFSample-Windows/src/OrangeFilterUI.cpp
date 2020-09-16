#include "OrangeFilterUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_extension.h"
#include "OrangeFilterRender.h"
#include "Texture.h"
#include "pathutils.h"
#include "fileutils.h"
#include "CrashReportExport.h"
#include "dwbase/log.h"
#include "OrangHelperManager.h"
#include <string>
#include <locale>
#include <codecvt>
#include "imgui_internal.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
const char TAGOFCAMERA[] = "OFSample";
const char TAGOF[] = "OF";
int oriWindowWidth = 800;
int oriWindowHeight = 600;
float scaleRatioW = 1.f;
float scaleRatioH = 1.f;


//内部测试和对外的资源商店不同
#define INTERNAL_TEST


//const char *CT_PLicense = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";   
const char *CT_PLicense = "0b7f3051-bf5d-11ea-90f8-b42e994ac7a8";

const std::string g_assetDir = "../assets/";
const std::string gBundlePath[] = {
    g_assetDir + "items/" + "Animoji",
    g_assetDir + "items/" + "ItemSticker",
    g_assetDir + "items/" + "ARMask",
};


static void crash_setup()
{
    std::wstring ver = OFCAMERA_VERSION;
    InitBugReport(L"OFSample-pc", ver.c_str(), L"OFSample-pc");
    SetShowReportUI(OF_TRUE);
    //SetCustomProperty(L"subprocess", L"subroutine");
#if 0
    throw("OFCamera crash test");
#endif
}

void OF_LogCallbackFun (const char* fmtMsg)
{
    LogFinal(TAGOF) << fmtMsg;
    printf_s("OrangeFilter %s\n", fmtMsg);
}


static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    //ImGuiStyle::ScaleAllSizes(1.0f);
    scaleRatioW = (float)width / (float)oriWindowWidth;
    scaleRatioH = (float)height / (float)oriWindowHeight;
}

static bool LayoutImageButtonWithText(const ImVec2& pos, const ImVec2& size, ImTextureID user_texture_id, ImTextureID user_texture_id2, const char* label)
{
    ImGui::BeginGroup();
    bool ret = ImGui::ImageRoundButton2(user_texture_id, user_texture_id2, ImVec2(size.x , size.y ));
    ImGui::Spacing();
    ImGui::Dummy(ImVec2(pos.x , 1)); ImGui::SameLine(); ImGui::Text(label);
    ImGui::EndGroup();
    return ret;
}

static bool LayoutButton(const ImVec2& pos, const ImVec2& size, const char* label)
{
    ImGui::BeginGroup();
    ImGui::Dummy(ImVec2(1, pos.y ));
    ImGui::Dummy(ImVec2(pos.x , 1)); ImGui::SameLine();
    bool ret = ImGui::Button(label, ImVec2(size.x , size.y));
    ImGui::EndGroup();
    return ret;
}

static int get_fps()
{
    static int fps = 0;
    static int lastTime = GetTickCount(); // ms
    static int frameCount = 0;

    ++frameCount;

    int curTime = GetTickCount();
    if (curTime - lastTime > 1000) // 取固定时间间隔为1秒
    {
        fps = frameCount - 1;
        frameCount = 0;
        lastTime = curTime;
    }
    return fps;
}

std::string string_To_UTF8(const std::string & str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char * pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

    delete[]pwBuf;
    delete[]pBuf;

    pwBuf = NULL;
    pBuf = NULL;

    return retStr;
}

struct BasicEffect
{
    const char* name;
    bool        created;
    bool        isEnable;
    bool        Dirty;
};

static BasicEffect g_effect[4];
static int g_beautyType = 0;  // 0-basic, 1-advance

OrangeFilterUI::OrangeFilterUI() : m_window(NULL),
m_orangeFilterRender(NULL),
m_pCameraBase(NULL),
m_isInitOrangeFilterSuccess(false),
m_selectedCamera(0),
m_bundleCategory(-1),
m_curRenderItemUIID(0),
m_curRenderItemName(""),
m_lastTime(0.0),
m_showItemSelectWindow(false),
m_showDegubInfoWindow(false),
m_FPS(60),
m_resolutionWidth(1280),
m_resolutionHeight(720),
m_nSelectFilterNum(0),
m_strSelectEffectsPath(""),
m_renderTime(33.0f),
m_fFilterIntensity(0.0f)
{
    crash_setup();
    OF_SetLogCallback(OF_LogCallbackFun);
    m_categoryBundles.clear();
}

OrangeFilterUI::~OrangeFilterUI()
{
    m_categoryBundles.clear();
	if (m_pCameraBase)
	{
		delete m_pCameraBase;
		m_pCameraBase = NULL;
	}
}

bool OrangeFilterUI::OrangeFilterUICreate(int width, int height)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        LogFinal(TAGOFCAMERA) << ("Failed to initialize GLFW");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    m_window = glfwCreateWindow(width, height, "OFSample", NULL, NULL);
    if (m_window == NULL)
    {
        LogFinal(TAGOFCAMERA) << ("glfwCreateWindow Failed ");
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowSizeCallback(m_window, window_size_callback);
    glfwSwapInterval(0); // Enable vsync

    //Init glew
    //GLEW obtains information on the supported extensions from the graphics driver. Experimental or pre-release drivers, however, might not report every available extension through the standard mechanism, in which case GLEW will report it unsupported. To circumvent this situation, the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), which ensures that all extensions with valid entry points will be exposed
    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        LogFinal(TAGOFCAMERA) << ("Failed to initialize GLEW");
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGuiStyle *style = &ImGui::GetStyle();
    ImVec4 *colors = style->Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(252.f / 255.f, 253.f / 255.f, 255.f / 255.f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(252.f / 255.f, 253.f / 255.f, 255.f / 255.f, 1.00f);
    colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
    colors[ImGuiCol_Button] = ImColor(252, 253, 255, 255);
    colors[ImGuiCol_ButtonHovered] = ImColor(152, 165, 245, 255);
    colors[ImGuiCol_ButtonActive] = ImColor(112, 136, 237, 255);
    colors[ImGuiCol_ScrollbarBg] = ImColor(252, 253, 255, 255);
    colors[ImGuiCol_ScrollbarGrab] = ImColor(112, 136, 237, 255);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(112, 136, 237, 255);
    colors[ImGuiCol_PopupBg] = ImColor(252, 253, 255, 255);


    m_runPath = CurrentApplicationDirA();
    std::string fontFile;
    PathJoinA(fontFile, m_runPath, "res/SimHei.ttf");
    std::string fontFileUtF8 = string_To_UTF8(fontFile);
    if (doesFileExist(fontFile))
    {
        io.Fonts->AddFontFromFileTTF(fontFileUtF8.c_str(), 14.5f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    }
	m_resolutionWidth = 800;
	m_resolutionHeight = 600;

    //init camera
	std::shared_ptr<CameraFactory> Factory  = std::shared_ptr<CameraFactory>(new OpencvCameraFactory);
	m_pCameraBase = Factory->CreateCameraMode();
	m_pCameraBase->initCamera(m_resolutionWidth, m_resolutionHeight, m_selectedCamera);
	m_pCameraBase->GetCameraResolution(m_resolutionWidth, m_resolutionHeight);
	m_pCameraBase->connectCamera();
	std::string licensePath;
	PathJoinA(licensePath, m_runPath, "of_offline_license.license"); 
	bool ret = OrangHelperManager::getOrangHelper()->createContext(CT_PLicense, licensePath, m_runPath);
	if (!ret)
	{
		printf("sdk init failed.\n");
		return ret;
	}

    //init orange filter
    m_orangeFilterRender = new OrangeFilterRender();
    m_isInitOrangeFilterSuccess = m_orangeFilterRender->init(m_runPath.c_str());

	std::shared_ptr<EffcctManager>  m_effectManager = std::shared_ptr<EffcctManager>(new EffcctManager());
    m_effectManager->InitEffectManager(m_runPath.c_str());

    //bool isLoadedEffect = m_effectManager->LoadAssetStore(effectURL.c_str(), m_runPath.c_str());

    bool isLoadedEffect = m_effectManager->LoadAssetStoreFormXml(m_runPath.c_str());// 本地加载
	//TODO:完善下载

    m_categoryBundles = m_effectManager->GetEffectCategoryList();
    return m_isInitOrangeFilterSuccess;
}

void OrangeFilterUI::ShowDemoTitleImage()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280, 60), ImGuiCond_Always);
    ImGui::Begin("TitleWindow##21", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    std::string filename = "title.png";
    Texture::SharedPtr tex = Texture::createTextureFromFile(filename, false);
    float my_tex_w = (float)ImGui::GetContentRegionAvailWidth();
    float my_tex_h = (float)ImGui::GetContentRegionAvail().y;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::Image(tex->getTextureID(), ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
    ImGui::PopStyleVar();
    ImGui::End();
    ImGui::PopStyleVar();
}

void OrangeFilterUI::ShowCameraListWindow()
{
    ImGui::SetNextWindowPos(ImVec2(5, 65), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 25), ImGuiCond_Always);
    ImGui::Begin("Window1##2", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    ImGui::Dummy(ImVec2(21, 0));
    ImGui::Text(u8"选择摄像头:");

    ImGui::SameLine();
	vector<string> devList;
	m_pCameraBase->getDeviceNameList(devList);
    if (devList.size())
    {
        ImGui::PushItemWidth(244);
        static std::string item_current = devList[0];
        if (ImGui::BeginCombo("##slect camera1", item_current.c_str())) // The second parameter is the label previewed before opening the combo.
        {
            for (unsigned int n = 0; n <devList.size(); n++)
            {
                bool is_selected = (item_current == devList[n]);
                if (ImGui::Selectable(devList[n].c_str(), is_selected))
                {
                    item_current = devList[n];
                    if (m_selectedCamera != n)
                    {
                        m_selectedCamera = n;
                        if (m_pCameraBase->isInit())
                        {
							m_pCameraBase->closeCamera();
							m_pCameraBase->initCamera(m_resolutionWidth, m_resolutionHeight, n);
                        }
                    }
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
    }
    else
    {
        ImGui::PushItemWidth(244);
        if (ImGui::BeginCombo("##slect camera122", u8"未检测到摄像头")) // The second parameter is the label previewed before opening the combo.
            ImGui::EndCombo();
        ImGui::PopItemWidth();
    }

    ImGui::End();
}

//渲染
void OrangeFilterUI::RenderFrame()
{
    bool show_frame = true;
	bool ret = false;
    cv::Mat frameMat = m_pCameraBase->getCameraData();
	if (!frameMat.empty())
	{
		static cv::Mat processedFrame = frameMat.clone();
		cv::cvtColor(frameMat, processedFrame, cv::COLOR_BGR2RGBA);
		static cv::Mat dstFrame = processedFrame.clone();
		auto errorcode = glGetError();
		ret = m_orangeFilterRender->applyFrame(processedFrame.data, dstFrame.data, processedFrame.cols, processedFrame.rows);
	}

    GLuint textureID = m_orangeFilterRender->getOutTextureID();
    m_FPS = get_fps();
    m_renderTime = 1000.f / (float)m_FPS;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(58.f / 255.f, 61.f / 255.f, 82.f / 255.f, 1.f));
    ImGui::SetNextWindowPos(ImVec2(5, 100), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 520), ImGuiCond_Always);
    ImGui::Begin("Frame Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollWithMouse);
    
	if (0 != textureID && ret)
    {
		float fRatio = (float)m_resolutionWidth / (float)m_resolutionHeight;
		int needHeight = 800 / fRatio;
        ImGui::Image((void *)(intptr_t)textureID, ImVec2(800, needHeight));
    }
    ImGui::End();
    ImGui::PopStyleColor();
}

void OrangeFilterUI::ShowDebugInfoWindow()
{
    if (m_showDegubInfoWindow)
    {
       
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(82.f / 255.f, 88.f / 255.f, 112.f / 255.f, .78f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(82.f / 255.f, 88.f / 255.f, 112.f / 255.f, 0.f));
        ImGui::SetNextWindowPos(ImVec2(26 , 142 ), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(154 , 98 ), ImGuiCond_Always);
        ImGui::Begin("debugInfo##2223", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_NoInputs*/);
        ImGui::TextColored(ImColor(0, 255, 0, 255), "FPS:%d", m_FPS);
        ImGui::TextColored(ImColor(0, 255, 0, 255), "Resolution:%d*%d", m_resolutionWidth, m_resolutionHeight);
        ImGui::TextColored(ImColor(0, 255, 0, 255), "RenderTime:%.2fms", m_renderTime);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(82.f / 255.f, 88.f / 255.f, 112.f / 255.f, .0f));
        if (LayoutButton(ImVec2(100, 0), ImVec2(37, 24), u8"关闭"))
        {
            m_showDegubInfoWindow = false;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(82.f / 255.f, 88.f / 255.f, 112.f / 255.f, .78f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(82.f / 255.f, 88.f / 255.f, 112.f / 255.f, .0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(82.f / 255.f, 88.f / 255.f, 112.f / 255.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::SetNextWindowPos(ImVec2(26 , 142 ), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(120 , 40 ), ImGuiCond_Always);
        ImGui::Begin("debugInfo##23", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_NoInputs*/);
        if (LayoutButton(ImVec2(0, 0), ImVec2(100, 25), u8"显示性能参数"))
        {
            m_showDegubInfoWindow = true;
        }
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
}

void OrangeFilterUI::ShowStickerWindow()
{
    ImGui::SetNextWindowPos(ImVec2(5, 600), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(800, 160), ImGuiCond_Always);
    ImGui::Begin("Sticker Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollWithMouse);
    for (int i = 0; i < 6; i++)
    {
        ImGui::Spacing();
    }
    std::string assertsPath;
    PathJoinA(assertsPath, m_runPath, "Assets");
    for (int i = 0; i < m_categoryBundles.size(); i++)
    {
        if (m_bundleCategory == i)
        {
            if (LayoutImageButtonWithText(ImVec2(0.f, 27.f), ImVec2(52, 52), Texture::createTextureFromFile("selected.png", false)->getTextureID(),
                Texture::createTextureFromFile("selected.png", false)->getTextureID(), m_categoryBundles[i].name.c_str()))
            {
                m_bundleCategory = -1;
                m_showItemSelectWindow = false;
            }
        }
        else
        {
            std::string  categoryID = std::to_string(m_categoryBundles[i].id);
            std::string categoryPath;
            PathJoinA(categoryPath, assertsPath, categoryID);
            std::string iconHoverFileName;
            PathJoinA(iconHoverFileName, categoryPath, "seletedCategoryThumb.png");
            std::string iconNorFileName;
            PathJoinA(iconNorFileName, categoryPath,"categoryThumb.png");

            if (LayoutImageButtonWithText(ImVec2(0.f, 27.f), ImVec2(52, 52), Texture::createTextureFromFile(iconNorFileName, false)->getTextureID(),
                Texture::createTextureFromFile(iconHoverFileName, false)->getTextureID(), m_categoryBundles[i].name.c_str()))
            {
				m_bundleCategory = i;
				m_showItemSelectWindow = true;
            }
        }
        ImGui::SameLine(0.f, 27.f );
    }

	// 滤镜进度条
	if (m_bundleCategory == 0 && m_nSelectFilterNum >= 0 && m_curRenderItemUIID != 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(120.f / 255.f, 136.f / 255.f, 234.f / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(224.f / 255.f, 227.f / 255.f, 238.f / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(150.f / 255.f, 157.f / 255.f, 181.f / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
		
		ImGui::Spacing();
		ImGui::PushItemWidth(300);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));
		
		ImGui::SameLine(350);
		ImGui::SliderFloat(string_To_UTF8(m_categoryBundles[0].effectItemList[m_nSelectFilterNum].name).c_str(), &m_fFilterIntensity, 1.0f, 100.0f);
		ImGui::PopItemWidth();

		//根据进度条设置滤镜的参数
		OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EffectParamType(GetSelectFilterParamType(m_nSelectFilterNum)), m_fFilterIntensity);

		//ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
    ImGui::End();
}

void OrangeFilterUI::ShowItemSelectWindow()
{

    if (m_showItemSelectWindow)
    {
        //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(252.f / 255.f, 253.f / 255.f, 255.f / 255.f, .70f));
		//弹出窗颜色，ImVec4 对应RGB+透明度 的值
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(128.f / 255.f, 128.f / 255.f, 128.f / 255.f, 0.80f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(252.f / 255.f, 253.f / 255.f, 255.f / 255.f, .0f));
        ImGui::SetNextWindowPos(ImVec2(5 , 520), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(800 , 110), ImGuiCond_Always);  //弹出窗大小与位置
        ImGui::Begin("itemSelect##1563", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

        std::string assertsPath;
        PathJoinA(assertsPath, m_runPath, "Assets");
		std::string strEffectsPath;
		PathJoinA(strEffectsPath, m_runPath, "effects");
        std::string  categoryID = std::to_string(m_categoryBundles[m_bundleCategory].id);
        std::string categoryPath;
        PathJoinA(categoryPath, assertsPath, categoryID);

        for (unsigned int i = 0; i < m_categoryBundles[m_bundleCategory].effectItemList.size(); i++)
        {
			ImGui::BeginGroup();
            std::string itemID = std::to_string(m_categoryBundles[m_bundleCategory].effectItemList[i].id);
            std::string itemDir;
            PathJoinA(itemDir, categoryPath, itemID);
            std::string itemPngPath;
            std::string itemZipPath;
            PathJoinA(itemPngPath, itemDir, "thumb.png");
            //PathJoinA(itemZipPath, itemDir, "effect.zip");
			PathJoinA(itemZipPath, strEffectsPath, string_To_UTF8(m_categoryBundles[m_bundleCategory].effectItemList[i].effectName));
			
            Texture::SharedPtr tex = NULL;
            if (doesFileExist(itemPngPath))
            {
                tex = Texture::createTextureFromFile(itemPngPath, false);
            }
            //if can't find the icon
            if (!tex)
            {
                tex = Texture::createTextureFromFile("icon_Movebutton_nor.png", false);
            }

            if (ImGui::ImageRoundButton((ImGuiID&)m_curRenderItemUIID, tex->getTextureID(), ImVec2(50 , 50 )))
            {
				const char* effectName = m_categoryBundles[m_bundleCategory].name.c_str();
				static bool _ret = false;
				if (m_bundleCategory == 0) // lookuptable
				{
					if (m_nSelectFilterNum != i || m_curRenderItemUIID == 0)
					{//把上一次的滤镜关闭，实际上可以重叠，demo都采用互斥。
						OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::EffectType(OrangeHelper::ET_FilterHoliday + m_nSelectFilterNum), false);
						m_nSelectFilterNum = -1;
					}
					_ret = OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::EffectType(OrangeHelper::ET_FilterHoliday + i),
						m_curRenderItemUIID != 0 ? true : false);
					if (_ret)
					{
						// 开启滤镜功能后，主动去获取默认或者上一次设置的参数；
						m_nSelectFilterNum = i;
						int nDefParam = OrangHelperManager::getOrangHelper()->getEffectParam(OrangeHelper::EffectParamType(GetSelectFilterParamType(m_nSelectFilterNum)));
						if (nDefParam)m_fFilterIntensity = nDefParam;
					}
				}
				else if (doesFileExist(itemZipPath))
				{
					if (m_strSelectEffectsPath != itemZipPath || m_curRenderItemUIID == 0)
					{
						_ret = OrangHelperManager::getOrangHelper()->releaseSticker(m_strSelectEffectsPath);
					}
					_ret = OrangHelperManager::getOrangHelper()->enableSticker(itemZipPath, m_curRenderItemUIID != 0 ? true : false);
					if (_ret)
					{
						m_strSelectEffectsPath = itemZipPath;
					}
				}
				else
				{
					printf_s("Effect zip is not exit!");
					_ret = false;
				}

				if (0 != m_curRenderItemUIID && _ret == false)
				{
					//未授权
					ImGui::OpenPopup(u8"授权提示");
				}
            }

			string str = m_categoryBundles[m_bundleCategory].effectItemList[i].name;
			if(str.length()>0)ImGui::Text(string_To_UTF8(str).c_str());
			ImGui::EndGroup();

            ImGui::SameLine(0.f, 27.f);
			
        }
		
		if (ImGui::BeginPopup(u8"授权提示", ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Text(u8"该特效包不可用，请确认是否被授权使用。");
			ImGui::EndPopup();
		}

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
}

void OrangeFilterUI::ShowSettingWindow(bool &bOpen)
{
	if (!ImGui::IsPopupOpen(u8"设置"))
		ImGui::OpenPopup(u8"设置");
	if (ImGui::BeginPopupModal(u8"设置", &bOpen, ImGuiWindowFlags_NoResize))
	{
		ImGui::SetWindowSize(ImVec2(400, 300));

#pragma region 包名
		ImGui::Spacing();
		//read file
		std::string static name = "";
		if(name.length() < 1)
			readBuildDisplayName("package.ini" ,name);
		if (name.length() < 1)
		{
			name = "null";
		}
		ImGui::Text(u8"包名: %s", name.c_str());
#pragma endregion
#pragma region 版本
		ImGui::Spacing();
		ImGui::Text(u8"版本: %ws", OFCAMERA_VERSION);
#pragma endregion

#pragma region 崩溃测试
		static bool isShowCrashView = false;
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Dummy(ImVec2(30, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 100, 100, 255));
		if (ImGui::Button(u8"崩溃测试", ImVec2(80, 0)))
		{
			isShowCrashView = true;
		}
		ImGui::PopStyleColor();
		if (isShowCrashView) 
		{
			if (!ImGui::IsPopupOpen(u8"崩溃确认"))
				ImGui::OpenPopup(u8"崩溃确认");
			bool bTemp = true;
			if (ImGui::BeginPopupModal(u8"崩溃确认", &bTemp, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
			{
				ImGui::SetWindowSize(ImVec2(200, 150));
				ImGui::TextColored(ImVec4(255, 0, 0, 255), u8"警告:\n");
				ImGui::TextColored(ImVec4(255, 0, 0, 255), u8"确认将导致该应用崩溃闪退");
				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::SameLine(20);
				if (ImGui::Button(u8"确认", ImVec2(60, 0))) 
				{
					isShowCrashView = false;
					//TODO::crash
					//throw("OFCamera crash test");
					const char * p[1] = {""};
					for (int i = 0; i < 10; ++i)
						OF_SetVenusModels(i, 0, p, 10);
					m_orangeFilterRender->unInit();
				}
				ImGui::SameLine(120);
				if (ImGui::Button(u8"取消", ImVec2(60, 0)))
				{
					isShowCrashView = false;
				}
				ImGui::EndPopup();
			}
		}
#pragma endregion


		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 120, ImGui::GetWindowHeight() - 50));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(200, 200, 200, 255));
		if (ImGui::Button(u8"关闭", ImVec2(80, 0)))
		{
			bOpen = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();
		ImGui::EndPopup();
	}
}

void OrangeFilterUI::ShowBeautyTabs()
{
    ImGui::SetNextWindowPos(ImVec2(810, 65), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(465, 630), ImGuiCond_Always);

    ImGui::Begin("BeautyTabs", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
   
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(120.f / 255.f, 136.f / 255.f, 234.f / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(224.f / 255.f, 227.f / 255.f, 238.f / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(150.f / 255.f, 157.f / 255.f, 181.f / 255.f, 1.f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));
    static OF_BeautyData beautyPara = {0.22f, 0.7f};
    ImGui::Spacing();

	static ImGuiComboFlags Checkboxflags = 0;
	if (ImGui::CheckboxFlags(u8"清晰美颜", (unsigned int*)&Checkboxflags, ImGuiComboFlags_NoArrowButton))
	{
		if (0 != Checkboxflags)
		{
			OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_BasicBeauty, false);
			int nDefIntensity = OrangHelperManager::getOrangHelper()->getEffectParam(OrangeHelper::EP_BasicBeautyIntensity5);
			int nDefOpacity = OrangHelperManager::getOrangHelper()->getEffectParam(OrangeHelper::EP_BasicBeautyOpacity5);
			if(0 != nDefIntensity) beautyPara.whiteningIntensity = nDefIntensity / 100.00f;
			if (0 != nDefOpacity) beautyPara.beautyIntensity = nDefOpacity / 100.00f;
		}
		OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_BasicBeauty5, (bool)Checkboxflags);
		Checkboxflags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
	}
	ImGui::SameLine(120);
	if (ImGui::CheckboxFlags(u8"朦胧美颜", (unsigned int*)&Checkboxflags, ImGuiComboFlags_NoPreview))
	{
		if (0 != Checkboxflags)
		{
			OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_BasicBeauty5, false);
			int nDefIntensity = OrangHelperManager::getOrangHelper()->getEffectParam(OrangeHelper::EP_BasicBeautyIntensity);
			int nDefOpacity = OrangHelperManager::getOrangHelper()->getEffectParam(OrangeHelper::EP_BasicBeautyOpacity);
			if (0 != nDefIntensity) beautyPara.whiteningIntensity = nDefIntensity / 100.00f;
			if (0 != nDefOpacity) beautyPara.beautyIntensity = nDefOpacity / 100.00f;
		}
		OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_BasicBeauty, (bool)Checkboxflags);
		Checkboxflags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both
	}
	

	static bool isShowSettingView = false;
	ImGui::SameLine(ImGui::GetWindowWidth() - 80);
	
	if (ImGui::Button(u8"设置", ImVec2(50, 0))) 
	{
		isShowSettingView = true;
	}
	if (isShowSettingView) 
	{
		ShowSettingWindow(isShowSettingView);
	}

    ImGui::Spacing();
    ImGui::SliderFloat(u8"美肤", &beautyPara.beautyIntensity, 0.0f, 1.0f);
    ImGui::Spacing();
    ImGui::SliderFloat(u8"亮白", &beautyPara.whiteningIntensity, 0.0f, 1.0f);
    ImGui::Spacing();

	
    if (Checkboxflags == ImGuiComboFlags_NoArrowButton) // 根据进度条设置清晰美颜参数
    {	
		OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_BasicBeautyIntensity5, beautyPara.whiteningIntensity * 100);  //美白
		OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_BasicBeautyOpacity5, beautyPara.beautyIntensity * 100);   //美肤		

    }else if (Checkboxflags == ImGuiComboFlags_NoPreview) //根据进度条设置朦胧美颜参数
	{
		OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_BasicBeautyIntensity, beautyPara.whiteningIntensity * 100);  //美白
		OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_BasicBeautyOpacity, beautyPara.beautyIntensity * 100);   //美肤
	}

    ImGui::Separator();

	static OF_FaceLiftingData faceliftingPara =
	{
		0.4f, // basicTypeIntensity;
		0.3f, // thinFaceIntensity;
		0.3f, // smallFaceIntensity;
		0.2f, // squashedFaceIntensity;
		0.0f, // foreheadLiftingIntensity;
		0.0f, // wideForeheadLiftingIntensity;
		0.28f, // bigSmallEyeIntensity;
		0.0f, // eyeOffestIntensity;
		0.0f, // eyesRotationIntensity;
		0.28f, // thinNoseIntensity;
		0.0f, // longNoseIntensity;
		0.0f, // thinNoseBridgeIntensity;
	   -0.3f, // thinMouthIntensity;
		0.0f, // movemouthIntensity;
		0.08f, // chinLiftingIntensity;
	};

	ImGui::Checkbox(u8"开启美型", &g_effect[2].isEnable);

	static char* items[] = { u8"基础美型", u8"高级美型" };
	if (ImGui::BeginCombo("##combo", items[g_beautyType]))
	{
		for (int n = 0; n < 2; n++)
		{
			bool is_selected = (g_beautyType == n);
			if (ImGui::Selectable(items[n], is_selected))
			{
				g_beautyType = n;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	
	ImGui::Spacing();
	if (g_beautyType == 0)  // basic
	{
		ImGui::SliderFloat(u8"强度", &faceliftingPara.basicTypeIntensity, 0.0f, 1.0f);
		OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_BasicBeautyType, g_effect[2].isEnable);
		if (g_effect[2].isEnable)
		{
			OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_SeniorBeautyType, false);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_BasicTypeIntensity, faceliftingPara.basicTypeIntensity * 100);
		}
	}
	else
	{
		ImGui::SliderFloat(u8"窄脸", &faceliftingPara.thinFaceIntensity, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"小脸", &faceliftingPara.smallFaceIntensity, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"瘦颧骨", &faceliftingPara.squashedFaceIntensity, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"额高", &faceliftingPara.foreheadLiftingIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"额宽", &faceliftingPara.wideForeheadLiftingIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"大眼", &faceliftingPara.bigSmallEyeIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"眼距", &faceliftingPara.eyeOffestIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"眼角", &faceliftingPara.eyesRotationIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"瘦鼻", &faceliftingPara.thinNoseIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"长鼻", &faceliftingPara.longNoseIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"窄鼻梁", &faceliftingPara.thinNoseBridgeIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"瘦嘴", &faceliftingPara.thinMouthIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"嘴位", &faceliftingPara.movemouthIntensity, -1.0f, 1.0f);
		ImGui::Spacing();
		ImGui::SliderFloat(u8"下巴", &faceliftingPara.chinLiftingIntensity, -1.0f, 1.0f);

		OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_SeniorBeautyType, g_effect[2].isEnable);
		if (g_effect[2].isEnable)
		{
			OrangHelperManager::getOrangHelper()->enableEffect(OrangeHelper::ET_BasicBeautyType, false);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeThinFaceIntensity, faceliftingPara.thinFaceIntensity * 100);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeSmallFaceIntensity, faceliftingPara.smallFaceIntensity * 100);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeSquashedFaceIntensity, faceliftingPara.squashedFaceIntensity * 100);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeForeheadLiftingIntensity, faceliftingPara.foreheadLiftingIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeWideForeheadIntensity, faceliftingPara.wideForeheadLiftingIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeBigSmallEyeIntensity, faceliftingPara.bigSmallEyeIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeEyesOffsetIntensity, faceliftingPara.eyeOffestIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeEyesRotationIntensity, faceliftingPara.eyesRotationIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeThinNoseIntensity, faceliftingPara.thinNoseIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeLongNoseIntensity, faceliftingPara.longNoseIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeThinNoseBridgeIntensity, faceliftingPara.thinNoseBridgeIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeThinmouthIntensity, faceliftingPara.thinMouthIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeMovemouthIntensity, faceliftingPara.movemouthIntensity * 50);
			OrangHelperManager::getOrangHelper()->setEffectParam(OrangeHelper::EP_SeniorTypeChinLiftingIntensity, faceliftingPara.chinLiftingIntensity * 50);
		}
	}

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::End();
}

void OrangeFilterUI::RendererRun()
{
    // Setup Platform/Renderer bindings
    bool opengLFlag = ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    bool opengLFlag1 = ImGui_ImplOpenGL3_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool showTitleUI = true;
    bool showCameraList = true;
    bool showStickerWin = true;
    bool showBeautyTab = true;
    bool showDebugInfo = true;

	float fEscapeMs = 0;
	float fFrameMs = 1.0f / 30;
	clock_t lastClock = clock();
    // Main loop
    while (!glfwWindowShouldClose(m_window))
    {
		clock_t curClock = clock();
		fEscapeMs +=  float(curClock - lastClock)/ CLOCKS_PER_SEC;
		lastClock = curClock;
		if (fEscapeMs < fFrameMs)
		{ 
			continue;
		}
		fEscapeMs -= fFrameMs;
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showTitleUI)
        {
            ShowDemoTitleImage();
        }
        //camera window
        if (showCameraList)
        {
            ShowCameraListWindow();
        }
        if (m_isInitOrangeFilterSuccess)
        {
            RenderFrame();
        }

        if (showDebugInfo)
        {
            ShowDebugInfoWindow();
        }

        if (showStickerWin)
        {
            ShowStickerWindow();
            ShowItemSelectWindow();
        }

        if (showBeautyTab)
        {
            ShowBeautyTabs();
        }
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore current shader using the commented lines below.
        GLint last_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        glUseProgram(0);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glUseProgram(last_program);

        glfwMakeContextCurrent(m_window);
        glfwSwapBuffers(m_window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_orangeFilterRender->unInit();
    cv::destroyAllWindows();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

//滤镜参数对应EffectParamType的位置，由于之前版本与后面版本 中间
//增加了其他参数，需要调整。
size_t OrangeFilterUI::GetSelectFilterParamType(size_t nBase)
{
	size_t nSelectFilterType = OrangeHelper::EP_FilterHolidayIntensity + nBase;
	if (nSelectFilterType > OrangeHelper::EP_FilterTenderIntensity)
	{
		nSelectFilterType += 15; // 这里是因为前5个滤镜与后21个滤镜中间相隔了15个其他类型
	}
	return nSelectFilterType;
}