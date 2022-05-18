#include "id.hpp"

#include "makesdna/DNA_ID.h"
#include "exec.hpp"

#include <fmt/core.h>

void bxx::id::set_float_prop(std::string const& prop, float value)
{
    exec({
        fmt::format("{}.{} = {}", get_full_name(), prop, value)
    });
}

void bxx::id::set_float_prop(std::string const& prop, float v1, float v2)
{
    exec({
        fmt::format("{}.{} = ({},{})", get_full_name(), prop, v1,v2)
    });
}

void bxx::id::set_float_prop(std::string const& prop, float v1, float v2, float v3)
{
    exec({
        fmt::format("{}.{} = ({},{},{})",get_full_name(),prop,v1,v2,v3)
    });
}

void bxx::id::set_float_prop(std::string const& prop, float v1, float v2, float v3, float v4)
{
    exec({
        fmt::format("{}.{} = ({},{},{},{})",get_full_name(),prop,v1,v2,v3,v4)
    });
}

void bxx::id::set_string_prop(std::string const& prop, std::string const& value)
{
    exec({
        fmt::format("{}.{} = '{}'", get_full_name(), prop, value)
    });
}