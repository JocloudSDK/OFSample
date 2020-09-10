#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>

typedef NS_ENUM(NSInteger, PreViewDirection) {
    PreViewDirection_Portrait = 0,    // Portrait
    PreViewDirection_LandscapeRight,   // LandscapeRight
};

@interface CameraUtil : NSObject

- (id)initWithCaptureSize:(AVCaptureSessionPreset)size direction:(PreViewDirection)direction;
- (CVPixelBufferRef) getCameraPixelBuffer;
- (void)releasePixelBuffer;
- (void)focusAuto;
- (void)focusAtPoint:(CGPoint)point;

@end
