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

u32 bgfxmemory_t_to_wasm(bgfx_memory_t *mem_block, u8* _mem) {
    u32 ptr = (u8 *)mem_block - _mem;
    bgfx_memory_s_wasm *mem_block_wasm = (bgfx_memory_s_wasm *)mem_block;
    
    mem_block_wasm->data = (u8*) mem_block->data - _mem;
    mem_block_wasm->size = mem_block->size;
    
    return ptr;
}

bgfx_memory_t* bgfxmemory_t_from_wasm(u32 ptr, u8* _mem) {
    bgfx_memory_s_wasm *mem_block_wasm = (bgfx_memory_s_wasm *)(ptr + _mem);
    bgfx_memory_t *mem_block = (bgfx_memory_t *)(ptr + _mem);
    
    mem_block->size = mem_block_wasm->size;
    mem_block->data = (uint8_t *) (mem_block_wasm->data + _mem);
    return mem_block;
}


IM3Function mallocFunc;

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
    
    m3ApiRawFunction(m3_bgfx_dbg_text_printf)
    {
        m3ApiGetArg(i32, arg0)
        m3ApiGetArg(i32, arg1)
        m3ApiGetArg(i32, arg2)
        m3ApiGetArgMem(char *, arg3)
        m3ApiGetArg(i32, arg4)
        
        printf("m3_bgfx_dbg_text_printf: %s", arg3);
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

_        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_caps", "*()", &m3_bgfx_get_caps)));
_        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_dbg_text_printf", "v(iiiii)", &m3_bgfx_dbg_text_printf)));

        m3_LinkBGFX_Gen  (module, handle);

        

    _catch:
        return result;
    }
