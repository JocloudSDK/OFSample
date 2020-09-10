#include "OrangeHelper.h"
#include <stdio.h>
#include <memory.h>
#include <cmath>
#include <algorithm>

using namespace std;

bool OrangeHelper::createContext(const std::string& serialNumber,
    const std::string& licensePath, const std::string& resDir, VenusType aiType)
{
    OF_Result ret = OF_CheckSerialNumber(serialNumber.c_str(), licensePath.c_str());
    if (ret != OF_Result_Success)
    {
        printf("check orangefilter license failed. ret = [%i]", ret);
        return false;
    }
    
    ret = OF_CreateContext(&_context, nullptr);
    if (ret != OF_Result_Success)
    {
        printf("create of context failed");
        return false;
    }
    
    _resDir = resDir;
    
    std::string defaultEffects[] =
    {
        "beauty_basic.zip",
        "beauty5_basic.zip",
        "basicthinface.zip",
        "faceliftingopt.zip",
        "filter_holiday.zip",
        "filter_clear.zip",
        "filter_warm.zip",
        "filter_fresh.zip",
        "filter_tender.zip",
        "lengse-adaier.zip",
        "lengse-danya.zip",
        "lengse-jiazhou.zip",
        "lengse-mojito.zip",
        "nuanse-fennen.zip",
        "nuanse-qingchun.zip",
        "nuanse-tianmei.zip",
        "nuanse-wennuan.zip",
        "nuanse-yangguang.zip",
        "shaonv-baixi.zip",
        "shaonv-luolita.zip",
        "shaonv-wenyi.zip",
        "zengqiang-fengqing.zip",
        "zengqiang-kekou.zip",
        "zengqiang-mingliang.zip",
        "zengqiang-nuolan.zip",
        "zengqiang-shuimitao.zip",
        "zhigan-bingqiling.zip",
        "zhigan-jiaopian.zip",
        "zhigan-zazhi.zip",
    };
    const int defaultEffectNum = 29;
    for (int i = 0; i < defaultEffectNum; i++)
    {
        _effects.push_back(EffectInfo(resDir + "/effects/" + defaultEffects[i], 0, false));
    }
    
    memset(&_frameData, 0, sizeof(OF_FrameData));
    
    std::string aiModelDir = resDir + "/venus_models";
    if (aiType & VenusType::VN_Face)
    {
        std::string faceModelPathArr[] =
        {
            aiModelDir + "/face/Face_Key_Points_0.vnmodel",
            aiModelDir + "/face/Face_Key_Points_1.vnmodel",
            aiModelDir + "/face/Face_Key_Points_2.vnmodel",
			aiModelDir + "/face/Face_Key_Points_3.vnmodel",
			aiModelDir + "/face/Face_Key_Points_4.vnmodel",
            aiModelDir + "/face/Face_Key_Points_5.vnmodel",
        };
        const int faceModelNum = 6;
        const char* faceModelPaths[faceModelNum];
        for (int i = 0; i < faceModelNum; i++)
        {
            faceModelPaths[i] = faceModelPathArr[i].c_str();
        }
        
        ret = OF_SetVenusModels(_context, OF_VenusDetectionType_Face, faceModelPaths, faceModelNum);
        if (ret != OF_Result_Success)
        {
            printf("set venus face models failed");
            return false;
        }
    }
    
    if (aiType & VenusType::VN_BodySegment)
    {
        std::string bodySegmentModelPathArr[] =
        {
            aiModelDir + "/segment/Background_Segmentation_0.dat",
            aiModelDir + "/segment/Background_Segmentation_1.dat",
        };
        const int bodySegmentModelNum = 2;
        const char *argBodySegmentDetection[bodySegmentModelNum];
        for (int i = 0; i < bodySegmentModelNum; i++)
        {
            argBodySegmentDetection[i] = bodySegmentModelPathArr[i].c_str();
        }
        
        ret = OF_SetVenusModels(_context, OF_VenusDetectionType_BodySegment, argBodySegmentDetection, bodySegmentModelNum);
        if (ret != OF_Result_Success)
        {
            printf("set venus body segment models failed");
            return false;
        }
    }
    
    if (aiType & VenusType::VN_Gesture)
    {
        std::string gestureModelPathArr[] =
        {
            aiModelDir + "/gesture/Gesture_Recognition_0.vnmodel",
            aiModelDir + "/gesture/Gesture_Recognition_1.vnmodel",
            aiModelDir + "/gesture/Gesture_Recognition_2.vnmodel",
        };
        const int gestureModelNum = 3;
        const char *argGestureDetection[gestureModelNum];
        for (int i = 0; i < gestureModelNum; i++)
        {
            argGestureDetection[i] = gestureModelPathArr[i].c_str();
        }
        
        ret = OF_SetVenusModels(_context, OF_VenusDetectionType_Gesture, argGestureDetection, gestureModelNum);
        if (ret != OF_Result_Success)
        {
            printf("set venus gesture models failed");
            return false;
        }
    }

    return true;
}

