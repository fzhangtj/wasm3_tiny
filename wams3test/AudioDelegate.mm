//
//  AudioDelegate.m
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/16.
//  Copyright © 2020 zorro. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "AudioDelegate.h"
#include <map>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>

#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"   /* Enables WAV decoding. */
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

void* AlignedAlloc(size_t size, size_t alignment)
{
    // Mac & iOS seem to really not want this allocated using posix_memalign.  Must be malloc.
    return malloc(size);
}

void AlignedFree(void* p)
{
    free(p);
}

std::mutex sourceListMutex;

uint32_t clipIDPool = 0;
std::map<uint32_t, SoundClip*> clipMap;
uint32_t sourceIDPool = 0;
std::map<uint32_t, SoundSource*> sourceMap;

std::map<uint32_t, uint32_t> sourceIdMap;
std::map<uint32_t, uint32_t> clipIdMap;

ma_device_config maConfig;
ma_device* maDevice;

struct UserData
{
    void* dummy;
};

UserData userData;

bool audioInitialized = false;
bool audioPaused = false;

void flushMemory()
{
    std::vector<std::map<uint32_t, SoundClip*>::iterator> clipDeleteList;
    std::vector<std::map<uint32_t, SoundSource*>::iterator> sourceDeleteList;

    for(auto it = clipMap.begin(); it != clipMap.end(); ++it) {
        SoundClip* clip = it->second;
        if (clip->isQueuedForDeletion() && clip->refCount() == 0) {
            clipDeleteList.push_back(it);
        }
    }
    for(int i=0; i<(int)clipDeleteList.size(); ++i) {
        SoundClip* clip = clipDeleteList[i]->second;
        delete clip;
        clipMap.erase(clipDeleteList[i]);
    }

    std::lock_guard<std::mutex> lock(sourceListMutex);
    for(auto it = sourceMap.begin(); it != sourceMap.end(); ++it) {
        SoundSource* source = it->second;
        if (source->readyToDelete()) {
            sourceDeleteList.push_back(it);
        }
    }

    for(int i=0; i<(int)sourceDeleteList.size(); ++i) {
        SoundSource* source = sourceDeleteList[i]->second;
        delete source;
        //printf("Deleting sound source.");
        sourceMap.erase(sourceDeleteList[i]);
    }
}


void sendFramesToDevice(ma_device* pDevice, void* pSamples, const void* pInput, ma_uint32 frameCount)
{
    const uint32_t bytesPerSample = ma_get_bytes_per_sample(pDevice->playback.format);
    const uint32_t bytpesPerFrame = ma_get_bytes_per_frame(pDevice->playback.format, pDevice->playback.channels);
    //ASSERT(bytesPerSample == 2);
    //ASSERT(bytpesPerFrame == 4);
    
    uint32_t nSamples = frameCount * 2; // Stereo
    static const int MAX_CHANNELS = 16; // Caps the performance

    memset(pSamples, 0, bytpesPerFrame * frameCount);
    if (audioPaused) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(sourceListMutex);
    
    int count = 0;
    for(auto it = sourceMap.begin(); it != sourceMap.end() && count < MAX_CHANNELS; ++it) {
        SoundSource* source = it->second;
        if (source->isPlaying() && source->volume()) {
            bool done = false;
            ++count;

            uint32_t totalFrames = 0;
            int16_t* target = (int16_t*)pSamples;
            int32_t volume = int32_t(source->volume() * 1024.0f);

            while(!done) {
                uint32_t decodedFrames = 0;
                const int16_t* src = source->fetch(frameCount - totalFrames, &decodedFrames);
                totalFrames += decodedFrames;
                
                // Now 'buffer' is the source. Apply the volume and copy to 'pSamples'
                for(uint32_t i=0; i<decodedFrames; ++i) {
                    int val = *target + *src * volume / 1024;
                    if (val < SHRT_MIN) val = SHRT_MIN;
                    if (val > SHRT_MAX) val = SHRT_MAX;
                    *target = val;
                    ++target;
                    ++src;

                    val = *target + *src * volume / 1024;
                    if (val < SHRT_MIN) val = SHRT_MIN;
                    if (val > SHRT_MAX) val = SHRT_MAX;
                    *target = val;
                    ++target;
                    ++src;
                }

                done = true;
                if (source->loop() && totalFrames < frameCount) {
                    done = false;
                    source->rewind();
                }
            }
        }
    }
    
    return;
}

