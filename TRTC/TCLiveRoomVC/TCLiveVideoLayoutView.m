//
//  TCLiveVideoLayoutView.m
//  TRTC
//
//  Created by Tencent on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveVideoLayoutView.h"
#import <ILiveSDK/ILiveRoomManager.h>
#import <ILiveSDK/ILiveQualityData.h>
#import <TILLiveSDK/TILLiveManager.h>
#import "TCLiveRequestManager.h"
#import "UIColorEX.h"
#import "TXCVideoPreprocessor.h"
#define LIVE_VIDEO_NUM 4

@interface TCLiveVideoLayoutView () <QAVRemoteVideoDelegate,QAVLocalVideoDelegate,ILiveScreenVideoDelegate,ILVLiveAVListener,ILiveMemStatusListener,TCLiveVideoElementViewDelegate,TXIVideoPreprocessorDelegate>
@property(nonatomic,strong)NSMutableArray *liveVideos;
@property(nonatomic,strong)NSMutableArray *liveRnederView;
@property(nonatomic,strong)UITextView *logView;
@property(nonatomic,assign) BOOL isShowLogInfo;
@property(nonatomic,assign) NSDate *startTime;
@property (nonatomic, strong) TXCVideoPreprocessor *preProcessor;
@property (nonatomic, assign) Byte  *processorBytes;
@end

@implementation TCLiveVideoLayoutView

-(instancetype)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        self.backgroundColor = [UIColor clearColor];
        self.layoutStyle = TCLiveRoomVideoLayoutStyle_1v3;
        [self initialVideoViews];
        [[TILLiveManager getInstance] setAVListener:self];
        [[ILiveRoomManager getInstance] setLocalVideoDelegate:self];
        [[ILiveRoomManager getInstance] setRemoteVideoDelegate:self];
        [[ILiveRoomManager getInstance] setScreenVideoDelegate:self];
        //美颜处理器
        self.preProcessor = [[TXCVideoPreprocessor alloc] init];
        [self.preProcessor setDelegate:self];
        //记录进入时间
        self.startTime = [NSDate date];
    }
    return self;
}
//初始化每路视频view
- (void)initialVideoViews{
    _liveVideos = [NSMutableArray array];
    //自定义视窗数组
    NSArray *frames = [self customLayoutFrames];
    for (int i = 0; i < frames.count; i++) {
        TCLiveVideoElementView *view = [[TCLiveVideoElementView alloc] initWithFrame:CGRectZero];
        view.delegate = self;
        [self addSubview:view];
        [_liveVideos addObject:view];
    }
}
-(UITextView *)logView{
    if(!_logView){
        _logView = [[UITextView alloc] initWithFrame:CGRectMake(0, self.frame.size.height/5, self.frame.size.width/3*2, self.frame.size.height/2)];
        _logView.textColor = [UIColor colorWithRGBHex:0xFF4081];
        _logView.font = [UIFont systemFontOfSize:14];
        _logView.backgroundColor = [UIColor clearColor];
        _logView.editable = NO;
    }
    return _logView;
}

