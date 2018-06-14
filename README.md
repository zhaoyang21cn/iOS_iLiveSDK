# IOS_TRTC
腾讯实时音视频(TRTC)，集成了账号登录、音视频通话、文本消息聊天等基础功能，可在无音视频基础技术的情况下，快速接入开发定制化的实时音视频产品。
# 集成音视频SDK
我们将底层音视频能力封装成了一套SDK集合，包含有

IMSDK：提供 IM 即时通信功能
AVSDK：提供底层音视频功能
ILiveSDK：在 AVSDK 基础上封装而成，提供更简单易用的音视频功能接口
BeautySDK：提供美颜预处理功能
在开发自己的实时音视频产品前需要先将上述SDK集成在工程中。

详细集成方法请参考<https://cloud.tencent.com/document/product/647/16809>
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
登录相关接口在ILiveLoginManager类中，调用方法：

	- (void)iLiveLogin:(NSString *)uid sig:(NSString *)sig succ:(TCIVoidBlock)succ failed:(TCIErrorBlock)failed;
其中uid，sig为用户登录id和密钥，需要从腾讯云后台获取，本文也提供了获取方法，调用接口
	
	- (void)requestLoginInfo:(LiveLoginInfoBlock)block;
代码示例：
	
	[[TCLiveRequestManager getInstance] requestLoginInfo:^(int code) {
        if (code == 0) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [[ILiveLoginManager getInstance] iLiveLogin:[TCLiveRequestManager getInstance].userID sig:[TCLiveRequestManager getInstance].userSig succ:^{
                    NSLog(@"-----> login  succ");            
                } failed:^(NSString *module, int errId, NSString *errMsg) {
                    NSLog(@"-----> login  fail,%@ %d %@",module, errId, errMsg);
                }];
            });
        }
    }];
## 进入房间
在本工程中用户可任意输入房间号码(整数数字)进入房间，进入房间的API有两种，第一种房间号不存在则需要以创建房间的方式进入，第二种若房间号存在则直接加入房间，具体调用方法如下：

1.创建房间

	- (void)doEnterRoom{
	    //房间配置项
	    ILiveRoomOption *option = [ILiveRoomOption defaultHostLiveOption];
	    //是否支持im
	    option.imOption.imSupport = YES;
	    //设置房间内音视频事件监听
	    option.memberStatusListener = self.videoLayoutView;
	    //设置房间中断监听
	    option.roomDisconnectListener = self;
	
	    [[ILiveRoomManager getInstance] createRoom:[self.roomInfo[@"roomID"] intValue] option:option succ:^{
	        NSLog(@"-----> create room succ");
	    } failed:^(NSString *module, int errId, NSString *errMsg) {
	        NSLog(@"-----> create room fail,%@ %d %@",module, errId, errMsg);
	    }];
	}
2.加入房间

	    [[ILiveRoomManager getInstance] joinRoom:[self.roomInfo[@"roomID"] intValue] option:option succ:^{
        NSLog(@"-----> join room succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> join room fail,%@ %d %@",module, errId, errMsg);
    }];
传入的参数有两个，一个roomID即房间号码，另个一个option为房间的配置参数。
    
3 视频画面展示

通过上述方法进入房间后，由于设置了音视频事件监听，当房间有音视频事件发生时会触发回调方法

	- (BOOL)onEndpointsUpdateInfo:(QAVUpdateEvent)event updateList:(NSArray *)endpoints
