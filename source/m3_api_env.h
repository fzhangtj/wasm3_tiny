//
//  m3_api_env.h
//  wams3test
//
//  Created by kg on 2020/1/11.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef m3_api_env_h
#define m3_api_env_h

#include "m3_core.h"

# if defined(__cplusplus)
extern "C" {
# endif

    M3Result    m3_LinkENV     (IM3Module module);

    void runAnimationFrame(f64 timeStamp);

#if defined(__cplusplus)
}
# endif

#endif /* m3_api_env_h */
