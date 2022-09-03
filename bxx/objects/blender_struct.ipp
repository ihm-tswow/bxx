#pragma once

#include <common/exec.hpp>

#include <cassert>

namespace bxx
{
    template <typename blender_type>
    blender_py_struct<blender_type>::blender_py_struct(PyObject* obj)
        : blender_struct<blender_type>(reinterpret_cast<blender_type*>(python_object_weak(obj).getattr<std::uint64_t>("as_pointer")))
    {}

    template <typename blender_type>
    blender_py_struct<blender_type>::blender_py_struct(python_object obj)
        : blender_struct<blender_type>(reinterpret_cast<blender_type*>(obj.getattr<std::uint64_t>("as_pointer")))
    {}

    template <typename blender_type>
    blender_struct<blender_type>::blender_struct(blender_type* ptr)
        : m_ptr(ptr)
    {
        assert(ptr);
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
