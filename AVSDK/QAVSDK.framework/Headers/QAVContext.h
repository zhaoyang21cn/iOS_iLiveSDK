/*!
 sdk_version = 1.9.8.8
 */
#import "QAVAppChannel.h"
#import "QAVAudioCtrl.h"
#import "QAVCommon.h"
#import "QAVEndpoint.h"
#import "QAVError.h"
#import "QAVRoomMulti.h"
#import "QAVSpearEnginCtrl.h"
#import "QAVAudioEffectCtrl.h"
#import "QAVVideoCtrl.h"
#import <Foundation/Foundation.h>
/*!
 @discussion    QAVContext对象配置信息
 */
@interface QAVContextStartParam : NSObject
/*!
 @abstract      用户的账号类型
 */
@property (copy, nonatomic) NSString *accountType;
/*!
 @abstract      用户名
 */
@property (copy, nonatomic) NSString *identifier;
/*!
 @abstract      App用户使用OAuth授权体系分配的Appid
 */
@property (copy, nonatomic) NSString *appidAt3rd;
/*!
 @abstract      用户标识接入SDK的应用ID
 */
@property (assign, nonatomic) int sdkAppId;
/*!
 @abstract      Spear引擎参数设置方式，默认为QAVSpearEngineCtrlTypeCloud。
 */
@property (assign, nonatomic) QAVSpearEngineCtrlType engineCtrlType;
@end

/*!
 @discussion    QAVContext通用事件委托
 */
@protocol QAVEventDelegate <NSObject>

@optional

/*!
 @abstract      返回QAVContext的一些通用事件通知
 @discussion    此函数用来返回QAVContext的一些通用事件通知
 @param         type      事件类型
                0 ---- 心跳包超时事件
 @param         message   事件描述
 */
- (void)onEvent:(int)type Message:(NSString *)message;

@end

/*!
 @discussion    QAVContext的状态类型
 */
typedef NS_ENUM(NSInteger, QAVContextState) {
    QAVContextStateStop     = 0,    ///< 初始状态，QAVContext的createInstance成功之后，调用QAVContext的stop之后均为此状态。
    QAVContextStateStarting = 1,    ///< 正在启动，APP调用QAVContext的start之后，QAVContext会从QAVContextStateStop切换到此状态。
    QAVContextStateStartOK  = 2,    ///< 已经启动，QAVContext start结束之后，QAVContext会从QAVContextStateStarting切换到此状态。
};
/*!
 @discussion    QAVContext对象封装类，代表着一个SDK运行实例
 */
@interface QAVContext : NSObject
/*!
 @abstract      QAVContext对象配置信息。
 */
@property (retain, nonatomic) QAVContextStartParam *param;
/*!
 @abstract      QAVContext当前的状态
 */
@property (readonly, nonatomic) QAVContextState state;
/*!
 @abstract      音视频房间(只有进入房间后以下属性才有用)
 */
@property (readonly, nonatomic) QAVRoomMulti *room;
/*!
 @abstract      音频控制器
 */
@property (readonly, nonatomic) QAVAudioCtrl *audioCtrl;
/*!
 @abstract      音效控制器
 */
@property (readonly, nonatomic) QAVAudioEffectCtrl* audioEffectCtrl;
/*!
 @abstract      视频控制器
 */
@property (readonly, nonatomic) QAVVideoCtrl *videoCtrl;
/*!
 @abstract      Spear流控引擎模式云配置方式
 */
@property (readonly, nonatomic) QAVCloudSpearEngineCtrl *cloudSpearCtrl;
/*!
 @abstract      Spear流控引擎模式自定义方式
 */
@property (readonly, nonatomic) QAVCustomSpearEngineCtrl *customSpearCtrl;


@property (assign, nonatomic) id<QAVEventDelegate> avDelegate;



/*!
 @abstract      获得SDK的版本信息。
 @return        SDK的版本号信息由四个数字组成，数字间以小数点分隔，四个数字的含义依次是：
                - 主版本号
                - 子版本号
                - 修订版本号
                - 编译版本号
 @warning       SDK在特性变化和Bug修复之后，会升级版本信息重新发布。
 */
+ (NSString *)getVersion;
/*!
 @abstract      创建QAVContext对象。
 @discussion    App使用SDK总是从CreateContext开始的，该静态成员函数返回一个新的QAVContext实例。
 @return        成功则返回QAVContext的实例指针；否则返回nil。
 @warning       - 创建成功之后，App需要进一步调用StartContext来启动QAVContext对象。<br/>
                - App必须保证QAVContext实例的唯一性，<br/>
                - 同时启动多个实例会因为设备占用等原因，出现无法预料的结果。<br/>
 */
+ (QAVContext *)CreateContext;
/*!
 @abstract      销毁目前的QAVContext的单例对象。
 
 @discussion    需要在CreateContext之后才能调用。
 */
