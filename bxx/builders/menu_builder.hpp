#pragma once

#include <bxx/builders/class_header_builder.hpp>

namespace bxx
{
    class python_object;
    class python_builder;

    class menu_builder : public class_header_builder<menu_builder>
    {
    public:
        menu_builder(std::string const& class_name);
        ~menu_builder();
        menu_builder& set_draw(std::function<void(python_object, python_object)> callback);
        void write(python_builder& builder);
    private:
        std::function<void(python_object, python_object)> m_draw;
    };
}
