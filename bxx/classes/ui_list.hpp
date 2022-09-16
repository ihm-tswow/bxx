#pragma once

#include <bxx/objects/python_object.hpp>
#include <bxx/classes/property_classes.hpp>
#include <bxx/classes/ui_layout.hpp>
#include <bxx/builders/ui_list_builder.hpp>

#include <type_traits>

#define UI_LIST_INTERNAL(cls,...)\
    static void register_class()\
    {\
        cls().register_class_internal();\
    }\
    std::string get_class_name()\
    {\
        return #cls;\
    }\
    cls() __VA_ARGS__ {}\
    cls(PyObject* obj)\
        __VA_ARGS__\
    {\
        bxx::details::replace_python_object(*this, obj);\
    }\

#define UI_LIST(cls,...)\
    UI_LIST_INTERNAL(cls,:,__VA_ARGS__)
#define UI_LIST_NO_PROPS(cls,...)\
    UI_LIST_INTERNAL(cls)

namespace bxx
{
    enum class ui_list_layout_type
    {
        DEFAULT,
        COMPACT,
        GRID
    };

    template <typename crtp>
    class ui_list : public property_class
    {
    public:
        virtual void draw_item(python_object ctx, ui_layout layout, python_object data, python_object item, std::uint64_t icon, python_object active_data, std::string const& active_propname) = 0;
        virtual void draw_filter(python_object ctx, ui_layout layout) {}
        virtual python_tuple filter_items(python_object ctx, python_object data, python_object propname) { return python_tuple(); }

        PYFIELD_STRINGENUM(ui_list_layout_type, layout_type)
        PYFIELD(std::string, filter_name)
        PYFIELD(std::string, list_id)
        PYFIELD(bool, use_filter_invert)
        PYFIELD(bool, use_filter_show)
        PYFIELD(bool, use_filter_sort_alpha)
        PYFIELD(bool, use_filter_sort_lock)
        PYFIELD(bool, use_filter_sort_reverse)

        void register_class_internal() final
        {
            class_property_builder tmp_builder;
            for (property_base* base : m_properties)
            {
                base->write_to(tmp_builder);
            }

            ui_list_builder builder(get_class_name());
            builder.read_properties_from(tmp_builder);

            builder.set_draw_item([this](python_object self, python_object ctx, python_object layout, python_object data, python_object item, python_object icon, python_object active_data, python_object active_propname) {
                crtp(self.get_pyobject()).draw_item(ctx, layout.get_pyobject(), data, item, details::py2cxx<std::uint64_t>(icon), active_data, details::py2cxx<std::string>(active_propname));
            });

            builder.set_draw_filter([this](python_object self, python_object ctx, python_object layout) {
                crtp(self.get_pyobject()).draw_filter(ctx,layout.get_pyobject());
            });

            if (!std::is_same<decltype(& crtp::filter_items), decltype(&ui_list::filter_items)>::value)
            {
                builder.set_filter_items([this](python_object self, python_object ctx, python_object layout, python_object prop) {
                    return crtp(self.get_pyobject()).filter_items(ctx, layout, prop);
                });
            }
        }
    };
}