//
//  NSBundle+LanguageAdd.m
//  SCClassRoom
//
//  Created by GasparChu on 2020/3/5.
//  Copyright © 2020 SY. All rights reserved.
//

#import "NSBundle+LanguageAdd.h"

static NSString *const LanguageKey = @"LanguageKey";

@implementation NSBundle (LanguageAdd)

+ (NSString *)yy_localizedStringWithKey:(NSString *)key
{
    NSString *sysLanguageCode = [[[NSUserDefaults standardUserDefaults] objectForKey:@"AppleLanguages"] firstObject];
    NSString *localLanguageCode = [[NSUserDefaults standardUserDefaults] objectForKey:LanguageKey];
    NSString *currentLanguageCode = localLanguageCode.length ? localLanguageCode : sysLanguageCode;
    
    NSBundle *currentBundle = nil;
    if ([currentLanguageCode containsString:@"en"]) {
        currentLanguageCode = @"en";
        currentBundle = [NSBundle bundleWithPath:[[NSBundle mainBundle] pathForResource:currentLanguageCode ofType:@"lproj"]];
    } else {
        currentLanguageCode = @"zh-Hans";
        currentBundle = [NSBundle bundleWithPath:[[NSBundle mainBundle] pathForResource:currentLanguageCode ofType:@"lproj"]];
    }
    return currentBundle ? [currentBundle localizedStringForKey:key value:nil table:@"Localizable"] : key;
}

+ (BOOL)yy_currentLanguageIsEnglish
{
    NSString *sysLanguageCode = [[[NSUserDefaults standardUserDefaults] objectForKey:@"AppleLanguages"] firstObject];
    NSString *localLanguageCode = [[NSUserDefaults standardUserDefaults] objectForKey:LanguageKey];
    NSString *currentLanguageCode = localLanguageCode.length ? localLanguageCode : sysLanguageCode;
    
    if ([currentLanguageCode containsString:@"en"]) {
        return YES;
    } else {
        return NO;
    }
}

+ (void)yy_switchLanguageToHans
{
    [self switchLanguageCode:@"zh-Hans"];
}

+ (void)yy_switchLanguageToEn
{
    [self switchLanguageCode:@"en"];
}

+ (void)switchLanguageCode:(NSString *)code
{
    [[NSUserDefaults standardUserDefaults] setObject:code forKey:LanguageKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

@end
