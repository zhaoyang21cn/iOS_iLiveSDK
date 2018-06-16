//
//  TCLiveChatTableViewCell.m
//  TRTC
//
//  Created by Tencent on 2018/6/8.
//  Copyright © 2018年 Tencent. All rights reserved.
//


#import "TCLiveChatTableViewCell.h"

@interface TCLiveChatTableViewCell ()
@property(nonatomic,strong) UILabel *contentLabel;
@property(nonatomic,strong) UIView *backMaskView;
@end

@implementation TCLiveChatTableViewCell

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        
        [self.contentView addSubview:self.backMaskView];
        [_backMaskView addSubview:self.contentLabel];
    }
    return self;
}

-(UILabel *)contentLabel{
    if (!_contentLabel) {
        _contentLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        _contentLabel.backgroundColor = [UIColor clearColor];
        _contentLabel.textAlignment = NSTextAlignmentLeft;
        _contentLabel.font = [UIFont systemFontOfSize:13];
        _contentLabel.textColor = [UIColor whiteColor];
        _contentLabel.numberOfLines = 0;
    }
    return _contentLabel;
}

-(UIView *)backMaskView{
    if (!_backMaskView) {
        _backMaskView = [[UIView alloc] initWithFrame:CGRectZero];
        _backMaskView.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.5];
        _backMaskView.layer.cornerRadius = 10;
        _backMaskView.layer.masksToBounds = YES;
    }
    return _backMaskView;
}

-(void)setModel:(NSMutableAttributedString *)model{
    _contentLabel.attributedText = model;
    CGSize size = [_contentLabel sizeThatFits:CGSizeMake(230, 10000)];
 
    _contentLabel.frame = CGRectMake(5, 5, size.width, size.height );
    _backMaskView.frame = CGRectMake(5, 5, size.width + 10, size.height + 10);
    self.cellHeight = size.height+20;
}

@end
