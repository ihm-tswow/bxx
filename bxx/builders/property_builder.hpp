#pragma once

#include <bxx/builders/value_builder.hpp>
#include <bxx/builders/python_builder.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_list.hpp>
#include <bxx/objects/python_tuple.hpp>

#include <bxx/script.hpp>

#include <string>
#include <map>
#include <optional>
#include <cassert>

namespace bxx
{
    enum class property_flag_items
    {
        HIDDEN,
        SKIP_SAVE,
        ANIMATABLE,
        LIBRARY_EDITABLE,
        PROPORTIONAL, // Adjust values proportionally to each other.
        TEXTEDIT_UPDATE // Update on every keystroke in textedit ÅemodeÅf.
    };

    enum class property_flag_enum_items
    {
        HIDDEN,
        SKIP_SAVE,
        ANIMATABLE,
        LIBRARY_EDITABLE,
        ENUM_FLAG
    };

    enum class library_flag_items
    {
        LIBRARY_OVERRIDABLE
    };

    enum class number_subtype
    {
        PIXEL,
        UNSIGNED,
        PERCENTAGE,
        FACTOR,
        ANGLE,
        TIME, // Time specified in frames, converted to seconds based on scene frame rate.
        TIME_ABSOLUTE, // Time specified in seconds, independent of the scene.
        DISTANCE,
        DISTANCE_CAMERA,
        POWER,
        TEMPERATURE,
        NONE
    };

    enum class number_array_subtype
    {
        COLOR,
        TRANSLATION,
        DIRECTION,
        VELOCITY,
        ACCELERATION,
        MATRIX,
        EULER,
        QUATERNION,
        AXISANGLE,
        XYZ,
        XYZ_LENGTH,
        COLOR_GAMMA,
        COORDINATES,
        LAYER,
        LAYER_MEMBER,
        NONE
    };

    enum class string_subtype
    {
        FILE_PATH,
        DIR_PATH,
        FILE_NAME,
        BYTE_STRING,
        PASSWORD, // A string that is displayed hidden(Åf******** Åf).
        NONE
    };

    enum class string_search_options
    {
        SORT,       // sorts resulting items
        SUGGESTION  // allow entering values not found in search candidates
    };

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

    class property_entry_base_base
    {
    public:
        property_entry_base_base(property_entry_base_base const&) = delete;
        property_entry_base_base(property_entry_base_base&&) = delete;
        property_entry_base_base(std::string const& id, std::string const& type)
            : m_id(id)
            , m_type(type)
        {}

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

        void for_each_option(std::function<void(std::string const& id, builder_value& option, bool last)> callback)
        {
            size_t i = 0;
            for (auto& [key, value] : m_options)
            {
                callback(key, value, i == m_options.size() - 1);
                ++i;
            }
        }
    protected:
        std::string m_id;
        std::string m_type;
        std::optional<std::string> m_draw_instruction;
        std::map<std::string, builder_value> m_options;
    };

    template <typename crtp>
    class property_entry_base : public property_entry_base_base
    {
    public:
        using property_entry_base_base::property_entry_base_base;

        template <typename T>
        crtp& set_attribute(std::string const& key, T const& value)
        {
            m_options.emplace(key,value);
            return *static_cast<crtp*>(this);
        }

        crtp& set_override_options(std::initializer_list<library_flag_items> items)
        {
            set_attribute("override", [&](set_builder& set) {
                set.set_bracket_type(python_builder::squiggly_brackets);
                for (library_flag_items item : items)
                {
                    set.insert(enums::get_enum_name<library_flag_items>(item));
                }
                });
            return *static_cast<crtp*>(this);
        }

        crtp& clear_draw_instruction()
        {
            m_draw_instruction->clear();
            return *static_cast<crtp*>(this);
        }

        crtp& set_draw_instruction(std::string const& python_code)
        {
            m_draw_instruction.emplace(python_code);
            return *static_cast<crtp*>(this);
        }
    };

