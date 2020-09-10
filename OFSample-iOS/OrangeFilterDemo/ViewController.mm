#import "ViewController.h"
#import <OpenGLES/ES3/gl.h>
#include "EffectRender.h"
#import <CoreMotion/CoreMotion.h>
#import <Accelerate/Accelerate.h>
#import "EffectsView.h"
#import "OrangeHelper.h"
#import "EffectModel.h"
#import "NSBundle+LanguageAdd.h"

#define SCREEN_WIDTH ([UIScreen mainScreen].bounds.size.width)
#define SCREEN_HEIGHT ([UIScreen mainScreen].bounds.size.height)

@interface ViewController ()<UIGestureRecognizerDelegate, EffectsViewDelegate> {
    EAGLContext* _context;
    CameraUtil* _cameraUtil;
    CGSize _captureSize;
    EffectRender* _render;
    UIImageView* _focusImage;
    OrangeHelper* _ofHelper;
}

@property (nonatomic, assign) int rotation;
@property (nonatomic, strong) CMMotionManager *motionManager;

@property (weak, nonatomic) IBOutlet NSLayoutConstraint *backViewHeightConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *stackViewLeftConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *stackViewRightConstraint;
@property (weak, nonatomic) IBOutlet UIView *backView;
@property (nonatomic, strong) EffectsView *effectsView;
@property (weak, nonatomic) IBOutlet UIButton *backBtn;
@property (weak, nonatomic) IBOutlet UIButton *hazyBtn;
@property (weak, nonatomic) IBOutlet UIButton *clearBtn;
@property (weak, nonatomic) IBOutlet UIButton *faceBtn;
@property (weak, nonatomic) IBOutlet UIButton *filterBtn;
@property (weak, nonatomic) IBOutlet UIButton *stickerBtn;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self initUI];
    [self initGLContext];
    [self initRender];
    
    self.motionManager = [[CMMotionManager alloc] init];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appDidBecomeActive)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
    
    // Set device orientation
    if (self.direction == PreViewDirection_Portrait) {
        NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    } else if (self.direction == PreViewDirection_LandscapeRight) {
        NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeRight];
        [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    // Update constraint
    if (self.direction == PreViewDirection_Portrait) {
        if (@available(iOS 11.0, *)) {
            if ([[UIApplication sharedApplication] delegate].window.safeAreaInsets.bottom > 0.0) {
                self.backViewHeightConstraint.constant = 44 + [[UIApplication sharedApplication] delegate].window.safeAreaInsets.bottom;
            }
        }
    } else if (self.direction == PreViewDirection_LandscapeRight) {
        if (@available(iOS 11.0, *)) {
            if ([[UIApplication sharedApplication] delegate].window.safeAreaInsets.bottom > 0.0) {
                self.backViewHeightConstraint.constant = 44 + [[UIApplication sharedApplication] delegate].window.safeAreaInsets.bottom;
                self.stackViewLeftConstraint.constant = [[UIApplication sharedApplication] delegate].window.safeAreaInsets.left;
                self.stackViewRightConstraint.constant = [[UIApplication sharedApplication] delegate].window.safeAreaInsets.right;
            }
        }
    }
}

- (BOOL)shouldAutorotate {
    return NO;
}

- (void)appDidBecomeActive {
    if (_cameraUtil != nil) {
        [_cameraUtil focusAuto];
    }
}

