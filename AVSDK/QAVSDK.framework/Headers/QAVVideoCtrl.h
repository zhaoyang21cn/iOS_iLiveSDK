#import "QAVCommon.h"
#import "QAVError.h"
#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
@protocol QAVRemoteVideoDelegate;
@protocol QAVLocalVideoDelegate;
@protocol QAVScreenVideoDelegate;
@protocol QAVMediaVideoDelegate;

/*!
 @abstract      摄像头操作回调
 @param         result          错误码。不同操作的异常错误码略有不同，参考具体API调用。
 @see           QAVResult
 */
typedef void (^cameraOptionComplete)(int result);
/*!
 @abstract      录屏操作回调
 @param         result          错误码。
 @see           QAVResult
 */
typedef void (^screenOptionComplete)(int result);

/*!
 @discussion    水印功能需要的分辨率类型
*/
typedef NS_ENUM(NSInteger, QAVCameraPreset) {
  
    QAVPresetType320240  = 1,
    QAVPresetType480360  = 2,
    QAVPresetType640480  = 3,
    QAVPresetType640368  = 4,
    QAVPresetType960540  = 5,
    QAVPresetType1280720 = 6,
    QAVPresetType192144  = 7,
    QAVPresetType320180  = 8,
};

/*!
 @discussion    摄像头位置，区分前后置摄像头
 */
typedef NS_ENUM(NSInteger, cameraPos) {
    CameraPosFront = 0, ///< 前置摄像头
    CameraPosBack  = 1, ///< 后置摄像头
};

/*!
 @discussion    摄像头数据发送模式
 */
typedef NS_ENUM(NSInteger, QAVCameraMode) {
    QAVCameraModeDefault = 0, ///< 默认模式，摄像头启动后会发送数据给服务器
    QAVCameraModeAutoSending  = 1, ///< 自动发送模式，摄像头启动后会发送数据给服务器
    QAVCameraModeNotSending = 2,  ///< 预览模式，摄像头启动后不会发送数据给服务器
};

/*!
 @discussion    采集角度，0,1,2,3分别对应iOS四个采集角度，4表示关闭外部设定
 */
typedef NS_ENUM(NSInteger, rotateAngle) {
    OrientationLandscapeRight = 0,      // Device oriented horizontally, home button on the left
    OrientationPortrait = 1,            // Device oriented vertically, home button on the bottom
    OrientationLandscapeLeft = 2,       // Device oriented horizontally, home button on the right
    OrientationPortraitUpsideDown = 3,  // Device oriented vertically, home button on the top
    OrientationUnknown = 4,             // close external orientation
};

/*!
 @brief AVVideoCtrl为视频控制器，主要用于简化视频用法。<br/>
 @discussion 通过AVVideoCtrl，可以处理本地摄像头数据、接收远端发送的数据，以及获取视频数据做一些图像处理逻辑等。<br/>
 在SDK正常启动之后，可以通过AVContext的GetAVVideoCtrl()来获取AVAVVideoCtrl对象。<br/>
 
 另外需要注意的是，当App被切到后台，需要关闭摄像头，否则会导致系统资源被占用而无法使用。<br/>
 */
@interface QAVVideoCtrl : NSObject

/*!
 @abstract      摄像头是否打开
 */
@property (assign, nonatomic) BOOL isCameraOn;

/*!
 @abstract      是否启用录屏
 */
@property (assign, nonatomic) BOOL isScreenRecordOn;

/*!
 @abstract      打开/关闭摄像头
 @discussion    主线程调用，异步返回结果。本地同时只能打开一个摄像头，同一房间内支持最多10个人同时打开摄像头。
 @param         pos             选择要操作的摄像头，CameraPosFront 前置，CameraPosBack 后置。
                                其他值会返回 AV_ERR_DEVICE_NOT_EXIST。
                                @see cameraPos
 @param         bEnable         是否打开，YES 打开，NO 关闭。
 @param         block           异步返回执行的结果 (^cameraOptionComplete)
 
 @return        QAV_OK 调用成功。只有返回QAV_OK时，才会通过block异步返回执行结果；其他情况不会回调block。
                QAV_ERR_NOT_IN_MAIN_THREAD 非主线程调用。
                QAV_ERR_EXCLUSIVE_OPERATION 重复操作，上次操作未完成。
                QAV_ERR_DEVICE_NOT_EXIST 设备不存在。
                其他情况请参照QAVResult。
 @see           QAVResult
 */
