//
//  InputDelegate.c
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/14.
//  Copyright © 2020 zorro. All rights reserved.
//


//Mainly Copied from Library/PackageCahce/com.unity.platforms.ios@0.1.8-preview.1 > Editor > Unity.Platforms.IOS > Tiny-iPhone~ > Sources > iPhoneInputImpl.mm

#include <set>
#include <cassert>
#import <UIKit/UIKit.h>

struct Touch
{
    UITouch const* native;
    int eventFrame;
    double timestamp;
    UInt32 id;
    long phase;
    long endPhaseInQueue;
    UInt32 xPos;
    UInt32 yPos;
    UITouchType type;
    float deltaTime;

    bool isEmpty() const { return native == 0; }
    bool isFinished() const { return (isEmpty() || (phase == UITouchPhaseEnded) || (phase == UITouchPhaseCancelled)); }
    bool willBeFinishedNextFrame() const { return (!isEmpty() && ((endPhaseInQueue == UITouchPhaseEnded) || (endPhaseInQueue == UITouchPhaseCancelled))); }
    bool isNow(uint frame) const { return (eventFrame == frame); }
    bool isOld(uint frame) const { return (eventFrame < frame); }
};

/*  Even though the maximum number of supported touches is usually no more than 10 in hardware,
    we may get more than 100 different finger IDs when the frame rate is low and the user is
    performing very rapid touches.

    The reason for this is that we use different finger IDs for touches that have began and ended
    during the same frame.
*/
enum { MaxTouchCount = 128 };
int gEventFrame = 0;
Touch gTouches[MaxTouchCount];
unsigned gTouchesActiveBound = 0;


//current the Engine reads only at most 100 touch infos at one frame
//therefore the buffer is enough (1024 > 100 * 4)
int32_t touch_info_stream[1024];
int32_t touch_count = 0;
int windowH;
int windowW;

Touch* FindTouch(UITouch const* native, uint eventFrame)
{
    // check if we have touch in the array already
    for (unsigned q = 0; q < gTouchesActiveBound; ++q)
    {
        Touch& t = gTouches[q];
        if (t.native != native)
            continue;

        // NOTE: touch event can be finished and then begin again between InputProcess() calls (by tapping finger fast)
        // to handle such situation we assign touch to new finger and make sure old one finishes
        if (!t.isOld(eventFrame) && t.isFinished())
            continue;

        if (t.willBeFinishedNextFrame())
            continue;

        assert(t.id == q);
        return &t;
    }

    // find empty slot for touch
    for (unsigned q = 0; q < MaxTouchCount; ++q)
    {
        Touch& t = gTouches[q];
        if (t.native != 0)
            continue;

        t.native = native;
        assert(t.id == q);
        gTouchesActiveBound = std::max(gTouchesActiveBound, q + 1);
        return &t;
    }

    // this is always possible under sufficiently eager user and slow framerate
    //ErrorString("Out of free touches!");
    return 0;
}

bool UpdateTouchData(UIView* view, UITouch const* native, uint eventFrame)
{
    assert(native && "UITouch pointer unexpectedly null");
    Touch* touch = FindTouch(native, eventFrame);
    if (!touch)
        return false;

    UITouchPhase newPhase = [native phase];
    if (newPhase == UITouchPhaseBegan)
    {
        touch->timestamp = [native timestamp];

        // during the same frame Ended/Cancelled should never appear after Begin
        assert(touch->phase == UITouchPhaseEnded || touch->phase == UITouchPhaseCancelled);
    }

    // handle phase priorities
    // Move is more important than Stationary
    // Ended/Cancelled is more important than Move
    // Begin is most important (during the same frame Ended/Cancelled never appears after Begin)
    if (newPhase == UITouchPhaseBegan || touch->isOld(eventFrame))
    {
        touch->phase = newPhase;
        touch->endPhaseInQueue = 0;
    }
    else if (newPhase == UITouchPhaseEnded || newPhase == UITouchPhaseCancelled)
    {
        // if touch began this frame, we will delay end phase for one frame
        if (touch->phase == UITouchPhaseBegan)
            touch->endPhaseInQueue = newPhase;
        else
            touch->phase = newPhase;
    }
    else if (newPhase == UITouchPhaseMoved && touch->phase == UITouchPhaseStationary)
        touch->phase = newPhase;

    touch->type = [native type];
    
    float posX = [native locationInView: view].x * view.contentScaleFactor;
    float posY = [native locationInView: view].y * view.contentScaleFactor;
    
    touch->xPos = posX;
    touch->yPos = posY;
    touch->deltaTime += std::max((float)([native timestamp] - touch->timestamp), 0.0f);
    touch->timestamp = [native timestamp];
    touch->eventFrame = eventFrame;

    // sanity checks
    touch->deltaTime = std::max(touch->deltaTime, 0.0f);
    touch->timestamp = std::max(touch->timestamp, 0.0);

    return true;
}

void ResetTouches()
{
    gTouchesActiveBound = 0;
    for (unsigned q = 0; q < MaxTouchCount; ++q)
    {
        gTouches[q].id = q;
        gTouches[q].native = 0;
        gTouches[q].eventFrame = 0;
        gTouches[q].phase = UITouchPhaseCancelled;
        gTouches[q].endPhaseInQueue = 0;
    }
}

