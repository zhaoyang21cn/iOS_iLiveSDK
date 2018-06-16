//
//  TCLiveVideoControlBar.m
//  TRTC
//
//  Created by Tencent on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//



#import "TCLiveVideoControlBar.h"
#import <ILiveSDK/ILiveRoomManager.h>
#import <ILiveSDK/ILiveRenderView.h>
#import <ILiveSDK/ILiveFrameDispatcher.h>
#import "TCLiveRequestManager.h"
#import "UIToastView.h"
#import "TCLiveRoomViewController.h"

#define CONTROLBAR_BUTTON_WIDTH 50

@interface TCLiveVideoControlBar ()
@property(nonatomic,strong) UIButton *chatBtn;
@property(nonatomic,strong) UIButton *switchCamera;
@property(nonatomic,strong) UIButton *beautyBtn;
@property(nonatomic,strong) UIButton *voiceBtn;
@property(nonatomic,strong) UIButton *changeRoleBtn;
@property(nonatomic,strong) UIButton *feedBackBtn;
@end

@implementation TCLiveVideoControlBar

-(instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self addSubview:self.chatBtn];
        [self addSubview:self.beautyBtn];
        [self addSubview:self.voiceBtn];
        [self addSubview:self.logBtn];
        [self addSubview:self.switchCamera];
        [self addSubview:self.feedBackBtn];
        [self addSubview:self.changeRoleBtn];
        self.backgroundColor = [UIColor clearColor];
    }
    return self;
}

