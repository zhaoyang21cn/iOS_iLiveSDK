//
//  IMSDKBuglyLog.h
//
//  Copyright © 2017年 tencent.com. All rights reserved.
//

#import <Foundation/Foundation.h>

// Log level for IMSDKBugly Log
typedef NS_ENUM(NSUInteger, IMSDKBuglyLogLevel) {
    IMSDKBuglyLogLevelSilent  = 0,
    IMSDKBuglyLogLevelError   = 1,
    IMSDKBuglyLogLevelWarn    = 2,
    IMSDKBuglyLogLevelInfo    = 3,
    IMSDKBuglyLogLevelDebug   = 4,
    IMSDKBuglyLogLevelVerbose = 5,
};
#pragma mark -

FOUNDATION_EXPORT void IMBLYLog(IMSDKBuglyLogLevel level, NSString *format, ...);

FOUNDATION_EXPORT void IMBLYLogv(IMSDKBuglyLogLevel level, NSString *format, va_list args);

#pragma mark -
#define IMSDKBugly_LOG_MACRO(_level, fmt, ...) [IMSDKBuglyLog level:_level tag:nil log:fmt, ##__VA_ARGS__]

#define IMBLYLogError(fmt, ...)   IMSDKBugly_LOG_MACRO(IMSDKBuglyLogLevelError, fmt, ##__VA_ARGS__)
#define IMBLYLogWarn(fmt, ...)    IMSDKBugly_LOG_MACRO(IMSDKBuglyLogLevelWarn,  fmt, ##__VA_ARGS__)
#define IMBLYLogInfo(fmt, ...)    IMSDKBugly_LOG_MACRO(IMSDKBuglyLogLevelInfo, fmt, ##__VA_ARGS__)
#define IMBLYLogDebug(fmt, ...)   IMSDKBugly_LOG_MACRO(IMSDKBuglyLogLevelDebug, fmt, ##__VA_ARGS__)
#define IMBLYLogVerbose(fmt, ...) IMSDKBugly_LOG_MACRO(IMSDKBuglyLogLevelVerbose, fmt, ##__VA_ARGS__)

#pragma mark - Interface
@interface IMSDKBuglyLog : NSObject

/**
 *    @brief  初始化日志模块
 *
 *    @param level 设置默认日志级别，默认BLYLogLevelSilent
 *
 *    @param printConsole 是否打印到控制台，默认NO
 */
+ (void)initLogger:(IMSDKBuglyLogLevel) level consolePrint:(BOOL) printConsole;

/**
 *    @brief 打印BLYLogLevelInfo日志
 *
 *    @param format   日志内容 总日志大小限制为：字符串长度30k，条数200
 */
+ (void)log:(NSString *)format, ...;

/**
 *    @brief  打印日志
 *
 *    @param level 日志级别
 *    @param message   日志内容 总日志大小限制为：字符串长度30k，条数200
 */
+ (void)level:(IMSDKBuglyLogLevel) level logs:(NSString *)message;

/**
 *    @brief  打印日志
 *
 *    @param level 日志级别
 *    @param format   日志内容 总日志大小限制为：字符串长度30k，条数200
 */
+ (void)level:(IMSDKBuglyLogLevel) level log:(NSString *)format, ...;

/**
 *    @brief  打印日志
 *
 *    @param level  日志级别
 *    @param tag    日志模块分类
 *    @param format   日志内容 总日志大小限制为：字符串长度30k，条数200
 */
+ (void)level:(IMSDKBuglyLogLevel) level tag:(NSString *) tag log:(NSString *)format, ...;

@end
