#pragma once

#include <bxx/objects/python_tuple.hpp>

namespace bxx
{
    template <typename T>
    void python_tuple::set(size_t index, T value)
    {
        PyTuple_SetItem(get_pyobject(), index, details::cxx2py(value, true).m_pyobj);
    }

    template <typename T>
    T python_tuple::get(size_t index)
    {
        PyObject* obj = PyTuple_GetItem(this->get_pyobject(), index);
        T value(details::py2cxx<T>(obj));
        return value;
    }
}