#pragma once

#include <bxx/objects/python_list.hpp>

namespace bxx
{
    template <typename T>
    void python_list::set(size_t index, T value)
    {
        PyList_SetItem(get_pyobject(), index, details::cxx2py(value, true).m_pyobj);
    }

    template <typename T>
    T python_list::get(size_t index)
    {
        PyObject* obj = PyList_GetItem(get_pyobject(), index);
        T value(details::py2cxx<T>(obj));
        return value;
    }

    template <typename T>
    void python_list::append(T value)
    {
        PyList_Append(get_pyobject(), details::cxx2py(value, false).m_pyobj);
    }
}