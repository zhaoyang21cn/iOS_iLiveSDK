//
//  TCLiveVideoLayoutView.h
//  TRTC
//
//  Created by Tencent on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <ILiveSDK/ILiveRenderView.h>

@class TCLiveVideoElementView;

typedef enum {
    TCLiveRoomVideoLayoutStyle_1v3,
    TCLiveRoomVideoLayoutStyle_4_geizi,
}TCLiveRoomVideoLayoutStyle;

@protocol TCLiveVideoElementViewDelegate
- (void)tapHandle:(TCLiveVideoElementView *)view;
@end

@interface TCLiveVideoLayoutView : UIView
//画面布局样式
@property(nonatomic,assign) TCLiveRoomVideoLayoutStyle layoutStyle;
//显示日志信息
- (void)showLogView:(NSString *)qualityParams;
//关闭日志信息
- (void)closeLogView;
//设置美颜
- (void)setBeautyLevel:(NSInteger)level;
@end

@interface TCLiveVideoElementView: UIView
//展示userID
@property(nonatomic,strong)UILabel *userIdLabel;
//展示视频分辨率、帧率等信息
@property(nonatomic,strong)UILabel *videoInfoLable;
@property(nonatomic,weak)TCLiveVideoLayoutView<TCLiveVideoElementViewDelegate> *delegate;
//可拖动开关
- (void)ennableDraggable:(BOOL)draggable;
@end
