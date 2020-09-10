//
//  EffectsView.m
//  OrangeFilterDemo
//
//  Created by GasparChu on 2020/7/30.
//  Copyright © 2020 tangqiuhu@yy.com. All rights reserved.
//

#define BEAUTY_HEIGHT       150

#import "EffectsView.h"
#import "EffectCell.h"
#import <Masonry.h>
#import "EffectModel.h"
#import "MJExtension.h"
#import "NSBundle+LanguageAdd.h"

@interface EffectsView ()<UICollectionViewDataSource, UICollectionViewDelegateFlowLayout, UIGestureRecognizerDelegate>

@property (nonatomic, assign) EffectsViewType currentType;
@property (nonatomic, copy) NSArray *dataArray;
@property (nonatomic, copy) NSArray *hazyBeautyArray;
@property (nonatomic, copy) NSArray *clearBeautyArray;
@property (nonatomic, copy) NSArray *seniorBeautyTypeArray;
@property (nonatomic, copy) NSArray *filterArray;
@property (nonatomic, copy) NSArray *stickerArray;

@property (nonatomic, assign) NSInteger hazyBeautySelectedIndex;
@property (nonatomic, assign) NSInteger clearBeautySelectedIndex;
@property (nonatomic, assign) NSInteger seniorBeautyTypeSelectedIndex;
@property (nonatomic, assign) NSInteger filterSelectedIndex;
@property (nonatomic, assign) NSInteger stickerSelectedIndex;

@property (strong, nonatomic) UIView *backView;
@property (strong, nonatomic) UIView *sliderView;
@property (strong, nonatomic) UISlider *slider;
@property (strong, nonatomic) UILabel *effectName;
@property (strong, nonatomic) UILabel *effectIntensity;
@property (strong, nonatomic) UICollectionView *collectionView;

@end

@implementation EffectsView

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self initDataSource];
        [self setupUI];
    }
    return self;;
}

- (void)setupUI
{
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapGestureClicked:)];
    tapGesture.delegate = self;
    [self addGestureRecognizer:tapGesture];
    
    [self addSubview:self.backView];
    [self.backView addSubview:self.sliderView];
    [self.sliderView addSubview:self.effectName];
    [self.sliderView addSubview:self.slider];
    [self.sliderView addSubview:self.effectIntensity];
    [self.backView addSubview:self.collectionView];
    
    [self.backView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_offset(0.f);
        make.bottom.mas_offset(BEAUTY_HEIGHT);
        make.height.mas_offset(BEAUTY_HEIGHT);
    }];
    [self.sliderView mas_makeConstraints:^(MASConstraintMaker *make) {
        if (@available(iOS 11.0, *)) {
            make.top.mas_offset(0.f);
            make.left.mas_equalTo(self.backView.mas_safeAreaLayoutGuideLeft);
            make.right.mas_equalTo(self.backView.mas_safeAreaLayoutGuideRight);
        } else {
            make.top.left.right.mas_offset(0.f);
        }
        make.height.mas_offset(60.f);
    }];
    [self.effectName mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_offset(0.f);
        make.left.mas_offset(10.f);
        make.width.mas_offset(100.f);
        make.height.mas_offset(20.f);
    }];
    [self.effectIntensity mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_offset(0.f);
        make.right.mas_offset(-15.f);
        make.width.mas_offset(50.f);
        make.height.mas_offset(20.f);
    }];
    [self.slider mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(self.effectName.mas_right).mas_offset(5.f);
        make.right.mas_equalTo(self.effectIntensity.mas_left).mas_offset(-5.f);
        make.centerY.mas_offset(0.f);
    }];
    [self.collectionView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(self.sliderView);
        make.bottom.mas_offset(0.f);
        make.height.mas_offset(90.f);
    }];
}

- (void)tapGestureClicked:(UITapGestureRecognizer *)sender
{
    [UIView animateWithDuration:0.25 animations:^{
        [self.backView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.bottom.mas_offset(BEAUTY_HEIGHT);
        }];
        [self layoutIfNeeded];
    } completion:^(BOOL finished) {
        if ([self.delegate respondsToSelector:@selector(effectsViewHidden)]) {
            [self.delegate effectsViewHidden];
        }
    }];
}

