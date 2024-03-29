#pragma once

#include <bxx/classes/property_classes.hpp>
#include <bxx/builders/property_group_builder.hpp>

#define PROPERTY_GROUP(cls,...)\
    static void register_class()\
    {\
        cls().register_class_internal();\
    }\
    using ref = bxx::property_entry_ref<#cls,cls>;\
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
        bxx::details::replace_python_object(*this,obj);\
    }\
    template <typename T>\
    cls(T obj)\
        : __VA_ARGS__\
    {\
        bxx::details::replace_python_object(*this,obj.get_pyobject());\
    }

namespace bxx
{
    enum class property_group_targets
    {
        Object,
        Scene,
        Light,
        Material,
        Image,
        EditBone
    };

    template <typename T>
    class property_group_class : public property_class
    {
    public:
        T get(python_object obj)
        {
            return obj.getattr<T>(get_class_name());
        }
    protected:
        // empty by default because property groups don't need to attach to anything (can be used in Collection properties etc)
        virtual std::vector<property_group_targets> targets() { return {}; };

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
                builder.register_to("bpy.types." + std::string(enums::get_enum_name(target)), get_class_name(), false);
            }
        }
    };
}