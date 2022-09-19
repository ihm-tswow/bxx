#pragma once

#include <common/exec.hpp>

#include <cassert>

namespace bxx
{
    namespace details
    {
        template <typename blender_type>
        blender_type* blender_py_ptr(PyObject* obj)
        {
            if (obj == nullptr)
            {
                return nullptr;
            }

            python_object_weak weak(obj);

            if (!weak.is_valid())
            {
                return nullptr;
            }

            return reinterpret_cast<blender_type*>(weak.call<std::uint64_t>("as_pointer"));
        }
    }

    template <typename blender_type>
    blender_py_struct<blender_type>::blender_py_struct(PyObject* obj)
        : blender_struct<blender_type>(details::blender_py_ptr<blender_type>(obj))
        , python_object(obj)
    {}

    template <typename blender_type>
    blender_py_struct<blender_type>::blender_py_struct(python_object const& obj)
        : blender_py_struct(obj.get_pyobject())
    {}

    template <typename blender_type>
    blender_py_struct<blender_type>::blender_py_struct(python_object&& obj) noexcept
        : blender_py_struct<blender_type>(obj.get_pyobject())
        //: blender_struct<blender_type>(details::blender_py_ptr<blender_type>(obj))
        //, python_object(obj)
    {}

    template <typename blender_type>
    blender_struct<blender_type>::blender_struct(blender_type* ptr)
        : m_ptr(ptr)
    {
    }

    template <typename blender_type>
    blender_type* blender_struct<blender_type>::get_raw_struct() const
    {
        return m_ptr;
    }

    template <typename blender_type>
    blender_type* blender_struct<blender_type>::get_raw_struct()
    {
        return m_ptr;
    }
}
