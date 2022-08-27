#include "python_object.hpp"

namespace bxx
{
    python_object cxx2py(python_object obj)
    {
        return obj;
    }

    python_object cxx2py(python_list obj)
    {
        return python_object(obj.m_obj);
    }

    python_object cxx2py(python_tuple obj)
    {
        return python_object(obj.m_obj);
    }

    python_object cxx2py(python_dict dict)
    {
        return python_object(dict.m_obj);
    }

    python_object cxx2py(std::string const& str)
    {
        return python_object(PyUnicode_FromString(str.c_str()));
    }

    python_object cxx2py(std::uint32_t val)
    {
        return python_object(PyLong_FromUnsignedLong(val));
    }

    python_object cxx2py(std::int32_t val)
    {
        return python_object(PyLong_FromLong(val));
    }

    python_object cxx2py(std::int64_t val)
    {
        return python_object(PyLong_FromLongLong(val));
    }

    python_object cxx2py(std::uint64_t val)
    {
        return python_object(PyLong_FromUnsignedLongLong(val));
    }

    python_object cxx2py(double val)
    {
        return python_object(PyFloat_FromDouble(val));
    }

    py_ref<python_list> create_python_list(size_t size)
    {
        return py_ref<python_list>(python_list(PyList_New(size)));
    }

    py_ref<python_tuple> create_python_tuple(size_t size)
    {
        return py_ref<python_tuple>(python_tuple(PyTuple_New(size)));
    }

    py_ref<python_dict> create_python_dict()
    {
        return py_ref<python_dict>(python_dict(PyDict_New()));
    }
}