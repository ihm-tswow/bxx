#include <common/exec.hpp>

std::string join_strings(std::initializer_list<std::string> const& python)
{
    std::string concat = "";
    for (std::string const& str : python)
    {
        concat += str + "\n";
    }
    return concat;
}

void exec(std::initializer_list<std::string> lines)
{
    get_pointers()->cy_exec(
        const_cast<char*>(join_strings(lines).c_str())
    );
}

int eval_int(std::initializer_list<std::string> lines)
{
    return PyLong_AsLong(get_pointers()->cy_eval(
        const_cast<char*>(join_strings(lines).c_str())
    ));
}

float eval_float(std::initializer_list<std::string> lines)
{
    return float(PyFloat_AsDouble(get_pointers()->cy_eval(
        const_cast<char*>(join_strings(lines).c_str())
    )));
}

std::string eval_string(std::initializer_list<std::string> lines)
{
    return std::string(_PyUnicode_AsString(get_pointers()->cy_eval(
        const_cast<char*>(join_strings(lines).c_str())
    )));
}

