//
//  TCLiveChatTableView.m
//  TRTC
//
//  Created by Tencent on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <ImSDK/ImSDK.h>
#import <ILiveSDK/ILiveSDK.h>
#import <ILiveSDK/ILiveRoomManager.h>
#import "TCLiveChatTableView.h"
#import "TCLiveChatTableViewCell.h"
#import "UIColorEX.h"
#import "UIToastView.h"
#import "TCLiveRequestManager.h"

@interface TCLiveChatTableView () <UITableViewDelegate,UITableViewDataSource,TIMMessageListener>
@property(nonatomic,strong)NSMutableArray *chatMessageList;
@property(nonatomic,strong)UIColor *contentColor;
@property(nonatomic,strong)UIColor *nickColor;
@end

@implementation TCLiveChatTableView

- (instancetype)initWithFrame:(CGRect)frame style:(UITableViewStyle)style{
    if (self = [super initWithFrame:frame style:style]) {
        _chatMessageList = [[NSMutableArray alloc] initWithCapacity:1];
        self.delegate = self;
        self.dataSource = self;
        self.separatorStyle = UITableViewCellSeparatorStyleNone;
        self.transform = CGAffineTransformMakeScale(1, -1);
        self.layer.cornerRadius = 4;
        self.layer.masksToBounds = YES;
        self.scrollEnabled = NO;
        //设置消息监听
        [[[ILiveSDK getInstance] getTIMManager] setMessageListener:self];
    }
    return self;
}

//接收消息
-(void)onNewMessage:(NSArray *)msgs{
    [self addChatMessage:msgs withContentColor:nil nickColor:nil];
}
//消息处理
- (void)addChatMessage:(NSArray *)msgList withContentColor:(UIColor *)contentColor nickColor:(UIColor *)nickColor{
    self.contentColor = contentColor;
    self.nickColor = nickColor;
    for (id item in msgList) {
        [self.chatMessageList insertObject:item atIndex:0];
    }
    //过滤非文本消息
    NSMutableArray *tempArr = [NSMutableArray array];
    for (int i = 0; i < msgList.count;i++) {
        TIMMessage *msg = msgList[i];
        if (![self isTextMsg:msg]) {
            [tempArr addObject:msg];
        }
    }
    [self.chatMessageList removeObjectsInArray:tempArr];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self reloadData];
    });
}
- (BOOL)isTextMsg:(TIMMessage *)msg {
    TIMOfflinePushInfo *info = msg.getOfflinePushInfo;
    if ([info.ext hasPrefix:@"TEXT"]) {
        return YES;
    }
    int count = [msg elemCount];
    for(int i = 0; i < count; i++) {
        TIMElem *elem = [msg getElem:i];
        if ([elem isKindOfClass:[TIMCustomElem class]]){
            if ([((TIMCustomElem*)elem).ext hasPrefix:@"TEXT"]) {
                return YES;
            }
        }
        else if ([elem isKindOfClass:[TIMTextElem class]]){
            return YES;
        }
    }
    return NO;
}
//发送消息
- (void)sendMessage:(NSString *)message{
    //消息组装
    TIMMessage *msge = [[TIMMessage alloc] init];
    TIMCustomElem *textElem = [[TIMCustomElem alloc] init];
    textElem.data = [message dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *descDic = @{@"nickName":[TCLiveRequestManager getInstance].userID};
    NSString *desc = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:descDic options:NSJSONReadingAllowFragments error:nil] encoding:NSUTF8StringEncoding];
    textElem.desc = desc;
    textElem.ext = @"TEXT";
    [msge addElem:textElem];
    //调用发送接口
    [[ILiveRoomManager getInstance] sendGroupMessage:msge succ:^{
        NSLog(@"send message succ");
    } failed:^(NSString *module, int errId, NSString *errMsg) {
        NSLog(@"send message fail");
        [[UIToastView getInstance] showToastWithMessage:@"发送消息失败" toastMode:UIToastShowMode_fail];
    }];
    [self addChatMessage:@[msge] withContentColor:nil nickColor:nil];
}
#pragma mark - UITableViewDelegate

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    TCLiveChatTableViewCell *cell = [self createChatTableViewCell:tableView withIndexPath:indexPath];
    return cell.cellHeight;
}
#pragma mark - UITableViewDataSource

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.chatMessageList.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    TCLiveChatTableViewCell *cell = [self createChatTableViewCell:tableView withIndexPath:indexPath];
    return cell;
}
//创建消息cell
- (TCLiveChatTableViewCell *)createChatTableViewCell:(UITableView *)tableView withIndexPath:(NSIndexPath *)indexPath{
    TCLiveChatTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"ChatTableviewCell"];
    if (!cell) {
        cell = [[TCLiveChatTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"ChatTableviewCell"];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        cell.backgroundColor = [UIColor clearColor];
        cell.contentView.transform = CGAffineTransformMakeScale (1,-1);
    }
    TIMMessage *msg = self.chatMessageList[indexPath.row];
    int count = [msg elemCount];
    for(int i = 0; i < count; i++) {
        TIMElem *elem = [msg getElem:i];
        //收到消息展示
        NSMutableAttributedString *msgInfo = [[NSMutableAttributedString alloc] initWithString:@""];
        if([elem isKindOfClass:[TIMTextElem class]]){
            msgInfo = [self getContentWithNick:msg.sender andContentTex:((TIMTextElem *)elem).text];
            [cell setModel:msgInfo];
            break;
        }
        else if ([elem isKindOfClass:[TIMCustomElem class]]){
            NSString *nick = msg.sender;
            NSString *dataStr = [[NSString alloc] initWithData:((TIMCustomElem *)elem).data encoding:NSUTF8StringEncoding];
            NSDictionary *descDic = [NSJSONSerialization JSONObjectWithData:[((TIMCustomElem *)elem).desc dataUsingEncoding:NSUTF8StringEncoding] options:NSJSONReadingAllowFragments error:nil];
            NSString *nickNmae = descDic[@"nickName"];
            if (nickNmae.length > 0) {
                nick = nickNmae;
            }
            msgInfo = [self getContentWithNick:nick andContentTex:dataStr];
            [cell setModel:msgInfo];
            break;
        }
    }
    return  cell;
}
- (NSMutableAttributedString *)getContentWithNick:(NSString *)nick andContentTex:(NSString *)contentText{
    NSString *content = [NSString stringWithFormat:@"%@：%@",nick, contentText];
    NSMutableAttributedString *msgInfo = [[NSMutableAttributedString alloc] initWithString:content];
    UIColor *contentColor = [UIColor whiteColor];
    UIColor *nickColor = [UIColor colorWithRGBHex:0xFF4081];
    if (self.contentColor) {
        contentColor = self.contentColor;
    }
    if(self.nickColor){
        nickColor = self.nickColor;
    }
    [msgInfo addAttribute:NSForegroundColorAttributeName value:contentColor range:[content rangeOfString:contentText]];
    [msgInfo addAttribute:NSForegroundColorAttributeName value:nickColor range:[content rangeOfString:nick]];
    
    return msgInfo;
}
//点击事件透传
-(UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
    return nil;
}
@end