void OrangeHelper::destroyContext()
{
    for (auto& info : _effects)
    {
        if (info.effectId > 0)
        {
            OF_DestroyEffect(_context, info.effectId);
            info.effectId = OF_INVALID_HANDLE;
        }
    }
    
    if (_sticker.effectId > 0)
    {
        OF_DestroyEffect(_context, _sticker.effectId);
        _sticker.effectId = OF_INVALID_HANDLE;
    }
    OF_DestroyContext(_context);
}

bool OrangeHelper::isContextValid()
{
    return _context != OF_INVALID_HANDLE;
}

bool OrangeHelper::enableEffect(EffectType effectType, bool enabled)
{
    int idx = (int)effectType; // use enum value as vector index
    if (_effects[idx].effectId == OF_INVALID_HANDLE)
    {
        string path = _effects[idx].path;
        OF_Result ret = OF_CreateEffectFromPackage(_context, path.c_str(), (OFHandle*)&_effects[idx].effectId);
        if (ret != OF_Result_Success)
        {
            printf("load effect failed");
            return false;
        }
    }
    _effects[idx].enabled = enabled;
    return true;
}

bool OrangeHelper::releaseEffect(EffectType effectType)
{
    int idx = (int)effectType; // use enum value as vector index
    if (_effects[idx].effectId != OF_INVALID_HANDLE)
    {
        OF_DestroyEffect(_context, _effects[idx].effectId);
        _effects[idx].effectId = OF_INVALID_HANDLE;
		_effects[idx].enabled = false;
    }
    return true;
}

bool OrangeHelper::enableSticker(const std::string& path, bool enabled)
{
    auto it = std::find_if(_effects.begin(), _effects.end(),
                           [&](EffectInfo i) { return i.path == path; });
    if (it == _effects.end())
    {
        int effectId = 0;
        OF_Result ret = OF_CreateEffectFromPackage(_context, path.c_str(), (OFHandle*)&effectId);
        if (ret != OF_Result_Success)
        {
            printf("load sticker failed");
            return false;
        }
        _effects.push_back(EffectInfo(path, effectId, enabled));
        return true;
    }
    else
    {
        it->enabled = enabled;
    }
    return true;
}

bool OrangeHelper::releaseSticker(const std::string &path)
{
    auto it = std::find_if(_effects.begin(), _effects.end(),
                           [&](EffectInfo i) { return i.path == path; });
    if (it != _effects.end())
    {
        if (it->effectId != OF_INVALID_HANDLE)
        {
            OF_DestroyEffect(_context, it->effectId);
        }
        _effects.erase(it);
        return true;
    }
    return false;
}

bool OrangeHelper::enableGesture(const std::string &path, bool enabled)
{
    return enableSticker(path, enabled);
}

bool OrangeHelper::releaseGesture(const std::string &path)
{
    return releaseSticker(path);
}

int OrangeHelper::getEffectParam(EffectParamType paramType)
{
    int filterId = getFilterId(paramType);
    if (filterId > 0)
    {
        string paramName = getParamName(paramType);
        OF_Param* param;
        OF_GetFilterParamData(_context, filterId, paramName.c_str(), &param);
        
        float range = param->data.paramf->maxVal - param->data.paramf->minVal;
        return std::round((100 * param->data.paramf->val / range));
    }
    return 0;
}

