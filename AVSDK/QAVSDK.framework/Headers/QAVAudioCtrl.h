#import "QAVSDK/QAVCommon.h"
#import "QAVSDK/QAVError.h"
#import <Foundation/Foundation.h>

@protocol QAVAudioDataDelegate;

/*!
 @abstract      音频操作回调
 @param         result          错误码。
 @see           QAVResult
 */
typedef void (^QAVAudioOptionComplete)(int result);
/*!
 @discussion    音频控制器的封装类
 */
@interface QAVAudioCtrl : NSObject

///---------------------------------------------------------------------------------------
/// @name       Property
///---------------------------------------------------------------------------------------

/*!
 @abstract      麦克风数字音量,动态音量取值范围[0,100]
 */
@property (readonly, nonatomic) UInt32 volume;

/*!
 @abstract      麦克风动态音量,动态音量取值范围[0,100]
 */
@property (readonly, nonatomic) UInt32 dynamicVolume;

/*!
 @abstract      外放模式
 */
@property (assign, nonatomic) QAVOutputMode outputMode;
/*!
 @abstract      音频数据委托
 */
@property(assign, nonatomic)id<QAVAudioDataDelegate> delegate;

///---------------------------------------------------------------------------------------
/// @name       Mic && Speaker
///---------------------------------------------------------------------------------------

/*!
 @abstract      打开/关闭音频预览。打开音频预览后，可以独立打开麦克风、扬声器、监听等功能。
 @param         isEnable        YES 打开，NO 关闭。
 @param         category        设置开预览时的音频场景。
 @return        YES 成功，NO 失败。
 */
- (BOOL)enablePreview:(BOOL)isEnable withCategory:(avAudioCategory)category;

/*!
 @abstract      打开/关闭扬声器。已废弃，建议使用带回调的版本
 @param         isEnable        是否打开。
 @return        YES表示操作成功，NO表示操作失败。
 */
- (BOOL)enableSpeaker:(BOOL)isEnable __attribute__((deprecated("已废弃，建议使用带回调的版本")));

/*!
 @abstract      打开/关闭麦克风。已废弃，建议使用带回调的版本
 @param         isEnable        是否打开。
 @return        YES表示操作成功，NO表示操作失败。
 */
- (BOOL)enableMic:(BOOL)isEnable __attribute__((deprecated("已废弃，建议使用带回调的版本")));

/*!
 @abstract      打开/关闭扬声器。
 @param         isEnable        是否打开。
 @return        YES表示操作成功，NO表示操作失败。
 */
- (BOOL)enableSpeaker:(BOOL)isEnable complete:(QAVAudioOptionComplete)block;

/*!
 @abstract      打开/关闭麦克风。
 @param         isEnable        是否打开。
 @return        YES表示操作成功，NO表示操作失败。
 */
- (BOOL)enableMic:(BOOL)isEnable complete:(QAVAudioOptionComplete)block;

/*!
 @abstract      获取麦克风状态。
 @return        具体参照QAVAudioState
 
                AV_AUDIO_CLOSE                 = 0,
                AV_AUDIO_OPEN                  = 1,
                AV_AUDIO_OPREATIING            = 2,
                AV_AUDIO_DEVICE_NOT_EXIST      = 3,
 
 @see           QAVAudioState
 */
- (QAVAudioState)getMicState;

/*!
 @abstract      获取扬声器状态。
 @return        具体参照QAVAudioState
 
                AV_AUDIO_CLOSE                 = 0,
                AV_AUDIO_OPEN                  = 1,
                AV_AUDIO_OPREATIING            = 2,
                AV_AUDIO_DEVICE_NOT_EXIST      = 3,
 @see           QAVAudioState
 */
- (QAVAudioState)getSpeakerState;

/*!
 @abstract      打开/关闭自监听。打开之后可以听自己的声音
 @param         isEnable        是否打开。
 @return        YES表示操作成功，NO表示操作失败。
 */
- (BOOL)enableLoopBack:(BOOL)isEnable;


///---------------------------------------------------------------------------------------
/// @name       Other Methods
///---------------------------------------------------------------------------------------

/*!
 @abstract      停止音频引擎
 @warning       app需要播放音乐时要先停掉我们的sdk。播放完了之后再调用resumeAudio重启音频引擎。
 */
- (void)pauseAudio;

/*!
 @abstract      停止音频引擎
 */
- (void)resumeAudio;

