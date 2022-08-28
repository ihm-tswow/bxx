#pragma once

#include "Python.h"
#include <string>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <array>
#include <functional>
#include <cassert>

namespace bxx
{
    class python_object;
    class python_dict;
    class python_tuple;
    class python_list;
    template <typename T>
    class py_ref;

    // todo: should be possible with disjunctions, but didn't get it to work
    template<class T> struct py2cxx_type;
    template<> struct py2cxx_type<std::string>       { typedef std::string type; };
    template<> struct py2cxx_type<std::uint32_t>     { typedef std::uint32_t type; };
    template<> struct py2cxx_type<std::int32_t>      { typedef std::int32_t type; };
    template<> struct py2cxx_type<std::uint64_t>     { typedef std::uint64_t type; };
    template<> struct py2cxx_type<std::int64_t>      { typedef std::int64_t type; };
    template<> struct py2cxx_type<bool>              { typedef bool type; };
    template<> struct py2cxx_type<double>            { typedef double type; };
    template<> struct py2cxx_type<python_object>     { typedef py_ref<python_object> type; };
    template<> struct py2cxx_type<python_list>       { typedef py_ref<python_list> type; };
    template<> struct py2cxx_type<python_tuple>      { typedef py_ref<python_tuple> type; };
    template<> struct py2cxx_type<python_dict>       { typedef py_ref<python_dict> type; };

    template <typename T>
    T py2cxx(PyObject*);

    python_object cxx2py(python_object);
    python_object cxx2py(python_list);
    python_object cxx2py(python_tuple);
    python_object cxx2py(python_dict);
    python_object cxx2py(std::string const&);
    python_object cxx2py(std::uint32_t);
    python_object cxx2py(std::int32_t);
    python_object cxx2py(std::int64_t);
    python_object cxx2py(std::uint64_t);
    python_object cxx2py(double);
    template <typename T>
    python_object cxx2py(py_ref<T> ref);

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
            return add(PyUnicode_FromString(value.c_str()));
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

    template <typename T>
    typename py2cxx_type<T>::type get_py_ref(PyObject* obj)
    {
        return (typename py2cxx_type<T>::type)(py2cxx<T>(obj));
    }

    template <typename T>
    typename py2cxx_type<T>::type get_py_ref_borrowed(PyObject* obj)
    {
        std::cout << "Normal ref borrowed\n";
        return (typename py2cxx_type<T>::type)(py2cxx<T>(obj));
    }

    template <typename T>
    class py_ref
    {
    public:
        py_ref(T value)
            : m_value(value)
        {
        }

        py_ref(py_ref<T> const& value)
            : m_value(value.m_value)
        {
            m_value.inc_ref();
        }

        ~py_ref()
        {
            m_value.dec_ref();
        }

        PyObject* return_pyobj()
        {
            m_value.inc_ref();
            return m_value.m_obj;
        }

        T* operator ->() { return &m_value; }

        operator PyObject* () const { return m_value.m_obj; }
        T m_value;
    };

    class python_object
    {
    public:
        python_object() : python_object(Py_None)
        {}

        python_object(PyObject* obj)
            : m_obj(obj){
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

        std::string str()
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

        std::string repr()
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

        template <typename T>
        typename py2cxx_type<T>::type getattr(std::string const& arr) {
            return get_py_ref<T>(PyObject_GetAttrString(m_obj, arr.c_str()));
        }

        template <typename T>
        void setattr(std::string const& arr, T const& value)
        {
            PyObject_SetAttrString(m_obj, arr.c_str(), cxx2py(value).m_obj);
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
        typename py2cxx_type<T>::type get(size_t index)
        {
            return get_py_ref_borrowed<T>(PyTuple_GetItem(m_obj, index));
        }

        template <typename T>
        void set(size_t index, T const& value)
        {
            if (PyTuple_SetItem(m_obj, index, cxx2py(value).m_obj))
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

        size_t len()
        {
            return PyDict_Size(m_obj);
        }

        template <typename T>
        typename py2cxx_type<T>::type get(std::string const& key)
        {
            return get_py_ref_borrowed<T>(PyDict_GetItemString(m_obj, key.c_str()));
        }

        template <typename T>
        void set(std::string const& key, T const& value)
        {
            PyDict_SetItemString(m_obj, key.c_str(), cxx2py(value).m_obj);
        }

        void remove(std::string const& key)
        {
            PyDict_DelItemString(m_obj,key.c_str());
        }
    };

    class python_list : public python_object
    {
    public:
        python_list()
            : python_object(PyList_New(0))
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
        typename py2cxx_type<T>::type get(size_t index)
        {
            return get_py_ref_borrowed<T>(PyList_GetItem(m_obj, index));
        }

        template <typename T>
        void set(size_t index, T const& value)
        {
            if (PyList_SetItem(m_obj, index, cxx2py(value).m_obj))
            {
                throw std::runtime_error("Failed to set python list item");
            }
        }

        template <typename T>
        void append(T const& value)
        {
            if (PyList_Append(m_obj, cxx2py(value).m_obj))
            {
                throw std::runtime_error("Failed to append python list item");
            }
        }
    };

    template <typename T>
    python_object cxx2py(py_ref<T> ref)
    {
        return ref.m_value.m_obj;
    }

    template <size_t size>
    PyObject* pyobject_stack<size>::convert(python_object const& value)
    {
        return value.m_obj;
    }

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

    template <typename T>
    py_ref<python_object> create_python_object(T const& value)
    {
        return py_ref<python_object>(cxx2py(value));
    }

    template <>
    inline typename py_ref<python_object> get_py_ref_borrowed<python_object>(PyObject* obj)
    {
        Py_IncRef(obj);
        return py_ref<python_object>(obj);
    }

    template <>
    inline typename py_ref<python_list> get_py_ref_borrowed<python_list>(PyObject* obj)
    {
        Py_IncRef(obj);
        return py_ref<python_list>(obj);
    }

    template <>
    inline typename py_ref<python_tuple> get_py_ref_borrowed<python_tuple>(PyObject* obj)
    {
        Py_IncRef(obj);
        return py_ref<python_tuple>(obj);
    }

    template <>
    inline typename py_ref<python_dict> get_py_ref_borrowed<python_dict>(PyObject* obj)
    {
        Py_IncRef(obj);
        return py_ref<python_dict>(obj);
    }


    py_ref<python_list> create_python_list(size_t size = 0);
    py_ref<python_tuple> create_python_tuple(size_t size);
    py_ref<python_dict> create_python_dict();
}