- (void)destroy;
/*!
 @abstract      启动QAVContext对象。
 @discussion    StartContext是一个异步操作，内部会启动工作线程，创建各种内部对象。
 
 @param         param           QAVContext对象配置信息
 @param         completion      返回启动Context的结果的block (^QAVCompletionHandler)。
 
 @see           QAVContextStartParam  QAVCompletionHandler
 @warning       - 只有当异步操作启动成功的时候，才会通过回调函数异步返回执行结果；<br/>
                - 当异步操作启动失败的时候，直接通过方法的返回值返回错误，不会再触发回调函数；<br/>
                - App收到block回调的时候，表示StartContext执行完毕；<br/>
                - startContext执行成功之后，App才能进一步调用QAVContext的其他成员函数；<br/>
                - startContext执行成功之后，App必须调用stopContext来终止QAVContext对象。<br/>
 @warning       以下两点约定也适用于SDK的其他异步操作接口：<br/>
                - 只有当异步操作启动成功的时候，才会通过回调函数异步返回执行结果；<br/>
                - 当异步操作启动失败的时候，直接通过方法的返回值返回错误，不会再触发回调函数。<br/>
 @warning       和账号是相关的，当通过IMSDK切换账号时，需要从新stop、start SDK。
 */
- (void)startWithParam:(QAVContextStartParam *)param
            completion:(QAVCompletionHandler)completion;
/*!
 @abstract      终止QAVContext对象。
 @discussion    stop是一个异步操作，内部会终止工作线程，销毁各种内部对象。
 @return        成功返回QAV_OK, 其他情况请参照QAVResult。
 
 @see           QAVResult
 @warning       - App应该设法保证startContext和stopContext的配对调用；<br/>
                - stopContext如果当前还没退出音视频房间，会结束失败，要先调用ExitRoom；<br/>
 */
- (QAVResult)stop;
/*!
 @abstract      进入音视频房间。
 @discussion    enterRoom: delegate:是一个异步操作，内部会连接服务器，申请进入音视频房间。
 @param         param           进入房间所需的参数，必须传递QAVMultiParam类型的参数。
 @param         dlg             房间委托对象
 
 @see           QAVMultiParam
 @warning       EnterRoom必须在StartContext执行成功之后调用才有效。<br/>
                异步操作执行结果通过继承QAVRoomDelegate的委托对象的OnEnterRoomComplete()返回。<br/>
                目前同一时刻只支持进入一个音视频房间，所以：<br/>
                - 调用EnterRoom之前，必须先退出上一个音视频房间；<br/>
                - 如果当前AVContext中已经存在一个音视频房间，调用EnterRoom会同步返回失败。<br/>
 */
- (void)enterRoom:(QAVMultiParam *)param delegate:(id<QAVRoomDelegate>)dlg;
/*!
 @abstract      平滑切换房间
 @discussion    switchRoomWithChangedInfo:是一个异步操作，内部会在不释放音视频设备的情况下连接。<br/>
 服务器切换音视频房间，此切房方案相较于此前的方案"退旧房进新房",速度上会快很多。<br/>
 @param         param      新房间相比原房间发生变化的信息
 */
- (void)switchRoomWithChangedInfo:(QAVChangeRoomInfo *)param;
/*!
 @abstract      退出音视频房间。
 @discussion    exitRoom是一个异步操作，内部会连接服务器，申请退出音视频房间。
 @return        - 返回值表示异步操作启动是否成功；
                - 异步操作执行结果通过继承QAVRoomDelegate的委托对象的OnExitRoomComplete()返回。
                - QAV_OK 启动成功。
                - 其他值 启动失败。
 
 @see           QAVResult
 */
- (QAVResult)exitRoom;
/*!
 @abstract      平滑切换房间
 @discussion    switchRoom:是一个异步操作，内部会在不释放音视频设备的情况下连接。<br/>
                服务器切换音视频房间，此切房方案相较于此前的方案"退旧房进新房",速度上会快很多。<br/>
 @param         relationId      业务侧的房间id
 */
- (void)switchRoom:(int)relationId;
/*!
 @abstract      设置App version
 @discussion    同步操作。<br/>
 @param         appVersion      业务侧的应用版本号
 */
- (void)setAppVersion:(NSString*)appVersion;
/*!
 @abstract      设置sdk log等级
 @discussion    1: error 用于关键日志; 2: info 用于提示信息; 3: debug 用于开发调试; 4: verbose 用于高频打印类信息。
 @param         level      log等级
 @return        - AV_OK：设置成功； <br/>
                - AV_ERR_INVALID_ARGUMENT：设置等级非上述范围
 */
- (int)setLogLevel:(int)level;
/*!
 @abstract      获取sdk当前的log等级
 @return        - 取值范围:正常1-4; <br/>
                - 发生错误-1
 */