//自定义布局(开发者可自定义每个视窗的frame来实现，视窗个数即为frames数组个数)
- (NSArray *)customLayoutFrames{
    NSMutableArray *frames = [NSMutableArray array];
    if (TCLiveRoomVideoLayoutStyle_1v3 == _layoutStyle) {
        [frames removeAllObjects];
        int smallViewWidth = ((self.frame.size.height - 20  - 150 - 10 *2)/3)*(3.0/4.0);
        int smallViewHeight = (self.frame.size.height - 20  - 150 - 10 *2)/3;
        CGRect frame1 = self.bounds;
        CGRect frame2 = CGRectMake(self.bounds.size.width - 10 - smallViewWidth, 84 , smallViewWidth, smallViewHeight);
        CGRect frame3 = CGRectMake(self.bounds.size.width - 10 - smallViewWidth, frame2.origin.y + frame2.size.height + 10, smallViewWidth, smallViewHeight);
        CGRect frame4 = CGRectMake(self.bounds.size.width - 10 - smallViewWidth, frame3.origin.y + frame3.size.height + 10, smallViewWidth, smallViewHeight);
        
        [frames addObject:[NSValue valueWithCGRect:frame1]];
        [frames addObject:[NSValue valueWithCGRect:frame2]];
        [frames addObject:[NSValue valueWithCGRect:frame3]];
        [frames addObject:[NSValue valueWithCGRect:frame4]];
    }
    else if (TCLiveRoomVideoLayoutStyle_4_geizi == _layoutStyle) {
        [frames removeAllObjects];
        CGRect frame1 = CGRectMake(20, 50, (self.frame.size.width -40)/2, (self.frame.size.height - 100)/2);
        CGRect frame2 = CGRectMake(20 + (self.frame.size.width -40)/2 +1, 50, (self.frame.size.width -40)/2, (self.frame.size.height - 100)/2);
        CGRect frame3 = CGRectMake(20, 50 + (self.frame.size.height - 100)/2 +1, (self.frame.size.width -40)/2, (self.frame.size.height - 100)/2);
        CGRect frame4 = CGRectMake(20 + (self.frame.size.width -40)/2 +1, 50 + (self.frame.size.height - 100)/2 +1,(self.frame.size.width -40)/2, (self.frame.size.height - 100)/2);
        [frames addObject:[NSValue valueWithCGRect:frame1]];
        [frames addObject:[NSValue valueWithCGRect:frame2]];
        [frames addObject:[NSValue valueWithCGRect:frame3]];
        [frames addObject:[NSValue valueWithCGRect:frame4]];
    }
    return frames;
}

//添加视频画面
-(void)addLiveRenderView:(ILiveRenderView *)renderView{
    NSArray *frames = [self customLayoutFrames];
    for (int i = 0; i < _liveVideos.count;i++) {
        TCLiveVideoElementView *videoView = _liveVideos[i];
        videoView.frame = [frames[i] CGRectValue];
        if (![self getLiveRenderViewOnElementView:videoView]) {
            renderView.frame = videoView.bounds;
            [videoView insertSubview:renderView atIndex:0];
            break;
        }
    }
}
//移除视频画面
- (void)removeLiverRenderVeiw:(ILiveRenderView *)renderView{
    if ([[renderView superview] isKindOfClass:[TCLiveVideoElementView class]]) {
        TCLiveVideoElementView *videoView = (TCLiveVideoElementView *)[renderView superview];
        videoView.frame = CGRectZero;
        [renderView removeFromSuperview];
    }
}

