#pragma once

#include "Python.h"
#include <string>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <array>
#include <functional>
#include <cassert>

#define PYFIELD(type,name)\
    void set_##name(type value) { setattr<type>(#name, value); }\
    type get_##name() { return getattr<type>(#name); }

#define PYFIELD_READ(type,name)\
    type get_##name() { return getattr<type>(#name); }

#define PYFIELD_STRINGENUM(type,name)\
    void set_##name(type value) { setattr<std::string>(#name, std::string(magic_enum::enum_name<type>(value)));}\
    type get_##name() { return magic_enum::enum_cast<type>(getattr<std::string>(#name)).value(); }

#define PYFIELD_STRINGENUM_READ(type,name)\
    type get_##name() { return magic_enum::enum_cast<type>(getattr<std::string>(#name)).value(); }

namespace bxx
{
    class python_object;
    class python_dict;
    class python_tuple;
    class python_list;

    template <typename T>
    T py2cxx(PyObject* obj)
    {
        return T(obj);
    }

    PyObject* cxx2py(std::string const&, bool theft = false);
    PyObject* cxx2py(char const*, bool theft = false);
    PyObject* cxx2py(std::uint32_t, bool theft = false);
    PyObject* cxx2py(std::int32_t, bool theft = false);
    PyObject* cxx2py(std::int64_t, bool theft = false);
    PyObject* cxx2py(std::uint64_t, bool theft = false);
    PyObject* cxx2py(double, bool theft = false);
    PyObject* cxx2py(python_object const& obj, bool theft = false);
    PyObject* cxx2py(python_dict const& obj, bool theft = false);
    PyObject* cxx2py(python_tuple const& obj, bool theft = false);
    PyObject* cxx2py(python_list const& obj, bool theft = false);
    PyObject* cxx2py(PyObject* obj, bool theft = false);

    template <typename T>
    struct kwarg
    {
        std::string key;
        T value;
        kwarg(std::string const& _key, T _value)
            : key(_key)
            , value(_value)
        {}
    };

    class python_object
    {
    public:
        python_object(python_object const& obj)
            : m_obj(obj.m_obj)
        {
            inc_ref();
        }

        python_object() : python_object(Py_None)
        {
            inc_ref();
        }

        python_object(PyObject* obj)
            : m_obj(obj){
            inc_ref();
        }

        ~python_object()
        {
            dec_ref();
        }

        size_t ref_count()
        {
            return Py_REFCNT(m_obj);
        }

        void inc_ref()
        {
            Py_IncRef(m_obj);
        }

        void dec_ref()
        {
            Py_DecRef(m_obj);
        }

        std::string str() const
        {
            PyObject* str = PyObject_Str(m_obj);
            if (!str)
            {
                return "<error str>";
            }
            char const* chr = _PyUnicode_AsString(str);
            Py_DecRef(str);
            return std::string(chr);
        }

        std::string repr() const
        {
            PyObject* str = PyObject_Repr(m_obj);
            if (!str)
            {
                return "<error repr>";
            }
            char const* chr = _PyUnicode_AsString(str);
            Py_DecRef(str);
            return std::string(chr);
        }

        void delattr(std::string const& arr) const {
            PyObject_DelAttrString(m_obj, arr.c_str());
        }

        template <typename T>
        typename T getattr(std::string const& arr) const {
            PyObject* obj = PyObject_GetAttrString(m_obj, arr.c_str());
            T value = py2cxx<T>(obj);
            Py_DecRef(obj);
            return value;
        }

        bool hasattr(std::string const& arr) const
        {
            return PyObject_HasAttrString(m_obj, arr.c_str());
        }

        template <typename T>
        void setattr(std::string const& arr, T const& value)
        {
            PyObject_SetAttrString(m_obj, arr.c_str(), cxx2py(value, false));
        }

        // todo: function call result values are leaking memory

        template<typename T, class... Args>
        T call(std::string const& method, Args&&... args)
        {
            std::vector<PyObject*> arg_objs;
            std::vector<kwarg<PyObject*>> kwarg_objs;
            arg_objs.reserve(sizeof...(Args));
            ([&] { convert_fn(args, arg_objs, kwarg_objs); }(), ...);

            PyObject* args = PyTuple_New(arg_objs.size());
            if (!args)
            {
                throw std::runtime_error("Unable to create PyTuple");
            }

            PyObject* kwargs = nullptr;
            if (kwarg_objs.size() > 0)
            {
                kwargs = PyDict_New();
                for (kwarg<PyObject*> kw : kwarg_objs)
                {
                    PyDict_SetItemString(kwargs, kw.key.c_str(), kw.value);
                }
            }

            for (size_t i = 0; i < arg_objs.size();++i)
            {
                PyTuple_SetItem(args, i, arg_objs[i]);
            }

            PyObject* fn = PyObject_GetAttrString(m_obj, method.c_str());
            PyObject* res = PyObject_Call(fn,args,kwargs);

            if (kwargs)
                Py_DecRef(kwargs);
            Py_DecRef(args);
            Py_DecRef(fn);
            
            T value = py2cxx<T>(res);
            Py_DecRef(res);
            return value;
        }

        template<typename T>
        T call(std::string const& method)
        {
            PyObject* fn = PyObject_GetAttrString(m_obj, method.c_str());
            if (!fn)
            {
                throw std::runtime_error("failed to get method " + method + " from python object");
            }

            PyObject* tup = PyTuple_New(0);

            if (!tup)
            {
                Py_DecRef(fn);
                throw std::runtime_error("failed to create tuple for object");
            }

            PyObject* res = PyObject_Call(fn, tup, nullptr);
            Py_DecRef(fn);
            Py_DecRef(tup);
            T value = py2cxx<T>(res);
            return value;
        }

