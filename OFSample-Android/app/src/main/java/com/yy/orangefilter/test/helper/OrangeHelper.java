package com.yy.orangefilter.test.helper;

import android.content.Context;
import android.content.SharedPreferences;
import android.opengl.GLES20;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import com.orangefilter.OrangeFilter;

import static android.preference.PreferenceManager.getDefaultSharedPreferences;

// 注意：这个类仅仅是对OrangeFilter SDK API使用的一次封装，它是对SDK
// API的二次封装，屏蔽了sdk的使用细节。尽量满足用户简化SDK使用的需求。
// 该类建议直接引入用户项目使用。除非必要，不建议修改，因为后续开发方会随着sdk版本升级对该类进行增量更新，以优化用户的使用体验。
// 如果用户有特殊的使用需求，依然可用参考该类的实现，使用OrangeFilterSDK的原生接口。

public class OrangeHelper {
    final private static String TAG = "OrangeHelper";
    final private static String mSDK_VERSION = "1.4.7";
    final public static int VENUS_NONE = 0; //不启用AI计算，无法使用高级美颜，贴纸，手势识别，背景分割
    final public static int VENUS_FACE = 1; //启用高级美颜和贴纸
    final public static int VENUS_GESTURE = 2; //启用手势识别，手势表情
    final public static int VENUS_SEGMENT = 4;  //启用背景分割抠图
    final public static int VENUS_ALL = 7;  //启用所有AI功能。

    final public static int LG_Info = OrangeFilter.OF_LogLevel_Info; //普通消息
    final public static int LG_Warn = OrangeFilter.OF_LogLevel_Warn; //警告
    final public static int LG_Error = OrangeFilter.OF_LogLevel_Error; //错误
    final public static int LG_Debug = OrangeFilter.OF_LogLevel_Debug; //错误
    final public static int LG_Verbose = OrangeFilter.OF_LogLevel_Verbose; //所有日志

    public static String LogMsg = ""; //记录最近的一次错误信息。
    private static Map<String, Integer> mEffectIds = new HashMap(); //内存已经加载的特效包
    private static Vector<Integer> mEffectJobs = new Vector<>(); //生效的特效包

    private static String mEffectPackagePaths = ""; //特效包存放路径
    private static String mVenusModlePath = ""; //VENUS AI计算依赖的模型数据文件存放路径。
    private static int mOFContext = 0; //引擎句柄
    private static long mStartTime;
    private static OrangeFilter.OF_FrameData mFrameData;
    private static OrangeFilter.OF_Texture[] mInputs;
    private static OrangeFilter.OF_Texture[] mOutputs;
    private static int[] mEffectJobIdsCache;
    private static int[] mEffectJobResultsCache;

    //传递Opengl纹理参数的结构，该结构传递的纹理用于渲染。
    public static class GLTexture {
        public int mTextureId; //OpenGL texture id
        public int mWidth; //OpenGL texture width.
        public int mHeight; //OpenGL texture height.
        private int mTarget = GLES20.GL_TEXTURE_2D; //OpenGL texture target, e.g. GL_TEXTURE_2D.
        private int mFormat = GLES20.GL_RGBA; //OpenGL texture format, e.g. GL_RGBA.
    }

    //传递识别图像的参数结构，该结构传递的图像用于人脸及动作识别。
    public static class ImageInfo {
        public int deviceType; //图像类别，默认为相机图像0，用户自定义传入的三方sdk图像为1.
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

    //特效参数结构体，用于查询参数
    public static class EffectParam {
        public int curVal; //当前值
        public int maxVal; //最大值
        public int minVal; //最小值
        public int defVal; //默认值
    }

