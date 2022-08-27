#pragma once

#include "shared_functions.hpp"
#include "python_object.hpp"

#include <string>

std::string join_strings(std::initializer_list<std::string> const& python);

void exec(std::string const& python);
void exec(std::initializer_list<std::string> const& python);
int eval_int(std::string const& python);
int eval_int(std::initializer_list<std::string> const& python);
float eval_float(std::string const& python);
float eval_float(std::initializer_list<std::string> const& python);
std::string eval_string(std::string const& python);
std::string eval_string(std::initializer_list<std::string> const& python);

bxx::py_ref<bxx::python_object> eval_pyobject(std::string const& python);
bxx::py_ref<bxx::python_object> eval_pyobject(std::initializer_list<std::string> const& python);

template <typename T>
T* eval_ptr(std::string const& python)
{
    return (T*)get_pointers()->cy_eval_ptr(const_cast<char*>(python.c_str()));
}

template <typename T>
T* eval_ptr(std::initializer_list<std::string> const& python)
{
    return eval_ptr<T>(join_strings(python));
}