    template <typename crtp, typename option_type>
    class property_options
    {
        crtp& set_property_options(std::initializer_list<option_type> items)
        {
            return static_cast<crtp*>(this)->set_attribute("options", [&](set_builder& set) {
                set.set_bracket_type(python_builder::squiggly_brackets);
                for (option_type item : items)
                {
                    set.insert(enums::get_enum_name<option_type>(item));
                }
            });
        }
    };

    template <typename crtp>
    class property_entry_update
    {
        crtp& set_update(std::function<void(python_object, python_object)> callback)
        {
            size_t event_index = lib_register_event([=](python_tuple args) {
                callback(args.get(0), args.get(1));
                return python_object();
                });
            return static_cast<crtp*>(this)->set_attribute("update", python_code(fmt::format("lambda x,y: fire_event({},{},x,y)", get_script_index(), event_index)));
        }
    };

    template <typename crtp>
    class property_entry_dynamic_access : public property_entry_update<crtp>
    {
        crtp& set_setter(std::function<void(python_object, python_object)> callback)
        {
            size_t event_index = lib_register_event([=](python_tuple args) {
                callback(args.get(0), args.get(1));
                return python_object();
                });
            return static_cast<crtp*>(this)->set_attribute("set", python_code(fmt::format("lambda x,y: fire_event({},{},x,y)", get_script_index(), event_index)));
        }

        crtp& set_getter(std::function<python_object(python_object)> callback)
        {
            size_t event_index = lib_register_event([=](python_tuple args) {
                return callback(args.get(0));
            });
            return static_cast<crtp*>(this)->set_attribute("get", python_code(fmt::format("lambda x,y: fire_event({},{},x,y)", get_script_index(), event_index)));
        }
    };

    template <typename crtp, typename num_type>
    class property_entry_number
        : public property_options<crtp, property_flag_items>
    {
        crtp& set_soft_min(num_type soft_min)
        {
            return static_cast<crtp*>(this)->set_attribute("soft_min", soft_min);
        }

        crtp& set_soft_max(num_type soft_max)
        {
            return static_cast<crtp*>(this)->set_attribute("soft_max", soft_max);
        }

        crtp& set_step(num_type step)
        {
            return static_cast<crtp*>(this)->set_attribute("step", step);
        }
    };

    template <typename crtp, typename num_type>
    class property_entry_number_primitive
        : public property_entry_number<crtp,num_type>
        , public property_entry_dynamic_access<crtp>
    {
        crtp& set_subtype(number_subtype type)
        {
            return static_cast<crtp*>(this)->set_attribute("subtype", enums::get_enum_name<number_subtype>(type));
        }
    };

    template <typename crtp, typename num_type>
    class property_entry_number_vector
        : public property_entry_number<crtp,num_type>
        , public property_entry_update<crtp>
    {
        crtp& set_subtype(number_array_subtype type)
        {
            return static_cast<crtp*>(this)->set_attribute("subtype", enums::get_enum_name<number_array_subtype>(type));
        }
    };

    template <typename crtp>
    class property_entry_float_base
    {
        crtp& set_precision(std::uint32_t precision)
        {
            return static_cast<crtp*>(this)->set_attribute("precision", precision);
        }
    };
    
    class property_entry_string
        : public property_entry_base<property_entry_string>
        , public property_entry_dynamic_access<property_entry_string>
        , public property_options<property_entry_string, property_flag_items>
    {
        property_entry_string& set_subtype(string_subtype type)
        {
            return set_attribute("subtype", enums::get_enum_name<string_subtype>(type));
        }

        property_entry_string& set_search_options(std::initializer_list<string_search_options> const& options)
        {
            return set_attribute("search_options", [&](set_builder& set) {
                for (string_search_options option : options)
                {
                    set.insert(enums::get_enum_name<string_search_options>(option));
                }
            });
        }

        property_entry_string& set_search(std::function<python_object(python_object, python_object, python_object)> search_callback)
        {
            size_t event_index = lib_register_event([=](python_tuple args) {
                return search_callback(args.get(0), args.get(1), args.get(2));
                });
            return set_attribute("search", python_code(fmt::format("lambda x,y,z: fire_event({},{},x,y,z)", get_script_index(), event_index)));
        }
    };