void FreeExpiredTouches(uint eventFrame)
{
    for (unsigned q = 0; q < gTouchesActiveBound; ++q)
    {
        Touch& touch = gTouches[q];
        if (touch.isOld(eventFrame) && touch.isFinished())
            touch.native = 0;
    }

    // update top bound
    unsigned q = gTouchesActiveBound;
    for (; q > 0; --q)
    {
        if (gTouches[q - 1].native != 0)
            break;
    }
    gTouchesActiveBound = q;
}

void UpdateTapTouches(uint eventFrame)
{
    for (unsigned q = 0; q < gTouchesActiveBound; ++q)
    {
        Touch& touch = gTouches[q];
        if (touch.isOld(eventFrame) && !touch.isFinished() && touch.willBeFinishedNextFrame())
        {
            touch.phase = touch.endPhaseInQueue;
            touch.endPhaseInQueue = 0;
            touch.eventFrame = eventFrame;
        }
    }
}

void UpdateStationaryTouches(uint eventFrame)
{
    for (unsigned q = 0; q < gTouchesActiveBound; ++q)
    {
        Touch& touch = gTouches[q];
        if (touch.isOld(eventFrame) && !touch.isFinished())
        {
            touch.phase = UITouchPhaseStationary;
            touch.eventFrame = eventFrame;
        }
    }
}

void CancelStaleTouches(NSSet* allTouches)
{
    for (unsigned q = 0; q < gTouchesActiveBound; ++q)
    {
        if (gTouches[q].isFinished() || gTouches[q].willBeFinishedNextFrame())
            continue;

        bool found = false;
        for (UITouch* touch in allTouches)
            found |= (gTouches[q].native == touch);

        // In some cases when the app is returning from background touches can just disappear so we cancel those
        if (!found)
        {
            gTouches[q].phase = UITouchPhaseCancelled;
        }
    }
}

void ProcessTouchEvents(UIView* view, NSSet* touches, NSSet* allTouches)
{
    for (UITouch* touch in touches)
        UpdateTouchData(view, touch, gEventFrame);
    CancelStaleTouches(allTouches);
}

void CancelTouches()
{
    for (unsigned int i = 0; i < gTouchesActiveBound; i++)
    {
        Touch& touch = gTouches[i];

        if (touch.isFinished() || touch.willBeFinishedNextFrame())
            continue;

        // if touch began this frame, we will delay end phase for one frame
        if (touch.phase == UITouchPhaseBegan)
            touch.endPhaseInQueue = UITouchPhaseCancelled;
        else
            touch.phase = UITouchPhaseCancelled;
    }
}

void ResetInput()
{
    gEventFrame = 1;
    ResetTouches();
}

void InputInit(UIView* view)
{
    [view setMultipleTouchEnabled:YES];
    ResetInput();
}

void InputInitWindowSize(int width, int height) {
    windowH = height;
    windowW = width;
}

int32_t getWindowHeight() {
    return windowH;
}

int32_t getWindowWidth() {
    return windowW;
}

void touchevent(int uid, int action, int xpos, int ypos)
{
    if (touch_count > 1020) {
        return;
    }
    //NSLog(@"action = %d", action);
    int touchState = 0;
    
    //Begin
    if (action == 0) {
        touchState = 1;
    }
    //End
    else if (action == 1) {
        touchState = 0;
    }
    //Move
    else if (action == 2) {
        touchState = 2;
    }
    //Cancel
    else if (action == 3) {
        touchState = 3;
    }
    
    touch_info_stream[touch_count] = touchState;
    touch_info_stream[touch_count + 1] = (int)uid;
    touch_info_stream[touch_count + 2] = ((int)xpos);
    touch_info_stream[touch_count + 3] = windowH - 1 - (int)ypos;
    
    //for(int i=0; i<=3; i++) {
    //    NSLog(@"touch info = %d", touch_info_stream[touch_count + i]);
    //}
    touch_count += 4;
}

int32_t* getTouches() {
    return touch_info_stream;
}

int32_t getTouchCount() {
    return touch_count;
}

void clearTouches() {
    touch_count = 0;
}

void InputProcess()
{
    assert((gEventFrame != 0) && "Must call InputInit() before");
    UpdateTapTouches(gEventFrame);
    UpdateStationaryTouches(gEventFrame);
    
    // pass touch data to InputSystem
    for (unsigned int i = 0; i < gTouchesActiveBound; i++)
    {
        Touch& touch = gTouches[i];
        int phase = -1;
        switch (touch.phase)
        {
            case UITouchPhaseBegan: phase = 0; break;
            case UITouchPhaseEnded: phase = 1; break;
            case UITouchPhaseMoved: phase = 2; break;
            case UITouchPhaseCancelled: phase = 3; break;
        }
        if (phase != -1) touchevent(touch.id, phase, touch.xPos, touch.yPos);
    }

    ++gEventFrame;
    FreeExpiredTouches(gEventFrame);
}

void InputShutdown()
{
    ResetTouches();
}

