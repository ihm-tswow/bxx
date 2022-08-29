#include "python_object.hpp"

namespace bxx
{
    PyObject* cxx2py(python_object const& obj, bool theft)
    {
        if (theft)
        {
            Py_IncRef(obj.m_obj);
        }
        return obj.m_obj;
    }

    PyObject* cxx2py(python_list const& obj, bool theft)
    {
        if (theft)
        {
            Py_IncRef(obj.m_obj);
        }
        return (obj.m_obj);
    }

    PyObject* cxx2py(python_tuple const& obj, bool theft)
    {
        if (theft)
        {
            Py_IncRef(obj.m_obj);
        }
        return (obj.m_obj);
    }

    PyObject* cxx2py(python_dict const& dict, bool theft)
    {
        if (theft)
        {
            Py_IncRef(dict.m_obj);
        }
        return dict.m_obj;
    }

    PyObject* cxx2py(std::string const& str,bool)
    {
        return PyUnicode_FromString(str.c_str());
    }

    PyObject* cxx2py(char const* str,bool)
    {
        return PyUnicode_FromString(str);
    }

    PyObject* cxx2py(std::uint32_t val, bool)
    {
        return PyLong_FromUnsignedLong(val);
    }

    PyObject* cxx2py(std::int32_t val, bool)
    {
        return PyLong_FromLong(val);
    }

    PyObject* cxx2py(std::int64_t val, bool)
    {
        return PyLong_FromLongLong(val);
    }

    PyObject* cxx2py(std::uint64_t val, bool)
    {
        return PyLong_FromUnsignedLongLong(val);
    }

    PyObject* cxx2py(double val, bool)
    {
        return PyFloat_FromDouble(val);
    }

    PyObject* cxx2py(PyObject* obj, bool is_theft)
    {
        if (is_theft)
        {
            Py_IncRef(obj);
        }
        return obj;
    }
}