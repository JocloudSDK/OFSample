/*
 * Copyright (C) 2016-2020 JOYY Inc.
 * All rights reserved.
 *
 */

#ifndef _ORANGEFILTER_H_
#define _ORANGEFILTER_H_

/************************************************************************/
/* Module export definition.                                            */
/************************************************************************/
#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
#   ifdef OF_LIB
#       define OF_API __declspec(dllexport)
#   elif defined(OF_STATIC)
#       define OF_API
#   else
#       define OF_API __declspec(dllimport)
#   endif
#else
#   define OF_API
#endif

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************/
/* Basic types                                                          */
/************************************************************************/
#if defined(OF_WIN32) || defined(_WIN32) || defined(WIN32)
typedef __int8                              OFInt8;
typedef __int16                             OFInt16;
typedef __int32                             OFInt32;
typedef __int64                             OFInt64;
typedef unsigned __int8                     OFUInt8;
typedef unsigned __int16                    OFUInt16;
typedef unsigned __int32                    OFUInt32;
typedef unsigned __int64                    OFUInt64;
#ifdef _WIN64
typedef unsigned __int64                    OFUIntPtr;
typedef __int64                             OFIntPtr;
#else  /* _WIN64 */
typedef __w64 unsigned int                  OFUIntPtr;
typedef __w64 int                           OFIntPtr;
#endif  /* _WIN64 */
#else
#include <stdint.h>
typedef int8_t                              OFInt8;
typedef int16_t                             OFInt16;
typedef int32_t                             OFInt32;
typedef int64_t                             OFInt64;
typedef uint8_t                             OFUInt8;
typedef uint16_t                            OFUInt16;
typedef uint32_t                            OFUInt32;
typedef uint64_t                            OFUInt64;
typedef uintptr_t                           OFUIntPtr;
typedef intptr_t                            OFIntPtr;
#endif
typedef int                                 OFBool;
typedef char                                OFPath[ 1024 ];
typedef char                                OFText[ 1024 ];
typedef char                                OFName[ 256 ];
typedef char                                OFUUID[ 64 ];
typedef OFUInt32                            OFHandle;

/************************************************************************/
/* Macros for utilites.                                                 */
/************************************************************************/
#define OF_NULL                             0
#define OF_TRUE                             1
#define OF_FALSE                            0
#define OF_INVALID_HANDLE                   0

/************************************************************************/
/* Global enum definition.                                              */
/************************************************************************/
typedef enum _OF_Result
{
    OF_Result_Success = 0,
    OF_Result_Failed,
    OF_Result_NotInit,
    OF_Result_InvalidInput,
    OF_Result_InvalidFrameHandler,
    OF_Result_InvalidFilter,
    OF_Result_InvalidEffect,
    OF_Result_AnimationStoped,
    OF_Result_OpenEffectFileError,
    OF_Result_InvalidEffectVersion,
    OF_Result_InvalidFilterType,
    OF_Result_InvalidSceneType,
    OF_Result_ParseDataError,
    OF_Result_ResNotExist,
    OF_Result_LicenseExpired,
    OF_Result_SerialNumberInvalid,
    OF_Result_NetError,
    OF_Result_LicensePathInvalid
} OF_Result;

typedef enum _OF_LogLevel
{
    OF_LogLevel_Info    = 0x00000001,
    OF_LogLevel_Warn    = 0x00000002,
    OF_LogLevel_Error   = 0x00000004,
    OF_LogLevel_Debug   = 0x00000008,
    OF_LogLevel_Verbose = 0xFFFFFFFF
} OF_LogLevel;

typedef enum _OF_GestureType
{
    OF_GestureType_Unknow = 0,
    OF_GestureType_V,
    OF_GestureType_ThumbUp,
    OF_GestureType_OneHandHeart,
    OF_GestureType_Lift,
    OF_GestureType_TwoHandHeart,
    OF_GestureType_Size,

    OF_GestureType_HappyNewYear_start = 20,
    OF_GestureType_Palm,
    OF_GestureType_ZuoYi,
    OF_GestureType_HappyNewYear_end,

    OF_GestureType_Palm_start = 30,
    OF_GestureType_PalmClose,
    OF_GestureType_PalmOpen,
    OF_GestureType_PalmTogether,
    OF_GestureType_Palm_end,

} OF_GestureType;
    
