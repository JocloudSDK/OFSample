#include "OrangHelperManager.h"
#include <assert.h>
#include "dwbase/log.h"

const char TAGOFHelper[] = "OrangHelper";

OrangHelperManager::OrangHelperManager()
{
	m_orangeHelper = new OrangeHelper();
}

OrangHelperManager::~OrangHelperManager()
{
	if (NULL != m_orangeHelper)
	{
		delete m_orangeHelper;
		m_orangeHelper = NULL;
	}
}

OrangHelperManager* OrangHelperManager::getOrangHelper()
{
	static OrangHelperManager the;
	return &the;
}

bool OrangHelperManager::createContext(
	const std::string& serialNumber,
	const std::string& licensePath,
	const std::string& resDir,
	OrangeHelper::VenusType aiType)
{
	assert(NULL != m_orangeHelper);
	LogFinal(TAGOFHelper) << ("call createContext");
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	return m_orangeHelper->createContext(serialNumber, licensePath, resDir, aiType);
}

void OrangHelperManager::destroyContext()
{
	assert(NULL != m_orangeHelper);
	LogFinal(TAGOFHelper) << ("call destroyContext");
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	m_orangeHelper->destroyContext();
}

bool OrangHelperManager::isContextValid() 
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	bool bResult = m_orangeHelper->isContextValid();
	LogFinal(TAGOFHelper) << ("end isContextValid bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::enableEffect(OrangeHelper::EffectType effectType, bool enabled)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	//LogFinal(TAGOFHelper) << ("call enableEffect [effectType=%d,enabled=%d]", effectType, enabled);
	bool bResult = m_orangeHelper->enableEffect(effectType, enabled);
	//LogFinal(TAGOFHelper) << ("end enableEffect bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::releaseEffect(OrangeHelper::EffectType effectType)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	//LogFinal(TAGOFHelper) << ("call releaseEffect [effectType=%d]", effectType);
	bool bResult = m_orangeHelper->releaseEffect(effectType);
	//LogFinal(TAGOFHelper) << ("end releaseEffect bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::enableSticker(const std::string& path, bool enabled)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	//LogFinal(TAGOFHelper) << ("call enableSticker [path=%d,enabled=%d]", path, enabled);
	bool bResult = m_orangeHelper->enableSticker(path, enabled);
	//LogFinal(TAGOFHelper) << ("end enableSticker bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::releaseSticker(const std::string& path)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	//LogFinal(TAGOFHelper) << ("call releaseSticker [path=%d]", path);
	bool bResult = m_orangeHelper->releaseSticker(path);
	//(TAGOFHelper) << ("end releaseSticker bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::enableGesture(const std::string& path, bool enabled)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	LogFinal(TAGOFHelper) << ("call enableGesture [path=%d,enabled=%d]", path, enabled);
	bool bResult = m_orangeHelper->enableGesture(path, enabled);
	LogFinal(TAGOFHelper) << ("end enableGesture bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::releaseGesture(const std::string& path)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	LogFinal(TAGOFHelper) << ("call releaseGesture [path=%d]", path);
	bool bResult = m_orangeHelper->releaseGesture(path);
	LogFinal(TAGOFHelper) << ("end releaseGesture bResult=%d", bResult);
	return bResult;
}

int OrangHelperManager::getEffectParam(OrangeHelper::EffectParamType paramType)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);

	//LogFinal(TAGOFHelper) << ("call getEffectParam [paramType=%d]", paramType);
	int nResult = m_orangeHelper->getEffectParam(paramType);
	//LogFinal(TAGOFHelper) << ("end getEffectParam bResult=%d", nResult);
	return nResult;
}

bool OrangHelperManager::getEffectParamDetail(OrangeHelper::EffectParamType paramType, OrangeHelper::EffectParam& paramVal)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);

	LogFinal(TAGOFHelper) << ("call getEffectParamDetail [paramType=%d,curVal=%d,maxVal=%d,minVal=%d,defVal=%d]", paramType, 
		paramVal.curVal, paramVal.maxVal, paramVal.minVal, paramVal.defVal);
	bool bResult = m_orangeHelper->getEffectParamDetail(paramType, paramVal);
	LogFinal(TAGOFHelper) << ("end getEffectParamDetail bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::setEffectParam(OrangeHelper::EffectParamType paramType, int value)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);

	//LogFinal(TAGOFHelper) << ("call setEffectParam [paramType=%d,value=%d]", paramType, value);
	bool bResult =  m_orangeHelper->setEffectParam(paramType, value);
	//LogFinal(TAGOFHelper) << ("end setEffectParam bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::updateFrameParams(const OrangeHelper::GLTexture& textureIn, const OrangeHelper::GLTexture& textureOut, const OrangeHelper::ImageInfo& image)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	return m_orangeHelper->updateFrameParams(textureIn, textureOut, image);
}

bool OrangHelperManager::checkStickerResult(const std::vector<std::string>& paths, std::vector<int>& results)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);

	bool bResult = m_orangeHelper->checkStickerResult(paths, results);
	LogFinal(TAGOFHelper) << ("end checkStickerResult bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::setLogLevel(int level)
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);

	LogFinal(TAGOFHelper) << ("call setLogLevel [level=%d]", level);
	bool bResult = m_orangeHelper->setLogLevel(level);
	LogFinal(TAGOFHelper) << ("end setLogLevel bResult=%d", bResult);
	return bResult;
}

bool OrangHelperManager::setLogCallback(void(*callback)(const char* msg))
{
	assert(NULL != m_orangeHelper);
	std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
	return m_orangeHelper->setLogCallback(callback);
}