- (void)setDefaultEffect
{
    _hazyBeautySelectedIndex = 1;
    [self setDataSourceAndViewWithType:ET_HazyBeauty];
    [self collectionView:self.collectionView didSelectItemAtIndexPath:[NSIndexPath indexPathForRow:self.hazyBeautySelectedIndex inSection:0]];
}

- (void)showViewWithType:(EffectsViewType)type
{
    self.currentType = type;
    [self setDataSourceAndViewWithType:type];
    [self.collectionView reloadData];
    
    if (self.currentType == ET_Sticker) {
        [self.collectionView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.height.mas_offset(BEAUTY_HEIGHT);
        }];
    } else {
        [self.collectionView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.height.mas_offset(90);
        }];
    }
    [self layoutIfNeeded];
    [UIView animateWithDuration:0.25 animations:^{
        [self.backView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.bottom.mas_offset(0);
        }];
        [self layoutIfNeeded];
    }];
}

- (void)setEffectParam:(EffectModel *)model
{
    self.slider.minimumValue = model.minVal;
    self.slider.maximumValue = model.maxVal;
    self.slider.value = model.curVal;
    self.effectIntensity.text = @(self.slider.value).stringValue;
}

- (void)sliderValueChanged:(UISlider *)sender {
    NSInteger index = -1;
    switch (self.currentType) {
        case ET_HazyBeauty:
            index = self.hazyBeautySelectedIndex;
            break;
        case ET_ClearBeauty:
            index = self.clearBeautySelectedIndex;
            break;
        case ET_SeniorBeautyType:
            index = self.seniorBeautyTypeSelectedIndex;
            break;
        case ET_Filter:
            index = self.filterSelectedIndex;
            break;
        case ET_Sticker:
            index = self.stickerSelectedIndex;
            break;
        default:
            break;
    }
    EffectModel *model = self.dataArray[index];
    model.curVal = sender.value;
    self.effectIntensity.text = @(model.curVal).stringValue;
    if ([self.delegate respondsToSelector:@selector(didSetEffectIntensityWitheffectModel:)]) {
        [self.delegate didSetEffectIntensityWitheffectModel:model];
    }
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return self.dataArray.count;
}

- (__kindof UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    EffectCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"EffectCell" forIndexPath:indexPath];
    EffectModel *model = self.dataArray[indexPath.row];
    [cell setName:model.title thumb:model.thumb selected:model.selected];
    return cell;
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    if (self.currentType == ET_Sticker) {
        return (CGSize){64, 64};
    }
    return (CGSize){64, 90};
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    NSInteger index = -1;
    switch (self.currentType) {
        case ET_HazyBeauty:
        {
            index = self.hazyBeautySelectedIndex;
            self.hazyBeautySelectedIndex = indexPath.row;
            
            // Operate hazy beauty to force clear beauty automatically invalid
            EffectModel *beautyModel = self.clearBeautyArray[self.clearBeautySelectedIndex];
            beautyModel.selected = NO;
            self.clearBeautySelectedIndex = 0;
            // Select the original by default
            EffectModel *beautyModel1 = self.clearBeautyArray[0];
            beautyModel1.selected = YES;
            break;
        }
        case ET_ClearBeauty:
        {
            index = self.clearBeautySelectedIndex;
            self.clearBeautySelectedIndex = indexPath.row;
            
            // Operate clear beauty to force hazy beauty automatically invalid
            EffectModel *beautyModel = self.hazyBeautyArray[self.hazyBeautySelectedIndex];
            beautyModel.selected = NO;
            self.hazyBeautySelectedIndex = 0;
            // Select the original by default
            EffectModel *beautyModel1 = self.hazyBeautyArray[0];
            beautyModel1.selected = YES;
            break;
        }
        case ET_SeniorBeautyType:
            index = self.seniorBeautyTypeSelectedIndex;
            self.seniorBeautyTypeSelectedIndex = indexPath.row;
            break;
        case ET_Filter:
            index = self.filterSelectedIndex;
            self.filterSelectedIndex = indexPath.row;
            break;
        case ET_Sticker:
            index = self.stickerSelectedIndex;
            self.stickerSelectedIndex = indexPath.row;
            break;
        default:
            break;
    }
    if (index >= 0) {
        NSIndexPath *preIndexPath = [NSIndexPath indexPathForRow:index inSection:0];
        EffectCell *preCell = (EffectCell *)[collectionView cellForItemAtIndexPath:preIndexPath];
        EffectModel *preModel = self.dataArray[preIndexPath.row];
        preModel.selected = NO;
        [preCell setName:preModel.title thumb:preModel.thumb selected:preModel.selected];
        
        if ([self.delegate respondsToSelector:@selector(releaseEffectType:effectModel:)]) {
            [self.delegate releaseEffectType:self.currentType effectModel:preModel];
        }
    }
    
    EffectCell *cell = (EffectCell *)[collectionView cellForItemAtIndexPath:indexPath];
    EffectModel *model = self.dataArray[indexPath.row];
    model.selected = YES;
    [cell setName:model.title thumb:model.thumb selected:model.selected];
    if (self.currentType != ET_Sticker) {
        self.sliderView.hidden = !indexPath.row;
        self.effectName.text = model.title;
    }
    
    if ([self.delegate respondsToSelector:@selector(didSelectedType:effectModel:indexRow:)]) {
        [self.delegate didSelectedType:self.currentType effectModel:model indexRow:indexPath.row];
    }
}

