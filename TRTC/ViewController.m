//
//  ViewController.m
//  TRTC
//
//  Created by ericxwli on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "ViewController.h"
#import <ILiveSDK/ILiveSDK.h>
#import <ILiveSDK/ILiveLoginManager.h>
#import "TCLiveRequestManager.h"
#import "TCLiveRoomListViewController.h"

@interface ViewController () 

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    NSString *ver = [[ILiveSDK getInstance] getVersion];
    NSLog(@"ILiveSDK Version is%@",ver);

    UIButton *loginBtn = [[UIButton alloc] initWithFrame:CGRectMake(100, 100, 200, 200)];
    loginBtn.titleLabel.font = [UIFont systemFontOfSize:14];
    [loginBtn setTitle:@"登录" forState:UIControlStateNormal];
    [loginBtn addTarget:self action:@selector(loginBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [loginBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.view addSubview:loginBtn];

}

- (void)loginBtnClick:(id)sender{
    [[ILiveLoginManager getInstance] iLiveLogin:[TCLiveRequestManager getInstance].userID sig:[TCLiveRequestManager getInstance].userSig succ:^{
        TCLiveRoomListViewController *vc = [TCLiveRoomListViewController new];
        [self.navigationController pushViewController:vc animated:YES];
        NSLog(@"-----> login  succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> login  fail,%@ %d %@",module, errId, errMsg);
    }];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
