# IOS_TRTC
腾讯实时音视频(TRTC)，集成了账号登录、音视频通话、文本消息聊天等基础功能，可在无音视频基础技术的情况下，快速接入开发定制化的实时音视频产品。
# 集成音视频SDK
我们将底层音视频能力封装成了一套SDK集合，包含有
BeautySDK：提供美颜预处理功能
IMSDK：提供 IM 即时通信功能
AVSDK：提供底层音视频功能
ILiveSDK：在 AVSDK 基础上封装而成，提供更简单易用的音视频功能接口
在开发自己的实时音视频产品前需要先将上述SDK集成在工程中，详细集成方法请参考https://cloud.tencent.com/document/product/647/16809
# API调用
## 初始化SDK
在成功集成SDK后，建议在程序启动时就进行SDK初始化，初始化需要用到在腾讯云后台创建应用时分配的 SDKAppId 和 AccountType，示例代码如下：
// 定义SDKAppId和AccountType
static  const int kSDKAppID = 后台创建应用对应的SDKAppId;
static  const int kAccountType = 后台创建应用对应的AccountType;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // 初始化SDK
    [[ILiveSDK getInstance] initSdk:kSDKAppID accountType:kAccountType];

    return YES;
}
## 账号登录
## 进入房间
## 退出房间
## 文本消息处理
# 定制化说明

