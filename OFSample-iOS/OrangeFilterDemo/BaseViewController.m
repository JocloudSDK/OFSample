//
//  BaseViewController.m
//  OrangeFilterDemo
//
//  Created by GasparChu on 2020/8/5.
//  Copyright © 2020 tangqiuhu@yy.com. All rights reserved.
//

#import "BaseViewController.h"
#import "ViewController.h"
#import <UIImageView+WebCache.h>
#import "NSBundle+LanguageAdd.h"


@interface BaseViewController ()

@property (weak, nonatomic) IBOutlet UIImageView *logoImageView;
@property (weak, nonatomic) IBOutlet UIButton *languageSwitchBtn;
@property (weak, nonatomic) IBOutlet UIButton *pBtn;
@property (weak, nonatomic) IBOutlet UIButton *lBtn;

@end

@implementation BaseViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.logoImageView sd_setImageWithURL:[NSURL URLWithString:@"https://www.jocloud.com/cn/img/logo-0.09870b342718788670474287268cee5d.png"] placeholderImage:[UIImage imageNamed:@"logo"]];
    [self.languageSwitchBtn setTitle:[NSBundle yy_localizedStringWithKey:@"language"] forState:UIControlStateNormal];
    [self.pBtn setTitle:[NSBundle yy_localizedStringWithKey:@"竖屏预览"] forState:UIControlStateNormal];
    [self.lBtn setTitle:[NSBundle yy_localizedStringWithKey:@"横屏预览"] forState:UIControlStateNormal];
}


- (IBAction)portraitBtnCliked:(UIButton *)sender {
    UIStoryboard *mainStoryboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    ViewController *vc = [mainStoryboard instantiateViewControllerWithIdentifier:@"ViewController"];
    if (@available(iOS 13.0, *)) {
        if (UIModalPresentationPageSheet == vc.modalPresentationStyle) {
            vc.modalPresentationStyle = UIModalPresentationFullScreen;
        }
    }
    vc.direction = PreViewDirection_Portrait;
    [self presentViewController:vc animated:YES completion:nil];
}

- (IBAction)landscapeRightBtnClicked:(UIButton *)sender {
    UIStoryboard *mainStoryboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    ViewController *vc = [mainStoryboard instantiateViewControllerWithIdentifier:@"ViewController"];
    if (@available(iOS 13.0, *)) {
        if (UIModalPresentationPageSheet == vc.modalPresentationStyle) {
            vc.modalPresentationStyle = UIModalPresentationFullScreen;
        }
    }
    vc.direction = PreViewDirection_LandscapeRight;
    [self presentViewController:vc animated:YES completion:nil];
}

- (IBAction)languageSwith:(UIButton *)sender {
    if ([NSBundle yy_currentLanguageIsEnglish]) {
        [NSBundle yy_switchLanguageToHans];
    } else {
        [NSBundle yy_switchLanguageToEn];
    }
    [self.languageSwitchBtn setTitle:[NSBundle yy_localizedStringWithKey:@"language"] forState:UIControlStateNormal];
    [self.pBtn setTitle:[NSBundle yy_localizedStringWithKey:@"竖屏预览"] forState:UIControlStateNormal];
    [self.lBtn setTitle:[NSBundle yy_localizedStringWithKey:@"横屏预览"] forState:UIControlStateNormal];
}

- (BOOL)shouldAutorotate {
    return NO;
}

@end
