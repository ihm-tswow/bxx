#pragma once

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <fmt/core.h>

#include <stdexcept>
#include <string>

#define BXX_SCRIPT_ASSERT(expression,exception_type,...)\
    if(expression); else throw exception_type(fmt::format(__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)

#define BXX_SCRIPT_ASSERT_EQUAL(expression_a,expression_b,exception_type,...)\
    BXX_SCRIPT_ASSERT(expression_a == expression_b,exception_type,__VA_ARGS__)

#define BXX_SCRIPT_ASSERT_NOT_EQUAL(expression_a,expression_b,exception_type,...)\
    BXX_SCRIPT_ASSERT(expression_a != expression_b,exception_type,__VA_ARGS__)

#define SCRIPT_ERROR_TYPE(name,parent) class name : public parent { using parent::parent; };

namespace bxx
{
    class script_error_base : public std::runtime_error
    {
    public:
        script_error_base(std::string const& message, char const* file, char const* function, int line);
    };

    SCRIPT_ERROR_TYPE(internal_error, script_error_base)
    SCRIPT_ERROR_TYPE(input_error, script_error_base)
}