typedef enum _OF_LimbsType 
{
    OF_GestureType_Hand = 0,
    OF_GestureType_Head = 1,
    OF_GestureType_Foot = 2,
    OF_GestureType_UnKnown = 10,
} OF_LimbsType;

typedef enum _OF_PostureType
{
    OF_PostureType_A = 0,
    OF_PostureType_B = 1,
    OF_PostureType_C = 2,
    OF_PostureType_MaxCount = 3,
    OF_PostureType_UnKnown = 10,
} OF_PostureType;

typedef enum _OF_PixelFormat
{
    OF_PixelFormat_YUV420F,                 /* Support, for iphone camera(kCVPixelFormatType_420YpCbCr8BiPlanarFullRange). */
    OF_PixelFormat_YUV420V,                 /* Not support current, for iphone camera(kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange). */
    OF_PixelFormat_RGB24,                   /* Support. */
    OF_PixelFormat_RGB32,                   /* Support. */
    OF_PixelFormat_BGR24,                   /* Support for windows platform */
    OF_PixelFormat_BGR32,                   /* Support for windows platform */
    OF_PixelFormat_GRAY,                    /* Support. */
    OF_PixelFormat_I420,                    /* Support for windows platform,I420(YU12): Y(w*h)+U(w*h/4)+V(w*h/4)*/
    OF_PixelFormat_NV12,
    OF_PixelFormat_NV21,
} OF_PixelFormat;

typedef enum _OF_RequiredFrameData
{
    OF_RequiredFrameData_None              = 0x00000000,
    OF_RequiredFrameData_FaceLandmarker    = 0x00000001,
    OF_RequiredFrameData_HeadPoseEstimate  = 0x00000002,
    OF_RequiredFrameData_FaceAction        = 0x00000004,
    OF_RequiredFrameData_Gesture           = 0x00000008,
    OF_RequiredFrameData_Body              = 0x00000010,
    OF_RequiredFrameData_BackgroundSegment = 0x00000020,
    OF_RequiredFrameData_ArCamera          = 0x00000040,
    OF_RequiredFrameData_AudioBeat         = 0x00000080,
    OF_RequiredFrameData_Avatar            = 0x00000100,
    OF_RequiredFrameData_Max               = 0x00000100,
    OF_RequiredFrameData_All               = 0xFFFFFFFF
} OF_RequiredFrameData;

typedef enum _OF_ParamType
{
    OF_ParamType_Float  = 0,
    OF_ParamType_Int    = 1,
    OF_ParamType_Bool   = 2,
    OF_ParamType_Enum   = 3,
    OF_ParamType_Color  = 4,
    OF_ParamType_Res    = 5,
    OF_ParamType_ResArr = 6,
    OF_ParamType_String = 7
} OF_ParamType;

typedef enum _OF_EffectPlayMode
{
    OF_EffectPlayMode_Loop,
    OF_EffectPlayMode_OneTime
} OF_EffectPlayMode;

typedef enum _OF_VenusDetectionType
{
    OF_VenusDetectionType_Face,
    OF_VenusDetectionType_BodySegment,
    OF_VenusDetectionType_Gesture,
    OF_VenusDetectionType_UnKnown
} OF_VenusDetectionType;

/************************************************************************/
/* Some base interface definition.                                      */
/************************************************************************/
typedef struct _OF_Rect
{
    double x;
    double y;
    double width;
    double height;
    float angle;
} OF_Rect;

