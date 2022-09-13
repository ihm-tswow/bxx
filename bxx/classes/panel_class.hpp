#pragma once

#include <bxx/classes/ui_class.hpp>
#include <bxx/builders/panel_builder.hpp>

namespace bxx
{
    template <
        typename T,
        bl_context context,
        bl_space_type space_type = bl_space_type::EMPTY,
        bl_region_type region_type = bl_region_type::WINDOW,
        bool(*poll)(python_object,python_object) = nullptr
    >
    class panel_class: public ui_class
    {
    public:
        virtual void draw(python_object ctx) = 0;
        virtual void draw_header(python_object ctx) {}
    protected:
        void register_internal()
        {
            panel_builder builder(get_class_name());

            builder.set_context(context);
            builder.set_space_type(space_type);
            builder.set_region_type(region_type);
            builder.set_draw([this](python_object self, python_object ctx) {
                T(self).draw(ctx);
            });
            builder.set_draw_header([this](python_object self, python_object ctx) {
                T(self).draw_header(ctx);
            });
            if (poll)
            {
                builder.set_poll(poll);
            }
        }
    };
}