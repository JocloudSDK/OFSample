# Overview
This demo shows how to implement effects, such as beauty, plastic surgery, filters, stickers, gestures, and makeups, through OrangeFilter SDK on iOS device. Directory structure explanation:

- /OrangeFilterDemo：Store the main code logic
- /venus_models：Store the necessary AI model files for recognition of face, gesture, and background segmentation
- /OrangeFilterDemo/effects：Store all beauty-related special effects packages

The effects supported are as follows:

| Special effects package | Features | Is the intensity adjustable |
| :--- | :--- | :--- |
| Beauty | Whitening, skin beautification (dermabrasion) | YES |
| Filter | One filter package corresponds to one type of filter | YES |
| Plastic surgery | Narrow face, small face, thin jawbone, forehead height, forehead width, big eyes, eye distance, eye corners, thin nose, long nose, narrow nose, small mouth, mouth position, chin | YES |
| Stickers | A special effects package corresponds to a sticker | NO |
| Gesture | 11 kinds of gesture recognition, including v, like, heart (including one-handed, two-handed), Spider-Man, lift, 666, hold a fist, open palms with five fingers, put together, and OK; the demo only shows some of them. | NO |
| Background segmentation | Human body contour detection, other special effects can be further used after human body contour and background segmentation | NO |
| makeups | No makeup starts on air, with makeup on the mirror | NO |


# 1 Quick Start

## 1.1 Prerequisite
- Xcode 10.0 and above.
- iOS 9.0 and above.

## 1.2 Integrate the SDK
- Copy the of_effect.framework package to the /libs directory
- Drag of_effect.framework to the Framewroks directory of the project, and sign of_effect.framework (Embed & Sign) under TARGETS -> General -> Framewroks of the project
- Copy the resources under venus_models to the /venus_models directory
- Copy the special effects package to the /OrangeFilterDemo/effects directory
- Copy the OrangeHelper class to your own project, and subsequent interface calls depend on this class
- Add permissions in the Info.plist file

```objc
1. App Transport Security Settings - > Allow Arbitrary Loads -> YES
2. Privacy -Camera Usage Description -> "Use Camera"
```

> **Note:**
>
> If the path of venus_models or special effects package is changed, please make sure the path is correct when loading resources.

## 1.3 Create an Engine Environment

```objc
    _ofHelper->createContext(serialNumber, licensePath, resDir, aiType);
```

**Parameters**

| Parameter | Description |
| :--- | :--- |
| serialNumber | Authorized serial number, obtained when OrangeFilter is purchased |
| licensePath | Writable path, used to save authorization information |
| aiType | AI effect type:<br>“VENUS_NONE”- basic beautification, disable AI calculation<br>"VENUS_FACE"- advanced beauty and stickers<br> VENUS_GESTURE- basic beautification and gesture recognition<br> VENUS_SEGMENT-basic beautification and background segmentation<br>VENUS_ALL -enbale AI functions |
| resDir | Resource storage root path; includes effect package, venus model files, and cache authorization information; default value is activity.getFilesDir().getPath(); null input is recommended |


## 1.4 Release Engine Resources

```objc
    _ofHelper->destroyContext();
```

> **Note:**
>
> After releasing the resource, the beautification effects are unavailable. You can reusing it by recreating an engine environment.


## 1.5 Rendering Effects Frame by Frame
```objc
    _ofHelper->updateFrameParams(textureIn, textureOut, imageInfo);
```
**Parameter**

