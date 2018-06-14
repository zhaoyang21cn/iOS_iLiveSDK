#pragma once

#import "QAVError.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/*!
 @abstract      通用的回调
 @param         result          返回的错误码
 @see           QAVResult
 @param         errorInfo       错误信息
 */
typedef void (^QAVCompletionHandler)(int result, NSString *errorInfo);

/*!
 @discussion    视频流的色彩格式,支持I420,NV21,NV12,RGB565,RGB24,ARGB,RGBA
 */
typedef NS_ENUM(NSInteger, avColorFormats) {
    AVCOLOR_FORMAT_I420   = 0,
    AVCOLOR_FORMAT_NV21   = 1,
    AVCOLOR_FORMAT_NV12   = 3,
    AVCOLOR_FORMAT_RGB565 = 7,
    AVCOLOR_FORMAT_RGB24  = 8,
    AVCOLOR_FORMAT_ARGB   = 9,
    AVCOLOR_FORMAT_RGBA   = 10,
    AVCOLOR_FORMAT_BGRA   = 11,
};

/*!
 @discussion    直播类型
 */
typedef NS_ENUM(NSInteger, QAVVideoMode) {
    QAVVideoModeNormal   = 0,   ///< 默认值，普通直播
    QAVVideoModePanorama = 1,   ///< 摄像头，全景直播
};

/*!
 @discussion    视频源类型
 */
typedef NS_ENUM(NSInteger, avVideoSrcType) {
    QAVVIDEO_SRC_TYPE_NONE   = 0,   ///< 默认值，无意义
    QAVVIDEO_SRC_TYPE_CAMERA = 1,   ///< 摄像头
    QAVVIDEO_SRC_TYPE_SCREEN = 2,   ///< 屏幕分享
    QAVVIDEO_SRC_TYPE_MEDIA = 3,   ///< 播片
};
/*!
 @discussion    视频帧描述信息
 */
@interface QAVFrameDesc : NSObject {
}

/*!
 @abstract      宽度(像素)
 */
@property (assign, nonatomic) UInt32 width;
/*!
 @abstract      高度(像素)
 */
@property (assign, nonatomic) UInt32 height;

/**
 @abstract      画面旋转的角度
 @discussion    1.source_type为AVVIDEO_SRC_TYPE_CAMERA时，表示视频源为摄像头。<br/>
                - 在终端上，摄像头画面是支持旋转的，App需要根据旋转角度调整渲染层的处理，以保证画面的正常显示。<br/>
                - 0: 无旋转角度 1: 顺时针90度 2：顺时针180度 3:逆时针270度<br/>
                2.source_type为其他值时，rotate恒为0。<br/>
 */
@property (assign, nonatomic) int rotate;
/*!
 @abstract      色彩格式
 */
@property (assign, nonatomic) avColorFormats color_format;
/*!
 @abstract      视频源类型
 */
@property (assign, nonatomic) avVideoSrcType srcType;
@end

/*!
 @discussion    视频帧
 */
@interface QAVVideoFrame : NSObject {
}
/*!
 @abstract      视频帧描述信息
 */
@property (retain, nonatomic) QAVFrameDesc *frameDesc;
/*!
 @abstract      视频帧所属的房间成员ID
 */
@property (copy, nonatomic) NSString *identifier;
/*!
 @abstract      视频帧的数据缓冲区大小(字节)
 */
@property (assign, nonatomic) UInt32 dataSize;
/*!
 @abstract      视频帧的数据缓冲区大小，单位：字节。
 */
@property(assign, nonatomic) UInt32 stride;
/*!
 @abstract      视频帧的数据缓冲区，SDK内部会管理缓冲区的分配和释放
 */
@property (assign, nonatomic) UInt8 *data;
/*!
 @abstract      视频帧所属的房间成员ID
 */
@property (assign, nonatomic) UInt64 relationId;
/*!
 @abstract      视频帧的时间戳，SDK内部会自动填写好，utc时间，0为无效值。
 */
@property (assign, nonatomic) UInt64 timeStamp;

@end
/*!
 @discussion    摄像头信息
 */
@interface QAVCameraInfo : NSObject {
}
/*!
 @abstract      设备ID
 */
@property (copy, nonatomic) NSString *deviceID;
/*!
 @abstract      采集画面宽度
 */
