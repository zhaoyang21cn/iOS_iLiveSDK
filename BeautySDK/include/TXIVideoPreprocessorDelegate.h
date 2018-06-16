//
//  TXIVideoPreprocessorDelegate.h
//  basic
//
//  Created by alderzhang on 2017/5/17.
//  Copyright © 2017年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <VideoToolbox/VideoToolbox.h>
#import "TXEVideoTypeDef.h"
#import "TXEVideoPreprocessorDef.h"

@protocol TXIVideoPreprocessorDelegate <NSObject>
@optional
/*
 * 添加水印前回调
 * @param   texture     纹理id
 * @param   width       宽
 * @param   height      高
 * @return  处理后纹理id
 */
- (GLuint)willAddWatermark:(GLuint)texture width:(NSInteger)width height:(NSInteger)height;

/**
 * 在OpenGL线程中回调，可以在这里释放创建的OpenGL资源
 */
- (void)onTextureDestoryed;

/*
 * 数据处理回调
 * @param   sampleBuffer 帧数据
 * @param   timeStamp    时间戳
 */
- (void)didProcessFrame:(CMSampleBufferRef)sampleBuffer timeStamp:(UInt64)timeStamp;

/*
 * 数据处理回调
 * @param   bytes       帧数据
 * @param   width       宽
 * @param   height      高
 * @param   format      输出数据格式
 * @param   timeStamp   时间戳
 */
- (void)didProcessFrame:(Byte *)bytes width:(NSInteger)width height:(NSInteger)height format:(TXEFrameFormat)format timeStamp:(UInt64)timeStamp;

/**
 * 人脸数据回调（启用了pitu模块才有效，开启pitu模块必须是打开动效或大眼瘦脸）
 * @prama points 人脸坐标
 */
- (void)onDetectFacePoints:(NSArray *)points;
@end
