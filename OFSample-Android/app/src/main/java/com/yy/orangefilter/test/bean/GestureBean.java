package com.yy.orangefilter.test.bean;

/**
 * 手势贴纸Bean
 */
public class GestureBean {
    private String effectPath;//特效路径
    private int thumb;
    private boolean isEnable = false;//是否开启状态；

    public GestureBean(String effectPath, int thumb, boolean isEnable) {
        this.effectPath = effectPath;
        this.thumb = thumb;
        this.isEnable = isEnable;
    }

    public String getEffectPath() {
        return effectPath;
    }

    public void setEffectPath(String effectPath) {
        this.effectPath = effectPath;
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

    public void setThumb(int thumb) {
        this.thumb = thumb;
    }
}
