//
//  TCLiveRoomViewController.h
//  TRTC
//
//  Created by ericxwli on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TCLiveVideoLayoutView.h"
typedef enum {
    TCLiveEnterRoomModeCreate = 0,
    TCLiveEnterRoomModeJoin,
}TCLiveEnterRoomMode;

@interface TCLiveRoomViewController : UIViewController
@property(nonatomic,strong) TCLiveVideoLayoutView *videoLayoutView;
@property(nonatomic,assign)TCLiveEnterRoomMode enterRoomMode;
@property(nonatomic,strong)NSDictionary *roomInfo;
@end
