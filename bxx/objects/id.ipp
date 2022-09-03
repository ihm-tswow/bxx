#pragma once

#include <bxx/objects/id.hpp>
#include <common/exec.hpp>
#include <common/shared_functions.hpp>

#include <fmt/core.h>

namespace bxx
{
    template <typename pointer_type>
    python_object id<pointer_type>::get_pyobject()
    {
        return eval_pyobject(fmt::format("out = {}", get_name_full()));
    }

    template <typename pointer_type>
    std::string id<pointer_type>::get_name_full() const
    {
        return fmt::format("{}['{}']", get_type_path(), get_name());
    }

    template <typename pointer_type>
    std::string id<pointer_type>::get_name() const
    {
        return this->get_raw_struct()->id.name + 2;
    }
}