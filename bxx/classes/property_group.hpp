#pragma once

#include "property_classes.hpp"
#include "../builders/property_group_builder.hpp"

#include "magic_enum.hpp"

#include "../id.hpp"

#define PROPERTY_GROUP(cls,...)\
    static void register_class()\
    {\
        cls().register_class_internal();\
    }\
    std::string get_class_name()\
    {\
        return #cls;\
    }\
    cls()\
        : __VA_ARGS__\
    {\
    }\
    cls(PyObject* obj)\
        : __VA_ARGS__\
    {\
        details::replace_python_object(*this,obj);\
    }\
    template <typename T>\
    cls(T const& obj)\
        : __VA_ARGS__\
    {\
        details::replace_python_object(*this,obj.getattr<python_object>(#cls).get_pyobject());\
    }

namespace bxx
{
    enum class property_group_targets
    {
        Object,
        Scene,
        Light,
        Material,
        Image
    };

    template <typename T>
    class property_group_class : public property_class
    {
    public:
        T get(id const& id)
        {
            id.getattr<T>(get_class_name());
        }

        T get(python_object const& obj)
        {
            return obj.getattr<T>(get_class_name());
        }
    protected:
        virtual std::vector<property_group_targets> targets() = 0;

        void register_class_internal() final
        {
            class_property_builder tmp_builder;
            for (property_base* base : m_properties)
            {
                base->write_to(tmp_builder);
            }
            property_group_builder builder(get_class_name(), true);
            builder.read_properties_from(tmp_builder);
            for (property_group_targets target: targets())
            {
                builder.register_to("bpy.types." + std::string(magic_enum::enum_name(target)), get_class_name(), false);
            }
        }
    };
}