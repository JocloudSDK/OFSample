# 1 Overview
This demo shows how to use the OrangeFilter SDK to implement cosmetic, plastic, filter, sticker, gesture, and other effects.The special effects package includes beauty, makeup, filter, plastic, sticker, gesture and background segmentation. The corresponding relationship between the special effects package and functions is as follows:

| Effect Package | Function | Adjustable or not |
| ---:| :--- | ---:|
| Beauty | Brighten, smooth Skin | Yes |
| beauty makeup | Beauty makeup the effect | No |
| Filter | One filter package corresponds to one type of filter | Yes |
| Shaping | Face slimming/shrinking, eye enlargement/distancing, forehead height/width, nose shape and lip shape adjustments | Yes |
| Sticker | An effects package corresponds to a type of a sticker | No |
| Gesture | 11 kinds of gesture recognition—including victory, thumbs-up, (one or two-handed) heart sign, Spiderman, holding, hang loose, deference, open palm, clasped palms, and OK. | No |
| Background Segmentation | Detection body contour and separate someone from the background, and then you can replace the backgrounds with desired effects | No |
| Makeup | No makeup starts on air, with makeup on the mirror | No |


# 2 Quick Start

## 2.1 Prerequisite
- Visual Studio 2015 and above.
## 2.2 directory structure
- bin：The compiled output path。
- src: Storing logical code。
## 2.3 Integrate the SDK
  1. Lib in the SDK package is copied to depends\dwinternal\orangefilterpub2013\lib\Release.
  2. Include in the SDK package is copied to depends\dwinternal\orangefilterpub2013\include.
  3. Dll  in the SDK package is copied to depends\dwinternal\orangefilterpub2013\bin.
  4. Copy the OrangeHelper class to your own project, and the interface calls are dependent on it.
  5. Put the the effect package in the directory orangefilter\Effects.
  6. Run the run_deploy.vcxPROj project to automatically copy the dependent Dll to the bin directory.
## 2.4 Create an Engine Environment

```objc
    bool OrangeHelper::createContext(const std::string& ofSerialNumber, const std::string& licensePath, const std::string& resDir, VenusType aiType = VN_All)
```
**Parameters** 

| Parameter | Description |
| :--- | :--- |
| context | Context |
| ofSerialNumber | Serial number for authentication, obtained when OrangeFilter is purchased |
|licensePath | Writable path，Used to save authorization information |
| resDir | Resource storage root path; includes effect package, venus model files, and cache authorization information; default value is activity.getFilesDir().getPath(); null input is recommended |
| aiType | AI effect type:<br>“VENUS_NONE”- basic beautification, disable AI calculation<br>"VENUS_FACE"- advanced beauty and stickers<br> VENUS_GESTURE- basic beautification and gesture recognition<br> VENUS_SEGMENT-basic beautification and background segmentation<br>VENUS_ALL -enbale AI functions |


## 2.5 Release Engine Resources

```objc
    void OrangeHelper::destroyContext();
```
> **Note**
>-  After releasing the resource, the beautification effects are unavailable. You can reusing it by recreating an engine environment.

