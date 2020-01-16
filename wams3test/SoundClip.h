//
//  SoundClip.h
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/16.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef SoundClip_h
#define SoundClip_h


class SoundClip
{
public:
    enum SoundClipStatus {
        WORKING,
        OK,
        FAIL
    };
    
    enum SoundClipInternalStatus {
        IN_INIT,
        IN_FETCHING,
        IN_WORKING,
        IN_OK,
        IN_FAIL
    };
    
    SoundClip(NSString* url) : m_url(url) {}
    ~SoundClip();
    
    const NSString* url() const { return m_url;}
    
    void addRef() { ++m_refCount; }
    void releaseRef() { --m_refCount; }
    int refCount() const { return m_refCount; }
    
    void queueDeletion()        { m_queuedForDelete = true; }
    bool isQueuedForDeletion()  { return m_queuedForDelete; }

    SoundClipStatus checkLoad();

    bool okay() const { return m_status == IN_OK; }            // Called from decoding thread
    const int16_t* frames() const { return m_frames; }      // Called from decoding thread
    uint64_t numFrames() const { return m_nFrames; }        // Called from decoding thread
    
private:
    NSString* m_url;
    
    void* m_memory = 0;
    size_t m_memorySize = 0;
    
    int m_refCount = 0;
    bool m_queuedForDelete = false;
    SoundClipInternalStatus m_status = IN_INIT;
    int16_t* m_frames = 0;
    uint64_t m_nFrames = 0;
};

#endif /* SoundClip_h */