- (QAVResult)enableCamera:(cameraPos)pos isEnable:(BOOL)bEnable complete:(cameraOptionComplete)block;


/*!
 @abstract      打开/关闭摄像头
 @discussion    主线程调用，异步返回结果。本地同时只能打开一个摄像头，同一房间内支持最多10个人同时打开摄像头。
 @param         pos             选择要操作的摄像头，CameraPosFront 前置，CameraPosBack 后置。
 其他值会返回 AV_ERR_DEVICE_NOT_EXIST。
 @see cameraPos
 @param         bEnable         是否打开，YES 打开，NO 关闭。
 @param         mode            摄像头数据发送模式
 @param         completion      异步返回执行的结果 (^cameraOptionComplete)
 
 @return        QAV_OK 调用成功。只有返回QAV_OK时，才会通过block异步返回执行结果；其他情况不会回调block。
                QAV_ERR_NOT_IN_MAIN_THREAD 非主线程调用。
                QAV_ERR_EXCLUSIVE_OPERATION 重复操作，上次操作未完成。
                QAV_ERR_DEVICE_NOT_EXIST 设备不存在。
                其他情况请参照QAVResult。
 @see           QAVResult
 */
- (QAVResult)enableCamera:(cameraPos)pos isEnable:(BOOL)bEnable mode:(QAVCameraMode)mode completion:(cameraOptionComplete)completion;

/*!
 @abstract      更新摄像头当前模式
 @discussion    在打开摄像头的情况下，可以通过这个接口动态修改当前摄像头的工作模式。
 @param         cameraId 摄像头设备id
 @param         mode 自动上传数据 CAMERA_MODE_AUTO_SENDING， 不上传数据 CAMERA_MODE_NOT_SENDING。
 @return        QAV_OK 成功。
 @note：        这个接口可以替换1.9.5版本的enableCameraSendPermission
 */
- (QAVResult)updateCameraMode:(cameraPos)cameraId mode:(QAVCameraMode)mode;

/*!
 @abstract      是否传输视频数据
 @discussion    在打开摄像头的情况下，可以通过这个接口使能视频数据发送。
 @param         enableTransmission 是否传输
 @param         completion 移步回调结果
 @note：        这个接口可以替换1.9.6版本的updateCameraMode;如果回调返回1，并且原因是send cmd failed，请稍后重试一下
 */
- (void)enableVideoTransmission:(BOOL) enableTransmission  completion:(QAVCompletionHandler)completion;


/*!
 @abstract      切换摄像头。
 @discussion    需要先调用 enableCamera 或 enableCameraPreview 打开摄像头成功后才生效。
 @param         pos             选择要操作的摄像头，CameraPosFront 前置，CameraPosBack 后置。
                                其他值会返回 AV_ERR_DEVICE_NOT_EXIST。
 @param         block           异步返回执行的结果 (^cameraOptionComplete)
 
 @return        QAV_OK 调用成功。只有返回QAV_OK时，才会通过block异步返回执行结果；其他情况不会回调block。
                QAV_ERR_NOT_IN_MAIN_THREAD 非主线程调用。
                QAV_ERR_EXCLUSIVE_OPERATION 重复操作，上次操作未完成。
                QAV_ERR_DEVICE_NOT_EXIST 设备不存在，例如手机只有一个摄像头。
                其他情况请参照QAVResult。
 @see           QAVResult
 */
- (QAVResult)switchCamera:(cameraPos)pos complete:(cameraOptionComplete)block;
/*!
 @abstract      判断当前摄像头为前置还是后置
 @return        YES 前置，NO 后置。
 */
- (BOOL)isFrontcamera;

