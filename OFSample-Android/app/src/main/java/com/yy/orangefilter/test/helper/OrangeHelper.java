package com.yy.orangefilter.test.helper;

import android.content.Context;
import android.content.SharedPreferences;
import android.opengl.GLES20;
import android.util.Log;

import com.orangefilter.OrangeFilter;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import static android.preference.PreferenceManager.getDefaultSharedPreferences;

// Note: This class is just a package for the OrangeFilter SDK API, it is for the SDK
// The secondary encapsulation of API shields the usage details of SDK. Try to meet the needs of users to simplify the use of SDK.
// This category is recommended to be directly introduced into user projects. Unless necessary, it is not recommended to modify, because subsequent developers will incrementally update this category with the upgrade of the SDK version to optimize the user experience.
// If users have special requirements, they can still refer to the implementation of this class and use the native interface of OrangeFilterSDK.

public class OrangeHelper {
    final private static String TAG = "OrangeHelper";
    final private static String mSDK_VERSION = "1.5.4";
    final public static int VENUS_NONE = 0; //Without AI calculation, advanced beauty, stickers, gesture recognition, background segmentation cannot be used
    final public static int VENUS_FACE = 1; //Enable advanced beauty and stickers
    final public static int VENUS_GESTURE = 2; //Enable gesture recognition, gesture expression
    final public static int VENUS_SEGMENT = 4;  //Enable background split cutout
    final public static int VENUS_ALL = 7;  //Enable all AI functions.

    final public static int LG_Info = OrangeFilter.OF_LogLevel_Info; //Info level
    final public static int LG_Warn = OrangeFilter.OF_LogLevel_Warn; //Warn level
    final public static int LG_Error = OrangeFilter.OF_LogLevel_Error; //Error level
    final public static int LG_Debug = OrangeFilter.OF_LogLevel_Debug; //Debug level
    final public static int LG_Verbose = OrangeFilter.OF_LogLevel_Verbose; //Verbose level

    public static String LogMsg = ""; //Record the most recent error message.

    private static Map<String, Integer> mEffectIds = new HashMap(); //Special effects package loaded in memory
    private static Vector<Integer> mEffectJobs = new Vector<>(); //Effective special effects package

    private static String mEffectPackagePaths = ""; //Special effect package storage path
    private static String mVenusModlePath = ""; //The storage path of the model data file that the AI calculation depends on.
    private static int mOFContext = 0;
    private static long mStartTime;
    private static OrangeFilter.OF_FrameData mFrameData;
    private static OrangeFilter.OF_Texture[] mInputs;
    private static OrangeFilter.OF_Texture[] mOutputs;
    private static int[] mEffectJobIdsCache;
    private static int[] mEffectJobResultsCache;

    //The structure of passing Opengl texture parameters, and the texture passed by the structure is used for rendering.
    public static class GLTexture {
        public int mTextureId; //OpenGL texture id
        public int mWidth; //OpenGL texture width.
        public int mHeight; //OpenGL texture height.
        private int mTarget = GLES20.GL_TEXTURE_2D; //OpenGL texture target, e.g. GL_TEXTURE_2D.
        private int mFormat = GLES20.GL_RGBA; //OpenGL texture format, e.g. GL_RGBA.
    }

    //Pass the parameter structure of the recognition image, and the image passed by the structure is used for face and action recognition.
    public static class ImageInfo {
        public int deviceType; //Image category, the default is camera image 0, user-defined incoming tripartite sdk image is 1.
        public int facePointDir; //When deviceType == 1, this variable can freely set the direction of rendering special effects.
        // Otherwise, use the underlying default direction to render. The default direction is set according to the camera device information.
        public byte[] data; //The identification data passed in by the user, usually pixel data
        public int dir; //The direction of the image, usually the gyroscope direction
        public int orientation; //The camera direction is usually obtained from the device camera information. When the input is deviceType=1, this value will not take effect.
        public int width; //Image width
        public int height; //Image high
        public int format; //Image format
        public boolean frontCamera; //Is it a front camera
        private int timestamp = 0; //Calculate the timestamp for face recognition, in seconds, if not set, the bottom layer uses the default value.

        public void setTimeStamp(int tsp) {
            timestamp = tsp;
        }
    }

    //Special effect parameter structure, used to query parameters
    public static class EffectParam {
        public int curVal; //The current value
        public int maxVal; //Max
        public int minVal; //Minimum
        public int defVal; //Defaults
    }

    public enum EffectType {
        ET_BasicBeauty, //Basic beauty
        ET_BasicBeautyType, //Basic plastic surgery
        ET_SeniorBeautyType, //Advanced plastic surgery
        ET_FilterHoliday, //Holiday filter
        ET_FilterClear, //Clear filter
        ET_FilterWarm, //Warm filter
        ET_FilterFresh, //Fresh filter
        ET_FilterTender, //Tender filter
        ET_BasicBeautyClear, //Basic beauty（Clear skin）

        //20款滤镜风格
        ET_FilterAdaier, //Adaier filter
        ET_FilterDanya, //Danya filter
        ET_FilterJiaZhou, //JiaZhou filter
        ET_FilterMojito, //Mojito filter
        ET_FilterFennen, //Fennen filter
        ET_FilterPure, //Pure filter
        ET_FilterSweet, //Sweet filter
        ET_FilterWenNuan, //WenNuan filter
        ET_FilterSun, //Sun filter
        ET_FilterGrilWhite, //GrilWhite filter
        ET_FilterGrilLuolita, //Luolita filter
        ET_FilterGrilLiterature, //Literature filter
        ET_FilterAmorous, //Amorous filter
        ET_FilterDelicious, //Delicious filter
        ET_FilterEnhanceClear, //EnhanceClear filter
        ET_FilterEnhanceNoulan, //EnhanceNoulan filter
        ET_FilterPeach, //Peach filter
        ET_FilterCream, //Cream filter
        ET_FilterFilm, //Film filter
        ET_FilterMagazine, //Magazine filter

    }

