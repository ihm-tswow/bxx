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
    return get_pointers()->cy_eval_int(
        const_cast<char*>(join_strings(lines).c_str())
    );
}

float eval_float(std::initializer_list<std::string> lines)
{
    return get_pointers()->cy_eval_float(
        const_cast<char*>(join_strings(lines).c_str())
    );
}

std::string eval_string(std::initializer_list<std::string> lines)
{
    return get_pointers()->cy_eval_string(
        const_cast<char*>(join_strings(lines).c_str())
    );
}

