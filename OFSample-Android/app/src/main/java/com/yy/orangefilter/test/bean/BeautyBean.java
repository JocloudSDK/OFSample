package com.yy.orangefilter.test.bean;

import com.yy.orangefilter.test.helper.OrangeHelper;

/**
 * 美颜、整形Bean
 */
public class BeautyBean {
    private int thumb;//图标
    private String optionName;
    private OrangeHelper.EffectParamType mParamType;
    private OrangeHelper.EffectParam of_paramf;
    private boolean isEnable = false;

    public BeautyBean(String optionName,
                      OrangeHelper.EffectParam of_paramf, OrangeHelper.EffectParamType paramType, int thumb) {
        this.optionName = optionName;
        this.of_paramf = of_paramf;
        this.mParamType = paramType;
        this.thumb = thumb;
    }

    public int getThumb() {
        return thumb;
    }

    public String getOptionName() {
        return optionName;
    }

    public OrangeHelper.EffectParam getOf_paramf() {
        return of_paramf;
    }

    public OrangeHelper.EffectParamType getParamType() {
        return mParamType;
    }

    public boolean isEnable() {
        return isEnable;
    }

    public void setEnable(boolean enable) {
        isEnable = enable;
    }
}
