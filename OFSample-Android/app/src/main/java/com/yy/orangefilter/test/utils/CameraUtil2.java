package com.yy.orangefilter.test.utils;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.MeteringRectangle;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.media.ImageReader.OnImageAvailableListener;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Size;
import android.view.Surface;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

public class CameraUtil2 {
    private static final String TAG = "CameraUtil";

    public static final int BACK_CAMERA = 0;
    public static final int FRONT_CAMERA = 1;

    private Activity mActivity;
    private SurfaceTexture mSurfaceTexture;
    private CameraDevice mCameraDevice;
    private int mSensorOrientation;
    private CameraCharacteristics mCameraCharacteristics;
    private Size mPreviewSize;
    private CaptureRequest.Builder mPreviewBuilder;
    private HandlerThread mBackgroundThread;
    private Handler mBackgroundHandler;
    private CameraCaptureSession mPreviewSession;
    private ImageReader mImageReader;
    private ImageReadCallback mImageReadCallback;

    public CameraUtil2(Activity activity) {
        mActivity = activity;
    }

    public void openCamera(int cameraIndex, int width, int height, SurfaceTexture surfaceTexture) {
        if (mCameraDevice != null) {
            closeCamera();
        }

        mSurfaceTexture = surfaceTexture;

        CameraManager manager = (CameraManager) mActivity.getSystemService(Context.CAMERA_SERVICE);
        if (manager == null) {
            return;
        }

        try {
            String[] cameras = manager.getCameraIdList();
            String cameraId;
            if (cameras.length >= 2) {
                cameraId = cameras[cameraIndex];
            } else if (cameras.length == 1) {
                cameraId = cameras[BACK_CAMERA];
            } else {
                return;
            }

            mCameraCharacteristics = manager.getCameraCharacteristics(cameraId);
            StreamConfigurationMap map = mCameraCharacteristics.get(
                CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP
            );
            mSensorOrientation = mCameraCharacteristics.get(CameraCharacteristics.SENSOR_ORIENTATION);

            mPreviewSize = chooseOptimalSize(map.getOutputSizes(
                SurfaceTexture.class), width, height, new Size(width, height)
            );
            if (mPreviewSize == null) {
                mPreviewSize = chooseOptimalSize(
                        map.getOutputSizes(SurfaceTexture.class),
                        height,
                        width,
                        new Size(height, width)
                );
            }

            if (ActivityCompat.checkSelfPermission(mActivity, Manifest.permission.CAMERA) !=
                    PackageManager.PERMISSION_GRANTED) {
                return;
            }

            manager.openCamera(cameraId, mStateCallback, mBackgroundHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    public void onResume() {
        startBackgroundThread();
    }

    public void onPause() {
        closeCamera();
        stopBackgroundThread();
    }

    public int getSensorOrientation() {
        return mSensorOrientation;
    }

    private void closeCamera() {
        closePreviewSession();
        if (mCameraDevice != null) {
            mCameraDevice.close();
            mCameraDevice = null;
        }
        closeImageReader();
    }

    private static Size chooseOptimalSize(Size[] choices, int width, int height, Size aspectRatio) {
        // Collect the supported resolutions that are at least as big as the preview Surface
        List<Size> bigEnough = new ArrayList<>();
        int w = aspectRatio.getWidth();
        int h = aspectRatio.getHeight();
        for (Size option : choices) {
            if (option.getHeight() == option.getWidth() * h / w &&
                    option.getWidth() >= width &&
                    option.getHeight() >= height) {
                bigEnough.add(option);
            }
        }

        // Pick the smallest of those, assuming we found any
        if (bigEnough.size() > 0) {
            return Collections.min(bigEnough, new CompareSizesByArea());
        }

        return null;
    }

    static class CompareSizesByArea implements Comparator<Size> {
        @Override
        public int compare(Size lhs, Size rhs) {
            // We cast here to ensure the multiplications won't overflow
            return Long.signum((long) lhs.getWidth() * lhs.getHeight() - (long) rhs.getWidth() * rhs.getHeight());
        }
    }

    private CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(@NonNull CameraDevice cameraDevice) {
            mCameraDevice = cameraDevice;
            startPreview();
        }

        @Override
        public void onDisconnected(@NonNull CameraDevice cameraDevice) {
            cameraDevice.close();
            mCameraDevice = null;
        }

        @Override
        public void onError(@NonNull CameraDevice cameraDevice, int error) {
            cameraDevice.close();
            mCameraDevice = null;
        }
    };

    private void startBackgroundThread() {
        mBackgroundThread = new HandlerThread("CameraUtilBackground");
        mBackgroundThread.start();
        mBackgroundHandler = new Handler(mBackgroundThread.getLooper());
    }

    private void stopBackgroundThread() {
        mBackgroundThread.quitSafely();
        try {
            mBackgroundThread.join();
            mBackgroundThread = null;
            mBackgroundHandler = null;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void startPreview() {
        try {
            closePreviewSession();

            mSurfaceTexture.setDefaultBufferSize(mPreviewSize.getWidth(), mPreviewSize.getHeight());
            mPreviewBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);

            ArrayList<Surface> surfaces = new ArrayList<>();

            Surface previewSurface = new Surface(mSurfaceTexture);
            mPreviewBuilder.addTarget(previewSurface);
            surfaces.add(previewSurface);

            if (mImageReadCallback != null) {
                closeImageReader();

                mImageReader = ImageReader.newInstance(
                        mPreviewSize.getWidth(),
                        mPreviewSize.getHeight(),
                        ImageFormat.YUV_420_888,
                        2
                );
                mImageReader.setOnImageAvailableListener(new OnImageAvailableListener() {
                    @Override
                    public void onImageAvailable(ImageReader imageReader) {
                        Image image = imageReader.acquireLatestImage();
                        if (image != null) {
                            mImageReadCallback.onReadImage(image);
                            image.close();
                        }
                    }
                }, mBackgroundHandler);
                mPreviewBuilder.addTarget(mImageReader.getSurface());
                surfaces.add(mImageReader.getSurface());
            }

            mCameraDevice.createCaptureSession(
                    surfaces,
                    new CameraCaptureSession.StateCallback() {
                        @Override
                        public void onConfigured(@NonNull CameraCaptureSession session) {
                            mPreviewSession = session;
                            updatePreview();
                        }

                        @Override
                        public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                            Toast.makeText(mActivity, "Failed", Toast.LENGTH_SHORT).show();
                        }
                    },
                    mBackgroundHandler
            );
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private void closePreviewSession() {
        if (mPreviewSession != null) {
            mPreviewSession.close();
            mPreviewSession = null;
        }
    }

    private void closeImageReader() {
        if (mImageReader != null) {
            mImageReader.close();
            mImageReader = null;
        }
    }

    private void updatePreview() {
        try {
            setUpCaptureRequestBuilder(mPreviewBuilder);
            HandlerThread thread = new HandlerThread("CameraPreview");
            thread.start();
            mPreviewSession.setRepeatingRequest(mPreviewBuilder.build(), null, mBackgroundHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private void setUpCaptureRequestBuilder(CaptureRequest.Builder builder) {
        builder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
    }

    public void touchFocus(PointF pos, int size) {
        if (mCameraCharacteristics == null) {
            return;
        }

        Rect rect = mCameraCharacteristics.get(CameraCharacteristics.SENSOR_INFO_ACTIVE_ARRAY_SIZE);

        int x = (int) (rect.width() * pos.y);
        int y = (int) (rect.height() * (1.0f - pos.x));
        int left = x - size / 2;
        if (left < rect.left) {
            left = rect.left;
        }
        int right = x + size / 2;
        if (right > rect.right) {
            right = rect.right;
        }
        int top = y - size / 2;
        if (top < rect.top) {
            top = rect.top;
        }
        int bottom = y + size / 2;
        if (bottom > rect.bottom) {
            bottom = rect.bottom;
        }

        Rect meterRect = new Rect(left, top, right, bottom);

        if (mPreviewBuilder == null || mPreviewSession == null) {
            return;
        }

        MeteringRectangle[] rectangle = new MeteringRectangle[]{
                new MeteringRectangle(meterRect, MeteringRectangle.METERING_WEIGHT_MAX)
        };

        /*
        mPreviewBuilder.set(CaptureRequest.CONTROL_AF_REGIONS, rectangle);
        mPreviewBuilder.set(CaptureRequest.CONTROL_AE_REGIONS, rectangle);
        mPreviewBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
        mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_AUTO);
        mPreviewBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CameraMetadata.CONTROL_AF_TRIGGER_START);
        mPreviewBuilder.set(
            CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER,
            CameraMetadata.CONTROL_AE_PRECAPTURE_TRIGGER_START
        );
        try {
            mPreviewSession.capture(mPreviewBuilder.build(), null, mBackgroundHandler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }

        Log.e(TAG, "touchFocus");
        */
    }

    public interface ImageReadCallback {
        void onReadImage(Image image);
    }

    public void setImageReadCallback(ImageReadCallback callback) {
        mImageReadCallback = callback;
    }
}
