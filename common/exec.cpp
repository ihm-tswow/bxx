#include "exec.hpp"

std::string join_strings(std::initializer_list<std::string> const& python)
{
    std::string concat = "";
    for (std::string const& str : python)
    {
        concat += str + "\n";
    }
    return concat;
}

void exec(std::string const& python)
{
    get_pointers()->cy_exec(const_cast<char*>(python.c_str()));
}

int eval_int(std::string const& python)
{
    return get_pointers()->cy_eval_int(const_cast<char*>(python.c_str()));
}

float eval_float(std::string const& python)
{
    return get_pointers()->cy_eval_float(const_cast<char*>(python.c_str()));
}

std::string eval_string(std::string const& python)
{
    return get_pointers()->cy_eval_string(const_cast<char*>(python.c_str()));
}

void exec(std::initializer_list<std::string> const& python)
{
    return exec(join_strings(python));
}
int eval_int(std::initializer_list<std::string> const& python)
{
    return eval_int(join_strings(python));
}
float eval_float(std::initializer_list<std::string> const& python)
{
    return eval_float(join_strings(python));
}
std::string eval_string(std::initializer_list<std::string> const& python)
{
    return eval_string(join_strings(python));
}

bxx::python_object eval_pyobject(std::string const& python)
{
    PyObject* obj = get_pointers()->cy_eval_pyobject(const_cast<char*>(python.c_str()));
    return bxx::python_object(obj);
}

bxx::python_object eval_pyobject(std::initializer_list<std::string> const& python)
{
    return eval_pyobject(join_strings(python));
}
