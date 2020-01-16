//
//  utils.m
//  wams3test
//
//  Created by fzhang on 16/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#include "utils.h"
#include <stdlib.h>

#import <Foundation/Foundation.h>

void* loadRemoteAsset(const char* path, int* size) {
    NSURL* nsbaseurl = [NSURL URLWithString:  [NSString stringWithUTF8String:BASE_URL]];
    NSURL *fileUrl = [NSURL URLWithString: [NSString stringWithUTF8String: path] relativeToURL: nsbaseurl];

    NSData *fileData = [NSData dataWithContentsOfURL:fileUrl];
    void* result = malloc(fileData.length);
    memcpy(result, fileData.bytes, fileData.length);
    *size = fileData.length;
    return result;
}
