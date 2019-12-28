#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace libruntime
{

template <typename T>
class OwnPtr
{
private:
    T *_ptr = nullptr;

public:
    OwnPtr() {}
    OwnPtr(T *ptr) : _ptr(ptr) {}

    ~OwnPtr()
    {
        if (_ptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
    }

    OwnPtr(OwnPtr &other) : _ptr(other.give_ref()) {}

    template <typename U>
    OwnPtr(OwnPtr<U> &other) : _ptr(static_cast<U *>(other.give_ref())) {}

    OwnPtr(OwnPtr &&other) : _ptr(other.give_ref()) {}

    template <typename U>
    OwnPtr(OwnPtr<U> &&other) : _ptr(static_cast<U *>(other.give_ref())) {}

    OwnPtr &operator=(OwnPtr &other)
    {
        if (necked() != other.necked())
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }

    template <typename U>
    OwnPtr &operator=(OwnPtr<U> &other)
    {
        if (necked() != other.necked())
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }

    OwnPtr &operator=(OwnPtr &&other)
    {
        if (this != &other)
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }

    template <typename U>
    OwnPtr &operator=(OwnPtr<U> &&other)
    {
        if (this != static_cast<void *>(&other))
        {
            if (_ptr)
            {
                delete _ptr;
            }

            _ptr = other.give_ref();
        }

        return *this;
    }

    T *operator->() const
    {
        return _ptr;
    }

    T &operator*() { return *_ptr; }

    const T &operator*() const { return *_ptr; }

    bool operator==(OwnPtr<T> other) const
    {
        return _ptr == other._ptr;
    }

    template <typename U>
    bool operator==(OwnPtr<U> other) const
    {
        return _ptr == static_cast<U *>(other._ptr);
    }

    operator bool() const
    {
        return _ptr != nullptr;
    }

    bool operator!() const
    {
        return _ptr == nullptr;
    }

    T *give_ref()
    {
        auto ref = _ptr;
        _ptr = nullptr;

        return ref;
    }

    T *necked()
    {
        return _ptr;
    }
};

template <typename Type, typename... Args>
inline OwnPtr<Type> own(Args &&... args)
{
    return OwnPtr<Type>(new Type(args...));
}

} // namespace libruntime