    public enum EffectType {
        ET_BasicBeauty, //基础美颜
        ET_BasicBeautyType, //基础整形
        ET_SeniorBeautyType, //高级整形
        ET_FilterHoliday, //假日滤镜
        ET_FilterClear, //清晰滤镜
        ET_FilterWarm, //暖阳滤镜
        ET_FilterFresh, //清新滤镜
        ET_FilterTender, //粉嫩滤镜
        ET_BasicBeauty5, //基础美颜（清晰磨皮）

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

    final private static String[] mEffectDefaults = { //默认枚举支持的特效
            "beauty_basic.zip",
            "basicthinface.zip",
            "faceliftingopt.zip",
            "filter_holiday.zip",
            "filter_clear.zip",
            "filter_warm.zip",
            "filter_fresh.zip",
            "filter_tender.zip",
            "beauty5_basic.zip",//清晰磨皮
            //20款滤镜风格
            "lengse-adaier.zip",
            "lengse-danya.zip",
            "lengse-jiazhou.zip",
            "lengse-muojito.zip",
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

    public enum EffectParamType {
        EP_BasicBeautyIntensity, //基础美颜
        EP_BasicBeautyOpacity, //基礎磨皮强度
        EP_BasicBeautyIntensity5, //基础美颜（清晰）
        EP_BasicBeautyOpacity5, //基础美颜(清晰磨皮)
        EP_FilterHolidayIntensity, //假日滤镜强度
        EP_FilterClearIntensity, //清晰滤镜强度
        EP_FilterWarmIntensity, //暖阳滤镜强度
        EP_FilterFreshIntensity, //清新滤镜强度
        EP_FilterTenderIntensity, //粉嫩滤镜强度
        EP_BasicTypeIntensity, //基础整形强度
        EP_SeniorTypeThinFaceIntensity, //高级瘦脸
        EP_SeniorTypeSmallFaceIntensity, //高级小脸
        EP_SeniorTypeSquashedFaceIntensity, //高級瘦颧骨
        EP_SeniorTypeForeheadLiftingIntensity, //高级额高
        EP_SeniorTypeWideForeheadIntensity, //高级额宽
        EP_SeniorTypeBigSmallEyeIntensity, //高级大眼
        EP_SeniorTypeEyesOffsetIntensity, //高级眼距
        EP_SeniorTypeEyesRotationIntensity, //高级眼角
        EP_SeniorTypeThinNoseIntensity, //高级瘦鼻
        EP_SeniorTypeLongNoseIntensity, //高级长鼻
        EP_SeniorTypeThinNoseBridgeIntensity, //高级窄鼻梁
        EP_SeniorTypeThinmouthIntensity, //高级小嘴
        EP_SeniorTypeMovemouthIntensity, //高级嘴位
        EP_SeniorTypeChinLiftingIntensity, //高级下巴

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
            "Intensity", //basic beauty
            "Opacity",
            "Intensity",//清晰
            "Opacity",
            "Intensity", //filter
            "Intensity",
            "Intensity",
            "Intensity",
            "Intensity",
            "Intensity", //basic beauty type
            "ThinfaceIntensity", //senior beauty type
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
            case EP_BasicBeautyOpacity5:
            case EP_BasicBeautyIntensity5:
                et = EffectType.ET_BasicBeauty5;
                break;
            case EP_BasicBeautyIntensity:
            case EP_BasicBeautyOpacity: {
                et = EffectType.ET_BasicBeauty;
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
        String path = mEffectPackagePaths + mEffectDefaults[et.ordinal()];
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
        int effectId = getEffectId(ep);
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
            String tarParamName = getFilterParamName(ep);
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
                    if (paramName.equals(tarParamName)) {
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
            LogMsg = "该变量所属的特效包尚未加载。" + ep.toString();
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

        //check sdk version ,if not equal current version delete data cache.
        checkResourcesValid(activity, bDownloadEffectsOnline ?  null : mEffectPackagePaths, mVenusModlePath + "/");

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

    //销毁美颜SDK
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

    //查询引擎是否可用
    public static boolean isContextValid() {
        return 0 != mOFContext;
    }

    //开启或关闭某特效
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
        String path = mEffectPackagePaths + mEffectDefaults[et.ordinal()];
        return enableEffect(path, bEnable);
    }

    //释放某特效
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
        String path = mEffectPackagePaths + mEffectDefaults[et.ordinal()];
        return releaseEffect(path);
    }

    //开启或关闭某手势特效
    public static boolean enableGesture(String path, boolean bEnable) {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用enableOrangeGesture接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        return enableEffect(path, bEnable);
    }

    //释放某手势特效包
    public static boolean releaseGesture(String path) {
        return releaseEffect(path);
    }

    //开启或关闭某贴纸特效
    public static boolean enableSticker(String path, boolean bEnable) {
        if (0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用enableOrangeSticker接口。";
            Log.e(TAG, LogMsg);
            return false;
        }
        return enableEffect(path, bEnable);
    }

    //释放某贴纸特效包
    public static boolean releaseSticker(String path) {
        return releaseEffect(path);
    }

    //获取某特效参数
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

    //获取某特效参数范围和默认值
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
            effectPram.curVal = (int) (Math.round(param.val / fArrange * 100));
            effectPram.minVal = (int) (param.minVal / fArrange * 100);
            effectPram.maxVal = (int) (param.maxVal / fArrange * 100);
            effectPram.defVal = (int) (param.defVal / fArrange * 100);
            return true;
        }
        return false;
    }

