#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Iteration.h>
#include <libruntime/Move.h>
#include <libruntime/Types.h>
#include <libsystem/Assert.h>

namespace libruntime
{

template <typename T>
class Vector
{
private:
    T *_storage;
    size_t _count;
    size_t _capacity;

public:
    size_t count() { return _count; }
    bool empty() { return _count == 0; }
    bool any() { return !empty(); }

    Vector() : Vector(16) {}

    Vector(size_t capacity)
    {
        _count = 0;
        _capacity = capacity;
        _storage = new T[capacity];
    }

    ~Vector()
    {
        clear();

        delete _storage;
    }

    T &operator[](size_t index)
    {
        assert(index < _count);

        return _storage[index];
    }

    void clear()
    {
        for (size_t i = 0; i < _count; i++)
        {
            _storage[i].~T();
        }

        _count = 0;
    }

    template <typename Callback>
    void foreach (Callback callback)
    {
        for (size_t i = 0; i < _count; i++)
        {
            if (callback(_storage[i]) == Iteration::STOP)
            {
                return;
            }
        }
    }

    template <typename Comparator>
    void sort(Comparator comparator)
    {
        for (size_t i = 0; i < _count - 1; i++)
        {
            for (size_t j = i + 1; j < _count; j++)
            {
                if (comparator(_storage[i], _storage[j]) > 0)
                {
                    swap(_storage[i], _storage[j]);
                }
            }
        }
    }

    void grow()
    {
        if (_count < _capacity)
        {
            size_t new_capacity = _capacity + _capacity / 4;
            T *new_storage = new T[new_capacity];

            for (size_t i = 0; i < _count; i++)
            {
                new_storage[i] = move(_storage[i]);
            }

            delete _storage;
            _storage = new_storage;
            _capacity = new_capacity;
        }

        _count++;
    }

    void shrink()
    {
        _count--;

        if (_count < _capacity - _capacity / 4)
        {
            size_t new_capacity = _capacity - _capacity / 4;
            T *new_storage = new T[new_capacity];

            for (size_t i = 0; i < _count; i++)
            {
                new_storage[i] = move(_storage[i]);
            }

            delete _storage;
            _storage = new_storage;
            _capacity = new_capacity;
        }
    }

    void insert(size_t index, T value)
    {
        grow();

        for (size_t j = _count; j > index; j--)
        {
            _storage[j] = move(_storage[j - 1]);
        }

        _storage[index] = move(value);
    }

    void insert_sorted(T value)
    {
        size_t insert_index = 0;

        for (size_t i = 0; i < _count; i++)
        {
            if (_storage[i] < value)
            {
                insert_index = i;
            }
            else
            {
                break;
            }
        }

        insert(insert_index, value);
    }

    template <typename Comparator>
    void insert_sorted(T value, Comparator comparator)
    {
        size_t insert_index = 0;

        for (size_t i = 0; i < _count; i++)
        {
            if (comparator(_storage[i], value))
            {
                insert_index = i;
            }
            else
            {
                break;
            }
        }

        insert(insert_index, value);
    }

    void remove(size_t index)
    {
        _storage[index].~T();

        for (size_t j = index + 1; j < _count; j++)
        {
            _storage[j - 1] = move(_storage[j]);
        }

        shrink();
    }

    void remove(T &value)
    {
        for (size_t i = 0; i < _count; i++)
        {
            if (_storage[i] == value)
            {
                remove(i);
                return;
            }
        }
    }

    void remove_all(T &value)
    {
        for (size_t i = 0; i < _count; i++)
        {
            if (_storage[i] == value)
            {
                remove(i);
            }
        }
    }

    void push(T value)
    {
        insert(0, value);
    }

    void push_back(T value)
    {
        insert(_count, value);
    }

    T pop()
    {
        assert(_count > 0);

        T value = move(_storage[0]);

        remove(0);

        return value;
    }

    T pop_back()
    {
        assert(_count > 0);

        T value = move(_storage[_count - 1]);

        remove(_count - 1);

        return value;
    }

    T &peek()
    {
        assert(_count > 0);
        return _storage[0];
    }

    T &peek_back()
    {
        assert(_count > 0);
        return _storage[_count - 1];
    }

    bool containe(T value)
    {
        for (size_t i = 0; i < _count; i++)
        {
            if (_storage[i] == value)
            {
                return true;
            }
        }

        return false;
    }
};

} // namespace libruntime