例如有成员进入房间，触发QAV_EVENT_ID_ENDPOINT_HAS_CAMERA_VIDEO，从而可以获得该成员视频画面，并添加在需要展示的view上，例如：

	- (BOOL)onEndpointsUpdateInfo:(QAVUpdateEvent)event updateList:(NSArray *)endpoints{
	    if (endpoints.count <= 0) {
	        return NO;
	    }
	    for (QAVEndpoint *endoption in endpoints) {
	        switch (event) {
	            case QAV_EVENT_ID_ENDPOINT_HAS_CAMERA_VIDEO:
	            {
	                ILiveFrameDispatcher *frameDispatcher = [[ILiveRoomManager getInstance] getFrameDispatcher];
                //传入成员userid获取画面
                ILiveRenderView *renderView = [frameDispatcher addRenderAt:CGRectZero forIdentifier:endoption.identifier srcType:QAVVIDEO_SRC_TYPE_CAMERA];
                //配置画面参数，可不配置使用默认参数
                renderView.isRotate = NO;//是否自动旋转
                renderView.autoRotate = NO;//开启自动旋转
                renderView.isMirror = YES;//是否镜像
                renderView.diffDirectionRenderMode = ILIVERENDERMODE_SCALEASPECTFILL;//画面填充方式
                if ([[TCLiveRequestManager getInstance].userID isEqualToString:endoption.identifier]) {
                    renderView.rotateAngle = ILIVEROTATION_90;//画面旋转角度
                }
                [self addLiveRenderView:renderView];
	            }
	                break;	                
	            default:
	                break;
	        }
	    }
	    return YES;
	}
完成上述步骤后，基本上就可以正常进行实时音视频聊天了。
## 消息处理
在上述进入房间时开启im功能，就可以使房间成员之间可以进行消息互动

	
	//房间配置项
	ILiveRoomOption *option = [ILiveRoomOption defaultHostLiveOption];
	//是否支持im
	option.imOption.imSupport = YES;
消息处理只需调用三个API即可完成，步骤如下：

	
1.消息接收

首先需要设置消息事件监听

	[[[ILiveSDK getInstance] getTIMManager] setMessageListener:self];
当有其他成员有消息发送时即可触发接收消息回调方法：
	
	-(void)onNewMessage:(NSArray *)msgs{
    	for (TIMMessage *msg in msgs) {
        	int count = [msg elemCount];
        	for(int i = 0; i < count; i++) {
            	TIMElem *elem = [msg getElem:i];
            	//文本消息类型两种处理方式
           	    if([elem isKindOfClass:[TIMTextElem class]]){
               	    NSString *msgContent = ((TIMTextElem *)elem).text;
               		break;
            	}
            	else if ([elem isKindOfClass:[TIMCustomElem class]]){
                	NSString *msgContent = [[NSString alloc] initWithData:((TIMCustomElem *)elem).data encoding:NSUTF8StringEncoding];
               	    break;
            	}
       	 }
    }
 2.发送消息
 消息发送调用接口：
 
 	- (void)sendGroupMessage:(TIMMessage *)message succ:(TCIVoidBlock)succ failed:(TCIErrorBlock)fail;
 代码示例：
 	
 	- (void)sendMessage:(NSString *)message{
	    //创建消息
	    TIMMessage *msge = [[TIMMessage alloc] init];
	    TIMCustomElem *textElem = [[TIMCustomElem alloc] init];
	    //设置消息内容
	    textElem.data = [message dataUsingEncoding:NSUTF8StringEncoding];
	    NSDictionary *descDic = @{@"nickName":@"昵称"};
	    NSString *desc = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:descDic options:NSJSONReadingAllowFragments error:nil] encoding:NSUTF8StringEncoding];
	    //设置描述信息
	    textElem.desc = desc;
	    //设置消息类型
	    textElem.ext = @"TEXT";
	    
	    [[ILiveRoomManager getInstance] sendGroupMessage:msge succ:^{
	        NSLog(@"send message succ");
	    } failed:^(NSString *module, int errId, NSString *errMsg) {
	        NSLog(@"send message fail");
	    }];
}
 
## 退出房间
退出房间调用接口，一般在挂断或者退出房间界面进行调用，代码示例：
	
	//关闭界面退出房间
	- (void)backBtnClicked:(UIButton *)sender{
	    [self.navigationController popViewControllerAnimated:YES];
	    [[ILiveRoomManager getInstance] quitRoom:^{
	        NSLog(@"-----> quit room succ");
	    } failed:^(NSString *module, int errId, NSString *errMsg) {
	        NSLog(@"-----> quit room fail,%@ %d %@",module, errId, errMsg);
	    }];
	}



# 视频通话界面复用
本文不仅了集成账号登录、音视频通话、文本消息互动的基础能力，还提供一套简单易用的视频通话界面，三行代码即可复用，首先将TCLiveRoomVC文件夹内容拷贝到自己工程，然后初始化TCLiveRoomViewController即可，代码示例：

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