| Parameter | Description |
|:---- |:---- |
| textureIn     |  Input texture encapsulated instance, refer to [GLTexture](#GLTexture)   |
| textureOut    |  Output texture encapsulated instance, refer to [GLTexture](#GLTexture)   |
| imageInfo     |  Image recognition encapsulated instance, refer to [GLTexture](#GLTexture)   |


> **Note:**
>
> Each frame needs to be refreshed for getting the the current texture rendering information.
>
> Returne false for failure. Make sure the effect is enabled.

## 1.6 Enable/Disable Effects
Enable/disable beauty, filters, basic and advanced shaping.

```objc
    _ofHelper->enableEffect(effectType, enabled);
```

**Parameter**

| Parameter | Description |
|:---------- |:---- |
| effectType | Effect type, see details in [EffectType](#EffectType) |
| enabled | Whether to enable. When called for the first time and passing false, the effect is not loaded. |

> **Note:**
>
> When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded, and you cannot get the actual parameters by calling `getEffectParam` and `getEffectParamDetail`.

## 1.7 Adjust Effect Intensity
Adjust the effect intensity of beautification, filter, shaping, etc.

```objc
    _ofHelper->setEffectParam(paramType, value);
```

**Parameter**

| Parameter | Description |
|:---- |:---- |
| paramType | Effect parameters ,see details in [EffectParamType](#EffectParamType) |
| value | Effect parameter value to be set; the value range of each parameter is different, see details in  [EffectParamType](#EffectParamType). |


## 1.8 Get the Current Effect Intensity

```objc
    _ofHelper->getEffectParam(paramType);
```

**Parameter**

| Parameter | Description |
|:---- |:---- |
| paramType | Effect parameters, see details in [EffectParamType](#EffectParamType) |

> **Note:**
>
> If the effect is released, disabled, or not initialized, you should load it before getting the effect parameters.


## 1.9 Get the Value Range and Default Value of an Effect Parameter

```objc
    _ofHelper->getEffectParamDetail(paramType, paramVal);
```

**Parameter**

| Parameter | Description |
|:---- |:---- |
| paramType | Effect parameters, see details in [EffectParamType](#EffectParamType) |
| paramVal | Effect parameter vaules, including the maximum value, minimum value, current value and default value. See details in [EffectParam](#EffectParam) |

> **Note:**
>
> If the effect is released, disabled, or not initialized, you should load it before getting the effect parameters.


## 1.10 Release Resources

```objc
    _ofHelper->releaseEffect(effectType);
```

**Parameter**

| Parameter | Description |
| :--- | :--- |
| effectType | Effect type, see details in [EffectType](#EffectType) |

> **Note:**
>
> This API is not recommended to use.


## 1.11 Enbale/Disable Sticker

```objc
    _ofHelper->enableSticker(path, enabled);
```

**Parameter**

| Parameter | Description |
|:---- |:---- |
| path     | The absolute path of the sticker |
| enabled  | Whether to enable the sticker |

> **Note:**
>
> If path is incorrect, loading the effect will fail.
>
> When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded.


## 1.12 Resource Sticker Release

```objc
    _ofHelper->releaseSticker(path);
```

**Parameter**

| Parameter | Description |
| :--- | :--- |
| path | The absolute path of the sticker  |

> **Note:**
>
> If path is incorrect, releasing resources will fail.


## 1.13 Search for the Sticker Execution Results

```objc
    _ofHelper->checkStickerResult(paths, results);
```

**Parameter**

| Parameter | Description |
|:---- |:---- |
| paths     | Full path of the sticker effect to be searched for |
| results   | Return value |

> **Note:**
>
> This API should be called after calling updateFrameParams.

## 1.14 Enable/Disable Gesture

```objc
    _ofHelper->enableGesture(path, enabled);
```

**Parameter**

| Parameter | Description |
|:---- |:---- |
| path     | The absolute path of the gesture effect |
| enabled  | Whether to enable the gesture. |

> **Note:**
>
> If path is incorrect, loading gestures will fail.
>
> When this API is first called with parameter **bEnable** set to **false**, the effects package will not be loaded.

## 1.15 Release Gesture Resources

```objc
    _ofHelper->releaseGesture(path);
```

> **Note:**
>
> If path is incorrect, releasing resources will fail.

## 1.16 Check Whether the engine environment is Available

```objc
    _ofHelper->isContextValid();
```

**Return Value**  
"true"-available；"false"- not available

## 1.17 Set Log Level

```objc
    _ofHelper->setLogLevel(logLevel);
```
You can set it to  0 to close all logs.

## 1.18 Redirect the Log Output

```objc
    _ofHelper->setLogCallback(void(*callback)(const char* msg))
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
    struct ImageInfo
    {
      int deviceType; //Image category, the default is camera image 0, user-defined incoming tripartite sdk image is 1.
      int facePointDir; //When deviceType == 1, this variable can freely set the direction of rendering special effects. Otherwise, use the underlying default direction to render. The default direction is set according to the camera device information.
      unsigned char* data; //The identification data passed in by the user, usually pixel data
      int dir; //The direction of the image, usually the gyroscope direction
      int orientation; //The camera direction is usually obtained from the device camera information. When the input is deviceType=1, the value will not take effect.
      int width;  //Image width
      int height; //Image height
      int format; //Image format
      bool frontCamera; //Is it a front camera
      float timestamp;  //Calculate the timestamp of face recognition, in seconds, if not set, the bottom layer uses the default value.
    };
```

## GLTexture

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

## Effect Package Name

```objc
// One-to-one correspondence with EffectType enumeration value
    std::string defaultEffects[] =
    {
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

Step 1: Delete the old of_effect.framework package and replace the new of_effect.framework package.

Step 2: Delete the resource file under the old venus_models folder and replace the new venus_models resource file (if the path or name is changed, it needs to be modified to the correct path or name).

> **Note:**
>
> If you use OrangeHelper to create an environment, you need to ensure that the path to resDir is correct to ensure that the special effects package and AI model file can be loaded.
>
> If not, you can see path settings of the venus_model in the **createContext** of **OrangeHelper**.

Step 3: Change the method of obtaining image data uint8_t *baseAddress = CVPixelBufferGetBaseAddress(inPixelBuffer); to uint8_t *baseAddress = (uint8_t *)inPixelBuffer;

> **Note:**
>
> When face recognition, you need to pass in the original image data (frameData.imageData = baseAddress;)
>
> Please note that you do not use the method provided by the system to get data, you must force it to be uint8_t * type

Step 4: Add new platform parameters to frameData, add frameData.imageDeviceType = 2;

Step 5: (optional) Update the special effects package.

> **Note:**
>
> For the SDK of version 1.2, there is no need to update the special effects package. For the version earlier than 1.2, please contact the Technical support for help.

Step 6: Edit the effect package name.

> **Note:**
>    
> If the the **OrangeHelper** class is not used, you can ignore the step.
> If the OrangeHelper class is used when upgrading, the process of width and height exchange is no need which is required in the old version.
