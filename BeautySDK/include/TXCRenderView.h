//
//  RenderView.h
//  BeautyDemo
//
//  Created by kennethmiao on 17/5/9.
//  Copyright © 2017年 kennethmiao. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreMedia/CoreMedia.h>
#import "TXINotifyDelegate.h"
#import "TXEVideoTypeDef.h"

typedef NS_ENUM(NSInteger, TXERenderMode) {
    TXE_RENDER_MODE_FILL_SCREEN = 0,    // 图像铺满屏幕
    TXE_RENDER_MODE_FILL_EDGE            // 图像长边填满屏幕
};

typedef NS_ENUM(NSInteger, TXERenderEvent) {
    TXE_RENDER_EVENT_RENDER_FIRST_FRAME = 10001,
    TXE_RENDER_EVENT_RENDER_FRAME,
    TXE_RENDER_EVENT_RENDER_LAG,
};

@interface RenderStats : NSObject
@property(nonatomic, assign) uint64_t firstFrameRenderTS;
@property(nonatomic, assign) uint64_t blockCount;           //大于1000ms的卡顿次数
@property(nonatomic, assign) uint64_t blockTime;            //大于500ms的卡顿总时长 BG用于计算卡顿率，总卡顿时长/总播放时长。
@property(nonatomic, assign) uint64_t totalBlockCount;      //大于500ms的卡顿次数 
@property(nonatomic, assign) uint64_t maxBlockDuration;     //最大卡顿时长
@property(nonatomic, assign) uint64_t renderFrameCount;     //累计渲染帧数，用于统计播放帧率。
@end

typedef void(^LayoutSubviewCallBack)();

@interface TXCRenderView : UIView
@property(nonatomic, weak) id <TXINotifyDelegate> notifyDelegate;
@property(nonatomic, assign) TXERenderMode renderMode;
@property(nonatomic, assign) TXERotation rotation;
@property(nonatomic, assign) BOOL mirror;
@property(nonatomic, assign) BOOL renderable;
@property(nonatomic, strong) LayoutSubviewCallBack layoutSubviewCallBack;

- (void)renderFrame:(Byte *)data format:(TXEFrameFormat)format width:(NSInteger)width height:(NSInteger)height;
- (void)renderTexture:(GLuint)texture width:(NSInteger)width height:(NSInteger)height;
- (void)renderFrame:(CMSampleBufferRef)sampleBuffer;
- (RenderStats *)getStats;
- (void)resetStats;
- (void)snapshot:(void (^)(UIImage *))completionBlock;
@end
