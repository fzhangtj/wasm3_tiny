//
//  m3_api_bgfx.c
//  wams3test
//
//  Created by fzhang on 09/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//
#include<bgfx/c99/bgfx.h>
#include "m3_api_bgfx.h"

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
    if (!result) {
        FATAL("m3_InvokeMallocFunc: %s", result);
    }
    
    // todo 
    uint64_t value = *(uint64_t*)(runtime->stack);
    return result;
}
