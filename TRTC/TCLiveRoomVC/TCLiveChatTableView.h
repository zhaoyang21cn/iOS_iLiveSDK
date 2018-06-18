//
//  TCLiveChatTableView.h
//  TRTC
//
//  Created by Tencent on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <ImSDK/ImSDK.h>

@interface TCLiveChatTableView : UITableView
//添加信息到聊天列表
- (void)addChatMessage:(NSArray *)msgList withContentColor:(UIColor *)contentColor nickColor:(UIColor *)nickColor;
//发送信息
- (void)sendMessage:(NSString *)message;
@end
