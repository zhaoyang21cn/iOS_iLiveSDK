//
//  RefreshFooter.m
//  TRTC
//
//  Created by Tencent on 2018/6/5.
//  Copyright © 2018年 Tencent. All rights reserved.
//

#import "RefreshFooter.h"

NSString* const RefreshFooterKeyPathContentOffset = @"contentOffset";
CGFloat const FooterHeight = 40;

@interface RefreshFooter ()
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) UIActivityIndicatorView *activityView;
@property (nonatomic, strong) UIImageView *arrowView;
@property (nonatomic, strong) UILabel *stateLable;
@property (nonatomic, copy) RefreshFooterBlock refreshHeaderBlock;
@property (nonatomic, assign) BOOL isRefresh;
@end

@implementation RefreshFooter
- (instancetype)initWithTarget:(id)target beginRefreshBlock:(RefreshFooterBlock)refreshHeaderBlock{
    self = [super init];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
        _refreshHeaderBlock = refreshHeaderBlock;
        _scrollView = (UIScrollView *)target;
        [_scrollView addSubview:self];
        NSKeyValueObservingOptions option = NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld;
        [_scrollView addObserver:self forKeyPath:RefreshFooterKeyPathContentOffset options:option context:nil];
    }
    return self;
}

- (void)layoutSubviews{
    [super layoutSubviews];
    CGFloat superWidth = self.scrollView.frame.size.width;
    
    CGFloat superHeight = self.scrollView.frame.size.height > self.scrollView.contentSize.height ? self.scrollView.frame.size.height : self.scrollView.contentSize.height;
    
    self.frame = CGRectMake(0, superHeight, superWidth, FooterHeight);
    
    self.activityView.center = CGPointMake(CGRectGetMidX(self.bounds) - 15, CGRectGetMidY(self.bounds));
    
    self.arrowView.center = CGPointMake(CGRectGetMidX(self.bounds) - 15, CGRectGetMidY(self.bounds));
    
    self.stateLable.center = CGPointMake(CGRectGetMidX(self.bounds) + 45, CGRectGetMidY(self.bounds));
    
    NSLog(@"layoutSubviews");
}

#pragma mark setter
- (UIActivityIndicatorView *)activityView{
    if (!_activityView) {
        _activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        _activityView.hidesWhenStopped = YES;
        [self addSubview:_activityView];
    }
    return _activityView;
}

- (UIImageView *)arrowView{
    if (!_arrowView) {
        _arrowView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 15, 40)];
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
        _stateLable.textColor = [UIColor grayColor];
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
            if (self.scrollView.frame.size.height > self.scrollView.contentSize.height) {
                [self.scrollView setContentInset:UIEdgeInsetsMake(- FooterHeight, 0, 0, 0)];
            }else{
                if (self.scrollView.frame.size.height + self.scrollView.contentOffset.y - self.scrollView.contentSize.height > 0) {
                    [self.scrollView setContentInset:UIEdgeInsetsMake(-(self.scrollView.contentSize.height - self.scrollView.frame.size.height) - FooterHeight, 0, 0, 0)];
                }
            }
            
            [self.activityView startAnimating];
            self.arrowView.hidden = YES;
            self.stateLable.text = @"加载中";
        }];
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            _refreshHeaderBlock();
        });
    }
}

- (void)endRefreshing{
    _isRefresh = NO;
    
    [UIView animateWithDuration:0.3 animations:^{
        [self.activityView stopAnimating];
        self.stateLable.hidden = YES;
        self.arrowView.transform =  CGAffineTransformMakeRotation(M_PI*2);
        self.arrowView.hidden = YES;
    }];
    
    [self setNeedsLayout];
}

#pragma mark KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context{
    if ([keyPath isEqualToString:RefreshFooterKeyPathContentOffset]) {
        [self scrollViewContentOffsetDidChange:change];
    }else{
        [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
    }
}

#pragma mark logicol
- (void)scrollViewContentOffsetDidChange:(NSDictionary *)change{
    
    self.stateLable.hidden = NO;
    CGFloat offset = self.scrollView.contentOffset.y;
    NSLog(@"%f,%f,%f,%f,%f",offset,self.scrollView.contentSize.height, self.scrollView.frame.size.height,self.scrollView.contentInset.bottom,self.scrollView.contentInset.top);
    if (offset > 0) {
        //正在拖拽
        if(self.scrollView.isDragging){
            //还原scrollView
            [self.scrollView setContentInset:UIEdgeInsetsMake(0, 0, 0, 0)];
            
            [UIView animateWithDuration:0.3 animations:^{
                self.arrowView.hidden = NO;
                if (self.scrollView.frame.size.height + self.scrollView.contentOffset.y - self.scrollView.contentSize.height > FooterHeight) {
                    self.arrowView.transform =  CGAffineTransformMakeRotation(M_PI);
                    self.stateLable.text = @"释放更新";
                }else{
                    self.arrowView.transform =  CGAffineTransformMakeRotation(M_PI*2);
                    self.stateLable.text = @"载入更多";
                }
            }];
            
            return;
        }
        
        if (self.scrollView.frame.size.height > self.scrollView.contentSize.height) {
            if (self.scrollView.contentOffset.y > 0) {
                [self beginRefreshing];
            }
            return;
        }
        
        if (self.scrollView.frame.size.height + self.scrollView.contentOffset.y - self.scrollView.contentSize.height > 10) {
            [self beginRefreshing];
        }
    }
}

-(void)dealloc{
    [self.scrollView removeObserver:self forKeyPath:RefreshFooterKeyPathContentOffset];
}
@end
