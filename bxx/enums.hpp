#pragma once

#include <common/script_error.hpp>

#include <string>
#include <vector>

namespace bxx
{
    SCRIPT_ERROR_TYPE(enum_cast_error,input_error)

    struct enum_meta
    {
        std::string m_name;
        std::string m_description = "";
        std::string m_icon = "";
    };

    namespace enums
    {
        template <typename T>
        std::string get_enum_name(T value);

        template <typename T>
        std::vector<std::pair<T,std::string>> get_enum_entries();

        template <typename T>
        T get_enum_value(std::string const& name);

        template <typename T>
        enum_meta get_enum_meta(T value);
    }
}

#include <bxx/enums.ipp>
#include <bxx/enums.generated.ipp>