- (void)setDataSourceAndViewWithType:(EffectsViewType)type
{
    UICollectionViewFlowLayout *layout = (UICollectionViewFlowLayout *)self.collectionView.collectionViewLayout;
    layout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
    
    NSInteger index = 0;
    switch (type) {
        case ET_HazyBeauty:
            self.dataArray = self.hazyBeautyArray;
            self.sliderView.hidden = !self.hazyBeautySelectedIndex;
            index = self.hazyBeautySelectedIndex;
            break;
        case ET_ClearBeauty:
            self.dataArray = self.clearBeautyArray;
            self.sliderView.hidden = !self.clearBeautySelectedIndex;
            index = self.clearBeautySelectedIndex;
            break;
        case ET_SeniorBeautyType:
            self.dataArray = self.seniorBeautyTypeArray;
            self.sliderView.hidden = !self.seniorBeautyTypeSelectedIndex;
            index = self.seniorBeautyTypeSelectedIndex;
            break;
        case ET_Filter:
            self.dataArray = self.filterArray;
            self.sliderView.hidden = !self.filterSelectedIndex;
            index = self.filterSelectedIndex;
            break;
        case ET_Sticker:
            self.dataArray = self.stickerArray;
            self.sliderView.hidden = YES;
            layout.scrollDirection = UICollectionViewScrollDirectionVertical;
            break;
        default:
            break;
    }
    
    EffectModel *model = self.dataArray[index];
    self.effectName.text = model.title;
    [self setEffectParam:model];
}

