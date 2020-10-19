# Overview
This demo shows how to implement effects, such as beauty, plastic surgery, filters, stickers, and gestures, through OrangeFilter SDK on Android device. Directory structure explanation:

- assets/effects: Store all beauty-related effects packages
- java/com/yy/orangefilter/test: Store logic codes

 The effects supported are as follows:

| Effect Package | Function | Adjustable or not |
| ---:| :--- | ---:|
| Beauty | Brighten, smooth Skin | Yes |
| Filter | One filter package corresponds to one type of filter | Yes |
| Shaping | Face slimming/shrinking, eye enlargement/distancing, forehead height/width, nose shape and lip shape adjustments | Yes |
| Sticker | An effects package corresponds to a type of a sticker | No |
| Gesture | 11 kinds of gesture recognition—including victory, thumbs-up, (one or two-handed) heart sign, Spiderman, holding, hang loose, deference, open palm, clasped palms, and OK. | No |
| Background Segmentation | Detection body contour and separate someone from the background, and then you can replace the backgrounds with desired effects | No |
| Makeup | No makeup starts on air, with makeup on the mirror | No |


# 1 Quick Start

## 1.1 Prerequisite
- Android Studio 2.0 and above.
- Android 4.0 (SDK API 14+) and above.
## 1.2 Integrate the SDK
1. Copy the aar package to libs.
2. Add the aar package in ***build.gradle*** and add the code:

```objc
  implementation fileTree(dir: 'libs', include: ['*.aar'])  
```
3. Copy the OrangeHelper class to your own project, and the interface calls are dependent on it.

