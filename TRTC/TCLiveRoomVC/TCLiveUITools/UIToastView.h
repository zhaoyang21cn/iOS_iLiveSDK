//
//  UIToastView.h
//  TRTC
//
//  Created by Tencent on 2018/6/9.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
    UIToastShowMode_Default = 1,
    UIToastShowMode_Succ,
    UIToastShowMode_fail,
}UIToastShowMode;

@interface UIToastView : UIView
+ (UIToastView *)getInstance;
- (void)showToastWithMessage:(NSString *)text toastMode:(UIToastShowMode )mode;
@end
