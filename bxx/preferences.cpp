#include "preferences.hpp"
#include "exec.hpp"

#include "fmt/core.h"

std::string bxx::preferences::get_string(std::string const& id, std::string const& def)
{
    return eval_string(fmt::format("out = preferences.get('{}','{}')", id, def));
}

int bxx::preferences::get_int(std::string const& id, int def)
{
    return eval_int(fmt::format("out = preferences.get('{}',{})", id, def));
}

float bxx::preferences::get_float(std::string const& id, float def)
{
    return eval_float(fmt::format("out = preferences.get('{}',{})", id, def));
}

bool bxx::preferences::get_bool(std::string const& id, bool def)
{
    return bool(eval_int(fmt::format("out = int(preferences.get('{}',{}))", id, def ? "True" : "False")));
}
