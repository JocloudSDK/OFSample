package com.yy.orangefilter.test.bean;

import com.yy.orangefilter.test.helper.OrangeHelper;

/**
 * 滤镜Bean
 */
public class FilterBean {
    private String optionName;
    private OrangeHelper.EffectParamType mParamType;
    private OrangeHelper.EffectParam of_paramf;
    private int thumb;
    private boolean isEnable = false;


    public FilterBean(String optionName,int thumb,
                      OrangeHelper.EffectParamType mParamType,OrangeHelper.EffectParam of_paramf) {
        this.optionName = optionName;
        this.thumb = thumb;
        this.mParamType = mParamType;
        this.of_paramf = of_paramf;
    }

    public String getOptionName() {
        return optionName;
    }

    public void setOptionName(String optionName) {
        this.optionName = optionName;
    }

    public boolean isEnable() {
        return isEnable;
    }

    public void setEnable(boolean enable) {
        isEnable = enable;
    }

    public int getThumb() {
        return thumb;
    }

    public OrangeHelper.EffectParamType getParamType() {
        return mParamType;
    }

    public void setParamType(OrangeHelper.EffectParamType paramType) {
        mParamType = paramType;
    }

    public OrangeHelper.EffectParam getOf_paramf() {
        return of_paramf;
    }

    public void setOf_paramf(OrangeHelper.EffectParam of_paramf) {
        this.of_paramf = of_paramf;
    }
}