- (void)initDataSource
{
    NSArray *hazyBeautyArr = @[@{@"title":[NSBundle yy_localizedStringWithKey:@"原图"], @"thumb":@"beauty_original", @"selected": @(NO), @"effectType": @(-1)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"美肤（朦胧）"], @"thumb":@"beauty_meifu", @"selected": @(YES), @"effectType": @(OrangeHelper::EP_BasicBeautyOpacity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"美白"], @"thumb":@"beauty_meibai", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_BasicBeautyIntensity)}];
    NSArray *clearBeautyArr = @[@{@"title":[NSBundle yy_localizedStringWithKey:@"原图"], @"thumb":@"beauty_original", @"selected": @(NO), @"effectType": @(-1)},
                                @{@"title":[NSBundle yy_localizedStringWithKey:@"美肤（清晰）"], @"thumb":@"beauty_meifu", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_ClearBeautyOpacity)},
                                @{@"title":[NSBundle yy_localizedStringWithKey:@"美白"], @"thumb":@"beauty_meibai", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_ClearBeautyIntensity)}];
    NSArray *filterArr = @[@{@"title":[NSBundle yy_localizedStringWithKey:@"原图"], @"thumb":@"beauty_original", @"selected": @(YES), @"effectType": @(-1)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"假日"], @"thumb":@"filter_jiari", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterHolidayIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"清晰"], @"thumb":@"filter_qingxi", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterClearIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"暖阳"], @"thumb":@"filter_nuanyang", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterWarmIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"清新"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterFreshIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"粉嫩"], @"thumb":@"filter_fennen", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterTenderIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"冷色-阿黛尔"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterCoolAdaierIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"冷色-淡雅"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterCoolDanyaIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"冷色-加州"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterCoolJiazhouIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"冷色-莫吉托"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterCoolMojitoIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"暖色-粉嫩"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterWarmFennenIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"暖色-青春"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterWarmPureIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"暖色-甜美"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterWarmSweetIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"暖色-温暖"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterWarmWennuanIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"暖色-阳光"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterWarmSunIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"少女-白皙"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterGrilWhiteIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"少女-洛丽塔"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterGrilLuolitaIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"少女-文艺"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterGrilLiteratureIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"增强-风情"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterEnhanceAmorousIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"增强-可口"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterEnhanceDeliciousIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"增强-明亮"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterEnhanceClearIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"增强-诺兰"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterEnhanceNuolanIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"增强-水蜜桃"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterEnhancePeachIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"质感-冰淇凌"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterZhiganCreamIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"质感-胶片"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterZhiganFilmIntensity)},
                           @{@"title":[NSBundle yy_localizedStringWithKey:@"质感-杂志"], @"thumb":@"filter_qingxin", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_FilterZhiganMagazineIntensity)}];
    NSArray *beautyTypeArr = @[@{@"title":[NSBundle yy_localizedStringWithKey:@"原图"], @"thumb":@"beauty_original", @"selected": @(YES), @"effectType": @(-1)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"窄脸"], @"thumb":@"beauty_zhailian", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeThinFaceIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"小脸"], @"thumb":@"beauty_xiaolian", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeSmallFaceIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"瘦颧骨"], @"thumb":@"beauty_shouegu", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeSquashedFaceIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"额高"], @"thumb":@"beauty_egao", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeForeheadLiftingIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"额宽"], @"thumb":@"beauty_ekuan", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeWideForeheadIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"大眼"], @"thumb":@"beauty_dayan", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeBigSmallEyeIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"眼距"], @"thumb":@"beauty_yanju", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeEyesOffsetIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"眼角"], @"thumb":@"beauty_yanjiao", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeEyesRotationIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"瘦鼻"], @"thumb":@"beauty_shoubi", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeThinNoseIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"长鼻"], @"thumb":@"beauty_changbi", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeLongNoseIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"窄鼻梁"], @"thumb":@"beauty_zhaibiliang", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeThinNoseBridgeIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"小嘴"], @"thumb":@"beauty_xiaozui", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeThinmouthIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"嘴位"], @"thumb":@"beauty_zuiwei", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeMovemouthIntensity)},
                               @{@"title":[NSBundle yy_localizedStringWithKey:@"下巴"], @"thumb":@"beauty_xiaba", @"selected": @(NO), @"effectType": @(OrangeHelper::EP_SeniorTypeChinLiftingIntensity)}];
    NSArray *stickerArr = @[@{@"title":@"", @"thumb":@"close", @"selected": @(YES), @"stickerType": @(-1)},
                            @{@"title":@"", @"thumb":@"sticker_gesture", @"selected": @(NO), @"stickerType": @(ET_EffectGesture),
                              @"path":[NSString stringWithFormat:@"%@/effects/gestures.zip", [[NSBundle mainBundle] bundlePath]]},
                            @{@"title":@"", @"thumb":@"sticker_background", @"selected": @(NO), @"stickerType": @(ET_EffectSticker),
                              @"path":[NSString stringWithFormat:@"%@/effects/segment.zip", [[NSBundle mainBundle] bundlePath]]},
                            @{@"title":@"", @"thumb":@"sticker_facepoint", @"selected": @(NO), @"stickerType": @(ET_EffectSticker),
                              @"path":[NSString stringWithFormat:@"%@/effects/facepoints.zip", [[NSBundle mainBundle] bundlePath]]},
                            @{@"title":@"", @"thumb":@"sticker_cat", @"selected": @(NO), @"stickerType": @(ET_EffectSticker),
                              @"path":[NSString stringWithFormat:@"%@/effects/cat.zip", [[NSBundle mainBundle] bundlePath]]},
                            @{@"title":@"", @"thumb":@"sticker_mikimouse", @"selected": @(NO), @"stickerType": @(ET_EffectSticker),
                              @"path":[NSString stringWithFormat:@"%@/effects/mickey_mouse.zip", [[NSBundle mainBundle] bundlePath]]},
                            @{@"title":@"", @"thumb":@"sticker_minimouse", @"selected": @(NO), @"stickerType": @(ET_EffectSticker),
                              @"path":[NSString stringWithFormat:@"%@/effects/minnie_mouse.zip", [[NSBundle mainBundle] bundlePath]]},
                            @{@"title":@"", @"thumb":@"sticker_hair", @"selected": @(NO), @"stickerType": @(ET_EffectSticker),
                              @"path":[NSString stringWithFormat:@"%@/effects/face3dplane.zip", [[NSBundle mainBundle] bundlePath]]}];
    
    self.hazyBeautyArray = [EffectModel mj_objectArrayWithKeyValuesArray:hazyBeautyArr];
    self.clearBeautyArray = [EffectModel mj_objectArrayWithKeyValuesArray:clearBeautyArr];
    self.seniorBeautyTypeArray = [EffectModel mj_objectArrayWithKeyValuesArray:beautyTypeArr];
    self.filterArray = [EffectModel mj_objectArrayWithKeyValuesArray:filterArr];
    self.stickerArray = [EffectModel mj_objectArrayWithKeyValuesArray:stickerArr];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    if (touch.view == self) {
        return YES;
    }
    return NO;
}