@property (assign, nonatomic) NSInteger width;
/*!
 @abstract      采集画面高度
 */
@property (assign, nonatomic) NSInteger height;
/*!
 @abstract      采集帧率
 */
@property (assign, nonatomic) NSInteger fps;

@end
/*!
 @discussion    视频接收模式
 */
typedef NS_ENUM(NSInteger, VideoRecvMode) {
    
    VIDEO_RECV_MODE_MANUAL                      = 0, ///< 手动模式。也就是需要业务侧调用请求画面"RequestView"接口来请求接收视频
    VIDEO_RECV_MODE_SEMI_AUTO_RECV_CAMERA_VIDEO = 1, ///< 半自动模式，针对摄像头视频。也就是当进入房间时，如果已经有人发送了视频，
                                                     ///< 则会自动接收这些视频，不用手动去请求。当然，进入房间后，如何其他人再发送的视频，则不会自动接收。
};
/*!
 @discussion    屏幕分享接收模式
 */
typedef NS_ENUM(NSInteger, ScreenRecvMode) {
    
    SCREEN_RECV_MODE_MANUAL                      = 0, ///< 手动模式。也就是需要业务侧调用请求画面"RequestView"接口来请求接收屏幕分享视频
    SCREEN_RECV_MODE_SEMI_AUTO_RECV_SCREEN_VIDEO = 1, ///< 半自动模式，针对屏幕分享视频。也就是当进入房间时，如果已经有人开启了屏幕分享，
                                                      ///< 则会自动接收该屏幕分享视频，不用手动去请求。进房后再开启的屏幕分享，则不会自动接收。
};


/*!
 @discussion    音频数据透传类型
 */
typedef NS_ENUM(NSInteger, QAVAudioDataSourceType) {
    QAVAudioDataSource_Mic          = 0,    ///< 麦克风输出
    QAVAudioDataSource_MixToSend    = 1,    ///< 发送混音输入
    QAVAudioDataSource_Send         = 2,    ///< 发送输出
    QAVAudioDataSource_MixToPlay    = 3,    ///< 扬声器混音输入
    QAVAudioDataSource_Play         = 4,    ///< 扬声器输出
    QAVAudioDataSource_NetStream    = 5,    ///< 扬声器远端用户声音输出
    QAVAudioDataSource_VoiceDispose = 6,    ///< 麦克风预处理
    QAVAudioDataSource_SyncMixToSend= 7,    ///< 发送混音输入，实时性更高
    QAVAudioDataSource_AACRAWSTREAM = 8,    ///< 原始AAC码流
    QAVAudioDataSource_End          = 9,
};

/*!
 @discussion    音频帧描述
 */
struct QAVAudioFrameDesc {
    NSInteger SampleRate;   ///< 采样率，单位：赫兹（Hz）。
    NSInteger ChannelNum;   ///< 通道数，1表示单声道（mono），2表示立体声
    NSInteger Bits;         ///< 音频的位宽，目前只能填16
};


/*!
 @discussion    音频帧
 */
@interface QAVAudioFrame : NSObject

/*!
 @abstract      音频帧所属的房间成员ID
 */
@property (copy, nonatomic) NSString *identifier;

/*!
 @abstract      音频帧描述
 */
@property (assign, nonatomic) struct QAVAudioFrameDesc desc;

/*!
 @abstract      音频帧的数据缓冲区大小
 */
@property (retain, nonatomic) NSData *buffer;
/*!
 @abstract      音频帧的时间戳，SDK内部会自动填写好，utc时间，0为无效值。
 */
@property (assign, nonatomic) UInt64 timeStamp;

@end

/*!
 @discussion    音视频事件主类型
 */
typedef NS_ENUM(NSInteger, avRoomMainEventType) {
    QAV_ROOM_MAIN_EVENT_TYPE_NONE             = 0,   ///< 初始值，没有意义
    QAV_ROOM_MAIN_EVENT_TYPE_NETWORK          = 1,   ///< 网络事件
    QAV_ROOM_MAIN_EVENT_TYPE_TIME_CONSUMEINFO = 100, ///< 耗时细分
};

/*!
 @discussion    音视频网络事件子类型
 */
typedef NS_ENUM(NSInteger, avSubEventNetworkType) {
    QAV_ROOM_AV_SUB_NETWORK_EVENT_TYPE_NONE    = 1,  ///< 初始值，没有意义
    QAV_ROOM_AV_SUB_NETWORK_EVENT_TYPE_TIMEOUT = 2,  ///< 网络事件
};