//画面切换
- (void)tapHandle:(TCLiveVideoElementView *)view{
    NSUInteger index = [_liveVideos indexOfObject:view];
    TCLiveVideoElementView *bigView = _liveVideos[0];
    ILiveRenderView *bigRenderView = [self getLiveRenderViewOnElementView:bigView];
    ILiveRenderView *renderView = [self getLiveRenderViewOnElementView:view];
    if (index > 0) {
        [UIView animateWithDuration:0.5 animations:^{
            bigView.frame = view.frame;
            view.frame = [[self customLayoutFrames][0] CGRectValue];
            bigRenderView.frame = bigView.bounds;
            renderView.frame = view.bounds;
            [self.liveVideos exchangeObjectAtIndex:0 withObjectAtIndex:index];
            [self exchangeSubviewAtIndex:0 withSubviewAtIndex:index];
        } completion:^(BOOL finished) {
            
        }];
        
    }
    
}
//获取指定视窗的渲染view
- (ILiveRenderView *)getLiveRenderViewOnElementView:(TCLiveVideoElementView *)elementView{
    ILiveRenderView *renderView = nil;
    for (id view in [elementView subviews]) {
        if ([view isKindOfClass:[ILiveRenderView class]]) {
            renderView = view;
        }
    }
    return renderView;
}
-(void)setBeautyLevel:(NSInteger)level{
    [self.preProcessor setBeautyLevel:level];
}
#pragma mark - ILiveMemStatusListener
//房间成员状态变化事件
- (BOOL)onEndpointsUpdateInfo:(QAVUpdateEvent)event updateList:(NSArray *)endpoints{
    if (endpoints.count <= 0) {
        return NO;
    }
    for (QAVEndpoint *endoption in endpoints) {
        switch (event) {
            case QAV_EVENT_ID_ENDPOINT_HAS_CAMERA_VIDEO:
            {
                ILiveFrameDispatcher *frameDispatcher = [[ILiveRoomManager getInstance] getFrameDispatcher];
                ILiveRenderView *renderView = [frameDispatcher addRenderAt:CGRectZero forIdentifier:endoption.identifier srcType:QAVVIDEO_SRC_TYPE_CAMERA];
                renderView.isRotate = NO;
                renderView.autoRotate = NO;
                renderView.isMirror = YES;
                renderView.identifier = endoption.identifier;
                renderView.diffDirectionRenderMode = ILIVERENDERMODE_SCALEASPECTFILL;
                if ([[TCLiveRequestManager getInstance].userID isEqualToString:endoption.identifier]) {
                    renderView.rotateAngle = ILIVEROTATION_90;
                }
                [self addLiveRenderView:renderView];
            }
                break;
            case QAV_EVENT_ID_ENDPOINT_NO_CAMERA_VIDEO:
            {
                ILiveFrameDispatcher *frameDispatcher = [[ILiveRoomManager getInstance] getFrameDispatcher];
                ILiveRenderView *renderView = [frameDispatcher removeRenderViewFor:endoption.identifier srcType:QAVVIDEO_SRC_TYPE_CAMERA];
                [self removeLiverRenderVeiw:renderView];
            }
                break;
                
            default:
                break;
        }
    }
    return YES;
}
/***本地画面帧数据回调***/
#pragma mark - QAVLocalVideoDelegate
- (void)OnLocalVideoPreview:(QAVVideoFrame *)frameData{
    [self showElementVideoInfoWithVideoFrame:frameData];
}

- (void)OnLocalVideoPreProcess:(QAVVideoFrame *)frameData{
    
    
    [self.preProcessor setOutputSize:CGSizeMake(frameData.frameDesc.width, frameData.frameDesc.height)];
    [self.preProcessor setCropRect:CGRectMake(0, 0,frameData.frameDesc.width, frameData.frameDesc.height)];
    [self.preProcessor processFrame:frameData.data width:frameData.frameDesc.width height:frameData.frameDesc.height orientation:TXE_ROTATION_0 inputFormat:TXE_FRAME_FORMAT_NV12 outputFormat:TXE_FRAME_FORMAT_NV12];
    //将处理完的数据拷贝到原来的地址空间，如果是同步处理，此时会先执行didProcessFrame回调
    if(self.processorBytes){
        memcpy(frameData.data, self.processorBytes, frameData.frameDesc.width * frameData.frameDesc.height * 3 / 2);
    }
}

- (void)OnLocalVideoRawSampleBuf:(CMSampleBufferRef)buf result:(CMSampleBufferRef *)ret{
    
}

/***远端视频帧数据回调***/
#pragma mark - QAVRemoteVideoDelegate
- (void)OnVideoPreview:(QAVVideoFrame *)frameData{
    [self showElementVideoInfoWithVideoFrame:frameData];
}
/***远端屏幕分享帧数据回调***/
#pragma mark - ILiveScreenVideoDelegate

