//
//  EffectCell.m
//  MouseLive
//
//  Created by GasparChu on 2020/4/16.
//  Copyright © 2020 sy. All rights reserved.
//

#import "EffectCell.h"
#import <Masonry.h>

@interface EffectCell ()

@property (nonatomic, strong) UIImageView *selectedImgView;
@property (nonatomic, strong) UIImageView *imgView;
@property (nonatomic, strong) UILabel *nameLabel;

@end

@implementation EffectCell

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setupUI];
    }
    return self;
}

- (void)setupUI
{
    [self.contentView addSubview:self.selectedImgView];
    [self.contentView addSubview:self.imgView];
    [self.contentView addSubview:self.nameLabel];
    
    [self.selectedImgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_offset(0.f);
        make.centerX.mas_offset(0.f);
        make.width.height.mas_offset(56.f);
    }];
    [self.imgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.top.mas_equalTo(self.selectedImgView).mas_offset(8.f);
        make.right.bottom.mas_equalTo(self.selectedImgView).mas_offset(-8.f);
    }];
    [self.nameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_offset(0.f);
        make.top.mas_equalTo(self.selectedImgView.mas_bottom).mas_offset(2.f);
    }];
}

- (void)setName:(NSString *)name thumb:(NSString *)thumb selected:(BOOL)selected
{
    self.selectedImgView.hidden = !selected;
    self.nameLabel.text = name;
    self.imgView.image = [UIImage imageNamed:thumb];
}

- (UIImageView *)selectedImgView
{
    if (!_selectedImgView) {
        UIImageView *imgView = [[UIImageView alloc] init];
        imgView.image = [UIImage imageNamed:@"border"];
        _selectedImgView = imgView;
    }
    return _selectedImgView;
}

- (UIImageView *)imgView
{
    if (!_imgView) {
        UIImageView *imgView = [[UIImageView alloc] init];
        imgView.contentMode = UIViewContentModeScaleAspectFit;
        _imgView = imgView;
    }
    return _imgView;
}

- (UILabel *)nameLabel
{
    if (!_nameLabel) {
        UILabel *label = [[UILabel alloc] init];
        label.textColor = [UIColor whiteColor];
        label.textAlignment = NSTextAlignmentCenter;
        label.font = [UIFont systemFontOfSize:11.f];
        _nameLabel = label;
    }
    return _nameLabel;
}

@end
