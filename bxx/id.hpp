#pragma once

typedef struct ID bl_id;

#include <string>

namespace bxx
{
    class id
    {
    public:
        virtual std::string get_name() const = 0;
        virtual std::string get_full_name() const = 0;
        void set_float_prop(std::string const& prop, float value);
        void set_float_prop(std::string const& prop, float v1, float v2);
        void set_float_prop(std::string const& prop, float v1, float v2, float v3);
        void set_float_prop(std::string const& prop, float v1, float v2, float v3, float v4);
        void set_string_prop(std::string const& prop, std::string const& value);
    };
}