/*!
 @discussion    音视频耗时事件子类型
 */
typedef NS_ENUM(NSInteger, avSubEventTimeConsumeType) {
    QAV_ROOM_AV_SUB_TIMECONSUME_EVENT_TYPE_NONE      = 1, ///< 初始值，没有意义
    QAV_ROOM_AV_SUB_TIMECONSUME_EVENT_TYPE_ENTERROOM = 2, ///< 进房时间耗时细分
};

/*!
 @discussion    音视频通话模式
 */
typedef NS_ENUM(NSInteger, avRoomMode) {
    MODE_AUDIO = 0,     ///< 纯语音通话，双方都不能进行视频上下行。
    MODE_VIDEO = 1,     ///< 音视频通话，对视频上下行没有约束。
};

/*!
 @discussion    音视频场景策略
 */
typedef NS_ENUM(NSInteger, avAudioCategory) {
    CATEGORY_VOICECHAT             = 0, ///< VoIP模式，适合于实时音频通信场景，如实时语音通话
    CATEGORY_MEDIA_PLAY_AND_RECORD = 1, ///< 媒体采集与播放模式，适合于那些对音质要求比较高的直播场景，如主播场景中的主播人员
    CATEGORY_MEDIA_PLAYBACK        = 2, ///< 媒体播放模式，适合于那些对音质要求比较高的直播场景，如主播场景中的听众
};
/*!
 @discussion    音频设备状态
 */
typedef NS_ENUM(NSInteger, QAVAudioState) {
    QAV_AUDIO_CLOSE             = 0,    ///< 关闭状态
    QAV_AUDIO_OPEN              = 1,    ///< 打开状态
    QAV_AUDIO_OPERATING         = 2,    ///< 正在运行
    QAV_AUDIO_DEVICE_NOT_EXIST  = 3,    ///< 该设备不存在
};
/*!
 @discussion    音频播放模式选择
 */
typedef NS_ENUM(NSInteger, QAVAudioOutputMode) {
    QAV_AUDIO_OUTPUT_MODE_HEADSET         = 0,    ///< 听筒
    QAV_AUDIO_OUTPUT_MODE_SPEAKER         = 1,    ///< 扬声器
    
};
/*!
 @discussion    音频场景
 */
typedef NS_ENUM(NSInteger, QAVAudioCategory) {
    QAV_AUDIO_CATEGORY_ERROR          = -1,   ///< 获取失败
    QAV_AUDIO_CATEGORY_REALTIME       = 0,    ///< 实时场景
    QAV_AUDIO_CATEGORY_ANCHOR         = 1,    ///< 主播场景
    QAV_AUDIO_CATEGORY_VIEWER         = 2,    ///< 观众场景
    QAV_AUDIO_CATEGORY_HQ             = 3,    ///< 高音质
};
/*!
 @discussion    输出类型
 */
typedef NS_ENUM(NSInteger, QAVOutputMode) {
    QAVOUTPUTMODE_EARPHONE = 0, ///< 输出类型为耳机模式。
    QAVOUTPUTMODE_SPEAKER,      ///< 输出类型为扬声器模式。
};

/*!
 @discussion    自定义数据类型
 */
typedef NS_ENUM(NSInteger, QAVCustomDataMode) {
    QAVCUSTOMDATAMODE_NOT_AUTOPUSH = 0, ///<  (预留类型，业务侧不要使用！！！)不自动推动，需接收端主动请求，独立信令
    QAVCUSTOMDATAMODE_AUTOPUSH = 1,     ///<  (预留类型，业务侧不要使用！！！)自动推送，独立信令
    QAVCUSTOMDATAMODE_WITH_AVDATA = 2,  ///<  跟随音视频数据发送
};

/*!
 @discussion    音视频事件更新
 */