    class property_entry_float
        : public property_entry_base<property_entry_float>
        , public property_entry_number_primitive<property_entry_float,double>
        , public property_entry_float_base<property_entry_float>
    {
        using property_entry_base<property_entry_float>::property_entry_base;
    };

    class property_entry_int
        : public property_entry_base<property_entry_int>
        , public property_entry_number_primitive<property_entry_int,std::int64_t>
    {
        using property_entry_base<property_entry_int>::property_entry_base;
    };

    class property_entry_bool
        : public property_entry_base<property_entry_bool>
        , public property_entry_dynamic_access<property_entry_bool>
    {
        using property_entry_base<property_entry_bool>::property_entry_base;
    };

    class property_entry_pointer
        : public property_entry_base<property_entry_pointer>
        , public property_entry_update<property_entry_pointer>
        , public property_options<property_entry_pointer, property_flag_items>
    {
        using property_entry_base<property_entry_pointer>::property_entry_base;
        property_entry_pointer& set_poll(std::function<bool(python_object, python_object)> callback)
        {
            size_t event_index = lib_register_event([=](python_tuple args) {
                return callback(args.get(0), args.get(1));
            });
            return set_attribute("poll", python_code(fmt::format("lambda x,y: fire_event({},{},x,y)", get_script_index(), event_index)));
        }
    };

    class property_entry_enum
        : public property_entry_base<property_entry_enum>
        , public property_entry_update<property_entry_enum>
        , public property_options<property_entry_enum, property_flag_enum_items>
    {
        using property_entry_base<property_entry_enum>::property_entry_base;
    };

    class property_entry : public property_entry_base<property_entry>
    {
        using property_entry_base<property_entry>::property_entry_base;
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
            for (std::shared_ptr<property_entry_base_base>& entry : m_properties)
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

        void for_each_property(std::function<void(property_entry_base_base& prop, bool is_last)> callback)
        {
            size_t i = 0;
            for (std::shared_ptr<property_entry_base_base>& entry : m_properties)
            {
                callback(*entry.get(), i == m_properties.size() - 1);
                ++i;
            }
        }

        T& add_string_property(std::string const& id, std::string const& name, std::string const& description, std::string const& def, std::function<void(property_entry_string&)> callback = nullptr)
        {
            return add_property<property_entry_string>(id, "bpy.props.StringProperty", [&](property_entry_string& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("default", def)
                ;
                if (callback)
                {
                    callback(entry);
                }
            });
        }

        T& add_int_property(std::string const& id, std::string const& name, std::string const& description, int min, int def, int max, std::function<void(property_entry_int&)> callback = nullptr)
        {
            return add_property<property_entry_int>(id, "bpy.props.IntProperty", [&](property_entry_int& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("default", def)
                .set_attribute("min", min)
                .set_attribute("max", max)
                ;
                if (callback)
                {
                    callback(entry);
                }
            });
        }

        T& add_float_property(std::string const& id, std::string const& name, std::string const& description, float min, float def, float max, std::function<void(property_entry_float&)> callback = nullptr)
        {
            return add_property<property_entry_float>(id, "bpy.props.FloatProperty", [&](property_entry_float& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("default", def)
                .set_attribute("min", min)
                .set_attribute("max", max)
                ;
                if (callback)
                {
                    callback(entry);
                }
            });
        }

        T& add_bool_property(std::string const& id, std::string const& name, std::string const& description, bool def, std::function<void(property_entry_bool&)> callback = nullptr)
        {
            return add_property<property_entry_bool>(id, "bpy.props.BoolProperty", [&](property_entry_bool& entry){ entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("default",def)
                ;
                if (callback)
                {
                    callback(entry);
                }
            });
        }

