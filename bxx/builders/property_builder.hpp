#pragma once

#include "value_builder.hpp"
#include "python_builder.hpp"

#include <string>
#include <map>
#include <optional>
#include <cassert>

namespace bxx
{
    struct enum_entry
    {
        std::string m_id;
        std::string m_name = m_id;
        std::string m_description;
        std::string m_icon;
        std::optional<std::int64_t> m_value;

        enum_entry(std::string id, std::string name, std::string description, std::string icon, std::int64_t value)
            : m_id(id)
            , m_name(name)
            , m_description(description)
            , m_icon(icon)
            , m_value(value)
        {}

        enum_entry(std::string id, std::string name, std::string description, std::string icon)
            : m_id(id)
            , m_name(name)
            , m_description(description)
            , m_icon(icon)
        {}

        enum_entry(std::string id, std::string name, std::string description)
            : m_id(id)
            , m_name(name)
            , m_description(description)
            , m_icon("")
        {}

        enum_entry(std::string id, std::string name)
            : m_id(id)
            , m_name(name)
            , m_description("")
            , m_icon("")
        {}

        enum_entry(std::string id)
            : m_id(id)
            , m_name(id)
            , m_description("")
            , m_icon("")
        {}

    };

    class property_entry
    {
    public:
        property_entry(std::string const& id, std::string const& type);

        template <typename T>
        property_entry& add_option(std::string const& key, T const& value)
        {
            m_options.emplace(key,value);
            return *this;
        }

        std::string get_id() const
        {
            return m_id;
        }

        std::string get_type() const
        {
            return m_type;
        }

        std::optional<std::string>& get_draw_instruction()
        {
            return m_draw_instruction;
        }

        property_entry& clear_draw_instruction()
        {
            m_draw_instruction->clear();
            return *this;
        }

        property_entry& set_draw_instruction(std::string const& python_code)
        {
            m_draw_instruction.emplace(python_code);
            return *this;
        }

        template <typename T>
        T const& get_option_or(std::string const& name, T const& def) const
        {
            auto itr = m_options.find(name);
            if (itr == m_options.end())
            {
                return def;
            }
            else
            {
                return itr->second.get<T>();
            }
        }

        void for_each_option(std::function<void(std::string const& id, builder_value & option, bool last)> callback)
        {
            size_t i = 0;
            for (auto & [key, value] : m_options)
            {
                callback(key, value, i == m_options.size() - 1);
                ++i;
            }
        }
    private:
        std::string m_id;
        std::string m_type;
        std::optional<std::string> m_draw_instruction;
        std::map<std::string, builder_value> m_options;
    };

    class property_builder_base
    {
    protected:

    };

    template <typename T>
    class property_builder
    {
    public:
        template <typename G>
        void read_properties_from(property_builder<G>& source)
        {
            m_properties = source.m_properties;
        }

        property_builder<T>() = default;

        virtual ~property_builder<T>(){}
        void write(python_builder & builder)
        {
            for (std::shared_ptr<property_entry>& entry : m_properties)
            {
                builder.begin_line("{}: {}", entry->get_id(), entry->get_type());
                auto block = builder.scoped_block(python_builder::round_brackets, 2);
                entry->for_each_option([&](std::string const& key, builder_value& option, bool last)
                    {
                        auto line = builder.scoped_line("{} = ", key);
                        option.write(builder);
                        if (!last)
                        {
                            builder.write(",");
                        }
                    });
            }
        }

        void for_each_property(std::function<void(property_entry& prop, bool is_last)> callback)
        {
            size_t i = 0;
            for (std::shared_ptr<property_entry>& entry : m_properties)
            {
                callback(*entry.get(), i == m_properties.size() - 1);
                ++i;
            }
        }

        T& add_string_property(std::string const& id, std::string const& name, std::string const& def, std::string const& description = "")
        {
            return add_property(id, "bpy.props.StringProperty", [&](property_entry& entry) { entry
                .add_option("name", name)
                .add_option("description", description)
                .add_option("default", def)
                ;
            });
        }

        T& add_int_property(std::string const& id, std::string const& name, int min, int def, int max, std::string const& description = "")
        {
            return add_property(id, "bpy.props.IntProperty", [&](property_entry& entry) { entry
                .add_option("name", name)
                .add_option("description", description)
                .add_option("default", def)
                .add_option("min", min)
                .add_option("max", max)
                ;
            });
        }

        T& add_float_property(std::string const& id, std::string const& name, float min, float def, float max, std::string const& description = "")
        {
            return add_property(id, "bpy.props.FloatProperty", [&](property_entry& entry) { entry
                .add_option("name", name)
                .add_option("description", description)
                .add_option("default", def)
                .add_option("min", min)
                .add_option("max", max)
                ;
            });
        }

        T& add_bool_property(std::string const& id, std::string const& name, bool def, std::string const& description = "")
        {
            return add_property(id, "bpy.props.BoolProperty", [&](property_entry& entry){ entry
                .add_option("name", name)
                .add_option("description", description)
                .add_option("default",def)
                ;
            });
        }

        T& add_enum_property(std::string const& id, std::string const& name, std::string const& def, std::vector<enum_entry> const& values, std::string const& description = "")
        {
            return add_property(id, "bpy.props.EnumProperty", [&](property_entry& entry) { entry
                .add_option("name", name)
                .add_option("description", description)
                .add_option("default", def)
                .add_option("items", [&](list_builder& builder) {
                    std::int64_t curMax = 0;
                    for (enum_entry const& entry : values)
                    {
                        builder.add([&](list_builder& map) { map
                            .set_bracket_type(python_builder::round_brackets)
                            .add(entry.m_id)
                            .add(entry.m_name)
                            .add(entry.m_description)
                            .add(entry.m_icon)
                            ;
                            if (entry.m_value.has_value())
                            {
                                curMax = entry.m_value.value();
                            }
                            map.add(curMax++);
                        });
                    }
                })
                ;
            });
        }

        T& add_enum_property(std::string const& id, std::string const& name, std::vector<enum_entry> const& values, std::string const& description = "")
        {
            return add_enum_property(id, name, values.begin()->m_id, values, description);
        }

        T& add_dynamic_enum_property(std::string const& id, std::string const& name, std::string const& code, std::string const& description = "")
        {
            return add_property(id, "bpy.props.EnumProperty", [&](property_entry& entry) { entry
                .add_option("name", name)
                .add_option("description", description)
                .add_option("items", python_code(code))
                ;
            });
        }

        T& add_property(std::string const& id, std::string const& type, std::function<void(property_entry&)> callback)
        {
            callback(*m_properties.emplace_back( std::make_shared<property_entry>(id, type) ).get());
            return *dynamic_cast<T*>(this);
        }
        std::vector<std::shared_ptr<property_entry>> m_properties;
    };
}