- (int)getLogLevel;
/*!
 @abstract      在进房前预览前初始化摄像头的参数，避免与进房后参数不对导致闪屏
 @discussion    如果需要在进房前做enableCamera的操作，则可以在调用enableCamera之前调用该接口，其他情况不要调用
 @return        成功返回QAV_OK, 其他情况请参照QAVResult。
 */
- (QAVResult)initCameraSettingWithWidth:(int)width 
                                 height:(int)height fps:(int)fps;
/*!
 @abstract      在进房前预览前利用房间后的流控角色初始化摄像头的参数，避免与进房后参数不对导致闪屏，注意要保证设置的角色与房间内的一致
 @discussion    如果需要在进房前做enableCamera的操作，则可以在调用enableCamera之前调用该接口，其他情况不要调用
 @return        成功返回QAV_OK, 其他情况请参照QAVResult。
 */
- (QAVResult)initCameraSettingWithControlRole:(NSString *)controlRole;
/*!
 *  设置音视频引擎的参数
 *  设置音视频引擎的参数的接口调用时机取决于key对应的调用时机
 *
 *  音视频引擎的参数(key)取值如下:
 *
 *  | 参数                      描述                                                  |
 *  |------------------------|-------------------------------------------------------|
 *  | RecvMixStreamCount     | 设置客户端的最大同时语音流混音的路数,假如客户端需要支持多于6个人同时讲话的或者限制同时讲话的人数,则需要设置这个值。 默认值为6路。 设置场景: 进入房间之前或者打开音频设备预览之前（若在进房之后设置只会在下次进房之后才生效）|
 *  | MixAudioFadeInTime     | 设置伴奏的淡入时间，支持设置发送混音输入,扬声器混音输入,同步发送混音输入的淡入时间 单位：ms；默认值为0。  设置场景: 注册音频数据类型的回调之前 |
 *  | MixAudioFadeOutTime    | 设置伴奏的淡出时间，支持设置发送混音输入,扬声器混音输入,同步发送混音输入的淡出时间 单位：ms；默认值为0。  设置场景: 注册音频数据类型的回调之前 |
 *  | ANSLevel               | 设置背景噪音抑制，ANS可探测出背景固定频率的杂音并消除背景噪音的等级，0为最低，1为低，2为中等，3为高 默认值为3 设置场景:在房间中设置或者在音频设备启动的时候设置 |
 *  | AECLevel               | 设置回声消除的等级，AEC可以消除各种延迟的回声,0为最低，1为低，2为中等，3为高  默认值为:3  设置场景:在房间中设置或者在音频设备启动的时候设置 |
 *  | ANSFlag                | 强制打开或者关闭背景噪音抑制，AEC可以消除各种延迟的回声,-1为取消客户端设置并听从流控，0为强制关闭，1为强制打开 默认值为:-1  设置场景:在房间中音频设备启动后设置 |
 *  | AGCFlag                | 强制打开或者关闭音频的自动增益补偿功能，打开后不会有声音忽大忽小的问题。-1为取消客户端设置并听从流控，0为强制关闭，1为强制打开 默认值为:-1  设置场景:在房间中音频设备启动后设置 |
 *  | AECFlag                | 强制打开或者关闭回声消除，AEC可以消除各种延迟的回声。-1为取消客户端设置并听从流控，0为强制关闭，1为强制打开 默认值为:-1  设置场景:在房间中音频设备启动后设置 |
 *  | AudioInterruptProcessOwner | 设置音频打断处理负责者(业务层或者SDK),如果业务层处理打断(电话，闹钟,QQ电话等),设置参数为Client,如果是SDK层处理打断，设置参数为SDK，如果不设置，默认SDK处理打断|
 *  | AudioInterrupt             | (只有在AudioInterruptProcessOwner为Client的时候，此参数有效) 通知SDK音频打断处理 ,Begin为打断开始，End为打断结束|
 
 @param object 参数对应的取值
 @param key   音视频引擎的参数
 @return 成功返回QAV_OK, 其他情况请参照QAVResult。
 */
- (QAVResult)setParameter:(NSString *)object forKey:(NSString *)key;

/*!
 @abstract      获取音视频引擎等房间参数
 @discussion    获取音视频引擎等房间参数,获取时机也是要与setParameter:forKey:的时机对应
  *  | 参数                   描述                                          |
  *  | RoomSeverInfo  | 获取房间连接的IP列表，IP数为8个，第一个为当前连接的接口机IP 获取场景:进房后|
  *  | AECFlag        | 当前AEC开关状态，0为关闭，1为打开  获取场景:在房间中音频设备启动后获取 |
  *  | AGCFlag        | 当前AGC开关状态，0为关闭，1为打开  获取场景:在房间中音频设备启动后获取 |
  *  | ANSFlag        | 当前ANS开关状态，0为关闭，1为打开  获取场景:在房间中音频设备启动后获取 |
 @return        返回音视频引擎的参数对应的数值，失败的话会返回""
 */
- (NSString *)parameterForKey:(NSString *)key;

@end
