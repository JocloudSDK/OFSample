package com.yy.orangefilter.test.utils;

import com.yy.orangefilter.test.R;
import com.yy.orangefilter.test.helper.OrangeHelper;

import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_BasicBeautyClearIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_BasicBeautyClearOpacity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_BasicBeautyIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_BasicBeautyOpacity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterAdaierIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterAmorousIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterClearIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterCreamIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterDanyaIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterDeliciousIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterEnhanceClearIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterEnhanceNoulanIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterFennenIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterFilmIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterFreshIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterGrilLiteratureIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterGrilLuolitaIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterGrilWhiteIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterHolidayIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterJiaZhouIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterMagazineIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterMojitoIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterPeachIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterPureIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterSunIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterSweetIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterTenderIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterWarmIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_FilterWenNuanIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeBigSmallEyeIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeChinLiftingIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeEyesOffsetIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeEyesRotationIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeForeheadLiftingIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeLongNoseIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeMovemouthIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeSmallFaceIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeSquashedFaceIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeThinFaceIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeThinNoseBridgeIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeThinNoseIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeThinmouthIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType.EP_SeniorTypeWideForeheadIntensity;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterAdaier;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterAmorous;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterClear;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterCream;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterDanya;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterDelicious;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterEnhanceClear;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterEnhanceNoulan;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterFennen;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterFilm;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterFresh;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterGrilLiterature;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterGrilLuolita;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterGrilWhite;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterHoliday;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterJiaZhou;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterMagazine;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterMojito;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterPeach;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterPure;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterSun;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterSweet;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterTender;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterWarm;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_FilterWenNuan;

/**
 * created by chenshibiao on 2020/8/5.
 */
public class Constant {