bool OrangeHelper::getEffectParamDetail(EffectParamType paramType, EffectParam& paramVal)
{
    int filterId = getFilterId(paramType);
    if (filterId > 0)
    {
        string paramName = getParamName(paramType);
        OF_Param* param;
        OF_GetFilterParamData(_context, filterId, paramName.c_str(), &param);
        
        float range = param->data.paramf->maxVal - param->data.paramf->minVal;
        paramVal.curVal = std::round((100 * param->data.paramf->val / range));
        paramVal.minVal = std::round((100 * param->data.paramf->minVal / range));
        paramVal.maxVal = std::round((100 * param->data.paramf->maxVal / range));
        paramVal.defVal = std::round((100 * param->data.paramf->defVal / range));

        return true;
    }
    return false;
}

bool OrangeHelper::setEffectParam(EffectParamType paramType, int value)
{
    int filterId = getFilterId(paramType);
    if (filterId > 0)
    {
        string paramName = getParamName(paramType);
        
        OF_Param* param;
        OF_GetFilterParamData(_context, filterId, paramName.c_str(), &param);
        
        float range = param->data.paramf->maxVal - param->data.paramf->minVal;
        param->data.paramf->val = value / 100.0f * range;
        
        OF_SetFilterParamData(_context, filterId, paramName.c_str(), param);
        return true;
    }
    return false;
}

bool OrangeHelper::updateFrameParams(const GLTexture& textureIn, const GLTexture& textureOut, const ImageInfo& image)
{
    std::vector<OF_Result> results;
    std::vector<OFHandle> ids;
    std::vector<EffectInfo*> infos;
    for (auto& info : _effects)
    {
        info.result = 0;
        if (info.effectId > 0 && info.enabled)
        {
            ids.push_back(info.effectId);
            infos.push_back(&info);
        }
    }
    //printf("wzw effect count %d, ids count %d\n", _effects.size(), ids.size());
    
    _inTex.textureID = textureIn.textureId;
    _inTex.width     = textureIn.width;
    _inTex.height    = textureIn.height;
    _inTex.format    = textureIn.format;
    _inTex.target    = textureIn.target;
    
    _outTex.textureID = textureOut.textureId;
    _outTex.width     = textureOut.width;
    _outTex.height    = textureOut.height;
    _outTex.format    = textureOut.format;
    _outTex.target    = textureOut.target;
    
    _frameData.imageData = image.data;
    _frameData.orientation = image.orientation;
    _frameData.width = image.width;
    _frameData.height = image.height;
    _frameData.format = image.format;
    _frameData.frontCamera = image.frontCamera;
    _frameData.timestamp = image.timestamp;
    _frameData.imageDir = image.dir;
    _frameData.imageDeviceType = 2;
    
    if (ids.size() > 0)
    {
        results.resize(ids.size());
        OF_ApplyFrameBatch(_context, &ids[0], (OFUInt32)ids.size(),
                           &_inTex, 1, &_outTex, 1, &_frameData,
                           &results[0], (OFUInt32)results.size());
        
        for (size_t i = 0; i < results.size(); i++)
        {
            infos[i]->result = (int)results[i];
        }
        return true;
    }
    return false;
}

bool OrangeHelper::checkStickerResult(const std::vector<std::string>& paths, std::vector<int>& results)
{
    results.resize(paths.size());
    for (size_t i = 0; i < paths.size(); i++)
    {
        std::string path = paths[i];
        auto it = std::find_if(_effects.begin(), _effects.end(),
                               [&](EffectInfo i) { return i.path == path; });
        results[i] = (it != _effects.end()) ? it->result : 0;
    }
    return true;
}

bool OrangeHelper::setLogLevel(int level)
{
	OF_Result ret = OF_SetLogLevel(level);
	return ret == OF_Result_Success;
}

bool OrangeHelper::setLogCallback(void(*callback)(const char* msg))
{
	OF_Result ret = OF_SetLogCallback(callback);
	return ret == OF_Result_Success;
}

