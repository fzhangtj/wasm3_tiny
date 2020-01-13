//
//  m3_api_bgfx.h
//  wams3test
//
//  Created by fzhang on 09/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef m3_api_bgfx_h
#define m3_api_bgfx_h

#include "m3_core.h"
#include <stdio.h>

#include<bgfx/c99/bgfx.h>
typedef uint32_t ptr32;

#define bgfx_handle_to_idx(handle) (handle.idx)

typedef struct wasm_bgfx_platform_data
{
       uint32_t             ndt;                /** Native display type.                     */
    uint32_t                nwh;                /** Native window handle.                    */
    uint32_t                context;            /** GL context, or D3D device.               */
    uint32_t                backBuffer;         /** GL backbuffer, or D3D render target view. */
    uint32_t               backBufferDS;       /** Backbuffer depth/stencil.                */

} wasm_bgfx_platform_data_t;

typedef struct wasm_bgfx_init_s
{
    bgfx_renderer_type_t type;
    uint16_t             vendorId;
    uint16_t             deviceId;
    bool                 debug;              /** Enable device for debuging.              */
    bool                 profile;            /** Enable device for profiling.             */
    wasm_bgfx_platform_data_t platformData;       /** Platform data.                           */
    bgfx_resolution_t    resolution;         /** Backbuffer resolution and reset parameters. See: `bgfx::Resolution`. */
    bgfx_init_limits_t   limits;
  
    ptr32 callback;
    
   ptr32 allocator;

} wasm_bgfx_init_t;


static void to_bgfx_init_t(bgfx_init_t * dest, wasm_bgfx_init_t *src) {
    memset(dest, 0, sizeof(bgfx_init_t));
    memcpy(&(dest->resolution), &(src->resolution), sizeof(dest->resolution));
    memcpy(&(dest->limits), &(src->limits), sizeof(dest->limits));
    dest->resolution.reset = src->resolution.reset;
       dest->type = src->type;
        dest->vendorId = src->vendorId;
        dest->deviceId = src->deviceId;
        dest->debug = src->debug;
        dest->profile = src->profile;
        dest->platformData.ndt = (void*)src->platformData.ndt;
        dest->platformData.nwh = (void*)src->platformData.nwh;
        dest->platformData.context = (void*)src->platformData.context;
        dest->platformData.backBuffer = (void*)src->platformData.backBuffer;
        dest->platformData.backBufferDS = (void*)src->platformData.backBufferDS;
}

#define m3BgfxApiGetInitArg(NAME) \
    m3ApiGetArgMem(wasm_bgfx_init_t*, _wasm_NAME) \
    bgfx_init_t _val_NAME;\
    bgfx_init_t* NAME = &_val_NAME; \
    to_bgfx_init_t(NAME, _wasm_NAME); \
NAME->platformData.nwh = whandle;

#define m3BgfxApiGetPlatformDataArg(NAME) \
    m3ApiGetArgMem(wasm_bgfx_platform_data_t*, _wasm_NAME) \
    bgfx_platform_data_t _val_NAME;\
    bgfx_platform_data_t* NAME = &_val_NAME; \
    memset(NAME, 0, sizeof(bgfx_platform_data_t)); \
NAME->nwh = whandle;



# if defined(__cplusplus)
extern "C" {
# endif

    M3Result    m3_LinkBGFX     (IM3Module io_module, void* handle);
    M3Result m3_InitMallocFunc(IM3Runtime runtime) ;


u32 bgfxmemory_t_to_wasm(bgfx_memory_t *mem_block, u8* _mem);
bgfx_memory_t* bgfxmemory_t_from_wasm(u32 ptr, u8* _mem);

#if defined(__cplusplus)
}

# endif

#endif /* m3_api_bgfx_h */