    final private static String[] mEffectDefaults = { //Special effects supported by default enumeration
            "beauty_basic.zip",
            "basicthinface.zip",
            "faceliftingopt.zip",
            "filter_holiday.zip",
            "filter_clear.zip",
            "filter_warm.zip",
            "filter_fresh.zip",
            "filter_tender.zip",
            "beauty5_basic.zip",//Clear skin
            //20款滤镜风格
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
            "zhigan-zazhi.zip"
    };

    private static String[] mEffectConfigs = { //customer configs effects 's paths.
            null, null, null, null, null, null, null, null, null, //9
            null, null, null, null, null, null, null, null, null,null, //10
            null, null, null, null, null, null, null, null, null,null //10
    };

    public enum EffectParamType {
        EP_BasicBeautyIntensity, //Basic beauty
        EP_BasicBeautyOpacity, //Basic microdermabrasion strength
        EP_BasicBeautyClearIntensity, //Basic beauty（Clear Skin）
        EP_BasicBeautyClearOpacity, //Clear Skin
        EP_FilterHolidayIntensity, //holiday
        EP_FilterClearIntensity, //clear
        EP_FilterWarmIntensity, //warm
        EP_FilterFreshIntensity, //fresh
        EP_FilterTenderIntensity, //pink
        EP_BasicTypeIntensity, //basic plastic
        EP_SeniorTypeThinFaceIntensity, //face
        EP_SeniorTypeSmallFaceIntensity, //resize
        EP_SeniorTypeSquashedFaceIntensity, //cheek
        EP_SeniorTypeForeheadLiftingIntensity, //forehead
        EP_SeniorTypeWideForeheadIntensity, //forehead
        EP_SeniorTypeBigSmallEyeIntensity, //eye size
        EP_SeniorTypeEyesOffsetIntensity, //distance
        EP_SeniorTypeEyesRotationIntensity, //slant
        EP_SeniorTypeThinNoseIntensity, //slim
        EP_SeniorTypeLongNoseIntensity, //length
        EP_SeniorTypeThinNoseBridgeIntensity, //btidge
        EP_SeniorTypeThinmouthIntensity, //resize
        EP_SeniorTypeMovemouthIntensity, //position
        EP_SeniorTypeChinLiftingIntensity, //chin

        //20款滤镜风格
        EP_FilterAdaierIntensity,
        EP_FilterDanyaIntensity,
        EP_FilterJiaZhouIntensity,
        EP_FilterMojitoIntensity,
        EP_FilterFennenIntensity,
        EP_FilterPureIntensity,
        EP_FilterSweetIntensity,
        EP_FilterWenNuanIntensity,
        EP_FilterSunIntensity,
        EP_FilterGrilWhiteIntensity,
        EP_FilterGrilLuolitaIntensity,
        EP_FilterGrilLiteratureIntensity,
        EP_FilterAmorousIntensity,
        EP_FilterDeliciousIntensity,
        EP_FilterEnhanceClearIntensity,
        EP_FilterEnhanceNoulanIntensity,
        EP_FilterPeachIntensity,
        EP_FilterCreamIntensity,
        EP_FilterFilmIntensity,
        EP_FilterMagazineIntensity

    }

    final private static String[] mEffectParamNames = {
            "Intensity", //Hazy
            "Opacity",
            "Intensity",//clear
            "Opacity",
            "Intensity", //filter
            "Intensity",
            "Intensity",
            "Intensity",
            "Intensity",
            "Intensity", //basic plastic
            "ThinfaceIntensity", //Senior plastic
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
            //20款风格滤镜 filter
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
            "Intensity"
    };

