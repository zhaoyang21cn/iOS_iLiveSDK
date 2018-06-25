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
//传入roomid(房间号)进入指定房间 设置role配置画面参数
-(instancetype)initWithRoomID:(NSString *)roomid role:(NSString *)role;
@end
