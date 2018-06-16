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
@property(nonatomic,assign) TCLiveRoomVideoLayoutStyle layoutStyle;
//添加画面渲染
- (void)addLiveRenderView:(ILiveRenderView *)renderView;
//移除画面渲染
- (void)removeLiverRenderVeiw:(ILiveRenderView *)renderView;
- (void)showLogView:(NSString *)qualityParams;
- (void)closeLogView;
- (void)showElementVideoInfoWithVideoFrame:(QAVVideoFrame *)frame;
- (void)setBeautyLevel:(NSInteger)level;
@end

@interface TCLiveVideoElementView: UIView
@property(nonatomic,strong)UILabel *userIdLabel;
@property(nonatomic,strong)UILabel *videoInfoLable;
@property(nonatomic,weak)TCLiveVideoLayoutView<TCLiveVideoElementViewDelegate> *delegate;
//可拖动开关
- (void)ennableDraggable:(BOOL)draggable;
@end