typedef struct _OF_Texture
{
    OFUInt32 textureID;                     /* OpenGL texture id.*/
    OFUInt32 textureID_a;                   /* OpenGL texture id.*/
    OFUInt32 target;                        /* OpenGL texture target, e.g. GL_TEXTURE_2D.*/
    OFUInt32 format;                        /* OpenGL texture format, e.g. GL_RGBA.*/
    OFUInt32 pixelComponentFormat;          /* OpenGL texture pixel component size. e.g. rgba4444 texture format is GL_RGBA, pixelComponentFormat is _GL_UNSIGNED_SHORT_4_4_4_4 */
    OFInt32 width;                          /* OpenGL texture width.*/
    OFInt32 height;                         /* OpenGL texture height.*/
} OF_Texture;

typedef struct _OF_HeadPose
{
    float modelViewMat[16];
    float projectionMat[16];
} OF_HeadPose;

typedef struct _OF_FaceMesh
{
    float* vertices;                        /* Memory needs to be managed by the caller, NOTE: vertices size = verticesCount * 3 */
    OFUInt32 verticesCount;
    float modelViewMat[16];
    float projectionMat[16];
} OF_FaceMesh;

typedef struct _OF_FaceFrameExtData
{
    float facePoints[106 * 2];
    OFUInt32 facePointsCount;
    float face3dPoints[106 * 3];
    OFUInt32 face3dPointsCount;
    float blendshapeCoeffs[51];
} OF_FaceFrameExtData;

typedef struct _OF_FaceFrameData
{
    float box[4];                           /* [left, top, width, height]. */
    float facePoints[106 * 2];              /* [f0x, f0y, f1x, f1y, f2x, f2y, ...]. */
    float extraFacePoints[134 * 2];         /* extra face data */
    float eyeballPoints[40 * 2];            /* eyeball data */
    float score;
    OFUInt32 gender;
    OFUInt32 beautyScore;
    OFUInt32 facePointsCount;
    OFUInt32 extraFacePointsCount;
    OFUInt32 eyeballPointsCount;
    OFBool isMouthOpen;
    OFBool isEyeBlink;
    OFBool isHeadYaw;
    OFBool isHeadPitch;
    OFBool isBrowJump;
    float openMouthIntensity;
    OF_HeadPose headPose;
    OF_FaceMesh faceMesh;
    float blendshapeWeightMap[ 51 ];
} OF_FaceFrameData;

#define OF_MAX_FACEFRAMEDATA_SIZE  5
typedef struct _OF_FaceFrameDataArr
{
    OF_FaceFrameData faceItemArr[OF_MAX_FACEFRAMEDATA_SIZE];
    OFUInt32 faceCount;
    OFInt32 faceUse;
} OF_FaceFrameDataArr;

typedef struct _OF_GestureFrameData
{
    OF_GestureType type;                    /* Gesture type */
    float x;                                /* X coordinate of gesture box' center */
    float y;                                /* Y coordinate of gesture box' center */
    float width;                            /* Width of gesture box' center */
    float height;                           /* Height of gesture box' center */
    float score;
} OF_GestureFrameData;

#define OF_MAX_GESTUREFRAMEDATA_SIZE 10
#define OF_MAX_BODYLANDMARKS_NUMB    32
typedef struct _OF_GestureFrameDataArr
{
    OFUInt32 gestureCount;                                         /* Gesture count */
    OF_GestureFrameData gestureArr[OF_MAX_GESTUREFRAMEDATA_SIZE];  /* Gesture data array */
} OF_GestureFrameDataArr;

typedef struct _OF_BodyFrameData
{
    float bodyPoints[OF_MAX_BODYLANDMARKS_NUMB * 2];               /* [f0x, f0y, f1x, f1y, f2x, f2y, ...]. */
    float bodyPointsScore[OF_MAX_BODYLANDMARKS_NUMB];
    OFUInt32 bodyPointsCount;
    OF_PostureType postureType;
} OF_BodyFrameData;

#define OF_MAX_BODYFRAMEDATA_SIZE 5
typedef struct _OF_BodyFrameDataArr
{
    OF_BodyFrameData bodyItemArr[OF_MAX_BODYFRAMEDATA_SIZE];
    OFUInt32 bodyCount;
} OF_BodyFrameDataArr;

typedef struct _OF_SegmentFrameData
{
    OF_Texture segmentTexture;              /* [in] [out]. Weight map texture of segment result. */
} OF_SegmentFrameData;

