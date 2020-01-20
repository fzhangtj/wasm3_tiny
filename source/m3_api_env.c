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


// int compareFuncs(const void * ptr1, const void *ptr2) {
//     IM3Function func1 = *(IM3Function *) ptr1;
//     IM3Function func2 = *(IM3Function *) ptr2;
    
//     return (int)(func2->hits1 - func1->hits1);
// }

void runAnimationFrame(f64 timeStamp)
{
    wasm3_lock();
    if (__raf.function) {
        AnimationFrameCallback raf = __raf;
        memset(&__raf, 0, sizeof(__raf));
        
        M3Result result = m3Err_none;
        
        u64 r;
        u64 args[2] = {timeStamp, raf.userData};
      
_       (m3_CallDirect(raf.function, args, &r))
        if (r) {
            __raf = raf;
        }
 
        // static int count = 0;
        // if (count++ % (1) == 0) {
        //     IM3Module module = raf.function->module;
            
        //     IM3Function funcs[module->numFunctions];
        //     for (u32 i = 0; i < module->numFunctions; ++i)
        //     {
        //         funcs[i] = & module->functions [i];
        //     }
            
        //     qsort(funcs, module->numFunctions, sizeof(IM3Function), compareFuncs);
            
        //     for (u32 i = 0; i < module->numFunctions; ++i)
        //     {
        //         IM3Function f = funcs[i];
        //         if (!f) {
        //             continue;
        //         }
                
        //         if (i < 10 && f && f->name) {
        //             //printf("hits; %s; %d; %lf\n", f->name, f->hits, f->hits1);
        //         }
                
        //         f->hits = 0;
        //         f->hits1 = 0.0;
        //     }

        //     printf("=========\n");
        // }
        
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

m3ApiRawFunction(js_inputGetCanvasLost)
{
    m3ApiReturnType (i32)
    m3ApiReturn(0)
}

m3ApiRawFunction(m3_memcpy)
{
    m3ApiReturnType (i32)
    m3ApiGetArgMem(void*, dst)
    m3ApiGetArgMem(void*, src)
    m3ApiGetArg  (i32,    len)

    void *res = memcpy(dst, src, len);
    
    m3ApiReturn(m3ApiPtrToOffset(res))
}

m3ApiRawFunction(m3_memset)
{
    m3ApiReturnType (i32)
    m3ApiGetArgMem(void*, ptr)
    m3ApiGetArg(i32, val)
    m3ApiGetArg(i32, len)

    void *res = memset(ptr, val, len);
    
    m3ApiReturn(m3ApiPtrToOffset(res))
}

m3ApiRawFunction(m3_memmove)
{
     m3ApiReturnType (i32)
       m3ApiGetArgMem(void*, dst)
       m3ApiGetArgMem(void*, src)
       m3ApiGetArg  (i32,    len)

       void *res = memmove(dst, src, len);
       
       m3ApiReturn(m3ApiPtrToOffset(res))
}

m3ApiRawFunction(m3_memcmp)
{
    m3ApiReturnType (i32)
    m3ApiGetArgMem(void*, ptr1)
     m3ApiGetArgMem(void*, ptr2)
    m3ApiGetArg(i32, len)

    int res = memcmp(ptr1, ptr2, len);
    
    m3ApiReturn(res)
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
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetTouchStream",               "i(ii)", &js_inputGetKeyStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetCanvasLost",                "i()", &js_inputGetCanvasLost)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_inputGetFocusLost",                 "i()", &js_inputGetCanvasLost)));

_   (m3_LinkRawFunction (module, "*", "memcpy",                         "i(iii)", &m3_memcpy));
_   (m3_LinkRawFunction (module, "*", "memset",                         "i(iii)", &m3_memset));
_   (m3_LinkRawFunction (module, "*", "memmove",                         "i(iii)", &m3_memmove));
_   (m3_LinkRawFunction (module, "*", "memcmp",                         "i(iii)", &m3_memcmp));

_catch:
    return result;
}

