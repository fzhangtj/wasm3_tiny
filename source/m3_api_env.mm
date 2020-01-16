//
//  m3_api_env.c
//  wams3test
//
//  Created by kg on 2020/1/11.
//  Copyright © 2020 zorro. All rights reserved.
//

#include "m3_api_env.h"

    
#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"
#include "m3_exec.h"
#include "emscripten_fetch.h"
#include "wasm3_lock.h"
#include "InputDelegate.h"
#include "AudioDelegate.h"

#include <time.h>
#include <unistd.h>


static M3Result SuppressLookupFailure (M3Result i_result)
{
    if (i_result == m3Err_functionLookupFailed)
        return m3Err_none;
    else
        return i_result;
}

m3ApiRawFunction(emscripten_get_now)
{
    m3ApiReturnType (double_t)
    
    f64 result = 0.0;

    struct timespec tp;
    if (!clock_gettime(CLOCK_MONOTONIC, &tp)) {
        result = tp.tv_sec * 1000.0 + tp.tv_nsec / 1000000.0;
    }

    //printf("[tiny_log] call emscripten_get_now %lf\n", result);
    m3ApiReturn(result)
}

m3ApiRawFunction(env_clock)
{
    m3ApiReturnType (uint32_t)

    m3ApiReturn(clock())
}

m3ApiRawFunction(js_html_initImageLoading)
{
    return m3Err_none;
}

m3ApiRawFunction(__syscall4)
{
    m3ApiReturnType (int32_t)

    m3ApiGetArg     (int32_t, which) // should be 4.
    m3ApiGetArgMem  (u32*,    args)

    int fd = args[0];
    char *buf = (char *)m3ApiOffsetToPtr(args[1]);
    size_t nbyte = args[2];

    size_t ret = write(fd, buf, nbyte);
    m3ApiReturn(ret);
}

typedef struct AnimationFrameCallback
{
    IM3Function function;
    i32 userData;
} AnimationFrameCallback;

AnimationFrameCallback __raf;

void runAnimationFrame(f64 timeStamp)
{
    wasm3_lock();
    if (__raf.function) {
        AnimationFrameCallback raf = __raf;
        memset(&__raf, 0, sizeof(__raf));
        
        M3Result result = m3Err_none;
        
        u64 r;
        u64 args[2] = {(u64)timeStamp, (u64)raf.userData};
      
_       (m3_CallDirect(raf.function, args, &r))
        if (r) {
            __raf = raf;
        }
        
_catch:
        if (result)
            fprintf (stderr, "error running runAnimationFrame: %s\n", result);
    }
    wasm3_unlock();
}

m3ApiRawFunction(emscripten_request_animation_frame_loop)
{
    printf("[tiny_log] call emscripten_request_animation_frame_loop\n");

    M3Result result = m3Err_none;
    
    m3ApiGetArg     (i32, cb)
    m3ApiGetArg     (i32, userdata)
    
    IM3Function function = NULL;
    
    IM3Module module = runtime->modules; // for now, assume it's always the first module.
_   (m3_FindIndirectFunction(&function, module, cb))
    
    __raf.function = function;
    __raf.userData = userdata;
        
_catch:
    return result;
}

m3ApiRawFunction(emscripten_throw_string)
{
    printf("[tiny_log] call emscripten_throw_string\n");

    M3Result result = m3Err_none;
    
    m3ApiGetArgMem  (char*,    buf)
    
    fprintf (stderr, "emscripten_throw_string: %s\n", buf);

    return m3Err_trapAbort;
}

m3ApiRawFunction(emscripten_is_main_browser_thread)
{
    m3ApiReturnType(i32)
    m3ApiReturn(1)
}


m3ApiRawFunction(js_html_init)
{
    return m3Err_none;
}

m3ApiRawFunction(js_html_getScreenSize)
{
    m3ApiGetArgMem  (u32*,    wPtr)
    m3ApiGetArgMem  (u32*,    hPtr)
    
    *wPtr = 375;
    *hPtr = 667;
    
    return m3Err_none;
}


m3ApiRawFunction(js_html_setCanvasSize)
{
    m3ApiReturnType (i32)
    m3ApiGetArg  (i32,    weight)
    m3ApiGetArg  (i32,    height)
    m3ApiGetArg  (i32,    webgl)

    m3ApiReturn(1)
}

m3ApiRawFunction(js_inputInit)
{
    m3ApiReturnType (i32)
    m3ApiReturn(1)
}

m3ApiRawFunction(js_inputResetStreams)
{
    clearTouches();
    return m3Err_none;
}

m3ApiRawFunction(js_inputSetMouseMode)
{
    m3ApiGetArg  (i32,    arg1)
    return m3Err_none;
}

m3ApiRawFunction(js_inputGetKeyStream)
{
    m3ApiReturnType (i32)
    m3ApiGetArg  (i32,    arg1)
    m3ApiGetArg  (i32,    arg2)
    m3ApiReturn(0)
}

m3ApiRawFunction(js_inputGetTouchStream)
{
    m3ApiReturnType (i32)
    m3ApiGetArg     (i32,  maxLen)
    m3ApiGetArgMem  (i32*, Ptr)

    int32_t touchCount = getTouchCount();
    int32_t* touches = getTouches();

    if (touchCount != 0) {
        if (touchCount > maxLen) {
            touchCount = maxLen;
        }
        //NSLog(@"touchCount = %d", touchCount);
        memcpy (Ptr, touches, touchCount * sizeof(int32_t));
    }

    m3ApiReturn(touchCount)
}