- (void)initRender {
    // replace this to your serial number
    NSString* ofSerialNumber = @"0b7f3051-bf5d-11ea-90f8-b42e994ac7a8";
    
    NSArray* documentsPathArr = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentPath = [documentsPathArr lastObject];
    NSString* ofLicensePath = [NSString stringWithFormat:@"%@/of_offline_license.license", documentPath];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:ofLicensePath]) {
        NSString *localLicensePath = [[NSBundle mainBundle] pathForResource:@"of_offline_license" ofType:@"license"];
        if ([fileManager fileExistsAtPath:localLicensePath]) {
            BOOL ret = [fileManager copyItemAtPath:localLicensePath toPath:ofLicensePath error:nil];
            if (!ret) {
                NSLog(@"Please confirm the address of the local license!");
                return;
            }
        } else {
            NSLog(@"Please confirm the address of the local license!");
            return;
        }
    }
    const char *dataPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    _ofHelper = new OrangeHelper;
    bool contextOk = _ofHelper->createContext([ofSerialNumber UTF8String], [ofLicensePath UTF8String], dataPath);
    if (!contextOk) {
        dispatch_async(dispatch_get_main_queue(), ^() {
            UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"" message:@"OrangeFilter SDK initialization failed. Please check whether the authorization has expired." preferredStyle:UIAlertControllerStyleAlert];
            [alert addAction:[UIAlertAction actionWithTitle:@"Ok" style:UIAlertActionStyleDefault handler:nil]];
            __weak __typeof__(self) weakSelf = self;
            [alert addAction:[UIAlertAction actionWithTitle:@"Retry" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                [weakSelf initRender];
            }]];
            [self presentViewController:alert animated:TRUE completion:nil];
        });
        return;
    }
    
    int width  = 0;
    int height = 0;
    if (self.direction == PreViewDirection_Portrait) {
        width  = (int) (SCREEN_WIDTH  * self.view.contentScaleFactor);
        height = (int) (SCREEN_HEIGHT * self.view.contentScaleFactor);
    } else if (self.direction == PreViewDirection_LandscapeRight) {
        width  = (int) (SCREEN_HEIGHT  * self.view.contentScaleFactor);
        height = (int) (SCREEN_WIDTH * self.view.contentScaleFactor);
    }
    _render = new EffectRender(_context, width, height, dataPath);
    _render->setOrangeHelper(_ofHelper);
    
    _captureSize = CGSizeMake(720, 1280);
    _cameraUtil = [[CameraUtil alloc] initWithCaptureSize:AVCaptureSessionPresetiFrame1280x720 direction:self.direction];
    [_cameraUtil focusAuto];
    
    [self.effectsView setDefaultEffect];
}

- (void)initUI {
    // focus
    self.view.userInteractionEnabled = TRUE;
    UITapGestureRecognizer* tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touchFocus:)];
    tap.delegate = self;
    [self.view addGestureRecognizer:tap];
    
    NSString* imagePath = [NSString stringWithFormat:@"%@/images/circle",
                           [[NSBundle mainBundle] bundlePath]];
    UIImage* img = [UIImage imageWithContentsOfFile:imagePath];
    
    const float imageH = 120.0f;
    const float imageW = 120.0f;
    _focusImage = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, imageW, imageH)];
    _focusImage.image = img;
    _focusImage.hidden = YES;
    [self.view addSubview:_focusImage];
    
    [self.backBtn setTitle:[NSBundle yy_localizedStringWithKey:@"返回"] forState:UIControlStateNormal];
    [self.hazyBtn setTitle:[NSBundle yy_localizedStringWithKey:@"朦胧美颜"] forState:UIControlStateNormal];
    [self.clearBtn setTitle:[NSBundle yy_localizedStringWithKey:@"清晰美颜"] forState:UIControlStateNormal];
    [self.faceBtn setTitle:[NSBundle yy_localizedStringWithKey:@"整型"] forState:UIControlStateNormal];
    [self.filterBtn setTitle:[NSBundle yy_localizedStringWithKey:@"滤镜"] forState:UIControlStateNormal];
    [self.stickerBtn setTitle:[NSBundle yy_localizedStringWithKey:@"贴纸"] forState:UIControlStateNormal];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    if (touch.view == self.view) {
        return YES;
    }
    return NO;
}

- (void)touchFocus:(UITapGestureRecognizer*)tap {
    
    CGPoint pos = [tap locationInView:self.view];
    
    float scale = self.view.contentScaleFactor;
    CGPoint posScaled = CGPointMake(pos.x * scale, pos.y * scale);
    CGSize viewSize = CGSizeMake(self.view.bounds.size.width * scale, self.view.bounds.size.height * scale);
    CGSize textureSize = _captureSize;
    
    int w, h;
    // clip
    if (viewSize.height / (float) viewSize.width > textureSize.height / (float) textureSize.width)
    {
        h = viewSize.height;
        w = h * textureSize.width / textureSize.height;
        
        posScaled.x += (w - viewSize.width) / 2;
    }
    else
    {
        w = viewSize.width;
        h = w * textureSize.height / textureSize.width;
        
        posScaled.y += (h - viewSize.height) / 2;
    }
    
    CGPoint posNormalized = CGPointMake(posScaled.x / w, posScaled.y / h);
    [_cameraUtil focusAtPoint:posNormalized];
    
    [self showFocusAnimation:pos];
}