/*!
 @abstract      修改音频场景
 @param         category        0代表实时场景，1代表主播模式，
                                2代表观看模式，3代表高音质模式，其他模式待扩展。
 @see           QAVResult
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 */
- (QAVResult)changeAudioCategory:(NSInteger)category;

/*!
 @abstract      获取通话中实时音频质量相关信息，业务侧可以不用关心，主要用
                来查看通话情况、排查问题等。
 @return        以字符串形式返回音频相关的质量参数。
 */
- (NSString *)getQualityTips;

/*!
 @abstract      开启/关闭外部音频数据采集
 @param         isEnable        是否打开。
 */
- (void)enableExternalAudioDataInput:(BOOL) isEnable;

/*!
 @abstract      填充外部采集的音频数据(只有在音频场景为播放场景的时候接口有效)
                适用于一些app用自己的设备采集到数据之后再塞到sdk内部或者app自己接管系统采集通过接口塞到sdk
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 */
- (QAVResult)fillExternalAudioFrame: (QAVAudioFrame *)audioFrame;

/*!
 @abstract      获取音频场景
 @return        0代表实时场景，1代表主播模式，
                2代表观看模式，3代表高音质模式，其他模式待扩展。
                -1代表获取失败
 */
- (QAVAudioCategory)getAudioCategory;
///---------------------------------------------------------------------------------------
/// @name       AudioDataCallBack
///---------------------------------------------------------------------------------------

/*!
 @abstract      设置音频回调的delegate
 @discussion    通过回调函数，来通知外部读取或者写入具体类型的音频数据。
 @param         dlg             继承了QAudioDataDelegate的对象实例
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @warning       业务测调用该接口的类，需要在这个类的dealloc函数将setAudioDataEventDelegate:nil,
                否则容易引起回调因为业务测的对象析构而导致回调crash
 @see           QAVResult
 */
- (QAVResult)setAudioDataEventDelegate:(id<QAVAudioDataDelegate>)dlg;

/*!
 @abstract      注册音频数据类型的回调
 @discussion    要注册监听的音频数据源类型，具体参考QAVAudioDataSourceType。
 @param         type            要注册监听的音频数据源类型，具体参考QAVAudioDataSourceType
 @return        成功返回QAV_OK, 其他情况请参照QAVResult。
 @see           QAVAudioDataSourceType QAVResult
 */
- (QAVResult)registerAudioDataCallback:(QAVAudioDataSourceType)type;

/*!
 @abstract      反注册音频数据类型的回调
 @discussion    要注册监听的音频数据源类型，具体参考QAVAudioDataSourceType。
 @param         type            要反注册监听的音频数据源类型，具体参考QAVAudioDataSourceType
 @return        成功返回QAV_OK, 其他情况请参照QAVResult。
 @see           QAVAudioDataSourceType QAVResult
 */
- (QAVResult)unregisterAudioDataCallback:(QAVAudioDataSourceType)type;

/*!
 @abstract      反注册所有数据的回调
 @return        成功返回QAV_OK, 其他情况请参照QAVResult。
 @see           QAVResult
 */

- (QAVResult)unregisterAudioDataCallbackAll;

///---------------------------------------------------------------------------------------
/// @name       AudioDataFormat && volume
///---------------------------------------------------------------------------------------

/*!
 @abstract      设置某类型的音频格式参数。
 @discussion    要注册监听的音频数据源类型，具体参考QAVAudioDataSourceType。
 @param         srcType         音频数据类型。
 @param         audioDesc       音频数据的格式。
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVAudioDataSourceType QAVResult
 */
- (QAVResult)setAudioDataFormat:(QAVAudioDataSourceType)srcType
                           desc:(struct QAVAudioFrameDesc)audioDesc;

/*!
 @abstract      获取某类型的音频格式参数。
 @param         srcType         音频数据类型。
 @return        返回struct QAVAudioFrameDesc。
 */
- (struct QAVAudioFrameDesc)getAudioDataFormat:(QAVAudioDataSourceType)srcType;

/*!
 @abstract      设置某类型的音频音量。
 @param         srcType         音频数据类型。
 @param         volume          音量 (范围 0-1)。
 @see           QAVAudioDataSourceType QAVResult
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @warning       没有注册对应类型的callback会直接返回QAV_ERR_FAIL。
 */
- (QAVResult)setAudioDataVolume:(QAVAudioDataSourceType)srcType
                         volume:(float)volume;

