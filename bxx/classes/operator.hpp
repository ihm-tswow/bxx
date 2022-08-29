#pragma once

#include "property_classes.hpp"
#include "../builders/operator_builder.hpp"

#include "ui_layout.hpp"

namespace bxx
{
    template <typename T>
    class operator_class : public property_class
    {
    public:
        operator_class<T>() {}

        virtual void execute() = 0;

        virtual void draw() {
            for (property_base* base : m_properties)
            {
                get_layout().prop(*base);
            }
        };

        PYFIELD(ui_layout, layout);

        void register_class_internal() final
        {
            class_property_builder tmp_builder;
            for (property_base* base : m_properties)
            {
                base->write_to(tmp_builder);
            }

            operator_builder op(get_class_name(), true);
            op.read_properties_from(tmp_builder);
            op.set_callback([this](bxx::python_object obj) {
                T t;
                t.m_obj = obj.m_obj;
                t.execute();
                t.m_obj = nullptr;
            });

            op.set_disable_drawing(false);
            op.set_draw([this](bxx::python_object obj) {
                T t;
                t.m_obj = obj.m_obj;
                t.draw();
                t.m_obj = nullptr;
            });
        }
    };
}