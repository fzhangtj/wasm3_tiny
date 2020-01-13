//
//  ViewController.m
//  wams3test
//
//  Created by zorro on 2020/1/7.
//  Copyright © 2020 zorro. All rights reserved.
//

#import "ViewController.h"

#include <Metal/Metal.h>
#define HAS_METAL_SDK 1

#ifdef HAS_METAL_SDK
static    id<MTLDevice>  m_device = NULL;
#else
static    void* m_device = NULL;
#endif


#include "m3.h"
#include "m3_api_wasi.h"
#include "m3_api_libc.h"
#include "m3_api_bgfx.h"
#include "m3_api_env.h"

#include "m3_env.h"

#include "extra/fib32.wasm.h"

#define FATAL(msg, ...) { printf("Error: [Fatal] " msg "\n", ##__VA_ARGS__); }

extern "C"
{
    void repl_free(IM3Runtime* runtime)
    {
        if (*runtime) {
            m3_FreeRuntime (*runtime);
        }
    }
    
    M3Result repl_init(IM3Environment env, IM3Runtime* runtime)
    {
        repl_free(runtime);
        *runtime = m3_NewRuntime (env, 64*1024, NULL);
        if (*runtime == NULL) {
            return "m3_NewRuntime failed";
        }
        return m3Err_none;
    }

    M3Result repl_call  (IM3Runtime runtime, const char* name)
    {
        M3Result result = m3Err_none;

        IM3Function func;
        result = m3_FindFunction (&func, runtime, name);
        if (result) return result;

        result = m3_CallWithArgs (func, 0, nil);
        if (result) return result;

        return result;
    }
   
    
    M3Result repl_load  (IM3Runtime runtime, const char* fn, void* whandle)
    {
        M3Result result = m3Err_none;
        
        NSURL *fileUrl = [NSURL URLWithString:@"http://10.86.98.112:8080/MoleTiny3D.wasm"];
        static NSData *fileData = [NSData dataWithContentsOfURL:fileUrl];
        
        u8* wasm = (u8*)fileData.bytes;
        u32 fsize = fileData.length;

        IM3Module module;
        result = m3_ParseModule (runtime->environment, &module, wasm, fsize);
        if (result) return result;
        
        result = m3_LoadModule (runtime, module);
        if (result) return result;
        
        result = m3_LinkWASI (runtime->modules);
        if (result) FATAL("m3_LinkWASI: %s", result);

        result = m3_LinkLibC (runtime->modules);
        if (result) FATAL("m3_LinkLibC: %s", result);
        
        result = m3_LinkENV (runtime->modules);
        if (result) FATAL("m3_LinkENV: %s", result);

        //[self View]
        result = m3_LinkBGFX (runtime->modules, whandle);
        if (result) FATAL("m3_LinkBGFX: %s", result);
        
        result = m3_InitMallocFunc(runtime);
        if (result) FATAL("m3_InitMallocFunc: %s", result);

        //m3_InitMallocFunc
        result = repl_call(runtime, "_start");
        
        return result;
    }

}


@implementation View

+ (Class)layerClass
{
#ifdef HAS_METAL_SDK
    Class metalClass = NSClassFromString(@"CAMetalLayer");    //is metal runtime sdk available
    if ( metalClass != nil)
    {
        m_device = MTLCreateSystemDefaultDevice(); // is metal supported on this device (is there a better way to do this - without creating device ?)
        if (m_device)
        {
            //[m_device retain];
            return metalClass;
        }
    }
#endif

    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)rect
{
    self = [super initWithFrame:rect];

    if (nil == self)
    {
        return nil;
    }
    

    return self;
}

- (void)layoutSubviews
{
    uint32_t frameW = (uint32_t)(self.contentScaleFactor * self.frame.size.width);
    uint32_t frameH = (uint32_t)(self.contentScaleFactor * self.frame.size.height);
    //s_ctx->m_eventQueue.postSizeEvent(s_defaultWindow, frameW, frameH);
}

- (void)start
{
    if (nil == m_displayLink)
    {
        m_displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
        //[m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        //        [m_displayLink addToRunLoop:[NSRunLoop currentRunLoop]];
        [m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        //[m_displayLink setPreferredFramesPerSecond:60];
    }
}

- (void)stop
{
    if (nil != m_displayLink)
    {
        [m_displayLink invalidate];
        m_displayLink = nil;
    }
}

- (void)renderFrame
{
    runAnimationFrame(0.0);
    
   // bgfx::renderFrame();
}

@end


@interface ViewController ()

@end

@implementation ViewController

- (void)loadView {
     CGRect  screenRect = [[UIScreen mainScreen] bounds];
    View* gameView = [[View alloc] initWithFrame: screenRect];
    //[myView addSubview:gameView];
    self.view = gameView;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    M3Result result = m3Err_none;
    
    IM3Environment env = m3_NewEnvironment ();
    IM3Runtime runtime = NULL;

    result = repl_init(env, &runtime);
    result = repl_load(runtime, "", (__bridge void*) self.view.layer);
    if (result) FATAL("repl_load: %s", result);
    
    printf("Finish Wasm3 Init!!");
    
    [(View *)self.view  start];
}


@end
