#pragma once

#include "orangefilter.h"
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
        ET_BasicBeauty,			//基础美颜
		ET_BasicBeauty5,		//清晰美颜
        ET_BasicBeautyType,		//基础整形
        ET_SeniorBeautyType,	//高级整形
        ET_FilterHoliday,		//假日滤镜
        ET_FilterClear,			//清晰滤镜
        ET_FilterWarm,			//暖阳滤镜
        ET_FilterFresh,			//清新滤镜
        ET_FilterTender,		//粉嫩滤镜
						//20款滤镜风格
		ET_FilterAdaier,		//冷色-阿黛尔滤镜
		ET_FilterDanya,			//冷色-淡雅滤镜
		ET_FilterJiaZhou,		//冷色-加州滤镜
		ET_FilterMojito,		//乐色-莫吉托滤镜
		ET_FilterFennen,		//暖色-粉嫩滤镜
		ET_FilterPure,			//暖色-青春滤镜
		ET_FilterSweet,			//暖色-甜美滤镜
		ET_FilterWenNuan,		//暖色-温暖滤镜
		ET_FilterSun,			//暖色-阳光滤镜
		ET_FilterGrilWhite,		//少女-白皙滤镜
		ET_FilterGrilLuolita,	//少女-洛丽塔滤镜
		ET_FilterGrilLiterature, //少女-文艺滤镜
		ET_FilterAmorous,		//增强-风情滤镜
		ET_FilterDelicious,		//增强-可口滤镜
		ET_FilterEnhanceClear,  //增强-明朗滤镜
		ET_FilterEnhanceNoulan, //增强-诺兰滤镜
		ET_FilterPeach,			//增强-水蜜桃滤镜
		ET_FilterCream,			//质感-冰淇淋滤镜
		ET_FilterFilm,			//质感-胶片滤镜
		ET_FilterMagazine		//质感-杂志滤镜
    };
    
	enum EffectParamType {
		EP_BasicBeautyIntensity,  //基础美颜[0,100]
		EP_BasicBeautyOpacity,    //基础磨皮强度[0,100]
		EP_BasicBeautyIntensity5,   //清晰美颜[0,100]
		EP_BasicBeautyOpacity5,		//清晰磨皮[0,100]
		EP_FilterHolidayIntensity,  //假日滤镜强度[0,100]
		EP_FilterClearIntensity,    //清晰滤镜强度[0,100]
		EP_FilterWarmIntensity,     //暖阳滤镜强度[0,100]
		EP_FilterFreshIntensity,    //清新滤镜强度[0,100]
		EP_FilterTenderIntensity,   //粉嫩滤镜强度[0,100]  
		//基础整形
		EP_BasicTypeIntensity,     	//基础整形强度[0,100] 
		//高级整形		
		EP_SeniorTypeThinFaceIntensity,      //高级瘦脸[0,100]     
		EP_SeniorTypeSmallFaceIntensity,     //高级小脸[0,100] 
		EP_SeniorTypeSquashedFaceIntensity,   //高级瘦颧骨[0,100]
		EP_SeniorTypeForeheadLiftingIntensity, //高级额高[-50,50]
		EP_SeniorTypeWideForeheadIntensity,    //高级额宽[-50,50]
		EP_SeniorTypeBigSmallEyeIntensity,     //高级大眼[-50,50]
		EP_SeniorTypeEyesOffsetIntensity,       //高级眼距[-50,50]        
		EP_SeniorTypeEyesRotationIntensity,    //高级眼角[-50,50]
		EP_SeniorTypeThinNoseIntensity,        //高级瘦鼻[-50,50]
		EP_SeniorTypeLongNoseIntensity,         //高级长鼻[-50,50]
		EP_SeniorTypeThinNoseBridgeIntensity,    //高级窄鼻梁[-50,50]
		EP_SeniorTypeThinmouthIntensity,         //高级小嘴[-50,50]
		EP_SeniorTypeMovemouthIntensity,         //高级嘴位[-50,50]
		EP_SeniorTypeChinLiftingIntensity,      //高级下巴[-50,50]

		//20款滤镜风格
		EP_FilterAdaierIntensity,			//冷色-阿黛尔滤镜强度[0,100]
		EP_FilterDanyaIntensity,			//冷色-淡雅滤镜强度[0,100]
		EP_FilterJiaZhouIntensity,			//冷色-加州滤镜强度[0,100]
		EP_FilterMojitoIntensity,			//乐色-莫吉托滤镜强度[0,100]
		EP_FilterFennenIntensity,			//暖色-粉嫩滤镜强度[0,100]
		EP_FilterPureIntensity,				//暖色-青春滤镜强度[0,100]
		EP_FilterSweetIntensity,			//暖色-甜美滤镜强度[0,100]
		EP_FilterWenNuanIntensity,			//暖色-温暖滤镜强度[0,100]
		EP_FilterSunIntensity,				//暖色-阳光滤镜强度[0,100]
		EP_FilterGrilWhiteIntensity,		//少女-白皙滤镜强度[0,100]
		EP_FilterGrilLuolitaIntensity,		//少女-洛丽塔滤镜强度[0,100]
		EP_FilterGrilLiteratureIntensity,	//少女-文艺滤镜强度[0,100]
		EP_FilterAmorousIntensity,			//增强-风情滤镜强度[0,100]
		EP_FilterDeliciousIntensity,		//增强-可口滤镜强度[0,100]
		EP_FilterEnhanceClearIntensity,		//增强-明朗滤镜强度[0,100]
		EP_FilterEnhanceNoulanIntensity,	//增强-诺兰滤镜强度[0,100]
		EP_FilterPeachIntensity,			//增强-水蜜桃滤镜强度[0,100]
		EP_FilterCreamIntensity,			//质感-冰淇淋滤镜强度[0,100]
		EP_FilterFilmIntensity,				//质感-胶片滤镜强度[0,100]
		EP_FilterMagazineIntensity			//质感-杂志滤镜强度[0,100]
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
		int deviceType;      //图像类别，默认为相机图像0，用户自定义传入图像为1.使用非相机图像输入可能需要设置
		int facePointDir;    //当 deviceType == 1时，该变量可以自由设置渲染特效的方向。否则使用底层默认的方向去渲染。默认方向是根据摄像头设备信息设置的。
		unsigned char* data;  //用户传入的识别数据，通常是像素数据
		int dir;   			  //图像的方向，通常用陀螺仪方向。
		int orientation;    //相机摄像头方向，通常从设备相机信息里获取，当输入为deviceType=1时，该值不生效。
		int width;   		//图像宽
		int height;    		//图像高
		int format;     	//图像格式
		bool frontCamera;   //是否是前置摄像头
		float timestamp;    //计算人脸识别的时间戳，单位秒，如果不设置，底层使用默认值。
	};

