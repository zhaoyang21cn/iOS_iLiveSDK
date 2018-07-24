//
//  TCLiveRequestManager.h
//  TRTC
//
//  Created by Tencent on 2018/5/31.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^LiveLoginInfoBlock)(int code);
typedef void (^LiveAuthBufferBlock)(NSDictionary *info);
@interface TCLiveRequestManager : NSObject
@property(nonatomic,assign)int sdkAppID;            //app标识，可在实时音视频控制台(https://console.cloud.tencent.com/rav)创建自己的应用生成
@property(nonatomic,assign)int accountType;         //登录实时音视频应用的帐号类型,在控制台创建应用后分配
@property(nonatomic,strong)NSString *userID;        //用户id标识（可由业务后台自己管理）
@property(nonatomic,strong)NSString *userSig;       //用于用户鉴权，生成方法https://cloud.tencent.com/document/product/647/17275 （可由业务后台自己管理）
+ (TCLiveRequestManager *)getInstance;
- (void)requestLoginInfo:(LiveLoginInfoBlock)block;
- (void)reqGetAuthBufferInfoWithParams:(NSDictionary *)params block:(LiveAuthBufferBlock)block;
@end
