# 前言
该demo展示了如何在Android端使用OrangeFilter SDK；数据源为相机数据；
目录结构解释：
- assets/effects:存放所有美颜相关特效包
- java/com/yy/orangefilter/test:存放逻辑代码


# 概述

该demo展示了如何使用OrangeFilter SDK实现美颜、整形、滤镜、贴纸、手势等特效。特效包包含美颜、滤镜、整形、贴纸、手势、背景分割、美妆，特效包与功能的对应关系如下：

| 特效包 | 功能 | 强度是否可调节 |
| ---:| :--- | ---:|
| 美颜 | 美白、美肤（磨皮） | 是 |
| 滤镜 | 一个滤镜包对应一种类型的滤镜 | 是 |
| 整形 | 窄脸、小脸、瘦颚骨、额高、额宽、大眼、眼距、眼角、瘦鼻、长鼻、窄鼻梁、小嘴、嘴位、下巴 | 是 | 
| 贴纸 | 一个特效包对应一种贴纸 | 否 |
| 手势 | 11种手势识别，包括比v，点赞，比心（包括单手、双手），蜘蛛侠，托举，666，抱拳，手掌五指张开，合十，OK；对应展示的效果可以定制。 | 否 |
| 背景分割 | 人体轮廓检测，人体轮廓和背景分割后可进一步使用其他特效 | 否 |
| 美妆 | 素颜开播，带妆上镜 | 否 |


# 1 如何使用

## 1.1 开发环境要求
- Android Studio 2.0及以上。
- Android 4.0 (SDK API 14+)以上。
## 1.2 集成SDK
- 将aar包拷贝到libs。
- 在build.gradle引入该aar包，添加代码：
```objc
  implementation fileTree(dir: 'libs', include: ['*.aar'])  
```
- 将OrangeHelper类拷贝到自己的项目中，后续接口调用都是依赖这个类。
- 将特效包放到main/assets/effects
- 如果资源是动态下载的，下载位置需要注意：自定义路径+/orangefilter/effects/；需要放到该路径下，在使用OrangeHelper的时候，将自定义路径传入。
- 在AndroidManifest文件添加权限。
```objc
    <!--相机权限-->
    <uses-permission android:name="android.permission.CAMERA"/>
    <!--文件读写权限-->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    <!--网络权限-->
    <uses-permission android:name="android.permission.INTERNET"/>
    <!--GL权限-->
    <uses-feature android:glEsVersion="0x00020000" android:required="true" />
```


## 1.3 创建OrangeFilter SDK引擎环境

```objc
    OrangeHelper.createContext(context, ofSerialNumber, aiType, resDir);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| context | Android上下文 |
| ofSerialNumber | 授权序列号,购买OrangeFilter时获取 |
| aiType | 有五种，分别有：VENUS_NONE（不启用AI计算）、VENUS_FACE（启用高级美颜和贴纸）、VENUS_GESTURE（启用手势识别，手势表情）、VENUS_SEGMENT（启用背景分割抠图）、VENUS_ALL（启用所有AI功能）；详情可参考OrangeHelper类注释说明。 |
| resDir | 自定义路径根目录，如果将特效包放在main/assets/effects下，该参数传null；如果是自定义路径，比如从服务器下载资源，下载保存位置需要按照以下规则：resDir+/orangefilter/effects/ |

## 1.4 释放OrangeFilter SDK引擎环境

```objc
    OrangeHelper.destroyContext();
