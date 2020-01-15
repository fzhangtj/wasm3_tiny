//
//  M3BxAllocator.hpp
//  wams3test
//
//  Created by fzhang on 13/01/20.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef M3BxAllocator_hpp
#define M3BxAllocator_hpp

#include <stdio.h>
#include <bx/bx.h>
#include <bx/allocator.h>

class M3BxAllocator : public bx::AllocatorI
{
public:
    ///
    M3BxAllocator();

    ///
    virtual ~M3BxAllocator();

    ///
    virtual void* realloc(
          void* _ptr
        , size_t _size
        , size_t _align
        , const char* _file
        , uint32_t _line
        ) override;
};


#endif /* M3BxAllocator_hpp */
