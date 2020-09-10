#pragma once

#include "of_effect/orangefilter.h"
#include <string>
#include <vector>
#include <map>

class OrangeHelper
{
public:
    enum VenusType
    {
        VN_None = 0x00, 
        VN_Face = 0x01,   
        VN_Gesture = 0x02,  
        VN_BodySegment = 0x04, 
        VN_All = 0xFF     
    };
    
    enum EffectType
    {
        ET_BasicBeauty,         //Basic beauty (Hazy dermabrasion)
        ET_ClearBasicBeauty,    //Basic beauty (Clear dermabrasion)
        ET_BasicBeautyType,     //Basic plastic surgery
        ET_SeniorBeautyType,    //Advanced plastic surgery
        ET_FilterHoliday,       //Holiday filter
        ET_FilterClear,         //Clear filter
        ET_FilterWarm,          //Warm sun filter
        ET_FilterFresh,         //Fresh filter
        ET_FilterTender,        //Tender filter
        ET_FilterCoolAdaier,
        ET_FilterCoolDanya,
        ET_FilterCoolJiazhou,
        ET_FilterCoolMojito,
        ET_FilterWarmFennen,
        ET_FilterWarmPure,
        ET_FilterWarmSweet,
        ET_FilterWarmWennuan,
        ET_FilterWarmSun,
        ET_FilterGrilWhite,
        ET_FilterGrilLuolita,
        ET_FilterGrilLiterature,
        ET_FilterEnhanceAmorous,
        ET_FilterEnhanceDelicious,
        ET_FilterEnhanceClear,
        ET_FilterEnhanceNuolan,
        ET_FilterEnhancePeach,
        ET_FilterZhiganCream,
        ET_FilterZhiganFilm,
        ET_FilterZhiganMagazine,
    };
    
    enum EffectParamType {
        EP_BasicBeautyIntensity,    //Basic beauty[0,100]
        EP_BasicBeautyOpacity,      //Basic microdermabrasion strength[0,100] (Hazy dermabrasion)
        EP_ClearBeautyIntensity,    //Basic beauty[0,100]
        EP_ClearBeautyOpacity,      //Basic microdermabrasion strength[0,100] (Clear dermabrasion)
        EP_FilterHolidayIntensity,  //Holiday filter strength[0,100]
        EP_FilterClearIntensity,    //Clear filter strength[0,100]
        EP_FilterWarmIntensity,     //Warm Sun Filter strength[0,100]
        EP_FilterFreshIntensity,    //Fresh filter strength[0,100]
        EP_FilterTenderIntensity,   //Tender filter strength[0,100]
        EP_BasicTypeIntensity,      //Basic plastic strength[0,100]
        EP_SeniorTypeThinFaceIntensity,       //Senior face-lift[0,100]
        EP_SeniorTypeSmallFaceIntensity,      //Senior face[0,100]
        EP_SeniorTypeSquashedFaceIntensity,   //Advanced skinny cheekbones[0,100]
        EP_SeniorTypeForeheadLiftingIntensity,//Senior high[-50,50]
        EP_SeniorTypeWideForeheadIntensity,   //Advanced forehead[-50,50]
        EP_SeniorTypeBigSmallEyeIntensity,    //Senior big eyes[-50,50]
        EP_SeniorTypeEyesOffsetIntensity,     //Advanced eye distance[-50,50]
        EP_SeniorTypeEyesRotationIntensity,   //Senior eye corner[-50,50]
        EP_SeniorTypeThinNoseIntensity,       //Advanced thin nose[-50,50]
        EP_SeniorTypeLongNoseIntensity,       //Senior Proboscis[-50,50]
        EP_SeniorTypeThinNoseBridgeIntensity, //Advanced narrow nose[-50,50]
        EP_SeniorTypeThinmouthIntensity,      //Senior small mouth[-50,50]
        EP_SeniorTypeMovemouthIntensity,      //Advanced mouth position[-50,50]
        EP_SeniorTypeChinLiftingIntensity,    //Senior Chin[-50,50]
        EP_FilterCoolAdaierIntensity,
        EP_FilterCoolDanyaIntensity,
        EP_FilterCoolJiazhouIntensity,
        EP_FilterCoolMojitoIntensity,
        EP_FilterWarmFennenIntensity,
        EP_FilterWarmPureIntensity,
        EP_FilterWarmSweetIntensity,
        EP_FilterWarmWennuanIntensity,
        EP_FilterWarmSunIntensity,
        EP_FilterGrilWhiteIntensity,
        EP_FilterGrilLuolitaIntensity,
        EP_FilterGrilLiteratureIntensity,
        EP_FilterEnhanceAmorousIntensity,
        EP_FilterEnhanceDeliciousIntensity,
        EP_FilterEnhanceClearIntensity,
        EP_FilterEnhanceNuolanIntensity,
        EP_FilterEnhancePeachIntensity,
        EP_FilterZhiganCreamIntensity,
        EP_FilterZhiganFilmIntensity,
        EP_FilterZhiganMagazineIntensity,
    };

