# 1 概述

该demo展示了如何在Windows使用OrangeFilter SDK实现美颜、整形、滤镜、贴纸、手势等特效。特效包包含美颜、美妆、滤镜、整形、贴纸、手势、背景分割。
特效包与功能的对应关系如下：

| 特效包 | 功能 | 强度是否可调节 |
| ---:| :--- | ---:|
| 美颜 | 美白、美肤（磨皮） | 是 |
| 美妆 | 一个特效包对应一种美妆效果 | 否 |
| 滤镜 | 一个滤镜包对应一种类型的滤镜 | 是 |
| 整形 | 窄脸、小脸、瘦颚骨、额高、额宽、大眼、眼距、眼角、瘦鼻、长鼻、窄鼻梁、小嘴、嘴位、下巴 | 是 | 
| 贴纸 | 一个特效包对应一种贴纸 | 否 |
| 手势 | 11种手势识别，包括比v，点赞，比心（包括单手、双手），蜘蛛侠，托举，666，抱拳，手掌五指张开，合十，OK；demo只是展示了其中的一部分。 | 否 |
| 背景分割 | 人体轮廓检测，人体轮廓和背景分割后可进一步使用其他特效 | 否 |


# 2 快速使用

## 2.1 开发环境要求
- visual studio 2015 及以上。
## 2.2 目录结构解释：
- bin：编译后输出路径。
- src：存放逻辑代码。
## 2.3 集成SDK
1. 将对应版本SDK包中的lib拷贝到：depends\dwinternal\orangefilterpub2013\lib。
2. 将对应版本SDK包中的include文件拷贝到：depends\dwinternal\orangefilterpub2013\include。
3. 将对应版本SDK包中DLL拷贝到运行目录：depends\dwinternal\orangefilterpub2013\bin。
4. 将OrangeHelper类拷贝到自己的项目中，后续接口调用都是依赖这个类。
5. 将特效包拷贝到：orangefilter\Effects。
6. 运行run_deploy.vcxproj工程自动拷贝项目依赖的Dll及资源文件到 bin目录。
## 2.4 创建OrangeFilter SDK引擎环境

```objc
    bool OrangeHelper::createContext(const std::string& serialNumber, const std::string& licensePath, const std::string& resDir, VenusType aiType = VN_All)
```
SDK的初始化前，需要具备渲染环境。
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| ofSerialNumber | 授权序列号,购买OrangeFilter时获取 |
|licensePath | 可写入的路径，用于保存授权信息 |
| aiType | 有五种，分别有：VENUS_NONE（不启用AI计算）、VENUS_FACE（启用高级美颜和贴纸）、VENUS_GESTURE（启用手势识别，手势表情）、VENUS_SEGMENT（启用背景分割抠图）、VENUS_ALL（启用所有AI功能）；详情可参考OrangeHelper类注释说明。 |
| resDir | 资源存放根路径，包括特效包，AI模型文件等。effects存放路径如Demo中effects在运行目录下，传入的是运行路径。 |

## 2.5 释放OrangeFilter SDK引擎环境

```objc
    void OrangeHelper::destroyContext();
```
> **注意**
>- 释放资源后，美颜将不可用，如需使用，需重新创建引擎环境。

## 2.6 每帧图像进行特效渲染
```objc
    bool OrangeHelper::updateFrameParams(const GLTexture& textureIn, const GLTexture& textureOut, const ImageInfo& imageInfo);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| textureIn | 输入纹理信息的封装实例，参考[GLTexture](#GLTexture) |
| textureOut | 输出纹理信息的封装实例，参考[GLTexture](#GLTexture) |
| imageInfo | 识别图像信息的封装实例，参考[ImageInfo](#ImageInfo) |

> **注意**
>
>- 每帧都需要刷新的，因为需要时刻知晓当前渲染纹理的信息。
>-  返回false，则数据帧渲染失败，原因可能为没有特效被开启使用。

## 2.7 美颜、滤镜、一键整形、高级整形的开启与关闭

```objc
    bool OrangeHelper::enableEffect(EffectType effectType, bool bEnable);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectType | 枚举类型[EffectType](#EffectType)，详见OrangeHelper类的EffectType。 |
| bEnable | 是否开启。如果是首次调用，传递false，不会加载特效资源包。 |

> **注意**
>- bEnable参数传递false，如果是首次调用，不会加载特效资源包，在这之后调用getEffectParam和getEffectParamDetail将获取不到真正的值，因为当前资源特效包尚未加载，无法获取里面的参数值。

## 2.8 美颜、滤镜、一键整形、高级整形的强度调节

```objc
    bool OrangeHelper::setEffectParam(EffectParamType effectParamType, int curVal);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)枚举，调节对应功能，传入对应枚举值。详见OrangeHelper类的EffectParamType说明。 |
| curVal | 设置的值，不同功能对应的取值范围都是不同的，可参考[EffectParamType](#EffectParamType)。 |

## 2.9 获取特效当前强度值
```objc
    int OrangeHelper::getEffectParam(EffectParamType effectParamType);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)枚举，详见OrangeHelper类的EffectParamType说明。 |