typedef NS_ENUM(NSInteger, QAVUpdateEvent) {
    QAV_EVENT_ID_NONE                      = 0, ///< 默认值，无意义。
    QAV_EVENT_ID_ENDPOINT_ENTER            = 1, ///< 进入房间事件。
    QAV_EVENT_ID_ENDPOINT_EXIT             = 2, ///< 退出房间事件。
    QAV_EVENT_ID_ENDPOINT_HAS_CAMERA_VIDEO = 3, ///< 有发摄像头视频事件。
    QAV_EVENT_ID_ENDPOINT_NO_CAMERA_VIDEO  = 4, ///< 无发摄像头视频事件。
    QAV_EVENT_ID_ENDPOINT_HAS_AUDIO        = 5, ///< 有发语音事件。
    QAV_EVENT_ID_ENDPOINT_NO_AUDIO         = 6, ///< 无发语音事件。
    QAV_EVENT_ID_ENDPOINT_HAS_SCREEN_VIDEO = 7, ///< 有发屏幕视频事件。
    QAV_EVENT_ID_ENDPOINT_NO_SCREEN_VIDEO  = 8, ///< 无发屏幕视频事件。
    QAV_EVENT_ID_ENDPOINT_HAS_MEDIA_FILE_VIDEO = 9, ///< 有发文件视频事件。
    QAV_EVENT_ID_ENDPOINT_NO_MEDIA_FILE_VIDEO  = 10, ///< 无发文件视频事件。
};
typedef NS_ENUM(NSInteger, QAVVoiceType) {
    QAV_VOICE_TYPE_ORIGINAL_SOUND = 0, ///<  原声
    
    QAV_VOICE_TYPE_LOLITA = 1, ///< 萝莉
    
    QAV_VOICE_TYPE_UNCLE = 2, ///<大叔
    
    QAV_VOICE_TYPE_INTANGIBLE = 3, ///< 空灵
    
    QAV_VOICE_TYPE_KINDER_GARTEN = 4, ///< 幼稚园
    
    QAV_VOICE_TYPE_HEAVY_GARTEN = 5, ///< 重机器
    
    QAV_VOICE_TYPE_OPTIMUS_PRIME = 6, ///< 擎天柱
    
    QAV_VOICE_TYPE_CAGED_ANIMAL = 7, ///< 困兽
    
    QAV_VOICE_TYPE_DIALECT = 8, ///< 土掉渣/歪果仁/方言
    
    QAV_VOICE_TYPE_METAL_ROBOT = 9, ///< 金属机器人
    
    QAV_VOICE_TYPE_DEAD_FATBOY = 10, ///< 死肥仔
};


/*!
 @discussion    多人房间参数
 */
@interface QAVMultiParam : NSObject {
}
/*!
 @abstract      音视频场景策略，多人房间专用
 */
@property (assign, nonatomic) avAudioCategory audioCategory;
/*!
 @abstract      业务侧创建的房间Id
 */
@property (assign, nonatomic) UInt32 relationId;
/*!
 @abstract      音视频权限位
 */
@property (assign, nonatomic) UInt64 authBits;
/*!
 @abstract      音视频权限加密串
 */
@property (copy, nonatomic) NSData *authBuffer;
/*!
 @abstract      角色名，web端音视频参数配置工具所设置的角色名
 */
@property (copy, nonatomic) NSString *controlRole;
/*!
 @abstract      是否自动创建音视频房间。如果房间不存在时，是否自动创建它
 */
@property (assign, nonatomic) BOOL createRoom;
/*!
 @abstract      视频接收模式
 */
@property (assign, nonatomic) VideoRecvMode videoRecvMode;
/*!
 @abstract      屏幕分享接收模式
 */
@property (assign, nonatomic) ScreenRecvMode screenRecvMode;
/*!
 @abstract      是否开启麦克风
 */
@property (assign, nonatomic) BOOL enableMic;
/*!
 @abstract      是否开启扬声器
 */
@property (assign, nonatomic) BOOL enableSpeaker;
/*!
 @abstract      音频播放模式
 */
@property (assign, nonatomic) QAVAudioOutputMode audioOutputMode;
/*!
 @abstract      高音质开关，YES为开，NO为关，默认关。
 */
@property (assign, nonatomic) BOOL enableHdAudio;
/*!
 @abstract      是否开启硬件编码。
 */
@property(assign, nonatomic) BOOL enableHwEnc;
/*!
 @abstract      是否开启硬件解码。
 */
@property(assign, nonatomic) BOOL enableHwDec;

/*!
 @abstract      拉取接口机iplist增加灰度控制。
 */
@property (assign, nonatomic) BOOL enableMultiRequestIplist;

