#pragma once

#include <bxx/objects/python_set.hpp>

namespace bxx
{
    template <typename T>
    void python_set::add(T value)
    {
        PySet_Add(get_pyobject(), details::cxx2py(value, false));
    }

    template <typename T>
    bool python_set::contains(T value)
    {
        return PySet_Contains(get_pyobject(), details::cxx2py(value));
    }
}