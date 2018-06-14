//
//  TCLiveRequestManager.m
//  TRTC
//
//  Created by ericxwli on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveRequestManager.h"
#import "UIToastView.h"
@implementation TCLiveRequestManager
+ (TCLiveRequestManager *)getInstance{
    static TCLiveRequestManager *singleTon = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        singleTon = [TCLiveRequestManager new];
    });
    return singleTon;
}

- (void)requestLoginInfo:(LiveLoginInfoBlock)block{
    NSString *user = [[NSUserDefaults standardUserDefaults] objectForKey:@"TCLIVE_USER"];
    if (user.length == 0) {
        user = @"";
    }
    NSDictionary *params = @{@"userID":user};
    NSMutableURLRequest *request = [self getSendPostRequest:@"https://xzb.qcloud.com/webrtc/weapp/webrtc_room/get_login_info" body:params];
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    [sessionConfig setTimeoutIntervalForRequest:30];
    
    __weak TCLiveRequestManager *weakself = self;
    NSURLSession *session = [NSURLSession sessionWithConfiguration:sessionConfig];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error|| data == nil) {
            block(-1);
            [[UIToastView getInstance] showToastWithMessage:@"登录请求失败" toastMode:UIToastShowMode_fail];
        }
        else{
            NSDictionary *info = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
            weakself.sdkAppID = [info[@"sdkAppID"] intValue];
            weakself.accountType = [info[@"accountType"]intValue];
            weakself.userID = info[@"userID"];
            weakself.userSig = info[@"userSig"];
            [[NSUserDefaults standardUserDefaults] setObject:info[@"userID"] forKey:@"TCLIVE_USER"];
            block(0);
        }
    }];
    [task resume];
}

- (void)reqCreateRoomWithParams:(NSDictionary *)params callBack:(LiveCreateRoomBlock)callback{
//    NSDictionary *reqParams = @{@"userID":self.userID,@"nickName":@"123",@"roomInfo":@"正在测试勿进"};
    NSMutableURLRequest *request = [self getSendPostRequest:@"https://xzb.qcloud.com/webrtc/weapp/webrtc_room/create_room" body:params];
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    [sessionConfig setTimeoutIntervalForRequest:30];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:sessionConfig];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error|| data == nil) {
            [[UIToastView getInstance] showToastWithMessage:@"创建房间请求失败" toastMode:UIToastShowMode_fail];
        }
        else{
            NSDictionary *info = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
            callback(info);
        }
    }];
    [task resume];
}

- (void)reqEnterRoomWithParams:(NSDictionary *)params callBack:(LiveEnterRoomBlock)callback{
    NSMutableURLRequest *request = [self getSendPostRequest:@"https://xzb.qcloud.com/webrtc/weapp/webrtc_room/enter_room" body:params];
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    [sessionConfig setTimeoutIntervalForRequest:30];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:sessionConfig];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error|| data == nil) {
            [[UIToastView getInstance] showToastWithMessage:@"进入房间请求失败" toastMode:UIToastShowMode_fail];
        }
        else{
            NSDictionary *info = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
            callback(info);
        }
    }];
    [task resume];
}

- (void)reqGetRoomListWithParams:(NSDictionary *)params callBack:(LiveGetRoomListBlock)callback{
    NSMutableURLRequest *request = [self getSendPostRequest:@"https://xzb.qcloud.com/webrtc/weapp/webrtc_room/get_room_list" body:params];
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    [sessionConfig setTimeoutIntervalForRequest:30];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:sessionConfig];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error|| data == nil) {
            [[UIToastView getInstance] showToastWithMessage:@"获取房间请求失败" toastMode:UIToastShowMode_fail];
        }
        else{
            NSDictionary *info = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
            callback(info);
        }
    }];
    [task resume];
}

- (void)reqQuitRoomWithParams:(NSDictionary *)params callBack:(LiveQuitRoomBlock)callback{
    NSMutableURLRequest *request = [self getSendPostRequest:@"https://xzb.qcloud.com/webrtc/weapp/webrtc_room/quit_room" body:params];
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    [sessionConfig setTimeoutIntervalForRequest:30];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:sessionConfig];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error|| data == nil) {
            [[UIToastView getInstance] showToastWithMessage:@"退出房间请求失败" toastMode:UIToastShowMode_fail];
        }
        else{
            callback(0);
        }
    }];
    [task resume];
}
- (void)sendHeartBeat:(NSDictionary *)params callBack:(LiveHearBeatBlock)callback{
    NSMutableURLRequest *request = [self getSendPostRequest:@"https://xzb.qcloud.com/webrtc/weapp/webrtc_room/heartbeat" body:params];
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    [sessionConfig setTimeoutIntervalForRequest:30];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:sessionConfig];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error|| data == nil) {
            [[UIToastView getInstance] showToastWithMessage:@"心跳包发送失败" toastMode:UIToastShowMode_fail];
        }
        else{
            
            NSDictionary *info = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
            callback(info);
        }
    }];
    [task resume];
}
- (NSMutableURLRequest *)getSendPostRequest:(NSString *)url body:(NSDictionary *)body{
    
    NSData *dataBody = [NSJSONSerialization dataWithJSONObject:body options:NSJSONWritingPrettyPrinted error:nil];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url] cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:10];
    [request setValue:[NSString stringWithFormat:@"%ld", (long) [dataBody length]] forHTTPHeaderField:@"Content-Length"];
    [request setHTTPMethod:@"POST"];
    [request setValue:@"application/json; charset=UTF-8" forHTTPHeaderField:@"Content-Type"];
    [request setValue:@"gzip" forHTTPHeaderField:@"Accept-Encoding"];
    [request setHTTPBody:dataBody];

    return request;
}
@end
