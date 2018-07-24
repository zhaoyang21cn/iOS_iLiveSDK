//
//  TCLiveRoomViewController.m
//  TRTC
//
//  Created by Tencent on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveRoomViewController.h"
#import <ILiveSDK/ILiveRoomOption.h>
#import <ILiveSDK/ILiveRoomManager.h>
#import <ILiveSDK/ILiveRenderView.h>
#import <ILiveSDK/ILiveFrameDispatcher.h>
#import <TILLiveSDK/TILLiveManager.h>
#import <QAVSDK/QAVCommon.h>
#import <ImSDK/ImSDK.h>
#import "TCLiveRequestManager.h"
#import "TCLiveVideoControlBar.h"
#import "TCLiveChatTableView.h"
#import "UIToastView.h"

#define LIVE_VIEW_HEIGHT  370
#define LIVE_CONTROL_BAR_HEIGHT 70
#define LIVE_INPUTTEXTFIELD_HEIGHT 40

@interface TCLiveRoomViewController ()<UITextFieldDelegate,TCLiveVideoControlBarDelegate>
@property(nonatomic,strong) TCLiveVideoLayoutView *videoLayoutView;
@property(nonatomic,strong) TCLiveVideoControlBar *controlBar;
@property(nonatomic,strong) TCLiveChatTableView *chatTableView;
@property(nonatomic,strong) UITextField *inputTextField;
@property(nonatomic,strong) NSString *roomID;
@property(nonatomic,strong) UIImageView *bgImageView;
@property(nonatomic,strong) NSTimer *logTimer;
@property(nonatomic,strong) NSTimer *heartBeatTimer;
@property(nonatomic,assign) CGRect origInputTextFieldFrame;
@property(nonatomic,assign) CGRect origChatTableViewFrame;
@property(nonatomic,strong) NSString *role;
@end

@implementation TCLiveRoomViewController

-(instancetype)initWithRoomID:(NSString *)roomid role:(NSString *)role{
    if (self = [super init]) {
        self.roomID = roomid;
        self.role = role;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = self.roomID;
    [self.navigationController.navigationBar setTitleTextAttributes:@{NSForegroundColorAttributeName:[UIColor whiteColor]}];
    
    [self enterRoom];
    
    [self customLeftButton];
    
    [self.view addSubview:self.bgImageView];

    [_bgImageView addSubview:self.videoLayoutView];
    
    [_bgImageView addSubview:self.chatTableView];
    
    [_bgImageView addSubview:self.controlBar];
    
    [_bgImageView addSubview:self.inputTextField];

    //监听键盘变化
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
}
-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    //导航栏透明化
    [self setNavigationBarTransparent];
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
}

- (void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    [self.navigationController.navigationBar setTranslucent:NO];
    [UIApplication sharedApplication].statusBarStyle =  UIStatusBarStyleDefault;
}
- (void)setNavigationBarTransparent{
    [self.navigationController.navigationBar setTranslucent:YES];
    [self.navigationController.navigationBar setBackgroundImage:[[UIImage alloc]init] forBarMetrics:UIBarMetricsDefault];
    [self.navigationController.navigationBar setShadowImage:[[UIImage alloc]init]];
    [self.navigationController.navigationBar setBackgroundColor:[UIColor clearColor]];
}

