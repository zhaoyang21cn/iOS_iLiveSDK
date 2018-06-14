//
//  TCLiveJoinRoomViewController.m
//  TRTC
//
//  Created by ericxwli on 2018/6/8.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveJoinRoomViewController.h"
#import "UIColorEX.h"
#import "TCLiveRoomViewController.h"
#import <ILiveSDK/ILiveRoomOption.h>
#import "TCLiveRequestManager.h"
#import <ILiveSDK/ILiveRoomManager.h>
#import "UIToastView.h"
@interface TCLiveJoinRoomViewController () <UITextFieldDelegate>
@property(nonatomic,strong)UITextField *inputTextField;
@property(nonatomic,strong)UIButton *joinRoomBtn;
@property(nonatomic,strong) UIView *botoomLine;
@property(nonatomic,strong) UIImageView *bgImageView;
@end

@implementation TCLiveJoinRoomViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _bgImageView = [[UIImageView alloc] initWithFrame:self.view.bounds];
    [_bgImageView setImage:[UIImage imageNamed:@"bg.png"]];
    _bgImageView.userInteractionEnabled = YES;
    [self.view addSubview:_bgImageView];

    [self.view addSubview:self.joinRoomBtn];
    
    [self.view addSubview:self.inputTextField];
}


- (UIButton *)joinRoomBtn{
    if (!_joinRoomBtn) {
        _joinRoomBtn = [[UIButton alloc] initWithFrame:CGRectMake(20, self.view.frame.size.height - 90 -64, self.view.frame.size.width - 40, 50)];
        _joinRoomBtn.layer.cornerRadius = 25;
        [_joinRoomBtn setTitle:@"确定创建并进入房间" forState:UIControlStateNormal];
        _joinRoomBtn.backgroundColor = [UIColor colorWithRGBHex:0x1472fc];
        [_joinRoomBtn addTarget:self action:@selector(joinRoomBtnClick:) forControlEvents:UIControlEventTouchUpInside];
        [_joinRoomBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    }
    return _joinRoomBtn;
}
- (UITextField *)inputTextField{
    if (!_inputTextField) {
        _inputTextField = [[UITextField alloc] initWithFrame:CGRectZero];
        _inputTextField.delegate = self;
        _inputTextField.backgroundColor= [UIColor clearColor];
        NSMutableAttributedString *str = [[NSMutableAttributedString alloc] initWithString:@"请输入房间名称" attributes:@{NSForegroundColorAttributeName:[UIColor grayColor]}];
        _inputTextField.attributedPlaceholder = str;
        _inputTextField.textColor = [UIColor grayColor];
        _inputTextField.returnKeyType = UIReturnKeyDone;
        
        _botoomLine = [[UIView alloc] initWithFrame:CGRectZero];
        _botoomLine.backgroundColor = [UIColor colorWithRGBHex:0x1472fc];
        [_inputTextField addSubview:_botoomLine];
    }
    _inputTextField.frame = CGRectMake(5, 20 , self.view.frame.size.width-10, 40);
    _botoomLine.frame = CGRectMake(0, 40-1, _inputTextField.frame.size.width, 1);
    return _inputTextField;
}

- (void)joinRoomBtnClick:(UIButton *)sender{
    if (self.inputTextField.text.length > 0) {
        TCLiveRoomViewController *vc = [TCLiveRoomViewController new];
        vc.enterRoomMode = TCLiveEnterRoomModeCreate;
        [[TCLiveRequestManager getInstance] reqCreateRoomWithParams:@{@"userID":[TCLiveRequestManager getInstance].userID,@"nickName":[TCLiveRequestManager getInstance].userID,@"roomInfo":self.inputTextField.text} callBack:^(NSDictionary *roomInfo) {
            dispatch_async(dispatch_get_main_queue(), ^{
                vc.roomInfo = roomInfo;
                [self.navigationController pushViewController:vc animated:YES];
                NSMutableArray *viewControllers = [NSMutableArray arrayWithArray:[self.navigationController viewControllers]];
                [viewControllers removeObject:self];
                self.navigationController.viewControllers = viewControllers;
            });
        }];
    }
    else{
        [[UIToastView getInstance] showToastWithMessage:@"房间号不能为空" toastMode:UIToastShowMode_fail];
    }
    
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField{
    [textField resignFirstResponder];
    return YES;
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