public:
    bool createContext(
           const std::string& serialNumber,
           const std::string& licensePath,
           const std::string& resDir,
           VenusType aiType = VN_All);

    void destroyContext();
    
    bool isContextValid();
    
    bool enableEffect(EffectType effectType, bool enabled);
    
    bool releaseEffect(EffectType effectType);
    
    bool enableSticker(const std::string& path, bool enabled);
    
    bool releaseSticker(const std::string& path);

    bool enableGesture(const std::string& path, bool enabled);
    
    bool releaseGesture(const std::string& path);
    
    int getEffectParam(EffectParamType paramType);
    
    bool getEffectParamDetail(EffectParamType paramType, EffectParam& paramVal);
    
    bool setEffectParam(EffectParamType paramType, int value);
    
    bool updateFrameParams(const GLTexture& textureIn, const GLTexture& textureOut, const ImageInfo& image);

    bool checkStickerResult(const std::vector<std::string>& paths, std::vector<int>& results);

	bool setLogLevel(int level);

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
	void copyLicenseFromBundle(const std::string &licenseName, const std::string &licensePath);
	bool isNeedCheckSerialNumberOnline(const std::string &licensePath);
    
    OFHandle _context;
    std::string _resDir;
    std::vector<EffectInfo> _effects;
    EffectInfo _sticker;
    OF_Texture _inTex;
    OF_Texture _outTex;
    OF_FrameData _frameData;
};
