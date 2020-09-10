package com.yy.orangefilter.test.utils;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class CameraProxy {

    private static final String TAG = "CameraProxy";
    private boolean isDebug = true;

    public Context mContext;
    private int mCameraId;
    private Camera mCamera;

    private SurfaceTexture mSurfaceTexture;

    private int mCameraDirection;

    private CameraInfo mCameraInfo = new CameraInfo();

    public CameraProxy(Context context) {
        mContext = context;
    }

    public Camera getCamera() {
        return mCamera;
    }

    public boolean openCamera(int cameraId) {
        try {
            releaseCamera();
            mCamera = Camera.open(cameraId);
            mCamera.getParameters();
            mCameraId = cameraId;
            mCamera.getCameraInfo(cameraId, mCameraInfo);

            setDefaultParameters();
        } catch (Exception e) {
            mCamera = null;
            Log.i(TAG, "openCamera fail msg=" + e.getMessage());
            return false;
        }
        return true;
    }

    public void releaseCamera() {
        if (mCamera != null) {
            mCamera.setPreviewCallback(null);
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
    }

    public void startPreview(SurfaceTexture surfaceTexture, PreviewCallback previewcallback) {
        try {
            mCamera.setPreviewTexture(surfaceTexture);
            if (previewcallback != null) {
                mCamera.setPreviewCallback(previewcallback);
            }
            mCamera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void startPreview() {
        if (mCamera != null) {
            mCamera.startPreview();
        }
    }

    public void stopPreview() {
        if (mCamera != null) {
            mCamera.stopPreview();
        }
    }

    public Size getPreviewSize() {
        if (mCamera != null) {
            return mCamera.getParameters().getPreviewSize();
        }
        return null;
    }

    public void addPreviewCallbackBuffer(byte[] callbackBuffer) {
        mCamera.addCallbackBuffer(callbackBuffer);
    }

    public int getOrientation() {
        if (mCameraInfo == null) {
            return 0;
        }
        return mCameraInfo.orientation;
    }

    public boolean isFlipHorizontal() {
        if (mCameraInfo == null) {
            return false;
        }
        return mCameraInfo.facing == CameraInfo.CAMERA_FACING_FRONT ? true : false;
    }

    public int getCameraId() {
        return mCameraId;
    }

    public boolean isFrontCamera() {
        return mCameraId == CameraInfo.CAMERA_FACING_FRONT;
    }

    public void setRotation(int rotation) {
        if (mCamera != null) {
            Camera.Parameters params = mCamera.getParameters();
            params.setRotation(rotation);
            mCamera.setParameters(params);
        }
    }

    public int getDisplayOrientation(int dir) {
        /**
         * 请注意前置摄像头与后置摄像头旋转定义不同
         * 请注意不同手机摄像头旋转定义不同
         */
        int newdir = dir;
        if (isFrontCamera() &&
                ((mCameraInfo.orientation == 270 && (dir & 1) == 1) ||
                        (mCameraInfo.orientation == 90 && (dir & 1) == 0))) {
            newdir = (dir ^ 2);
        }
        return newdir;
    }

    public boolean needMirror() {
        return isFrontCamera();
    }

    private void setDefaultParameters() {
        Parameters parameters = mCamera.getParameters();
        Log.i(TAG, "parameters: " + parameters.flatten());
        if (parameters.getSupportedFocusModes()
                .contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)) {
            parameters.setFocusMode(Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        }
        List<String> flashModes = parameters.getSupportedFlashModes();
        if (flashModes != null && flashModes.contains(Parameters.FLASH_MODE_OFF)) {
            parameters.setFlashMode(Parameters.FLASH_MODE_OFF);
        }

        mCamera.setParameters(parameters);
    }

    public Parameters getParameters() {
        return mCamera.getParameters();
    }

    public void setPreviewSize(int width, int height) {
        if (mCamera == null) {
            return;
        }
        Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(width, height);
        mCamera.setParameters(parameters);
    }

    public ArrayList<Integer> getSupportedPreviewSize(int[] previewSizes) {
        ArrayList<Integer> result = new ArrayList<Integer>();
        if (mCamera != null) {
            List<Size> sizes = mCamera.getParameters().getSupportedPreviewSizes();

            for (Size s : sizes) {
                Log.i(TAG, "getSupportedPreviewSize: " + s.width + "x" + s.height);
            }

            for (int i = 0; i < previewSizes.length; i += 2) {
                int width = previewSizes[i];
                int height = previewSizes[i + 1];
                for (Size s : sizes) {
                    if ((s.width == width) && (s.height == height)) {
                        result.add(width);
                        result.add(height);
                    }
                }
            }
        }
        if (result.isEmpty()) {
            Size preferredSize = mCamera.getParameters().getPreferredPreviewSizeForVideo();
            result.add(preferredSize.width);
            result.add(preferredSize.height);
        }
        return result;
    }

    public int getNumberOfCameras() {
        return Camera.getNumberOfCameras();
    }

    public void setDisplayOrientation(int mode) {
        mCamera.setDisplayOrientation(mode);
    }
}
