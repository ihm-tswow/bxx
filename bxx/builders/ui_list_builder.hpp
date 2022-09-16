#pragma once

#include <bxx/builders/class_header_builder.hpp>
#include <bxx/builders/property_builder.hpp>

#include <string>
#include <functional>

namespace bxx
{
    class ui_list_builder
        : public class_header_builder<ui_list_builder>
        , public property_builder<ui_list_builder>
    {
    public:
        using draw_item_type = std::function<void(python_object, python_object, python_object, python_object, python_object, python_object, python_object, python_object)>;
        using draw_filter_type = std::function<void(python_object, python_object, python_object)>;
        using filter_items_type = std::function<python_tuple(python_object, python_object, python_object, python_object)>;
        ui_list_builder(std::string const& class_name);
        ~ui_list_builder();
        ui_list_builder& set_draw_item(draw_item_type draw_item);
        ui_list_builder& set_draw_filter(draw_filter_type draw_item);
        ui_list_builder& set_filter_items(filter_items_type draw_item);
        void write(python_builder& builder);
    private:
        draw_item_type m_draw_item = nullptr;
        draw_filter_type m_draw_filter = nullptr;
        filter_items_type m_filter_items = nullptr;
    };
}