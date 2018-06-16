#import <Foundation/Foundation.h>

/*!
 @discussion    房间成员类
 */
@interface QAVEndpoint : NSObject {
}

/*!
 @abstract      成员id。
 */
@property (readonly, nonatomic) NSString *identifier;

/*!
 @abstract      是否有发音频。
 */
@property (readonly, nonatomic) BOOL isAudio;

/*!
 @abstract      是否为跨房间连麦成员。
 */
@property (readonly, nonatomic) BOOL isLinkedRoomUser;

/*!
 @abstract      是否有发来自摄像头或外部视频捕获设备的视频。
 */
@property (readonly, nonatomic) BOOL isCameraVideo;

/*!
 @abstract      是否有发来自屏幕的视频。
 */
@property (readonly, nonatomic) BOOL isScreenVideo;

/*!
 @abstract      是否有发来自播片的视频。
 */
@property (readonly, nonatomic) BOOL isMediaVideo;

@end