- (void)enterRoom{
    [[TCLiveRequestManager getInstance] reqGetAuthBufferInfoWithParams:@{@"roomID":self.roomID,@"userID":[TCLiveRequestManager getInstance].userID} block:^(NSDictionary *info) {
        ILiveRoomOption *option = [ILiveRoomOption defaultHostLiveOption];
        option.imOption.imSupport = YES;
        option.memberStatusListener = self.videoLayoutView;
        option.roomDisconnectListener = self;
        option.controlRole = self.role;
        option.avOption.privateMapKey = [info[@"privateMapKey"] dataUsingEncoding:NSUTF8StringEncoding];
        
        [[ILiveRoomManager getInstance] createRoom:[self.roomID intValue] option:option succ:^{
            NSLog(@"-----> create room succ");
            [[UIToastView getInstance] showToastWithMessage:@"创建房间成功" toastMode:UIToastShowMode_Succ];
            [self.controlBar enableBeauty:YES];//进入房间默认开启美颜
        } failed:^(NSString *module, int errId, NSString *errMsg) {
            if(errId == 10021){
                //表示房间已存在直接加入房间
                [[ILiveRoomManager getInstance] joinRoom:[self.roomID intValue] option:option succ:^{
                    NSLog(@"-----> join room succ");
                    [[UIToastView getInstance] showToastWithMessage:@"加入房间成功" toastMode:UIToastShowMode_Succ];
                    [self.controlBar enableBeauty:YES];//进入房间默认开启美颜
                } failed:^(NSString *module, int errId, NSString *errMsg) {
                    NSLog(@"-----> join room fail,%@ %d %@",module, errId, errMsg);
                    [[UIToastView getInstance] showToastWithMessage:errMsg toastMode:UIToastShowMode_fail];
                }];
            }
            else{
                NSLog(@"-----> create room fail,%@ %d %@",module, errId, errMsg);
                [[UIToastView getInstance] showToastWithMessage:errMsg toastMode:UIToastShowMode_fail];
            }
        }];
    }];
    
}