/*!
 @abstract      设置视频数据的采集角度。
 @param         rotate  OrientationLandscapeRight，OrientationPortrait，
                        OrientationLandscapeLeft，OrientationPortraitUpsideDown，设置固定的采集角度，设置后SDK不再自动做旋转。
                        OrientationUnknown，清除用户设置的角度，SDK自动判断采集角度，自动处理旋转逻辑。
 @see           rotateAngle
 @return        QAV_OK 成功。
                QAV_ERR_NOT_IN_MAIN_THREAD 非主线程调用。
                QAV_ERR_FAIL VideoCtrl实例不存在。
                其他情况请参照QAVResult。
 @see           QAVResult
 */
- (QAVResult)setRotation:(rotateAngle)rotate;
/*!
 @abstract      设置获取本地视频数据的delegate。
 @discussion	设置后，可以获取本地采集的视频数据，同时SDK停止本地预览。
                仅对于摄像头采集视频生效，外部采集的方案不生效。
 @param         delegate        用于获取本地视频数据，同时关闭SDK本地渲染；为nil时取消回调，同时恢复SDK本地渲染。
 */
- (void)setLocalVideoDelegate:(id<QAVLocalVideoDelegate>)delegate;

/*!
 @abstract      设置接收远端视频数据的delegate。
 @discussion	设置后，可以获取所请求的远端视频数据，对其进行处理或渲染，同时SDK停止远端视频的渲染。
 @param         delegate        用于获取远端视频数据，同时关闭SDK远端渲染；为nil时取消回调，同时恢复SDK远端渲染。
 */
- (void)setRemoteVideoDelegate:(id<QAVRemoteVideoDelegate>)delegate;

/*!
 @abstract      设置接收远端屏幕分享视频的delegate。
 @param         delegate        用于获取远端屏幕分享数据，为nil时取消回调。
*/
- (void)setScreenVideoDelegate:(id<QAVScreenVideoDelegate>)delegate;

/*!
 @abstract      设置接收远端播放的视频文件数据的delegate。
 @param         delegate        用于接收远端播放的视频，为nil时取消回调。
 */
- (void)setMediaVideoDelegate:(id<QAVMediaVideoDelegate>)delegate;

/*!
 @abstract      打开/关闭外部视频采集功能。
 discussion     SDK提供两套视频采集方案：SDK控制摄像头采集和外部采集。
                外部采集支持直接输入视频数据，视频数据可以来自摄像头、视频文件、扩展设备、第三方视频源等等，同时，用户可以对视频数据进行前处理后再传给SDK。
                SDK采集和外部采集两个方案是互斥的，只能二选一；打开外部采集时，会关闭SDK的摄像头采集。

 @param         isEnableExternalCapture YES 打开，NO 关闭。
 @param         shouldRender 打开外部采集时，本地Preview是否使用SDK内置的视频渲染。YES 使用SDK渲染，NO 不使用SDK渲染。
 @return        QAV_OK 成功。
                QAV_ERR_NOT_IN_MAIN_THREAD 非主线程调用。
                QAV_ERR_EXCLUSIVE_OPERATION 重复操作，上次操作未完成。
                QAV_ERR_FAIL 失败。
 
 @see           fillExternalCaptureFrame
 @see           OnCameraSettingNotify
 @see           QAVResult
 */
- (QAVResult)enableExternalCapture:(BOOL)isEnableExternalCapture shouldRender:(BOOL)shouldRender;

/*!
 @abstract      开启外部视频采集功能时，向SDK传入外部采集的视频帧。

 @discussion    - 不建议在主线程调用，在子线程中调用该函数。<br/>
                - 为防止快进和慢放，调用频率要平滑，尽量和视频源帧率一致。<br/>
                - SDK会通过OnCameraSettingNotify事件通知最优帧率和分辨率，尽量选择和事件通知一致的帧率、分辨率。如果设备不支持，帧率选择大于事件通知的帧率，分辨率选择等比例且大于事件通知的分辨率。<br/>
                - 支持的视频格式参考 @see avColorFormats。<br/>
                - 视频源类型目前仅支持QAVVIDEO_SRC_TYPE_CAMERA，@see avVideoSrcType。<br/>
  
 @param         frame           视频帧数据，可以来自摄像头、视频文件、扩展设备、第三方视频源等等。
                                可以对视频数据做美颜、滤镜、特效等处理后再传给SDK，但是要按照 QAVVideoFrame 的格式传入。
 @return        QAV_OK 成功。
                QAV_ERR_ROOM_NOT_EXIST 房间不存在，进房后调用才生效。
                QAV_ERR_DEVICE_NOT_EXIST 视频设备不存在。
                QAV_ERR_FAIL 失败。
 
 @see           QAVVideoFrame
 */
