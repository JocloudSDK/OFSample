package com.yy.orangefilter.test.utils;

import android.opengl.GLES20;

public class GLTexture {
    private int mTextureId;
    private int mTarget;
    private int mWidth;
    private int mHeight;
    private int mFormat;

    public GLTexture(int target) {
        int[] tex = new int[1];
        GLES20.glGenTextures(1, tex, 0);
        mTextureId = tex[0];
        mTarget = target;
        mWidth = 0;
        mHeight = 0;

        GLES20.glBindTexture(mTarget, mTextureId);
        GLES20.glTexParameterf(mTarget, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(mTarget, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(mTarget, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameterf(mTarget, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glBindTexture(mTarget, 0);
    }

    public void texImage(int w, int h) {
        mWidth = w;
        mHeight = h;
        mFormat = GLES20.GL_RGBA;

        GLES20.glBindTexture(mTarget, mTextureId);
        GLES20.glTexImage2D(mTarget, 0, mFormat, mWidth, mHeight, 0, mFormat, GLES20.GL_UNSIGNED_BYTE, null);
        GLES20.glBindTexture(mTarget, 0);
    }

    public void release() {
        GLES20.glDeleteTextures(1, new int[] { mTextureId }, 0);
    }

    public int getTextureId() {
        return mTextureId;
    }

    public int getTarget() {
        return mTarget;
    }

    public int getFormat() {
        return mFormat;
    }

    public int getWidth() {
        return mWidth;
    }

    public int getHeight() {
        return mHeight;
    }
}