typedef struct _OF_AudioFrameData
{
    float beat;
    float loudness;
    float loudnessSmooth;
    OFUInt8 frequencyData[512];
    OFUInt32 frequencyDataCount;
} OF_AudioFrameData;

typedef struct _OF_FrameData
{
    OFUInt8* imageData;                     /* [in] Image raw buffer, some filter may need this, most of the filter is set to OF_NULL. */
    OFInt32 imageDir;                       /* [in] Accelerometer direction. Deg0(0), Deg90(1), Deg180(2), Deg270(3). */
    OFInt32 orientation;                    /* [in] For android, it is the orientation of the camera image. 90, 270 */
    OFInt32 width;                          /* [in] Width of data. */
    OFInt32 height;                         /* [in] Height of data. */
    OFInt32 widthStep;                      /* [in] The number of bytes in per line of the data. */
    OFInt32 format;                         /* [in] Format of input image data, One of the OF_PixelFormat. */
    float timestamp;                        /* [in] The timestamp of Frame Data, unit is seconds. */
    OFBool isUseCustomHarsLib;              /* [in] Whether to use third party human action library, e.g Sensetime, Face++. */
    OF_FaceFrameDataArr faceFrameDataArr;   /* [in] [out] Face action data. */
    OF_GestureFrameDataArr gestureDataArr;  /* [in] [out] Gesture action data. */
    OF_BodyFrameDataArr bodyFrameDataArr;   /* [in] [out] Body action data. */
    OF_SegmentFrameData segmentFrameData;   /* [in] [out] Segment action data. */
    OF_AudioFrameData audioFrameData;       /* [in] [out] Audio data, e.g from music rhythm. */
    float cameraMat[16];                    /* [in] Some AR filter */
    OFBool trackOn;                         /* [in] Has track camera matrix data in cameraMat. */
    OFInt32 curNode;                        /* [out] */
    float pickPoint[2];                     /* [in] [out] */
    OFBool pickOn;                          /* [in] */
    OFBool pickResult;                      /* [out] */
    OFBool frontCamera;                     /* [in] Is front camera. */
	OFInt32 facePointOrientation;			/* [in] For android, it is the orientation of the face point. Deg0(0), Deg90(1), Deg180(2), Deg270(3). */
    OFInt8  imageDeviceType;                /* [in] For android, image where froms. default 0 form camera, others maybe user inputs*/
} OF_FrameData;

typedef struct _OF_Paramf
{
    char name[64];
    float minVal;
    float maxVal;
    float val;
    float defVal;
    OFInt32 nCurveIndex;
} OF_Paramf;

typedef struct _OF_Parami
{
    char name[64];
    OFInt32 minVal;
    OFInt32 maxVal;
    OFInt32 val;
    OFInt32 defVal;
} OF_Parami;

typedef struct _OF_ParamBool
{
    char name[64];
    OFBool val;
    OFBool defVal;
} OF_ParamBool;

typedef struct _OF_ParamString
{
    char name[64];
    OFText val;
    OFText defVal;
} OF_ParamString;

typedef struct _OF_ParamEnum
{
    char name[64];
    OFUInt32 val;
    OFUInt32 defVal;
    OFUInt32 enumCount;
    OFName enumList[40];
} OF_ParamEnum;

typedef struct _OF_ParamRes
{
    char name[64];
    OFUInt32 resType;
    OFName resName;
} OF_ParamRes;

#define OF_MAX_PARAM_RES_ARR_SIZE 100
typedef struct _OF_ParamResArr
{
    char name[64];
    OFUInt32 resType;
    OFUInt32 resCount;
    OFName resNames[OF_MAX_PARAM_RES_ARR_SIZE];
} OF_ParamResArr;

typedef struct _OF_ParamColor
{
    char name[64];
    float val[4];
    float defVal[4];
    OFInt32 nCurveIndex;
} OF_ParamColor;

