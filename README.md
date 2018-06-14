# IOS_TRTC
腾讯实时音视频(TRTC)，集成了账号登录、音视频通话、文本消息聊天等基础功能，可在无音视频基础技术的情况下，快速接入开发定制化的实时音视频产品。
# 集成音视频SDK
我们将底层音视频能力封装成了一套SDK集合，包含有

SDK  | 说明
------------- | -------------
IMSDK  | 提供 IM 即时通信功能
AVSDK  | 提供底层音视频功能
ILiveSDK  | 在 AVSDK 基础上封装而成，提供更简单易用的音视频功能接口
BeautySDK  | 提供美颜预处理功能

在开发自己的实时音视频产品前需要先将上述SDK集成在工程中。

详细集成方法请参考<https://cloud.tencent.com/document/product/647/16809>
# API调用
在集成完SDK后，只需要5步API调用，即可完成音视频通话、文本消息聊天等功能，具体调用接口如下：

1、初始化SDK

	[[ILiveSDK getInstance] initSdk:sdkAppID accountType:accountType];
详情参见：https://cloud.tencent.com/document/product/647/16810
2、账号登录

	[[ILiveLoginManager getInstance] iLiveLogin:userID sig:userSig succ:^{
        NSLog(@"-----> login  succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> login  fail,%@ %d %@",module, errId, errMsg);        
    }];
详情参见：https://cloud.tencent.com/document/product/647/16810
3、创建房间或者加入房间

如果房间号不存在使用创建房间接口
	
	[[ILiveRoomManager getInstance] createRoom:roomID option:option succ:^{
        NSLog(@"-----> create room succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> create room fail,%@ %d %@",module, errId, errMsg);
    }];
详情参见：https://cloud.tencent.com/document/product/647/16811
如果房间号已存在则使用加入房间接口

	[[ILiveRoomManager getInstance] joinRoom:[self.roomInfo[@"roomID"] intValue] option:option succ:^{
        NSLog(@"-----> join room succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> join room fail,%@ %d %@",module, errId, errMsg);
    }];
详情参见：https://cloud.tencent.com/document/product/647/16814
4、视频画面展示
	
	- (BOOL)onEndpointsUpdateInfo:(QAVUpdateEvent)event updateList:(NSArray *)endpoints{
	    for (QAVEndpoint *endoption in endpoints) {
	        switch (event) {
	            case QAV_EVENT_ID_ENDPOINT_HAS_CAMERA_VIDEO:
	            {
	            ILiveFrameDispatcher *frameDispatcher = [[ILiveRoomManager getInstance] getFrameDispatcher];
                //传入成员userid获取画面
                ILiveRenderView *renderView = [frameDispatcher addRenderAt:CGRectZero forIdentifier:endoption.identifier srcType:QAVVIDEO_SRC_TYPE_CAMERA];
                [self addLiveRenderView:renderView];//添加画面到指定view
	            }
	                break;	                
	            default:
	                break;
	        }
	    }
	    return YES;
	}
详情参见：https://cloud.tencent.com/document/product/647/16814
5、消息处理
	
	设置消息监听
	[[[ILiveSDK getInstance] getTIMManager] setMessageListener:self];
	消息接收
	-(void)onNewMessage:(NSArray *)msgs{  
		//消息处理  
	}
	消息发送
	[[ILiveRoomManager getInstance] sendGroupMessage:msge succ:^{
	        NSLog(@"send message succ");
	    } failed:^(NSString *module, int errId, NSString *errMsg) {
	        NSLog(@"send message fail");
	 }];
详情参见：https://cloud.tencent.com/document/product/647/16815
# 一步接入视频通话
本文不仅了集成账号登录、音视频通话、文本消息互动的基础能力，还提供一套简单易用的视频通话界面，在完成账号登录后，三行代码即可接入，首先将TCLiveRoomVC文件夹内容拷贝到自己工程，然后初始化TCLiveRoomViewController即可，代码示例：

	TCLiveRoomViewController *vc = [[TCLiveRoomViewController alloc] init];
	vc.roomInfo = @{@"roomID":@(123456)};
    [self.navigationController pushViewController:vc animated:YES];
在复用的同时，本界面中的视频画面、消息聊天、控制按钮模块都可单独使用，只需要创建设置大小位置，添加到指定界面即可，代码示例如下：
	
	
	- (void)viewDidLoad {
	    [super viewDidLoad];
	
	    [self.view addSubview:self.videoLayoutView];
	    
	    [self.view addSubview:self.chatTableView];
	    
	    [self.view addSubview:self.controlBar];
	    
	    [self.view addSubview:self.inputTextField];
    
    }
	//视频区域
	-(TCLiveVideoLayoutView *)videoLayoutView{
	    if (!_videoLayoutView) {
	        _videoLayoutView = [[TCLiveVideoLayoutView alloc] initWithFrame:CGRectZero];
	        _videoLayoutView.roomInfo = self.roomInfo;
	    }
	    _videoLayoutView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
	    return _videoLayoutView;
	}	

	//消息列表
	- (UITableView *)chatTableView{
	    if (!_chatTableView) {
	        _chatTableView = [[TCLiveChatTableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
	        _chatTableView.backgroundColor = [UIColor clearColor];
	    }
	    _chatTableView.frame = CGRectMake(0, self.view.frame.size.height - LIVE_CONTROL_BAR_HEIGHT - 300, 250, 300);
	    return _chatTableView;
	}
		//控制bar
	- (TCLiveVideoControlBar *)controlBar{
	    if (!_controlBar) {
	        _controlBar = [[TCLiveVideoControlBar alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - LIVE_CONTROL_BAR_HEIGHT, self.view.frame.size.width, LIVE_CONTROL_BAR_HEIGHT)];
	        _controlBar.delegate = self;
	    }
	    return _controlBar;
	}
		//输入框
	- (UITextField *)inputTextField{
	    if (!_inputTextField) {
	        _inputTextField = [[UITextField alloc] initWithFrame:CGRectZero];
	        _inputTextField.delegate = self;
	        _inputTextField.backgroundColor= [UIColor whiteColor];
	        _inputTextField.borderStyle = UITextBorderStyleLine;
	        _inputTextField.placeholder = @"请输入内容";
	        _inputTextField.returnKeyType = UIReturnKeySend;
	    }
	    _inputTextField.frame = CGRectMake(0, self.view.frame.size.height , self.view.frame.size.width, LIVE_INPUTTEXTFIELD_HEIGHT);
	    return _inputTextField;
	}
