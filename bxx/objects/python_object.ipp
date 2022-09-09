#pragma once

#include <common/script_error.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/mathutils.hpp>
#include <common/shared_functions.hpp>

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

    template <typename T, class ... Args>
    T python_object_base::call(std::string const& method, Args&&... args)
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to call function {} on null python_object", method.c_str());
        details::call_arg_stack<sizeof...(Args)> arg_refs;
        details::call_kwarg_stack<sizeof...(Args)> kwarg_refs;
        ([&] { details::convert_fn(args, arg_refs, kwarg_refs); }(), ...);

        python_object arg_tup = python_object::steal(PyTuple_New(arg_refs.m_size));

        std::optional<python_object> kwarg_dict;
        if (kwarg_refs.m_size)
        {
            kwarg_dict.emplace(python_object::steal(PyDict_New()));
            for (size_t i = 0; i < kwarg_refs.m_size; ++i)
            {
                PyDict_SetItemString(kwarg_dict->get_pyobject(), kwarg_refs.m_names[i].c_str(), kwarg_refs.m_refs[i].value().m_pyobj);
            }
        }

        for (size_t i = 0; i < arg_refs.m_size; ++i)
        {
            PyTuple_SetItem(arg_tup, i, arg_refs.m_refs[i].value().m_pyobj);
        }

        python_object fn = python_object::steal(PyObject_GetAttrString(get_pyobject(), method.c_str()));
        python_object res = python_object::steal(PyObject_Call(fn, arg_tup, kwarg_dict.has_value() ? kwarg_dict->get_pyobject() : nullptr));
        BXX_SCRIPT_ASSERT(res.get_pyobject(), internal_python_error, "failed to call function {}", method.c_str());
        return details::py2cxx<T>(res);
    }

    template<typename T>
    inline T python_object_base::call(std::string const& method)
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to call function {} on null python_object", method.c_str());
        python_object tup = python_object::steal(PyTuple_New(0));
        BXX_SCRIPT_ASSERT(tup.get_pyobject(), internal_python_error, "failed to create call argument tuple");

        python_object fn = getattr(method);
        BXX_SCRIPT_ASSERT(fn.get_pyobject(), python_key_error, "could not find python function {}", method.c_str());
        python_object res = python_object::steal(PyObject_Call(fn, tup.get_pyobject(), nullptr));
        BXX_SCRIPT_ASSERT(res.get_pyobject(), internal_python_error, "failed to call function {}", method.c_str());

        return details::py2cxx<T>(res);
    }

    template <typename T>
    inline T python_object_base::getattr(std::string const& arr) const
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to get attribute {} on null python_object", arr.c_str());
        BXX_SCRIPT_ASSERT(hasattr(arr), python_key_error, "tried to get missing python attribute {}", arr.c_str());
        python_object obj = python_object::steal(PyObject_GetAttrString(get_pyobject(), arr.c_str()));
        BXX_SCRIPT_ASSERT(obj.get_pyobject(), python_key_error, "could not find python attribute {}", arr.c_str());
        return details::py2cxx<T>(obj);
    }

    template <typename T>
    void python_object_base::setattr(std::string const& arr, T value)
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to set attribute {} on null python_object",arr.c_str());
        int err = PyObject_SetAttrString(get_pyobject(), arr.c_str(), details::cxx2py(value, false));
        BXX_SCRIPT_ASSERT(!err, internal_python_error, "failed to set python attribute ", arr.c_str());
    }

    template <typename T>
    T python_object_base::get_item(std::string const& key) const
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to get item {} from null python_object",key.c_str());
        python_object str = python_object::steal(PyUnicode_FromString(key.c_str()));
        BXX_SCRIPT_ASSERT(str.get_pyobject(), internal_python_error, "failed to create python string for item {}", key.c_str());
        python_object obj = python_object::steal(PyObject_GetItem(get_pyobject(), str));
        BXX_SCRIPT_ASSERT(obj.get_pyobject(), python_key_error, "could not find python item for ", key.c_str());
        return details::py2cxx<T>(obj);
    }

    template <typename T>
    T python_object_base::get_item(std::uint32_t key) const
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to get item from null python_object");
        python_object num = python_object::steal(PyLong_FromLong(key));
        BXX_SCRIPT_ASSERT(num.get_pyobject(), python_object_error, "failed to create python number");
        python_object obj = python_object::steal(PyObject_GetItem(get_pyobject(), num));
        BXX_SCRIPT_ASSERT(obj, python_key_error, "failed to get python item");
        return details::py2cxx<T>(obj);
    }

    template <typename T>
    void python_object_base::set_item(std::string const& key, T value)
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to set item on null python_object");
        python_object str = python_object::steal(PyUnicode_FromString(key.c_str()));
        BXX_SCRIPT_ASSERT(str.get_pyobject(), internal_python_error, "failed to create python string");
        PyObject_SetItem(get_pyobject(), str, details::cxx2py(value, false));
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
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create string from null python object");
            BXX_SCRIPT_ASSERT(PyUnicode_Check(obj), python_type_error, "tried to extract string from non-string python object");
            return _PyUnicode_AsString(obj);
        }

        template <> inline std::uint32_t py2cxx<std::uint32_t>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create uint32 from null python object");
            BXX_SCRIPT_ASSERT(PyLong_Check(obj), python_type_error, "tried to extract uint32 from non-long python object");
            return PyLong_AsUnsignedLong(obj);
        }

        template <> inline std::int32_t py2cxx<std::int32_t>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create int32 from null python object");
            BXX_SCRIPT_ASSERT(PyLong_Check(obj), python_type_error, "tried to extract int32 from non-long python object");
            return PyLong_AsLong(obj);
        }

        template <> inline std::uint64_t py2cxx<std::uint64_t>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create uint64 from null python object");
            BXX_SCRIPT_ASSERT(PyLong_Check(obj), python_type_error, "tried to extract uint64 from non-long python object");
            return PyLong_AsUnsignedLongLong(obj);
        }

        template <> inline std::int64_t py2cxx<std::int64_t>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create int64 from null python object");
            BXX_SCRIPT_ASSERT(PyLong_Check(obj), python_type_error, "tried to extract int64 from non-long python object");
            return PyLong_AsLongLong(obj);
        }

        template <> inline float py2cxx<float>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create float from null python object");
            BXX_SCRIPT_ASSERT(PyFloat_Check(obj), python_type_error, "tried to extract float from non-float python object");
            return static_cast<float>(PyFloat_AsDouble(obj));
        }

        template <> inline double py2cxx<double>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create double from null python object");
            BXX_SCRIPT_ASSERT(PyFloat_Check(obj), python_type_error, "tried to extract double from non-float python object");
            return PyFloat_AsDouble(obj);
        }

        template <> inline mathutils::vec2 py2cxx<mathutils::vec2>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create vec2 from null python object");
            BXX_SCRIPT_ASSERT(PyTuple_Check(obj), python_type_error, "tried to extract vec2 from non-tuple python object");
            BXX_SCRIPT_ASSERT(PyTuple_Size(obj) >= 2, python_size_error, "tried to create vec2 from too small tuple");
            return {
                py2cxx<float>(PyTuple_GetItem(obj,0)),
                py2cxx<float>(PyTuple_GetItem(obj,1))
            };
        }

        template <> inline mathutils::vec3 py2cxx<mathutils::vec3>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create vec3 from null python object");
            BXX_SCRIPT_ASSERT(PyTuple_Check(obj), python_type_error, "tried to extract vec3 from non-tuple python object");
            BXX_SCRIPT_ASSERT(PyTuple_Size(obj) >= 3, python_size_error, "tried to create vec3 from too small tuple");
            return {
                py2cxx<float>(PyTuple_GetItem(obj,0)),
                py2cxx<float>(PyTuple_GetItem(obj,1)),
                py2cxx<float>(PyTuple_GetItem(obj,2))
            };
        }

        template <> inline mathutils::quaternion py2cxx<mathutils::quaternion>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(obj, python_object_error, "tried to create quaternion from null python object");
            BXX_SCRIPT_ASSERT(PyTuple_Check(obj), python_type_error, "tried to extract quaternion from non-tuple python object");
            BXX_SCRIPT_ASSERT(PyTuple_Size(obj) >= 4, python_size_error, "tried to create quaternion from too small tuple");
            return {
                py2cxx<float>(PyTuple_GetItem(obj,0)),
                py2cxx<float>(PyTuple_GetItem(obj,1)),
                py2cxx<float>(PyTuple_GetItem(obj,2)),
                py2cxx<float>(PyTuple_GetItem(obj,3))
            };
        }

        template <> inline mathutils::rgba py2cxx<mathutils::rgba>(PyObject* obj)
        {
            BXX_SCRIPT_ASSERT(PyTuple_Check(obj), python_object_error, "tried to extract rgba from non-tuple python object");
            BXX_SCRIPT_ASSERT(PyTuple_Check(obj), python_type_error, "tried to extract quaternion from non-tuple python object");
            BXX_SCRIPT_ASSERT(PyTuple_Size(obj) >= 4, python_size_error, "tried to create rgba from too small tuple");
            return {
                py2cxx<float>(PyTuple_GetItem(obj,0)),
                py2cxx<float>(PyTuple_GetItem(obj,1)),
                py2cxx<float>(PyTuple_GetItem(obj,2)),
                py2cxx<float>(PyTuple_GetItem(obj,3))
            };
        }
    }

    template <typename ...Args>
    inline python_object eval_pyobject(fmt::format_string<Args...> str, Args... args)
    {
        PyObject* obj = get_pointers()->cy_eval(
            const_cast<char*>(fmt::format(str, std::forward<Args>(args)...).c_str())
        );
        BXX_SCRIPT_ASSERT(obj, python_object_error, "evaluated null PyObject");
        return obj;
    }

    template <typename T, typename ...Args>
    T call_python_function(std::string const& name, Args&&... args)
    {
        return eval_pyobject("out = registered_python_functions").call<T>(name, std::forward<Args>(args)...);
    }
}