    private static int getEffectId(EffectParamType ep) {
        if (0 == mOFContext) {
            return 0;
        }
        EffectType et = EffectType.ET_SeniorBeautyType;
        switch (ep) {
            case EP_BasicBeautyIntensity:
            case EP_BasicBeautyOpacity: {
                et = EffectType.ET_BasicBeauty;
            }
            break;
            case EP_BasicBeautyClearIntensity:
            case EP_BasicBeautyClearOpacity: {
                et = EffectType.ET_BasicBeautyClear;
            }
            break;
            case EP_BasicTypeIntensity: {
                et = EffectType.ET_BasicBeautyType;
            }
            break;
            case EP_FilterHolidayIntensity: {
                et = EffectType.ET_FilterHoliday;
            }
            break;
            case EP_FilterClearIntensity: {
                et = EffectType.ET_FilterClear;
            }
            break;
            case EP_FilterWarmIntensity: {
                et = EffectType.ET_FilterWarm;
            }
            break;
            case EP_FilterFreshIntensity: {
                et = EffectType.ET_FilterFresh;
            }
            break;
            case EP_FilterTenderIntensity: {
                et = EffectType.ET_FilterTender;
            }
            break;
            case EP_FilterAdaierIntensity: {
                et = EffectType.ET_FilterAdaier;
            }
            break;
            case EP_FilterDanyaIntensity: {
                et = EffectType.ET_FilterDanya;
            }
            break;
            case EP_FilterJiaZhouIntensity: {
                et = EffectType.ET_FilterJiaZhou;
            }
            break;
            case EP_FilterMojitoIntensity: {
                et = EffectType.ET_FilterMojito;
            }
            break;
            case EP_FilterFennenIntensity: {
                et = EffectType.ET_FilterFennen;
            }
            break;
            case EP_FilterPureIntensity: {
                et = EffectType.ET_FilterPure;
            }
            break;
            case EP_FilterSweetIntensity: {
                et = EffectType.ET_FilterSweet;
            }
            break;
            case EP_FilterWenNuanIntensity: {
                et = EffectType.ET_FilterWenNuan;
            }
            break;
            case EP_FilterSunIntensity: {
                et = EffectType.ET_FilterSun;
            }
            break;
            case EP_FilterGrilWhiteIntensity: {
                et = EffectType.ET_FilterGrilWhite;
            }
            break;
            case EP_FilterGrilLuolitaIntensity: {
                et = EffectType.ET_FilterGrilLuolita;
            }
            break;
            case EP_FilterGrilLiteratureIntensity: {
                et = EffectType.ET_FilterGrilLiterature;
            }
            break;
            case EP_FilterAmorousIntensity: {
                et = EffectType.ET_FilterAmorous;
            }
            break;
            case EP_FilterDeliciousIntensity: {
                et = EffectType.ET_FilterDelicious;
            }
            break;
            case EP_FilterEnhanceClearIntensity: {
                et = EffectType.ET_FilterEnhanceClear;
            }
            break;
            case EP_FilterEnhanceNoulanIntensity: {
                et = EffectType.ET_FilterEnhanceNoulan;
            }
            break;
            case EP_FilterPeachIntensity: {
                et = EffectType.ET_FilterPeach;
            }
            break;
            case EP_FilterCreamIntensity: {
                et = EffectType.ET_FilterCream;
            }
            break;
            case EP_FilterFilmIntensity: {
                et = EffectType.ET_FilterFilm;
            }
            break;
            case EP_FilterMagazineIntensity: {
                et = EffectType.ET_FilterMagazine;
            }
            break;

            default: {
                et = EffectType.ET_SeniorBeautyType;
            }
            break;
        }
        String path = null != mEffectConfigs[et.ordinal()] ? mEffectConfigs[et.ordinal()] :
                mEffectPackagePaths + mEffectDefaults[et.ordinal()];
        int effectId = 0;
        if (mEffectIds.containsKey(path)) {
            effectId = mEffectIds.get(path);
        }
        return effectId;
    }

    private static String getFilterParamName(EffectParamType ep) {
        return mEffectParamNames[ep.ordinal()];
    }

    private static int getFilterId(EffectParamType ep) {
        if (0 == mOFContext) {
            return 0;
        }
        int tarId = getFilterIdByEffectId(getEffectId(ep), getFilterParamName(ep));
        if (0 == tarId) {
            LogMsg = "该变量所属的特效包尚未加载。" + ep.toString();
            Log.e(TAG, LogMsg);
        }
        return tarId;
    }

    private static int getFilterIdByEffectId(int effectId, String filterParamName) {
        if (0 == mOFContext) {
            return 0;
        }
        if (effectId > 0) {
            //获取特效包所有信息的结构体。
            OrangeFilter.OF_EffectInfo effectInfo = new OrangeFilter.OF_EffectInfo();
            if (OrangeFilter.OF_Result_Success != OrangeFilter.getEffectInfo(mOFContext, effectId,
                    effectInfo)) {
                LogMsg = "获取effectinfo失败， effectId = " + effectId;
                Log.e(TAG, LogMsg);
                return 0;
            }
            if (1 == effectInfo.filterCount) {
                return effectInfo.filterList[0];
            }
            int tarFilterId = 0;
            //每个特效包里包含多个脚本，每个脚本有自己的filterId，通过包里脚本的个数进行遍历。
            for (int i = 0; i < effectInfo.filterCount; ++i) {
                //遍历特效包里的脚本filterId
                int filterId = effectInfo.filterList[i];
                //每个脚本对象内部存在多个变量，获取变量数量
                int filterParamCout = OrangeFilter.getFilterParamCount(mOFContext, filterId);
                //遍历当前脚本内部的变量
                for (int j = 0; j < filterParamCout; ++j) {
                    //获取当前变量的变量名
                    String paramName = OrangeFilter.getFilterParamName(mOFContext, filterId, j);
                    //匹配变量名和目标变量名，匹配成功返回目标变量所属的filterId（脚本实例id）
                    if (paramName.equals(filterParamName)) {
                        tarFilterId = filterId;
                        break;
                    }
                }
                if (tarFilterId > 0) {
                    break;
                }
            }
            return tarFilterId;
        } else {
            LogMsg = "该变量所属的特效包尚未加载。" + filterParamName;
            Log.e(TAG, LogMsg);
        }
        return 0;
    }

