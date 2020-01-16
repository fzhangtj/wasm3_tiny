//
//  utils.h
//  wams3test
//
//  Created by fzhang on 16/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef utils_h
#define utils_h

# if defined(__cplusplus)
extern "C" {
# endif

void* loadRemoteAsset(const char* path, int* size);

#if defined(__cplusplus)
}
# endif

#define BASE_URL "http://192.168.0.102:8080"
#endif /* utils_h */