> **注意**
>- 如果对应特效没有被初始化，没有开启过，或者已经被释放了，是获取不到值的。需重新加载开启。

## 2.10 获取某特效参数范围和默认值
```objc
    bool OrangeHelper::getEffectParamDetail(EffectParamType effectParamType, EffectParam& effectParam);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)枚举，详见OrangeHelper类的EffectParamType说明。 |
| effectParam | [EffectParam](#EffectParam),返回值，里面包含最大值、最小值、当前值和默认值。 |

> **注意**
>- 如果对应特效没有被初始化，没有开启过，或者已经被释放了，是获取不到值的。需重新加载开启。


## 2.11 美颜、滤镜、一键整形、高级整形的资源释放

```objc
    bool OrangeHelper::releaseEffect(EffectType effectType);;
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectType | 枚举类型[EffectType](#EffectType)，详见OrangeHelper类的EffectType。 |

> **注意**
>- 释放资源，SDK不建议调用，因为美颜、滤镜、一键整形、高级整形的属于常用功能，消耗资源不高。

## 2.12 贴纸的开启与关闭

```objc
    bool OrangeHelper::enableSticker(const std::string& path, bool bEnable);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 贴纸特效包的绝对路径，一定要正确，否则加载不了特效包。 |
| bEnable | 是否开启该贴纸。 |

> **注意**
>- path一定要正确，否则加载特效不成功。
>- bEnable参数传递false，如果是首次调用，不会加载特效资源包。

## 2.13 贴纸的资源释放

```objc
    bool OrangeHelper::releaseSticker(const std::string& path);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 贴纸特效包的绝对路径，一定要正确，否则会释放错误或者释放失败。 |

> **注意**
>- path路径是绝对路径，需要传递正确，否则会释放错误或者释放失败。

## 2.14 查询贴纸的执行结果
```objc
    bool OrangeHelper::checkStickerResult(const std::vector<std::string>& stickerPaths, std::vector<int>& stickerResults);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| stickerPaths | 需要查询的贴纸特效包路径数组 |
| stickerResults | 查询的对应特效包的结果返回值 |

> **注意**
>- 查询贴纸的每一帧返回结果，可以在updateFrameParams调用结束后调用。一般该接口使用场景是：某个贴纸动画特效使用完毕后需要释放，可作为一个监听使用，详情见官网错误码。

## 2.15 手势的开启与关闭

```objc
    bool OrangeHelper::enableGesture(const std::string& path, bool bEnable);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 手势特效包的绝对路径，一定要正确，否则加载不了特效包。 |
| bEnable | 是否开启该手势。 |

> **注意**
>- path一定要正确，否则加载手势特效不成功。
>- bEnable参数传递false，如果是首次调用，不会加载特效资源包。

## 2.16 手势的资源释放

```objc
    bool OrangeHelper::releaseGesture(const std::string& path);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 手势特效包的绝对路径，一定要正确，否则会释放错误或者释放失败。 |

> **注意**
>- path路径是绝对路径，需要传递正确，否则会释放错误或者释放失败。

## 2.17 查询引擎环境是否可用

```objc
    bool OrangeHelper::isContextValid();
```
返回值：
true：可用；false：不可用。

## 2.18 设置日志级别

```objc
    OrangeHelper.setLogLevel(logLevel);
```
可以传递0关闭所有日志，详情参考OrangeHelper类对setLogLevel的说明。

## 2.19 重定向日志输出

```objc
    bool OrangeHelper::setLogLevel(int level)
```
| 参数 | 说明 |
| :--- | :--- |
| level | 日志级别: 枚举类型[LogLevel](#LogLevel) |


## 2.20 结构定义与枚举

### EffectParamType

```objc
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

		//21款滤镜风格
		EP_FilterWhiteningIntensity,      //美白滤镜强度[0,100]
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
```

### EffectParam

```objc
    struct EffectParam {
        int curVal; //当前值
        int maxVal; //最大值
        int minVal; //最小值
        int defVal; //默认值
    };
```

### EffectType

```objc
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
					//21款滤镜风格
		ET_FilterWhitening,		//美白滤镜    // 从 9-29 开始
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
```

### ImageInfo

```objc
	struct ImageInfo
    {
        int deviceType;      //图像类别，默认为相机图像0，用户自定义传入图像为1.使用非相机图像输入可能需要设置
        int facePointDir;    //当 deviceType ==1时，该变量可以自由设置渲染特效的方向。否则使用底层默认的方向去渲染。
                             //默认方向是根据摄像头设备信息设置的。
        unsigned char* data;  //用户传入的识别数据，通常是像素数据
        int dir;   			  //图像的方向，通常用陀螺仪方向。
        int orientation;    //相机摄像头方向，通常从设备相机信息里获取，当输入为deviceType=1时，该值不生效。
        int width;   		//图像宽
        int height;    		//图像高
        int format;     	//图像格式
        bool frontCamera;   //是否是前置摄像头
        float timestamp;    //计算人脸识别的时间戳，单位秒，如果不设置，底层使用默认值。
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

### 特效包命名
```objc
std::string defaultEffects[] =  //默认枚举支持的特效
    {
		"beauty_basic.zip",		//基础美颜
		"beauty5_basic.zip",	//清晰美颜
		"basicthinface.zip", 	//基础整形（一键整形）
		"faceliftingopt.zip", 	//高级整形
		"filter_holiday.zip",   //假日滤镜 
		"filter_clear.zip",     //清晰滤镜
		"filter_warm.zip",      //暖阳滤镜
		"filter_fresh.zip",     //清新滤镜
		"filter_tender.zip",    //粉嫩滤镜
		"huajiaotoutiao-meibai.zip", //美白滤镜       
		"lengse-adaier.zip",         //冷色-阿黛尔滤镜
		"lengse-danya.zip",			 //冷色-淡雅滤镜
		"lengse-jiazhou.zip",		//冷色-加州滤镜
		"lengse-muojito.zip",			//乐色-莫吉托滤镜
		"nuanse-fennen.zip",			//暖色-粉嫩滤镜
		"nuanse-qingchun.zip",			//暖色-青春滤镜
		"nuanse-tianmei.zip",			//暖色-甜美滤镜
		"nuanse-wennuan.zip",			//暖色-温暖滤镜
		"nuanse-yangguang.zip",			//暖色-阳光滤镜
		"shaonv-baixi.zip",				//少女-白皙滤镜
		"shaonv-luolita.zip",			//少女-洛丽塔滤镜
		"shaonv-wenyi.zip",				//少女-文艺滤镜
		"zengqiang-fengqing.zip",		//增强-风情滤镜
		"zengqiang-kekou.zip",			//增强-可口滤镜
		"zengqiang-mingliang.zip",		//增强-明朗滤镜
		"zengqiang-nuolan.zip",			//增强-诺兰滤镜
		"zengqiang-shuimitao.zip",		//增强-水蜜桃滤镜
		"zhigan-bingqiling.zip",		//质感-冰淇淋滤镜
		"zhigan-jiaopian.zip",			//质感-胶片滤镜
		"zhigan-zazhi.zip"				//质感-杂志滤镜
    };
```
### 日记级别

```objc
    String[] LogLevel = { //默认枚举支持的特效
            "LG_Info", //普通消息
            "LG_Warn", //警告消息
            "LG_Error", //错误消息
            "LG_Debug", //调试SDK级别消息
            "LG_Verbose", //所有消息
    };
```
# 3 联系我们
- 如果你想了解更多官方示例，可以参考场景Demo [点击下载](https://docs.jocloud.com/download)
- 如果你想了解更多美颜相关产品介绍，请参考  [产品介绍](https://docs.jocloud.com/cloud/cn/product_category/beauty_sdk/product_overview/product/product.html)
- 完整的 API 文档见 [API参考](https://docs.jocloud.com/cloud/cn/product_category/beauty_sdk/api/Windows/v1.4.2/category.html)