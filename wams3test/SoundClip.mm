//
//  SoundClip.m
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/16.
//  Copyright © 2020 zorro. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "SoundClip.h"
#include "m3.h"
#include "m3_code.h"
#include "miniaudio.h"


SoundClip::SoundClipStatus SoundClip::checkLoad()
{
    if (this->m_status == IN_INIT) {
        //start fetch from url
        NSURL* nsbaseurl = [NSURL URLWithString: @"http://10.86.96.228:8080/"];
        NSURL* nsurl = [NSURL URLWithString: this->m_url relativeToURL: nsbaseurl];
        
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL: nsurl];
        [request setHTTPMethod: @"GET"];
        
        id completionHandler = ^(NSData *receivedData, NSURLResponse *response, NSError *error) {dispatch_async(dispatch_get_main_queue(), ^{
            M3Result result = m3Err_none;
            if (error)
            {
                this->m_status = IN_FAIL;
                printf("error in audio loading: %s", this->m_url);
            }
            else {
                NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                u64 len = [receivedData length];
                const void* data = [receivedData bytes];
                this->m_memory = (void*)data;
                this->m_memorySize = len;
                NSLog(@"response = %@, %d\n", this->m_url, this->m_memorySize);
                
                this->m_status = IN_WORKING;
            }
            
        _catch:
            if (result) {
                this->m_status = IN_FAIL;
                printf("error in dataTask completionHandler: %s.", result);
            }
        });};
        
        NSURLSessionDataTask *task = [[NSURLSession sharedSession]
                    dataTaskWithRequest:request completionHandler:completionHandler];
        [task resume];
        
        this->m_status = IN_FETCHING;
    }
    else if (this->m_status == IN_WORKING) {
        ma_decoder_config config;
        memset(&config, 0, sizeof(config));
        config.format = ma_format_s16;
        config.channels = 2;
        config.sampleRate = 44100;
        void* mem = 0;
        
        auto result = ma_decode_memory(m_memory, m_memorySize, &config, &m_nFrames, &mem);
        if (result != MA_SUCCESS) {
            free(m_memory);
            m_memory = 0;
            m_memorySize = 0;
            
            printf("Error decoding memory (in SoundClip::checkLoad())");
            this->m_status = IN_FAIL;
        }
        else {
            m_frames = (int16_t*) mem;
            m_status = (m_frames && m_nFrames > 0) ? IN_OK : IN_FAIL;
        }
    }
    
    if (this->m_status == IN_INIT || this->m_status == IN_FETCHING || this->m_status == IN_WORKING) {
        return WORKING;
    }
    else if (this->m_status == IN_OK) {
        return OK;
    }
    
    return FAIL;
}

SoundClip::~SoundClip()
{
    if (m_frames) {
        ma_free(m_frames);
    }
    if (m_memory) {
        free(m_memory);
    }
}