-(void)onScreenVideoPreview:(QAVVideoFrame *)frameData{
    [self showElementVideoInfoWithVideoFrame:frameData];
}
/***首帧回调***/
#pragma mark - ILVLiveAVListener
- (void)onFirstFrameRecved:(int)width height:(int)height identifier:(NSString *)identifier srcType:(avVideoSrcType)srcType;{
 
}
/***美颜相关处理***/
#pragma mark - TXIVideoPreprocessorDelegate
- (void)didProcessFrame:(Byte *)bytes width:(NSInteger)width height:(NSInteger)height format:(TXEFrameFormat)format timeStamp:(UInt64)timeStamp
{
    self.processorBytes = bytes;
}
#pragma mark - LOG相关
- (void)showLogView:(NSString *)qualityParams{
    NSString *role = [[[[qualityParams componentsSeparatedByString:@"ControlRole="] lastObject] componentsSeparatedByString:@","] firstObject];

    self.logView.text = [NSString stringWithFormat:@"发送速率:%ldkbps  丢包率:%.1f%%\n接收速率:%ldkbps  丢包率:%.1f%%\n应用CPU:%.1f%%  系统CPU:%.1f%%\n角色:%@\nSDKAPPID:%d\nSDKVersion:%@",(long)[[ILiveRoomManager getInstance] getQualityData].sendRate,[[ILiveRoomManager getInstance] getQualityData].sendLossRate/100.0,(long)[[ILiveRoomManager getInstance] getQualityData].recvRate,[[ILiveRoomManager getInstance] getQualityData].recvLossRate/100.0,[[ILiveRoomManager getInstance] getQualityData].appCPURate/100.0,[[ILiveRoomManager getInstance] getQualityData].sysCPURate/100.0,role,[TCLiveRequestManager getInstance].sdkAppID,[[ILiveSDK getInstance] getVersion]];
    if(![_logView superview]){
        [self addSubview:_logView];
    }
    self.isShowLogInfo = YES;
}

- (void)showElementVideoInfoWithVideoFrame:(QAVVideoFrame *)frame{
    if(!self.isShowLogInfo){
        return;
    }
    NSString *userId = frame.identifier;
    NSString *fps = @"";
    if (userId.length == 0){
        userId = [TCLiveRequestManager getInstance].userID;
    }
    else{
        NSString *qualityParams = [[[ILiveSDK getInstance] getAVContext].room getQualityTips];
        NSString *decode = [[qualityParams componentsSeparatedByString:@"音频部分:========"] firstObject];
        NSString *itemDecode = [[[[decode componentsSeparatedByString:[NSString stringWithFormat:@"成员:%@",userId]] lastObject] componentsSeparatedByString:@"接收参数"] firstObject];
        fps = [[[[itemDecode componentsSeparatedByString:@"FPS="] lastObject] componentsSeparatedByString:@","] firstObject];
    }
    int width = frame.frameDesc.width;
    int height = frame.frameDesc.height;
    for (int i = 0; i < _liveVideos.count;i++) {
        TCLiveVideoElementView *videoView = _liveVideos[i];
        ILiveRenderView *renderView = [self getLiveRenderViewOnElementView:videoView];
        if ([renderView.identifier isEqualToString:userId]) {
            if ([userId isEqualToString:[TCLiveRequestManager getInstance].userID]) {
                videoView.videoInfoLable.text = [NSString stringWithFormat:@"%dx%d",width,height];
            }
            else{
                videoView.videoInfoLable.text = [NSString stringWithFormat:@"%dx%d  fps:%d",width,height,[fps intValue]/10];
            }
            videoView.userIdLabel.text = [NSString stringWithFormat:@"%@",userId];
            
        }
    }
}

- (void)closeLogView{
    [_logView removeFromSuperview];
    _logView = nil;
    for (int i = 0; i < _liveVideos.count;i++) {
        TCLiveVideoElementView *videoView = _liveVideos[i];
        [videoView.userIdLabel removeFromSuperview];
        videoView.userIdLabel = nil;
        [videoView.videoInfoLable removeFromSuperview];
        videoView.videoInfoLable = nil;
    }
    self.isShowLogInfo = NO;
}
@end

@interface TCLiveVideoElementView() <UIGestureRecognizerDelegate>
@property(nonatomic,strong) UIPanGestureRecognizer *panGesture;
@end

@implementation TCLiveVideoElementView

