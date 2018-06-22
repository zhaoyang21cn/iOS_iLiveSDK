//
//  UIColorEX.m
//  TRTC
//
//  Created by Tencent on 2018/6/5.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "UIColorEX.h"

@implementation UIColor (EX)

+ (UIColor *)colorWithRGBHex: (unsigned int)hex
{
    int r = (hex >> 16) & 0xFF;
    int g = (hex >> 8) & 0xFF;
    int b = (hex) & 0xFF;
    
    return [UIColor colorWithRed:r / 255.0f
                           green:g / 255.0f
                            blue:b / 255.0f
                           alpha:1.0f];
}

@end
