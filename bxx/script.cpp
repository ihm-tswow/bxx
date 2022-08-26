#include "script.hpp"

static std::string script_name = "";
static size_t script_index;

void setup_script_data(std::string const& name, size_t index)
{
    script_name = name;
    script_index = index;
}

size_t bxx::get_script_index()
{
    return script_index;
}

std::string bxx::cur_script_name()
{
    return script_name;
}