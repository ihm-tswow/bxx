#pragma once

#include "Python.h"
#include <string>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <array>
#include <functional>

namespace bxx
{
    template <typename T>
    struct kwarg
    {
        std::string key;
        T value;
        kwarg(std::string const& _key, T const& _value)
            : key(_key)
            , value(_value)
        {}
    };

    class python_object;

    template <size_t size>
    class pyobject_stack
    {
    public:
        pyobject_stack() = default;
        pyobject_stack(pyobject_stack const& stack) = delete;
        ~pyobject_stack()
        {
            reset();
        }

        PyObject* convert(PyObject* value)
        {
            return value;
        }

        PyObject* convert(python_object const& value);


        PyObject* convert(char const* value)
        {
            return add(PyBytes_FromString(value));
        }

        PyObject* convert(std::string const& value)
        {
            return add(PyBytes_FromString(value.c_str()));
        }

        PyObject* convert(std::uint64_t value)
        {
            return add(PyLong_FromUnsignedLongLong(value));
        }

        PyObject* convert(std::int64_t value)
        {
            return add(PyLong_FromLongLong(value));
        }

        PyObject* convert(std::uint32_t value)
        {
            return add(PyLong_FromUnsignedLong(value));
        }

        PyObject* convert(std::int32_t value)
        {
            return add(PyLong_FromLong(value));
        }

        template <typename T>
        PyObject* convert_fn(T const& value, std::vector<PyObject*>& args, std::vector<kwarg<PyObject*>>& kwargs)
        {
            return args.emplace_back(convert(value));
        }

        template <typename T>
        PyObject* convert_fn(kwarg<T> const& value, std::vector<PyObject*>& args, std::vector<kwarg<PyObject*>>& kwargs)
        {
            PyObject* v = convert(value.value);
            kwargs.emplace_back(kwarg<PyObject*>(value.key,v));
            return v;
        }

        void reset()
        {
            for (size_t i = 0; i < m_idx; ++i)
            {
                Py_DecRef(m_objects[i]);
            }
            m_idx = 0;
        }
    protected:
        PyObject* add(PyObject* obj)
        {
            m_objects[m_idx++] = obj;
            return obj;
        }
        std::array<PyObject*, size> m_objects;
        size_t m_idx = 0;
    };

    class python_dictionary;
    class python_tuple;
    class python_list;

    class python_object
    {
    public:
        python_object(PyObject* obj)
            : m_obj(obj){
            if (obj)
            {
                Py_IncRef(m_obj);
            }
        }

        ~python_object()
        {
            if (m_obj)
            {
                Py_DecRef(m_obj);
            }
        }

        size_t ref_count()
        {
            return Py_REFCNT(m_obj);
        }

        template <typename T>
        T getattr(std::string const& arr) {
            return T(python_object(PyObject_GetAttrString(m_obj, arr.c_str()))); 
        }

        template <typename T>
        void setattr(std::string const& arr, T value)
        {
            pyobject_stack<1> stack;
            PyObject* obj = stack.convert(value);
            PyObject_SetAttrString(m_obj, arr.c_str(), obj);
        }

        template<class... Args>
        python_object call(std::string const& method, Args&&... args)
        {
            std::vector<PyObject*> arg_objs;
            std::vector<kwarg<PyObject*>> kwarg_objs;
            arg_objs.reserve(sizeof...(Args));
            pyobject_stack<sizeof...(Args)> stack;
            ([&] { stack.convert_fn(args, arg_objs, kwarg_objs); }(), ...);

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

            return python_object(res);
        }

        template<>
        python_object call(std::string const& method)
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
            return python_object(res);
        }

        // conversions
        template <typename T> T as();
        template <> std::string as() { return std::string(PyBytes_AS_STRING(m_obj)); }
        template <> std::uint64_t as() { return std::uint64_t(PyLong_AsUnsignedLongLong(m_obj)); }
        template <> std::int64_t as() { return std::uint64_t(PyLong_AsLongLong(m_obj)); }
        template <> double as() { return std::uint64_t(PyFloat_AsDouble(m_obj)); }
        template <> python_object as();
        template <> python_dictionary as();
        template <> python_tuple as();
        template <> python_list as();

        template <typename T> bool is() { return false; }
        template <> bool is<std::string>() { return m_obj && PyBytes_Check(m_obj); }
        template <> bool is<std::uint64_t>() { return m_obj && PyLong_Check(m_obj); }
        template <> bool is<std::int64_t>() { return m_obj && PyLong_Check(m_obj); }
        template <> bool is<double>() { return m_obj && PyFloat_Check(m_obj); }
        template <> bool is<python_tuple>() { return m_obj && PyTuple_Check(m_obj); };
        template <> bool is<python_list>() { return m_obj && PyList_Check(m_obj); };
        template <> bool is<python_dictionary>() { return m_obj && PyDict_Check(m_obj); };
        // all python_objects are python_objects, we just don't always treat them as such
        template <> bool is<python_object>() { return m_obj; };
        operator long() const;
        operator bool() const;
        operator double() const;
        operator std::string() const;
        operator python_object();
    protected:
        PyObject* m_obj;
    };

    class python_tuple : public python_object
    {
    public:
        python_tuple(PyObject* obj)
            : python_object(obj) {}

        size_t len()
        {
            return PyTuple_Size(m_obj);
        }

        template <typename T>
        T get(size_t index)
        {
            return python_object(PyTuple_GetItem(m_obj, index)).as<T>();
        }

        template <typename T>
        void set(size_t index, T const& value)
        {
            pyobject_stack<1> stack;
            PyObject* obj = stack.convert(value);
            PyTuple_SetItem(m_obj, index, obj);
        }
    };

    class python_dictionary : public python_object
    {
    public:
        python_dictionary(PyObject* obj)
            : python_object(obj) {}

        size_t len()
        {
            return PyDict_Size(m_obj);
        }

        template <typename T>
        T get(std::string const& key)
        {
            PyObject* obj = PyDict_GetItemString(m_obj, key.c_str());
            return python_object(obj).as<T>();
        }

        template <typename T>
        void set(std::string const& key, T const& value)
        {
            pyobject_stack<1> stack;
            PyObject* obj = stack.convert(value);
            PyDict_SetItemString(m_obj,key.c_str(), obj);
        }

        void remove(std::string const& key)
        {
            PyDict_DelItemString(m_obj,key.c_str());
        }
    };

    class python_list : public python_object
    {
    public:
        python_list(PyObject* obj)
            : python_object(obj) {}

        size_t len()
        {
            return PyList_Size(m_obj);
        }

        template <typename T>
        T get(size_t index)
        {
            return python_object(PyList_GetItem(m_obj, index)).as<T>();
        }

        template <typename T>
        void set(size_t index, T const& value)
        {
            pyobject_stack<1> stack;
            PyObject* obj = stack.convert(value);
            PyList_SetItem(m_obj, index, obj);
        }

        template <typename T>
        void append(T const& value)
        {
            pyobject_stack<1> stack;
            PyObject* obj = stack.convert(value);
            PyList_Append(m_obj, obj);
        }
    };

    template <>
    inline python_object python_object::as()
    {
        return python_object(m_obj);
    }

    template <>
    inline python_dictionary python_object::as()
    {
        return python_dictionary(m_obj);
    }

    template <>
    inline python_tuple python_object::as()
    {
        return python_tuple(m_obj);
    }

    template <>
    inline python_list python_object::as()
    {
        return python_list(m_obj);
    }

    template <size_t size>
    PyObject* pyobject_stack<size>::convert(python_object const& value)
    {
        return value.m_obj;
    }

}