    public static final int[] beautyHazyOptionName = new int[]{//朦胧美颜
            R.string.skin_hazy,
            R.string.whitening
    };
    //Clear Skin
    public static final int[] beautyClearOptionName = new int[]{
            R.string.skin_clear,
            R.string.whitening
    };
    public static final com.yy.orangefilter.test.helper.OrangeHelper.EffectParamType[] beautyTypeHazy =
            new OrangeHelper.EffectParamType[]{
                    EP_BasicBeautyOpacity, //Smoothen
                    EP_BasicBeautyIntensity, //Whiten
            };
    public static final OrangeHelper.EffectParamType[] beautyTypeClear =
            new OrangeHelper.EffectParamType[]{
                    EP_BasicBeautyClearOpacity,//Smoothen strength
                    EP_BasicBeautyClearIntensity,//Whiten strength
            };
    public static final int[] resBeautyOption = new int[]{
            R.drawable.beauty_1,
            R.drawable.beauty_0
    };
    //plastic
    public static int[] plasticOptionName = new int[]{
            R.string.narrow_face,
            R.string.small_face,
            R.string.thin_jawbone,
            R.string.high_forehead,
            R.string.width_forehead,
            R.string.big_eyes,
            R.string.eye_distance,
            R.string.corner_of_eye,
            R.string.thin_nose,
            R.string.long_nose,
            R.string.narrow_nose,
            R.string.small_mouth,
            R.string.mouth_position,
            R.string.chin,
    };
    public static final OrangeHelper.EffectParamType[] plasticType = new OrangeHelper.EffectParamType[]{
            EP_SeniorTypeThinFaceIntensity, //slim
            EP_SeniorTypeSmallFaceIntensity, //resize
            EP_SeniorTypeSquashedFaceIntensity, //cheek
            EP_SeniorTypeForeheadLiftingIntensity, //forehead
            EP_SeniorTypeWideForeheadIntensity, //forehead
            EP_SeniorTypeBigSmallEyeIntensity, //eye size
            EP_SeniorTypeEyesOffsetIntensity, //distance
            EP_SeniorTypeEyesRotationIntensity, //slant
            EP_SeniorTypeThinNoseIntensity, //slim
            EP_SeniorTypeLongNoseIntensity, //length
            EP_SeniorTypeThinNoseBridgeIntensity, //bridge
            EP_SeniorTypeThinmouthIntensity, //resize
            EP_SeniorTypeMovemouthIntensity, //position
            EP_SeniorTypeChinLiftingIntensity, //chin
    };
    public static final int[] resPlasticOption = new int[]{
            R.drawable.beauty_2,
            R.drawable.beauty_3,
            R.drawable.beauty_4,
            R.drawable.beauty_5,
            R.drawable.beauty_6,
            R.drawable.beauty_7,
            R.drawable.beauty_8,
            R.drawable.beauty_9,
            R.drawable.beauty_10,
            R.drawable.beauty_11,
            R.drawable.beauty_12,
            R.drawable.beauty_13,
            R.drawable.beauty_14,
            R.drawable.beauty_15
    };
    public static final int[] filterOptionName = new int[]{
            R.string.holiday,
            R.string.clear,
            R.string.warm,
            R.string.refreshing,
            R.string.matte,
            R.string.cool_adaier,
            R.string.cool_danya,
            R.string.cool_jiazhou,
            R.string.cool_mojito,
            R.string.warm_fennen,
            R.string.warm_pure,
            R.string.warm_sweet,
            R.string.warm_wennuan,
            R.string.warm_sun,
            R.string.gril_white,
            R.string.gril_luolita,
            R.string.gril_literature,
            R.string.enhance_amorous,
            R.string.enhance_delicious,
            R.string.enhance_clear,
            R.string.enhance_nuolan,
            R.string.enhance_peach,
            R.string.zhigan_cream,
            R.string.zhigan_film,
            R.string.zhigan_magazine

    };
    public static final OrangeHelper.EffectType[] filterType = new OrangeHelper.EffectType[]{
            ET_FilterHoliday, //Filter Holiday
            ET_FilterClear, //Filter Clear
            ET_FilterWarm, //Filter Warm
            ET_FilterFresh, //Filter Fresh
            ET_FilterTender, //Filter Tender

            //20款风格滤镜
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
    };
    public static final OrangeHelper.EffectParamType[] filterEffectType =
            new OrangeHelper.EffectParamType[]{
                    EP_FilterHolidayIntensity, //Holiday filter strength
                    EP_FilterClearIntensity, //Clear filter strength
                    EP_FilterWarmIntensity, //Warm filter strength
                    EP_FilterFreshIntensity, //Fresh filter strength
                    EP_FilterTenderIntensity, //Tender filter strength

                    //20款风格滤镜
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
            };
    public static final int[] resFilterOption = new int[]{
            R.drawable.filter_jiari,
            R.drawable.filter_qingxi,
            R.drawable.filter_nuanyang,
            R.drawable.filter_qingxin,
            R.drawable.filter_fennen,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,
            R.drawable.filter_jiari,

    };
    //Sticker
    //Sticker path
    public static final String[] stickerPath = new String[]{
            "/orangefilter/effects/segment.zip",
            "/orangefilter/effects/sticker_aral.zip",
            "/orangefilter/effects/sticker_black_cat_ears.zip",
            "/orangefilter/effects/sticker_bud_glasses.zip",
            "/orangefilter/effects/sticker_devil_gauze_mask.zip",
            "/orangefilter/effects/sticker_seeking_attention.zip",
            "/orangefilter/effects/sticker_sunglasses.zip",
            "/orangefilter/effects/sticker_YYbear.zip"
    };
    //Sticker icon
    public static final int[] resStickerOption = new int[]{
            R.drawable.sticker_background,
            R.drawable.sticker_aral,
            R.drawable.sticker_black_cat_ears,
            R.drawable.sticker_bud_glasses,
            R.drawable.sticker_devil_gauze_mask,
            R.drawable.sticker_seeking_attention,
            R.drawable.sticker_sunglasses,
            R.drawable.sticker_yybear,
    };

    //Gesture
    //Gesture path
    public static final String[] gesturesPath = new String[]{
            "/orangefilter/effects/gesture_666.zip",
            "/orangefilter/effects/gesture_baoquan.zip",
            "/orangefilter/effects/gesture_heshi.zip",
            "/orangefilter/effects/gesture_ok.zip",
            "/orangefilter/effects/gesture_onehandheart.zip",

            "/orangefilter/effects/gesture_palm.zip",
            "/orangefilter/effects/gesture_thumbsup.zip",
            "/orangefilter/effects/gesture_tuojv.zip",
            "/orangefilter/effects/gesture_twohandheart.zip",
            "/orangefilter/effects/gesture_yeah.zip",

            "/orangefilter/effects/gesture_zhizhu.zip",

    };

