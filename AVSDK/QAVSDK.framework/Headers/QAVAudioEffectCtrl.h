
#ifndef QAVEFFECT_QAVAudioEffectCtrl_h
#define QAVEFFECT_QAVAudioEffectCtrl_h
#import "QAVSDK/QAVError.h"

typedef void (^QAVAccompanyComplete)(int result, NSString* filePath);

@interface QAVAudioEffectCtrl : NSObject

// 控制接口，一次仅能播放一个伴奏
// loopBack:是否混音发送，一般都设置为true
// loopCount:循环次数，-1为无限循环
// block: 播放结束时的回调
- (QAVResult)startAccompany:(NSString*)filePath loopBack:(bool)loopback loopCount:(int)loopCount duckerTimeMs:(int)duckerTimeMs complete:(QAVAccompanyComplete)block;
- (QAVResult)stopAccompany:(int)duckerTimeMs;
- (QAVResult) pauseAccompany;
- (QAVResult) resumeAccompany;
- (bool)isAccompanyPlayEnd;

- (QAVResult) enableAccompanyPlay:(bool)enable;
- (QAVResult) enableAccompanyLoopBack:(bool)enable;

// 播放伴奏的音量，为DB音量，默认值为61，大于61增益，小于61减益
- (QAVResult) setAccompanyVolumeDB:(int)nDB;
- (int) getAccompanyVolumeDB;

// 获得伴奏播放进度
// 需要注意，Current / Total = 当前循环次数，Current % Total = 当前循环播放位置
- (unsigned int) getAccompanyFileTotalTimeByMs;
- (unsigned int) getAccompanyFileCurrentPlayedTimeByMs;
- (QAVResult) SetAccompanyFileCurrentPlayedTimeByMs:(unsigned int)timeMs;


// 播放音效的音量，为线性音量，默认值为100，大于100增益，小于100减益
-(int)getEffectsVolume;
- (QAVResult)setEffectsVolume:(int)volume;

// 控制接口，soundId需App侧进行管理，唯一标识一个独立文件；
// loop:是否进行循环播放；
// pitch:未实现，预期未修改音调，默认值1.0；
// pan:未实现，预期为音效空间位置，默认值0表示音效出现在正前方，-1表示音效出现在左方，1表示音效出现在右方
- (QAVResult)playEffect:(int)soundId filePath:(NSString*)filePath loop:(bool)loop pitch:(double)pitch pan:(double)pan gain:(double)gain;
- (QAVResult)pauseEffect:(int)soundId;
- (QAVResult)pauseAllEffects;
- (QAVResult)resumeEffect:(int)soundId;
- (QAVResult)resumeAllEffects;
- (QAVResult)stopEffect:(int)soundId;
- (QAVResult)stopAllEffects;

// 预加载管理
- (QAVResult)preloadEffect:(int)soundId filePath:(NSString*) filePath;
- (QAVResult)unloadEffect:(int)soundId;
- (QAVResult)selectEffectInterface:(int)type;
@end

#endif
