//
//  EffectModel.h
//  OrangeFilterDemo
//
//  Created by GasparChu on 2020/8/3.
//  Copyright © 2020 tangqiuhu@yy.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "OrangeHelper.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, EffectStickerType) {
    ET_EffectSticker = 100,   // Sticker
    ET_EffectGesture,         // Gesture sticker
};


@interface EffectModel : NSObject

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *thumb;
@property (nonatomic, assign) BOOL selected;

// Skin(Hazy、Clear)、Face、Filter
@property (nonatomic, assign) OrangeHelper::EffectParamType effectType;
@property (nonatomic, assign) int curVal; // current value
@property (nonatomic, assign) int maxVal; // max value
@property (nonatomic, assign) int minVal; // min value
@property (nonatomic, assign) int defVal; // default value

// Gesture sticker、Sticker
@property (nonatomic, assign) EffectStickerType stickerType;
@property (nonatomic, copy) NSString *path;


@end

NS_ASSUME_NONNULL_END