typedef struct _OF_Param
{
    OFName name;
    OF_ParamType type;
    union
    {
        OF_Paramf* paramf;
        OF_Parami* parami;
        OF_ParamBool* paramBool;
        OF_ParamEnum* paramEnum;
        OF_ParamRes* paramRes;
        OF_ParamResArr* paramResArr;
        OF_ParamColor* paramColor;
        OF_ParamString* paramString;
    } data;
} OF_Param;

#define OF_MAX_EFFECT_INPUT_SIZE   5
#define OF_MAX_EFFECT_FILTERS_SIZE 30
#define OF_MAX_EXT_DATA_SIZE       1024

typedef struct _OF_EffectInfo
{
    OFUInt32 version;
    OFPath resPath;
    OFName luaScriptName;
    OFName audioName;
    OFUInt64 duration;
    OF_EffectPlayMode playMode;
    OFUInt64 timestamp;
    OFBool isFadeout;
    float fadeoutStart;
    OFUInt64 trackDataCacheTime;
    OFUInt32 filterCount;
    OFHandle filterList[OF_MAX_EFFECT_FILTERS_SIZE];
    OFUInt64 filterTimeRange[OF_MAX_EFFECT_FILTERS_SIZE][2];
    char extData[OF_MAX_EXT_DATA_SIZE];
    OFUInt32 extDataSize;
} OF_EffectInfo;

/************************************************************************/
/* Global exported API definition.                                      */
/************************************************************************/
/**
* The first time you use any api, you need to call the function to check
* if the serial number is valid. If it is valid, it will generate a local
* license with the path you gave. the next time you run call it.
* the funtion use the local license offline check first.
* @param  The serial number you purchased
* @param  The local license generate path
* @return == OF_Result_Success in case of success, others in case of failure.
*/
OF_API OF_Result OF_CheckSerialNumber(
    const char* szSerialNumber, const char* szLicensePath);

/**
* The first time you use any api, you need to call the function to check
* if the serial number is valid. If it is valid, it will generate a local
* license with the path you gave. the next time you run call it.
* the funtion can choose use the server license online check first or not.
* @param  The serial number you purchased
* @param  The local license generate path
* @param  check serial number online first or not.
* @return == OF_Result_Success in case of success, others in case of failure.
*/

OF_API OF_Result OF_CheckSerialNumberOnline(
	const char* szSerialNumber, const char* szLicensePath, bool onlineFirst);
/**
 * Allocate an effect context for your effect resources, OF_DestroyContext()
 * can be used to free the context and everything allocated by the context
 * within it. This function must be called before all others.
 * @param[out] contextID, is set to the created context, or to NULL in case
 * of failure
 * @param[in] venusModelDirPath, (Deprecated), path of venus models directory, e,g "d:/venus_models",
 * if you need to conduct face, background segmentation, or gesture detection,
 * in this directory, it should contain the corresponding folders named as 'face',
 * 'segment', or 'gesture' within its models, respectively.
 * if you're not going to use venus, set to OF_NULL.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_CreateContext(
    OFHandle* contextID,                        /* [out] */
    const char* venusModelDirPath               /* [in] Deprecated, set OF_NULL and use OF_SetVenusModels */
);

/**
 * Free an context and all its resources.
 * @param[in] contextID, context to destroy.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_DestroyContext(
    OFHandle contextID);                       /* [in] */

/**
 * Set venus models.
 * @param[in] contextID, context to set venus face/segment/gesture models.
 * @param[in] type, OF_VenusDetectionType.
 * @param[in] models, path of venus models.
 * @param[in] count, number of venus models.
 * @return, OF_Result_Success in case of success, others in case of failure.
*/
OF_API OF_Result OF_SetVenusModels(
    OFHandle contextID,                        /* [in] */
    OFInt32 type,                              /* [in] */
    const char* models[],                      /* [in] */
    OFUInt32 count);                           /* [in] */

/**
 * Create an effect from effect package file,
 * OF_DestroyEffect can be used to free the effect resource.
 * @param[in] contextID, context to create effect.
 * @param[in] filePath, effect package file path.
 * @param[out] effectID, is set to the created effect, or to NULL in case of failure.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_CreateEffectFromPackage(
    OFHandle contextID,                         /* [in] */
    const char* filePath,                       /* [in] */
    OFHandle* effectID);                        /* [out] */