/*!
 @abstract      退房后自动关闭摄像头，YES为退房自动关闭，NO为退房不关闭摄像头，默认为YES。
 */
@property (assign, nonatomic) BOOL autoCloseCamera;
@end

/*!
 @discussion    多人房间参数
 */
@interface QAVChangeRoomInfo : NSObject {
}
/*!
 @abstract      业务侧创建的房间Id
 */
@property (assign, nonatomic) UInt32 relationId;
/*!
 @abstract      音视频权限位
 */
@property (assign, nonatomic) UInt64 authBits;
/*!
 @abstract      音视频权限加密串
 */
@property (copy, nonatomic) NSData *authBuffer;
/*!
 */
@end

/*!
 @discussion    自定义数据描述
 */
@interface QAVCustomData : NSObject

@property(copy,nonatomic) NSData *data;

@property(assign,nonatomic) QAVCustomDataMode pushMode;

@end



/*!
 @discussion    切换角色参数
 */
@interface QAVChangeRoleInfo : NSObject {
}
/*!
 @abstract      业务侧创建的房间Id
 */
@property (assign, nonatomic) NSString *role;
/*!
 @abstract      音视频权限位
 */
@property (assign, nonatomic) UInt64 authBits;
/*!
 @abstract      音视频权限加密串
 */
@property (copy, nonatomic) NSData *authBuffer;
/*!
 */
@end

/*!
 @discussion    音视频权限位
 */
#define QAV_AUTH_BITS_DEFAULT 0xFFFFFFFFFFFFFFFF ///< 缺省值。拥有所有权限。
#define QAV_AUTH_BITS_OPEN 0x000000FF            ///< 权限全开
#define QAV_AUTH_BITS_CLOSE 0x00000000           ///< 权限全关
#define QAV_AUTH_BITS_CREATE_ROOM 0x00000001     ///< 创建房间权限。
#define QAV_AUTH_BITS_JOIN_ROOM 0x00000002       ///< 加入房间的权限。
#define QAV_AUTH_BITS_SEND_AUDIO 0x00000004      ///< 发送语音的权限。
#define QAV_AUTH_BITS_RECV_AUDIO 0x00000008      ///< 接收语音的权限。
#define QAV_AUTH_BITS_SEND_VIDEO 0x00000010      ///< 发送视频的权限。
#define QAV_AUTH_BITS_RECV_VIDEO 0x00000020      ///< 接收视频的权限。
#define QAV_AUTH_BITS_SEND_SUB 0x00000040        ///< 发送辅路视频的权限。
#define QAV_AUTH_BITS_RECV_SUB 0x00000080        ///< 接收辅路视频的权限。

//查询房间状态字段所用的key值

