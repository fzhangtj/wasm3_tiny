//
//  InputDelegate.h
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/14.
//  Copyright © 2020 zorro. All rights reserved.
//


//Mainly Copied from Library/PackageCahce/com.unity.platforms.ios@0.1.8-preview.1 > Editor > Unity.Platforms.IOS > Tiny-iPhone~ > Sources > iPhoneInputImpl.h

#ifndef InputDelegate_h
#define InputDelegate_h


#import <UIKit/UIKit.h>

void InputInit(UIView* view);
void InputShutdown();
void InputProcess();
void ProcessTouchEvents(UIView* view, NSSet* touches, NSSet* allTouches);
void InputInitWindowSize(int with, int height);
int32_t* getTouches();
int32_t getTouchCount();
void clearTouches();
int32_t getWindowHeight();
int32_t getWindowWidth();

#endif /* InputDelegate_h */