// private
int OrangeHelper::getEffectId(EffectParamType ep)
{
    EffectType et = ET_SeniorBeautyType;
    switch(ep)
    {
        case EP_BasicBeautyIntensity:  et = ET_BasicBeauty; break;
        case EP_BasicBeautyOpacity:    et = ET_BasicBeauty; break;
        case EP_ClearBeautyIntensity:  et = ET_ClearBasicBeauty; break;
        case EP_ClearBeautyOpacity:    et = ET_ClearBasicBeauty; break;
        case EP_BasicTypeIntensity:    et = ET_BasicBeautyType; break;
        case EP_FilterHolidayIntensity:et = ET_FilterHoliday; break;
        case EP_FilterClearIntensity:  et = ET_FilterClear; break;
        case EP_FilterWarmIntensity:   et = ET_FilterWarm; break;
        case EP_FilterFreshIntensity:  et = ET_FilterFresh; break;
        case EP_FilterTenderIntensity: et = ET_FilterTender; break;
        case EP_FilterCoolAdaierIntensity: et = ET_FilterCoolAdaier; break;
        case EP_FilterCoolDanyaIntensity: et = ET_FilterCoolDanya; break;
        case EP_FilterCoolJiazhouIntensity: et = ET_FilterCoolJiazhou; break;
        case EP_FilterCoolMojitoIntensity: et = ET_FilterCoolMojito; break;
        case EP_FilterWarmFennenIntensity: et = ET_FilterWarmFennen; break;
        case EP_FilterWarmPureIntensity: et = ET_FilterWarmPure; break;
        case EP_FilterWarmSweetIntensity: et = ET_FilterWarmSweet; break;
        case EP_FilterWarmWennuanIntensity: et = ET_FilterWarmWennuan; break;
        case EP_FilterWarmSunIntensity: et = ET_FilterWarmSun; break;
        case EP_FilterGrilWhiteIntensity: et = ET_FilterGrilWhite; break;
        case EP_FilterGrilLuolitaIntensity: et = ET_FilterGrilLuolita; break;
        case EP_FilterGrilLiteratureIntensity: et = ET_FilterGrilLiterature; break;
        case EP_FilterEnhanceAmorousIntensity: et = ET_FilterEnhanceAmorous; break;
        case EP_FilterEnhanceDeliciousIntensity: et = ET_FilterEnhanceDelicious; break;
        case EP_FilterEnhanceClearIntensity: et = ET_FilterEnhanceClear; break;
        case EP_FilterEnhanceNuolanIntensity: et = ET_FilterEnhanceNuolan; break;
        case EP_FilterEnhancePeachIntensity: et = ET_FilterEnhancePeach; break;
        case EP_FilterZhiganCreamIntensity: et = ET_FilterZhiganCream; break;
        case EP_FilterZhiganFilmIntensity: et = ET_FilterZhiganFilm; break;
        case EP_FilterZhiganMagazineIntensity: et = ET_FilterZhiganMagazine; break;
        default: et = ET_SeniorBeautyType; break;
    }
    return _effects[(int)et].effectId;
}

// private
int OrangeHelper::getFilterId(EffectParamType ep)
{
    string targetName = getParamName(ep);
    int effectId = getEffectId(ep);
    if (effectId > 0)
    {
        OF_EffectInfo info;
        OF_GetEffectInfo(_context, effectId, &info);
        
        for (int i = 0; i < info.filterCount; ++i)
        {
            int filterId = info.filterList[i];
            OFUInt32 paramCnt = 0;
            OF_GetFilterParamCount(_context, filterId, &paramCnt);
            
            for (int n = 0; n < paramCnt; ++n)
            {
                OFName paramName;
                OF_GetFilterParamName(_context, filterId, n, paramName);
                if (targetName == paramName)
                    return filterId;
            }
        }
    }
    return 0;
}

// private
std::string OrangeHelper::getParamName(EffectParamType ep)
{
    string ParamNames[] = {
        "Intensity",//basic beauty
        "Opacity",
        "Intensity",//clear beauty
        "Opacity",
        "Intensity",//filter
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",//basic beauty type
        "ThinfaceIntensity",//senior beauty type
        "SmallfaceIntensity",
        "SquashedFaceIntensity",
        "ForeheadLiftingIntensity",
        "WideForeheadIntensity",
        "BigSmallEyeIntensity",
        "EyesOffset",
        "EyesRotationIntensity",
        "ThinNoseIntensity",
        "LongNoseIntensity",
        "ThinNoseBridgeIntensity",
        "ThinmouthIntensity",
        "MovemouthIntensity",
        "ChinLiftingIntensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
        "Intensity",
    };
    return ParamNames[ep];
}