	enum LogLevel {
		LG_Info = 0x01,
		LG_Warn = 0x02,
		LG_Error = 0x04,
		LG_Debug = 0x08,
		LG_Verbose = 0xFF,
	};
    
    struct EffectParam
    {
        int curVal;
        int maxVal;
        int minVal;
        int defVal;
    };
    
    struct GLTexture
    {
        int textureId; //OpenGL texture id
        int width;  //OpenGL texture width.
        int height; //OpenGL texture height.
        int format; //OpenGL texture format, e.g. GL_RGBA.
        int target; //OpenGL texture target, e.g. GL_TEXTURE_2D.
    };
    
    struct ImageInfo
    {
        int deviceType;
        int facePointDir;
        unsigned char* data;
        int dir;
        int orientation;
        int width;
        int height;
        int format;
        bool frontCamera;
        float timestamp;
    };

public:
    // Initialize the SDK environment
    bool createContext(
           const std::string& serialNumber,
           const std::string& licensePath,
           const std::string& resDir,
           VenusType aiType = VN_All);

    // Destroy the SDK environment
    void destroyContext();
    
    // Is Orange Beauty SDK available?
    bool isContextValid();
    
    // Turn on or turn off a special effect (the first call to the interface will automatically load the special effect package)
    bool enableEffect(EffectType effectType, bool enabled);
    
    // Close and release a special effect memory
    bool releaseEffect(EffectType effectType);
    
    // Turn on or off a gesture special effect (the interface will automatically load the special effect package for the first time)
    bool enableSticker(const std::string& path, bool enabled);
    
    // Close and release the memory of a gesture special effect package
    bool releaseSticker(const std::string& path);

    // Turn on or off the special effects of a sticker (the first call to the interface will automatically load the special effects package)
    bool enableGesture(const std::string& path, bool enabled);
    
    // Close and release the memory of a sticker special effects package
    bool releaseGesture(const std::string& path);
    
    // Get the current value of a special effect parameter
    int getEffectParam(EffectParamType paramType);
    
    // Get a special effect parameter range and default value
    bool getEffectParamDetail(EffectParamType paramType, EffectParam& paramVal);
    
    // Set the current value of a special effect parameter
    bool setEffectParam(EffectParamType paramType, int value);
    
    // Frame rendering special effect callback function
    bool updateFrameParams(const GLTexture& textureIn, const GLTexture& textureOut, const ImageInfo& image);

    // Query the result of the sticker
    bool checkStickerResult(const std::vector<std::string>& paths, std::vector<int>& results);

    // Set log level
	bool setLogLevel(int level);

    // Redirect log output
	bool setLogCallback(void(*callback)(const char* msg));

private:
    struct EffectInfo
    {
        std::string path;
        int effectId;
        bool enabled;
        int result;
        EffectInfo() : effectId(0), enabled(false) {}
        EffectInfo(const std::string& p, int i, bool e) : path(p), effectId(i), enabled(e), result(0) {}
    };
    
    int getEffectId(EffectParamType ep);
    int getFilterId(EffectParamType ep);
    std::string getParamName(EffectParamType ep);
    
    OFHandle _context;
    std::string _resDir;
    std::vector<EffectInfo> _effects;
    EffectInfo _sticker;
    OF_Texture _inTex;
    OF_Texture _outTex;
    OF_FrameData _frameData;
};