-(instancetype)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        [self ennableDraggable:YES];
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapGestuer:)];
        [self addGestureRecognizer:tap];
    }
    return self;
}
- (UILabel *)userIdLabel{
    if (!_userIdLabel) {
        _userIdLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 12, self.bounds.size.width, 8)];
        _userIdLabel.textColor = [UIColor colorWithRGBHex:0xFF4081];
        _userIdLabel.textAlignment = NSTextAlignmentLeft;
        _userIdLabel.font = [UIFont systemFontOfSize:9];
        [self addSubview:_userIdLabel];  
    }
    return _userIdLabel;
}
-(UILabel *)videoInfoLable{
    if (!_videoInfoLable) {
        _videoInfoLable = [[UILabel alloc] initWithFrame:CGRectMake(0, 0 , self.bounds.size.width, 12)];
        _videoInfoLable.textColor = [UIColor colorWithRGBHex:0xFF4081];
        _videoInfoLable.textAlignment = NSTextAlignmentLeft;
        _videoInfoLable.font = [UIFont systemFontOfSize:13];
        [self addSubview:_videoInfoLable];
    }
    return _videoInfoLable;
}
- (void)layoutSubviews{
    if (self.bounds.size.width == [UIScreen mainScreen].bounds.size.width) {
        _videoInfoLable.frame = CGRectMake(0, 84 , self.bounds.size.width, 12);
        _userIdLabel.frame = CGRectMake(0, 12 + 84, self.bounds.size.width, 8);
    }
    else{
        _videoInfoLable.frame = CGRectMake(0, 0 , self.bounds.size.width, 12);
        _userIdLabel.frame = CGRectMake(0, 12, self.bounds.size.width, 8);
    }
}
//可拖动能力
-(void)ennableDraggable:(BOOL)draggable {
    [self setUserInteractionEnabled:YES];
    [self removeConstraints:self.constraints];
    for (NSLayoutConstraint *constraint in self.superview.constraints) {
        if ([constraint.firstItem isEqual:self]) {
            [self.superview removeConstraint:constraint];
        }
    }
    [self setTranslatesAutoresizingMaskIntoConstraints:YES];
    if (draggable) {
        if (!_panGesture) {
            _panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
            _panGesture.delegate = self;
            [self addGestureRecognizer:_panGesture];
        }
    }else{
        if (_panGesture) {
            _panGesture = nil;
            [self removeGestureRecognizer:_panGesture];
        }
    }
}
- (void)tapGestuer:(UITapGestureRecognizer *)gesture{
    if ([_delegate respondsToSelector:@selector(tapHandle:)]) {
        [_delegate tapHandle:self];
    }
}
- (void)pan:(UIPanGestureRecognizer *)panGestureRecognizer {
    switch (panGestureRecognizer.state) {
        case UIGestureRecognizerStateBegan: {
            [self dragging:panGestureRecognizer];
        }
            break;
        case UIGestureRecognizerStateChanged: {
            [self dragging:panGestureRecognizer];
        }
            break;
        default:
            break;
    }
}

-(void)dragging:(UIPanGestureRecognizer *)panGestureRecognizer {
    UIView *view = panGestureRecognizer.view;
    CGPoint translation = [panGestureRecognizer translationInView:view.superview];
    CGPoint center = CGPointMake(view.center.x + translation.x, view.center.y + translation.y);
    //不能拖过边界
    CGSize size = view.frame.size;
    CGSize superSize = view.superview.frame.size;
    CGFloat width = size.width;
    CGFloat height = size.height;
    CGFloat superWidth = superSize.width;
    CGFloat superHeight = superSize.height;
    center.x = (center.x<width/2)?width/2:center.x;
    center.x = (center.x+width/2>superWidth)?superWidth-width/2:center.x;
    center.y = (center.y<height/2)?height/2:center.y;
    center.y = (center.y+height/2>superHeight)?superHeight-height/2:center.y;

    [view setCenter:center];
    [panGestureRecognizer setTranslation:CGPointZero inView:view.superview];
}
@end