    //gesture icon
    public static final int[] resGesturesOption = new int[]{
            R.drawable.gesture_666,
            R.drawable.gesture_baoquan,
            R.drawable.gesture_heshi,
            R.drawable.gesture_ok,
            R.drawable.gesture_onehandheart,

            R.drawable.gesture_palm,
            R.drawable.gesture_thumbsup,
            R.drawable.gesture_tuojv,
            R.drawable.gesture_twohandheart,
            R.drawable.gesture_yeah,

            R.drawable.gesture_zhizhu,
    };

    public static boolean isEnableGesture_666 = false;
    public static boolean isEnableGesture_baoquan = false;
    public static boolean isEnableGesture_heshi = false;
    public static boolean isEnableGesture_ok = false;
    public static boolean isEnableGesture_onehandheart = false;

    public static boolean isEnableGesture_palm = false;
    public static boolean isEnableGesture_thumbsup = false;
    public static boolean isEnableGesture_tuojv = false;
    public static boolean isEnableGesture_twohandheart = false;
    public static boolean isEnableGesture_yeah = false;
    public static boolean isEnableGesture_zhizhu = false;

    public static String gesturePath_666 = "";
    public static String gesturePath_baoquan = "";
    public static String gesturePath_heshi = "";
    public static String gesturePath_ok = "";
    public static String gesturePath_onehandheart = "";

    public static String gesturePath_palm = "";
    public static String gesturePath_thumbsup = "";
    public static String gesturePath_tuojv = "";
    public static String gesturePath_twohandheart = "";
    public static String gesturePath_yeah = "";
    public static String gesturePath_zhizhu = "";

    public static void disableAllGesture() {
        isEnableGesture_666 = false;
        isEnableGesture_baoquan = false;
        isEnableGesture_heshi = false;
        isEnableGesture_ok = false;
        isEnableGesture_onehandheart = false;

        isEnableGesture_palm = false;
        isEnableGesture_thumbsup = false;
        isEnableGesture_tuojv = false;
        isEnableGesture_twohandheart = false;
        isEnableGesture_yeah = false;
        isEnableGesture_zhizhu = false;

        // gesturePath_666 = "";
        // gesturePath_baoquan = "";
        // gesturePath_heshi = "";
        // gesturePath_ok = "";
        // gesturePath_onehandheart = "";
        //
        // gesturePath_palm = "";
        // gesturePath_thumbsup = "";
        // gesturePath_tuojv = "";
        // gesturePath_twohandheart = "";
        // gesturePath_yeah = "";
        // gesturePath_zhizhu = "";
    }

    public static void initGesturePath(String dir) {
        gesturePath_666 = dir + gesturesPath[0];
        gesturePath_baoquan = dir + gesturesPath[1];
        gesturePath_heshi = dir + gesturesPath[2];
        gesturePath_ok = dir + gesturesPath[3];
        gesturePath_onehandheart = dir + gesturesPath[4];

        gesturePath_palm = dir + gesturesPath[5];
        gesturePath_thumbsup = dir + gesturesPath[6];
        gesturePath_tuojv = dir + gesturesPath[7];
        gesturePath_twohandheart = dir + gesturesPath[8];
        gesturePath_yeah = dir + gesturesPath[9];
        gesturePath_zhizhu = dir + gesturesPath[10];
    }

    public static void enableGesture(int index, boolean result) {
        switch (index) {
            case 1:
                isEnableGesture_666 = result;
                break;
            case 2:
                isEnableGesture_baoquan = result;
                break;
            case 3:
                isEnableGesture_heshi = result;
                break;
            case 4:
                isEnableGesture_ok = result;
                break;
            case 5:
                isEnableGesture_onehandheart = result;
                break;
            case 6:
                isEnableGesture_palm = result;
                break;
            case 7:
                isEnableGesture_thumbsup = result;
                break;
            case 8:
                isEnableGesture_tuojv = result;
                break;
            case 9:
                isEnableGesture_twohandheart = result;
                break;
            case 10:
                isEnableGesture_yeah = result;
                break;
            case 11:
                isEnableGesture_zhizhu = result;
                break;
            default:
                break;
        }
    }

    public static boolean isDisableAllGesture() {
        return !isEnableGesture_666 && !isEnableGesture_baoquan && !isEnableGesture_heshi && !isEnableGesture_ok && !isEnableGesture_onehandheart &&
                !isEnableGesture_palm && !isEnableGesture_thumbsup && !isEnableGesture_tuojv && !isEnableGesture_twohandheart && !isEnableGesture_yeah && !isEnableGesture_zhizhu;
    }
}
