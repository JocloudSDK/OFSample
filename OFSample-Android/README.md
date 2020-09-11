# Preface
This demo shows how to use OrangeFilter SDK on Android; the data source is camera data;
Directory structure explanation:
- assets/effects:Store all beauty-related effects packages
- java/com/yy/orangefilter/test:Store logic code


# Overview

This demo shows how to use OrangeFilter SDK to achieve special effects such as beauty, plastic surgery, filters, stickers, and gestures. The special effects package includes beauty, filters, shaping, stickers, gestures, and background segmentation. The corresponding relationship between the special effects package and functions is as follows:

| Special effects package | Special effects package | Is the intensity adjustable |
| ---:| :--- | ---:|
| Beauty | Whitening, skin beautification (dermabrasion) | YES |
| Filter | One filter package corresponds to one type of filter | YES |
| Plastic surgery | Narrow face, small face, thin jawbone, forehead height, forehead width, big eyes, eye distance, eye corners, thin nose, long nose, narrow nose, small mouth, mouth position, chin | YES | 
| Stickers | A special effects package corresponds to a sticker | NO |
| Gesture | 11 kinds of gesture recognition, including v, like, heart (including one-handed, two-handed), Spider-Man, lift, 666, hold a fist, open palms with five fingers, put together, and OK; the demo only shows some of them. | NO |
| Background segmentation | Human body contour detection, other special effects can be further used after human body contour and background segmentation | NO |


# 1 How to use

## 1.1 Development environment requirements
- Android Studio 2.0 and above.
- Android 4.0 (SDK API 14+) and above.
## 1.2 Integrated SDK
- Copy the aar package to libs.
- Introduce the aar package in build.gradle and add the code:
```objc
  implementation fileTree(dir: 'libs', include: ['*.aar'])  
```
- Copy the OrangeHelper class to your own project, and subsequent interface calls are dependent on this class.
- Put the special effects package in main/assets/effects
- If the resource is downloaded dynamically, you need to pay attention to the download location: custom path +/orangefilter/effects/; it needs to be placed under this path. When using OrangeHelper, pass in the custom path.
- Add permissions to the AndroidManifest file.
```objc
    <!--Camera permissions-->
    <uses-permission android:name="android.permission.CAMERA"/>
    <!--File read and write permissions-->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    <!--Network permissions-->
    <uses-permission android:name="android.permission.INTERNET"/>
    <!--GL permissions-->
    <uses-feature android:glEsVersion="0x00020000" android:required="true" />
```


## 1.3 Create OrangeFilter SDK engine environment

```objc
    OrangeHelper.createContext(context, ofSerialNumber, aiType, resDir);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| context | Context |
| ofSerialNumber | Authorized serial number, obtained when OrangeFilter is purchased |
| aiType | There are five types, namely: VENUS_NONE (not enable AI calculation), VENUS_FACE (enable advanced beauty and stickers), VENUS_GESTURE (enable gesture recognition, gesture expression), VENUS_SEGMENT (enable background segmentation cutout), VENUS_ALL (enable all AI functions) ); For details, please refer to the OrangeHelper class annotation description. |
| resDir | The root path of resource storage, including special effects packages, AI model files, etc. If the path of venus_models or special effects packages is changed, please make sure that the incoming resDir path is correct |

## 1.4 Release OrangeFilter SDK engine environment resources

```objc
    OrangeHelper.destroyContext();
