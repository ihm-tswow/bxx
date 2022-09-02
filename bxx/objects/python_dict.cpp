#pragma once

#include <bxx/objects/python_dict.hpp>
#include <bxx/objects/python_list.hpp>

namespace bxx
{
    python_dict::python_dict(PyObject* obj)
        : python_object(obj)
    {}

    python_dict::python_dict()
        : python_object(details::pyobject_steal(PyDict_New()))
    {}

    void python_dict::del(std::string const& key)
    {
        PyDict_DelItemString(get_pyobject(), key.c_str());
    }

    python_list python_dict::items()
    {
        return python_list(PyDict_Items(get_pyobject()));
    }

    python_list python_dict::keys()
    {
        return python_list(PyDict_Keys(get_pyobject()));
    }

    python_list python_dict::values()
    {
        return python_list(PyDict_Values(get_pyobject()));
    }

    size_t python_dict::len()
    {
        return PyDict_Size(get_pyobject());
    }
}