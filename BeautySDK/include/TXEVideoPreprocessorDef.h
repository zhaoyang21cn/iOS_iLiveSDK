//
//  TXEPreprocessorDef.h
//  basic
//
//  Created by alderzhang on 2017/5/24.
//  Copyright © 2017年 Tencent. All rights reserved.
//

#ifndef TXEPreprocessorDef_h
#define TXEPreprocessorDef_h

typedef NS_ENUM(NSInteger, TXEFilterType)
{
    TXE_FILTER_TYPE_NONE        = 0,        //无滤镜效果
    TXE_FILTER_TYPE_WHITE       = 1,        //美白
    TXE_FILTER_TYPE_FRESH       = 2,        //清新
    TXE_FILTER_TYPE_ROMANTIC    = 3,        //浪漫
    TXE_FILTER_TYPE_AESTHETICS  = 4,        //唯美
    TXE_FILTER_TYPE_PINK        = 5,        //粉嫩
    TXE_FILTER_TYPE_NOSTALGIC   = 6,        //怀旧
    TXE_FILTER_TYPE_BLUE        = 7,        //蓝调
    TXE_FILTER_TYPE_CLEAR       = 8,        //清亮
    TXE_FILTER_TYPE_JAPANESE    = 9,        //日系
};

typedef NS_ENUM(NSInteger, TXEBeautyStyle)
{
    TXE_BEAUTY_STYLE_SMOOTH       = 0,        //光滑
    TXE_BEAUTY_STYLE_NATURE       = 1,        //自然
};


typedef NS_ENUM(NSInteger, TXEPreprocessEvent)
{
    TXE_PREPROCESS_EVENT_FACERECOGNIZE_SUCCESS  =   4001,    //人脸识别成功
    TXE_PREPROCESS_EVENT_FACERECOGNIZE_FAILED   =   4002,    //人脸识别失败
};
#endif /* TXEPreprocessorDef_h */
