//
//  m3_api_bgfx.c
//  wams3test
//
//  Created by fzhang on 09/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//
#include<bgfx/c99/bgfx.h>
#include "m3_api_bgfx.h"

# if defined(__cplusplus)
extern "C" {
# endif
    
#include "m3_api_wasi.h"

#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"
    
    
    
#include <stdlib.h>
#include <string.h>
#define FATAL(msg, ...) { printf("Error: [Fatal] " msg "\n", ##__VA_ARGS__); }


static IM3Function mallocFunc;

M3Result m3_InitMallocFunc(IM3Runtime runtime) {
    M3Result result = m3_FindFunction (&mallocFunc, runtime, "malloc");
    return result;
}

void* m3_InvokeMallocFunc(IM3Runtime runtime, int size) {
    char num[20];
    sprintf(num, "%d", size);
  //  itoa (size, num, 10);
    const char* i_argv[1] = { num, NULL };

    M3Result result = m3_CallWithArgs (mallocFunc, 1, i_argv);
    if (result) {
        FATAL("m3_InvokeMallocFunc: %s", result);
    }
    
    // todo 
    uint64_t value = *(uint64_t*)(runtime->stack);
    return result;
}

//void* m3_internal_bgfx_get_caps() {
//    const bgfx_caps_t* ret = bgfx_get_caps();
//}


m3ApiRawFunction(m3_bgfx_get_caps)
{
    static u64 capsPtr = 0;
    m3ApiReturnType(ptr32)
    const bgfx_caps_t* ret = bgfx_get_caps();
    
    u64 malloc_args[] = {sizeof(bgfx_caps_t)};
    if (capsPtr == 0) {
        M3Result result = m3_CallDirect(mallocFunc, malloc_args, &capsPtr);
        if (result) {
            FATAL("m3_InvokeMallocFunc: %s", result);
        }
    }
//    bgfx_init_t x;
//    x.allocator
//   // bx::AllocatorI
   
    memcpy(m3ApiOffsetToPtr(capsPtr), ret, sizeof(bgfx_caps_t));
    
    m3ApiReturn(capsPtr)
    return m3Err_none;
}
   
    static
    M3Result SuppressLookupFailure(M3Result i_result)
    {
        if (i_result == m3Err_functionLookupFailed)
            return m3Err_none;
        else
            return i_result;
    }
    
    extern M3Result  m3_LinkBGFX_Gen  (IM3Module module, void* handle);
    M3Result  m3_LinkBGFX  (IM3Module module, void* handle)
    {
        M3Result result = m3Err_none;

        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_caps", "*()", &m3_bgfx_get_caps)));
        
        m3_LinkBGFX_Gen  (module, handle);

        

    _catch:
        return result;
    }
