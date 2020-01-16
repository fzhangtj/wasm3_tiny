//
//  AudioDelegate.h
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/16.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef AudioDelegate_h
#define AudioDelegate_h

#include "SoundClip.h"
#include "SoundSource.h"

uint32_t startLoad(NSString* path);

int checkLoading(uint32_t clipId);

void initAudio();

uint32_t playSource(uint32_t clipID, uint32_t sourceID, float volume, bool loop);

bool isPlaying(uint32_t sourceID);

bool stopSource(uint32_t sourceID);

void pauseAudio(bool pause);

#endif /* AudioDelegate_h */
