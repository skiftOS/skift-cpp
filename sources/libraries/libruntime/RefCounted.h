#pragma once

#include <libruntime/Assert.h>

namespace libruntime
{

template <typename T>
class RefCounted
{
private:
    int _refcount;

public:
    RefCounted()
    {
        _refcount = 1;
    }

    ~RefCounted()
    {
        assert(_refcount == 0);
    }

    void ref()
    {
        assert(_refcount > 0);

        _refcount++;
    }

    void deref()
    {
        assert(_refcount > 0);

        _refcount--;

        if (_refcount == 0)
        {
            delete static_cast<T *>(this);
        }
    }

    int refcount()
    {
        return _refcount;
    }
};

} // namespace libruntime