    //激活某个指定特效，如果不存在就尝试加载
    private static boolean enableEffect(String path, boolean bEnable) {
        if (null == path || 0 == path.length()) {
            return false;
        }
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用enableOrangeEffect接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        int iRet = 0;
        if (bEnable) {
            //是否已经加载
            if (!mEffectIds.containsKey(path)) {
                iRet = OrangeFilter.createEffectFromPackage(mOFContext, path);
                if (0 != iRet) {
                    mEffectIds.put(path, iRet);
                    Log.i(TAG, "加载特效包成功 " + path);
                } else {
                    LogMsg = "加载特效包失败，失败信息路径 " + path;
                    Log.e(TAG, LogMsg);
                    return false;
                }
            }
            int effectId = (int) mEffectIds.get(path);
            //是否已经生效
            if (mEffectJobs.contains(effectId)) {
                return true;
            }
            //todo:改成优先级队列
            mEffectJobs.add(effectId);
        } else {
            //是否已经加载
            if (!mEffectIds.containsKey(path)) {
                return true;
            }
            //是否已经生效
            int effectId = (int) mEffectIds.get(path);
            if (!mEffectJobs.contains(effectId)) {
                return true;
            }

            mEffectJobs.removeElement(effectId);
        }
        return true;
    }

    //释放某个指定特效包
    private static boolean releaseEffect(String path) {
        if (0 == mOFContext) {
            return false;
        }
        //是否已经加载
        if (!mEffectIds.containsKey(path)) {
            return true;
        }
        int effectId = (int) mEffectIds.get(path);
        enableEffect(path, false);
        int iRet = OrangeFilter.destroyEffect(mOFContext, effectId);
        if (OrangeFilter.OF_Result_Success != iRet) {
            LogMsg = "卸载特效包失败，失败信息id " + iRet;
            Log.e(TAG, LogMsg);
            return false;
        }
        mEffectIds.remove(path);
        return true;
    }

