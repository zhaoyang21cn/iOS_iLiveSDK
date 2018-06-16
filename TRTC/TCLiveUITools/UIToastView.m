//
//  UIToastView.m
//  TRTC
//
//  Created by Tencent on 2018/6/9.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "UIToastView.h"

#define UITOAST_HEIGHT  30
#define UITOAST_IMAGE_HEIGHT 15
@interface UIToastView ()
@property(nonatomic,strong)UILabel *toastLabel;
@property(nonatomic,strong)UIImageView *toastImageView;
@end

@implementation UIToastView

+ (UIToastView *)getInstance{
    static UIToastView *singleTon = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        singleTon = [UIToastView new];
    });
    return singleTon;
}

-(instancetype)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        self.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.5];
        self.layer.cornerRadius = 4;
        self.layer.masksToBounds = YES;
        
        [self addSubview:self.toastImageView];
        [self addSubview:self.toastLabel];
    }
    return self;
}
- (UILabel *)toastLabel{
    if (!_toastLabel) {
        _toastLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        _toastLabel.numberOfLines = 1;
        _toastLabel.textAlignment = NSTextAlignmentCenter;
        _toastLabel.textColor = [UIColor whiteColor];
        _toastLabel.font = [UIFont systemFontOfSize:16];
    }
    return _toastLabel;
}

-(UIImageView *)toastImageView{
    if (!_toastImageView) {
        _toastImageView = [[UIImageView alloc] initWithFrame:CGRectZero];
        
    }
    return _toastImageView;
}

- (void)showToastWithMessage:(NSString *)text toastMode:(UIToastShowMode )mode{
    dispatch_async(dispatch_get_main_queue(), ^{
        UIToastView *view = [UIToastView new];
        CGRect rect = [text boundingRectWithSize:CGSizeMake([UIScreen mainScreen].bounds.size.width - 80, 20) options:NSStringDrawingUsesLineFragmentOrigin attributes:@{NSFontAttributeName:[UIFont systemFontOfSize:16]} context:nil];
        view.frame = CGRectMake(0, 0, rect.size.width + 10 + 20, UITOAST_HEIGHT);
        view.toastImageView.frame = CGRectMake(5, 0, UITOAST_IMAGE_HEIGHT, UITOAST_IMAGE_HEIGHT);
        view.toastImageView.center = CGPointMake(view.toastImageView.center.x, view.frame.size.height/2);
        view.toastLabel.frame = CGRectMake(25, 0, rect.size.width, UITOAST_HEIGHT);
        view.toastLabel.text = text;
        if (UIToastShowMode_Succ == mode){
            view.toastImageView.image = [UIImage imageNamed:@"ic_toast_success@2x"];
        }
        else if(UIToastShowMode_fail == mode){
            view.toastImageView.image = [UIImage imageNamed:@"icon_sign@2x"];
        }
        else{
            view.toastImageView.frame = CGRectZero;
            view.frame = CGRectMake(0, 0, rect.size.width + 10 , UITOAST_HEIGHT);
            view.toastLabel.frame = CGRectMake(5, 0, rect.size.width, UITOAST_HEIGHT);
        }
        view.center = [[UIApplication sharedApplication] keyWindow].center;
        [[[UIApplication sharedApplication] keyWindow] addSubview:view];
        [UIView animateWithDuration:0.5 animations:^{
            view.alpha = 1;
        } completion:^(BOOL finished) {
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                [UIView animateWithDuration:0.5 animations:^{
                    view.alpha = 0;
                } completion:^(BOOL finished) {
                    [view removeFromSuperview];
                }];
            });
        }];
    });
    
}

@end
