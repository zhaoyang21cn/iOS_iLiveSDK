#import <Foundation/Foundation.h>

// 注意！！！这个头文件里的所有接口都面向内部的，外面的都需要经过咨询和了解才能使用

/*!
 @discussion    信令通道类型
 */
typedef NS_ENUM(NSInteger, IMChannelType) {
    IMChannelTypeIMSDK   = 0,             ///< 默认值，使用imsdk通道
    IMChannelTypeImplementInternal = 1,   ///< 使用QAVSDK的Http/Https通道
    IMChannelTypeImplementExternal = 2,   ///< 用户自己实现通道
};


@protocol QAVAppChannel;

/*!
 @discussion    账号信息
 @warning       内部用的，外面的暂时不要用
 */
@interface QAVUserID : NSObject

/*!
 @abstract      uid 类型
*/
@property (nonatomic, copy) NSString *uidtype;

/*!
 @abstract      相对应的appid
 */
@property (nonatomic, assign) unsigned int userappid;
/*!
 @abstract      用户的账号
 */
@property (nonatomic, copy) NSString *userid;
/*!
 @abstract      相对应的数字账号
 */
@property (nonatomic, assign) unsigned long long tinyid;
/*!
 @abstract      相对应的数字账号
 */
@property (nonatomic, assign) unsigned long long uin;

@end

@interface KSLoginParam : NSObject{
    NSString*       accountType;        // 用户的账号类型
    NSString*       identifier;         // 用户名
    NSString*       userSig;            // 鉴权Token
    NSString*       appidAt3rd;          // App用户使用OAuth授权体系分配的Appid
    
    int             sdkAppId;           // 用户标识接入SDK的应用ID
}
/**
 *  用户的账号类型
 */
@property(nonatomic,retain) NSString* accountType;

/**
 * 用户名
 */
@property(nonatomic,retain) NSString* identifier;

/**
 *  鉴权Token
 */
@property(nonatomic,retain) NSString* userSig;

/**
 *  App用户使用OAuth授权体系分配的Appid
 */
@property(nonatomic,retain) NSString* appidAt3rd;


/**
 *  用户标识接入SDK的应用ID
 */
@property(nonatomic,assign) int sdkAppId;

@end


/*!
 @abstract      客户端与服务器交互的回调
 @param         result          返回的错误码
 @param         errorInfo       错误信息
 @param         response        服务器返回信息
 */
typedef void (^QAVCsCmdlCallback)(int result, NSString *errorInfo,
                                  NSData *response);

/*!
 @abstract      游戏场景客户端与服务器交互的回调
 @param         result          返回的错误码
 */
typedef void (^QAVGameReportCallback)(int result);
/*!

 @abstract      identifier与tinyid之间转换的回调
 @param         result          返回的错误码
 @param         List            返回的identifier或者tinyid集合
 */
typedef void (^QAVIDResponse)(int result, NSArray *List);

/*!
 @discussion    AVSDK业务协议通道自定义
 @warning       内部用的，外面的暂时不要用
 */
@protocol QAVAppChannel <NSObject>
@required

/*!
 @abstract      返回自己的数字账号。
 @return        返回自己的数字账号。
 */
- (UInt64)getTinyId;

/*!
 @abstract      App Server协议族对应的Cs信令请求。
 @param         callback        返回上报的block (^QAVCsCmdlCallback)
 @param         request         发送给服务器请求的数据
 @return        - YES:表示App已经实现了此接口。
                - NO:表示App没有实现此接口，SDK会调用IMSDK发包。
 */
- (BOOL)requestAppCmd:(NSData *)request result:(QAVCsCmdlCallback)callback;

/*!
 @abstract      App Server协议族对应的Cs信令请求。（空间专用）
 @param         callback        返回上报的block (^QAVCsCmdlCallback)
 @param         roomId          房间号
 @param         request         发送给服务器请求的数据
 @return        - YES:表示App已经实现了此接口。
 - NO:表示App没有实现此接口，SDK会调用IMSDK发包。
 */
- (BOOL)requestAppCmd:(NSData *)request roomId:(UInt32)roomId result:(QAVCsCmdlCallback)callback;

/*!
 @abstract      Info Server协议族对应的Cs信令
 @param         request         发送给服务器请求的数据
 @param         callback        返回上报的block (^QAVCsCmdlCallback)
 @return        - YES:表示App已经实现了此接口。
                - NO:表示App没有实现此接口，SDK会调用IMSDK发包。
 */
- (BOOL)requestInfoCmd:(NSData *)request result:(QAVCsCmdlCallback)callback;

/*!
 @abstract      上报 Server协议族对应的CS信令。
 @param         request         发送给服务器请求的数据
 @param         type            1表示结束数据上报。
 @param         callback        返回上报的block (^QAVCsCmdlCallback)
 @return        - YES:表示App已经实现了此接口。
                - NO:表示App没有实现此接口，SDK会调用IMSDK发包。
 */
