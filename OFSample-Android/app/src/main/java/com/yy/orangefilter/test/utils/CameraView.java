package com.yy.orangefilter.test.utils;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.AttributeSet;
import android.util.Log;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

public class CameraView extends GLSurfaceView implements GLSurfaceView.Renderer {
    private static final String TAG = "CameraView";

    private Accelerometer mAccelerometer = null;
    private CameraUtil mCamera;
    private CameraUtil.ReadedImage mReadedImage = new CameraUtil.ReadedImage();
    private GLTexture mOESTexture;
    private SurfaceTexture mSurfaceTexture;
    private boolean mCameraOpening;
    private QuadRenderer mQuadRendererOES;
    private QuadRenderer mQuadRenderer;
    private DrawFrameCallback mDrawFrameCallback;
    private int mViewWidth;
    private int mViewHeight;
    private int mRenderWidth;
    private int mRenderHeight;
    private int mViewRenderHeight;
    private int mViewRenderWidth;
    private GLTexture mTextureIn;
    private GLTexture mTextureOut;
    private int mFrameBuffer;
    private float[] mIdentityMatrix;
    private float[] mFlipYMatrix;
    private static int mGLContextVersion = 0;

    public CameraView(Context context) {
        this(context, null);
    }

    public CameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {
        private static final int EGL_OPENGL_ES2_BIT  = 0x0004;
        private static final int EGL_OPENGL_ES3_BIT  = 0x0040;

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
            Log.i(TAG, "egl chooseConfig");

            int[] attribs = {
                EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                EGL10.EGL_RED_SIZE, 8,
                EGL10.EGL_GREEN_SIZE, 8,
                EGL10.EGL_BLUE_SIZE, 8,
                EGL10.EGL_NONE
            };

            int[] countArray = new int[1];
            if (egl.eglGetConfigs(display, null, 0, countArray)) {
                int count = countArray[0];
                if (count > 0) {
                    EGLConfig[] configs = new EGLConfig[count];

                    // try es3
                    boolean success = egl.eglChooseConfig(display, attribs, configs, configs.length, countArray);
                    int err = egl.eglGetError();
                    if (err != EGL10.EGL_SUCCESS) {
                        Log.e(TAG, "egl chooseConfig error for es3: " + err);
                    }
                    if (success && countArray[0] > 0) {
                        Log.w(TAG, "egl chooseConfig: es3");
                        return configs[0];
                    }

                    // es2
                    attribs[1] = EGL_OPENGL_ES2_BIT;
                    success = egl.eglChooseConfig(display, attribs, configs, configs.length, countArray);
                    err = egl.eglGetError();
                    if (err != EGL10.EGL_SUCCESS) {
                        Log.e(TAG, "egl chooseConfig error for es2: " + err);
                    }
                    if (success && countArray[0] > 0) {
                        Log.w(TAG, "egl chooseConfig: es2");
                        return configs[0];
                    }
                }
            }

            Log.e(TAG, "egl chooseConfig: none");
            return null;
        }
    }

    private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
        private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config) {
            Log.i(TAG, "egl createContext");

            int[] attribs = {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL10.EGL_NONE
            };

            // es3
            EGLContext context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attribs);
            int err = egl.eglGetError();
            if (err != EGL10.EGL_SUCCESS) {
                Log.e(TAG, "egl createContext error for es3: " + err);
            }
            if (context != null && context != EGL10.EGL_NO_CONTEXT) {
                mGLContextVersion = 3;
                Log.w(TAG, "egl createContext: es3");
                return context;
            }

            // es2
            attribs[1] = 2;
            context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attribs);
            err = egl.eglGetError();
            if (err != EGL10.EGL_SUCCESS) {
                Log.e(TAG, "egl createContext error for es2: " + err);
            }
            if (context != null && context != EGL10.EGL_NO_CONTEXT) {
                mGLContextVersion = 2;
                Log.w(TAG, "egl createContext: es2");
                return context;
            }

            Log.e(TAG, "egl createContext: none");
            return null;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }

    private void init() {
        mAccelerometer = new Accelerometer(getContext());
        mCamera = new CameraUtil(getContext());
        mRenderWidth = 0;
        mRenderHeight = 0;
        mFrameBuffer = 0;

        mIdentityMatrix = new float[16];
        Matrix.setIdentityM(mIdentityMatrix, 0);

        mFlipYMatrix = new float[16];
        Matrix.setIdentityM(mFlipYMatrix, 0);
        Matrix.scaleM(mFlipYMatrix, 0, 1f, -1, 1);

        setEGLConfigChooser(new ConfigChooser());
        setEGLContextFactory(new ContextFactory());
        setPreserveEGLContextOnPause(true);
        setRenderer(this);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    private void release() {
        Log.d(TAG, "release");

        if (mDrawFrameCallback != null) {
            mDrawFrameCallback.onRelease();
        }

        if (mOESTexture != null) {
            mOESTexture.release();
            mOESTexture = null;

            mSurfaceTexture.release();
            mSurfaceTexture = null;

            mQuadRendererOES.release();
            mQuadRendererOES = null;

            mQuadRenderer.release();
            mQuadRenderer = null;

            if (mTextureIn != null) {
                mTextureIn.release();
                mTextureIn = null;
            }

            if (mTextureOut != null) {
                mTextureOut.release();
                mTextureOut = null;
            }

            if (mFrameBuffer != 0) {
                GLES20.glDeleteFramebuffers(1, new int[]{mFrameBuffer}, 0);
                mFrameBuffer = 0;
            }
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        mAccelerometer.start();
        mCamera.onResume();
    }

    @Override
    public void onPause() {
        mAccelerometer.stop();
        mCamera.onPause();

        queueEvent(new Runnable() {
            @Override
            public void run() {
                release();
            }
        });

        super.onPause();
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        Log.w(TAG, "onSurfaceCreated GLES Version: " + GLES20.glGetString(GLES20.GL_VERSION));
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) {
        Log.w(TAG, "onSurfaceChanged: " + w + " " + h);

        mViewWidth = w;
        mViewHeight = h;

        float viewFactor = mViewWidth / (float) mViewHeight;
        float renderFactor = mRenderWidth / (float) mRenderHeight;
        mViewRenderHeight = mViewHeight;
        mViewRenderWidth = mViewWidth;
        if (viewFactor < renderFactor - 0.000001) {
            mViewRenderWidth = (int) (mViewWidth * (renderFactor / viewFactor));
        } else if (viewFactor > renderFactor + 0.000001) {
            mViewRenderHeight = (int) (mViewHeight * (viewFactor / renderFactor));
        }

        if (mOESTexture == null) {
            mOESTexture = new GLTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES);

            mSurfaceTexture = new SurfaceTexture(mOESTexture.getTextureId());
            mCamera.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
                @Override
                public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                    CameraView.this.requestRender();
                    mCameraOpening = false;
                }
            });

            mCameraOpening = false;
            mQuadRendererOES = new QuadRenderer(true);
            mQuadRenderer = new QuadRenderer(false);

            if (mRenderWidth > 0 && mRenderHeight > 0) {
                if (mTextureIn == null) {
                    mTextureIn = new GLTexture(GLES20.GL_TEXTURE_2D);
                    mTextureIn.texImage(mRenderWidth, mRenderHeight);
                }
                if (mTextureOut == null) {
                    mTextureOut = new GLTexture(GLES20.GL_TEXTURE_2D);
                    mTextureOut.texImage(mRenderWidth, mRenderHeight);
                }

                int[] fbos = new int[1];
                GLES20.glGenFramebuffers(1, fbos, 0);
                mFrameBuffer = fbos[0];
            }

            if (mDrawFrameCallback != null) {
                mDrawFrameCallback.onInit();
            }
        }

        if (!mCameraOpening) {
            mCameraOpening = true;

            mCamera.openCamera(Camera.CameraInfo.CAMERA_FACING_FRONT, mSurfaceTexture);
        }
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        mSurfaceTexture.updateTexImage();

        boolean frontCamera = mCamera.getCameraID() == Camera.CameraInfo.CAMERA_FACING_FRONT;

        float[] transform = new float[16];
        Matrix.setIdentityM(transform, 0);

        if (frontCamera) {
            // rotate 90 ccw
            if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT){
                Matrix.rotateM(transform, 0, 90, 0, 0, 1);
            }

            // flip x
            float[] flipX = new float[16];
            Matrix.setIdentityM(flipX, 0);
            Matrix.scaleM(flipX, 0, -1f, 1, 1);
            multiplyMatrixRight(flipX, transform);
        } else {
            // rotate -90 ccw
            if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
                Matrix.rotateM(transform, 0, -90, 0, 0, 1);
            }
        }

        mCamera.useReadedImage(mReadedImage);

        if (mDrawFrameCallback != null) {
            // get default fbo
            int[] fbos = new int[1];
            GLES20.glGetIntegerv(GLES20.GL_FRAMEBUFFER_BINDING, fbos, 0);
            int defaultFBO = fbos[0];

            multiplyMatrixRight(mFlipYMatrix, transform);

            // draw oes texture to in texture
            bindFrameBuffer(mFrameBuffer, mTextureIn);
            setViewportAndClear(mTextureIn.getWidth(), mTextureIn.getHeight());
            mQuadRendererOES.draw(transform, mOESTexture.getTextureId());

            // apply draw callback
            mDrawFrameCallback.onDrawFrame(mTextureIn, mTextureOut, mReadedImage);

            // draw out texture to surface view
            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, defaultFBO);
            setViewportAndClear(mViewRenderWidth, mViewRenderHeight);
            mQuadRenderer.draw(mIdentityMatrix, mTextureOut.getTextureId());

            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, defaultFBO);
        } else {
            // draw oes texture to surface view
            setViewportAndClear(mViewWidth, mViewHeight);
            mQuadRendererOES.draw(transform, mOESTexture.getTextureId());
        }
    }

    public void copyTexture(GLTexture textureIn, GLTexture textureOut) {
        bindFrameBuffer(mFrameBuffer, textureOut);
        setViewportAndClear(textureOut.getWidth(), textureOut.getHeight());
        mQuadRenderer.draw(mFlipYMatrix, textureIn.getTextureId());
    }

    private void setViewportAndClear(int w, int h) {
        GLES20.glViewport(0, 0, w, h);
        GLES20.glClearColor(0, 0, 0, 1);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
    }

    private void bindFrameBuffer(int fbo, GLTexture texture) {
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, fbo);
        GLES20.glFramebufferTexture2D(
            GLES20.GL_FRAMEBUFFER,
            GLES20.GL_COLOR_ATTACHMENT0,
            texture.getTarget(),
            texture.getTextureId(),
            0);

        int status = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
        if (status != GLES20.GL_FRAMEBUFFER_COMPLETE) {
            Log.e(TAG, "bindFrameBuffer failed status: " + status);
        }
    }

    private void multiplyMatrixRight(float[] left, float[] right) {
        float[] result = new float[16];
        Matrix.multiplyMM(result, 0, left, 0, right, 0);
        System.arraycopy(result, 0, right, 0, result.length);
    }

    private void transformMatrix(float[] transform) {
        float[] result = new float[16];
        Matrix.invertM(result, 0, transform, 0);

        result[12] = 0;
        result[13] = 0;
        result[14] = 0;

        float[] flipX = new float[16];
        Matrix.setIdentityM(flipX, 0);
        Matrix.scaleM(flipX, 0, -1f, 1, 1);

        multiplyMatrixRight(flipX, result);

        System.arraycopy(result, 0, transform, 0, result.length);
    }

    public interface DrawFrameCallback {
        void onDrawFrame(GLTexture textureIn, GLTexture textureOut, CameraUtil.ReadedImage image);

        void onInit();

        void onRelease();
    }

    public void setDrawFrameCallback(int renderWidth, int renderHeight, DrawFrameCallback callback) {
        mRenderWidth = renderWidth;
        mRenderHeight = renderHeight;
        mDrawFrameCallback = callback;
    }
}
