#pragma once

#include "Python.h"

#include <string>
#include <functional>

namespace bxx
{
    std::string cur_script_name();
    size_t get_script_index();
    size_t lib_register_event(std::function<void(PyObject*)> evt);
}