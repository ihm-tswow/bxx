#pragma once

typedef struct ID bl_id;

#include "python_object.hpp"
#include "exec.hpp"

#include <fmt/core.h>
#include <string>

namespace bxx
{
    class python_object;
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

        float get_float_prop(std::string const& prop);

        template <typename T = python_object>
        T to_python_object() const
        {
            python_object obj = eval_pyobject(fmt::format("out = {}", get_full_name()));
            return T(obj.m_obj);
        }

        template <typename T = python_object>
        T getattr(std::string const& key) const
        {
            return to_python_object().getattr<T>(key);
        }

        template <typename T>
        void setattr(std::string const& key, T const& value)
        {
            to_python_object().setattr<T>(key, value);
        }

        bool hasattr(std::string const& key) const;
    };
}