- (int)fillExternalCaptureFrame:(QAVVideoFrame *)frame;

/*!
 @abstract      获取摄像头视频的预览layer。
 @return        成功返回AVCaptureVideoPreviewLayer实例，失败返回nil。
 */
- (AVCaptureVideoPreviewLayer *)getPreviewLayer;

/*!
 @abstract      获取摄像头的CaptureSession
 @return        成功返回AVCaptureSession实例，失败返回nil。
 */
- (AVCaptureSession *)getCaptureSession;

/*!
 @abstract      查询当前机型是否支持美颜、美白。
 @discussion    支持美颜的机型：iphone4s及以上,ipad1及以上，ipodtouch5及以上。
                通常美颜、美白一起使用，该标准也适用于美白。
 @return        YES 支持，NO 不支持。
 */
- (BOOL)isEnableBeauty;

/*!
 @abstract      设置美颜程度，打开美颜功能后，可设置美颜程度。
 @param         beautyParam     美颜程度，取值范围 0 ~ 9，0代表关闭美颜。
 */
- (void)inputBeautyParam:(float)beautyParam;

/*!
 @abstract      设置美白程度，打开美白功能后，可设置美白程度。
 @param         beautyParam     美白程度，取值范围 0 ~ 9，0代表关闭美白。
 */
- (void)inputWhiteningParam:(float)beautyParam;

/*!
 @abstract      添加水印到某一编码分辨率。一个编码分辨率对应一个水印图片。
 @param         preset          对应的编码分辨率，具体参照cameraPos。
 
                                QAVPresetType320240  = 1,
                                QAVPresetType480360  = 2,
                                QAVPresetType640480  = 3,
                                QAVPresetType640368  = 4,
                                QAVPresetType960540  = 5,
                                QAVPresetType1280720 = 6,
                                QAVPresetType192144  = 7,
                                QAVPresetType320180  = 8,
 
 @param         watermark       水印图片必须先加载成UIImage。格式的话一般是建议png
 
 @discussion    - 这个接口注意在startcontext成功后，打开摄像头之前调。<br/>
                - 注意水印的大小不能超过preset参数里的宽的1/4,高的1/6。<br/>
                - 水印的宽、高必须是2的整数倍。<br/>
 
 @return        QAV_OK 成功，QAV_ERR_FAIL 失败。
 
 @see           QAVCameraPreset  QAVResult
 
 */
- (QAVResult)addWaterMark:(QAVCameraPreset)preset image:(UIImage *)watermark;

/*!
 @abstract      实时获取通话中的视频质量信息。
 @discussion	进房后、主线程调用。主要用来查看通话情况、排查问题等，用户不关注可以不调用。
 @return        以字符串形式返回视频相关的质量信息。
 */
- (NSString *)getQualityTips;

/*!
 @abstract      开启/关闭录屏
 @param         isEnable    YES 开启，NO 关闭。
 @param         mode                    清晰度模式，1 超清模式，2 高清模式，3 标清模式。
 @param         block                   异步返回执行结果的 (^screenOptionComplete)
 @return        QAV_OK 调用成功。只有返回QAV_OK时，才会通过block异步返回执行结果；其他情况不会回调block。
                QAV_ERR_NOT_IN_MAIN_THREAD  非主线程调用。
                QAV_ERR_CONTEXT_NOT_START   context未start。
                QAV_ERR_ROOM_NOT_EXIST      房间未创建或已销毁。
                AV_ERR_RESOURCE_IS_OCCUPIED 房间内已有人进行屏幕分享或者播放视频文件
                QAV_ERR_FAIL 其他错误
 @see           QAVResult
 */
