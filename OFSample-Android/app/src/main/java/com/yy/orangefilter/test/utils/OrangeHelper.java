package com.yy.orangefilter.test.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

import com.orangefilter.OrangeFilter;
import com.yy.orangefilter.test.R;

import java.io.File;

import static android.preference.PreferenceManager.getDefaultSharedPreferences;

//注意：这个类仅仅是对OrangeFilter SDK API使用的一次封装，它不属于SDK的发布内容，该类实现的接口具有可借鉴，可复用的价值。
//用戶可以直接引用該类到项目中使用，也可以拷贝代码段使用，但是请了解这里代码接口所实现的功能意图。
//用户也可以不使用这里的代码，借鉴这里封装OrangeFilter SDK API的思路，在自己项目里直接调用SDK API。

public class OrangeHelper {
    final private static String TAG = "OrangeHelper";
    final public static int VENUS_NONE = 0; //不启用AI计算，无法使用高级美颜，贴纸，手势识别，背景分割
    final public static int VENUS_FACE = 1; //启用高级美颜和贴纸
    final public static int VENUS_GESTURE = 2; //启用手势识别，手势表情
    final public static int VENUS_SEGMENT = 4;  //启用背景分割抠图
    final public static int VENUS_ALL = 7;  //启用所有AI功能。
    public static String LogMsg = ""; //记录最近的一次错误信息。

    private static String mVenusModlePath = "";//VENUS AI计算依赖的模型数据文件存放路径。
    private static int mOFContext = 0; //引擎句柄
    private static long mStartTime;
    private static OrangeFilter.OF_FrameData mFrameData;
    private static OrangeFilter.OF_Texture[] mInputs;
    private static OrangeFilter.OF_Texture[] mOutputs;

