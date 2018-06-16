
//
//  RefreshHeader.m
//  TRTC
//
//  Created by Tencent on 2018/6/5.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RefreshHeader.h"

NSString* const RefreshHeaderKeyPathContentOffset = @"contentOffset";
CGFloat const HeaderHeight = 64;

@interface RefreshHeader ()
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) UIActivityIndicatorView *activityView;
@property (nonatomic, strong) UIImageView *arrowView;
@property (nonatomic, strong) UILabel *stateLable;
@property (nonatomic, copy) RefreshHeaderBlock refreshHeaderBlock;
@property (nonatomic, assign) BOOL isRefresh;
@end

@implementation RefreshHeader

- (instancetype)initWithTarget:(id)target beginRefreshBlock:(RefreshHeaderBlock)refreshHeaderBlock{
    self = [super init];
    if (self) {
        self.frame = CGRectMake(0, 0, 0, HeaderHeight);
        _refreshHeaderBlock = refreshHeaderBlock;
        
        _scrollView = (UIScrollView *)target;
        [_scrollView addSubview:self];
        NSKeyValueObservingOptions option = NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld;
        [_scrollView addObserver:self forKeyPath:RefreshHeaderKeyPathContentOffset options:option context:nil];
    }
    return self;
}

- (void)layoutSubviews{
    [super layoutSubviews];
    
    CGFloat superWidth = self.scrollView.frame.size.width;
    
    self.frame = CGRectMake(0, - HeaderHeight, superWidth, HeaderHeight);
    
    self.activityView.center = CGPointMake(CGRectGetMidX(self.bounds) - 15, CGRectGetMidY(self.bounds));
    
    self.arrowView.center = CGPointMake(CGRectGetMidX(self.bounds) - 15, CGRectGetMidY(self.bounds));
    
    self.stateLable.center = CGPointMake(CGRectGetMidX(self.bounds)+45, CGRectGetMidY(self.bounds));
}

#pragma mark setter
- (UIActivityIndicatorView *)activityView{
    if (!_activityView) {
        _activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
        _activityView.hidesWhenStopped = YES;
        [self addSubview:_activityView];
    }
    return _activityView;
}

- (UIImageView *)arrowView{
    if (!_arrowView) {
//        _arrowView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 15, 40)];
        _arrowView.image = [UIImage imageNamed:@"regresh_header_arrow"];
        [self addSubview:_arrowView];
    }
    return _arrowView;
}

- (UILabel *)stateLable{
    if (!_stateLable) {
        _stateLable = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 90, 40)];
        _stateLable.backgroundColor = [UIColor clearColor];
        _stateLable.font = [UIFont systemFontOfSize:12];
        _stateLable.textAlignment = NSTextAlignmentLeft;
        _stateLable.textColor = [UIColor whiteColor];
        [self addSubview:_stateLable];
    }
    return _stateLable;
}

#pragma mark public
- (void)beginRefreshing{
    if (!_isRefresh) {
        
        _isRefresh = YES;
        
        //设置偏移量,衔接加载的更多数据
        [UIView animateWithDuration:0.3 animations:^{
            //刷新控件停留
            [self.scrollView setContentInset:UIEdgeInsetsMake(HeaderHeight, 0, 0, 0)];
            
            [self.activityView startAnimating];
            self.arrowView.hidden = YES;
            self.stateLable.text = @"加载中";
        }];
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            self.refreshHeaderBlock();
        });
    }
}

- (void)endRefreshing{
    _isRefresh = NO;
    
    [UIView animateWithDuration:0.3 animations:^{
        [self.activityView stopAnimating];
        self.arrowView.hidden = YES;
        self.arrowView.transform =  CGAffineTransformMakeRotation(M_PI*2);
        
        //还原scrollView
        [self.scrollView setContentInset:UIEdgeInsetsMake(0, 0, 0, 0)];
    }];
}

#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context{
    if ([keyPath isEqualToString:RefreshHeaderKeyPathContentOffset]) {
        [self scrollViewContentOffsetDidChange:change];
    }else{
        [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
    }
}

#pragma mark logicol
//判断contentOffset<0代表手势为往下拖动，根据isDragging属性判断是否松开，如果松开且偏移量大于（HeaderHeight）,启动刷新动作
- (void)scrollViewContentOffsetDidChange:(NSDictionary *)change{
    
    self.stateLable.hidden = NO;
    CGFloat offset = self.scrollView.contentOffset.y;
    if (offset < 0) {
        //正在拖拽
        if(self.scrollView.isDragging){
            //还原scrollView
            [self.scrollView setContentInset:UIEdgeInsetsMake(0, 0, 0, 0)];
            
            [UIView animateWithDuration:0.3 animations:^{
//                self.arrowView.hidden = NO;
                if (offset < -HeaderHeight*1.5) {
                    self.arrowView.transform =  CGAffineTransformMakeRotation(M_PI);
                    [self.activityView startAnimating];
                    self.stateLable.text = @"下拉刷新";
                }else{
                    self.arrowView.transform =  CGAffineTransformMakeRotation(M_PI*2);
                    self.stateLable.text = @"载入更多";
                }
            }];
            
            return;
        }
        
        //松开且偏移量大于头部高度，启动刷新
        if (offset < -HeaderHeight) {
            [self beginRefreshing];
        }
    }
}

-(void)dealloc{
    [self.scrollView removeObserver:self forKeyPath:RefreshHeaderKeyPathContentOffset];
}

@end


@implementation UIColor (hex)
+ (UIColor *)colorWithHexString:(NSString *)hex {
    if ([hex length] != 6 && [hex length] != 3) {
        return nil;
    }
    
    NSUInteger digits = [hex length] / 3;
    CGFloat maxValue = (digits==1) ? 15.0 : 255.0;
    
    CGFloat red = [[hex substringWithRange:NSMakeRange(0, digits)] integerValueFromHex] / maxValue;
    CGFloat green = [[hex substringWithRange:NSMakeRange(digits, digits)] integerValueFromHex] / maxValue;
    CGFloat blue = [[hex substringWithRange:NSMakeRange(2 * digits, digits)] integerValueFromHex] / maxValue;
    
    return [UIColor colorWithRed:red green:green blue:blue alpha:1.0];
}
@end


@implementation NSString (hex)
- (NSUInteger)integerValueFromHex {
    NSScanner *scanner = [NSScanner scannerWithString:self];
    unsigned int value;
    if ([scanner scanHexInt:&value]) {
        return [NSNumber numberWithUnsignedInt:value].unsignedIntegerValue;
    }
    return 0;
}
@end