    //设置某特效参数当前值
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

    //这个函数是！！！每帧！！！都需要刷新的，因为需要时刻知晓当前渲染纹理的信息。
    //用户根据自己情况可以把这些代码集成到自己的项目中，这些参数最终会决定人脸识别等高级效果是否成功。
    public static boolean updateFrameParams(GLTexture textureIn, //输入纹理
                                            GLTexture textureOut, //输出纹理
                                            ImageInfo image) { //相机采集的图像
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
        mFrameData.width = image.height; //这里没写错，就是反正写的
        mFrameData.height = image.width; //这里没写错，就是反正写的
        mFrameData.widthStep = mFrameData.width;
        mFrameData.timestamp = image.timestamp > 0 ? image.timestamp :
                (System.currentTimeMillis() - mStartTime) / 1000.0f;
        mFrameData.isUseCustomHarsLib = useCustomHarsLib;
        mFrameData.trackOn = false;
        mFrameData.curNode = 0;
        mFrameData.pickOn = false;
        mFrameData.pickResult = false;

        //！！！下面信息很重要，会决定高级美颜效果是否生效，而且具体情况会根据用户的实际输入情况作出修改调整。！！！
        //android摄像头默认OF_PixelFormat_NV21
        // 格式，如果使用三方sdk，或者自己输入图片，需要用户根据实际输入image.data确认具体的格式。比如OF_PixelFormat_RGB32也是有可能的。
        //格式输入错误，会导致无法正确识别人脸，手势等高级效果。
        mFrameData.imageDeviceType = image.deviceType; //默认攝像頭采集圖像，常见用法
        if (1 == mFrameData.imageDeviceType) { //这种模式下更加灵活，需要用户自己设置更多的参数，可以解决任何的图像输入情况，三方sdk接入的必备。
            //这种参数从1.3.0版本开始支持。这种模式可以解决所有识别问题，但是设置参数交给用户来决定。会更灵活。
            //设置人脸渲染结果的方向
            mFrameData.facePointOrientation = image.facePointDir; //具体看用户设置
            //设置图像识别方向
            mFrameData.imageDir = image.dir; //可以用陀螺仪方向（左转90度0， 竖着正立1， 右转90度2， 倒立竖着3）
            //设置渲染结果是否镜像
            mFrameData.frontCamera = image.frontCamera; //调整识别结果是否需要水平翻转（镜像）
            //设置用户自定义图像数据
            mFrameData.imageData = image.data; //图像数据buff
            //自定义图像数据格式
            mFrameData.format = image.format; //android摄像头默认采集格式，不是摄像头输入的要注意。
        } else {
            //这种默认摄像头采集图像模式下，sdk已经做了很多情况的处理，简单易用。
            mFrameData.orientation = image.orientation; //android摄像头硬件方向，从摄像头配置里获取（90， 270）
            mFrameData.imageDir = image.dir; //可以用陀螺仪方向（左转90度0， 竖着正立1， 右转90度2， 倒立竖着3）
            mFrameData.frontCamera = image.frontCamera; //调整识别结果是否需要水平翻转（镜像）
            mFrameData.imageData = image.data; //图像数据buff
            mFrameData.format = image.format; //android摄像头默认采集格式，不是摄像头输入的要注意。
        }

        //向SDK请求识别图像
        int ret = OrangeFilter.prepareFrameData(mOFContext, mFrameData);
        if (OrangeFilter.OF_Result_Success != ret) {
            LogMsg = "OrangeFilter.prepareFrameData准备数据出错，返回 " + ret;
            Log.e(TAG, LogMsg);
            return false;
        }
        //准备渲染特效包
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
        //开始渲染图像
        ret = OrangeFilter.applyFrameBatch(mOFContext, mEffectJobIdsCache, mInputs, mOutputs,
                mEffectJobResultsCache);

        if (OrangeFilter.OF_Result_Success != ret) {
            LogMsg = "OrangeFilter.applyFrameBatch出错，返回 " + ret;
            Log.e(TAG, LogMsg);
            return false;
        }
        //查询结果
        count = mEffectJobResultsCache.length;
        for (i = 0; i < count; ++i) {
            if (mEffectJobResultsCache[i] > 0) {
                LogMsg = "特效沒有生效 id " + mEffectJobIdsCache[i];
                Log.e(TAG, LogMsg);
            }
        }
        return true;
    }