- (void)showFocusAnimation:(CGPoint)pos {
    _focusImage.hidden = NO;
    _focusImage.transform = CGAffineTransformIdentity;
    _focusImage.alpha = 1.0f;
    _focusImage.center = pos;
    [_focusImage.layer removeAllAnimations];
    
    [UIView animateWithDuration:0.3 animations:^ {
        _focusImage.transform = CGAffineTransformMakeScale(0.6, 0.6);
    } completion:^(BOOL finished) {
        if (finished) {
            [UIView animateWithDuration:0.8 animations:^ {
                _focusImage.alpha = 0.0f;
            } completion:^(BOOL finished) {
                if (finished) {
                    _focusImage.hidden = YES;
                }
            }];
        }
    }];
}

- (void)dealloc {
    delete _render;
    _cameraUtil = nil;
    _context = nil;
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)initGLContext {
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (_context == nil) {
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
    
    GLKView* view = (GLKView*) self.view;
    view.context = _context;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
    view.drawableStencilFormat = GLKViewDrawableStencilFormatNone;
    
    [EAGLContext setCurrentContext:_context];
}

- (void)glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    [self startUpdateAccelerometerResult];
    int rotation = 0;
    if (self.direction == PreViewDirection_Portrait) {
        rotation = self.rotation;
    } else if (self.direction == PreViewDirection_LandscapeRight) {
        rotation = (self.rotation + 1) % 4;
    }
    CVPixelBufferRef pixelBuffer = _render->Render([_cameraUtil getCameraPixelBuffer], rotation);
    if (pixelBuffer)
    {
        CVPixelBufferRelease(pixelBuffer);
        pixelBuffer = nullptr;
    }
    [_cameraUtil releasePixelBuffer];
}

- (void)startUpdateAccelerometerResult
{
    if ([self.motionManager isAccelerometerAvailable] == YES)
    {
        [self.motionManager setAccelerometerUpdateInterval:0.1];
        __weak __typeof(self)weakSelf = self;
        
        [self.motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMAccelerometerData *accelerometerData, NSError *error)
         {
            double x = accelerometerData.acceleration.x;
            double y = accelerometerData.acceleration.y;
            if (fabs(y) >= fabs(x))
            {
                if (y > 0.0 && y <= 1.0) // upside down
                {
                    weakSelf.rotation = 2;
                    return;
                }
                if(y < 1.0 && y >= -1.0) // portrait
                {
                    weakSelf.rotation = 0;
                    return;
                }
            }
            else
            {
                if (x > 0.0 && x <= 1.0) // landscape right
                {
                    weakSelf.rotation = 1;
                    return;
                }
                if(x < 1.0 && x >= -1.0) // landscape left
                {
                    weakSelf.rotation = 3;
                    return;
                }
            }
        }];
    }
}

- (void)stopUpdate
{
    if ([self.motionManager isAccelerometerActive] == YES)
    {
        [self.motionManager stopAccelerometerUpdates];
    }
}

#pragma mark debug method
- (IBAction)backBtnClicked:(UIButton *)sender {
    _ofHelper->destroyContext();
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)hazyBeautyClicked:(UIButton *)sender {
    self.backView.hidden = YES;
    [self.view addSubview:self.effectsView];
    [self.effectsView showViewWithType:ET_HazyBeauty];
}

- (IBAction)clearBeautyClicked:(UIButton *)sender {
    self.backView.hidden = YES;
    [self.view addSubview:self.effectsView];
    [self.effectsView showViewWithType:ET_ClearBeauty];
}

- (IBAction)seniorBeautyTypeClicked:(UIButton *)sender {
    self.backView.hidden = YES;
    [self.view addSubview:self.effectsView];
    [self.effectsView showViewWithType:ET_SeniorBeautyType];
}

- (IBAction)filterClicked:(UIButton *)sender {
    self.backView.hidden = YES;
    [self.view addSubview:self.effectsView];
    [self.effectsView showViewWithType:ET_Filter];
}

- (IBAction)stickerClicked:(UIButton *)sender {
    self.backView.hidden = YES;
    [self.view addSubview:self.effectsView];
    [self.effectsView showViewWithType:ET_Sticker];
}