m3ApiRawFunction(js_inputGetCanvasLost)
{
    m3ApiReturnType (i32)
    m3ApiReturn(0)
}

m3ApiRawFunction(js_html_initAudio)
{
    m3ApiReturnType (i32)
    initAudio();
    m3ApiReturn(1)
}

m3ApiRawFunction(js_html_audioIsUnlocked)
{
    m3ApiReturnType (i32)
    m3ApiReturn(1)
}

m3ApiRawFunction(js_html_audioUnlock)
{
    return m3Err_none;
}

m3ApiRawFunction(js_html_audioPause)
{
    //pauseAudio(true);
    return m3Err_none;
}

m3ApiRawFunction(js_html_audioResume)
{
    //pauseAudio(false);
    return m3Err_none;
}

m3ApiRawFunction(js_html_audioStartLoadFile)
{
    m3ApiReturnType (i32)
    m3ApiGetArg(u32, fetchptr)
    m3ApiGetArg(int32_t, audioClipIdx)

    char *audioClipName = (char *)m3ApiOffsetToPtr(fetchptr);
    NSString* url = [NSString stringWithUTF8String: audioClipName];
    
    uint32_t clipIndex = startLoad(url);
    m3ApiReturn(clipIndex)
}

m3ApiRawFunction(js_html_audioCheckLoad)
{
    m3ApiReturnType (i32)
    m3ApiGetArg(int32_t, audioClipIdx)
    
    int status = checkLoading(audioClipIdx);
    m3ApiReturn(status);
}

m3ApiRawFunction(js_html_audioFree)
{
    m3ApiGetArg(int32_t, audioClipIdx)
    return m3Err_none;
}

m3ApiRawFunction(js_html_audioPlay)
{
    m3ApiReturnType (i32)
    m3ApiGetArg(int32_t, audioClipIdx)
    m3ApiGetArg(int32_t, audioSourceIdx)
    m3ApiGetArg(double_t, volume)
    m3ApiGetArg(int32_t, loop)
    
    playSource(audioClipIdx, audioSourceIdx, volume, loop);
    
    m3ApiReturn(1)
}

m3ApiRawFunction(js_html_audioStop)
{
    m3ApiReturnType (i32)
    m3ApiGetArg(int32_t, audioSourceIdx)
    m3ApiGetArg(int32_t, doStop)
    
    uint32_t isOk = 0;
    if (doStop != 0) {
        isOk = stopSource(audioSourceIdx) ? 1 : 0;
    }
    
    m3ApiReturn(isOk)
}

m3ApiRawFunction(js_html_audioIsPlaying)
{
    m3ApiReturnType (i32)
    m3ApiGetArg(int32_t, audioSourceIdx)
    
    uint32_t isPlay = isPlaying(audioSourceIdx) ? 1 : 0;
    
    m3ApiReturn(isPlay)
}

M3Result    m3_LinkENV     (IM3Module module)
{
    M3Result result = m3Err_none;

    const char* mod_name  = "env";

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_get_now",                            "F()",   &emscripten_get_now)));
  
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_performance_now",                    "F()",   &emscripten_get_now)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "clock",                                         "i()",   &env_clock)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_initImageLoading",                      "v()",   &js_html_initImageLoading)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "__syscall4",                                    "i(ii)", &__syscall4)));
    
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_request_animation_frame_loop",       "v(ii)", &emscripten_request_animation_frame_loop)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_throw_string",                       "v(i)",  &emscripten_throw_string)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_is_main_browser_thread",             "i()",   &emscripten_is_main_browser_thread)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_start_fetch",                        "v(i)",  &emscripten_start_fetch)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "_emscripten_fetch_free",                        "v(i)",  &_emscripten_fetch_free)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_init",                                  "v()",   &js_html_init)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_getScreenSize",                         "v(ii)", &js_html_getScreenSize)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_getCanvasSize",                         "v(ii)", &js_html_getScreenSize)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_getFrameSize",                          "v(ii)", &js_html_getScreenSize)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_setCanvasSize",                         "i(iii)", &js_html_setCanvasSize)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputInit",                         "i()", &js_inputInit)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputResetStreams",                 "v()", &js_inputResetStreams)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputSetMouseMode",                 "v(i)", &js_inputSetMouseMode)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetKeyStream",                 "i(ii)", &js_inputGetKeyStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetMouseStream",               "i(ii)", &js_inputGetKeyStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetTouchStream",               "i(ii)", &js_inputGetTouchStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetCanvasLost",                "i()", &js_inputGetCanvasLost)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetFocusLost",                 "i()", &js_inputGetCanvasLost)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_initAudio",                 "i()", &js_html_initAudio)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioIsUnlocked",                 "i()", &js_html_audioIsUnlocked)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioUnlock",                 "v()", &js_html_audioUnlock)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioPause",                 "v()", &js_html_audioPause)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioResume",                 "v()", &js_html_audioResume)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioStartLoadFile",                 "i(*i)", &js_html_audioStartLoadFile)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioCheckLoad",                 "i(i)", &js_html_audioCheckLoad)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioFree",                 "v(i)", &js_html_audioFree)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioPlay",                 "i(iiFi)", &js_html_audioPlay)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioStop",                 "i(ii)", &js_html_audioStop)));
    (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_audioIsPlaying",                 "i(i)", &js_html_audioIsPlaying)));

_catch:
    return result;
}