- (BOOL)requestReportCmd:(NSData *)request
                    type:(unsigned int)type
                  result:(QAVCsCmdlCallback)callback;

/*!
 @abstract      其他Server协议族对应的CS信令。
 @param         request         发送给服务器请求的数据
 @param         name            "VideoCCSvc.opensdk"表示是配置系统服务器。
 @param         callback        返回上报的block (^QAVCsCmdlCallback)
 @return        - YES:表示App已经实现了此接口。
                - NO:表示App没有实现此接口，SDK会调用IMSDK发包。
 */
- (BOOL)requestCmd:(NSData *)request
           cmdName:(NSString *)name
            result:(QAVCsCmdlCallback)callback;


/*!
 @abstract      上报 Wup格式的协议对应的CS信令。
 @param         pWup            已经组好的wup格式的包
 @param         cmd             后台的命令字
 @param         pSeq             去重序列号
 @param         callback        返回上报的block (^QAVGameReportCallback)
 @return        - YES:表示App已经实现了此接口。
 - NO:表示App没有实现此接口，SDK会调用Http发包。
 */
- (BOOL)SendWupBufferBase:(unsigned char *)pWup
                      cmd:(NSString *)cmd
                      seq:(int *)pSeq
                   result:(QAVGameReportCallback)callback;


/*!
 @abstract      语音消息转文字的CS信令。
 @param         sdkappid           用户标识接入SDK的应用ID
 @param         host               域名
 @param         sign               签名
 @param         ptturl             有效的url
 @param         callback           返回上报的block (^QAVGameReportCallback)
 @return        - YES:表示App已经实现了此接口。
 - NO:表示App没有实现此接口，SDK会调用Http发包。
 */
- (BOOL)requestSendMessageCmdWithAppid:(int)sdkappid
                                  host:(NSString *)host
                                  sign:(NSString *)sign
                                ptturl:(NSString *)ptturl
                                result:(QAVCsCmdlCallback)callback;

@optional

/*!
 @abstract      将identifier转成tinyid。
 @param         arrList          存放QAVUserID的数组。
 @param         ret              返回的block (^QAVIDResponse)
 
 @return        成功返回YES，否则返回NO。
 */
- (BOOL)identifierToTinyId:(NSArray *)arrList ret:(QAVIDResponse)ret;

/*!
 @abstract      将tinyid转成identifier。
 @param         arrList          存放QAVUserID的数组。
 @param         ret              返回的block (^QAVIDResponse)
 
 @return        成功返回YES，否则返回NO。
 */
- (BOOL)tinyIDToIdentifier:(NSArray *)arrList ret:(QAVIDResponse)ret;


@end

/*!
 @discussion    外部接管sdk的log实现。
 */
@protocol QAVLogger <NSObject>
@required

/*!
 @discussion    获取sdk的log路径，例如写在Documents目录。
 */
- (NSString *)getLogPath;

@optional
/*!
 @discussion    是否写sdk的log
 */
- (BOOL)isLogWrite;

/**
 @brief 是否打印sdk的log到控制台
 */
/*!
 @discussion    是否打印sdk的log到控制台。
 */
- (BOOL)isLogPrint;

/*!
 @discussion    获取sdk的log文件最大大小。
 */
- (int)maxFileSize;

/*!
 @discussion    log上传到bugly。
 @param         content         上报的内容
 @param         lv              log等级
 */
- (void)logToReport:(NSString *)content level:(int)lv;

/*!
 @discussion    log上传到APP。
 @param         content         上报的内容
 @param         lv              log等级
 */
- (void)logToAPP:(NSString *)content level:(int)lv;


- (void)setIsEnablePrintLog:(BOOL)isEnable;
- (void)setIsEnableWriteLog:(BOOL)isEnable;

@end

/*!
 @discussion    业务通道和log自定义管理类。
 */
@interface QAVAppChannelMgr : NSObject

/*!
 @discussion    设置通道实例。
 */
+ (id<QAVAppChannel>)sharedInst;

/*!
 @discussion    设置log接管实例。
 */
+ (id<QAVLogger>)sharedLoggerInst;

/*!
 @discussion    设置外部的通道，一般不要设置。
 @param         channel         继承了QAVAppChannel的实例化对象
 */
+ (void)setExternalChannel:(id<QAVAppChannel>)channel;

/*!
 @discussion    设置外部的logger。
 @param         logger          继承了QAVLogger的实例化对象
 */
+ (void)setExternalLogger:(id<QAVLogger>)logger;


/*!
 @discussion    设置通道，默认为imsdk通道。
 @param         type          通道类型
 @see           IMChannelType
 */
+ (void)setIMChannelType:(IMChannelType)type;

/*!
 @discussion    获取当前的通道，如果不设置，默认为imsdk通道。
 @return        返回当前的通道
 */
+ (IMChannelType)getIMChannelType;

+ (void)setSSOHost:(NSString*) host;

@end