- (EffectsView *)effectsView
{
    if (!_effectsView) {
        _effectsView = [[EffectsView alloc] init];
        _effectsView.delegate = self;
        _effectsView.direction = self.direction;
        CGFloat width = 0;
        CGFloat height = 0;
        if (self.direction == PreViewDirection_Portrait) {
            width = SCREEN_WIDTH;
            height = SCREEN_HEIGHT;
        } else if (self.direction == PreViewDirection_LandscapeRight) {
            width = SCREEN_HEIGHT;
            height = SCREEN_WIDTH;
        }
        _effectsView.frame = (CGRect){0, 0, width, height};
    }
    return _effectsView;
}

#pragma mark EffectsViewDelegate
- (void)effectsViewHidden
{
    [self.effectsView removeFromSuperview];
    self.backView.hidden = NO;
}

- (void)releaseEffectType:(EffectsViewType)type effectModel:(EffectModel *)model
{
    switch (type) {
        case ET_HazyBeauty:
            _ofHelper->releaseEffect(OrangeHelper::ET_ClearBasicBeauty);
            break;
        case ET_ClearBeauty:
            _ofHelper->releaseEffect(OrangeHelper::ET_BasicBeauty);
            break;
        case ET_SeniorBeautyType:
            
            break;
        case ET_Filter:{
            OrangeHelper::EffectType effectType = [self getEffectTypeWithModel:model];
            _ofHelper->enableEffect(effectType, false);
            break;
        }
        case ET_Sticker:
            if (model.stickerType == ET_EffectGesture) {
                _ofHelper->releaseGesture(model.path.UTF8String);
            } else if (model.stickerType == ET_EffectSticker) {
                _ofHelper->releaseSticker(model.path.UTF8String);
            }
            break;
        default:
            break;
    }
}

- (void)didSelectedType:(EffectsViewType)type effectModel:(EffectModel *)model indexRow:(NSInteger)indexRow
{
    switch (type) {
        case ET_HazyBeauty:
            [self enableBeautyEffect:OrangeHelper::ET_BasicBeauty model:model indexRow:indexRow];
            break;
        case ET_ClearBeauty:
            [self enableBeautyEffect:OrangeHelper::ET_ClearBasicBeauty model:model indexRow:indexRow];
            break;
        case ET_SeniorBeautyType:
            [self enableBeautyEffect:OrangeHelper::ET_SeniorBeautyType model:model indexRow:indexRow];
            break;
        case ET_Filter:
            [self enableFilterEffect:model indexRow:indexRow];
            break;
        case ET_Sticker:
            [self enableSticker:model indexRow:indexRow];
            break;
        default:
            break;
    }
}

- (void)enableSticker:(EffectModel *)model indexRow:(NSInteger)indexRow
{
    if (model.stickerType == ET_EffectGesture) {
        _ofHelper->enableGesture(model.path.UTF8String, true);
    } else if (model.stickerType == ET_EffectSticker) {
        _ofHelper->enableSticker(model.path.UTF8String, true);
    }
}

- (void)enableBeautyEffect:(OrangeHelper::EffectType)effectType model:(EffectModel *)model indexRow:(NSInteger)indexRow
{
    if (0 == indexRow) {
        bool ret = _ofHelper->enableEffect(effectType, false);
        if (!ret) {
            NSLog(@"enableBeautyEffect false fail");
        }
        return;
    }
    bool ret = _ofHelper->enableEffect(effectType, true);
    if (!ret) {
        NSLog(@"enableBeautyEffect true fail");
    }
    OrangeHelper::EffectParam param;
    bool ret1 = _ofHelper->getEffectParamDetail(model.effectType, param);
    if (!ret1) {
        NSLog(@"enableBeautyEffect getEffectParamDetail fail");
    }
    model.curVal = param.curVal;
    model.maxVal = param.maxVal;
    model.minVal = param.minVal;
    model.defVal = param.defVal;
    [self.effectsView setEffectParam:model];
}

- (void)enableFilterEffect:(EffectModel *)model indexRow:(NSInteger)indexRow
{
    OrangeHelper::EffectType effectType = [self getEffectTypeWithModel:model];
    if (0 == indexRow) {
        return;
    }
    bool ret = _ofHelper->enableEffect(effectType, true);
    if (!ret) {
        NSLog(@"enableFilterEffect true fail");
    }
    OrangeHelper::EffectParam param;
    bool ret1 = _ofHelper->getEffectParamDetail(model.effectType, param);
    if (!ret1) {
        NSLog(@"enableFilterEffect getEffectParamDetail fail");
    }
    model.curVal = param.curVal;
    model.maxVal = param.maxVal;
    model.minVal = param.minVal;
    model.defVal = param.defVal;
    [self.effectsView setEffectParam:model];
}

