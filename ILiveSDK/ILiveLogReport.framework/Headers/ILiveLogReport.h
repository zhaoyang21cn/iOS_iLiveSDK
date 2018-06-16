//
//  ILiveLogReport.h
//  ILiveLogReport
//
//  Created by wilderliao on 2018/3/26.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ILiveLogReport : NSObject

+ (instancetype)shareInstance;

/**
 初始化

 @param sdkAppId sdkappid
 @param ver sdk版本号
 */
- (void)initSdk:(int)sdkAppId sdkVersion:(NSString *)ver;

/**
 事件上报

 @param eventId 事件id
 @param rTime 接口调用耗时时长(选填，不统计则填0)
 @param evtTime 事件持续时长(选填，不统计则填0)
 @param roomId 房间id(选填，不统计则填0)
 @param ext 扩展信息(选填，不上报填nil)
 @param errCode 错误码
 @param errMsg 错误信息
 */
+ (void)reportEvent:(int)eventId retTime:(int)rTime eventTime:(int)evtTime roomId:(int)roomId extInfo:(NSString *)ext code:(int)errCode msg:(NSString *)errMsg;

/**
 日志上报

 @param logInfo 日志信息
 */
+ (void)reportLog:(NSString *)logInfo;

@end