```
> **注意**

>  释放资源后，美颜将不可用，如需使用，需重新创建引擎环境。

## 1.5 每帧图像进行特效渲染
```objc
    OrangeHelper.updateFrameParams(textureIn, textureOut, imageInfo);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| textureIn | 输入纹理信息的封装实例，参考[GLTexture](#GLTexture) |
| textureOut | 输出纹理信息的封装实例，参考[GLTexture](#GLTexture) |
| imageInfo | 识别图像信息的封装实例，参考[ImageInfo](#ImageInfo) |

> **注意**

> - 每帧都需要刷新的，因为需要时刻知晓当前渲染纹理的信息。

> - 返回false，则数据帧渲染失败，原因可能为没有特效被开启使用。

## 1.6 美颜、滤镜、一键整形、高级整形的开启与关闭

```objc
    OrangeHelper.enableEffect(effectType, bEnable);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectType | 枚举类型[EffectType](#EffectType)，分别有8种，详见OrangeHelper类的EffectType。 |
| bEnable | 是否开启。如果是首次调用，传递false，不会加载特效资源包。 |

> **注意**

> bEnable参数传递false，如果是首次调用，不会加载特效资源包，在这之后调用getEffectParam和getEffectParamDetail将获取不到真正的值，因为当前资源特效包尚未加载，无法获取里面的参数值。

## 1.7 美颜、滤镜、一键整形、高级整形的强度调节

```objc
    OrangeHelper.setEffectParam(effectParamType, curVal);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)枚举，一共有22种，调节对应功能，传入对应枚举值。详见OrangeHelper类的EffectParamType说明。 |
| curVal | 设置的值，不同功能对应的取值范围都是不同的，可参考[EffectParamType](#EffectParamType)。 |

## 1.8 获取特效当前强度值
```objc
    OrangeHelper.getEffectParam(effectParamType);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)枚举，一共有22种，详见OrangeHelper类的EffectParamType说明。 |

> **注意**

> 如果对应特效没有被初始化，没有开启过，或者已经被释放了，是获取不到值的。需重新加载开启。

## 1.9 获取某特效参数范围和默认值
```objc
    OrangeHelper.getEffectParamDetail(effectParamType, effectParam);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)枚举，一共有22种，详见OrangeHelper类的EffectParamType说明。 |
| effectParam | [EffectParam](#EffectParam),返回值，里面包含最大值、最小值、当前值和默认值。 |

> **注意**

> 如果对应特效没有被初始化，没有开启过，或者已经被释放了，是获取不到值的。需重新加载开启。


## 1.10 美颜、滤镜、一键整形、高级整形的资源释放

```objc
    OrangeHelper.releaseEffect(EffectType et);
```
参数解释：

| 参数 | 说明 |
| :--- | :--- |
| effectType | 枚举类型[EffectType](#EffectType)，分别有8种，详见OrangeHelper类的EffectType。 |

> **注意**

> 释放资源，SDK不建议调用，因为美颜、滤镜、一键整形、高级整形的属于常用功能，消耗资源不高。

## 1.11 贴纸的开启与关闭

```objc
    OrangeHelper.enableSticker(path, bEnable);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 贴纸特效包的绝对路径，一定要正确，否则加载不了特效包。 |
| bEnable | 是否开启该贴纸。 |

> **注意**

> - path一定要正确，否则加载特效不成功。

> - bEnable参数传递false，如果是首次调用，不会加载特效资源包。

## 1.12 贴纸的资源释放

```objc
    OrangeHelper.releaseSticker(path);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 贴纸特效包的绝对路径，一定要正确，否则会释放错误或者释放失败。 |

> **注意**

> path路径是绝对路径，需要传递正确，否则会释放错误或者释放失败。

## 1.13 查询贴纸的执行结果
```objc
    OrangeHelper.checkStickerResult(String[] stickerPaths, int[] stickerResults);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| stickerPaths | 需要查询的贴纸特效包路径数组 |
| stickerResults | 查询的对应特效包的结果返回值 |

> **注意**

> 查询贴纸的每一帧返回结果，可以在updateFrameParams调用结束后调用。一般该接口使用场景是：某个贴纸动画特效使用完毕后需要释放，可作为一个监听使用，详情见官网错误码。

## 1.14 手势的开启与关闭

```objc
    OrangeHelper.enableGesture(path, bEnable);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 手势特效包的绝对路径，一定要正确，否则加载不了特效包。 |
| bEnable | 是否开启该手势。 |

> **注意**

> - path一定要正确，否则加载手势特效不成功。

> - bEnable参数传递false，如果是首次调用，不会加载特效资源包。

## 1.15 手势的资源释放

```objc
    OrangeHelper.releaseGesture(path);
```
参数说明：

| 参数 | 说明 |
| :--- | :--- |
| path | 手势特效包的绝对路径，一定要正确，否则会释放错误或者释放失败。 |

> **注意**

> path路径是绝对路径，需要传递正确，否则会释放错误或者释放失败。

## 1.16 查询引擎环境是否可用

```objc
    OrangeHelper.isContextValid();
```
返回值：
true：可用；false：不可用。

## 1.17 设置日志级别

```objc
    OrangeHelper.setLogLevel(logLevel);
```
可以传递0关闭所有日志，详情参考OrangeHelper类对setLogLevel的说明。

## 1.18 重定向日志输出

```objc
    OrangeHelper.setLogCallback(OrangeFilter.OF_LogListener logListener)
```
## 1.19 混淆设置
如果有混淆设置，在proguard-rules文件加上以下代码：

```objc
    -keep class com.orangefilter.**  {*;}
```

# 2 结构定义与枚举

## EffectParamType

```objc
    enum EffectParamType {
        EP_BasicBeautyIntensity, //基础美颜[0,100]
        EP_BasicBeautyOpacity, //基础磨皮强度[0,100]
        //滤镜
        EP_FilterHolidayIntensity, //假日滤镜强度[0,100]
        EP_FilterClearIntensity, //清晰滤镜强度[0,100]
        EP_FilterWarmIntensity, //暖阳滤镜强度[0,100]
        EP_FilterFreshIntensity, //清新滤镜强度[0,100]
        EP_FilterTenderIntensity, //粉嫩滤镜强度[0,100]
        //基础整形
        EP_BasicTypeIntensity, //基础整形强度[0,100]
        //高级整形
        EP_SeniorTypeThinFaceIntensity, //高级瘦脸[0,100]
        EP_SeniorTypeSmallFaceIntensity, //高级小脸[0,100]
        EP_SeniorTypeSquashedFaceIntensity, //高级瘦颧骨[0,100]
        EP_SeniorTypeForeheadLiftingIntensity, //高级额高[-50,50]
        EP_SeniorTypeWideForeheadIntensity, //高级额宽[-50,50]
        EP_SeniorTypeBigSmallEyeIntensity, //高级大眼[-50,50]
        EP_SeniorTypeEyesOffsetIntensity, //高级眼距[-50,50]
        EP_SeniorTypeEyesRotationIntensity, //高级眼角[-50,50]
        EP_SeniorTypeThinNoseIntensity, //高级瘦鼻[-50,50]
        EP_SeniorTypeLongNoseIntensity, //高级长鼻[-50,50]
        EP_SeniorTypeThinNoseBridgeIntensity, //高级窄鼻梁[-50,50]
        EP_SeniorTypeThinmouthIntensity, //高级小嘴[-50,50]
        EP_SeniorTypeMovemouthIntensity, //高级嘴位[-50,50]
        EP_SeniorTypeChinLiftingIntensity, //高级下巴[-50,50]
    }
```

## EffectParam

```objc
    class EffectParam {
        public int curVal; //当前值
        public int maxVal; //最大值
        public int minVal; //最小值
        public int defVal; //默认值
    }
```

## EffectType

```objc
    enum EffectType {
        ET_BasicBeauty, //基础美颜
        ET_BasicBeautyType, //基础整形
        ET_SeniorBeautyType, //高级整形
        ET_FilterHoliday, //假日滤镜
        ET_FilterClear, //清晰滤镜
        ET_FilterWarm, //暖阳滤镜
        ET_FilterFresh, //清新滤镜
        ET_FilterTender, //粉嫩滤镜
    }
```

## ImageInfo

```objc
    class ImageInfo {
        public int deviceType; //图像类别，默认为相机图像0，用户自定义传入的三方SDK图像为1.
        public int facePointDir; //当 deviceType == 1时，
        // 该变量可以自由设置渲染特效的方向。否则使用底层默认的方向去渲染。默认方向是根据摄像头设备信息设置的。
        public byte[] data; //用户传入的识别数据，通常是像素数据
        public int dir; //图像的方向，通常用陀螺仪方向
        public int orientation; //相机摄像头方向，通常从设备相机信息里获取，当输入为deviceType=1时，该值不生效。
        public int width; //图像宽
        public int height; //图像高
        public int format; //图像格式
        public boolean frontCamera; //是否是前置摄像头
        private int timestamp = 0; //计算人脸识别的时间戳，单位秒，如果不设置，底层使用默认值。

        public void setTimeStamp(int tsp) {
            timestamp = tsp;
        }
    }
```

## GLTexture

```objc
    class GLTexture {
        public int mTextureId; //OpenGL texture ID
        public int mWidth; //OpenGL texture width
        public int mHeight; //OpenGL texture height
        private int mTarget = GLES20.GL_TEXTURE_2D; //OpenGL texture target, e.g. GL_TEXTURE_2D
        private int mFormat = GLES20.GL_RGBA; //OpenGL texture format, e.g. GL_RGBA
    }
```

## 特效包命名

```objc
    String[] mEffectDefaults = { //默认枚举支持的特效
            "beauty_basic.zip",//基础美颜
            "basicthinface.zip",//基础整形（一键整形）
            "faceliftingopt.zip",//高级整形
            "filter_holiday.zip",//假日滤镜
            "filter_clear.zip",//清晰滤镜
            "filter_warm.zip",//暖阳滤镜
            "filter_fresh.zip",//清新滤镜
            "filter_tender.zip",//粉嫩滤镜
    };
```

# SDK V1.4升级流程

步骤1：删除旧的aar包，替换新的aar包。

步骤2：替换venus model。
>    **说明**
>    - 如果使用OrangeHelper创建环境，则可以删除assets下的venusmodel文件及文件夹，Android端已经集成到SDK
里面。
>    - 如果未使用OrangeHelper，则参考OrangeHelper的createContext方法，查看venus model路径设置。

步骤3（可选操作）：添加Face_Key_Points_5.vnmodel文件及路径。如果不使用OrangeHelper类，由于face 对应的venus models增加了一个Face_Key_Points_5.vnmodel文件，对应的加载Venus models的地方需要添加上去对应的路径，否则会加载一些特效包发生崩溃。
>    **说明**

>    如果使用OrangeHelper类，则忽略这一条。

步骤4（可选操作）：更新特效包。

>    **说明**

>   1.2版本无需更新特效包，1.2之前的版本，请联系聚联云技术支持更新对应的特效包。

步骤5：修改整形、高级整形、滤镜特效包的名字。

>    **说明**

>   - 修改规则看OrangeHelper类的mEffectDefaults。如果使用原接口，不使用OrangeHelper类，可以不改。
>   - 升级时，如果用了OrangeHelper类，onDrawFrame方法里面，原来使用是需要宽高交换，如果使用了OrangeHelper，则不需要宽高交换了，OrangeHelper内部会做相应处理。