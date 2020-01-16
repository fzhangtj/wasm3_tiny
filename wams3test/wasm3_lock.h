//
//  Wasm3Lock.hpp
//  wams3test
//
//  Created by fzhang on 16/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef Wasm3Lock_hpp
#define Wasm3Lock_hpp


# if defined(__cplusplus)
extern "C" {
# endif
    int wasm3_lock_init();
    int wasm3_lock();
    int wasm3_unlock();
#if defined(__cplusplus)
}

# endif
#endif /* Wasm3Lock_hpp */
