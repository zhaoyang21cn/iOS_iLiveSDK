//
//  TCLiveChatTableViewCell.h
//  TRTC
//
//  Created by Tencent on 2018/6/8.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TCLiveChatTableViewCell : UITableViewCell
@property(nonatomic,assign)CGFloat cellHeight;
-(void)setModel:(NSMutableAttributedString *)model;
@end
