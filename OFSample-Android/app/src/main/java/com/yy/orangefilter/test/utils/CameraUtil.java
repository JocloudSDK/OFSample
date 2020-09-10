package com.yy.orangefilter.test.utils;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.Log;

import java.util.ArrayList;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class CameraUtil {
    private static final String TAG = "CameraUtil";

    private Context mContext;
    public CameraProxy mCameraProxy;
    private SurfaceTexture mSurfaceTexture;
    private int mCameraID = Camera.CameraInfo.CAMERA_FACING_FRONT;
    private final int[] mRequestPreviewSizes = new int[]{
            640, 360,
            1280, 720,
            640, 480
    };
    private ArrayList<Integer> mSupportedPreviewSizes;
    private int mImageWidth, mImageHeight;
    private byte[] mImageData;
    private byte[] mTmpBuffer;
    private HandlerThread mHandlerThread;
    private Handler mProcessHandler;
    private boolean mIsPaused = false;
    private static final int MESSAGE_PROCESS_IMAGE = 100;
    private SurfaceTexture.OnFrameAvailableListener mOnFrameAvailableListener;
    Lock mReadedImageLock = new ReentrantLock();
    ReadedImage mReadedImage = new ReadedImage();

    public static class ReadedImage {
        public byte[] data;
        public int dir;
        public int orientation;
        public int width;
        public int height;
        public boolean frontCamera;
        public boolean used = true;
    }

    public CameraUtil(Context context) {
        mContext = context;
        mCameraProxy = new CameraProxy(context);

        mHandlerThread = new HandlerThread("ProcessImageThread");
        mHandlerThread.start();
        mProcessHandler = new Handler(mHandlerThread.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == MESSAGE_PROCESS_IMAGE && !mIsPaused) {
                    mReadedImageLock.lock();
                    try {
                        if (mReadedImage.used) {
                            processImageData();
                            requestRender();
                        }
                    } finally {
                        mReadedImageLock.unlock();
                    }
                }
            }
        };
    }

    public void openCamera(int cameraId, SurfaceTexture surfaceTexture) {
        mCameraID = cameraId;
        if (mCameraProxy.getNumberOfCameras() == 1) {
            mCameraID = Camera.CameraInfo.CAMERA_FACING_BACK;
        }
        mSurfaceTexture = surfaceTexture;
        mCameraProxy.openCamera(mCameraID);
        mSupportedPreviewSizes = mCameraProxy.getSupportedPreviewSize(mRequestPreviewSizes);
        mImageWidth = mSupportedPreviewSizes.get(0);
        mImageHeight = mSupportedPreviewSizes.get(1);
        mCameraProxy.setPreviewSize(mImageWidth, mImageHeight);
        mCameraProxy.startPreview(mSurfaceTexture, mPreviewCallback);

        Log.w(TAG, "preview size: " + mImageWidth + "x" + mImageHeight);
    }

    public int getCameraID() {
        return mCameraID;
    }

    public void onResume() {
        mIsPaused = false;
        if (mCameraProxy.getCamera() == null) {
            if (mCameraProxy.getNumberOfCameras() == 1) {
                mCameraID = Camera.CameraInfo.CAMERA_FACING_BACK;
            }
            mCameraProxy.openCamera(mCameraID);
        }
    }

    public void onPause() {
        mIsPaused = true;
        mCameraProxy.releaseCamera();
    }

    private static int rotateYUV(byte[] dataIn, int width, int height, int degree, byte[] dataOut) {

        int k = 0;
        int height2 = (int) (1.5 * height);
        if (degree > 0) {
            //旋转Y部分
            for (int i = width - 1; i >= 0; --i) {
                for (int j = 0; j < height; ++j) {
                    dataOut[k] = dataIn[j * width + i];
                    ++k;
                }
            }
            //旋转UV部分
            for (int i = width - 1; i > 0; --i) {
                for (int j = height; j < height2; ++j) {
                    dataOut[k] = dataIn[j * width + i - 1];
                    ++k;
                    dataOut[k] = dataIn[j * width + i];
                    ++k;
                }
                --i;
            }
        } else {
            //旋转Y部分
            for (int i = 0; i < width; ++i) {
                for (int j = height - 1; j >= 0; --j) {
                    dataOut[k] = dataIn[j * width + i];
                    ++k;
                }
            }
            //旋转UV部分
            for (int i = 0; i < width; ++i) {
                for (int j = height2 - 1; j >= height; --j) {
                    dataOut[k] = dataIn[j * width + i];
                    ++k;
                    dataOut[k] = dataIn[j * width + i + 1];
                    ++k;
                }
                ++i;
            }
        }

        return 0;
    }

    private Camera.PreviewCallback mPreviewCallback = new Camera.PreviewCallback() {
        @Override
        public void onPreviewFrame(final byte[] data, Camera camera) {
            if (mImageData == null || mImageData.length != mImageHeight * mImageWidth * 3 / 2) {
                mImageData = new byte[mImageWidth * mImageHeight * 3 / 2];
            }
            synchronized (mImageData) {

                if (mCameraProxy.mContext.getResources().getConfiguration().orientation ==
                        Configuration.ORIENTATION_LANDSCAPE) {
                    rotateYUV(data, mImageWidth, mImageHeight,
                            mCameraProxy.isFrontCamera() ? -90 : 90, mImageData);
                } else {
                    System.arraycopy(data, 0, mImageData, 0, data.length);
                }
            }

            mProcessHandler.removeMessages(MESSAGE_PROCESS_IMAGE);
            mProcessHandler.sendEmptyMessage(MESSAGE_PROCESS_IMAGE);
        }
    };

    private void processImageData() {
        if (mTmpBuffer == null || mTmpBuffer.length != mImageHeight * mImageWidth * 3 / 2) {
            mTmpBuffer = new byte[mImageWidth * mImageHeight * 3 / 2];
        }

        synchronized (mImageData) {
            System.arraycopy(mImageData, 0, mTmpBuffer, 0, mImageData.length);
        }
/*
        // 如果使用前置摄像头，请注意显示的图像与帧图像左右对称，需处理坐标
        boolean frontCamera = (mCameraID == Camera.CameraInfo.CAMERA_FACING_FRONT);

        // 获取重力传感器返回的方向
        int dir = Accelerometer.getDirection();
        final int original_dir = dir;

        // 在使用后置摄像头，且传感器方向为 0 �?2 时，后置摄像头与前置 orientation 相反
        if (!frontCamera && dir == 0) {
            dir = 2;
        } else if (!frontCamera && dir == 2) {
            dir = 0;
        }

        // 请注意前置摄像头与后置摄像头旋转定义不同
        // 请注意不同手机摄像头旋转定义不同
        if (((mCameraProxy.getOrientation() == 270 && (dir & 1) == 1) ||
                (mCameraProxy.getOrientation() == 90 && (dir & 1) == 0))) {
            dir = (dir ^ 2);
        }
*/
        mReadedImage.used = false;
        mReadedImage.data = mTmpBuffer;
        mReadedImage.width = mImageWidth;
        mReadedImage.height = mImageHeight;
        mReadedImage.dir = Accelerometer.getDirection();
        mReadedImage.orientation = mCameraProxy.getOrientation();
        mReadedImage.frontCamera = mCameraID == Camera.CameraInfo.CAMERA_FACING_FRONT;
    }

    public void useReadedImage(ReadedImage target) {
        mReadedImageLock.lock();
        try {
            if (mReadedImage.data != null) {
                if (target.data == null || target.data.length < mReadedImage.data.length) {
                    target.data = new byte[mReadedImage.data.length];
                }
                System.arraycopy(mReadedImage.data, 0, target.data, 0, mReadedImage.data.length);
                target.width = mReadedImage.width;
                target.height = mReadedImage.height;
                target.dir = mReadedImage.dir;
                target.orientation = mReadedImage.orientation;
                target.frontCamera = mReadedImage.frontCamera;
                mReadedImage.used = true;
            }
        } finally {
            mReadedImageLock.unlock();
        }
    }

    private void requestRender() {
        if (mOnFrameAvailableListener != null) {
            mOnFrameAvailableListener.onFrameAvailable(mSurfaceTexture);
        }
    }

    public void setOnFrameAvailableListener(SurfaceTexture.OnFrameAvailableListener listener) {
        mOnFrameAvailableListener = listener;
    }
}
