#import "QAVSDK/QAVCommon.h"
#import "QAVSDK/QAVError.h"
#import <Foundation/Foundation.h>


typedef NS_ENUM(NSInteger, QAVSpearEngineCtrlType) {
    QAVSpearEngineCtrlTypeCloud = 1,
    QAVSpearEngineCtrlTypeCustom = 2,
    QAVSpearEngineCtrlTypeSSO    = 3,
};

/*! 
 @discussion    web配置封装类
 @discussion    Spear流控参数Json格式，字段意义和范围。<br/>
                {<br/>
                "audio":{ 音频参数 (所有参数必填)<br/>
                "aec":0, AEC开关 (-1自适应, 0关，1开)<br/>
                "agc":0, AGC开关 (-1自适应, 0关，1开)<br/>
                "ans":0, ANC开关 (-1自适应, 0关，1开)<br/>
                "au_scheme":1, 音频场景 (2种场景: 1主播，2观众)<br/>
                "codec_prof":4106, 音频编码协议 (固定为4106 acc)<br/>
                "sample_rate":48000, 采样率 (固定为48000)<br/>
                "channel":2, 通道数 (2个档位: 1单通道，2双通道)<br/>
                "frame":40, 帧长 (-1自适应, 自定义2个档位: 40, 80)<br/>
                "kbps":24, 编码码率 (-1自适应, 最小值10, 最大值64)<br/>
                "silence_detect":0 静音检测 (0关，1开)<br/>
                "anti_dropout":0, 冗余抗丢包 (-1自适应, 0关，1开)<br/>
                },<br/>
                "video":{ 视频参数 (所有参数必填)<br/>
                "codec_prof":5, 视频编码协议 (-1自适应, 5[h264])<br/>
                "format_fix_height":368, 固定图像高 (和WEB流控配置可选分辨率对齐,其他分辨率不支持)<br/>
                "format_fix_width":640, 固定图像宽 (和WEB流控配置可选分辨率对齐,其他分辨率不支持)<br/>
                "format_max_height":-1, 最大图像高 (和WEB流控配置可选分辨率对齐,其他分辨率不支持)<br/>
                "format_max_width":-1, 最大图像宽 (和WEB流控配置可选分辨率对齐,其他分辨率不支持)<br/>
                "fps":25, 编码帧率 (-1自适应, 最小值10, 最大值30)<br/>
                "live_adapt":0, 自适应流控 (-1自适应, 0关，1开)<br/>
                "fqueue_time":5, 帧序列时间 (-1自适应, 最小值1, 最大值5)<br/>
                "maxkbps":800, 最大码率 (最小值30, 最大值1500)<br/>
                "minkbps":800, 最小码率 (最小值30, 最大值1500)<br/>
                "maxqp":31, 最大QP (-1自适应, 最小值10, 最大值51)<br/>
                "minqp":18, 最大QP (-1自适应, 最小值10, 最大值51)<br/>
                "qclear":1, qclear (-1自适应, 0关，1开)<br/>
                "anti_dropout":0, 冗余抗丢包 (-1自适应, 0关，1开)<br/>
                "small_video_upload":0 小画面上传 (目前不支持打开，固定填0)<br/>
                },<br/>
                "net":{ 网络参数 (所有参数必填)<br/>
                "rc_anti_dropout":1, 重传抗丢包 (-1自适应, 0关，1开)V
                "rc_init_delay":1000, 重传初始延迟 (-1自适应, 最小值0, 最大值10000)<br/>
                "rc_max_delay":2000 重传最大延迟 (-1自适应, 最小值0, 最大值10000)<br/>
                }<br/>
 
 */
@interface QAVCloudSpearEngineCtrl : NSObject {
}

/*!
 @abstract      设置默认流控参数。
 @discussion    仅当WEB配置拉取异常的情况下（从未从云流控配置系统成功拉取过配置）生效。
 @param         jsonParam       默认流控参数。
 @return        设置成功返回AV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVResult
 */
- (QAVResult)setDefaultParam:(NSString *)jsonParam;

@end

/**
 @discussion    音频控制器的封装类
 */
@interface QAVCustomSpearEngineCtrl : NSObject {
}

/*!
 @abstract      设置Spear场景。
 @discussion    设置Spear场景。
 @param         scene           Spear场景
 @return        设置成功返回AV_OK, 否则返回QAV_ERR_FAIL。
 */
- (int)setScene:(int)scene;

/*!
 @abstract      设置特定角色流控参数。
 @param         jsonParam       流控参数。
 @param         role            流控角色名
 @return        设置成功返回AV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVResult
 */
- (QAVResult)addParamByRole:(NSString *)role jsonParam:(NSString *)jsonParam;

/*!
 @abstract      清空所有流控参数。
 */
- (void)clear;

/*!
 @abstract      获取当前自定义流控参数个数。
 @return        流控参数个数。
 */
- (int)getParamCount;

/*!
 @abstract      根据index获取已存在的流控角色名。
 @param         index           流控角色索引。
 @return        存在则返回流控角色名, 否则返回空串。
 */
- (NSString *)getRoleByIndex:(int)index;

/*!
 @abstract      判断角色是否已存在。
 @param         role            流控角色名
 @return        存在则返回YES, 否则返回NO。
 */
- (BOOL)hasRole:(NSString *)role;

/*!
 @abstract      获取流控角色对应的流控参数。
 @param         role            流控角色名
 @return        存在则返回对应的流控参数, 否则返回空串。
 */
- (NSString *)getParamByRole:(NSString *)role;

/*!
 @abstract      获取当前默认流控角色名。
 @discussion    SDK缺省将index为0对应的角色设置为默认角色。
 @return        存在则返回流控角色名, 否则返回空串。
 */
- (NSString *)getDefaultRole;

/*!
 @abstract      设置当前默认流控角色名。
 @param         role            流控角色名
 @return        成功返回AV_OK, 否则返回QAV_ERR_FAIL。
 @see           QAVResult
 */
- (QAVResult)setDefaultRole:(NSString *)role;


@end
