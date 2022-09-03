#pragma once

#include <string>
#include <functional>

namespace bxx
{
    class python_object;
    class python_tuple;
    std::string cur_script_name();
    size_t get_script_index();
    size_t lib_register_event(std::function<python_object(python_tuple tuple)> evt);
}