/**
 * Create an effect from effect package in memory,
 * OF_DestroyEffect can be used to free the effect resource.
 * @param[in] contextID, context to create effect.
 * @param[in] stream, package memory pointer.
 * @param[in] streamSize, package memory size.
 * @param[out] effectID, is set to the created effect, or to NULL in case of failure.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_CreateEffectFromPackageMemory(
    OFHandle contextID,                         /* [in] */
    const char* stream,                         /* [in] */
    OFInt32 streamSize,                         /* [in] */
    OFHandle* effectID);                        /* [out] */

/**
 * Free an effect resource.
 * @param[in] contextID, context to destroy effect.
 * @param[in] effectID, effect to destroy.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_DestroyEffect(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

/**
 * Apply a frame, render effect contents on output texture.
 * @param[in] contextID, context to render effect.
 * @param[in] handlerID, effect to render.
 * @param[in] inputArray, input textures, should have one or more input,
 * input count required by effect can use OF_GetRequiredInputCount to query.
 * @param[in] inputCount, input texture array size.
 * @param[in] outputArray, output textures render to,
 * output count required by effect can use OF_GetRequiredOutputCount to query.
 * @param[in] outputCount, output texture array size.
 * @param[in] frameData, frame data for special effects.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_ApplyFrame(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OF_Texture* inputArray,                     /* [in] */
    OFUInt32 inputCount,                        /* [in] */
    OF_Texture* outputArray,                    /* [in] */
    OFUInt32 outputCount,                       /* [in] */
    OF_FrameData* frameData);                   /* [in] */

/**
 * Apply a frame, render effect contents on output texture.
 * @param[in] contextID, context to render effect.
 * @param[in] idList, effect id list to render.
 * @param[in] idCount, effect id list size.
 * @param[in] inputArray, input textures, should have one or more input,
 * input count required by effect can use OF_GetRequiredInputCount to query.
 * @param[in] outputArray, output textures render to,
 * output count required by effect can use OF_GetRequiredOutputCount to query.
 * @param[in] outputCount, output texture array size.
 * @param[in] frameData, frame data for special effects.
 * @param[out] resultList, apply result for every effect.
 * @param[in] resultCount, result list size, should be same to effect id list size.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_ApplyFrameBatch(
    OFHandle contextID,                         /* [in] */
    const OFHandle* idList,                     /* [in] */
    OFUInt32 idCount,                           /* [in] */
    OF_Texture* inputArray,                     /* [in] */
    OFUInt32 inputCount,                        /* [in] */
    OF_Texture* outputArray,                    /* [in] */
    OFUInt32 outputCount,                       /* [in] */
    OF_FrameData* frameData,                    /* [in] */
    OF_Result *resultList,                      /* [out] */
    OFUInt32 resultCount);                      /* [in] */

/**
 * read pixels from texture to data.
 * @param[in] contextID, of context id.
 * @param[in] texture, texture to read.
 * @param[out] data, returns the pixel data.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_ReadPixels(
    OFHandle contextID,
    OF_Texture* texture,
    OFUInt8* data);

/**
 * Query effect is allow input and output texture structure have same texture id,
 * some effect allow input and output texture be same to avoid copy for performance.
 * @param[in] contextID, context.
 * @param[in] handlerID, effect to query.
 * @param[out] isAllow, set to OF_TRUE if same input and output texture is allowed, else to OF_FALSE if not.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_IsAllowSameInputOutput(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OFBool* isAllow);                           /* [out] */

/**
 * Query effect expect how many input textures.
 * @param[in] contextID, context.
 * @param[in] handlerID, effect to query.
 * @param[out] count, input texture count expected by effect.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetRequiredInputCount(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OFUInt32* count);                           /* [out] */

/**
 * Query effect expect how many output textures.
 * @param[in] contextID, context.
 * @param[in] handlerID, effect to query.
 * @param[out] count, output texture count expected by effect.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetRequiredOutputCount(
    OFHandle contextID,                         /* [in] */
    OFHandle handlerID,                         /* [in] */
    OFUInt32* count);                           /* [out] */