void initAudio() {
    if (!audioInitialized) {
        maConfig = ma_device_config_init(ma_device_type_playback);
        maConfig.playback.format = ma_format_s16;
        maConfig.playback.channels = 2;
        maConfig.sampleRate = 44100;
        maConfig.dataCallback = sendFramesToDevice;
        maConfig.pUserData = &userData;
        
        if (maConfig.playback.format != ma_format_s16) {
            printf("Failed to get signed-16 format.");
            return;
        }
        if (maConfig.playback.channels != 2) {
            printf("Failed to get stereo format.");
            return;
        }
        if (maConfig.sampleRate != 44100) {
            printf("Failed to get 44100 Hz.");
            return;
        }
        
        maDevice = (ma_device*) AlignedAlloc(sizeof(ma_device), sizeof(void*));
        
        if (ma_device_init(NULL, &maConfig, maDevice) != MA_SUCCESS) {
            printf("Failed to init audio device.");
            return;
        }
        
        if (ma_device_start(maDevice) != MA_SUCCESS) {
            printf("Failed to start audio device.");
            return;
        }
    }
    
    printf("initAudio() okay");
    audioInitialized = true;
}

uint32_t startLoad(NSString* path, uint32_t clipID) {
    if (!audioInitialized) return 0;
    
    //NSLog(@"load audio: %@ as %d", path, clipID);
    
    ++clipIDPool;
    clipMap[clipIDPool] = new SoundClip(path);
    clipIdMap[clipID] = clipIDPool;
    
    return clipIDPool;
}

int checkLoading(uint32_t clipId) {
    if (!audioInitialized) return SoundClip::SoundClipStatus::FAIL;
    flushMemory();
    
    auto idt = clipIdMap.find(clipId);
    if (idt == clipIdMap.end()){
        return SoundClip::SoundClipStatus::FAIL;
    }
    
    uint32_t clipSourceId = idt->second;
    
    auto it = clipMap.find(clipSourceId);
    if (it == clipMap.end())
    {
        return SoundClip::SoundClipStatus::FAIL;
    }
    SoundClip* clip = it->second;
    return clip->checkLoad();
}

void updateSourceIdMap(uint32_t sourceId, uint32_t soundSourceId) {
    auto it = sourceIdMap.find(sourceId);
    if (it != sourceIdMap.end()) {
        uint32_t oldSSId = it->second;
        auto sit = sourceMap.find(oldSSId);
        if (sit != sourceMap.end()) {
            SoundSource* source = sit->second;
            source->stop();
            sourceMap.erase(sit);
            delete source;
        }
    }
    
    sourceIdMap[sourceId] = soundSourceId;
}

uint32_t playSource(uint32_t clipID, uint32_t sourceID, float volume, bool loop)
{
    if (!audioInitialized) return 0;
    flushMemory();
    
    auto idt = clipIdMap.find(clipID);
    if (idt == clipIdMap.end())
    {
        return SoundClip::SoundClipStatus::FAIL;
    }
    
    uint32_t clipSourceId = idt->second;

    auto it = clipMap.find(clipSourceId);
    if (it == clipMap.end()) {
        printf("playSource() clipID=%d failed.", clipID);
        return false;
    }

    SoundClip* clip = it->second;
    //ASSERT(clip);
    if (clip == NULL) {
        printf("invalid clip!");
    }

    SoundSource* source = new SoundSource(clip);

    source->setVolume(volume);
    source->setLoop(loop);
    source->play();

    if (source->getStatus() == SoundSource::SoundStatus::Playing)
    {
        std::lock_guard<std::mutex> lock(sourceListMutex);
        sourceMap[++sourceIDPool] = source;
        
        //printf("SoundSource %d for clip %d created\n", sourceID, clipID);
        
        updateSourceIdMap(sourceID, sourceIDPool);
        
        return sourceIDPool;
    }
    source->stop();
    delete source;
    return 0;
}

bool isPlaying(uint32_t sourceID)
{
    if (!audioInitialized) return false;

    std::lock_guard<std::mutex> lock(sourceListMutex);
    
    auto idt = sourceIdMap.find(sourceID);
    if (idt == sourceIdMap.end()) {
        return false;
    }
    
    uint32_t soundSourceId = idt->second;
    
    auto it = sourceMap.find(soundSourceId);
    if (it == sourceMap.end()) {
        // This isn't an error; the lifetime of an Audio object on the C#
        // side doesn't match the object here. If it's deleted, it just isn't playing.
        return false;
    }
    const SoundSource* source = it->second;
    return source->getStatus() == SoundSource::SoundStatus::NotYetStarted ||
           source->getStatus() == SoundSource::SoundStatus::Playing;
}

void pauseAudio(bool _audioPaused)
{
    if (_audioPaused != audioPaused) {
        audioPaused = _audioPaused;
        //printf("%s", audioPaused ? "*paused*" : "*un-paused");
    }
}

bool stopSource(uint32_t sourceID)
{
    if (!audioInitialized) return false;

    std::lock_guard<std::mutex> lock(sourceListMutex);
    auto idt = sourceIdMap.find(sourceID);
    if (idt == sourceIdMap.end()) {
        return false;
    }
    
    uint32_t soundSourceId = idt->second;
    
    auto it = sourceMap.find(soundSourceId);
    if (it == sourceMap.end()) {
        return false;
    }

    //printf("stopSource() source=%d", sourceID);

    SoundSource* source = it->second;
    source->stop();
    //printf("SoundSource %d deleted", sourceID);
    delete source;
    sourceMap.erase(it);
    return true;
}