UIKIT_EXTERN NSString *const ROOMSTATPARATICKCOUNTBEGIN;           ///< 统计开始时间
UIKIT_EXTERN NSString *const ROOMSTATPARATICKCOUNTEND;             ///< 统计结束时间
UIKIT_EXTERN NSString *const ROOMSTATPARABASICINFO;                ///< 基本信息
UIKIT_EXTERN NSString *const ROOMSTATPARANETWORK;                  ///< 网络信息
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOPARAM;               ///< 视频信息
UIKIT_EXTERN NSString *const ROOMSTATPARAAPPCPURATE;               ///< app的cpu占用率
UIKIT_EXTERN NSString *const ROOMSTATPARASYSCPURATE;               ///< 系统的cpu占用率
UIKIT_EXTERN NSString *const ROOMSTATPARASDKVERSION;               ///< sdk版本号
UIKIT_EXTERN NSString *const ROOMSTATPARAINTERFACEIP;              ///< 接口机IP
UIKIT_EXTERN NSString *const ROOMSTATPARAINTERFACEPORT;            ///< 接口机端口
UIKIT_EXTERN NSString *const ROOMSTATPARACLIENTIP;                 ///< 客户端ip
UIKIT_EXTERN NSString *const ROOMSTATPARAKBPSRECV;                 ///< 收包速率,单位kbps
UIKIT_EXTERN NSString *const ROOMSTATPARAPACKETRECV;               ///< 收包速率,每秒收包的个数
UIKIT_EXTERN NSString *const ROOMSTATPARAKBPSSEND;                 ///< 发包速率,单位kbps
UIKIT_EXTERN NSString *const ROOMSTATPARAPACKETSEND;               ///< 发包速率,每秒发包的个数
UIKIT_EXTERN NSString *const ROOMSTATPARARTT;                      ///< 往返时延
UIKIT_EXTERN NSString *const ROOMSTATPARALOSSMODELRECV;            ///< 下行平均连续丢包个数
UIKIT_EXTERN NSString *const ROOMSTATPARALOSSRATERECV;             ///< 下行丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARALOSSMODELSEND;            ///< 上行平均连续丢包个数，从Hello的回包中获
UIKIT_EXTERN NSString *const ROOMSTATPARALOSSRATESEND;             ///< 上行丢包率，从Hello的回包中获得
UIKIT_EXTERN NSString *const ROOMSTATPARALOSSRATESENDUDT;          ///< 上行UDT丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARALOSSRATERECVUDT;          ///< 下行UDT丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARACAPTURE;                  ///< 视频采集信息
UIKIT_EXTERN NSString *const ROOMSTATPARACAPTUREWIDTH;             ///< 视频采集宽度
UIKIT_EXTERN NSString *const ROOMSTATPARACAPTUREHEIGHT;            ///< 视频采集高度
UIKIT_EXTERN NSString *const ROOMSTATPARACAPTUREFPS;               ///< 视频采集帧率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODE;              ///< 视频编码参数
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEVIEWTYPE;      ///< 视频类型
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEWIDTH;         ///< 视频编码宽度
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEHEIGHT;        ///< 视频编码高度
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEFPS;           ///< 视频编码帧率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEBR;            ///< 视频编码码率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEAG;            ///< 视频编码角度
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODETYPE;          ///< 视频编码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODEHW;            ///< 是否开启硬编解码
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOENCODECOUNT;         ///< 视频编码路数
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDPARAM;           ///< 视频发送参数
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDLOSSRATE;        ///< 视频发送参数丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDIFEC;            ///< 视频发送参数I帧fec
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDSPFEC;           ///< 视频发送参数sp帧
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDPKT;             ///< 视频发送参数包数
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOISANCHOR;            ///< 是否为主播
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDSTNSBG;          ///< 人为补零 //人为丢包
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDSTNSBD;          ///< 解码端统计的丢包 //jitter中统计的EOS、大于一定数量的连续丢包
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEOSENDSTNSBB;          ///< 卡顿次数 //可能的小卡顿次数 //连续plc
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEPARAM;         ///< 视频解码参数
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEVIEWTYPE;      ///< 视频类型
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODETINYID;        ///< 视频对应用户
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEWIDTH;         ///< 视频编码宽度
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEHEIGHT;        ///< 视频编码高度
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEFPS;           ///< 视频编码帧率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEBR;            ///< 视频编码码率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODETYPE;          ///< 视频编码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODEHW;            ///< 是否开启硬编解码
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEODECODECOUNT;         ///< 视频解码路数
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEORECVLOSSRATE;        ///< 视频接收丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEORECVJITTER;          ///< 视频抖动
UIKIT_EXTERN NSString *const ROOMSTATPARAVIDEORECVBR;              ///< 视频接收码率
UIKIT_EXTERN NSString *const ROOMSTATPARASAMPLERATE;               ///< 音频流控下发参数采样率
UIKIT_EXTERN NSString *const ROOMSTATPARACODECTYPE;                ///< 音频流控下发参数通道数，1表示单声道（mono），2表示立体声（stereo）
UIKIT_EXTERN NSString *const ROOMSTATPARACHANNELCOUNT;             ///< 音频流控下发参数编码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSMAINBIG;               ///< 视频流控下发参数大画面宽
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSWIDTH;                 ///< 视频流控下发参数大画面宽
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSHEIGHT;                ///< 视频流控下发参数大画面高
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSFPS;                   ///< 视频流控下发参数大画面帧率
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSBITRATE;               ///< 视频流控下发参数大画面码率
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSENCODETYPE;            ///< 视频流控下发参数编码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSMINQP;                 ///< 视频流控下发最小质量
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSMAXQP;                 ///< 视频流控下发最大质量
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSFECTYPE;               ///< 视频流控下发参数fec类型
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSIFEC;                  ///< 视频流控下发i帧fec
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSPFEC;                 ///< 视频流控下发sp帧fec
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSPFEC;                  ///< 视频流控下发p帧fec
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSIMTU;                  ///< 视频流控下发参数I帧mtu
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSPMTU;                 ///< 视频流控下发参数sp帧mtu
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSPMTU;                  ///< 视频流控下发参数p帧mtu
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSIMINPKG;               ///< 视频流控下发参数i帧最小包
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSPMINPKG;              ///< 视频流控下发参数sp帧最小包
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSPMINPKG;               ///< 视频流控下发参数p帧最小包
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSIMINSIZE;              ///< 视频流控下发参数i帧最小包大小
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSPMINSIZE;             ///< 视频流控下发参数sp帧最小包大小
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSPMINSIZE;              ///< 视频流控下发参数p帧最小包大小
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSGOPTYPE;               ///< 视频流控下发参数gop类型
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSGOP;                   ///< 视频流控下发参数gop
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSHW;                    ///< 视频流控下发参数硬编码
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOPARAM;               ///< 音频流控参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOCAPTURE;             ///< 音频采集参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOCAPTURESAMPLERATE;   ///< 音频采集参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOCAPTURECHANNELCOUNT; ///< 音频采集参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOENCODEPARAM;         ///< 音频编码参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOENCODETYPE;          ///< 音频编码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOENCODEBR;            ///< 音频编码码率
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOENCODEVAD;           ///< 音频采集参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOSENDPARAM;           ///< 音频发送参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOSENDLOSSRATE;        ///< 音频发送对丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOSENDFEC;             ///< 音频发送的fec
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOSENDJETTER;          ///< 音频抖动
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOSENDBR;              ///< 音频发送码率
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOSENDBRUDT;           ///< 音频发送码率udt
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIODECODECOUNT;         ///< 音频解码路数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIODECODEPARAM;         ///< 音频解码参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIODECODETYPE;          ///< 音频解码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIODECODESAMPLETYPE;    ///< 音频解码采样率
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIODECODECHANNELCOUNT;  ///< 音频解码通道数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIODECODETINYID;        ///< 对应的tinyid
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIORECVPARAM;           ///< 音频接收参数
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIORECVPLAYDELAY;       ///< 音频播放时延
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIORECVLOSSRATE;        ///< 收包丢包率
UIKIT_EXTERN NSString *const ROOMSTATPARAAUDIOCATEGORY;            ///< 音频场景
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSMALLWIDTH;            ///< 视频流控下发参数小画面宽
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSMALLHEIGHT;           ///< 视频流控下发参数小画面高
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSMALLFPS;              ///< 视频流控下发参数小画面帧率
UIKIT_EXTERN NSString *const ROOMSTATPARAQOSSMALLBITRATE;          ///< 视频流控下发参数小画面码率
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSPARAM;           ///< 音频流控下发参数
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSSAMPLERATE;      ///< 音频流控下发码率
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSENCODETYPE;      ///< 音频流控下发编码类型
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSCHANNELCOUNT;    ///< 音频流控下发采集通道数
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSBITRATE;         ///< 音频流控下发码率
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSAEC;             ///< 音频流控下发aec
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSAGC;             ///< 音频流控下发agc
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSFEC;             ///< 音频流控下发fec
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSVAD;             ///< 音频流控下发vad
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSPACK;            ///< 音频流控下发pack
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSRECN;            ///< 音频流控下发recn
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSRECM;            ///< 音频流控下发recm
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSMTU;             ///< 音频流控下发mtu
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSJITTERMIN;       ///< 音频流控下发，Jitter的最小延时，单位毫秒
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSJITTERMINMAX;    ///< 音频流控下发，Jitter最大延时的最小阀值
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSJITTERMAXMAX;    ///< 音频流控下发，Jitter最大延时的最大阀值
UIKIT_EXTERN NSString *const ROOMSTATPARAEAUDIOQOSJITTERDROP;      ///< 音频流控下发，Jitter最大延时的损失比例

//解码画面，最多4路主路＋1路辅路
UIKIT_EXTERN NSString *const ROOMSTATPARAUINO;
UIKIT_EXTERN NSString *const ROOMSTATPARAUIN1;
UIKIT_EXTERN NSString *const ROOMSTATPARAUIN2;
UIKIT_EXTERN NSString *const ROOMSTATPARAUIN3;
UIKIT_EXTERN NSString *const ROOMSTATPARAUIN4;
