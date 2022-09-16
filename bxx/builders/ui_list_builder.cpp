#include <bxx/builders/ui_list_builder.hpp>

namespace bxx
{
    ui_list_builder::ui_list_builder(std::string const& class_name)
        : class_header_builder(class_name)
    {
        add_parent_class("bpy.types.UIList");
    }

    ui_list_builder::~ui_list_builder()
    {
        python_builder builder;
        write(builder);
        builder.exec();
    }

    ui_list_builder& ui_list_builder::set_draw_item(draw_item_type draw_item)
    {
        m_draw_item = draw_item;
        return *this;
    }

    ui_list_builder& ui_list_builder::set_draw_filter(draw_filter_type draw_filter)
    {
        m_draw_filter = draw_filter;
        return *this;
    }

    ui_list_builder& ui_list_builder::set_filter_items(filter_items_type filter_items)
    {
        m_filter_items = filter_items;
        return *this;
    }

    void ui_list_builder::write(python_builder& builder)
    {
        class_header_builder<ui_list_builder>::write(builder, [&]() {
            if (m_draw_item)
            {
                auto draw_item = m_draw_item;
                size_t event_id = lib_register_event([draw_item](python_tuple tuple) {
                    draw_item(
                        tuple.get<python_object>(0),
                        tuple.get<python_object>(1),
                        tuple.get<python_object>(2),
                        tuple.get<python_object>(3),
                        tuple.get<python_object>(4),
                        tuple.get<python_object>(5),
                        tuple.get<python_object>(6),
                        tuple.get<python_object>(7)
                    );
                    return python_object();
                });
                builder.begin_line("def draw_item(self,ctx, layout, data, item, icon, active_data, active_propname):");
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("fire_event({},{},self,ctx,layout,data,item,icon,active_data,active_propname)", get_script_index(), event_id);
            }

            if (m_draw_filter)
            {
                auto draw_filter = m_draw_filter;
                size_t event_id = lib_register_event([draw_filter](python_tuple tuple) {
                    draw_filter(
                        tuple.get<python_object>(0),
                        tuple.get<python_object>(1),
                        tuple.get<python_object>(2)
                    );
                    return python_object();
                });
                builder.begin_line("def draw_filter(self,ctx, layout):");
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("fire_event({},{},self,ctx,layout)", get_script_index(), event_id);
            }

            if (m_filter_items)
            {
                auto filter_items = m_filter_items;
                size_t event_id = lib_register_event([filter_items](python_tuple tuple) {
                    return filter_items(
                        tuple.get<python_object>(0),
                        tuple.get<python_object>(1),
                        tuple.get<python_object>(2),
                        tuple.get<python_object>(3)
                    );
                });
                builder.begin_line("def filter_items(self,ctx, layout, propname):");
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("fire_event({},{},self,ctx,layout,propname)", get_script_index(), event_id);
            }
        });
        builder.write_line("register_class({},{})", get_script_index(), get_class_name());
    }
}