- (OrangeHelper::EffectType)getEffectTypeWithModel:(EffectModel *)model
{
    OrangeHelper::EffectType effectType = OrangeHelper::ET_FilterHoliday;
    switch (model.effectType) {
        case OrangeHelper::EP_FilterHolidayIntensity:
            effectType = OrangeHelper::ET_FilterHoliday;
            break;
        case OrangeHelper::EP_FilterClearIntensity:
            effectType = OrangeHelper::ET_FilterClear;
            break;
        case OrangeHelper::EP_FilterWarmIntensity:
            effectType = OrangeHelper::ET_FilterWarm;
            break;
        case OrangeHelper::EP_FilterFreshIntensity:
            effectType = OrangeHelper::ET_FilterFresh;
            break;
        case OrangeHelper::EP_FilterTenderIntensity:
            effectType = OrangeHelper::ET_FilterTender;
            break;
        case OrangeHelper::EP_FilterCoolAdaierIntensity:
            effectType = OrangeHelper::ET_FilterCoolAdaier;
            break;
        case OrangeHelper::EP_FilterCoolDanyaIntensity:
            effectType = OrangeHelper::ET_FilterCoolDanya;
            break;
        case OrangeHelper::EP_FilterCoolJiazhouIntensity:
            effectType = OrangeHelper::ET_FilterCoolJiazhou;
            break;
        case OrangeHelper::EP_FilterCoolMojitoIntensity:
            effectType = OrangeHelper::ET_FilterCoolMojito;
            break;
        case OrangeHelper::EP_FilterWarmFennenIntensity:
            effectType = OrangeHelper::ET_FilterWarmFennen;
            break;
        case OrangeHelper::EP_FilterWarmPureIntensity:
            effectType = OrangeHelper::ET_FilterWarmPure;
            break;
        case OrangeHelper::EP_FilterWarmSweetIntensity:
            effectType = OrangeHelper::ET_FilterWarmSweet;
            break;
        case OrangeHelper::EP_FilterWarmWennuanIntensity:
            effectType = OrangeHelper::ET_FilterWarmWennuan;
            break;
        case OrangeHelper::EP_FilterWarmSunIntensity:
            effectType = OrangeHelper::ET_FilterWarmSun;
            break;
        case OrangeHelper::EP_FilterGrilWhiteIntensity:
            effectType = OrangeHelper::ET_FilterGrilWhite;
            break;
        case OrangeHelper::EP_FilterGrilLuolitaIntensity:
            effectType = OrangeHelper::ET_FilterGrilLuolita;
            break;
        case OrangeHelper::EP_FilterGrilLiteratureIntensity:
            effectType = OrangeHelper::ET_FilterGrilLiterature;
            break;
        case OrangeHelper::EP_FilterEnhanceAmorousIntensity:
            effectType = OrangeHelper::ET_FilterEnhanceAmorous;
            break;
        case OrangeHelper::EP_FilterEnhanceDeliciousIntensity:
            effectType = OrangeHelper::ET_FilterEnhanceDelicious;
            break;
        case OrangeHelper::EP_FilterEnhanceClearIntensity:
            effectType = OrangeHelper::ET_FilterEnhanceClear;
            break;
        case OrangeHelper::EP_FilterEnhanceNuolanIntensity:
            effectType = OrangeHelper::ET_FilterEnhanceNuolan;
            break;
        case OrangeHelper::EP_FilterEnhancePeachIntensity:
            effectType = OrangeHelper::ET_FilterEnhancePeach;
            break;
        case OrangeHelper::EP_FilterZhiganCreamIntensity:
            effectType = OrangeHelper::ET_FilterZhiganCream;
            break;
        case OrangeHelper::EP_FilterZhiganFilmIntensity:
            effectType = OrangeHelper::ET_FilterZhiganFilm;
            break;
        case OrangeHelper::EP_FilterZhiganMagazineIntensity:
            effectType = OrangeHelper::ET_FilterZhiganMagazine;
            break;
        default:
            break;
    }
    return effectType;
}

- (void)didSetEffectIntensityWitheffectModel:(EffectModel *)model
{
    bool ret = _ofHelper->setEffectParam(model.effectType, model.curVal);
    if (!ret) {
        NSLog(@"didSetEffectIntensityType fail");
    }
}

@end