- (void)customLeftButton{
    UIButton *backBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    backBtn.frame = CGRectMake(0, 0, 44, 44);
    [backBtn setImage:[UIImage imageNamed:@"ui_title_arrow_left.png"] forState:UIControlStateNormal];
    [backBtn addTarget:self action:@selector(backBtnClicked:) forControlEvents:UIControlEventTouchUpInside];
    backBtn.frame = CGRectMake(0, 0, 44, 44);
    [backBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    backBtn.imageEdgeInsets = UIEdgeInsetsMake(10, 0, 10, 20);
    UIBarButtonItem *item = [[UIBarButtonItem alloc]initWithCustomView:backBtn];
    self.navigationItem.leftBarButtonItem = item;
}

- (UIImageView *)bgImageView{
    if (!_bgImageView) {
        _bgImageView = [[UIImageView alloc] initWithFrame:self.view.bounds];
        [_bgImageView setImage:[UIImage imageNamed:@"bg.png"]];
        _bgImageView.userInteractionEnabled = YES;
    }
    return _bgImageView;
}

//视频区域
-(TCLiveVideoLayoutView *)videoLayoutView{
    if (!_videoLayoutView) {
        _videoLayoutView = [[TCLiveVideoLayoutView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    }
    return _videoLayoutView;
}
//控制bar
- (TCLiveVideoControlBar *)controlBar{
    if (!_controlBar) {
        _controlBar = [[TCLiveVideoControlBar alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - LIVE_CONTROL_BAR_HEIGHT, self.view.frame.size.width, LIVE_CONTROL_BAR_HEIGHT)];
        _controlBar.delegate = self;
    }
    return _controlBar;
}
//消息列表
- (UITableView *)chatTableView{
    if (!_chatTableView) {
        _chatTableView = [[TCLiveChatTableView alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - LIVE_CONTROL_BAR_HEIGHT - 400, 250, 400) style:UITableViewStylePlain];
        _chatTableView.backgroundColor = [UIColor clearColor];
        self.origChatTableViewFrame = self.chatTableView.frame;
    }
    return _chatTableView;
}
//输入框
- (UITextField *)inputTextField{
    if (!_inputTextField) {
        _inputTextField = [[UITextField alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height , self.view.frame.size.width, LIVE_INPUTTEXTFIELD_HEIGHT)];
        _inputTextField.delegate = self;
        _inputTextField.backgroundColor= [[UIColor whiteColor] colorWithAlphaComponent:0.9];
        _inputTextField.placeholder = @"请输入内容";
        _inputTextField.returnKeyType = UIReturnKeySend;
        self.origInputTextFieldFrame = self.inputTextField.frame;
    }
    return _inputTextField;
}
//关闭界面退出房间
- (void)backBtnClicked:(UIButton *)sender{
    [self.navigationController popViewControllerAnimated:YES];
    [[ILiveRoomManager getInstance] quitRoom:^{
        NSLog(@"-----> quit room succ");
        [[UIToastView getInstance] showToastWithMessage:@"退出房间成功" toastMode:UIToastShowMode_Succ];
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> quit room fail,%@ %d %@",module, errId, errMsg);
        [[UIToastView getInstance] showToastWithMessage:@"退出房间失败" toastMode:UIToastShowMode_fail];
    }];
    [_logTimer invalidate];
    _logTimer = nil;
    
}
#pragma mark - ILiveRoomDisconnectListener
- (BOOL)onRoomDisconnect:(int)reason;{
    [self backBtnClicked:nil];
    return YES;
}
#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField;{
    [textField resignFirstResponder];
    NSString *text = textField.text;
    if (text.length > 0) {
        [self.chatTableView sendMessage:text];
        textField.text = nil;
    }
    else{
        return NO;
    }
    return YES;
}
#pragma mark - TCLiveVideoControlBarDelegate
-(void)logBtnClick:(UIButton *)sender{
    if (sender.selected) {
        if (!_logTimer) {
            [self logUpdate];
            _logTimer = [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(logUpdate) userInfo:nil repeats:YES];
        }
    }
    else{
        [_logTimer invalidate];
        _logTimer = nil;
        [_videoLayoutView closeLogView];
    }
}
- (void)chatBtnClick:(UIButton *)sender{
    [self.inputTextField becomeFirstResponder];
}
- (void)logUpdate{
    QAVContext *avContext = [[ILiveSDK getInstance] getAVContext];
    NSString *qualityStr = [avContext.room getQualityTips];
    [_videoLayoutView showLogView:qualityStr];
}

- (void)beautyBtnClick:(UIButton *)sender{
    if (sender.selected) {
        [self.videoLayoutView setBeautyLevel:9];
    }
    else{
        [self.videoLayoutView setBeautyLevel:0];
    }
}
#pragma mark -  键盘事件
- (void)keybaordAnimationWithDuration:(CGFloat)duration keyboardOriginY:(CGFloat)keyboardOriginY{
    
    __block TCLiveRoomViewController *blockSelf = self;
    //作为视图的键盘，弹出动画也是UIViewAnimationOptionCurveEaseIn的方式
    [UIView animateWithDuration:duration delay:0 options:UIViewAnimationOptionCurveEaseIn animations:^{
        //text field
        if(keyboardOriginY == blockSelf.view.frame.size.height){
            blockSelf.inputTextField.frame = blockSelf.origInputTextFieldFrame;
            blockSelf.chatTableView.frame =  blockSelf.origChatTableViewFrame;
        }
        else{
            blockSelf.inputTextField.frame = CGRectMake(blockSelf.inputTextField.frame.origin.x,keyboardOriginY - blockSelf.inputTextField.frame.size.height, blockSelf.inputTextField.frame.size.width, blockSelf.inputTextField.frame.size.height);
            blockSelf.chatTableView.frame =  CGRectMake(blockSelf.chatTableView.frame.origin.x,keyboardOriginY - blockSelf.chatTableView.frame.size.height - LIVE_INPUTTEXTFIELD_HEIGHT, blockSelf.chatTableView.frame.size.width, blockSelf.chatTableView.frame.size.height);
        }
        
    } completion:nil];
}
- (void)keyboardWillChangeFrame:(NSNotification *)notify{
    NSDictionary    * info = notify.userInfo;
    //动画时间
    CGFloat animationDuration = [info[UIKeyboardAnimationDurationUserInfoKey] floatValue];
    //键盘目标位置
    CGRect  keyboardAimFrame = [info[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    if ([self.inputTextField isFirstResponder]) {
        [self keybaordAnimationWithDuration:animationDuration keyboardOriginY:keyboardAimFrame.origin.y];
    }
}
@end
