#pragma once

#include "property_builder.hpp"
#include "class_header_builder.hpp"

#include <string>
#include <set>

namespace bxx
{
    struct property_group_registry
    {
        std::string m_name;
        bool is_collection;
    };

    class property_group_builder
        : public property_builder<property_group_builder>
        , public class_header_builder<property_group_builder>
    {
    public:
        property_group_builder(std::string const& class_name, bool should_register = true);
        ~property_group_builder();
        property_group_builder& add_pointer_property(std::string const& id, std::string const& type, std::string const& name, std::string const& description = "");
        property_group_builder& add_collection_property(std::string const& id, std::string const& type, std::string const& name, std::string const& description = "");
        property_group_builder& register_to(std::string const& target, std::string name = "", bool collection = false);

        void write(python_builder& builder);
    private:
        bool m_should_register;
        std::map<std::string, property_group_registry> m_registered_to;
    };
}