- (UIView *)backView
{
    if (!_backView) {
        UIView *view = [[UIView alloc] init];
        view.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.5];
        _backView = view;
    }
    return _backView;
}

- (UIView *)sliderView
{
    if (!_sliderView) {
        UIView *view = [[UIView alloc] init];
        view.backgroundColor = [UIColor clearColor];
        view.hidden = YES;
        _sliderView = view;
    }
    return _sliderView;
}

- (UISlider *)slider
{
    if (!_slider) {
        UISlider *slider1 = [[UISlider alloc] init];
        [slider1 addTarget:self action:@selector(sliderValueChanged:) forControlEvents:UIControlEventValueChanged];
        _slider = slider1;
    }
    return _slider;
}

- (UILabel *)effectName
{
    if (!_effectName) {
        UILabel *label = [[UILabel alloc] init];
        label.textColor = [UIColor whiteColor];
        label.font = [UIFont systemFontOfSize:12.f];
        _effectName = label;
    }
    return _effectName;
}

- (UILabel *)effectIntensity
{
    if (!_effectIntensity) {
        UILabel *label = [[UILabel alloc] init];
        label.textColor = [UIColor whiteColor];
        label.font = [UIFont systemFontOfSize:14.f];
        _effectIntensity = label;
    }
    return _effectIntensity;
}

- (UICollectionView *)collectionView
{
    if (!_collectionView) {
        UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
        layout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
        layout.sectionInset = (UIEdgeInsets){10, 0, 0, 0};
        UICollectionView *view = [[UICollectionView alloc] initWithFrame:CGRectZero collectionViewLayout:layout];
        view.backgroundColor = [UIColor clearColor];
        view.delegate = self;
        view.dataSource = self;
        _collectionView = view;
        
        [_collectionView registerClass:[EffectCell class] forCellWithReuseIdentifier:@"EffectCell"];
    }
    return _collectionView;
}

@end