/*!
 @abstract      获取某类型的音频音量。
 @param         srcType         音频数据类型。
 @see           QAVAudioDataSourceType
 @return        返回音量 (范围 0-1)
 @warning       没有注册对应类型的callback会直接返回QAV_ERR_FAIL。
 */
- (float)getAudioDataVolume:(QAVAudioDataSourceType)srcType;

/*!
 @abstract      设置某类型的音频音量。
 @param         srcType         音频数据类型。
 @param         volume          音量 (范围 0-121)。
                                0代表静音，－30db－30db，每增加1增加0.5db
 @see           QAVAudioDataSourceType
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @warning       没有注册对应类型的callback会直接返回QAV_ERR_FAIL。
 */
- (QAVResult)SetAudioDataDBVolume:(QAVAudioDataSourceType)srcType
                           volume:(int)volume;

/*!
 @abstract      获取某类型的音频音量。
 @param         srcType         音频数据类型。
 @see           QAVAudioDataSourceType
 @return        音量 (范围 0-121)。0代表静音，－30db－30db，每增加1增加0.5db
 */
- (int)getAudioDataDBVolume:(QAVAudioDataSourceType)srcType;

/*!
 @abstract      设置变声类型。
 @param         type         变声类型。
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 */
- (QAVResult)setVoiceType:(QAVVoiceType)type;


/*!
 @abstract      获取房间内其他成员的实时音量。
 @param         identifier         成员帐号。
 @return        音频设备音量，正常取值范围0－32676,发生错误时返回-1
 */
-(int)getDynamicVolumeById: (NSString *)identifier;

/*!
 @abstract      设置送到扬声器的音频流的音量接口
 @param         identifierArray    代表需要改变音量的音频流。
 @param         volumeArray        音频流i的音量值的目标值，值域[0, 1.0], 0表示静音，1表示原音量,小于1时表示缩小音。
 @param         timeInArray        音频流i的音量值渐入值，值域[0, 10000], 单位为ms，淡入效果。
 @param         timeOutArray       音频流i的音量值的渐出值，值域[0, 10000], 单位为ms，淡出效果。
 @param         block              set结果回调，表示是否成功。
 @return        成功返回QAV_OK, 其他参考错误码定义。
 */
- (int)setSpeakerAudioVolume:(NSMutableArray*) identifierArray
                     volume :(NSMutableArray*) volumeArray
                     timeIn :(NSMutableArray*) timeInArray
                    timeOut :(NSMutableArray*) timeOutArray
                    complete:(QAVAudioOptionComplete)block;

@end

/*!
 @discussion    音频数据委托协议。
 */
@protocol QAVAudioDataDelegate <NSObject>
/*!
 @abstract      音频数据透传回调。
 @discussion    通过回调函数，来通知外部读取或者写入具体类型的音频数据。
 @param         audioFrame      音频数据，输出类型从此参数读取数据，
                                输入数据类型将数据写入此参数。
 @param         type            音频数据类型。
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVResult
 @warning       回调函数设定为专门处理数据用。函数回调在非主线程，请确保
                线程安全。特别是不要在回调函数中直接调用SDK接口。
 */
- (QAVResult)audioDataComes:(QAVAudioFrame *)audioFrame
                       type:(QAVAudioDataSourceType)type;
/*!
 @abstract      传给sdk的音频数据回调。
 @discussion    通过回调函数，来通知外部读取或者写入具体类型的音频数据。
 @param         audioFrame      音频数据，输出类型从此参数读取数据，
                                输入数据类型将数据写入此参数。
 @param         type            音频数据类型。
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVResult
 @warning       回调函数设定为专门处理数据用。函数回调在非主线程，请确保
                线程安全。特别是不要在回调函数中直接调用SDK接口。
 */
- (QAVResult)audioDataShouInput:(QAVAudioFrame *)audioFrame
                           type:(QAVAudioDataSourceType)type;
/*!
 @abstract      音频数据预处理回调
 @discussion    通过回调函数，来通知外部读取或者写入具体类型的音频数据。
 @param         audioFrame      音频数据，输出类型从此参数读取数据，
                                输入数据类型将数据写入此参数。
 @param         type            音频数据类型。
 @return        成功返回QAV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVResult
 @warning       回调函数设定为专门处理数据用。函数回调在非主线程，请确保
                线程安全。特别是不要在回调函数中直接调用SDK接口。
 */
- (QAVResult)audioDataDispose:(QAVAudioFrame *)audioFrame
                         type:(QAVAudioDataSourceType)type;




@end

