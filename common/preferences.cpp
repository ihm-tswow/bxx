#include "preferences.hpp"
#include "exec.hpp"

// note: we avoid fmt::format in this file since core is using it

std::string bxx::preferences::get_string(std::string const& id, std::string const& def)
{
    return eval_string("out = preferences.get('" + id + "', '" + def + "')");
}

int bxx::preferences::get_int(std::string const& id, int def)
{
    return eval_int("out = preferences.get('" + id + "', " + std::to_string(def) + "");
}

float bxx::preferences::get_float(std::string const& id, float def)
{
    return eval_float("out = preferences.get('" + id + "', " + std::to_string(def) + "");
}

bool bxx::preferences::get_bool(std::string const& id, bool def)
{
    return bool(eval_int("out = int(preferences.get('" + id + "', " + (def ? "True" : "False") + "))"));
}

std::string bxx::get_addon_name()
{
    return eval_string("out = get_addon_name()");
}
