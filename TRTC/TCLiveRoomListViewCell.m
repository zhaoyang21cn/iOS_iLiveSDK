//
//  TCLiveRoomListView.m
//  TRTC
//
//  Created by ericxwli on 2018/6/3.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "TCLiveRoomListViewCell.h"
#import "UIColorEX.h"
@implementation TCLiveRoomListViewCell
{
    NSDictionary *_roomInfo;
    UILabel *_roomNameLabel;
    UILabel *_roomIdLabel;
    UIImageView *_backgroundView;
    UIImageView *_subscriptIcon;
}
-(instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        self.backgroundColor = [UIColor colorWithRGBHex:0x0B1829];
        
        _backgroundView = [[UIImageView alloc] initWithFrame:CGRectMake(10, 5, self.frame.size.width - 20, self.frame.size.height - 10)];
        [_backgroundView setImage:[UIImage imageNamed:@"muti_room_bg.png"]];
        _backgroundView.layer.cornerRadius = 1;
        [self addSubview:_backgroundView];
        
        _roomNameLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, self.frame.size.width - 20, 20)];
        [_roomNameLabel setFont:[UIFont systemFontOfSize:14]];
        [_roomNameLabel setTextColor:[UIColor whiteColor]];
        _roomNameLabel.textAlignment = NSTextAlignmentLeft;
        [_backgroundView addSubview:_roomNameLabel];
        
        _roomIdLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, _roomNameLabel.frame.origin.y + _roomNameLabel.frame.size.height, self.frame.size.width - 20, 20)];
        [_roomIdLabel setFont:[UIFont systemFontOfSize:14]];
        [_roomIdLabel setTextColor:[UIColor whiteColor]];
        _roomIdLabel.textAlignment = NSTextAlignmentLeft;
        [_backgroundView addSubview:_roomIdLabel];
        
        _subscriptIcon = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"multiroom.png"]];
        _subscriptIcon.backgroundColor = [UIColor clearColor];
        [_backgroundView addSubview:_subscriptIcon];
    }
    return self;
}
-(void)layoutSubviews{
    _backgroundView.frame = CGRectMake(10, 5, self.frame.size.width - 20, self.frame.size.height - 10);
    _roomNameLabel.frame = CGRectMake(10, 10, self.frame.size.width - 20, 20);
    _roomIdLabel.frame =CGRectMake(10, _roomNameLabel.frame.origin.y + _roomNameLabel.frame.size.height, self.frame.size.width - 20, 20);
    _subscriptIcon.frame = CGRectMake(_backgroundView.frame.size.width - 30, _backgroundView.frame.size.height - 25, 25, 20);
}
- (void)setRoomInfo:(NSDictionary *)roomInfo{
    if (_roomInfo != roomInfo) {
        _roomInfo = roomInfo;
        _roomNameLabel.text = [NSString stringWithFormat:@"房间名：%@",roomInfo[@"roomInfo"]];
        _roomIdLabel.text = [NSString stringWithFormat:@"房间ID：%@",roomInfo[@"roomID"]];
    }
}

@end
