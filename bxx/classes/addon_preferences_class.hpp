#pragma once

#include "property_classes.hpp"
#include "exec.hpp"
#include "../builders/addon_preferences_builder.hpp"
#include "preferences.hpp"

#define ADDON_PREFERENCES(cls,...)\
    static void register_class()\
    {\
        cls().register_class_internal();\
    }\
    \
    std::string get_class_name()\
    {\
        return #cls;\
    }\
    \
    cls(): __VA_ARGS__ {}\
    \
    cls(PyObject* obj): addon_preferences_class<cls>(obj), __VA_ARGS__ {}\
    static cls get()\
    {\
        return cls(bxx::get_addon_preferences());\
    }\

namespace bxx
{
    template <typename T>
    class addon_preferences_class : public property_class
    {
    public:
        PYFIELD(ui_layout, layout)
        virtual void draw(python_object ctx) {
            for (property_base* base : m_properties)
            {
                get_layout().prop(*base);
            }
        };

        addon_preferences_class()
            : property_class(nullptr)
        {}

        addon_preferences_class(PyObject* obj)
            : property_class(obj)
        {}
    protected:
        void register_class_internal() final
        {
            class_property_builder tmp_builder;
            for (property_base* base : m_properties)
            {
                base->write_to(tmp_builder);
            }

            addon_preferences_builder addon(get_class_name());
            addon.read_properties_from(tmp_builder);
            addon.set_draw([=](python_object self, python_object ctx) {
                T(self).draw(ctx);
            });
        }
    };
}