#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libc/string.h>
#include <libruntime/Types.h>

#include "system/memory/Memory.h"

namespace system::tasking
{

class Stack
{
private:
    uintptr_t _ptr;
    memory::MemoryRegion _region;

public:
    Stack(size_t how_many_pages)
    {
        _region = memory::alloc_region(how_many_pages);
        _ptr = _region.end_address();
    }

    ~Stack()
    {
        memory::free_region(_region);
    }

    template <typename T>
    uintptr_t push(T value)
    {
        return push<T>(&value);
    }

    template <typename T>
    uintptr_t push(T *object)
    {
        _ptr -= sizeof(T);
        libc::memcpy(reinterpret_cast<void *>(_ptr), object, sizeof(T));

        return _ptr;
    }

    uintptr_t get_pointer(void)
    {
        return _ptr;
    }

    void set_pointer(uintptr_t ptr)
    {
        _ptr = ptr;
    }
};

} // namespace system::tasking
