//
//  TCLiveRoomViewController.h
//  TRTC
//
//  Created by Tencent on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TCLiveVideoLayoutView.h"

@interface TCLiveRoomViewController : UIViewController
@property(nonatomic,strong) TCLiveVideoLayoutView *videoLayoutView;
@property(nonatomic,strong) NSString *roomID;
-(instancetype)initWithRoomID:(NSString *)roomid;
@end
