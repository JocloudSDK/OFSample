#import "CameraUtil.h"

@interface CameraUtil () <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    CVPixelBufferRef _pixelBuffer;
    dispatch_queue_t _queue;
}

@property (nonatomic) AVCaptureSession* cameraSession;
@property (nonatomic) AVCaptureDevice* cameraDevice;
@property (nonatomic) AVCaptureDeviceInput* cameraInput;
@property (nonatomic) AVCaptureVideoDataOutput* cameraOutput;
@property (nonatomic, assign) PreViewDirection direction;

@end

@implementation CameraUtil

- (CVPixelBufferRef) getCameraPixelBuffer
{
    return _pixelBuffer;
}

- (void) releasePixelBuffer
{
    if (_pixelBuffer)
    {
        CVPixelBufferRelease(_pixelBuffer);
        _pixelBuffer = nullptr;
    }
}

- (id)initWithCaptureSize:(AVCaptureSessionPreset)size direction:(PreViewDirection)direction
{
    self = [super init];
    
    _pixelBuffer = nullptr;
    _queue = nullptr;
    _direction = direction;
    
    self.cameraSession = [[AVCaptureSession alloc] init];
    [self.cameraSession setSessionPreset:size];
    
    [self selectCamera:true];
    
    return self;
}

- (void)dealloc
{
    if (_pixelBuffer)
    {
        CVPixelBufferRelease(_pixelBuffer);
        _pixelBuffer = nullptr;
    }
}

- (void)selectCamera:(BOOL)front
{
    if ([self.cameraSession isRunning])
    {
        [self.cameraSession stopRunning];
    }
    if (self.cameraInput)
    {
        [self.cameraSession removeInput:self.cameraInput];
    }
    if (self.cameraOutput)
    {
        [self.cameraSession removeOutput:self.cameraOutput];
    }
    
    [self.cameraSession beginConfiguration];
    
    NSArray<AVCaptureDevice*>* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice* device in devices)
    {
        if (device.position == (front ? AVCaptureDevicePositionFront : AVCaptureDevicePositionBack))
        {
            self.cameraDevice = device;
            break;
        }
    }
    
    self.cameraInput = [AVCaptureDeviceInput deviceInputWithDevice:self.cameraDevice error:nil];
    [self.cameraSession addInput:self.cameraInput];
    
    self.cameraOutput = [[AVCaptureVideoDataOutput alloc] init];
    [self.cameraSession addOutput:self.cameraOutput];
    
    AVCaptureConnection* connection = [self.cameraOutput connectionWithMediaType:AVMediaTypeVideo];
    [connection setVideoMirrored:front];
    if (self.direction == PreViewDirection_Portrait) {
        [connection setVideoOrientation:AVCaptureVideoOrientationPortrait ];
    } else if (self.direction == PreViewDirection_LandscapeRight) {
        [connection setVideoOrientation:AVCaptureVideoOrientationLandscapeRight];
    }
    [self.cameraOutput setAlwaysDiscardsLateVideoFrames:YES];
    [self.cameraOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
//    [self.cameraOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    if (!_queue)
    {
        _queue = dispatch_queue_create("video_cap", NULL);
    }
    [self.cameraOutput setSampleBufferDelegate:self queue:_queue];
    
    [self.cameraDevice setActiveVideoMaxFrameDuration:CMTimeMake(1, 24)];
    [self.cameraDevice setActiveVideoMinFrameDuration:CMTimeMake(1, 24)];
    
    [self.cameraSession commitConfiguration];
    [self.cameraSession startRunning];
}

- (void)focusAuto
{
    NSError *error;
    if ([self.cameraDevice lockForConfiguration:&error])
    {
        if ([self.cameraDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus])
        {
            [self.cameraDevice setFocusPointOfInterest:CGPointMake(0.5, 0.5)];
            [self.cameraDevice setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
        }
        
        if ([self.cameraDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure ])
        {
            [self.cameraDevice setExposurePointOfInterest:CGPointMake(0.5, 0.5)];
            [self.cameraDevice setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
        }
        
        [self.cameraDevice unlockForConfiguration];
    }
}

- (void)focusAtPoint:(CGPoint)point
{
    NSError *error;
    if ([self.cameraDevice lockForConfiguration:&error])
    {
        // Focus mode and focus point
        if ([self.cameraDevice isFocusModeSupported:AVCaptureFocusModeAutoFocus])
        {
            [self.cameraDevice setFocusPointOfInterest:point];
            [self.cameraDevice setFocusMode:AVCaptureFocusModeAutoFocus];
        }
        // Exposure mode and exposure point
        if ([self.cameraDevice isExposureModeSupported:AVCaptureExposureModeAutoExpose ])
        {
            [self.cameraDevice setExposurePointOfInterest:point];
            [self.cameraDevice setExposureMode:AVCaptureExposureModeAutoExpose];
        }
        
        [self.cameraDevice unlockForConfiguration];
    }
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if(!_pixelBuffer){
        CFRetain(sampleBuffer);
        _pixelBuffer = (CVPixelBufferRef)CMSampleBufferGetImageBuffer(sampleBuffer);
    }
}

@end
