//
//  TXCVideoPreProcessor.h
//  beauty
//
//  Created by kennethmiao on 17/3/14.
//  Copyright © 2017年 kennethmiao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <VideoToolbox/VideoToolbox.h>
#import <UIKit/UIKit.h>
#import "TXEVideoPreprocessorDef.h"
#import "TXIVideoPreprocessorDelegate.h"
#import "TXINotifyDelegate.h"
#import "TXEVideoTypeDef.h"


@interface TXCVideoPreprocessor : NSObject

/*
 * 设置输入裁剪区域
 */
@property (nonatomic, assign) CGRect cropRect;

/*
 * 设置裁剪后旋转角度
 */
@property (nonatomic, assign) TXERotation rotateAngle;

/*
 * 设置旋转后缩放大小
 */
@property (nonatomic, assign) CGSize outputSize;

/*
 * 设置缩放后是否镜像
 */
@property (nonatomic, assign) BOOL mirror;

/*
 * 设置水印是否镜像
 */
@property (nonatomic, assign) BOOL waterMirror;

/*
 * 设置绿幕是否镜像
 */
@property (nonatomic, assign) BOOL greenMirror;

/*
 * 是否异步回调（默认NO）
 */
@property (nonatomic, assign) BOOL isAsync;

/*
 * 设置代理
 * @param   delegate   代理
 */
- (void)setDelegate:(id<TXIVideoPreprocessorDelegate>)delegate;

/*
 * 设置通知
 * @param   notify   通知
 */
- (void)setNotify:(id<TXINotifyDelegate>)notify;

/*
 * 预处理数据
 * @param   sampleBuffer 帧数据（420f和y420）
 * @param   orientation  人脸识别方向（无需人脸识别：TXE_ROTATION_0）
 * @param   outputFormat 输出数据格式
 * @return  int          是否调用成功，0成功，-1失败
 */
- (int)processFrame:(CMSampleBufferRef)sampleBuffer orientation:(TXERotation)orientation outputFormat:(TXEFrameFormat)outputFormat;

/*
 * 预处理数据
 * @param   data         帧数据
 * @param   width        宽
 * @param   height       高
 * @param   orientation  人脸识别方向（无需人脸识别：TXE_ROTATION_0）
 * @param   inputFormat  输入数据格式
 * @param   outputFormat 输出数据格式
 * @return  int          是否调用成功，0成功，-1失败
 */
- (int)processFrame:(Byte *)data width:(NSInteger)width height:(NSInteger)height orientation:(TXERotation)orientation inputFormat:(TXEFrameFormat)inputFormat outputFormat:(TXEFrameFormat)outputFormat;

/*
 * 设置美颜风格
 * @param   level    设置美颜类型，默认TXE_BEAUTY_TYPE_SMOOTH
 */
- (void)setBeautyStyle:(TXEBeautyStyle)style;

/*
 * 设置美颜（0-10）
 * @param   level    美颜程度，0表示原图
 */
- (void)setBeautyLevel:(NSInteger)level;

/*
 * 设置美白（0-10）
 * @param   level    美白程度，0表示原图
 */
- (void)setWhitenessLevel:(NSInteger)level;

/*
 * 设置红润（0-10）
 * @param   level    红润程度，0表示原图
 */
- (void)setRuddinessLevel:(NSInteger)level;

/*
 * 设置滤镜
 * @param   type  滤镜类型
 */
- (void)setFilterType:(TXEFilterType)type;

/*
 * 设置滤镜
 * @param   imagePath  滤镜资源路径
 */
- (void)setFilterImage:(NSString *)imagePath;

/*
 * 设置滤镜
 * @param   image  滤镜图片
 */
- (void)setFilterUIImage:(UIImage *)image;

/*
 * 设置滤镜融合度（0-1）
 * @param   level    滤镜融合度
 */
- (void)setFilterMixLevel:(CGFloat)level;

/*
 * 设置绿幕
 * @param   file  绿幕文件路径
 */
- (void)setGreenScreenFile:(NSString *)file;

/*
 * 设置大眼（0-10）
 * @param   level    大眼程度
 */
- (void)setEyeScaleLevel:(NSInteger)level;

/*
 * 设置瘦脸（0-10）
 * @param   level    瘦脸程度
 */
- (void)setFaceSlimLevel:(NSInteger)level;

/*
 * 设置美型（0-10）
 * @param   level    美型程度
 */
- (void)setFaceBeautyLevel:(NSInteger)level;

/*
 * 设置V脸（0-10）
 * @param   level    V脸程度
 */
- (void)setFaceVLevel:(NSInteger)level;

/*
 * 设置下巴拉伸或收缩（-10-10，0为正常）
 * @param   level    伸缩程度
 */
- (void)setChinLevel:(NSInteger)level;

/*
 * 设置短脸（0-10）
 * @param   level    短脸程度
 */
- (void)setFaceShortLevel:(NSInteger)level;

/*
 * 设置瘦鼻（0-10）
 * @param   level    瘦鼻程度
 */
- (void)setNoseSlimLevel:(NSInteger)level;

/*
 * 设置动效
 * @param   templatePath  动效资源路径
 */
- (void)setMotionTemplate:(NSString *)templatePath;

/*
 * 设置水印
 * @param   view  水印view
 */
- (void)setWaterMark:(UIView *)view;

/*
 * 获取版本号
 */
+ (NSString *)getVersion;
@end