4. Put the the effect package in the directory ***main/assets/effects***.

   > **Note:**
   >
   > The path to save dynamically downloaded resources is: ***custom path +/orangefilter/effects/***. And the ***custom path*** should be set when using `OrangeHelper`.

5. Add permissions to the AndroidManifest file.
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


## 1.3 Create an Engine Environment

```objc
    OrangeHelper.createContext(context, ofSerialNumber, aiType, resDir);
```
**Parameters** 

| Parameter | Description |
| :--- | :--- |
| context | Context |
| ofSerialNumber | Serial number for authentication, obtained when OrangeFilter is purchased |
| aiType | AI effect type:<br>“VENUS_NONE”- basic beautification, disable AI calculation<br>"VENUS_FACE"- advanced beauty and stickers<br> VENUS_GESTURE- basic beautification and gesture recognition<br> VENUS_SEGMENT-basic beautification and background segmentation<br>VENUS_ALL -enbale AI functions |
| resDir | Resource storage root path; includes effect package, venus model files, and cache authorization information; default value is activity.getFilesDir().getPath(); null input is recommended |

## 1.4 Release Engine Resources

```objc
    OrangeHelper.destroyContext();
```
> **Note**

>  After releasing the resource, the beautification effects are unavailable. You can reusing it by recreating an engine environment.

## 1.5 Rendering Effects Frame by Frame
```objc
    OrangeHelper.updateFrameParams(textureIn, textureOut, imageInfo);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| textureIn | Input texture encapsulated instance, refer to [GLTexture](#GLTexture) |
| textureOut | Output texture encapsulated instance, refer to [GLTexture](#GLTexture) |
| imageInfo | Image recognition encapsulated instance, refer to [ImageInfo](#ImageInfo) |

> **Note**
> - Each frame needs to be refreshed for getting the the current texture rendering information.
> -  Returne false for failure. Make sure the effect is enabled.

## 1.6 Enable/Disable Effects
Enable/disable beauty, filters, basic and advanced shaping.

```objc
    OrangeHelper.enableEffect(effectType, bEnable);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectType | Effect type, see details in [EffectType](#EffectType) |
| bEnable | Whether to enable. When called for the first time and passing false, the effect is not loaded. |

> **Note:**
>
> When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded, and you cannot get the actual parameters by calling `getEffectParam` and `getEffectParamDetail`.

## 1.7 Adjust Effect Intensity
Adjust the effect intensity of beautification, filter, shaping, etc.
```objc
    OrangeHelper.setEffectParam(effectParamType, curVal);
```
**Parameter **

| Parameter | Description |
| :--- | :--- |
| effectParamType | Effect parameters ,see details in [EffectParamType](#EffectParamType) |
| curVal | Effect parameter value to be set; the value range of each parameter is different, see details in  [EffectParamType](#EffectParamType). |

## 1.8 Get the Current Effect Intensity 
```objc
    OrangeHelper.getEffectParam(effectParamType);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectParamType | Effect parameters, see details in [EffectParamType](#EffectParamType) |

> **Note:**
>
> If the effect is released, disabled, or not initialized, you should load it before getting the effect parameters.

## 1.9 Get the Value Range and Default Value of an Effect Parameter
```objc
OrangeHelper.getEffectParamDetail(effectParamType, effectParam);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectParamType | Effect parameters, see details in [EffectParamType](#EffectParamType) |
| effectParam | Effect parameter vaules, including the maximum value, minimum value, current value and default value. See details in [EffectParam](#EffectParam) |

> **Note**
>
> If the effect is released, disabled, or not initialized, you should load it before getting the effect parameters.


## 1.10 Release Resources

```objc
    OrangeHelper.releaseEffect(EffectType et);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectType | Effect type, see details in [EffectType](#EffectType) |

> **Note**
>
> This API is not recommended to use.

## 1.11 Enbale/Disable Sticker

```objc
    OrangeHelper.enableSticker(path, bEnable);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the sticker |
| bEnable | Whether to enable the sticker |

> **Note**
>
> - If path is incorrect, loading the effect will fail.
> - When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded.

## 1.12 Resource Sticker Release

```objc
    OrangeHelper.releaseSticker(path);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the sticker  |

> **Note**
>
> If path is incorrect, releasing resources will fail.

## 1.13 Search for the Sticker Execution Results
```objc
    OrangeHelper.checkStickerResult(String[] stickerPaths, int[] stickerResults);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| stickerPaths | Full path of the sticker effect to be searched for |
| stickerResults | Return value |

> **Note**
>
> This API should be called after calling updateFrameParams.

## 1.14 Enable/Disable Gesture

```objc
    OrangeHelper.enableGesture(path, bEnable);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the gesture effect |
| bEnable | Whether to enable the gesture. |

> **Note:**
>
> - If path is incorrect, loading gestures will fail.
> - When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded.

## 1.15 Release Gesture Resources

```objc
    OrangeHelper.releaseGesture(path);
```
**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the gesture effect |

> **Note:**
> If path is incorrect, releasing resources will fail.

## 1.16 Check Whether the engine environment is Available

```objc
    OrangeHelper.isContextValid();
```
**Return Value**
"true"-available；"false"- not available

## 1.17 Set Log Level

```objc
    OrangeHelper.setLogLevel(logLevel);
```
You can set it to  0 to close all logs. 

## 1.18 Redirect the Log Output

```objc
    OrangeHelper.setLogCallback(OrangeFilter.OF_LogListener logListener)
```
## 1.19 Obfuscation Settings
If there is a confusion setting, add the following code to the proguard-rules file:

```objc
    -keep class com.orangefilter.**  {*;}
```

# 2 Structure and Enumeration

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
        public int height; //Image height
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

## Effect Package Name

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

# 3 Upgrade Method

The method to upgrade the SDK to version 1.4 is as below.

Step 1: Replace the old aar package with the new one.

Step 2: Replace the venus model.
>    **Note:**
>
>    - If the engine environment is created by the OrangeHelper , you can delete the venus model in the file ***assets***.
>    - If not, you can see path settings of the venus_model in the **createContext** of **OrangeHelper**.

Step 3: (optional) Add the ***Face_Key_Points_5.vnmodel*** file and path. 

Since a ***face_key_POINts_5.vnModel*** file is added to the face corresponding Venus Models, the path to which the Venus models have been loaded needs to be added. If the OrangeHelper class is not used, a crash occurs when loading effects package.

>    **Note:** If the OrangeHelper class is used, ignore this step.

Step 4: (optional) Update the special effects package.

>    **Note:** For the SDK of version 1.2, there is no need to update the special effects package. For the version earlier than 1.2, please contact the Technical support for help.

Step 5: Edit the effect package name.

> **Note:**
>    
> - If the the **OrangeHelper** class is not used, you can ignore the step.
> - If the OrangeHelper class is used when upgrading, the process of width and height exchange is no need which is required in the old version.