- (QAVResult)enableScreenRecord:(BOOL)isEnable withMode:(int)mode complete:(screenOptionComplete)block;

/*!
@abstract      向音视频SDK传入捕获的屏幕画面。
@param         frame           视频帧数据及相关参数。
@discussion    - 要控制好传入视频帧的频率，具体频率视实际使用场景而定。
               - 图像颜色格式。仅支持yuv格式。
               - 视频源类型。当前仅支持QAVVIDEO_SRC_TYPE_SCREEN。
@return        QAV_OK 调用成功。
               QAV_ERR_CONTEXT_NOT_START context未start。
               QAV_ERR_ROOM_NOT_EXIST      房间未创建或已销毁。
               QAV_ERR_INVALID_ARGUMENT    数据帧错误，空指针或者格式错误
               QAV_ERR_FAIL 其他错误
@see           QAVVideoFrame
  */
- (QAVResult)fillScreenRecordFrame:(QAVVideoFrame*)frame;

@end

/*!
 @discussion    获取本地视频数据的delegate。
 */
@protocol QAVLocalVideoDelegate <NSObject>
@required

/*!
 @abstract      本地视频预览的回调
 @discussion	主线程回调，方面直接在回调中实现视频渲染。
 @param         frameData       本地预览的视频帧，仅用于预览，对视频帧的修改不会影响视频传输。
 @see           QAVVideoFrame
 */
- (void)OnLocalVideoPreview:(QAVVideoFrame *)frameData;

/*!
 @abstract      对本地采集视频进行预处理的回调。
 @discussion	主线程回调，方面直接在回调中实现视频渲染。
 @param         frameData       本地采集的视频帧，对其中data数据的美颜、滤镜、特效等图像处理，会回传给SDK编码、发送，在远端收到的视频中生效。
 @see           QAVVideoFrame
 */
- (void)OnLocalVideoPreProcess:(QAVVideoFrame *)frameData;

/*!
 @abstract      本地摄像头采集原始数据格式的回调。
 @discussion	主线程回调，方面直接在回调中实现视频渲染。
				同 OnLocalVideoPreProcess 的效果，对视频数据的处理，会在远端收到的视频中生效。
 @param         buf             摄像头采集的原始格式视频数据。
 @param         ret             摄像头采集的原始格式视频数据。
 */
- (void)OnLocalVideoRawSampleBuf:(CMSampleBufferRef)buf result:(CMSampleBufferRef *)ret;
@end
/*!
 @discussion    接收远端视频数据的delegate。
 */
@protocol QAVRemoteVideoDelegate <NSObject>
@required
/*!
 @abstract      接收远端视频数据的回调。
 @discussion	主线程回调，方面直接在回调中实现视频渲染。
 @param         frameData       收到的远端视频帧。
 @see           QAVVideoFrame
 */
- (void)OnVideoPreview:(QAVVideoFrame *)frameData;
@end

/*!
 @discussion    接收远端屏幕分享视频的delegate。
 */
@protocol QAVScreenVideoDelegate <NSObject>
@required
/*!
 @abstract      接收屏幕分享数据的回调。
 @discussion	主线程回调，方面直接在回调中实现视频渲染。
 @param         frameData       收到的屏幕分享的视频帧。
 @see           QAVVideoFrame
 */
- (void)OnVideoPreview:(QAVVideoFrame *)frameData;
@end

/*!
 @discussion    接收远端播放的视频文件数据的delegate。
 */
@protocol QAVMediaVideoDelegate <NSObject>
@required
/*!
 @abstract      接收视频文件数据的回调。
 @discussion	主线程回调，方面直接在回调中实现视频渲染。
 @param         frameData       收到的远端播放视频文件的视频帧。
 @see           QAVVideoFrame
 */
- (void)OnVideoPreview:(QAVVideoFrame *)frameData;
@end