        T& add_mask_property(std::string const& id, std::string const& name, std::string const& description, std::vector<std::string> const& def, std::vector<enum_entry> const& values, std::function<void(property_entry_enum&)> callback = nullptr)
        {
            return add_property<property_entry_enum>(id, "bpy.props.EnumProperty", [&](property_entry_enum& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("options", [&](set_builder& builder) { builder
                    .set_bracket_type(python_builder::squiggly_brackets)
                    .insert("ENUM_FLAG")
                    ;
                })
                .set_attribute("items", [&](list_builder& builder) {
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

                if (def.size() > 0)
                {
                    entry.set_attribute("default", [&](set_builder& builder) {
                        for (std::string const& str : def)
                        {
                            builder.insert(str);
                        }
                    });
                }

                if (callback)
                {
                    callback(entry);
                }
            });
        }

        T& add_enum_property(std::string const& id, std::string const& name, std::string const& description, std::string const& def, std::vector<enum_entry> const& values, std::function<void(property_entry_enum&)> callback = nullptr)
        {
            return add_property<property_entry_enum>(id, "bpy.props.EnumProperty", [&](property_entry_enum& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("default", def)
                .set_attribute("items", [&](list_builder& builder) {
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
                if (callback)
                {
                    callback(entry);
                }
            });
        }

        T& add_enum_property(std::string const& id, std::string const& name, std::string const& description, std::vector<enum_entry> const& values, std::function<void(property_entry_enum&)> callback = nullptr)
        {
            return add_enum_property(id, name, description, values.begin()->m_id, values, callback);
        }

        T& add_dynamic_enum_property(std::string const& id, std::string const& name, std::string const& description, std::function<std::vector<enum_entry>(python_object,python_object)> callback, std::function<void(property_entry_enum&)> builder_callback = nullptr)
        {
            size_t event_index = lib_register_event([=](python_tuple args) {
                std::vector<enum_entry> vec = callback(args.get<python_object>(0), args.get<python_object>(1));
                python_list list;
                size_t cur_index = 0;
                for (enum_entry const& entry : vec)
                {
                    python_tuple tup(5);
                    if (entry.m_value.has_value())
                    {
                        cur_index = entry.m_value.value();
                    }
                    tup.set<std::string>(0, entry.m_id);
                    tup.set<std::string>(1, entry.m_name);
                    tup.set<std::string>(2, entry.m_description);
                    tup.set<std::string>(3, entry.m_icon);
                    tup.set<std::int64_t>(4, cur_index++);
                    list.append(tup);
                }
                return list;
            });

            return add_property<property_entry_enum>(id, "bpy.props.EnumProperty", [&](property_entry_enum& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("items", python_code(fmt::format("lambda x,y: fire_event({},{},x,y)", get_script_index(), event_index)))
                ;
                if (builder_callback)
                {
                    builder_callback(entry);
                }
            });
        }

        T& add_dynamic_enum_property(std::string const& id, std::string const& name, std::string const& description, std::string const& code, std::function<void(property_entry_enum&)> callback = nullptr)
        {
            return add_property<property_entry_enum>(id, "bpy.props.EnumProperty", [&](property_entry_enum& entry) { entry
                .set_attribute("name", name)
                .set_attribute("description", description)
                .set_attribute("items", python_code(code))
                ;
                if (callback)
                {
                    callback(entry);
                }
            });
        }

        template <typename prop_type = property_entry>
        T& add_property(std::string const& id, std::string const& type, std::function<void(prop_type&)> callback)
        {
            std::shared_ptr<prop_type> ptr = std::make_shared<prop_type>(id, type);
            callback(*ptr.get());
            m_properties.push_back(ptr);
            return *dynamic_cast<T*>(this);
        }
        std::vector<std::shared_ptr<property_entry_base_base>> m_properties;
    };
}