    //创建美颜SDK，返回引擎Context id，后续的所有操作都需要Context id作为输入参数。
    public static int CreateContext(Context activity, //android activity
                                    String ofSerialNumber, //sdk 授权码，商务获取。
                                    int venusType){ // AI功能类型启用，见类成员变量VENUS_XXXX
        //使用默认的数据模型文件存放路径。后续的模型文件加载，依赖该路径。
        //请确保改路径下真实存在当前版本SDK一通发布的平台模型数据文件。
        //文件通常分为三类， Face， Gesture， Segment。
        //如果不启用相应的AI功能，完全可以删除掉对应的模型文件，以减少包体大小。
        //如果真实删除了模型文件，请不要加载相应的文件到SDK，否则会报错找不到文件。
        //如果不能理解上述信息，请不要删除任何文件，使用默认的VENUS_ALL来加载所有AI功能。
        mVenusModlePath = activity.getFilesDir().getPath() + "/orangefilter/models/venus_models";
        return CreateContext(activity, ofSerialNumber, venusType, mVenusModlePath);
    }
    //创建美颜SDK，返回引擎Context id，后续的所有操作都需要Context id作为输入参数。
    public static int CreateContext(Context activity, String ofSerialNumber, int venusType,
                                    String venusModelPath){
        Log.i(TAG, "lazy init： CreateContext");
        //检查资源,下面这个方法不建议放到这里，因为首次安装app和版本更新，会有解压资源资源的操作，建议用户酌情安置在CreateContext函数！！之前！！调用。
        //！！！！！这个函数是必须存在的，不在这里就请挪到这个函数前面。！！！！
        //如果你知道这个函数做了什么（准备可用AI模型资源文件），你也可以不使用这里提供的方法。请看代码实现。
        RefreshResources(activity, venusModelPath);
        //设置授权文件的缓存路径，用于授权成功后本地授权缓存。
        final String ofLicenseName = "of_offline_license.license";
        String ofLicensePath = activity.getFilesDir().getPath() + "/" + ofLicenseName;
        //检查授权序列号是否有效， ！！必须调用！！这是对SDK合法授权的检测，否则无法启用SDK。
        int ret = OrangeFilter
                .checkSerialNumber(activity, ofSerialNumber, ofLicensePath);
        if (ret != OrangeFilter.OF_Result_Success) {
            //授权检测失败，请检查授权序列号是否输入正确，授权时间是否在有效期内。（咨询商务相关人士）
            LogMsg = "OrangeFilter license invalid. ret = [" + ret + "]";
            Log.e(TAG, LogMsg);
            return 0;
        }
        //创建美颜渲染引擎。iContext是SDK的环境句柄，在整个SDK生命周期中存在，且后续的操作均依赖这个id。
        int iContext = OrangeFilter.createContext();
        if (0 == iContext) {
            //失败通常由授权信息导致。请检查授权。
            LogMsg = "OrangeFilter SDK 初始化失败，请检查授权是否过期。";
            Log.e(TAG, LogMsg);
            return 0;
        }
        //初始化必需的变量。
        mStartTime = System.currentTimeMillis();
        mFrameData = new OrangeFilter.OF_FrameData();//用于请求AI算法相关的数据参数
        mInputs = new OrangeFilter.OF_Texture[1];//输入纹理队列，目前默认1张纹理
        mOutputs = new OrangeFilter.OF_Texture[1];//输出纹理队列，目前默认1张纹理
        mInputs[0] = new OrangeFilter.OF_Texture();//申请内存
        mOutputs[0] = new OrangeFilter.OF_Texture();//申请内存

        //是否需要人脸关键点检测功能，通常高级美颜依赖这个功能。比如人脸贴纸。
        if(0 != (venusType & VENUS_FACE)){
            final String[] faceModelPath = {
                    venusModelPath + "/face/Face_Key_Points_0.vnmodel",
                    venusModelPath + "/face/Face_Key_Points_1.vnmodel",
                    venusModelPath + "/face/Face_Key_Points_2.vnmodel",
                    venusModelPath + "/face/Face_Key_Points_3.vnmodel",
                    venusModelPath + "/face/Face_Key_Points_4.vnmodel",
            };
            ret = OrangeFilter.setVenusModels(
                    iContext,
                    OrangeFilter.OF_VenusDetectionType_Face,
                    faceModelPath
            );
            if(OrangeFilter.OF_Result_Success == ret){
                Log.e(TAG, "加载人脸关键点识别数据成功");
            }else {
                LogMsg = "加载人脸关键点识别数据失败！！！ ret = [" + ret + "]";
                Log.e(TAG, LogMsg);
            }
        }
        //是否需要检测手势，这个功能可以启用手势表情相关动作，识别各种手势动作的前提。
        if(0 != (venusType & VENUS_GESTURE)){
            final String[] gestureModelPath = {
                    venusModelPath + "/gesture/Gesture_Recognition_0.vnmodel",
                    venusModelPath + "/gesture/Gesture_Recognition_1.vnmodel",
                    venusModelPath + "/gesture/Gesture_Recognition_2.vnmodel",
            };
            ret = OrangeFilter.setVenusModels(
                    iContext,
                    OrangeFilter.OF_VenusDetectionType_Gesture,
                    gestureModelPath
            );
            if(OrangeFilter.OF_Result_Success == ret){
                Log.e(TAG, "加载手势识别数据成功");
            }else {
                LogMsg = "加载手势识别数据失败！！！ ret = [" + ret + "]";
                Log.e(TAG, LogMsg);
            }
        }
        //是否开启背景分割功能。人物抠图换背景图等相关的特效依赖这个功能。
        if(0 != (venusType & VENUS_SEGMENT)){
            final String[] bodySegmentModelPath = {
                    venusModelPath + "/segment/Background_Segmentation_0.dat",
                    venusModelPath + "/segment/Background_Segmentation_1.dat",
            };
            ret = OrangeFilter.setVenusModels(
                    iContext,
                    OrangeFilter.OF_VenusDetectionType_BodySegment,
                    bodySegmentModelPath
            );
            if(OrangeFilter.OF_Result_Success == ret){
                Log.e(TAG, "加载背景分割数据成功");
            }else
            {
                LogMsg = "加载背景分割数据失败！！！ ret = [" + ret + "]";
                Log.e(TAG, LogMsg);
            }
        }
        mOFContext = iContext;
        return iContext;
    }
    //销毁美颜SDK， contextId是创建SDK时候的返回值。
    public static void DestroyContext(){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用DestroyContext接口。";
            Log.e(TAG, LogMsg);
        }
        OrangeFilter.destroyContext(mOFContext);
        mOFContext = 0;
    }

    public static boolean IsContextValid(){
        return 0 != mOFContext;
    }

    //加載特效包，返回特效包effctId（成功非0），后续对特效包滤镜的操作都依赖这个id
    public static int LoadEffectFromPackage(String effectPackagePath){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用LoadEffectFromPackage接口。";
            Log.e(TAG, LogMsg);
            return 0;
        }
        return OrangeFilter.createEffectFromPackage(mOFContext, effectPackagePath);
    }

    //卸载指定特效包，effectId创建特效包时返回的特效包id
    public static void UnLoadEffectPackage(int effectId){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用UnLoadEffectPackage接口。";
            Log.e(TAG, LogMsg);
            return;
        }
        //测试打印输出特效信息
        //ShowEffectDetailInfo(effectId);
        OrangeFilter.destroyEffect(mOFContext, effectId);
    }

    //获取特效包effect的内部信息， effectId 创建特效包时候返回的特效包id， effectInfo自己外面定义的effectInfo对象，用于容纳effect信息。
    //成功返回OF_Result_Success后，即可从effectInfo中获取数据。
    public static int GetEffectInfo(int effectId, OrangeFilter.OF_EffectInfo effectInfo){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用GetEffectInfo接口。";
            Log.e(TAG, LogMsg);
            return OrangeFilter.OF_Result_Failed;
        }
        return OrangeFilter.getEffectInfo(mOFContext, effectId, effectInfo);
    }

    //这个函数是！！！每帧！！！都需要刷新的，因为需要时刻知晓当前渲染纹理的信息。
    //用户根据自己情况可以把这些代码集成到自己的项目中，这些参数最终会决定人脸识别等高级效果是否成功。
    public static void UpdateFrameParams(GLTexture textureIn/*输入纹理*/,
                                         GLTexture textureOut/*输出纹理*/,
                                         CameraUtil.ReadedImage image/*相机采集的图像*/,
                                         int[] effectArray/*使用的特效id队列*/,
                                         int[]resultArray/*每个特效的结果*/){
        mInputs[0].width = textureIn.getWidth();
        mInputs[0].height = textureIn.getHeight();
        mInputs[0].format = textureIn.getFormat();
        mInputs[0].target = textureIn.getTarget();
        mInputs[0].textureID = textureIn.getTextureId();

        mOutputs[0].width = textureOut.getWidth();
        mOutputs[0].height = textureOut.getHeight();
        mOutputs[0].format = textureOut.getFormat();
        mOutputs[0].target = textureOut.getTarget();
        mOutputs[0].textureID = textureOut.getTextureId();

        boolean useCustomHarsLib = false;

        if (!useCustomHarsLib) {
            if (mFrameData.faceFrameDataArr == null) {
                mFrameData.faceFrameDataArr = new OrangeFilter.OF_FaceFrameData[0];
            }
        }
        mFrameData.width = image.height;//这里没写错，就是反正写的
        mFrameData.height = image.width;//这里没写错，就是反正写的
        mFrameData.widthStep = mFrameData.width;
        mFrameData.timestamp = (System.currentTimeMillis() - mStartTime) / 1000.0f;
        mFrameData.isUseCustomHarsLib = useCustomHarsLib;
        mFrameData.trackOn = false;
        mFrameData.curNode = 0;
        mFrameData.pickOn = false;
        mFrameData.pickResult = false;

        //！！！下面信息很重要，会决定高级美颜效果是否生效，而且具体情况会根据用户的实际输入情况作出修改调整。！！！
        //android摄像头默认OF_PixelFormat_NV21
        // 格式，如果使用三方sdk，或者自己输入图片，需要用户根据实际输入image.data确认具体的格式。比如OF_PixelFormat_RGB32也是有可能的。
        //格式输入错误，会导致无法正确识别人脸，手势等高级效果。
        // mFrameData.imageDeviceType = 0;//默认攝像頭采集圖像，常见用法
        // if(1 == mFrameData.imageDeviceType ){//这种模式下更加灵活，需要用户自己设置更多的参数，可以解决任何的图像输入情况，三方sdk接入的必备。
        //     //这种参数从1.3.0版本开始支持。这种模式可以解决所有识别问题，但是设置参数交给用户来决定。会更灵活。
        //     //设置人脸渲染结果的方向
        //     mFrameData.facePointOrientation = image.dir;//可以用陀螺仪方向（左转90度0， 竖着正立1， 右转90度2， 倒立竖着3）
        //     //设置图像识别方向
        //     mFrameData.imageDir = image.dir;//可以用陀螺仪方向（左转90度0， 竖着正立1， 右转90度2， 倒立竖着3）
        //     //设置渲染结果是否镜像
        //     mFrameData.frontCamera = image.frontCamera;//调整识别结果是否需要水平翻转（镜像）
        //     //设置用户自定义图像数据
        //     mFrameData.imageData = image.data;//图像数据buff
        //     //自定义图像数据格式
        //     mFrameData.format = OrangeFilter.OF_PixelFormat_NV21;//android摄像头默认采集格式，不是摄像头输入的要注意。
        // }else{
            //这种默认摄像头采集图像模式下，sdk已经做了很多情况的处理，简单易用。
            mFrameData.orientation = image.orientation;//图像方向（-90， 0， 90， 270）
            mFrameData.imageDir = image.dir;//可以用陀螺仪方向（左转90度0， 竖着正立1， 右转90度2， 倒立竖着3）
            mFrameData.frontCamera = image.frontCamera;//调整识别结果是否需要水平翻转（镜像）
            mFrameData.imageData = image.data;//图像数据buff
            mFrameData.format = OrangeFilter.OF_PixelFormat_NV21;//android摄像头默认采集格式，不是摄像头输入的要注意。
        // }

        //向SDK请求识别图像
        OrangeFilter.prepareFrameData(mOFContext, mFrameData);
        //开始渲染图像
        OrangeFilter.applyFrameBatch(mOFContext, effectArray, mInputs, mOutputs,
                resultArray);
    }

    //example:获取指定effect中的某个滤镜（filter）中的变量名字.并打印它，修改它。
    public static void ShowEffectDetailInfo(int effectId){
        if(0 == mOFContext) {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用ShowMeEffectDetailInfo接口。";
            Log.e(TAG, LogMsg);
            return;
        }
        OrangeFilter.OF_EffectInfo effectInfo = new OrangeFilter.OF_EffectInfo();
        OrangeHelper.GetEffectInfo(effectId, effectInfo);
        Log.i(TAG, "特效包id：" + effectId + "  包含的滤镜（filter）个数：" + effectInfo.filterCount);
        for(int i = 0; i < effectInfo.filterCount; ++i) {
            int filterId = effectInfo.filterList[i];
            Log.i(TAG, "第"+ i +"个 filter  id = " + filterId);
            int paramsCount = OrangeHelper.GetFilterParamCount(filterId);
            Log.i(TAG, "这个filter中包含的变量信息如下： 变量个数 = " + paramsCount);
            for(int j = 0; j < paramsCount; ++j){
                String filterParamName = OrangeHelper.GetFilterParamName(filterId, j);
                OrangeFilter.OF_Param paramValue = OrangeHelper.GetFilterParam(filterId, filterParamName);
                //所以，如果你知道某个变量名字是存在的，你可以直接获取变量值。当然你需要区分filterId，因为不同的filter中可能存在相同的名字.
                Log.i(TAG,
                        "当前filter中第" + j + "个变量名字： " + filterParamName + "  变量值：" );
                if(null != paramValue && paramValue.getType() == OrangeFilter.OF_ParamType_Float){
                    OrangeFilter.OF_Paramf paramf= (OrangeFilter.OF_Paramf)paramValue;
                    Log.i(TAG, " 最小值: " + paramf.minVal +
                            " 最大值:" + paramf.maxVal +
                            " 默认值:" + paramf.defVal +
                            " 当前值:" + paramf.val);
                    //那么，现在可以用同样的方式，给某个特效包effectId中某个滤镜filterId，设置某个变量的名字了。
                    //設置当前值，需要注意变量类型别弄错。

                    paramf.val = paramf.val +0.123456789f;//尽量不要修改变量的最大最小值，可能出现难以想象的错误。
                    if(paramf.val > paramf.maxVal){
                        paramf.val = paramf.maxVal;
                    }else if(paramf.val < paramf.minVal){
                        paramf.val = paramf.minVal;
                    }
                    Log.i(TAG, "尝试修改这个变量的值： 修改后 " +paramf.val);
                    // OrangeHelper.SetFilterParam(filterId, filterParamName,paramf);

                }else if(null != paramValue && paramValue.getType() == OrangeFilter.OF_ParamType_Int){
                    OrangeFilter.OF_Parami parami= (OrangeFilter.OF_Parami)paramValue;
                    Log.i(TAG, " 最小值: " + parami.minVal +
                            " 最大值:" + parami.maxVal +
                            " 默认值:" + parami.defVal +
                            " 当前值:" + parami.val);
                }else if(null != paramValue && paramValue.getType() == OrangeFilter.OF_ParamType_Bool){
                    OrangeFilter.OF_ParamBool paramb= (OrangeFilter.OF_ParamBool)paramValue;
                    //省略1万字
                }
            }
        }
    }

    //获取指定filterId中的指定变量名的数据。filterId可以从effectInfo中获取， paramName从文档获取，或者遍历打印一次所有变量名字，获取想要动态修改的变量名。
    public static OrangeFilter.OF_Param GetFilterParam(int filterId, String paramName){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用GetFilterParam接口。";
            Log.e(TAG, LogMsg);
            OrangeFilter.OF_Paramf ret = new OrangeFilter.OF_Paramf();
            ret.name = "null";
            return ret;
        }
        return OrangeFilter.getFilterParamData(mOFContext, filterId, paramName);
    }

    //设置指定filterId中的变量。
    public static int SetFilterParam(int filterId, String paramName,
                                                       OrangeFilter.OF_Param paramValue){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用SetFilterParam接口。";
            Log.e(TAG, LogMsg);
            return OrangeFilter.OF_Result_Failed;
        }
        return OrangeFilter.setFilterParamData(mOFContext, filterId, paramName, paramValue);
    }

    //获取指定filterId中的变量个数。
    public static int GetFilterParamCount(int filterId){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用GetFilterParamCount接口。";
            Log.e(TAG, LogMsg);
            return OrangeFilter.OF_Result_Failed;
        }
        return OrangeFilter.getFilterParamCount(mOFContext, filterId);
    }
    //获取指定filterId中的第几个变量的名字。
    public static String GetFilterParamName(int filterId, int paramIndex){
        if(0 == mOFContext)
        {
            LogMsg = "只有通过OrangeHelper.CreateContext方式创建的引擎，才可以使用GetFilterParamName接口。";
            Log.e(TAG, LogMsg);
            return "";
        }
        return OrangeFilter.getFilterParamName(mOFContext, filterId, paramIndex);
    }

    public static void RefreshResources(Context activity, String venusModelPath){
        //检查当前的版本号来判定缓存资源是否和当前SDK版本匹配，不匹配即删除缓存。
        CheckResourcesValid(activity);
        File modelDir = new File(venusModelPath);
        //如果资源不存在，则从SDK包里解压到venusModelPath。
        if (!(modelDir.isDirectory() && modelDir.exists())) {
            //解压人脸点检测AI模型文件
            new File(venusModelPath + "/face").mkdirs();
            OrangeFilter.extractAssetsDir(
                    activity.getAssets(),
                    "models/venus_models/face",
                    venusModelPath + "/face"
            );
            //解压背景分割AI模型文件
            new File(venusModelPath + "/segment").mkdirs();
            OrangeFilter.extractAssetsDir(
                    activity.getAssets(),
                    "models/venus_models/segment",
                    venusModelPath + "/segment"
            );
            //解压手势识别AI模型文件
            new File(venusModelPath + "/gesture").mkdirs();
            OrangeFilter.extractAssetsDir(
                    activity.getAssets(),
                    "models/venus_models/gesture",
                    venusModelPath + "/gesture"
            );
        }
    }

    private static void CheckResourcesValid(Context activity) {
        SharedPreferences pf = getDefaultSharedPreferences(activity);
        final String configKey = "setup_version";
        final String buildVersion = activity.getString(R.string.build_version);//build_version
        // 是String类型的字符窜， 用户需要根据自己的情况来判断app版本是否变化了。（我们的方式是gradle里自动写当前版本信息到字符窜xml里，这样每次版本变化下面可以知晓。）
        //gradle里加个自动化处理，每次构建可以写app版本信息。大概下面这个样子，用户自己需要的话就去改。
/*        new File('app/src/main/res/values/packagbuild.xml').withWriter('UTF-8') { writer ->
                writer.write(String.format("<resources>\n" +
                        "    <string name=\"build_name\">%s</string>\n" +
                        "    <string name=\"build_version\">%s</string>\n" +
                        "</resources>", "$System.env.BUILD_DISPLAY_NAME", "$version"));
        }*/
        Log.i(TAG, "to delete effects and venus directory ....");
        if (!pf.getString(configKey, "-1").equals(buildVersion)) {
            final String effectPath = activity.getFilesDir().getPath()
                    + "/orangefilter/effects/";
            //delete dir
            File dir = new File(effectPath);
            deleteDirWihtFile(dir);
            final String venusModelPath = activity.getFilesDir().getPath()
                    + "/orangefilter/models/venus_models";
            dir = new File(venusModelPath);
            deleteDirWihtFile(dir);

            SharedPreferences.Editor pfe = pf.edit();
            pfe.putString(configKey, buildVersion);
            pfe.commit();
            Log.i(TAG, "delete effects and venus directory！");
        }
    }

    private static void deleteDirWihtFile(File dir) {
        if (dir == null || !dir.exists() || !dir.isDirectory()) {
            return;
        }
        for (File file : dir.listFiles()) {
            if (file.isFile()) {
                //Log.i(TAG, file.getName());
                file.delete();
            } else if (file.isDirectory()) {
                deleteDirWihtFile(file);
            }
        }
        dir.delete();
    }
}
