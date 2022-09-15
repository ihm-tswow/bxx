#include <bxx/builders/property_group_builder.hpp>
#include <bxx/builders/string_builder.hpp>
#include <bxx/script.hpp>
#include <common/exec.hpp>

namespace bxx
{
    property_group_builder::property_group_builder(std::string const& class_name, bool should_register)
        : class_header_builder<property_group_builder>(class_name)
        , m_should_register(should_register)
    {
        add_parent_class("bpy.types.PropertyGroup");
    }

    property_group_builder::~property_group_builder()
    {
        if (m_should_register)
        {
            python_builder builder;
            write(builder);
            exec({ builder.get_code() });
        }
    }

    property_group_builder& property_group_builder::register_to(std::string const& target, std::string name, bool collection)
    {
        if (name.size() == 0)
        {
            name = get_class_name();
        }
        m_registered_to[target] = { name,collection };
        return *this;
    }

    void property_group_builder::write(python_builder& builder)
    {
        class_header_builder::write(builder, [&]() {
            property_builder::write(builder);
        });

        builder.write_line("bpy.utils.register_class({})", get_class_name());
        for (auto const& [target,reg] : m_registered_to)
        {
            builder.write_line("register_property_group({},{},\"{}\",{},{})"
                ,get_script_index()
                ,target
                ,string_to_python_string(reg.m_name)
                ,get_class_name()
                ,reg.is_collection ? "True" : "False"
            );
        }
    }
}