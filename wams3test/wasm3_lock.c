//
//  Wasm3Lock.cpp
//  wams3test
//
//  Created by fzhang on 16/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//
#include <stdio.h>

#include<pthread.h>
#include <stdlib.h>
#include "wasm3_lock.h"
//
//bool Wasm3Lock::init() {
//    pthread_mutexattr_t attr;
//    pthread_mutexattr_init(&attr);
//    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
//    int result = pthread_mutex_init(&Wasm3Lock::wasmMutex, &attr);
//    if (result) {
//        printf("pthread_mutex_init fail with code %d\n", result);
//        return false;
//    }
//    return true;
//}

static pthread_mutex_t wasmMutex;
static int initialized = 0;
int wasm3_lock_init() {
    if (initialized) {
        printf("wasm3_lock_init already initialized\n");
        return -1;
    }
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int result = pthread_mutex_init(&wasmMutex, &attr);
    if (result) {
        printf("pthread_mutex_init fail with code %d\n", result);
        return -1;
    }
    
    initialized = 1;
    return 0;
}

int wasm3_lock() {
//    if (!initialized) {
//        printf("Wasm3Lock not initialized!\n");
//        return -1;
//    }
//    int lockresult = pthread_mutex_lock( &wasmMutex );
//    if (lockresult) {
//        printf("pthread_mutex_lock fail with code %d\n", lockresult);
//        return -1;
//    }
    return 0;
}

int wasm3_unlock() {
//    if (!initialized) {
//        printf("Wasm3Lock not initialized!\n");
//        return -1;
//    }
//    int unlockresult = pthread_mutex_unlock(&wasmMutex );
//    if (unlockresult) {
//        printf("pthread_mutex_unlock fail with code %d\n", unlockresult);
//        return -1;
//    }
    return 0;
}

