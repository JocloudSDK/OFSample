package com.yy.orangefilter.test;

import android.Manifest;
import android.app.AlertDialog;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.orangefilter.OrangeFilter;
import com.yy.orangefilter.test.bean.BeautyBean;
import com.yy.orangefilter.test.bean.FilterBean;
import com.yy.orangefilter.test.bean.StickersBean;
import com.yy.orangefilter.test.helper.OrangeHelper;
import com.yy.orangefilter.test.ui.BeautyDialog;
import com.yy.orangefilter.test.ui.FilterDialog;
import com.yy.orangefilter.test.ui.StickersDialog;
import com.yy.orangefilter.test.utils.CameraUtil;
import com.yy.orangefilter.test.utils.CameraView;
import com.yy.orangefilter.test.utils.GLTexture;

import java.util.ArrayList;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

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
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_BasicBeauty;
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_BasicBeauty5;
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
import static com.yy.orangefilter.test.helper.OrangeHelper.EffectType.ET_SeniorBeautyType;
import static com.yy.orangefilter.test.helper.OrangeHelper.VENUS_ALL;
import static com.yy.orangefilter.test.utils.Constant.beautyClearOptionName;
import static com.yy.orangefilter.test.utils.Constant.beautyHazyOptionName;
import static com.yy.orangefilter.test.utils.Constant.beautyTypeClear;
import static com.yy.orangefilter.test.utils.Constant.beautyTypeHazy;
import static com.yy.orangefilter.test.utils.Constant.filterEffectType;
import static com.yy.orangefilter.test.utils.Constant.filterOptionName;
import static com.yy.orangefilter.test.utils.Constant.filterType;
import static com.yy.orangefilter.test.utils.Constant.plasticOptionName;
import static com.yy.orangefilter.test.utils.Constant.plasticType;
import static com.yy.orangefilter.test.utils.Constant.resBeautyOption;
import static com.yy.orangefilter.test.utils.Constant.resFilterOption;
import static com.yy.orangefilter.test.utils.Constant.resPlasticOption;
import static com.yy.orangefilter.test.utils.Constant.resStickerOption;
import static com.yy.orangefilter.test.utils.Constant.stickerPath;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private static final String TAG = "MainActivity";

    private static final int PERMISSION_REQUEST_CAMERA = 0;
    private static final int PERMISSION_REQUEST_STORAGE = 1;

    private CameraView mCameraView;
    private TextView tvBeautyHazy;//Hazy Skin
    private TextView tvBeautyClear;//Clear Skin
    private TextView tvPlastic;
    private TextView tvFilter;
    private TextView tvSticker;
    private LinearLayout llBottom;

    private int mOFContext;
    private AlertDialog mDialog;

    private boolean isEnableBeautyHazy = true;
    private boolean isEnableBeautyClear = false;
    private boolean isEnablePlastic = false;

    private OrangeHelper.EffectType selectFilterType = null;
    private OrangeHelper.EffectParamType selectEffectParamType = null;
    private int filterVal = 0;
    private OrangeHelper.EffectType oldselectFilterType = null;

    private String stickerEffectPath = "";//sticker path
    private boolean isEnableSticker = false;
    private String oldstickerEffectPath = "";//old sticker path
    private boolean isReleaseOldSticker = false;

    private List<BeautyBean> beautyHazyList = new ArrayList<>();
    private List<BeautyBean> beautyClearList = new ArrayList<>();
    private List<BeautyBean> plasticList = new ArrayList<>();
    private List<FilterBean> filterList = new ArrayList<>();
    private List<StickersBean> stickerList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        checkPermissions();
    }

    private void checkPermissions() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) !=
                    PackageManager.PERMISSION_GRANTED) {
                this.requestPermissions(
                        new String[]{
                                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                Manifest.permission.READ_EXTERNAL_STORAGE
                        },
                        PERMISSION_REQUEST_STORAGE
                );
            } else if (this.checkSelfPermission(Manifest.permission.CAMERA) !=
                    PackageManager.PERMISSION_GRANTED) {
                this.requestPermissions(new String[]{Manifest.permission.CAMERA},
                        PERMISSION_REQUEST_CAMERA);
            } else {
                initView();
            }
        } else {
            initView();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            this.checkPermissions();
        } else {
            Toast.makeText(this, getResources().getString(R.string.permission_denied),
                    Toast.LENGTH_LONG).show();
            finish();
        }
    }

    private void initView() {
        setContentView(R.layout.layout_mainactivity);
        tvBeautyHazy = findViewById(R.id.tvBeautyHazy);
        tvBeautyClear = findViewById(R.id.tvBeautyClear);
        tvPlastic = findViewById(R.id.tvPlastic);
        tvFilter = findViewById(R.id.tvFilter);
        tvSticker = findViewById(R.id.tvSticker);
        llBottom = findViewById(R.id.ll_bottom);
        mCameraView = findViewById(R.id.cameraView);

        tvBeautyHazy.setOnClickListener(this::onClick);
        tvBeautyClear.setOnClickListener(this::onClick);
        tvPlastic.setOnClickListener(this::onClick);
        tvFilter.setOnClickListener(this::onClick);
        tvSticker.setOnClickListener(this::onClick);

        // for render
        mCameraView.setDrawFrameCallback(360, 640, mDrawFrameCallback);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.tvBeautyHazy:
                isEnableBeautyHazy = true;
                isEnableBeautyClear = false;
                llBottom.setVisibility(View.GONE);
                showHazyBeautyDialog();
                break;
            case R.id.tvBeautyClear:
                isEnableBeautyHazy = false;
                isEnableBeautyClear = true;
                llBottom.setVisibility(View.GONE);
                showClearBeautyDialog();
                break;
            case R.id.tvPlastic:
                llBottom.setVisibility(View.GONE);
                showPlasticDialog();
                break;
            case R.id.tvFilter:
                llBottom.setVisibility(View.GONE);
                showFilterDialog();
                break;
            case R.id.tvSticker:
                llBottom.setVisibility(View.GONE);
                showStickerDialog();
                break;
        }
    }

    /**
     * 初始化朦胧美颜特效包参数
     * init Hazy Beauty Option
     */
    private void initHazyBeautyOption() {
        beautyHazyList.clear();
        OrangeHelper.enableEffect(ET_BasicBeauty, true);
        BeautyBean originalBeauty =
                new BeautyBean(getResources().getString(R.string.original), null, null,
                        R.drawable.beauty_original);
        beautyHazyList.add(0, originalBeauty);

        OrangeHelper.enableEffect(ET_BasicBeauty, true);
        for (int i = 0; i < beautyHazyOptionName.length; i++) {
            OrangeHelper.EffectParam of_paramf = new OrangeHelper.EffectParam();
            OrangeHelper.getEffectParamDetail(beautyTypeHazy[i], of_paramf);
            BeautyBean basicBeauty =
                    new BeautyBean(getResources().getString(beautyHazyOptionName[i]), of_paramf,
                            beautyTypeHazy[i], resBeautyOption[i]);
            if (i == 0) {
                basicBeauty.setEnable(true);
            }
            beautyHazyList.add(basicBeauty);
        }
    }

    /**
     * 初始化清晰美颜特效包参数
     * init Clear Beauty Option
     */
    private void initClearBeautyOption() {
        beautyClearList.clear();
        OrangeHelper.enableEffect(ET_BasicBeauty5, true);
        BeautyBean originalBeauty =
                new BeautyBean(getResources().getString(R.string.original), null, null,
                        R.drawable.beauty_original);
        beautyClearList.add(0, originalBeauty);

        for (int i = 0; i < beautyClearOptionName.length; i++) {
            OrangeHelper.EffectParam of_paramf = new OrangeHelper.EffectParam();
            OrangeHelper.getEffectParamDetail(beautyTypeClear[i], of_paramf);
            BeautyBean basicBeauty =
                    new BeautyBean(getResources().getString(beautyClearOptionName[i]), of_paramf,
                            beautyTypeClear[i], resBeautyOption[i]);
            if (i == 0) {
                basicBeauty.setEnable(true);
            }
            beautyClearList.add(basicBeauty);
        }

        OrangeHelper.enableEffect(ET_BasicBeauty5, false);
    }

    /**
     * 初始化整形特效包参数
     * init Plastic Option
     */
    private void initPlasticOption() {
        plasticList.clear();

        OrangeHelper.enableEffect(ET_SeniorBeautyType, true);
        //Original
        BeautyBean originalPlastic =
                new BeautyBean(getResources().getString(R.string.original), null, null,
                        R.drawable.beauty_original);
        originalPlastic.setEnable(true);
        plasticList.add(originalPlastic);

        for (int i = 0; i < plasticOptionName.length; i++) {
            OrangeHelper.EffectParam of_paramf = new OrangeHelper.EffectParam();
            OrangeHelper.getEffectParamDetail(plasticType[i], of_paramf);
            BeautyBean beautyBean =
                    new BeautyBean(getResources().getString(plasticOptionName[i]), of_paramf,
                            plasticType[i],
                            resPlasticOption[i]);
            plasticList.add(beautyBean);
        }

        OrangeHelper.enableEffect(ET_SeniorBeautyType, false);
    }

    /**
     * 初始化滤镜特效包参数
     * init Filter Option
     */
    private void initFilterOption() {
        filterList.clear();
        //Original
        FilterBean orignialFilter = new FilterBean(getResources().getString(R.string.original),
                R.drawable.beauty_original, null, null);
        orignialFilter.setEnable(true);
        filterList.add(orignialFilter);

        for (int i = 0; i < filterOptionName.length; i++) {
            OrangeHelper.enableEffect(filterType[i], true);
            // OrangeHelper.setEffectParam(filterEffectType[i],1);//set default
            OrangeHelper.EffectParam effectParam = new OrangeHelper.EffectParam();
            OrangeHelper.getEffectParamDetail(filterEffectType[i], effectParam);
            FilterBean filterBean = new FilterBean(getResources().getString(filterOptionName[i]),
                    resFilterOption[i],
                    filterEffectType[i], effectParam);
            filterList.add(filterBean);
            OrangeHelper.enableEffect(filterType[i], false);
        }
    }

    /**
     * 初始化贴纸特效包
     * init Sticker Option
     */
    private void initStickerOption() {
        stickerList.clear();
        for (int i = 0; i < stickerPath.length; i++) {
            String effectPath = getFilesDir().getPath() + stickerPath[i];
            int thumb = resStickerOption[i];
            StickersBean stickersBean = new StickersBean(effectPath, thumb, false);
            stickerList.add(stickersBean);
        }
        stickerList.add(0, new StickersBean(null, R.drawable.close, true));
    }

    /**
     * 朦胧美颜弹窗
     * Hazy Beauty Dialog
     */
    private void showHazyBeautyDialog() {
        if (beautyHazyList.size() == 0) {
            llBottom.setVisibility(View.VISIBLE);
            return;
        }
        BeautyDialog beautyDialog = new BeautyDialog(beautyHazyList,
                new BeautyDialog.OnItemSelectedListener() {

                    @Override
                    public void onItemSelected(int index) {
                        if (index == 0) {//Original
                            isEnableBeautyHazy = false;
                        } else {
                            isEnableBeautyHazy = true;
                        }
                    }

                    @Override
                    public void onSeekChanged(OrangeHelper.EffectParamType paramType, int value) {
                        OrangeHelper.setEffectParam(paramType, value);
                    }

                    @Override
                    public void onDialogDismiss() {
                        llBottom.setVisibility(View.VISIBLE);
                    }
                });
        beautyDialog.show(getSupportFragmentManager());
    }

    /**
     * 清晰美颜弹窗
     * Clear Beauty Dialog
     */
    private void showClearBeautyDialog() {
        if (beautyClearList.size() == 0) {
            llBottom.setVisibility(View.VISIBLE);
            return;
        }
        BeautyDialog beautyDialog = new BeautyDialog(beautyClearList,
                new BeautyDialog.OnItemSelectedListener() {

                    @Override
                    public void onItemSelected(int index) {
                        if (index == 0) {//Original
                            isEnableBeautyClear = false;
                        } else {
                            isEnableBeautyClear = true;
                        }
                    }

                    @Override
                    public void onSeekChanged(OrangeHelper.EffectParamType paramType, int value) {
                        OrangeHelper.setEffectParam(paramType, value);
                    }

                    @Override
                    public void onDialogDismiss() {
                        llBottom.setVisibility(View.VISIBLE);
                    }
                });
        beautyDialog.show(getSupportFragmentManager());
    }

    /**
     * 整形弹窗
     * Plastic Dialog
     */
    private void showPlasticDialog() {
        if (plasticList.size() == 0) {
            llBottom.setVisibility(View.VISIBLE);
            return;
        }
        BeautyDialog beautyDialog = new BeautyDialog(plasticList,
                new BeautyDialog.OnItemSelectedListener() {

                    @Override
                    public void onItemSelected(int index) {
                        if (index == 0) {//Original
                            isEnablePlastic = false;
                        } else {
                            isEnablePlastic = true;
                        }
                    }

                    @Override
                    public void onSeekChanged(OrangeHelper.EffectParamType paramType, int value) {
                        OrangeHelper.setEffectParam(paramType, value);
                    }

                    @Override
                    public void onDialogDismiss() {
                        llBottom.setVisibility(View.VISIBLE);
                    }
                });
        beautyDialog.show(getSupportFragmentManager());
    }

    /**
     * 滤镜弹窗
     * Filter Dialog
     */
    private void showFilterDialog() {
        if (filterList.size() == 0) {
            llBottom.setVisibility(View.VISIBLE);
            return;
        }
        FilterDialog filterDialog = new FilterDialog(filterList,
                new FilterDialog.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(int index) {
                        if (index == 0) {
                            //disable  filter
                            if (selectFilterType != null) {
                                OrangeHelper.enableEffect(selectFilterType, false);
                            }

                            selectFilterType = null;
                            selectEffectParamType = null;
                            filterVal = 0;
                        } else {
                            //disable filter
                            if (selectFilterType != null) {
                                oldselectFilterType = selectFilterType;
                                OrangeHelper.enableEffect(oldselectFilterType, false);
                            }

                            filterVal = filterList.get(index).getOf_paramf().curVal;
                            if (index == 1) {
                                selectFilterType = ET_FilterHoliday;
                                selectEffectParamType = EP_FilterHolidayIntensity;
                            } else if (index == 2) {
                                selectFilterType = ET_FilterClear;
                                selectEffectParamType = EP_FilterClearIntensity;
                            } else if (index == 3) {
                                selectFilterType = ET_FilterWarm;
                                selectEffectParamType = EP_FilterWarmIntensity;
                            } else if (index == 4) {
                                selectFilterType = ET_FilterFresh;
                                selectEffectParamType = EP_FilterFreshIntensity;
                            } else if (index == 5) {
                                selectFilterType = ET_FilterTender;
                                selectEffectParamType = EP_FilterTenderIntensity;
                            } else if (index == 6) {
                                selectFilterType = ET_FilterAdaier;
                                selectEffectParamType = EP_FilterAdaierIntensity;
                            } else if (index == 7) {
                                selectFilterType = ET_FilterDanya;
                                selectEffectParamType = EP_FilterDanyaIntensity;
                            } else if (index == 8) {
                                selectFilterType = ET_FilterJiaZhou;
                                selectEffectParamType = EP_FilterJiaZhouIntensity;
                            } else if (index == 9) {
                                selectFilterType = ET_FilterMojito;
                                selectEffectParamType = EP_FilterMojitoIntensity;
                            } else if (index == 10) {
                                selectFilterType = ET_FilterFennen;
                                selectEffectParamType = EP_FilterFennenIntensity;
                            } else if (index == 11) {
                                selectFilterType = ET_FilterPure;
                                selectEffectParamType = EP_FilterPureIntensity;
                            } else if (index == 12) {
                                selectFilterType = ET_FilterSweet;
                                selectEffectParamType = EP_FilterSweetIntensity;
                            } else if (index == 13) {
                                selectFilterType = ET_FilterWenNuan;
                                selectEffectParamType = EP_FilterWenNuanIntensity;
                            } else if (index == 14) {
                                selectFilterType = ET_FilterSun;
                                selectEffectParamType = EP_FilterSunIntensity;
                            } else if (index == 15) {
                                selectFilterType = ET_FilterGrilWhite;
                                selectEffectParamType = EP_FilterGrilWhiteIntensity;
                            } else if (index == 16) {
                                selectFilterType = ET_FilterGrilLuolita;
                                selectEffectParamType = EP_FilterGrilLuolitaIntensity;
                            } else if (index == 17) {
                                selectFilterType = ET_FilterGrilLiterature;
                                selectEffectParamType = EP_FilterGrilLiteratureIntensity;
                            } else if (index == 18) {
                                selectFilterType = ET_FilterAmorous;
                                selectEffectParamType = EP_FilterAmorousIntensity;
                            } else if (index == 19) {
                                selectFilterType = ET_FilterDelicious;
                                selectEffectParamType = EP_FilterDeliciousIntensity;
                            } else if (index == 20) {
                                selectFilterType = ET_FilterEnhanceClear;
                                selectEffectParamType = EP_FilterEnhanceClearIntensity;
                            } else if (index == 21) {
                                selectFilterType = ET_FilterEnhanceNoulan;
                                selectEffectParamType = EP_FilterEnhanceNoulanIntensity;
                            } else if (index == 22) {
                                selectFilterType = ET_FilterPeach;
                                selectEffectParamType = EP_FilterPeachIntensity;
                            } else if (index == 23) {
                                selectFilterType = ET_FilterCream;
                                selectEffectParamType = EP_FilterCreamIntensity;
                            } else if (index == 24) {
                                selectFilterType = ET_FilterFilm;
                                selectEffectParamType = EP_FilterFilmIntensity;
                            } else if (index == 25) {
                                selectFilterType = ET_FilterMagazine;
                                selectEffectParamType = EP_FilterMagazineIntensity;
                            }
                        }
                    }

                    @Override
                    public void onSeekChanged(OrangeHelper.EffectParamType effectParam, int value) {
                        selectEffectParamType = effectParam;
                        filterVal = value;
                    }

                    @Override
                    public void onDialogDismiss() {
                        llBottom.setVisibility(View.VISIBLE);
                    }
                });
        filterDialog
                .show(getSupportFragmentManager());
    }

    /**
     * 贴纸弹窗
     * Sticker Dialog
     */
    private void showStickerDialog() {
        if (stickerList.size() == 0) {
            llBottom.setVisibility(View.VISIBLE);
            return;
        }
        StickersDialog stickersDialog = new StickersDialog(stickerList,
                new StickersDialog.OnItemSelectedListener() {
                    @Override
                    public void onCleanSticker() {
                        isEnableSticker = false;
                        oldstickerEffectPath = stickerEffectPath;
                        isReleaseOldSticker = true;
                    }

                    @Override
                    public void onItemSelected(int index) {
                        String selectStickerPath = stickerList.get(index).getEffectPath();
                        if (selectStickerPath.equals(stickerEffectPath)) {
                        } else {
                            isReleaseOldSticker = true;
                            oldstickerEffectPath = stickerEffectPath;
                            stickerEffectPath = selectStickerPath;
                            isEnableSticker = true;
                        }
                    }

                    @Override
                    public void onDialogDismiss() {
                        llBottom.setVisibility(View.VISIBLE);
                    }
                });

        stickersDialog.show(getSupportFragmentManager());
    }

    private CameraView.DrawFrameCallback mDrawFrameCallback = new CameraView.DrawFrameCallback() {
        @Override
        public void onDrawFrame(GLTexture textureIn, GLTexture textureOut,
                                CameraUtil.ReadedImage image) {

            if (!OrangeHelper.isContextValid() || image.data == null) {
                mCameraView.copyTexture(textureIn, textureOut);
                return;
            }

            OrangeHelper.enableEffect(ET_BasicBeauty, isEnableBeautyHazy);//hazy

            OrangeHelper.enableEffect(ET_BasicBeauty5, isEnableBeautyClear);//clear

            OrangeHelper.enableEffect(ET_SeniorBeautyType, isEnablePlastic);//plastic

            if (selectFilterType != null && selectEffectParamType != null) {//filter
                OrangeHelper.enableEffect(selectFilterType, true);
                OrangeHelper.setEffectParam(selectEffectParamType, filterVal);
            }

            if (isEnableSticker) {//sticker
                if (isReleaseOldSticker && !TextUtils.isEmpty(oldstickerEffectPath)) {
                    OrangeHelper.releaseSticker(oldstickerEffectPath);
                    isReleaseOldSticker = false;
                }
                OrangeHelper.enableSticker(stickerEffectPath, true);
            } else if (!TextUtils.isEmpty(oldstickerEffectPath)) {
                OrangeHelper.enableSticker(stickerEffectPath, false);
                stickerEffectPath = "";
            }

            OrangeHelper.ImageInfo imageInfo = new OrangeHelper.ImageInfo();
            imageInfo.deviceType = 0;
            imageInfo.facePointDir = 1;
            imageInfo.data = image.data;
            imageInfo.dir = image.dir;
            imageInfo.orientation = image.orientation;
            imageInfo.width = image.width;
            imageInfo.height = image.height;
            imageInfo.format = OrangeFilter.OF_PixelFormat_NV21;
            imageInfo.frontCamera = image.frontCamera;

            OrangeHelper.GLTexture OH_textureIn = new OrangeHelper.GLTexture();
            OH_textureIn.mHeight = textureIn.getHeight();
            OH_textureIn.mWidth = textureIn.getWidth();
            OH_textureIn.mTextureId = textureIn.getTextureId();

            OrangeHelper.GLTexture OH_textureOut = new OrangeHelper.GLTexture();
            OH_textureOut.mHeight = textureOut.getHeight();
            OH_textureOut.mWidth = textureOut.getWidth();
            OH_textureOut.mTextureId = textureOut.getTextureId();

            if (!OrangeHelper.updateFrameParams(OH_textureIn, OH_textureOut, imageInfo)) {
                mCameraView.copyTexture(textureIn, textureOut);
            }
        }

        @Override
        public void onInit() {
            boolean isCreateSucceed = OrangeHelper.createContext(MainActivity.this,
                    getResources().getString(R.string.orangefilter_serial_number), VENUS_ALL, null);
            Log.e(TAG, isCreateSucceed ? "onInit: 创建成功" : "onInit: 创建失败");

            if (isCreateSucceed) {
                initHazyBeautyOption();
                initClearBeautyOption();
                initFilterOption();
                initPlasticOption();
                initStickerOption();
            }
        }

        @Override
        public void onRelease() {
            Log.i(TAG, "onRelease");
            //release source
            if (mOFContext != 0) {
                OrangeHelper.destroyContext();
                mOFContext = 0;
            }

            if (mDialog != null) {
                mDialog.dismiss();
                mDialog = null;
            }
        }
    };

    @Override
    protected void onDestroy() {
        if (mCameraView != null) {
            mCameraView.onPause();
        }
        super.onDestroy();
    }
}