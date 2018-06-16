//
//  TXEVideoTypeDef.h
//  basic
//
//  Created by alderzhang on 2017/5/24.
//  Copyright © 2017年 Tencent. All rights reserved.
//

#ifndef TXEVideoTypeDef_h
#define TXEVideoTypeDef_h

#import <AVFoundation/AVFoundation.h>

typedef NS_ENUM(NSInteger, TXEFrameFormat) {
    TXE_FRAME_FORMAT_NONE = 0,
    TXE_FRAME_FORMAT_NV12 = 1,        //NV12格式
    TXE_FRAME_FORMAT_I420 = 2,        //I420格式
    TXE_FRAME_FORMAT_RGBA = 3,        //RGBA格式
};

//顺时针旋转角度
typedef NS_ENUM(NSInteger, TXERotation) {
    TXE_ROTATION_0 = 0,         //旋转0
    TXE_ROTATION_90 = 1,        //旋转90
    TXE_ROTATION_180 = 2,       //旋转180
    TXE_ROTATION_270 = 3,       //旋转270
};

#endif /* TXEVideoTypeDef_h */
