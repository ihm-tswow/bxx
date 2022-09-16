#pragma once

#include <common/script_index.hpp>

#include <string>
#include <functional>

namespace bxx
{
    class python_object;
    class python_tuple;
    size_t lib_register_event(std::function<python_object(python_tuple args)> evt);
    void register_cxx_function(std::string const& name, std::function<python_object(python_tuple args)> callback);
}