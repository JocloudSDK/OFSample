#pragma once
/*************
YY OrangHelper 管理设配类，单例对象

****************/
#include <mutex>
#include "OrangeHelper.h"

class OrangHelperManager
{
public:
	~OrangHelperManager();

	static OrangHelperManager *getOrangHelper();

	bool createContext(
		const std::string& serialNumber,
		const std::string& licensePath,
		const std::string& resDir,
		OrangeHelper::VenusType aiType = OrangeHelper::VN_All);

	void destroyContext();

	bool isContextValid();

	bool enableEffect(OrangeHelper::EffectType effectType, bool enabled);

	bool releaseEffect(OrangeHelper::EffectType effectType);

	bool enableSticker(const std::string& path, bool enabled);

	bool releaseSticker(const std::string& path);

	bool enableGesture(const std::string& path, bool enabled);

	bool releaseGesture(const std::string& path);

	int getEffectParam(OrangeHelper::EffectParamType paramType);

	bool getEffectParamDetail(OrangeHelper::EffectParamType paramType, OrangeHelper::EffectParam& paramVal);

	bool setEffectParam(OrangeHelper::EffectParamType paramType, int value);

	bool updateFrameParams(const OrangeHelper::GLTexture& textureIn, const OrangeHelper::GLTexture& textureOut, const OrangeHelper::ImageInfo& image);

	bool checkStickerResult(const std::vector<std::string>& paths, std::vector<int>& results);

	bool setLogLevel(int level);

	bool setLogCallback(void(*callback)(const char* msg));
private:
	OrangHelperManager();
	std::recursive_mutex g_uiMutex;
	OrangeHelper* m_orangeHelper;
};

