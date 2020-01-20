//
//  m3_api_image.c
//  wams3test
//
//  Created by fzhang on 16/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#include "m3_api_image.h"

#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"
    
    
int startload_stb(const char *imageFile, const char *maskFile);
int checkload_stb(int32_t idx);
void freeImage_stb(int idx);
int imageToMemory_stb(int idx, int w, int h, uint8_t* destPtr);
void finishLoading_stb(int idx, int* w, int* h, int* hasAlpha);

m3ApiRawFunction(m3LoadImage)
{
    m3ApiReturnType(int32_t)
    m3ApiGetArgMem   (const char*       , imageFile)
    m3ApiGetArgMem   (const char*       , maskFile)
    int32_t ret = startload_stb(imageFile, maskFile);
    m3ApiReturn(ret)
}

m3ApiRawFunction(m3CheckLoadImage)
{
    m3ApiReturnType(int32_t)
    m3ApiGetArg(int32_t, idx)
    int ret = checkload_stb(idx);
    m3ApiReturn(ret)
}

m3ApiRawFunction(m3FreeImage)
{
    m3ApiGetArg(int32_t, idx)
    freeImage_stb(idx);
    return m3Err_none;
}

m3ApiRawFunction(m3ImageToMemory)
{
    m3ApiReturnType(int32_t)
    m3ApiGetArg(int32_t, idx)
    m3ApiGetArg(int32_t, w)
    m3ApiGetArg(int32_t, h)
    m3ApiGetArgMem(int8_t*, destPtr)
    int32_t ret = imageToMemory_stb(idx, w, h, destPtr);
    m3ApiReturn(ret)
}

m3ApiRawFunction(m3FinishLoadImage)
{
    m3ApiReturnType(int32_t)
    m3ApiGetArg(int32_t, idx)
    m3ApiGetArgMem(int32_t*, w)
    m3ApiGetArgMem(int32_t*, h)
    m3ApiGetArgMem(int32_t*, hasAlpha)
    finishLoading_stb(idx, w, h, hasAlpha);
    return m3Err_none;
}

m3ApiRawFunction(m3ImageInit)
{
    return m3Err_none;
}

static M3Result SuppressLookupFailure (M3Result i_result)
{
    if (i_result == m3Err_functionLookupFailed)
        return m3Err_none;
    else
        return i_result;
}


M3Result    m3_LinkImage     (IM3Module module)
{
    M3Result result = m3Err_none;

    const char* mod_name  = "env";
//
//_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "emscripten_get_now",                            "F()",   &emscripten_get_now)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_loadImage",                            "i(**)",   &m3LoadImage)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_checkLoadImage",                            "i(i)",   &m3CheckLoadImage)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_freeImage",                            "v(i)",   &m3FreeImage)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_imageToMemory",                            "i(iii*)",   &m3ImageToMemory)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_initImageLoading",                            "v()",   &m3ImageInit)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, mod_name, "js_html_finishLoadImage",                            "v(i***)",   &m3FinishLoadImage)));
_catch:
    return result;
}

