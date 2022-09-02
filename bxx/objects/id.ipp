#pragma once

#include <bxx/objects/id.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    template <typename pointer_type>
    id<pointer_type>::id(PyObject* obj)
        : python_object(obj)
        // todo: move to c implementation
        , m_ptr(reinterpret_cast<pointer_type*>(call<std::uint64_t>("as_pointer")))
    {}

    template <typename pointer_type>
    std::string id<pointer_type>::get_full_name() const
    {
        return getattr<std::string>("name_full");
    }

    template <typename pointer_type>
    std::string id<pointer_type>::get_name() const
    {
        return getattr<std::string>("name");
    }

    template <typename pointer_type>
    pointer_type* id<pointer_type>::get_pointer() const
    {
        return m_ptr;
    }

    template <typename pointer_type>
    pointer_type* id<pointer_type>::get_pointer()
    {
        return m_ptr;
    }
}