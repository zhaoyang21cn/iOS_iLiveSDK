//
//  ViewController.m
//  TRTC
//
//  Created by Tencent on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveJoinRoomViewController.h"
#import "UIColorEX.h"
#import "TCLiveRoomViewController.h"
#import "UIToastView.h"
#import "TCLiveConfigDefine.h"

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
    self.title = @"腾讯视频通话";

    [self.navigationController.navigationBar setTranslucent:NO];
    
    [self.view addSubview:self.bgImageView];
    
    [self.view addSubview:self.inputTextField];
    
    [self.view addSubview:self.joinRoomBtn];
    
    
}
-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self.navigationController.navigationBar setTitleTextAttributes:@{NSForegroundColorAttributeName:[UIColor blackColor]}];
}
-(UIImageView *)bgImageView{
    if (!_bgImageView) {
        _bgImageView = [[UIImageView alloc] initWithFrame:self.view.bounds];
        [_bgImageView setImage:[UIImage imageNamed:@"bg.png"]];
        _bgImageView.userInteractionEnabled = YES;
    }
    return _bgImageView;
}

- (UIButton *)joinRoomBtn{
    if (!_joinRoomBtn) {
        _joinRoomBtn = [[UIButton alloc] initWithFrame:CGRectMake(20, _inputTextField.frame.size.height + _inputTextField.frame.origin.y + 50, self.view.frame.size.width - 40, 50)];
        _joinRoomBtn.layer.cornerRadius = 25;
        [_joinRoomBtn setTitle:@"加入房间" forState:UIControlStateNormal];
        _joinRoomBtn.backgroundColor = [UIColor colorWithRGBHex:0x1472fc];
        [_joinRoomBtn addTarget:self action:@selector(joinRoomBtnClick:) forControlEvents:UIControlEventTouchUpInside];
        [_joinRoomBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    }
    return _joinRoomBtn;
}
- (UITextField *)inputTextField{
    if (!_inputTextField) {
        _inputTextField = [[UITextField alloc] initWithFrame:CGRectMake(5, 20 , self.view.frame.size.width-10, 40)];
        _inputTextField.delegate = self;
        _inputTextField.backgroundColor= [UIColor clearColor];
        NSMutableAttributedString *str = [[NSMutableAttributedString alloc] initWithString:@"请输入房间号码" attributes:@{NSForegroundColorAttributeName:[UIColor grayColor]}];
        _inputTextField.attributedPlaceholder = str;
        _inputTextField.textColor = [UIColor grayColor];
        _inputTextField.returnKeyType = UIReturnKeyDone;
        _inputTextField.keyboardType = UIKeyboardTypeNumberPad;
        
        _botoomLine = [[UIView alloc] initWithFrame:CGRectMake(0, 40-1, _inputTextField.frame.size.width, 1)];
        _botoomLine.backgroundColor = [UIColor colorWithRGBHex:0x1472fc];
        [_inputTextField addSubview:_botoomLine];
    }
    return _inputTextField;
}

- (void)joinRoomBtnClick:(UIButton *)sender{
    if (self.inputTextField.text.length > 0) {
        TCLiveRoomViewController *vc = [[TCLiveRoomViewController alloc] initWithRoomID:self.inputTextField.text role:Default_Role];
        [self.navigationController pushViewController:vc animated:YES];
    }
    else{
        [[UIToastView getInstance] showToastWithMessage:@"请输入房间号" toastMode:UIToastShowMode_fail];
    }
    
}
#pragma mark - UITextFieldDelegate
- (BOOL)textFieldShouldReturn:(UITextField *)textField{
    [textField resignFirstResponder];
    return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSCharacterSet *cs = [[NSCharacterSet characterSetWithCharactersInString:@"0123456789"] invertedSet];
    NSString *filtered = [[string componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
    return [string isEqualToString:filtered];
}
@end
