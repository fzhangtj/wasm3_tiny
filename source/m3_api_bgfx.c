//
//  m3_api_bgfx.c
//  wams3test
//
//  Created by fzhang on 09/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//
#include<bgfx/c99/bgfx.h>
#include "m3_api_bgfx.h"
//#include "bx_p.h"
//#include <bx/allocator.h>

//#include <malloc.h>

# if defined(__cplusplus)
extern "C" {
# endif
    
#include "m3_api_wasi.h"

#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"
#include "wasm3_lock.h"
    
    
#include <stdlib.h>
#include <string.h>
#define FATAL(msg, ...) { printf("Error: [Fatal] " msg "\n", ##__VA_ARGS__); }
    
    
#define m3ApiRawFunction(NAME) \
    const void * __##NAME (IM3Runtime runtime, uint64_t * _sp, void * _mem);  \
    const void * NAME (IM3Runtime runtime, uint64_t * _sp, void * _mem) \
    { \
        wasm3_unlock(); \
        const void *res = __##NAME(runtime, _sp, _mem); \
        wasm3_lock(); \
        return res; \
    }\
    \
    inline const void * __##NAME (IM3Runtime runtime, uint64_t * _sp, void * _mem)

    
static void* whandle;
static IM3Function mallocFunc;
static IM3Function reallocFunc;
static IM3Function freeFunc;

extern  void InitAllocator(IM3Runtime _runtime) ;
extern  bgfx_allocator_interface_t* GetAllocator();
M3Result m3_InitMallocFunc(IM3Runtime runtime) {
    M3Result result = m3_FindFunction (&mallocFunc, runtime, "malloc");

    result = m3_FindFunction (&freeFunc, runtime, "free");
    result = m3_FindFunction (&reallocFunc, runtime, "realloc");

    return result;
}
    

ptr32 m3_InvokeMallocFunc(int size) {
    u64 bytePtr;
    u64 malloc_args[] = {size};
   // *((uint32_t*)malloc_args) = size;
    M3Result result =  m3_CallDirect(mallocFunc, malloc_args, &bytePtr);
    if (result) {
        FATAL("m3_InvokeFreeFunc: %s", result);
    }
//    printf("alloc_call m3_InvokeMallocFunc %d %p\n", (u32)bytePtr, mallocFunc->module->runtime->memory.mallocated);
    return (ptr32)bytePtr;
}
    
void m3_InvokeFreeFunc(ptr32 p) {
  //  u64 bytePtr;
    u64 free_args[] = {p};
   // *((ptr32*)free_args) = p;
    M3Result result =  m3_CallDirect(freeFunc, free_args, NULL);
//    printf("alloc_call m3_InvokeFreeFunc %d %p\n", p, freeFunc->module->runtime->memory.mallocated);
    if (result) {
        FATAL("m3_InvokeFreeFunc: %s", result);
    }
}
    
ptr32 m3_InvokeReallocFunc(ptr32 p, int size) {
    u64 bytePtr;
    u64 realloc_args[] = {p, size};
//    *((ptr32*)realloc_args) = p;
//    *((uint32_t*)(realloc_args + 1)) = size;
    M3Result result =  m3_CallDirect(reallocFunc, realloc_args, &bytePtr);
    if (result) {
        FATAL("m3_InvokeReallocFunc: %s", result);
    }
//    printf("alloc_call m3_InvokeReallocFunc old=%d new=%d %p\n", p, (u32)bytePtr,
//           reallocFunc->module->runtime->memory.mallocated);
    return (ptr32)bytePtr;
    //return 0;
}
    
void transient_index_buffer_t_to_wasm(bgfx_transient_index_buffer_t_wasm* dest,  bgfx_transient_index_buffer_t* source, u8* _mem) {
    dest->size = source->size;
    dest->handle = source->handle;
    dest->startIndex = source->startIndex;
    if (source->data == 0) {
        dest->data = 0;
    } else {
        dest->data = m3ApiPtrToOffset(source->data);
    }
    
}

void wasm_to_transient_vertex_buffer_t(bgfx_transient_vertex_buffer_t* dest, bgfx_transient_vertex_buffer_t_wasm* source, u8* _mem) {
    dest->startVertex = source->startVertex;
    dest->stride = source->stride;
    dest->handle = source->handle;
    dest->layoutHandle = source->layoutHandle;
    dest->size = source->size;
    if (source->data == 0) {
        dest->data = 0;
    } else {
        dest->data = m3ApiOffsetToPtr(source->data);
    }
}

void transient_vertex_buffer_t_to_wasm(bgfx_transient_vertex_buffer_t_wasm* dest,  bgfx_transient_vertex_buffer_t* source, u8* _mem) {
    dest->startVertex = source->startVertex;
    dest->stride = source->stride;
    dest->handle = source->handle;
    dest->layoutHandle = source->layoutHandle;
    dest->size = source->size;
    if (source->data == 0) {
        dest->data = 0;
    } else {
        dest->data = m3ApiPtrToOffset(source->data);
    }
}


//void* m3_internal_bgfx_get_caps() {
//    const bgfx_caps_t* ret = bgfx_get_caps();
//}
m3ApiRawFunction(m3_bgfx_init)
    {
    m3ApiReturnType(int32_t)
        m3BgfxApiGetInitArg(_bgfx__init)
        _bgfx__init->allocator = GetAllocator();
    bool ret = bgfx_init(_bgfx__init);
    m3ApiReturn(ret)
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
    
m3ApiRawFunction(m3_bgfx_copy)
{
    M3Result result = m3Err_none;

    m3ApiReturnType(ptr32)
    m3ApiGetArgMem   (const void*       , _bgfx__data)
    m3ApiGetArg   (uint32_t       , _bgfx__size)
    
    int ptr111 = m3ApiPtrToOffset(_bgfx__data);
    if (ptr111==0) {
        printf("gd\n");
    }
    int totalSize = sizeof(bgfx_memory_s_wasm) + _bgfx__size;
    const bgfx_memory_t* bgfxmem = bgfx_alloc(totalSize);
    memcpy(bgfxmem->data, _bgfx__data, _bgfx__size);
    ((bgfx_memory_t*)bgfxmem)->size = _bgfx__size;
    
    bgfx_memory_s_wasm *ret = (bgfx_memory_s_wasm*)(bgfxmem->data + _bgfx__size);
    ret->size = _bgfx__size;
    ret->data = m3ApiPtrToOffset(bgfxmem->data);
    BGFX_CALL_TRACE("copy");
    m3ApiReturn(m3ApiPtrToOffset(ret))
    
_catch:
    return result;
}

m3ApiRawFunction(m3_bgfx_alloc)
{
    M3Result result = m3Err_none;

    m3ApiReturnType(ptr32)
    m3ApiGetArg   (uint32_t       , _bgfx__size)
    
    int totalSize = sizeof(bgfx_memory_s_wasm) + _bgfx__size;
    const bgfx_memory_t* bgfxmem = bgfx_alloc(totalSize);
    ((bgfx_memory_t*)bgfxmem)->size = _bgfx__size;
    
    bgfx_memory_s_wasm *ret = (bgfx_memory_s_wasm*)(bgfxmem->data + _bgfx__size);
    ret->size = _bgfx__size;
    ret->data = m3ApiPtrToOffset(bgfxmem->data);
    BGFX_CALL_TRACE("alloc");
    m3ApiReturn(m3ApiPtrToOffset(ret))
    
_catch:
    return result;
}
       
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
        whandle = handle;
_        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_caps", "*()", &m3_bgfx_get_caps)));
        
_       (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_init", "i(*)", &m3_bgfx_init)));
        //m3_bgfx_alloc
_        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_copy", "*(*i)", &m3_bgfx_copy)));
_        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_alloc", "*(i)", &m3_bgfx_alloc)));
        _        (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_dbg_text_printf", "v(iiiii)", &m3_bgfx_dbg_text_printf)));
        m3_LinkBGFX_Gen  (module, handle);
        InitAllocator(module->runtime);

    _catch:
        return result;
    }


//u32 bgfxmemory_t_to_wasm(bgfx_memory_t *mem_block, u8* _mem) {
//    if (mem_block == NULL) {
//        return 0;
//    }
//    u32 ptr = (u8 *)mem_block - _mem;
//    bgfx_memory_s_wasm *mem_block_wasm = (bgfx_memory_s_wasm *)mem_block;
//
//    mem_block_wasm->data = (u8*) mem_block->data - _mem;
//    mem_block_wasm->size = mem_block->size;
//
//    return ptr;
//}

bgfx_memory_t* bgfxmemory_t_from_wasm(u32 ptr, u8* _mem) {
    if (ptr == 0) {
        return NULL;
    }
    
    bgfx_memory_s_wasm *mem_block_wasm = (bgfx_memory_s_wasm *)(ptr + _mem);
    bgfx_memory_t *mem_block = (bgfx_memory_t *)(m3ApiOffsetToPtr(mem_block_wasm->data) - sizeof(bgfx_memory_t));
//    if (mem_block->size != mem_block_wasm->size) {
//        printf("memory not match");
//        exit(1);
//    }
    return mem_block;
}

void wasm_to_transient_index_buffer_t(bgfx_transient_index_buffer_t* dest, bgfx_transient_index_buffer_t_wasm* source, u8* _mem) {
    dest->size = source->size;
    dest->handle = source->handle;
    dest->startIndex = source->startIndex;
    if (source->data == 0) {
        dest->data = 0;
    } else {
        dest->data = m3ApiOffsetToPtr(source->data);
    }
    
}
    