        // conversions
        template <typename T> bool is() { return false; }
        template <> bool is<std::string>() { return m_obj && PyBytes_Check(m_obj); }
        template <> bool is<std::uint64_t>() { return m_obj && PyLong_Check(m_obj); }
        template <> bool is<std::int64_t>() { return m_obj && PyLong_Check(m_obj); }
        template <> bool is<std::uint32_t>() { return m_obj && PyLong_Check(m_obj); }
        template <> bool is<std::int32_t>() { return m_obj && PyLong_Check(m_obj); }
        template <> bool is<double>() { return m_obj && PyFloat_Check(m_obj); }
        template <> bool is<python_tuple>() { return m_obj && PyTuple_Check(m_obj); };
        template <> bool is<python_list>() { return m_obj && PyList_Check(m_obj); };
        template <> bool is<python_dict>() { return m_obj && PyDict_Check(m_obj); };
        template <> bool is<python_object>() { return m_obj; };
        PyObject* m_obj;
    protected:
        python_object(PyObject* obj, bool is_new)
            : m_obj(obj)
        {
            if (!is_new)
            {
                inc_ref();
            }
        }
    private:
        template <typename T>
        PyObject* convert_fn(T value, std::vector<PyObject*>& args, std::vector<kwarg<PyObject*>>& kwargs)
        {
            return args.emplace_back(cxx2py(value,true));
        }

        template <typename T>
        PyObject* convert_fn(kwarg<T> value, std::vector<PyObject*>& args, std::vector<kwarg<PyObject*>>& kwargs)
        {
            PyObject* v = cxx2py(value.value,false);
            kwargs.emplace_back(kwarg<PyObject*>(value.key, v));
            return v;
        }
    };

    class python_tuple : public python_object
    {
    public:
        python_tuple(PyObject* obj)
            : python_object(obj) {}

        python_tuple(size_t size)
            : python_object(PyTuple_New(size), true)
        {
        }

        python_tuple() = delete;

        size_t len() const
        {
            return PyTuple_Size(m_obj);
        }

        template <typename T>
        T get(size_t index) const
        {
            return py2cxx<T>(PyTuple_GetItem(m_obj, index));
        }

        template <typename T>
        void set(size_t index, T const& value)
        {
            if (PyTuple_SetItem(m_obj, index, cxx2py(value, true)))
            {
                throw std::runtime_error("Failed to insert tuple object");
            }
        }
    };

    class python_dict : public python_object
    {
    public:
        python_dict(PyObject* obj)
            : python_object(obj) {}

        python_dict()
            : python_object(PyDict_New(), true)
        {}

        size_t len()
        {
            return PyDict_Size(m_obj);
        }

        python_list items();
        python_list keys();
        python_list values();

        template <typename T>
        typename T get(std::string const& key)
        {
            return py2cxx<T>(PyDict_GetItemString(m_obj, key.c_str()));
        }

        template <typename T>
        void set(std::string const& key, T const& value)
        {
            PyDict_SetItemString(m_obj, key.c_str(), cxx2py(value, false));
        }

        void del(std::string const& key)
        {
            PyDict_DelItemString(m_obj, key.c_str());
        }
    };

    class python_list : public python_object
    {
    public:
        python_list()
            : python_object(PyList_New(0), true)
        {}

        python_list(size_t size)
            : python_object(PyList_New(size), true)
        {}

        python_list(int size) // <-- makes it so that python_list(0) resolves as a size and not nullptr
            : python_object(PyList_New(size), true)
        {}

        python_list(std::nullptr_t)
            : python_object()
        {}

        python_list(PyObject* obj)
            : python_object(obj) {}

        size_t len()
        {
            return PyList_Size(m_obj);
        }

        python_tuple to_tuple()
        {
            return python_tuple(PyList_AsTuple(m_obj));
        }

        template <typename T>
        typename T get(size_t index)
        {
            return py2cxx<T>(PyList_GetItem(m_obj, index));
        }

        template <typename T>
        void set(size_t index, T const& value)
        {
            if (PyList_SetItem(m_obj, index, cxx2py(value, true)))
            {
                throw std::runtime_error("Failed to set python list item");
            }
        }

        template <typename T>
        void append(T const& value)
        {
            if (PyList_Append(m_obj, cxx2py(value, false)))
            {
                throw std::runtime_error("Failed to append python list item");
            }
        }
    };

    // Python -> cxx conversions

    template <>
    inline std::string py2cxx(PyObject* py)
    {
        return _PyUnicode_AsString(py);
    }

    template <>
    inline std::uint32_t py2cxx(PyObject* py)
    {
        return PyLong_AsUnsignedLong(py);
    }

    template <>
    inline std::int32_t py2cxx(PyObject* py)
    {
        return PyLong_AsLong(py);
    }

    template <>
    inline std::uint64_t py2cxx(PyObject* py)
    {
        return PyLong_AsUnsignedLongLong(py);
    }

    template <>
    inline std::int64_t py2cxx(PyObject* py)
    {
        return PyLong_AsLongLong(py);
    }

    template <>
    inline double py2cxx(PyObject* py)
    {
        return PyFloat_AsDouble(py);
    }

    template <>
    inline float py2cxx(PyObject* py)
    {
        return float(PyFloat_AsDouble(py));
    }

    template <>
    inline bool py2cxx(PyObject* py)
    {
        return PyObject_IsTrue(py);
    }

    template <>
    inline python_object py2cxx(PyObject* py)
    {
        return python_object(py);
    }

    template <>
    inline python_list py2cxx(PyObject* py)
    {
        return python_list(py);
    }

    template <>
    inline python_dict py2cxx(PyObject* py)
    {
        return python_dict(py);
    }
}