/**
 * Query effects expect frame data types.
 * @param[in] contextID, context.
 * @param[in] idList, effect array to query.
 * @param[in] idCount, effect array size.
 * @param[out] requiredFrameData, out frame data types required by effects, combined to one OFUInt32 by bit,
 * see OF_RequiredFrameData types.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetRequiredFrameData(
    OFHandle contextID,                         /* [in] */
    const OFHandle* idList,                     /* [in] */
    OFUInt32 idCount,                           /* [in] */
    OFUInt32* requiredFrameData);               /* [out] */

/**
 * Query effect information.
 * @param[in] contextID, context.
 * @param[in] effectID, effect to query.
 * @param[out] effectInfo, effect information structure to fill, see OF_EffectInfo for detail.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetEffectInfo(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OF_EffectInfo* effectInfo);                 /* [out] */

/**
 * Query filter parameter count,
 * can use OF_GetEffectInfo to query filter list in effect first.
 * @param[in] contextID, context.
 * @param[in] filterID, filter to query.
 * @param[out] paramCount, set to parameter count the filter have.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetFilterParamCount(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32* paramCount);                      /* [out] */

/**
 * Query filter parameter name by index.
 * @param[in] contextID, context.
 * @param[in] filterID, filter to query.
 * @param[in] paramIndex, parameter index to query.
 * @param[out] paramName, set to parameter name.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetFilterParamName(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    OFUInt32 paramIndex,                        /* [in] */
    OFName paramName);                          /* [out] */

/**
 * Query filter parameter data by name.
 * @param[in] contextID, context.
 * @param[in] filterID, filter to query.
 * @param[in] paramName, parameter name to query.
 * @param[out] param, set to parameter data pointer, see OF_Param for detail.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetFilterParamData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    const char* paramName,                      /* [in] */
    OF_Param** param);                          /* [out] */

/**
 * Set filter parameter data by name.
 * @param[in] contextID, context.
 * @param[in] filterID, filter to set.
 * @param[in] paramName, parameter name to set.
 * @param[in] param, parameter data to set, see OF_Param for detail.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_SetFilterParamData(
    OFHandle contextID,                         /* [in] */
    OFHandle filterID,                          /* [in] */
    const char* paramName,                      /* [in] */
    OF_Param* param);                           /* [in] */

typedef void(*OF_LogCallback) (const char* fmtMsg);
typedef void(*OF_LogCallback2) (const char* fmtMsg, const OFInt32 currentLogLevel);

/**
 * Set the log callback function.
 * @param[in] logFun, the log callback function user defined.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_SetLogCallback(
    OF_LogCallback logFun);                    /* [in] */

/**
 * Set the log callback function.
 * @param[in] logFun, the log callback function user defined.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_SetLogCallback2(
    OF_LogCallback2 logFun);                   /* [in] */

/**
 * Set the log level.
 * @param[in] logLevel, the flags of log level, see OF_LogLevel.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_SetLogLevel(
    OFUInt32 logLevel);                     /* [in] */

/**
 * Get the log level.
 * @param[out] logLevel, Default = OF_LogLevel_Error | OF_LogLevel_Warn | OF_LogLevel_Info.
 * @return, OF_Result_Success in case of success, others in case of failure.
 */
OF_API OF_Result OF_GetLogLevel(
    OFUInt32* logLevel);                    /* [out] */

OF_API OF_Result OF_RestartEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_API OF_Result OF_PauseEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_API OF_Result OF_PlayEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID);                         /* [in] */

OF_API OF_Result OF_SeekEffectAnimation(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFUInt64 timestamp);                        /* [in] */

OF_API OF_Result OF_GetEffectAnimationTimeStamp(
    OFHandle contextID,                         /* [in] */
    OFHandle effectID,                          /* [in] */
    OFUInt64* timestamp);                       /* [out] */

#ifdef __cplusplus
}
#endif

#endif // _ORANGEFILTER_H_