## 2.6 Rendering Effects Frame by Frame
```objc
      bool OrangeHelper::updateFrameParams(const GLTexture& textureIn, const GLTexture& textureOut, const ImageInfo& imageInfo);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| textureIn | Input texture encapsulated instance, refer to [GLTexture](#GLTexture) |
| textureOut | Output texture encapsulated instance, refer to [GLTexture](#GLTexture) |
| imageInfo | Image recognition encapsulated instance, refer to [ImageInfo](#ImageInfo) |

> **Note**
>- Each frame needs to be refreshed for getting the the current texture rendering information.
>- Returne false for failure. Make sure the effect is enabled.

## 2.7 Enable/Disable Effects
Enable/disable beauty, filters, basic and advanced shaping.

```objc
    bool OrangeHelper::enableEffect(EffectType effectType, bool bEnable);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectType | Effect type, see details in [EffectType](#EffectType) |
| bEnable | Whether to enable. When called for the first time and passing false, the effect is not loaded. |

> **Note:**
>- When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded, and you cannot get the actual parameters by calling `getEffectParam` and `getEffectParamDetail`.

## 2.8 Adjust Effect Intensity
Adjust the effect intensity of beautification, filter, shaping, etc.
```objc
    bool OrangeHelper::setEffectParam(EffectParamType effectParamType, int curVal);
```
**Parameter **

| Parameter | Description |
| :--- | :--- |
| effectParamType | Effect parameters ,see details in [EffectParamType](#EffectParamType) |
| curVal | Effect parameter value to be set; the value range of each parameter is different, see details in  [EffectParamType](#EffectParamType). |

## 2.9 Get the Current Effect Intensity 
```objc
    int OrangeHelper::getEffectParam(EffectParamType effectParamType);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectParamType | Effect parameters, see details in [EffectParamType](#EffectParamType) |

> **Note:**
>- If the effect is released, disabled, or not initialized, you should load it before getting the effect parameters.

## 2.10 Get the Value Range and Default Value of an Effect Parameter
```objc
    bool OrangeHelper::getEffectParamDetail(EffectParamType effectParamType, EffectParam& effectParam);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectParamType | Effect parameters, see details in [EffectParamType](#EffectParamType) |
| effectParam | Effect parameter vaules, including the maximum value, minimum value, current value and default value. See details in [EffectParam](#EffectParam) |

> **Note**
>- If the effect is released, disabled, or not initialized, you should load it before getting the effect parameters.


## 2.11 Release Resources

```objc
    bool OrangeHelper::releaseEffect(EffectType effectType);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectType | Effect type, see details in [EffectType](#EffectType) |

> **Note**
>- This API is not recommended to use.

## 2.12 Enbale/Disable Sticker

```objc
    bool OrangeHelper::enableSticker(const std::string& path, bool bEnable);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the sticker |
| bEnable | Whether to enable the sticker |

> **Note**
>- If path is incorrect, loading the effect will fail.
>- When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded.

## 2.13 Resource Sticker Release

```objc
    bool OrangeHelper::releaseSticker(const std::string& path);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the sticker  |

> **Note**
>- If path is incorrect, releasing resources will fail.

## 2.14 Search for the Sticker Execution Results
```objc
    bool OrangeHelper::checkStickerResult(const std::vector<std::string>& stickerPaths, std::vector<int>& stickerResults);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| stickerPaths | Full path of the sticker effect to be searched for |
| stickerResults | Return value |

> **Note**
>- This API should be called after calling updateFrameParams.

## 2.15 Enable/Disable Gesture

```objc
    bool OrangeHelper::enableGesture(const std::string& path, bool bEnable);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the gesture effect |
| bEnable | Whether to enable the gesture. |

> **Note:**
>- If path is incorrect, loading gestures will fail.
>- When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded.

## 2.16 Release Gesture Resources

```objc
    bool OrangeHelper::releaseGesture(const std::string& path);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the gesture effect |

> **Note:**
>- If path is incorrect, releasing resources will fail.

## 2.17 Check Whether the engine environment is Available

```objc
    bool OrangeHelper::isContextValid();
```
**Return Value**
"true"-available；"false"- not available

## 2.18 Set Log Level

```objc
   OrangeHelper.setLogLevel(logLevel);
```
You can set it to  0 to close all logs. 

## 2.19 Redirect the Log Output

```objc
    bool OrangeHelper::setLogLevel(int level)
```
| Parameter | Description |
| :--- | :--- |
| level | loglevel: see details in [LogLevel](#LogLevel) |

## 2.20 Structure and Enumeration

### EffectParamType

```objc
    enum EffectParamType {
		EP_BasicBeautyIntensity,  //Basic beauty[0,100]
		EP_BasicBeautyOpacity,    //Basic microdermabrasion strength[0,100] (Hazy dermabrasion)
		EP_BasicBeautyIntensity5,   //Clear beauty[0,100]
		EP_BasicBeautyOpacity5,		//Clear  microdermabrasion strength[0,100]
		EP_FilterHolidayIntensity,  //Holiday filter strength[0,100]
		EP_FilterClearIntensity,    //Clear filter strength[0,100]
		EP_FilterWarmIntensity,     //Warm Sun Filter strength[0,100]
		EP_FilterFreshIntensity,    //Fresh filter strength[0,100
		EP_FilterTenderIntensity,   //Tender filter strength[0,100] 
		//Based on plastic
		EP_BasicTypeIntensity,     	//Basic plastic strength[0,100]
		//Advanced plastic		
		EP_SeniorTypeThinFaceIntensity,      //Senior face-lift[0,100]    
		EP_SeniorTypeSmallFaceIntensity,     //Senior face[0,100]
		EP_SeniorTypeSquashedFaceIntensity,   //Advanced skinny cheekbones[0,100]
		EP_SeniorTypeForeheadLiftingIntensity, //Senior high[-50,50]
		EP_SeniorTypeWideForeheadIntensity,    //Advanced forehead[-50,50]
		EP_SeniorTypeBigSmallEyeIntensity,     //Senior big eyes[-50,50]
		EP_SeniorTypeEyesOffsetIntensity,       //Advanced eye distance[-50,50]     
		EP_SeniorTypeEyesRotationIntensity,    //Senior eye corner[-50,50]
		EP_SeniorTypeThinNoseIntensity,        //Advanced thin nose[-50,50]
		EP_SeniorTypeLongNoseIntensity,         //Senior Proboscis[-50,50]
		EP_SeniorTypeThinNoseBridgeIntensity,    //Advanced narrow nose[-50,50]
		EP_SeniorTypeThinmouthIntensity,         //Senior small mouth[-50,50]
		EP_SeniorTypeMovemouthIntensity,         //Advanced mouth position[-50,50]
		EP_SeniorTypeChinLiftingIntensity,      //Senior Chin[-50,50]

		//21 filter styles
		EP_FilterWhiteningIntensity,      //skin filter strength[0,100]
		EP_FilterAdaierIntensity,			//Cool colour-Adele filter strength[0,100]
		EP_FilterDanyaIntensity,			//Cool colour-quietly elegant filter strength[0,100]
		EP_FilterJiaZhouIntensity,			//Cool colour-California filter strength[0,100]
		EP_FilterMojitoIntensity,			//Leose-mojito filter strength[0,100]
		EP_FilterFennenIntensity,			//Warm Colour-tender filter strength[0,100]
		EP_FilterPureIntensity,				//Warm Colour-youth filter strength[0,100]
		EP_FilterSweetIntensity,			//Warm Colour-sweet filter strength[0,100]
		EP_FilterWenNuanIntensity,			//Warm Colour-warmth filter strength[0,100]
		EP_FilterSunIntensity,				//Warm Colour-sunshine filter strength[0,100]
		EP_FilterGrilWhiteIntensity,		//Maid-fair filter strength[0,100]
		EP_FilterGrilLuolitaIntensity,		//Maid-lolita filter strength[0,100]
		EP_FilterGrilLiteratureIntensity,	//Maid-art  filter strength[0,100]
		EP_FilterAmorousIntensity,			//Enhance-Amorous feelings filter strength[0,100]
		EP_FilterDeliciousIntensity,		//Enhance-tasty filter strength[0,100]
		EP_FilterEnhanceClearIntensity,		//Enhance-clear filter strength[0,100]
		EP_FilterEnhanceNoulanIntensity,	//Enhance-nolan filter strength[0,100]
		EP_FilterPeachIntensity,			//Enhance-Peach filter strength[0,100]
		EP_FilterCreamIntensity,			//Texture-ice cream filter strength[0,100]
		EP_FilterFilmIntensity,				//Texture-film filter strength[0,100]
		EP_FilterMagazineIntensity			//Texture-magazine filter strength[0,100]
    };
```

### EffectParam

```objc
    struct EffectParam {
		int curVal; //The current value
		int maxVal; //Max value
		int minVal; //Min value
		int defVal; //Default value
    };
```

### EffectType

```objc
    enum EffectType {
		ET_BasicBeauty,			//Basic beauty (Hazy dermabrasion)
		ET_BasicBeauty5,		//Clear beauty
		ET_BasicBeautyType,		//Basic plastic surgery
		ET_SeniorBeautyType,	//Advanced plastic surgery
		ET_FilterHoliday,		//Holiday filter
		ET_FilterClear,			//Clear filter
		ET_FilterWarm,			//Warm sun filter
		ET_FilterFresh,			//Fresh filter
		ET_FilterTender,		//Tender filter
		ET_FilterWhitening,		//skin filter   //21 filter styles 
		ET_FilterAdaier,		//Cool colour-Adele filter
		ET_FilterDanya,			//Cool colour-quietly elegant filter
		ET_FilterJiaZhou,		//Cool colour-California filter
		ET_FilterMojito,		//Leose-mojito filter
		ET_FilterFennen,		//Warm Colour-tender filter
		ET_FilterPure,			//Warm Colour-youth filter
		ET_FilterSweet,			//Warm Colour-sweet filter
		ET_FilterWenNuan,		//Warm Colour-warmth filter
		ET_FilterSun,			//Warm Colour-sunshine filter
		ET_FilterGrilWhite,		//Maid-fair filter
		ET_FilterGrilLuolita,	//Maid-lolita filter
		ET_FilterGrilLiterature, //Maid-art filter
		ET_FilterAmorous,		//Enhance-Amorous feelings filter
		ET_FilterDelicious,		//Enhance-tasty filter
		ET_FilterEnhanceClear,  //Enhance-clear filter
		ET_FilterEnhanceNoulan, //Enhance-nolan filter
		ET_FilterPeach,			//Enhance-Peach filter
		ET_FilterCream,			//Texture-ice cream filter
		ET_FilterFilm,			//Texture-film filter
		ET_FilterMagazine		//Texture-magazine filter 
    };
```

### ImageInfo

```objc
    struct ImageInfo {
        int deviceType; //Image category, the default is camera image 0, user-defined incoming tripartite sdk image is 1.
        int facePointDir; //When deviceType == 1, this variable can freely set the direction of rendering special effects.
        Otherwise, use the underlying default direction to render.
        // The default direction is set according to the camera device information.
        unsigned char* data; //The identification data passed in by the user, usually pixel data
        int dir; //The direction of the image, usually the gyroscope direction
        int orientation; //The camera direction is usually obtained from the device camera information. When the input 
        is deviceType=1, the value will not take effect.
        int width; //Image width
        int height; //Image height
        int format; //Image format
        boolean frontCamera; //Is it a front camera
        float timestamp; //Calculate the timestamp of face recognition, in seconds, if not set, the bottom layer uses the default value.
    };
```

### GLTexture

```objc
    struct GLTexture
    {   
        int textureId; //OpenGL texture id
        int width;  //OpenGL texture width.
        int height; //OpenGL texture height.
        int format; //OpenGL texture format, e.g. GL_RGBA.
        int target; //OpenGL texture target, e.g. GL_TEXTURE_2D.
    };
```

### Effect Package Name

```objc
    String[] mEffectDefaults = {
		"beauty_basic.zip",    	//Basic beauty (Hazy dermabrasion)
		"beauty5_basic.zip",		//Basic beauty (Clear dermabrasion)
		"basicthinface.zip", 		//Basic plastic surgery
		"faceliftingopt.zip", 		//Advanced plastic surgery
		"filter_holiday.zip",   	//Holiday filter
		"filter_clear.zip",     	//Clear filter
		"filter_warm.zip",      	//Warm sun filter
		"filter_fresh.zip",     	//Fresh filter
		"filter_tender.zip",    	//Tender filter
		"huajiaotoutiao-meibai.zip", //skin filter
		"lengse-adaier.zip",         //Cool colour-Adele filter
		"lengse-danya.zip",			 //Cool colour-quietly elegant
		"lengse-jiazhou.zip",		//Cool colour-California filter 
		"lengse-muojito.zip",			//Leose-mojito filter
		"nuanse-fennen.zip",			//Warm Colour-tender filter
		"nuanse-qingchun.zip",			//Warm Colour-youth filter
		"nuanse-tianmei.zip",			//Warm Colour-sweet filter
		"nuanse-wennuan.zip",			//Warm Colour-warmth filter 
		"nuanse-yangguang.zip",			//Warm Colour-sunshine filter
		"shaonv-baixi.zip",				//Maid-fair filter
		"shaonv-luolita.zip",			//Maid-lolita filter
		"shaonv-wenyi.zip",				//Maid-art  filter
		"zengqiang-fengqing.zip",		//Enhance-Amorous feelings filter
		"zengqiang-kekou.zip",			//Enhance-tasty filter
		"zengqiang-mingliang.zip",		//Enhance-clear filter
		"zengqiang-nuolan.zip",			//Enhance-nolan filter
		"zengqiang-shuimitao.zip",		//Enhance-Peach filter
		"zhigan-bingqiling.zip",		//Texture-ice cream filter
		"zhigan-jiaopian.zip",			//Texture-film filter
		"zhigan-zazhi.zip"				//Texture-magazine filter
    };
```
### Loglevel

```objc
    String[] LogLevel = {  
		"LG_Info", //Ordinary message
		"LG_Warn", //Warning message
		"LG_Error", //Error message
		"LG_Debug", //Debug SDK-level messages
		"LG_Verbose", //All of the messages
    };
```

# 3 Contact us
- If you would like to see more official examples, see the scenario Demo [Click to download](https://docs.jocloud.com/download)
- If you would like to know more about beauty products, please refer to [Product Introduction](https://docs.jocloud.com/cloud/cn/product_category/beauty_sdk/product_overview/product/product.html)
- See the full API documentation [API Reference](https://docs.jocloud.com/cloud/cn/product_category/beauty_sdk/api/Windows/v1.4.2/category.html)