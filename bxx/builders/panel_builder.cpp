#include "panel_builder.hpp"
#include "../script.hpp"
#include "python_object.hpp"

#include "magic_enum.hpp"

namespace bxx
{
    panel_builder::panel_builder(std::string const& class_name)
        : class_header_builder<panel_builder>(class_name)
    {
        add_parent_class("bpy.types.Panel");
        set_id("BXX_PT_" + class_name);
    }

    panel_builder::~panel_builder()
    {
        python_builder builder;
        write(builder);
        builder.exec();
    }

    panel_builder& panel_builder::set_space_type(bl_space_type space_type)
    {
        set_class_variable("bl_space_type", std::string(magic_enum::enum_name<bl_space_type>(space_type)));
        return *this;
    }

    panel_builder& panel_builder::set_region_type(bl_region_type region_type)
    {
        set_class_variable("bl_region_type", std::string(magic_enum::enum_name<bl_region_type>(region_type)));
        return *this;
    }

    panel_builder& panel_builder::set_context(bl_context context)
    {
        set_class_variable("context", std::string(magic_enum::enum_name<bl_context>(context)));
        return *this;
    }

    panel_builder& panel_builder::set_draw(std::function<void(python_object, python_object)> callback)
    {
        m_draw = callback;
        return *this;
    }

    panel_builder& panel_builder::set_draw_header(std::function<void(python_object, python_object)> callback)
    {
        m_draw_header = callback;
        return *this;
    }

    panel_builder& panel_builder::set_poll(std::function<bool(python_object, python_object)> callback)
    {
        m_poll = callback;
        return *this;
    }

    void panel_builder::write(python_builder& builder)
    {
        class_header_builder<panel_builder>::write(builder, [&, this]() {
            auto draw = m_draw;
            auto draw_header = m_draw_header;
            auto poll = m_poll;
            size_t event_index = lib_register_event([=](bxx::python_tuple const& tuple) {
                draw(tuple.get<python_object>(0), tuple.get<python_object>(1));
                return python_object();
            });
            builder.begin_line("def draw(self,context):");
            {
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("fire_event({},{},self,context)", get_script_index(), event_index);
            }

            if (poll)
            {
                size_t poll_index = lib_register_event([=](bxx::python_tuple const& tuple) {
                    return python_object(cxx2py(poll(tuple.get<python_object>(0), tuple.get<python_object>(1))));
                });

                builder.write_line("@classmethod");
                builder.begin_line("def poll(cls,context):");
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("return fire_event({},{},cls,context)", get_script_index(), poll_index);
            }

            if (draw_header)
            {
                size_t draw_header_index = lib_register_event([=](bxx::python_tuple const& tuple) {
                    draw_header(tuple.get<python_object>(0), tuple.get<python_object>(1));
                    return python_object();
                });

                builder.begin_line("def draw_header(self,context):");
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("fire_event({},{},self,context)", get_script_index(), draw_header_index);
            }
        });
        builder.write_line("register_class({},{})", get_script_index(), get_class_name());
    }
}