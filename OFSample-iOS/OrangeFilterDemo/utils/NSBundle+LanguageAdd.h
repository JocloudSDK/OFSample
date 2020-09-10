//
//  NSBundle+LanguageAdd.h
//  SCClassRoom
//
//  Created by GasparChu on 2020/3/5.
//  Copyright © 2020 SY. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSBundle (LanguageAdd)

/// Get local string
/// @param key string key
+ (NSString *)yy_localizedStringWithKey:(NSString *)key;

/// switch to hans
+ (void)yy_switchLanguageToHans;

/// swith to english
+ (void)yy_switchLanguageToEn;

/// get current language is english
+ (BOOL)yy_currentLanguageIsEnglish;

@end

NS_ASSUME_NONNULL_END
