//
//  M3BxAllocator.cpp
//  wams3test
//
//  Created by fzhang on 13/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#include "M3BxAllocator.h"
#include "m3_api_wasi.h"

#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"
#include "m3_api_bgfx.h"
#include <bgfx/c99/bgfx.h>
#ifndef BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
#    define BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT 8
#endif // BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
extern "C" void* checkBound (IM3Runtime runtime, void* ptr);
extern "C" ptr32 m3_InvokeMallocFunc(int size);
    
extern "C"  void m3_InvokeFreeFunc(ptr32 p);
    
extern "C"  ptr32 m3_InvokeReallocFunc(ptr32 p, int size);

static IM3Runtime runtime;

bgfx_allocator_interface_t g_bgfx_allocator_interface_t;
bgfx_allocator_vtbl_s vt;
M3BxAllocator gM3BxAllocator;
extern "C" void InitAllocator(IM3Runtime _runtime) {
    runtime = _runtime;
   // bgfx_init_t x;
   // *allocator = (bgfx_allocator_interface_t*)(&gM3BxAllocator);
}

class M3AllocatorC99 : public bx::AllocatorI
{
public:
    virtual ~M3AllocatorC99()
    {
    }

    virtual void* realloc(void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line) override
    {
        return m_interface->vtbl->realloc(m_interface, _ptr, _size, _align, _file, _line);
    }

    bgfx_allocator_interface_t* m_interface;
};


void* m3_bgfx_allocator(bgfx_allocator_interface_t* _this, void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line) {
    u8* _mem = m3MemData(runtime->memory.mallocated);
    M3AllocatorC99 d;
    d.m_interface =_this;
    if (0 == _size)
    {
        if (NULL != _ptr)
        {
            if (BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT >= _align)
            {
                u32 offset = m3ApiPtrToOffset(_ptr);
                m3_InvokeFreeFunc(offset);
                //::free(_ptr);
                return NULL;
            }
            
            //#    if BX_COMPILER_MSVC
            //                BX_UNUSED(_file, _line);
            //                _aligned_free(_ptr);
            //#    else
            bx::alignedFree(&d, _ptr, _align, _file, _line);
            //#    endif // BX_
        }
        
        return NULL;
    }
    else if (NULL == _ptr)
    {
        if (BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT >= _align)
        {
            // return ::malloc(_size);
            ptr32 offset = m3_InvokeMallocFunc(_size);
            return m3ApiOffsetToPtr(offset);
        }
        
        //#    if BX_COMPILER_MSVC
        //            BX_UNUSED(_file, _line);
        //            return _aligned_malloc(_size, _align);
        //#    else
        return bx::alignedAlloc(&d, _size, _align, _file, _line);
        //#    endif // BX_
    }
    //
    if (BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT >= _align)
    {
        u32 offset = m3ApiPtrToOffset(_ptr);
        ptr32 result = m3_InvokeReallocFunc(offset, _size);
        return m3ApiOffsetToPtr(result);
        // return ::realloc(_ptr, _size);
    }
    //
    //#    if BX_COMPILER_MSVC
    //        BX_UNUSED(_file, _line);
    //        return _aligned_realloc(_ptr, _size, _align);
    //#    else
    return bx::alignedRealloc(&d, _ptr, _size, _align, _file, _line);
    //#    endif // BX_
}


extern "C" bgfx_allocator_interface_t* GetAllocator() {
//    bgfx_init_t x;
//    x.allocator;
    vt.realloc = &m3_bgfx_allocator;
    g_bgfx_allocator_interface_t.vtbl = &vt;
    return &g_bgfx_allocator_interface_t;
}

M3BxAllocator::M3BxAllocator()
    {
    }

    M3BxAllocator::~M3BxAllocator()
    {
    }
//#define m3ApiOffsetToPtr(offset)   (void*)((u8*)_mem + (u32)(offset))
//#define m3ApiPtrToOffset(ptr)      (u32)((u8*)ptr - (u8*)_mem)
    void* M3BxAllocator::realloc(void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line)
    {
        u8* _mem = (u8*)runtime->memory.mallocated;
        //        return NULL;
        if (0 == _size)
        {
            if (NULL != _ptr)
            {
                if (BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT >= _align)
                {
                    u32 offset = m3ApiPtrToOffset(_ptr);
                    m3_InvokeFreeFunc(offset);
                    //::free(_ptr);
                    return NULL;
                }

//#    if BX_COMPILER_MSVC
//                BX_UNUSED(_file, _line);
//                _aligned_free(_ptr);
//#    else
                bx::alignedFree(this, _ptr, _align, _file, _line);
//#    endif // BX_
            }

            return NULL;
        }
        else if (NULL == _ptr)
        {
            if (BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT >= _align)
            {
                // return ::malloc(_size);
                ptr32 offset = m3_InvokeMallocFunc(_size);
                return m3ApiOffsetToPtr(offset);
            }

//#    if BX_COMPILER_MSVC
//            BX_UNUSED(_file, _line);
//            return _aligned_malloc(_size, _align);
//#    else
            return bx::alignedAlloc(this, _size, _align, _file, _line);
//#    endif // BX_
        }
//
        if (BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT >= _align)
        {
            u32 offset = m3ApiPtrToOffset(_ptr);
            ptr32 result = m3_InvokeReallocFunc(offset, _size);
            return m3ApiOffsetToPtr(result);
            // return ::realloc(_ptr, _size);
        }
//
//#    if BX_COMPILER_MSVC
//        BX_UNUSED(_file, _line);
//        return _aligned_realloc(_ptr, _size, _align);
//#    else
       return bx::alignedRealloc(this, _ptr, _size, _align, _file, _line);
//#    endif // BX_
    }
