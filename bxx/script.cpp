#include "script.hpp"

static std::string script_name = "";

void __set_script_name(std::string const& name)
{
    script_name = name;
}

std::string bxx::cur_script_name()
{
    return script_name;
}