-(UIButton *)chatBtn{
    if(!_chatBtn){
        _chatBtn = [self createCustomControlBtn:@"聊天" withImage:[UIImage imageNamed:@"chat.png"] selectedImage:nil];
        [_chatBtn addTarget:self action:@selector(chatBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _chatBtn.frame = CGRectMake(self.frame.size.width / 2 - CONTROLBAR_BUTTON_WIDTH * 3.5, 0, CONTROLBAR_BUTTON_WIDTH, self.frame.size.height);
    return _chatBtn;
}
-(UIButton *)switchCamera{
    if (!_switchCamera) {
        _switchCamera = [self createCustomControlBtn:@"翻转" withImage:[UIImage imageNamed:@"camera.png"] selectedImage:[UIImage imageNamed:@"camera-gray.png"]];
        [_switchCamera addTarget:self action:@selector(switchCameraClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _switchCamera.frame = CGRectMake(self.frame.size.width/2 - CONTROLBAR_BUTTON_WIDTH * 2.5,0,CONTROLBAR_BUTTON_WIDTH,self.frame.size.height);
    return _switchCamera;
}

- (UIButton *)beautyBtn{
    if (!_beautyBtn) {
        _beautyBtn = [self createCustomControlBtn:@"美颜" withImage:[UIImage imageNamed:@"beauty.png"] selectedImage:[UIImage imageNamed:@"beauty-dis.png"]];
        [_beautyBtn addTarget:self action:@selector(beautyBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _beautyBtn.frame = CGRectMake(self.frame.size.width/2 - CONTROLBAR_BUTTON_WIDTH *1.5, 0, CONTROLBAR_BUTTON_WIDTH, self.frame.size.height);
    return _beautyBtn;
}

- (UIButton *)voiceBtn{
    if (!_voiceBtn) {
        _voiceBtn = [self createCustomControlBtn:@"声音" withImage:[UIImage imageNamed:@"mic.png"] selectedImage:[UIImage imageNamed:@"mic-dis.png"]];
        [_voiceBtn addTarget:self action:@selector(voiceBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _voiceBtn.frame = CGRectMake(self.frame.size.width/2 - CONTROLBAR_BUTTON_WIDTH * 0.5, 0, CONTROLBAR_BUTTON_WIDTH, self.frame.size.height);
    return _voiceBtn;
}

-(UIButton *)changeRoleBtn{
    if (!_changeRoleBtn) {
        _changeRoleBtn = [self createCustomControlBtn:@"配置" withImage:[UIImage imageNamed:@"role.png"] selectedImage:[UIImage imageNamed:@"role.png"]];
        [_changeRoleBtn addTarget:self action:@selector(changeRoleBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _changeRoleBtn.frame = CGRectMake(self.frame.size.width/2 + CONTROLBAR_BUTTON_WIDTH * 0.5, 0, CONTROLBAR_BUTTON_WIDTH, self.frame.size.height);
    return _changeRoleBtn;
}

- (UIButton *)feedBackBtn{
    if (!_feedBackBtn) {
        _feedBackBtn = [self createCustomControlBtn:@"反馈" withImage:[UIImage imageNamed:@"feedback.png"] selectedImage:[UIImage imageNamed:@"feedback.png"]];
        [_feedBackBtn addTarget:self action:@selector(feedBackBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _feedBackBtn.frame = CGRectMake(self.frame.size.width/2 + CONTROLBAR_BUTTON_WIDTH * 1.5, 0, CONTROLBAR_BUTTON_WIDTH, self.frame.size.height);
    return _feedBackBtn;
}

- (UIButton *)logBtn{
    if (!_logBtn) {
        _logBtn = [self createCustomControlBtn:@"信息" withImage:[UIImage imageNamed:@"log.png"] selectedImage:[UIImage imageNamed:@"log2.png"]];
        [_logBtn addTarget:self action:@selector(logBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    _logBtn.frame = CGRectMake(self.frame.size.width/2 +  CONTROLBAR_BUTTON_WIDTH * 2.5, 0, CONTROLBAR_BUTTON_WIDTH, self.frame.size.height);
    return _logBtn;
}

- (UIButton *)createCustomControlBtn:(NSString *)wording withImage:(UIImage *)image selectedImage:(UIImage *)highlightImage{
    UIButton *customButton = [[UIButton alloc] initWithFrame:CGRectZero];
    [customButton setTitle:wording forState:UIControlStateNormal];
    customButton.titleLabel.font = [UIFont systemFontOfSize:11];
    [customButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [customButton setImage:image forState:UIControlStateNormal];
    [customButton setImage:highlightImage forState:UIControlStateSelected];
    CGFloat imageHeight = image.size.height;
    CGFloat imageWidth = image.size.width;
    [customButton setTitleEdgeInsets:UIEdgeInsetsMake(15, -60, -15, 0)];
    [customButton setImageEdgeInsets:UIEdgeInsetsMake(10, 10, self.frame.size.height - (CONTROLBAR_BUTTON_WIDTH - 20)/imageWidth *imageHeight -10, 10)];
    return customButton;
}
//聊天按钮
- (void)chatBtnClick:(UIButton *)sender{
    if (!sender.selected) {
        sender.selected = YES;
    }
    else{
        sender.selected = NO;
    }
    if ([_delegate respondsToSelector:@selector(chatBtnClick:)]) {
        [_delegate chatBtnClick:sender];
    }
}
//切换摄像头
- (void)switchCameraClick:(UIButton *)sender{
    ILiveFrameDispatcher *frameDispatcher = [[ILiveRoomManager getInstance] getFrameDispatcher];
    ILiveRenderView *renderView = [frameDispatcher getRenderView:[TCLiveRequestManager getInstance].userID srcType:QAVVIDEO_SRC_TYPE_CAMERA];
    if (!sender.selected) {
        sender.selected = YES;
        renderView.isMirror = NO;
    }
    else{
        sender.selected = NO;
        renderView.isMirror = YES;
    }
    [[ILiveRoomManager getInstance] switchCamera:^{
        NSLog(@"switch camera succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"switch camera fail");
        [[UIToastView getInstance] showToastWithMessage:@"切换摄像头失败" toastMode:UIToastShowMode_fail];
    }];
    if ([_delegate respondsToSelector:@selector(switchCameraClick:)]) {
        [_delegate switchCameraBtnClick:sender];
    }
}
//美颜
- (void)beautyBtnClick:(UIButton *)sender{
    if (!sender.selected) {
        sender.selected = YES;
    }
    else{
        sender.selected = NO;
    }
    if ([_delegate respondsToSelector:@selector(beautyBtnClick:)]) {
        [_delegate beautyBtnClick:sender];
    }
}
- (void)enableBeauty:(BOOL)enable{
    if (enable) {
        _beautyBtn.selected = YES;
    }
    else{
        _beautyBtn.selected = NO;
    }
    if ([_delegate respondsToSelector:@selector(beautyBtnClick:)]) {
        [_delegate beautyBtnClick:_beautyBtn];
    }
}
//语音开关
- (void)voiceBtnClick:(UIButton *)sender{
    if (!sender.selected) {
        sender.selected = YES;
    }
    else{
        sender.selected = NO;
    }
    [[ILiveRoomManager getInstance] enableMic:!sender.selected succ:^{
        NSLog(@"enable mic succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"enable mic fail");
        [[UIToastView getInstance] showToastWithMessage:@"关麦失败" toastMode:UIToastShowMode_fail];
    }];
    if ([_delegate respondsToSelector:@selector(voiceBtnClick:)]) {
        [_delegate voiceBtnClick:sender];
    }
    
}
//配置角色
- (void)changeRoleBtnClick:(UIButton *)sender{
    if (!sender.selected) {
        sender.selected = YES;
    }
    else{
        sender.selected = NO;
    }
    if ([_delegate respondsToSelector:@selector(changeRoleBtnClick:)]) {
        [_delegate changeRoleBtnClick:sender];
    }
    if ([_delegate isKindOfClass:[UIViewController class]]) {
        [self showChangeRoleMenuOnVC:(UIViewController *)_delegate];
    }
}
//日志
- (void)logBtnClick:(UIButton *)sender{
    if (!sender.selected) {
        sender.selected = YES;
    }
    else{
        sender.selected = NO;
    }
    if ([_delegate respondsToSelector:@selector(logBtnClick:)]) {
        [_delegate logBtnClick:sender];
    }
}
//问题反馈
- (void)feedBackBtnClick:(UIButton *)sender{
    if (!sender.selected) {
        sender.selected = YES;
    }
    else{
        sender.selected = NO;
    }
    if ([_delegate respondsToSelector:@selector(feedBackBtnClick:)]) {
        [_delegate feedBackBtnClick:sender];
    }
    if ([_delegate isKindOfClass:[UIViewController class]]) {
        [self showFeedBackMenuOnVC:(UIViewController *)_delegate];
    }
}
//log展示开关
- (void)enableLog:(BOOL)endable{
    self.logBtn.selected = endable;
    if ([_delegate respondsToSelector:@selector(logBtnClick:)]) {
        [_delegate logBtnClick:self.logBtn];
    }
}
//弹出反馈菜单
-(void)showFeedBackMenuOnVC:(UIViewController *)vc{
    UIAlertController *actionController = [UIAlertController alertControllerWithTitle:@"请选择反馈类型" message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    UIAlertAction *catonSheet = [UIAlertAction actionWithTitle:@"视频卡顿" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self feedBackWithExt:@"视频卡顿"];
    }];
    UIAlertAction *blurActionSheet = [UIAlertAction actionWithTitle:@"画面不清晰" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self feedBackWithExt:@"画面不清晰"];
    }];
    UIAlertAction *voiceActionSheet = [UIAlertAction actionWithTitle:@"音质较差" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self feedBackWithExt:@"音质较差"];
    }];
    UIAlertAction *jubaoActionSheet = [UIAlertAction actionWithTitle:@"举报" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self showInputFeedBackAlertOnVC:vc];
    }];
    UIAlertAction *otherActionSheet = [UIAlertAction actionWithTitle:@"其他" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self showInputFeedBackAlertOnVC:vc];
    }];
    UIAlertAction *cancleActionSheet = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
    [actionController addAction:catonSheet];
    [actionController addAction:blurActionSheet];
    [actionController addAction:voiceActionSheet];
    [actionController addAction:jubaoActionSheet];
    [actionController addAction:otherActionSheet];
    [actionController addAction:cancleActionSheet];
    [vc presentViewController:actionController animated:YES completion:nil];
}
//反馈上报
- (void)feedBackWithExt:(NSString *)ext{
    [[ILiveSDK getInstance] uploadLog:ext logDayOffset:0 uploadResult:^(int retCode, NSString *retMsg, NSString *logKey) {
        if (retCode == 0) {
            NSLog(@"上报成功");
            [[UIToastView getInstance] showToastWithMessage:@"非常感谢您的反馈" toastMode:UIToastShowMode_Succ];
        }
        else{
            NSLog(@"上报失败");
            [[UIToastView getInstance] showToastWithMessage:[NSString stringWithFormat:@"反馈失败%@",retMsg] toastMode:UIToastShowMode_fail];
        }
    }];
}
//其他反馈输入
- (void)showInputFeedBackAlertOnVC:(UIViewController *)vc{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"请输入具体问题描述" message:nil preferredStyle:UIAlertControllerStyleAlert];
    [alertController addTextFieldWithConfigurationHandler:nil];
    [alertController addAction:[UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleDefault handler:nil]];
    [alertController addAction:[UIAlertAction actionWithTitle:@"提交" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self feedBackWithExt:alertController.textFields.firstObject.text];
    }]];
    [vc presentViewController:alertController animated:YES completion:nil];
}
//弹出配置菜单
-(void)showChangeRoleMenuOnVC:(UIViewController *)vc{
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"请选择配置分辨率" message:nil preferredStyle:UIAlertControllerStyleAlert];
    [alertController addAction:[UIAlertAction actionWithTitle:@"1280x720 1000-1800kbps 20fps" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[ILiveRoomManager getInstance] changeRole:@"ed1280" succ:^{
            NSLog(@"更换ed1280 succ");
            [self enableLog:YES];
        } failed:^(NSString *module, int errId, NSString *errMsg) {
            NSLog(@"更换ed1280 fail");
        }];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:@"960x540 500-800kbps 15fps" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[ILiveRoomManager getInstance] changeRole:@"ed960" succ:^{
            NSLog(@"更换ed960 succ");
            [self enableLog:YES];
        } failed:^(NSString *module, int errId, NSString *errMsg) {
            NSLog(@"更换ed960 fail");
        }];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:@"640x480 400-800kbps 15fps" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[ILiveRoomManager getInstance] changeRole:@"ed640" succ:^{
            NSLog(@"更换ed640 succ");
            [self enableLog:YES];
        } failed:^(NSString *module, int errId, NSString *errMsg) {
            NSLog(@"更换ed640 fail");
        }];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:@"480x360 300-600kbps 15fps" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[ILiveRoomManager getInstance] changeRole:@"ed480" succ:^{
            NSLog(@"更换ed480 succ");
            [self enableLog:YES];
        } failed:^(NSString *module, int errId, NSString *errMsg) {
            NSLog(@"更换ed480 fail");
        }];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:@"320x240 200-400kbps 15fps" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[ILiveRoomManager getInstance] changeRole:@"ed320" succ:^{
            NSLog(@"更换ed320 succ");
            [self enableLog:YES];
        } failed:^(NSString *module, int errId, NSString *errMsg) {
            NSLog(@"更换ed320 fail");
        }];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil]];
    [vc presentViewController:alertController animated:YES completion:nil];
}
@end
