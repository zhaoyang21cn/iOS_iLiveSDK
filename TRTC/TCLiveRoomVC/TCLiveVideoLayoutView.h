//
//  TCLiveVideoLayoutView.h
//  TRTC
//
//  Created by ericxwli on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <ILiveSDK/ILiveRenderView.h>

@class TCLiveVideoElementView;

typedef enum {
    TCLiveRoomVideoLayoutStyle_1v1 = 0,
    TCLiveRoomVideoLayoutStyle_1v2,
    TCLiveRoomVideoLayoutStyle_1v2_gezi,
    TCLiveRoomVideoLayoutStyle_1v3,
    TCLiveRoomVideoLayoutStyle_1v3_gezi,
    TCLiveRoomVideoLayoutStyle_4_geizi,
}TCLiveRoomVideoLayoutStyle;

@protocol TCLiveVideoElementViewDelegate
- (void)tapHandle:(TCLiveVideoElementView *)view;
@end

@interface TCLiveVideoLayoutView : UIView
@property(nonatomic,assign) TCLiveRoomVideoLayoutStyle layoutStyle;
@property(nonatomic,strong)NSDictionary *roomInfo;
-(void)addLiveRenderView:(ILiveRenderView *)renderView;
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
- (void)ennableDraggable:(BOOL)draggable;  //可拖动开关
@end
