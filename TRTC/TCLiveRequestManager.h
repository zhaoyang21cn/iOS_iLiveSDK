//
//  TCLiveRequestManager.h
//  TRTC
//
//  Created by ericxwli on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^LiveLoginInfoBlock)(int code);
typedef void (^LiveCreateRoomBlock)(NSDictionary *roomInfo);
typedef void (^LiveEnterRoomBlock)(NSDictionary *roomInfo);
typedef void (^LiveGetRoomListBlock)(NSDictionary *roomListInfo);
typedef void (^LiveQuitRoomBlock)(int code);
typedef void (^LiveHearBeatBlock)(NSDictionary *info);
@interface TCLiveRequestManager : NSObject
@property(nonatomic,assign)int sdkAppID;
@property(nonatomic,assign)int accountType;
@property(nonatomic,strong)NSString *userID;
@property(nonatomic,strong)NSString *userSig;
+ (TCLiveRequestManager *)getInstance;
- (void)requestLoginInfo:(LiveLoginInfoBlock)block;
- (void)reqCreateRoomWithParams:(NSDictionary *)params callBack:(LiveCreateRoomBlock)callback;
- (void)reqEnterRoomWithParams:(NSDictionary *)params callBack:(LiveEnterRoomBlock)callback;
- (void)reqGetRoomListWithParams:(NSDictionary *)params callBack:(LiveGetRoomListBlock)callback;
- (void)reqQuitRoomWithParams:(NSDictionary *)params callBack:(LiveQuitRoomBlock)callback;
- (void)sendHeartBeat:(NSDictionary *)params callBack:(LiveHearBeatBlock)callback;
@end
