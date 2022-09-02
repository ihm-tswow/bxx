#pragma once

#include <bxx/objects/python_object.hpp>

#include <array>
#include <optional>

namespace bxx
{
    template <typename T>
    kwarg<T>::kwarg(std::string const& key, T value)
        : m_key(key)
        , m_value(value)
    {}

    namespace details
    {
        template <size_t size>
        struct call_arg_stack
        {
            std::array<std::optional<python_tempref>, size> m_refs;
            size_t m_size = 0;
            template <typename T>
            void add(T value)
            {
                m_refs[m_size++].emplace(cxx2py(value, true));
            }
        };

        template <size_t size>
        struct call_kwarg_stack : public call_arg_stack<size>
        {
            std::array<std::string,size> m_names;

            template <typename T>
            void add_pair(std::string const& str, T value)
            {
                m_names[this->m_size] = str;
                this->m_refs[this->m_size++].emplace(cxx2py(value, false)); // dictionaries don't steal references
            }
        };

        template <typename T, size_t size>
        void convert_fn(T value, call_arg_stack<size>& args, call_kwarg_stack<size>&)
        {
            args.add(value);
        }

        template <typename T, size_t size>
        void convert_fn(kwarg<T> value, call_arg_stack<size>&, call_kwarg_stack<size>& kwargs)
        {
            kwargs.add_pair(value.m_key, value.m_value);
        }
    }

    template <typename T>
    python_object::python_object(T value)
        : python_object(details::cxx2py(value,true).m_pyobj)
    {
    }

    template <typename T, class ... Args>
    T python_object::call(std::string const& method, Args&&... args) const
    {
        details::call_arg_stack<sizeof...(Args)> arg_refs;
        details::call_kwarg_stack<sizeof...(Args)> kwarg_refs;
        ([&] { details::convert_fn(args, arg_refs, kwarg_refs); }(), ...);

        PyObject* arg_tup = PyTuple_New(arg_refs.m_size);
        PyObject* kwarg_dict = nullptr;
        if (kwarg_refs.m_size)
        {
            kwarg_dict = PyDict_New();
            for (size_t i = 0; i < kwarg_refs.m_size; ++i)
            {
                PyDict_SetItemString(kwarg_dict, kwarg_refs.m_names[i].c_str(), kwarg_refs.m_refs[i].value().m_pyobj);
            }
        }

        for (size_t i = 0; i < arg_refs.m_size; ++i)
        {
            PyTuple_SetItem(arg_tup, i, arg_refs.m_refs[i].value().m_pyobj);
        }

        PyObject* fn = PyObject_GetAttrString(get_pyobject(), method.c_str());
        PyObject* res = PyObject_Call(fn, arg_tup, kwarg_dict);
        T value(details::py2cxx<T>(res));
        Py_DecRef(res);
        Py_DecRef(fn);
        Py_DecRef(arg_tup);
        if (kwarg_dict)
        {
            Py_DecRef(kwarg_dict);
        }
        return value;
    }

    template<typename T>
    inline T python_object::call(std::string const& method) const
    {
        PyObject* fn = PyObject_GetAttrString(m_pyobject, method.c_str());
        PyObject* tup = PyTuple_New(0);
        PyObject* res = PyObject_Call(fn, tup, nullptr);
        Py_DecRef(fn);
        Py_DecRef(tup);
        T value(details::py2cxx<T>(res));
        Py_DecRef(res);
        return value;
    }

    template <typename T>
    inline T python_object::getattr(std::string const& arr) const
    {
        PyObject* obj = PyObject_GetAttrString(m_pyobject, arr.c_str());
        T value(details::py2cxx<T>(obj));
        Py_DecRef(obj);
        return value;
    }

    template <typename T>
    void python_object::setattr(std::string const& arr, T value)
    {
        PyObject_SetAttrString(m_pyobject, arr.c_str(), details::cxx2py(value, false));
    }

    template <> inline bool python_object::is<bool>()
    {
        return PyBool_Check(get_pyobject());
    }

    template <> inline bool python_object::is<std::string>()
    {
        return PyUnicode_Check(get_pyobject());
    }

    template <> inline bool python_object::is<std::uint32_t>()
    {
        return PyLong_Check(get_pyobject());
    }

    template <> inline bool python_object::is<std::int32_t>()
    {
        return PyLong_Check(get_pyobject());
    }

    template <> inline bool python_object::is<std::uint64_t>()
    {
        return PyLong_Check(get_pyobject());
    }

    template <> inline bool python_object::is<std::int64_t>()
    {
        return PyLong_Check(get_pyobject());
    }

    template <> inline bool python_object::is<float>()
    {
        return PyFloat_Check(get_pyobject());
    }

    template <> inline bool python_object::is<double>()
    {
        return PyFloat_Check(get_pyobject());
    }

    template <typename T>
    python_object& python_object::operator=(T value)
    {
        set_object(cxx2py(value));
        return *this;
    }

    namespace details
    {
        template <typename T>
        T py2cxx(PyObject* obj)
        {
            return T(obj);
        }

        template <> inline std::string py2cxx<std::string>(PyObject* obj)
        {
            return _PyUnicode_AsString(obj);
        }

        template <> inline std::uint32_t py2cxx<std::uint32_t>(PyObject* obj)
        {
            return PyLong_AsUnsignedLong(obj);
        }

        template <> inline std::int32_t py2cxx<std::int32_t>(PyObject* obj)
        {
            return PyLong_AsLong(obj);
        }

        template <> inline std::uint64_t py2cxx<std::uint64_t>(PyObject* obj)
        {
            return PyLong_AsUnsignedLongLong(obj);
        }

        template <> inline std::int64_t py2cxx<std::int64_t>(PyObject* obj)
        {
            return PyLong_AsLongLong(obj);
        }

        template <> inline float py2cxx<float>(PyObject* obj)
        {
            return static_cast<float>(PyFloat_AsDouble(obj));
        }

        template <> inline double py2cxx<double>(PyObject* obj)
        {
            return PyFloat_AsDouble(obj);
        }
    }
}