    private static boolean isNeedCheckSerialNumberOnline(Context activity, String licensePath) {
        File license = new File(licensePath);
        if (license.exists() && license.isFile()) {
            String result = "";
            try {
                FileInputStream fis = new FileInputStream(licensePath);
                int length = fis.available();
                length = length > 256 ? 256 : length; //读取头
                byte[] buffer = new byte[length];
                fis.read(buffer);
                fis.close();
                result = new String(buffer, "UTF-8");
                //"Range":"0000-00-00 - 0000-00-00","Type":
                int bIndex = result.indexOf("\"Range\":\"");
                int eIndex = result.indexOf("\",\"Type\":");
                if (bIndex < 0 || eIndex < bIndex) {
                    return true;
                }
                result = result.substring(bIndex + "\"Range\":\"0000-00-00 - ".length(), eIndex);
                Log.i(TAG, "authorization :" + result);
                if (result.length() != "0000-00-00".length()) {
                    return true;
                }
                Date authorDate = Date.valueOf(result);
                Date now = new Date(System.currentTimeMillis() + 7 * 24 * 3600 * 1000);
                return authorDate.before(now);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return true;
    }

    private static void copyLicenseFromAssets(Context activity, String licenseName,
                                              String licensePath) {
        Log.i(TAG, "从资源包拷贝授权文件到存储...");
        FileOutputStream fos = null;
        InputStream in = null;
        try {
            in = activity.getResources().getAssets().open(licenseName);
            if (null != in) {
                int length = in.available();
                byte[] buffer = new byte[length];
                in.read(buffer);
                fos = new FileOutputStream(licensePath);
                fos.write(buffer);
                fos.flush();
            }
        } catch (IOException e) {
            e.printStackTrace();
            LogMsg = "本地授权文件不存在 : " + licenseName;
            Log.e(TAG, LogMsg);
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    //创建美颜SDK，后续的所有操作都需要Context id作为输入参数。  resDir资源路径，建议设置null
    public static boolean createContext(Context activity, String ofSerialNumber,
                                        int venusType,
                                        String resDir /*建议传null*/) {
        return createContext(activity, ofSerialNumber, venusType, resDir, false);
    }
    //创建美颜SDK，后续的所有操作都需要Context id作为输入参数。  resDir资源路径，建议设置null
    //bOnline set 'ture' if download effects form server. default false;
    public static boolean createContext(Context activity, String ofSerialNumber,
                                        int venusType,
                                        String resDir, /*建议传null*/
                                        boolean bDownloadEffectsOnline/**/) {
        Log.i(TAG, "lazy init： CreateContext");
        //约定的特效包存放路径
        if (null == resDir) {
            resDir = activity.getFilesDir().getPath();
        }
        mEffectPackagePaths = resDir + "/orangefilter/effects/";
        mVenusModlePath = resDir + "/orangefilter/models/venus_models";
        //设置授权文件的缓存路径，用于授权成功后本地授权缓存。
        final String ofLicenseName = "of_offline_license.license";
        String ofLicensePath = resDir + "/" + ofLicenseName;
        //版本变化清理资源
        //check sdk version ,if not equal current version delete data cache.
        checkResourcesValid(activity, bDownloadEffectsOnline ?  null : mEffectPackagePaths,
                mVenusModlePath + "/", ofLicensePath);
        //尝试拷贝资源包中的license
        File license = new File(ofLicensePath);
        if (!(license.exists() && license.isFile())) {
            copyLicenseFromAssets(activity, ofLicenseName, ofLicensePath);
        }
        //检查授权序列号是否有效， ！！必须调用！！这是对SDK合法授权的检测，否则无法启用SDK。
        boolean onlineCheck = isNeedCheckSerialNumberOnline(activity, ofLicensePath);
        Log.i(TAG, "online check serial number first " + onlineCheck);
        int ret = OrangeFilter
                .checkSerialNumberOnline(activity, ofSerialNumber, ofLicensePath, onlineCheck);
        if (ret != OrangeFilter.OF_Result_Success) {
            //授权检测失败，请检查授权序列号是否输入正确，授权时间是否在有效期内。（咨询商务相关人士）
            LogMsg = "OrangeFilter license invalid. ret = [" + ret + "]";
            Log.e(TAG, LogMsg);
            return false;
        }
        //创建美颜渲染引擎。iContext是SDK的环境句柄，在整个SDK生命周期中存在，且后续的操作均依赖这个id。
        int iContext = OrangeFilter.createContext();
        if (0 == iContext) {
            //失败通常由授权信息导致。请检查授权。
            LogMsg = "OrangeFilter SDK 初始化失败，请检查授权是否过期。";
            Log.e(TAG, LogMsg);
            return false;
        }
        //初始化必需的变量。
        mStartTime = System.currentTimeMillis();
        mFrameData = new OrangeFilter.OF_FrameData(); //用于请求AI算法相关的数据参数
        mInputs = new OrangeFilter.OF_Texture[1]; //输入纹理队列，目前默认1张纹理
        mOutputs = new OrangeFilter.OF_Texture[1]; //输出纹理队列，目前默认1张纹理
        mInputs[0] = new OrangeFilter.OF_Texture(); //申请内存
        mOutputs[0] = new OrangeFilter.OF_Texture(); //申请内存

        //解压effects包
        File effectDir = new File(mEffectPackagePaths);
        if (!(effectDir.isDirectory() && effectDir.exists())) {
            effectDir.mkdirs();
            OrangeFilter.extractAssetsDir(activity.getAssets(), "effects", mEffectPackagePaths);
        }

        //是否需要人脸关键点检测功能，通常高级美颜依赖这个功能。比如人脸贴纸。
        if (0 != (venusType & VENUS_FACE)) {
            File faceDir = new File(mVenusModlePath + "/face");
            if (!(faceDir.isDirectory() && faceDir.exists())) {
                faceDir.mkdirs();
                OrangeFilter.extractAssetsDir(
                        activity.getAssets(),
                        "models/venus_models/face",
                        mVenusModlePath + "/face"
                );
            }
            final String[] faceModelPath = {
                    mVenusModlePath + "/face/Face_Key_Points_0.vnmodel",
                    mVenusModlePath + "/face/Face_Key_Points_1.vnmodel",
                    mVenusModlePath + "/face/Face_Key_Points_2.vnmodel",
                    mVenusModlePath + "/face/Face_Key_Points_3.vnmodel",
                    mVenusModlePath + "/face/Face_Key_Points_4.vnmodel",
                    mVenusModlePath + "/face/Face_Key_Points_5.vnmodel", //form 1.8.54 venus
            };
            ret = OrangeFilter.setVenusModels(
                    iContext,
                    OrangeFilter.OF_VenusDetectionType_Face,
                    faceModelPath
            );
            if (OrangeFilter.OF_Result_Success == ret) {
                Log.i(TAG, "加载人脸关键点识别数据成功");
            } else {
                LogMsg = "加载人脸关键点识别数据失败！！！ ret = [" + ret + "]";
                Log.e(TAG, LogMsg);
            }
        }
        //是否需要检测手势，这个功能可以启用手势表情相关动作，识别各种手势动作的前提。
        if (0 != (venusType & VENUS_GESTURE)) {
            File gestureDir = new File(mVenusModlePath + "/gesture");
            if (!(gestureDir.isDirectory() && gestureDir.exists())) {
                gestureDir.mkdirs();
                OrangeFilter.extractAssetsDir(
                        activity.getAssets(),
                        "models/venus_models/gesture",
                        mVenusModlePath + "/gesture"
                );
            }
            final String[] gestureModelPath = {
                    mVenusModlePath + "/gesture/Gesture_Recognition_0.vnmodel",
                    mVenusModlePath + "/gesture/Gesture_Recognition_1.vnmodel",
                    mVenusModlePath + "/gesture/Gesture_Recognition_2.vnmodel",
            };
            ret = OrangeFilter.setVenusModels(
                    iContext,
                    OrangeFilter.OF_VenusDetectionType_Gesture,
                    gestureModelPath
            );
            if (OrangeFilter.OF_Result_Success == ret) {
                Log.i(TAG, "加载手势识别数据成功");
            } else {
                LogMsg = "加载手势识别数据失败！！！ ret = [" + ret + "]";
                Log.e(TAG, LogMsg);
            }
        }
        //是否开启背景分割功能。人物抠图换背景图等相关的特效依赖这个功能。
        if (0 != (venusType & VENUS_SEGMENT)) {
            File segmentDir = new File(mVenusModlePath + "/segment");
            if (!(segmentDir.isDirectory() && segmentDir.exists())) {
                segmentDir.mkdirs();
                OrangeFilter.extractAssetsDir(
                        activity.getAssets(),
                        "models/venus_models/segment",
                        mVenusModlePath + "/segment"
                );
            }
            final String[] bodySegmentModelPath = {
                    mVenusModlePath + "/segment/Background_Segmentation_0.dat",
                    mVenusModlePath + "/segment/Background_Segmentation_1.dat",
            };
            ret = OrangeFilter.setVenusModels(
                    iContext,
                    OrangeFilter.OF_VenusDetectionType_BodySegment,
                    bodySegmentModelPath
            );
            if (OrangeFilter.OF_Result_Success == ret) {
                Log.i(TAG, "加载背景分割数据成功");
            } else {
                LogMsg = "加载背景分割数据失败！！！ ret = [" + ret + "]";
                Log.e(TAG, LogMsg);
            }
        }
        mOFContext = iContext;
        return true;
    }

    //Destroy the OrangeFilter SDK
    public static boolean destroyContext() {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用DestroyContext接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        mEffectJobs.removeAllElements();
        for (Map.Entry<String, Integer> entry : mEffectIds.entrySet()) {
            OrangeFilter.destroyEffect(mOFContext, entry.getValue());
        }
        mEffectIds.clear();
        if (OrangeFilter.OF_Result_Success != OrangeFilter.destroyContext(mOFContext)) {
            return false;
        }
        mOFContext = 0;
        return true;
    }

    //Query engine is available
    public static boolean isContextValid() {
        return 0 != mOFContext;
    }

    //config effect enums references
    public static void configEffectEnums(EffectType et, String effectFilePath) {
        Log.i(TAG,
                "customer config effect enums : " + et.toString() + " to effect path : " + effectFilePath);
        mEffectConfigs[et.ordinal()] = effectFilePath;
    }

    //Turn a effect on or off
    public static boolean enableEffect(EffectType et, boolean bEnable) {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用enableOrangeEffect接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        if (null == et) {
            LogMsg = "参数 et不能为null";
            Log.e(TAG, LogMsg);
            return false;
        }
        String path = null != mEffectConfigs[et.ordinal()] ? mEffectConfigs[et.ordinal()] :
                mEffectPackagePaths + mEffectDefaults[et.ordinal()];
        return enableEffect(path, bEnable);
    }

    //Release a effect
    public static boolean releaseEffect(EffectType et) {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用releaseOrangeEffect接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        if (null == et) {
            LogMsg = "参数 et不能为null";
            Log.e(TAG, LogMsg);
            return false;
        }
        String path = null != mEffectConfigs[et.ordinal()] ? mEffectConfigs[et.ordinal()] :
                mEffectPackagePaths + mEffectDefaults[et.ordinal()];
        return releaseEffect(path);
    }

    //Turn on or off a gesture effect
    public static boolean enableGesture(String path, boolean bEnable) {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用enableOrangeGesture接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        return enableEffect(path, bEnable);
    }

    //Release a gesture effects package
    public static boolean releaseGesture(String path) {
        return releaseEffect(path);
    }

    //Turn on or off a sticker effect
    public static boolean enableSticker(String path, boolean bEnable) {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用enableOrangeSticker接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        return enableEffect(path, bEnable);
    }

    //Release a sticker effects package
    public static boolean releaseSticker(String path) {
        return releaseEffect(path);
    }

    //Get a special effect parameter
    public static int getEffectParam(EffectParamType ep) {
        if (0 == mOFContext) {
            return 0;
        }
        int filterId = getFilterId(ep);
        if (filterId > 0) {
            OrangeFilter.OF_Paramf param =
                    (OrangeFilter.OF_Paramf) OrangeFilter.getFilterParamData(mOFContext, filterId,
                            getFilterParamName(ep));
            if (null != param && param.maxVal - param.minVal > 0) {
                return (int) (param.val / (param.maxVal - param.minVal) * 100);
            }
        }
        return 0;
    }

    //Get a special effect parameter range and default value
    public static boolean getEffectParamDetail(EffectParamType ep, EffectParam effectPram) {
        if (0 == mOFContext) {
            return false;
        }
        int filterId = getFilterId(ep);
        if (filterId > 0) {
            OrangeFilter.OF_Paramf param =
                    (OrangeFilter.OF_Paramf) OrangeFilter
                            .getFilterParamData(mOFContext, filterId, getFilterParamName(ep));
            if (null == param) {
                return false;
            }
            float fArrange = param.maxVal - param.minVal;
            effectPram.curVal = (int) (param.val / fArrange * 100);
            effectPram.minVal = (int) (param.minVal / fArrange * 100);
            effectPram.maxVal = (int) (param.maxVal / fArrange * 100);
            effectPram.defVal = (int) (param.defVal / fArrange * 100);
            return true;
        }
        return false;
    }

    //get custom filter param, param default name is 'Intensity'
    public static int getCustomFilterParam(String filterPath) {
        if (0 == mOFContext) {
            return 0;
        }
        if (mEffectIds.containsKey(filterPath)) {
            int effectId = mEffectIds.get(filterPath);
            if (effectId > 0) {
                OrangeFilter.OF_Paramf param =
                        (OrangeFilter.OF_Paramf) OrangeFilter.getFilterParamData(mOFContext,
                                getFilterIdByEffectId(effectId, "Intensity"),
                                "Intensity");
                if (null != param && param.maxVal - param.minVal > 0) {
                    return (int) (param.val / (param.maxVal - param.minVal) * 100);
                }
            }
        }
        LogMsg = "OrangeFilter.getCustomFilterParam获取数据出错， 没有找到该滤镜 " + filterPath;
        Log.e(TAG, LogMsg);
        return 0;
    }

    //set custom filter param, param default name is 'Intensity'
    public static boolean setCustomFilterParam(String filterPath, int curVal) {
        if (0 == mOFContext) {
            return false;
        }
        if (mEffectIds.containsKey(filterPath)) {
            int effectId = mEffectIds.get(filterPath);
            if (effectId > 0) {
                OrangeFilter.OF_Paramf param =
                        (OrangeFilter.OF_Paramf) OrangeFilter
                                .getFilterParamData(mOFContext, getFilterIdByEffectId(effectId,
                                        "Intensity"), "Intensity");
                if (null == param) {
                    return false;
                }
                float val = curVal / 100.0f;
                if (val > param.maxVal) {
                    param.val = param.maxVal;
                } else if (val < param.minVal) {
                    param.val = param.minVal;
                } else {
                    param.val = val * (param.maxVal - param.minVal);
                }
                OrangeFilter.setFilterParamData(mOFContext, getFilterIdByEffectId(effectId,
                        "Intensity"), "Intensity", param);
                return true;
            }
        }
        return false;
    }

    //Set the current value of a special effect parameter
    public static boolean setEffectParam(EffectParamType ep, int curVal) {
        if (0 == mOFContext) {
            return false;
        }
        int filterId = getFilterId(ep);
        if (filterId > 0) {
            String paramName = getFilterParamName(ep);
            OrangeFilter.OF_Paramf param =
                    (OrangeFilter.OF_Paramf) OrangeFilter
                            .getFilterParamData(mOFContext, filterId, paramName);
            if (null == param) {
                return false;
            }
            float val = curVal / 100.0f;
            if (val > param.maxVal) {
                param.val = param.maxVal;
            } else if (val < param.minVal) {
                param.val = param.minVal;
            } else {
                param.val = val * (param.maxVal - param.minVal);
            }
            OrangeFilter.setFilterParamData(mOFContext, filterId, paramName, param);
            return true;
        }
        return false;
    }

    //This function is! ! ! Every frame! ! ! Both need to be refreshed, because you need to know the current rendering texture information at all times.
    //Users can integrate these codes into their own projects according to their own situation.
    // These parameters will ultimately determine the success of advanced effects such as face recognition.
    public static boolean updateFrameParams(GLTexture textureIn, //Input texture
                                            GLTexture textureOut, //Output texture
                                            ImageInfo image) { //Images captured by the camera
        if (0 == mOFContext) {
            return false;
        }
        mInputs[0].width = textureIn.mWidth;
        mInputs[0].height = textureIn.mHeight;
        mInputs[0].format = textureIn.mFormat;
        mInputs[0].target = textureIn.mTarget;
        mInputs[0].textureID = textureIn.mTextureId;

        mOutputs[0].width = textureOut.mWidth;
        mOutputs[0].height = textureOut.mHeight;
        mOutputs[0].format = textureOut.mFormat;
        mOutputs[0].target = textureOut.mTarget;
        mOutputs[0].textureID = textureOut.mTextureId;

        boolean useCustomHarsLib = false;

        if (!useCustomHarsLib) {
            if (mFrameData.faceFrameDataArr == null) {
                mFrameData.faceFrameDataArr = new OrangeFilter.OF_FaceFrameData[0];
            }
        }
        mFrameData.width = image.height; //It’s not wrong here, it’s written anyway
        mFrameData.height = image.width; //It’s not wrong here, it’s written anyway
        mFrameData.widthStep = mFrameData.width;
        mFrameData.timestamp = image.timestamp > 0 ? image.timestamp :
                (System.currentTimeMillis() - mStartTime) / 1000.0f;
        mFrameData.isUseCustomHarsLib = useCustomHarsLib;
        mFrameData.trackOn = false;
        mFrameData.curNode = 0;
        mFrameData.pickOn = false;
        mFrameData.pickResult = false;

        //! ! ! The following information is very important and will determine whether the advanced beauty effect is effective,
        // and the specific situation will be modified and adjusted according to the actual input of the user. ! ! !
        //The android camera defaults to OF_PixelFormat_NV21 format. If you use the three-party SDK or enter the picture yourself,
        // the user needs to confirm the specific format according to the actual input image.data. For example, OF_PixelFormat_RGB32 is also possible.
        //If the format is entered incorrectly, advanced effects such as faces and gestures cannot be correctly recognized.
        mFrameData.imageDeviceType = image.deviceType; //The default camera captures images, common usage
        if (1 == mFrameData.imageDeviceType) { //This mode is more flexible and requires users to set more parameters by themselves,
            // which can solve any image input situation, which is necessary for three-party SDK access.
            //This parameter is supported from version 1.3.0. This mode can solve all recognition problems,
            // but the setting parameters are left to the user to decide. Will be more flexible.
            //Set the direction of the face rendering result
            mFrameData.facePointOrientation = image.facePointDir; //See user settings
            //Set image recognition direction
            mFrameData.imageDir = image.dir; //You can use the gyroscope direction (turn left 90 degrees 0, stand upright 1, turn right 90 degrees 2, upside down 3)
            //Set whether the rendering result is mirrored
            mFrameData.frontCamera = image.frontCamera; //Adjust whether the recognition result needs to be flipped horizontally (mirror)
            //Set user-defined image data
            mFrameData.imageData = image.data; //Image data buff
            //Custom image data format
            mFrameData.format = image.format; //The default capture format of the android camera is not the camera input.
        } else {
            //In this default camera capture image mode, the SDK has already done a lot of processing, simple and easy to use.
            mFrameData.orientation = image.orientation; //Image direction (-90, 0, 90, 270)
            mFrameData.imageDir = image.dir; //You can use the gyroscope direction (turn left 90 degrees 0, stand upright 1, turn right 90 degrees 2, upside down 3)
            mFrameData.frontCamera = image.frontCamera; //Adjust whether the recognition result needs to be flipped horizontally (mirror)
            mFrameData.imageData = image.data; //Image data buff
            mFrameData.format = image.format; //The default capture format of the android camera is not the camera input.
        }

        //Request recognition image from SDK
        int ret = OrangeFilter.prepareFrameData(mOFContext, mFrameData);
        if (OrangeFilter.OF_Result_Success != ret) {
            LogMsg = "OrangeFilter.prepareFrameData准备数据出错，返回 " + ret;
            Log.e(TAG, LogMsg);
            return false;
        }
        //Prepare to render the special effects package
        int effectCount = mEffectJobs.size();
        if (null == mEffectJobIdsCache || mEffectJobIdsCache.length != effectCount) {
            mEffectJobIdsCache = new int[effectCount];
            mEffectJobResultsCache = new int[effectCount];
        }
        int count = mEffectJobs.size();
        int i = 0, j = 0;
        while (i < count) {
            mEffectJobIdsCache[j++] = mEffectJobs.get(i++);
        }
        //Start rendering image
        ret = OrangeFilter.applyFrameBatch(mOFContext, mEffectJobIdsCache, mInputs, mOutputs,
                mEffectJobResultsCache);

        if (OrangeFilter.OF_Result_Success != ret) {
            LogMsg = "OrangeFilter.applyFrameBatch出错，返回 " + ret;
            Log.e(TAG, LogMsg);
            return false;
        }
        //search result
        count = mEffectJobResultsCache.length;
        for (i = 0; i < count; ++i) {
            if (mEffectJobResultsCache[i] > 0) {
                LogMsg = "特效沒有生效 id " + mEffectJobIdsCache[i];
                Log.e(TAG, LogMsg);
            }
        }
        return true;
    }

    //The result of each frame of the query sticker can be called after the updateFrameParams call ends.
    //[in]stickerPaths The sticker special effect package to be queried. [out] The return value of the corresponding special effect package of the stickerResults query.
    public static boolean checkStickerResult(String[] stickerPaths, int[] stickerResults) {
        if (0 == mOFContext) {
            return false;
        }
        if (null == stickerPaths || null == stickerResults) {
            LogMsg = "checkEffectResult 参数不能为空";
            Log.e(TAG, LogMsg);
            return false;
        }

        if (stickerPaths.length > stickerResults.length) {
            LogMsg = "checkEffectResult 参数2数组长度不能小于参数1数组长度";
            Log.e(TAG, LogMsg);
            return false;
        }

        if (null != mEffectJobIdsCache) {
            String stickerName;
            int stickerId = 0;
            int ret = 1;
            for (int i = 0; i < stickerPaths.length; ++i) {
                stickerName = stickerPaths[i];
                ret = OrangeFilter.OF_Result_InvalidEffect;
                if (null != stickerName && mEffectIds.containsKey(stickerName)) {
                    stickerId = mEffectIds.get(stickerName);
                    for (int j = 0; j < mEffectJobIdsCache.length; ++j) {
                        if (stickerId == mEffectJobIdsCache[j]) {
                            ret = mEffectJobResultsCache[j];
                            break;
                        }
                    }
                }
                stickerResults[i] = ret;
            }
        }
        return true;
    }

    //Set the log level, return true if successful
    //The following is the log mask, users can use a combination of ‘or’
    /*
        OrangeFilter.OF_LogLevel_Info; //Info
        OrangeFilter.OF_LogLevel_Warn; //Warn
        OrangeFilter.OF_LogLevel_Error; //Error
        OrangeFilter.OF_LogLevel_Verbose; //Verbose
    * */
    //You can pass 0 to close all logs. It is not recommended and cannot locate errors. The SDK outputs normal message + warning + error by default
    public static boolean setLogLevel(int logLevel) {
        if (0 == mOFContext) {
            return false;
        }
        return OrangeFilter.OF_Result_Success == OrangeFilter.setLogLevel(logLevel);
    }

    //To redirect log output, users can use logCallBackFunc in the listener to redirect the required log output.
    //For example, some third-party SDKs will intercept the default log of android. At this time, the user may redirect the log of OrangeFilter.
    //Users need to implement logCallBackFunc in OF_LogListener interface, other interfaces are downward compatible interfaces and can be ignored.
    public static boolean setLogCallback(OrangeFilter.OF_LogListener logListener) {
        if (0 == mOFContext) {
            return false;
        }
        if (null != logListener) {
            return OrangeFilter.OF_Result_Success == OrangeFilter.setLogCallback(logListener);
        }
        return false;
    }

    private static void checkResourcesValid(Context activity, String effectPath, String venusPath,
                                            String ofLicensePath) {
        SharedPreferences pf = getDefaultSharedPreferences(activity);
        final String configKey = "setup_ofsdk_version";
        final String buildVersion = mSDK_VERSION;
        Log.i(TAG, "to delete effects and venus directory ....");
        if (!pf.getString(configKey, "-1").equals(buildVersion)) {
            //delete license
            if (null != ofLicensePath) {
                File file = new File(ofLicensePath);
                if (file.isFile()) {
                    file.delete();
                }
            }
            //delete dir
            if (null != effectPath) {
                File dir = new File(effectPath);
                deleteDirWithFile(dir);
            }
            if (null != venusPath) {
                File dir = new File(venusPath);
                deleteDirWithFile(dir);
            }
            SharedPreferences.Editor pfe = pf.edit();
            pfe.putString(configKey, buildVersion);
            pfe.commit();
            Log.i(TAG, "delete effects and venus directory！");
        }
    }

    private static void deleteDirWithFile(File dir) {
        if (dir == null || !dir.exists() || !dir.isDirectory()) {
            return;
        }
        for (File file : dir.listFiles()) {
            if (file.isFile()) {
                //Log.i(TAG, file.getName());
                file.delete();
            } else if (file.isDirectory()) {
                deleteDirWithFile(file);
            }
        }
        dir.delete();
    }
}
