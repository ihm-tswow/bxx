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

    template <typename property_type = python_object, typename owner_type = python_object>
    class generic_property
    {
    public:
        generic_property(generic_property<property_type, owner_type> const&) = delete;
        generic_property(generic_property<property_type, owner_type>&&) = delete;
        generic_property(owner_type owner, std::string const& name)
            : m_owner(owner)
            , m_id(name)
        {}

        property_type get_value() { return m_owner.getattr<property_type>(m_id); }
        void set_value(property_type const& type) { return m_owner.getattr<property_type>(m_id); }

        owner_type get_owner() { return m_owner; }
        std::string get_id() { return m_id; }
    private:
        owner_type m_owner;
        std::string m_id;
    };

    template <typename property_type = python_object, typename owner_type = python_object>
    class generic_property_readonly
    {
    public:
        generic_property_readonly(generic_property_readonly<property_type, owner_type> const&) = delete;
        generic_property_readonly(generic_property_readonly<property_type, owner_type> &&) = delete;
        generic_property_readonly(python_object owner, python_object prop)
            : m_owner(owner_type(owner))
            , m_property(property_type(prop))
        {}

        property_type& get_value() { return m_property; }
        owner_type get_owner() { return m_owner; }
    private:
        owner_type m_owner;
        property_type m_property;
    };

    template <
        typename crtp,
        typename item_type = python_object,
        typename data_type = python_object,
        typename active_type = python_object
    >
    class ui_list : public property_class
    {
    public:
        virtual void draw_item(python_object ctx, ui_layout layout, generic_property_readonly<item_type,data_type> item, generic_property<std::int64_t,active_type> active_index, std::uint64_t icon) = 0;
        virtual void draw_filter(python_object ctx, ui_layout layout) {}
        virtual python_tuple filter_items(python_object ctx, generic_property<item_type,data_type> item) { return python_tuple(); }

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
                crtp(self.get_pyobject()).draw_item(
                    ctx,
                    layout.get_pyobject(),
                    generic_property_readonly<item_type,data_type>(data, item),
                    generic_property<std::int64_t,active_type>(active_data, details::py2cxx<std::string>(active_propname)),
                    details::py2cxx<std::uint64_t>(icon)
                );
            });

            builder.set_draw_filter([this](python_object self, python_object ctx, python_object layout) {
                crtp(self.get_pyobject()).draw_filter(ctx,layout.get_pyobject());
            });

            if (!std::is_same<decltype(& crtp::filter_items), decltype(&ui_list::filter_items)>::value)
            {
                builder.set_filter_items([this](python_object self, python_object ctx, python_object data, python_object propname) {
                    return crtp(self.get_pyobject()).filter_items(ctx, generic_property<item_type,data_type>(data, details::py2cxx<std::string>(propname)));
                });
            }
        }
    };
}