#import "QAVSDK.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
/*!
 @abstract      渲染视图类
 @discussion    一定要使用createRenderView来创建对象
 */
@interface QAVGLRenderView : NSObject

/*!
 @abstract      渲染视图的frame。
*/
@property (nonatomic, readonly) CGRect frame;

/*!
 @abstract      渲染视图对应的key
 */
@property (nonatomic, readonly) NSString *key;

/*!
 @abstract      创建QAVGLRenderView对象。
 @param         frame           创建QAVGLRenderView所需的frame。
 @return        返回QAVGLRenderView的实例指针；
 */
+ (QAVGLRenderView *)createRenderView:(CGRect)frame;

/*!
 @abstract      修改QAVGLRenderView对象的frame。
 @param         frame           修改QAVGLRenderView所需的frame。
 */
- (void)setFrame:(CGRect)frame;

/*!
 @abstract      设置QAVGLRenderView对象的镜像属性。
 @param         needMirror      修改QAVGLRenderView所需的frame。
 */
- (void)setNeedMirrorReverse:(BOOL)needMirror;

- (void)setHasBlackEdge:(BOOL)hasEdge;

@end


/*!
 @abstract      控制渲染视图类
 @discussion    一定要使用createBaseView来创建对象,添加进来的QAVGLRenderView都是在这里进行渲染的
 */
@interface QAVGLBaseView : UIView <QAVLocalVideoDelegate, QAVRemoteVideoDelegate, QAVScreenVideoDelegate, QAVMediaVideoDelegate>

/*!
 @abstract      渲染视图的key集合。第一个元素代表最底层的视图key，最后一个元素代表最上层的视图key
 */
@property (nonatomic, readonly) NSMutableArray *subViewsKey;

/*!
 @abstract      创建QAVGLBaseView对象。
 @param         frame           创建QAVGLBaseView所需的frame。
 @return        返回QAVGLRenderView的实例指针；
 @warning       一定要使用createBaseView:(CGRect)frame 来创建QAVGLBaseView,并且frame的size不能为0
                使用initWithFrame:(CGRect)frame会出现无法预料的结果。
 */
+ (QAVGLBaseView *)createBaseView:(CGRect)frame;

/*!
 @abstract      是否允许切换大小画面切换，应当在初始化之后调用。
 @param         bEnable     是否打开大小画面切换
 */
- (void)enableSwitchViewSize:(BOOL)bEnable;

/*!
 @abstract      销毁QAVGLBaseView对象。
 @warning       在结束渲染的时候一定要调用这个方法
 */
- (void)destroyBaseView;

/*!
 @abstract      通过QAVGLRenderView对象的key将QAVGLRenderView对象的层次放到最顶端。
                对应subViewsKey中的第一个key
 @param         key             QAVGLRenderView对象对应的key。
 */
- (void)bringSubviewToFront:(NSString *)key;

/*!
 @abstract      通过QAVGLRenderView对象的key将QAVGLRenderView对象的层次放到最底端。
                对应subViewKey中的最后一个key
 @param         key             QAVGLRenderView对象对应的key。
 */
- (void)bringSubviewToBack:(NSString *)key;

/*!
 @abstract      增加QAVGLRenderView对象。
 @discussion    key与QAVGLRenderView对象是一一对应的。渲染本地视图key默认为1
 @param         view            key所对应的QAVGLRenderView实例化对象
 @param         key             QAVGLRenderView对象对应的唯一的key key为identifier+srcType
                                - srcType = 1 ->摄像头。
                                - srcType = 2 ->屏幕分享。
 */
- (void)addSubview:(QAVGLRenderView *)view forKey:(NSString *)key;

/*!
 @abstract      增加QAVGLRenderView对象。
 @discussion    key与QAVGLRenderView对象是一一对应的。渲染本地视图key默认为1
 @param         view            key所对应的QAVGLRenderView实例化对象
 @param         key             QAVGLRenderView对象对应的唯一的key key为identifier+srcType
 @param         image          如果没有视频数据流时，默认显示image，image大小最好与view成比例，参数可以为空
 - srcType = 1 ->摄像头。
 - srcType = 2 ->屏幕分享。
 */
- (void)addSubview:(QAVGLRenderView*)view withBackgroundImage:(UIImage*)image forKey:(NSString *)key;

/*!
 @abstract      为渲染控制视图设置背景图片
 @param         image            所设置的背景图片
 */
- (void)setBackgroundImage:(UIImage*)image;

/*!
 @abstract      移除QAVGLRenderView对象。
 @discussion    key与QAVGLRenderView对象是一一对应的。渲染本地视图key默认为1
 @param         key             QAVGLRenderView对象对应的唯一的key key为identifier+srcType
                                - srcType = 1 ->摄像头。
                                - srcType = 2 ->屏幕分享。
 */

- (void)removeSubviewForKey:(NSString *)key;

/*!
 @abstract      得到QAVGLRenderView对象。
 @discussion    key与QAVGLRenderView对象是一一对应的。渲染本地视图key默认为1
 @param         key             QAVGLRenderView对象对应的唯一的key key为identifier+srcType
                                - srcType = 1 ->摄像头。
                                - srcType = 2 ->屏幕分享。
 */
- (QAVGLRenderView *)getSubviewForKey:(NSString *)key;

/*!
 @abstract      停止所有渲染
 */
- (void)stopDisplay;

/*!
 @abstract      开始所有渲染
 */
- (void)startDisplay;

/*!
 @abstract      锁定屏幕角度, 与setRotation:(rotateAngle)rotate配合使用
 @param         angle       angle与UIDeviceOrientation对应关系如下:<br/>
                UIDeviceOrientationPortrait:          0;
                UIDeviceOrientationLandscapeLeft:     1;
                UIDeviceOrientationLandscapeRight:    3;
                UIDeviceOrientationPortraitUpsideDown:2;
 */
- (void)lockAngle:(int)angle;

- (void)setHasBlackEdge:(BOOL)hasEdge;

@end
