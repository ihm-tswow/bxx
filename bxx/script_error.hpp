#pragma once

#include <fmt/core.h>

#include <stdexcept>

#define BXX_SCRIPT_ASSERT(val,...) if(!val) throw bxx::script_error(fmt::format(__VA_ARGS__));
#define BXX_SCRIPT_ASSERT_EQUAL(a,b,...);//if(a != b) throw bxx::script_error(__VA_ARGS__);

namespace bxx
{
    class script_error : public std::runtime_error
    {
    public:
        template <typename ...Args>
        script_error(std::string const& err)
            : std::runtime_error(err)
        {}
    };
}