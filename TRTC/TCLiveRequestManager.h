//
//  TCLiveRequestManager.h
//  TRTC
//
//  Created by Tencent on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^LiveLoginInfoBlock)(int code);

@interface TCLiveRequestManager : NSObject
@property(nonatomic,assign)int sdkAppID;
@property(nonatomic,assign)int accountType;
@property(nonatomic,strong)NSString *userID;
@property(nonatomic,strong)NSString *userSig;
+ (TCLiveRequestManager *)getInstance;
- (void)requestLoginInfo:(LiveLoginInfoBlock)block;
@end
