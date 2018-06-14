//
//  TCLiveRoomListViewController.m
//  TRTC
//
//  Created by ericxwli on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveRoomListViewController.h"
#import "TCLiveRequestManager.h"
#import <ILiveSDK/ILiveLoginManager.h>
#import <ILiveSDK/ILiveRoomOption.h>
#import <ILiveSDK/ILiveRoomManager.h>
#import <ILiveSDK/ILiveRenderView.h>
#import <ILiveSDK/ILiveFrameDispatcher.h>
#import "TCLiveRoomViewController.h"
#import "TCLiveRoomListViewCell.h"
#import <ILiveLogReport/ILiveLogReport.h>
#import "UIColorEX.h"
#import "RefreshHeader.h"
#import "TCLiveJoinRoomViewController.h"
#import "UIToastView.h"
@interface TCLiveRoomListViewController () <UITableViewDelegate,UITableViewDataSource>
@property(nonatomic,strong)NSMutableArray *roomList;
@property(nonatomic,strong)UITableView *roomListView;
@property(nonatomic,strong)RefreshHeader *refreshHeader;
@end

@implementation TCLiveRoomListViewController

- (void)viewDidLoad {
    
    
    [super viewDidLoad];
    self.title = @"腾讯视频通话";
    [self.navigationController.navigationBar setTranslucent:NO];
    self.view.backgroundColor = [UIColor colorWithRGBHex:0x0B1829];
    // Do any additional setup after loading the view.
    _roomList = [NSMutableArray array];
    
    UIButton *createRoomBtn = [[UIButton alloc] initWithFrame:CGRectMake(20, self.view.frame.size.height - 90 -64, self.view.frame.size.width - 40, 50)];
    createRoomBtn.layer.cornerRadius = 25;
    [createRoomBtn setTitle:@"创建房间" forState:UIControlStateNormal];
    createRoomBtn.backgroundColor = [UIColor colorWithRGBHex:0x1472fc];
    [createRoomBtn addTarget:self action:@selector(createRoomBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [createRoomBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [self.view addSubview:createRoomBtn];
    
    _roomListView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, createRoomBtn.frame.origin.y - 5) style:UITableViewStylePlain];
    _roomListView.dataSource = self;
    _roomListView.delegate = self;
    _roomListView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _roomListView.backgroundColor = [UIColor colorWithRGBHex:0x0B1829];
//    _roomListView.tableHeaderView = [self headViewForRoomList];
    [self.view addSubview:_roomListView];
    
    _refreshHeader = [[RefreshHeader alloc] initWithTarget:_roomListView beginRefreshBlock:^{
        [self refreshRoomList];
    }];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleEnterForeground)  name:UIApplicationWillEnterForegroundNotification object:nil];
}

- (void)createRoomBtnClick:(id)sender{
    TCLiveJoinRoomViewController *vc = [[TCLiveJoinRoomViewController alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
    [ILiveLogReport reportEvent:1203 retTime:0 eventTime:0 roomId:0 extInfo:@"create_room" code:0 msg:nil];
}

-(void)dealloc{
    [[ILiveLoginManager getInstance] iLiveLogout:^{
        NSLog(@"-----> logout  succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"-----> logout  fail,%@ %d %@",module, errId, errMsg);
    }];
}
-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    [self refreshRoomList];
}
- (void)handleEnterForeground{
    [self refreshRoomList];
}
- (void)refreshRoomList{
    [self.roomList removeAllObjects];
    __weak TCLiveRoomListViewController *weaself = self;
    [[TCLiveRequestManager getInstance] reqGetRoomListWithParams:@{@"index":@(0),@"count":@(10)} callBack:^(NSDictionary *roomListInfo) {
        NSArray *rooms = roomListInfo[@"rooms"];
        dispatch_async(dispatch_get_main_queue(), ^{
            __strong typeof (weaself) strongSelf = weaself;
            [strongSelf.roomList addObjectsFromArray:rooms];
            [strongSelf.refreshHeader endRefreshing];
            [strongSelf.roomListView reloadData];
            
        });
    }];
}
#pragma mark - UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.roomList.count;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    TCLiveRoomListViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"RoomListCell"];
    if (!cell) {
        cell = [[TCLiveRoomListViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"RoomListCell"];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    if (self.roomList.count > indexPath.row) {
        NSDictionary  *roomInfo = self.roomList[indexPath.row];
        [cell setRoomInfo:roomInfo];
    }
    return cell;
}
#pragma mark - UITableViewDelegate
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSDictionary *roomInfo = self.roomList[indexPath.row];
    TCLiveRoomViewController *vc = [[TCLiveRoomViewController alloc] init];
    vc.roomInfo = roomInfo;
    vc.enterRoomMode = TCLiveEnterRoomModeJoin;
    if ([TCLiveRequestManager getInstance].userID && roomInfo[@"roomID"]) {
        [[TCLiveRequestManager getInstance] reqEnterRoomWithParams:@{@"userID":[TCLiveRequestManager getInstance].userID,@"nickName":@"1112",@"roomID":roomInfo[@"roomID"]} callBack:^(NSDictionary *roomInfo) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.navigationController pushViewController:vc animated:YES];
            });
            
        }];
    }
    else{
        [[UIToastView getInstance] showToastWithMessage:@"登录信息或者房间信息异常" toastMode:UIToastShowMode_fail];
    }
    [ILiveLogReport reportEvent:1203 retTime:0 eventTime:0 roomId:0 extInfo:@"join_room" code:0 msg:nil];
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    return 80;
}

- (UIView *)headViewForRoomList{
    UIView *headerView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 60)];
    UILabel *label = [[UILabel alloc] initWithFrame:headerView.bounds];
    label.textAlignment = NSTextAlignmentCenter;
    label.textColor = [UIColor whiteColor];
    label.font = [UIFont systemFontOfSize:16];
    label.text = @"腾讯视频通话";
    [headerView addSubview:label];
    return headerView;
}
//- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
//    return 60;
//}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
