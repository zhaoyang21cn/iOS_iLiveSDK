#ifndef TXINotifyDelegate_h
#define TXINotifyDelegate_h

#import <Foundation/Foundation.h>

@protocol TXINotifyDelegate <NSObject>

- (void)onNotifyEvent:(int)event withParams:(NSDictionary*)params;

@end

#endif /* TXINotifyDelegate_h */
