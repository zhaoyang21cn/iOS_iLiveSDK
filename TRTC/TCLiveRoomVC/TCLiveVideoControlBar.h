//
//  TCLiveVideoControlBar.h
//  TRTC
//
//  Created by Tencent on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol TCLiveVideoControlBarDelegate <NSObject>
@optional
//切换摄像头
- (void)switchCameraBtnClick:(UIButton *)sender;
//开关美颜
- (void)beautyBtnClick:(UIButton *)sender;
//开关麦克风
- (void)voiceBtnClick:(UIButton *)sender;
//展示日志
- (void)logBtnClick:(UIButton *)sender;
//反馈
- (void)feedBackBtnClick:(UIButton *)sender;
//切换配置
- (void)changeRoleBtnClick:(UIButton *)sender;
//聊天
- (void)chatBtnClick:(UIButton *)sender;
@end

@interface TCLiveVideoControlBar : UIView
@property(nonatomic,weak)id<TCLiveVideoControlBarDelegate> delegate;
@property(nonatomic,strong) UIButton *logBtn;
- (void)enableLog:(BOOL)endable;
- (void)enableBeauty:(BOOL)enable;
@end
