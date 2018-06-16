//
//  RefreshFooter.h
//  TRTC
//
//  Created by Tencent on 2018/6/5.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void (^RefreshFooterBlock)(void);

@interface RefreshFooter : UIView

- (instancetype)initWithTarget:(id)target beginRefreshBlock:(RefreshFooterBlock)refreshHeaderBlock;

- (void)beginRefreshing;

- (void)endRefreshing;
@end