    //查询贴纸的每一帧返回结果，可以在updateFrameParams调用结束后调用。
    //[in]stickerPaths 需要查询的贴纸特效包。  [out]stickerResults查询的对应特效包的返回值。
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

    //設置日志級別,成功返回true
    //以下为日志掩码，用户可以自己‘或’组合使用
    /*
        OrangeFilter.OF_LogLevel_Info; //普通消息
        OrangeFilter.OF_LogLevel_Warn; //警告
        OrangeFilter.OF_LogLevel_Error; //错误
        OrangeFilter.OF_LogLevel_Verbose; //所有日志
    * */
    //可以传递0关闭所有日志，不建议使用，无法定位错误。SDK默认输出 普通消息+警告+错误
    public static boolean setLogLevel(int logLevel) {
        if (0 == mOFContext) {
            return false;
        }
        return OrangeFilter.OF_Result_Success == OrangeFilter.setLogLevel(logLevel);
    }

    //重定向日志输出，用户可以使用listener里面的 logCallBackFunc 将所需日志重定向输出。
    //比如某些三方的sdk会截获android的默认日志，这个时候用户可能会重定向OrangeFilter的日志。
    //用户需要实现OF_LogListener接口里面的logCallBackFunc， 其他接口为向下兼容接口，可以忽略。
    public static boolean setLogCallback(OrangeFilter.OF_LogListener logListener) {
        if (0 == mOFContext) {
            return false;
        }
        if (null != logListener) {
            return OrangeFilter.OF_Result_Success == OrangeFilter.setLogCallback(logListener);
        }
        return false;
    }

    private static void checkResourcesValid(Context activity, String effectPath, String venusPath) {
        SharedPreferences pf = getDefaultSharedPreferences(activity);
        final String configKey = "setup_ofsdk_version";
        final String buildVersion = mSDK_VERSION;
        Log.i(TAG, "to delete effects and venus directory ....");
        if (!pf.getString(configKey, "-1").equals(buildVersion)) {
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
