#pragma once

#include "property_builder.hpp"
#include "class_header_builder.hpp"

#include <functional>

namespace bxx
{
    class python_builder;
    class python_object;

    class addon_preferences_builder
        : public bxx::property_builder<addon_preferences_builder>
        , public bxx::class_header_builder<addon_preferences_builder>
    {
    public:
        addon_preferences_builder(std::string const& classname);
        ~addon_preferences_builder();
        addon_preferences_builder& set_draw(std::function<void(python_object, python_object)>);
        void write(python_builder& builder);
    private:
        std::function<void(python_object,python_object)> m_draw;
    };
}