#include <common/script_index.hpp>

static size_t script_index = -1;
static std::string script_name = "";

void setup_script_data(std::string const& name, size_t index)
{
    script_name = name;
    script_index = index;
}

namespace bxx
{
    size_t bxx::get_script_index()
    {
        return script_index;
    }

    std::string cur_script_name()
    {
        return script_name;
    }
}