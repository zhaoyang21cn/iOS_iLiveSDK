//
//  TCLiveVideoControlBar.h
//  TRTC
//
//  Created by ericxwli on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol TCLiveVideoControlBarDelegate <NSObject>
@optional
- (void)switchCameraBtnClick:(UIButton *)sender;
- (void)beautyBtnClick:(UIButton *)sender;
- (void)voiceBtnClick:(UIButton *)sender;
- (void)logBtnClick:(UIButton *)sender;
- (void)feedBackBtnClick:(UIButton *)sender;
- (void)changeRoleBtnClick:(UIButton *)sender;
- (void)chatBtnClick:(UIButton *)sender;
@end

@interface TCLiveVideoControlBar : UIView
@property(nonatomic,weak)id<TCLiveVideoControlBarDelegate> delegate;
@property(nonatomic,strong) UIButton *logBtn;
- (void)logBtnClick:(UIButton *)sender;
- (void)enableLog:(BOOL)endable;
- (void)enableBeauty:(BOOL)enable;
@end