```
> **Note**

>  After the resources are released, the beauty will be unavailable. If you need to use it, you need to recreate the engine environment.

## 1.5 Rendering special effects per frame
```objc
    OrangeHelper.updateFrameParams(textureIn, textureOut, imageInfo);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| textureIn | Encapsulation example of input texture information, refer to [GLTexture](#GLTexture) |
| textureOut | Package example of output texture information, refer to [GLTexture](#GLTexture) |
| imageInfo | To identify the package example of image information, refer to [ImageInfo](#ImageInfo) |

> **Note**

> - Every frame needs to be refreshed, because the current rendering texture information needs to be known at all times.

> - If false is returned, the data frame rendering fails. The reason may be that no special effects are enabled.

## 1.6 Turn on and off beauty, filters, one-click plastic surgery, and advanced plastic surgery

```objc
    OrangeHelper.enableEffect(effectType, bEnable);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| effectType | There are 8 enumeration types [EffectType](#EffectType), respectively, see EffectType of OrangeHelper class for details. |
| bEnable | Whether to open. If it is the first call, pass false and the special effect resource pack will not be loaded. |

> **Note**

> If the enabled parameter is passed false, if it is the first call, the special effect resource package will not be loaded. After that, calling getEffectParam and getEffectParamDetail will not get the real value, because the current resource special effect package has not been loaded, and the parameter values inside cannot be obtained.

## 1.7 Intensity adjustment of beauty, filters, one-key shaping, advanced shaping

```objc
    OrangeHelper.setEffectParam(effectParamType, curVal);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| effectParamType | [EffectParamType](#EffectParamType)enumeration, there are 22 kinds in total, adjust the corresponding function, and input the corresponding enumeration value. See the EffectParamType description of the OrangeHelper class for details. |
| curVal | The set value and the value range corresponding to different functions are different, please refer to [EffectParamType](#EffectParamType). |

## 1.8 Get the current intensity value of the special effect
```objc
    OrangeHelper.getEffectParam(effectParamType);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| effectParamType | There are 22 types of [EffectParamType](#EffectParamType) enumeration. For details, see the description of EffectParamType of the OrangeHelper class. |

> **Note**

> If the corresponding special effect has not been initialized, has not been turned on, or has been released, no value can be obtained. Need to reload to open.

## 1.9 Get a special effect parameter range and default value
```objc
    OrangeHelper.getEffectParamDetail(effectParamType, effectParam);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| effectParamType | There are 22 kinds of [EffectParamType](#EffectParamType) enumeration. For details, please refer to the description of EffectParamType of OrangeHelper class. |
| effectParam | [EffectParam](#EffectParam),the return value, which contains the maximum value, minimum value, current value and default value. |

> **Note**

> If the corresponding special effect has not been initialized, has not been turned on, or has been released, no value can be obtained. Need to reload to open.


## 1.10 Release of resources for beauty, filters, one-click plastic surgery, and advanced plastic surgery

```objc
    OrangeHelper.releaseEffect(EffectType et);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| effectType | There are 8 enumeration types [EffectType](#EffectType), respectively, see EffectType of OrangeHelper class for details. |

> **Note**

> It is not recommended to use the SDK to release resources, because beauty, filters, one-click plastic surgery, and advanced plastic surgery are commonly used functions and consume low resources.

## 1.11 Turning on and off the sticker

```objc
    OrangeHelper.enableSticker(path, bEnable);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the sticker special effect package must be correct, otherwise the special effect package cannot be loaded. |
| bEnable | Whether to open the sticker. |

> **Note**

> - The path must be correct, otherwise the loading of special effects will not succeed.

> - If the enabled parameter is passed false, if it is the first call, the special effect resource pack will not be loaded.

## 1.12 Resource release of stickers

```objc
    OrangeHelper.releaseSticker(path);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| path | The path path is an absolute path and needs to be passed correctly, otherwise it will be released incorrectly or failed.  |

> **Note**

> The path path is an absolute path and needs to be passed correctly, otherwise it will be released incorrectly or failed. 

## 1.13 Query the execution result of the sticker
```objc
    OrangeHelper.checkStickerResult(String[] stickerPaths, int[] stickerResults);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| stickerPaths | The path array of the sticker special effect package to be queried. |
| stickerResults | The result return value of the query corresponding special effect package. |

> **Note**

> The result of each frame of the query sticker can be called after the updateFrameParams call ends. The general usage scenario of this interface is: a certain sticker animation effect needs to be released after use, which can be used as a monitor，See the error code on the official website for details.

## 1.14 Turn on and off gestures

```objc
    OrangeHelper.enableGesture(path, bEnable);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the gesture special effect package must be correct, otherwise the special effect package cannot be loaded. |
| bEnable | Whether to enable the gesture. |

> **Note**

> - The path must be correct, otherwise the loading of gesture effects will fail.

> - If the enabled parameter is passed false, if it is the first call, the special effect resource pack will not be loaded.

## 1.15 Gesture resource release

```objc
    OrangeHelper.releaseGesture(path);
```
Parameter explanation:

| Parameter | Description |
| :--- | :--- |
| path | The path path is an absolute path and needs to be passed correctly, otherwise it will be released incorrectly or failed. |

> **注意**

> The path path is an absolute path and needs to be passed correctly, otherwise it will be released incorrectly or failed.

## 1.16 Check whether the engine environment is available

```objc
    OrangeHelper.isContextValid();
```
Return value:
true：available；false： not available

## 1.17 Set log level

```objc
    OrangeHelper.setLogLevel(logLevel);
```
You can pass 0 to close all logs. For details, refer to the description of setLogLevel in the OrangeHelper class.

## 1.18 Structure definition and enumeration

```objc
    OrangeHelper.setLogCallback(OrangeFilter.OF_LogListener logListener)
```
## 1.19 Obfuscation settings
If there is a confusion setting, add the following code to the proguard-rules file:

```objc
    -keep class com.orangefilter.**  {*;}
```

# 2 Structure definition and enumeration

## EffectParamType

```objc
    enum EffectParamType {
        EP_BasicBeautyIntensity, //Basic beauty[0,100]
        EP_BasicBeautyOpacity, //Basic microdermabrasion strength[0,100] (Hazy dermabrasion)
        EP_ClearBeautyIntensity, //Basic beauty[0,100]
        EP_ClearBeautyOpacity, //Basic microdermabrasion strength[0,100] (Clear dermabrasion)
        EP_FilterHolidayIntensity, //Holiday filter strength[0,100]
        EP_FilterClearIntensity, //Clear filter strength[0,100]
        EP_FilterWarmIntensity, //Warm Sun Filter strength[0,100]
        EP_FilterFreshIntensity, //Fresh filter strength[0,100]
        EP_FilterTenderIntensity, //Tender filter strength[0,100]
        EP_BasicTypeIntensity, //Basic plastic strength[0,100]
        EP_SeniorTypeThinFaceIntensity, //Senior face-lift[0,100]
        EP_SeniorTypeSmallFaceIntensity, //Senior face[0,100]
        EP_SeniorTypeSquashedFaceIntensity, //Advanced skinny cheekbones[0,100]
        EP_SeniorTypeForeheadLiftingIntensity, //Senior high[-50,50]
        EP_SeniorTypeWideForeheadIntensity, //Advanced forehead[-50,50]
        EP_SeniorTypeBigSmallEyeIntensity, //Senior big eyes[-50,50]
        EP_SeniorTypeEyesOffsetIntensity, //Advanced eye distance[-50,50]
        EP_SeniorTypeEyesRotationIntensity, //Senior eye corner[-50,50]
        EP_SeniorTypeThinNoseIntensity, //Advanced thin nose[-50,50]
        EP_SeniorTypeLongNoseIntensity, //Senior Proboscis[-50,50]
        EP_SeniorTypeThinNoseBridgeIntensity, //Advanced narrow nose[-50,50]
        EP_SeniorTypeThinmouthIntensity, //Senior small mouth[-50,50]
        EP_SeniorTypeMovemouthIntensity, //Advanced mouth position[-50,50]
        EP_SeniorTypeChinLiftingIntensity, //Senior Chin[-50,50]
    }
```

## EffectParam

```objc
    class EffectParam {
        public int curVal; //The current value
        public int maxVal; //Max value
        public int minVal; //Min value
        public int defVal; //Default value
    }
```

## EffectType

```objc
    enum EffectType {
        ET_BasicBeauty, //Basic beauty (Hazy dermabrasion)
        ET_ClearBasicBeauty, //Basic beauty (Clear dermabrasion)
        ET_BasicBeautyType, //Basic plastic surgery
        ET_SeniorBeautyType, //Advanced plastic surgery
        ET_FilterHoliday, //Holiday filter
        ET_FilterClear, //Clear filter
        ET_FilterWarm, //Warm sun filter
        ET_FilterFresh, //Fresh filter
        ET_FilterTender, //Tender filter
    }
```

## ImageInfo

```objc
    class ImageInfo {
        public int deviceType; //Image category, the default is camera image 0, user-defined incoming tripartite sdk image is 1.
        public int facePointDir; //When deviceType == 1, this variable can freely set the direction of rendering special effects. Otherwise, use the underlying default direction to render.
        // The default direction is set according to the camera device information.
        public byte[] data; //The identification data passed in by the user, usually pixel data
        public int dir; //The direction of the image, usually the gyroscope direction
        public int orientation; //The camera direction is usually obtained from the device camera information. When the input is deviceType=1, the value will not take effect.
        public int width; //Image width
        public int height; //Image high
        public int format; //Image format
        public boolean frontCamera; //Is it a front camera
        private int timestamp = 0; //Calculate the timestamp of face recognition, in seconds, if not set, the bottom layer uses the default value.

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
    String[] mEffectDefaults = {
          "beauty_basic.zip",  //Basic beauty (Hazy dermabrasion)
          "beauty5_basic.zip", //Basic beauty (Clear dermabrasion)
          "basicthinface_1.2.0.zip", //Basic plastic surgery
          "faceliftingopt_1.2.1.zip",//Advanced plastic surgery
          "slookuptable1.zip", //Holiday filter
          "slookuptable2.zip", //Clear filter
          "slookuptable3.zip", //Warm sun filter
          "slookuptable4.zip", //Fresh filter
          "slookuptable5.zip"  //Tender filter
    };
```

# SDK V1.4Upgrade process

Step 1: Delete the old aar package and replace the new aar package.

Step 2: Replace the venus model.
>    **Description**
>    - If you use OrangeHelper to create the environment, you can delete the venusmodel files and folders under assets, and the Android side has been integrated into the SDK
       inside.
>    - If OrangeHelper is not used, you can refer to the createContext method of OrangeHelper to view the venus_model path setting.

Step 3 (optional operation): Add the Face_Key_Points_5.vnmodel file and path. If you don't use the OrangeHelper class, because the venus models corresponding to the face have added a Face_Key_Points_5.vnmodel file, you need to add the corresponding path to the place where the Venus models are loaded, otherwise it will load some special effects packages and crash.
>    **Description**

>    If you use the OrangeHelper class, ignore this one.

Step 4 (optional operation): Update the special effects package.

>    **Description**

>   There is no need to update the special effect package for version 1.2. For versions before 1.2, please contact the technical support of Jocloud to update the corresponding special effect package.

Step 5: Modify the name of the plastic, advanced plastic, and filter special effects package.

>    **Description**

>   - To modify the rules, see mEffectDefaults of the OrangeHelper class. If you use the original interface instead of the OrangeHelper class, you can leave it unchanged.
>   - When upgrading, if you use the OrangeHelper class, in the onDrawFrame method, the original use requires width and height exchange. If you use OrangeHelper, you don't need to exchange width and height, and OrangeHelper will do the corresponding processing internally.