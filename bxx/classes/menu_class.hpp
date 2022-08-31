#pragma once

#include "ui_class.hpp"
#include "../builders/menu_builder.hpp"

namespace bxx
{
    template <typename T>
    class menu_class : public ui_class
    {
    public:
        virtual void draw(python_object ctx) = 0;
    protected:
        void register_internal()
        {
            menu_builder builder(get_class_name());
            builder.set_draw([this](python_object self, python_object ctx) {
                T(self).draw(ctx);
            });
        }
    };
}