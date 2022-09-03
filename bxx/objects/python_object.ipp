#pragma once

#include <bxx/objects/python_object.hpp>
#include <common/shared_functions.hpp>
#include <bxx/mathutils.hpp>

#include <fmt/core.h>

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

    template <typename pyref_type>
    template <typename T, class ... Args>
    T python_object_base<pyref_type>::call(std::string const& method, Args&&... args)
    {
        pyref_type self = get_pyobject();
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

        PyObject* fn = PyObject_GetAttrString(self, method.c_str());
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

    template <typename pyref_type>
    template<typename T>
    inline T python_object_base<pyref_type>::call(std::string const& method)
    {
        pyref_type self = get_pyobject();
        PyObject* fn = PyObject_GetAttrString(self, method.c_str());
        PyObject* tup = PyTuple_New(0);
        PyObject* res = PyObject_Call(fn, tup, nullptr);
        Py_DecRef(fn);
        Py_DecRef(tup);
        T value(details::py2cxx<T>(res));
        Py_DecRef(res);
        return value;
    }

    template <typename pyref_type>
    template <typename T>
    inline T python_object_base<pyref_type>::getattr(std::string const& arr)
    {
        pyref_type self = get_pyobject();
        PyObject* obj = PyObject_GetAttrString(self, arr.c_str());
        T value(details::py2cxx<T>(obj));
        Py_DecRef(obj);
        return value;
    }

    template <typename pyref_type>
    template <typename T>
    void python_object_base<pyref_type>::setattr(std::string const& arr, T value)
    {
        pyref_type self = get_pyobject();
        PyObject_SetAttrString(self, arr.c_str(), details::cxx2py(value, false));
    }

    template <typename pyref_type>
    size_t python_object_base<pyref_type>::ref_count()
    {
        return Py_REFCNT(get_pyobject());
    }

    template <typename pyref_type>
    std::string python_object_base<pyref_type>::str()
    {
        pyref_type self = get_pyobject();
        PyObject* str = PyObject_Str(self);
        if (!str)
        {
            return "<error str>";
        }
        char const* chr = _PyUnicode_AsString(str);
        Py_DecRef(str);
        return std::string(chr);
    }

    template <typename pyref_type>
    std::string python_object_base<pyref_type>::repr()
    {
        pyref_type self = get_pyobject();
        PyObject* str = PyObject_Repr(self);
        if (!str)
        {
            return "<error repr>";
        }
        char const* chr = _PyUnicode_AsString(str);
        Py_DecRef(str);
        return std::string(chr);
    }

    template <typename pyref_type>
    void python_object_base<pyref_type>::delattr(std::string const& arr)
    {
        PyObject_DelAttrString(get_pyobject(), arr.c_str());
    }

    template <typename pyref_type>
    bool python_object_base<pyref_type>::hasattr(std::string const& arr)
    {
        return PyObject_HasAttrString(get_pyobject(), arr.c_str());
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

        template <> inline mathutils::vec2 py2cxx<mathutils::vec2>(PyObject* obj)
        {
            return {
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,0))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,1)))
            };
        }

        template <> inline mathutils::vec3 py2cxx<mathutils::vec3>(PyObject* obj)
        {
            return {
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,0))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,1))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,2)))
            };
        }

        template <> inline mathutils::quaternion py2cxx<mathutils::quaternion>(PyObject* obj)
        {
            return {
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,0))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,1))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,2))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,3)))
            };
        }

        template <> inline mathutils::rgba py2cxx<mathutils::rgba>(PyObject* obj)
        {
            return {
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,0))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,1))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,2))),
                float(PyFloat_AsDouble(PyTuple_GetItem(obj,3)))
            };
        }
    }

    template <typename ...Args>
    inline PyObject* eval_pyobject_raw(fmt::format_string<Args...> str, Args... args)
    {
        return get_pointers()->cy_eval_pyobject(
            const_cast<char*>(fmt::format(str, std::forward<Args>(args)...).c_str())
        );
    }
}