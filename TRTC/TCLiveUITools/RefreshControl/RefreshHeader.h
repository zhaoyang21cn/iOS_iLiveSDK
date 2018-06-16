//
//  RefreshHeader.h
//  TRTC
//
//  Created by Tencent on 2018/6/5.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void (^RefreshHeaderBlock)(void);

@interface RefreshHeader : UIView

- (instancetype)initWithTarget:(id)target beginRefreshBlock:(RefreshHeaderBlock)refreshHeaderBlock;

- (void)beginRefreshing;

- (void)endRefreshing;

@end



@interface UIColor (hex)
+ (UIColor *)colorWithHexString:(NSString *)hex;
@end

@interface NSString (hex)
- (NSUInteger)integerValueFromHex;
@end
