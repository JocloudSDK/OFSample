//
//  EffectsView.h
//  OrangeFilterDemo
//
//  Created by GasparChu on 2020/7/30.
//  Copyright © 2020 tangqiuhu@yy.com. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CameraUtil.h"

NS_ASSUME_NONNULL_BEGIN

@class EffectModel;
/*
 Display view type.
 The relationship between hazy beauty and clear beauty is mutually exclusive. When one becomes effective, the other automatically fails. The business display logic is optional when connected.
 */
typedef NS_ENUM(NSInteger, EffectsViewType) {
    ET_HazyBeauty = 0,          // Hazy Skin
    ET_ClearBeauty,             // Clear Skin
    ET_SeniorBeautyType,        // Face
    ET_Filter,                  // Filter
    ET_Sticker,                 // Sticker
};

@protocol EffectsViewDelegate <NSObject>

/// View hidden
- (void)effectsViewHidden;

/// Release effects
/// @param type view type
/// @param model effect model
- (void)releaseEffectType:(EffectsViewType)type effectModel:(EffectModel *)model;

/// Select effect type
/// @param type view type
/// @param model effect model
/// @param indexRow effect index
- (void)didSelectedType:(EffectsViewType)type effectModel:(EffectModel *)model indexRow:(NSInteger)indexRow;

/// Set effect Intensity
/// @param model effect model
- (void)didSetEffectIntensityWitheffectModel:(EffectModel *)model;

@end

@interface EffectsView : UIView

@property (nonatomic, assign) PreViewDirection direction;
@property (nonatomic, weak) id<EffectsViewDelegate> delegate;

/// Set default effect
- (void)setDefaultEffect;

/// Show view
/// @param type view type
- (void)showViewWithType:(EffectsViewType)type;

/// Set effectParam
/// @param model effect model
- (void)setEffectParam:(EffectModel *)model;

@end

NS_ASSUME_NONNULL_END
