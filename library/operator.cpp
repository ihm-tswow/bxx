#include "operator.hpp"
#include "exec.hpp"
#include "script.hpp"

#include "fmt/core.h"

#include <algorithm>
#include <sstream>
#include <map>

static std::map < std::string, std::function<void(nlohmann::json)>> operator_map;
std::function<void(nlohmann::json)> bxx::operators::get_callback(std::string const& id)
{
    auto itr = operator_map.find(id);
    return itr != operator_map.end() ? itr->second : nullptr;
}

bxx::operator_builder::operator_builder(std::string const& bl_id)
    : m_bl_id(bl_id)
    , m_name(bl_id)
{}

bxx::operator_builder bxx::operators::create(std::string const& id)
{
    return bxx::operator_builder{ id };
}

bxx::operator_builder& bxx::operator_builder::set_name(std::string const& name)
{
    m_name = name;
    return *this;
}

bxx::operator_builder& bxx::operator_builder::set_description(std::string const& description)
{
    m_description = description;
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_string_parameter(std::string const& id, std::string const& name, std::string const& def, std::string const& description)
{
    add_parameter(id, "bpy.props.StringProperty", {
        {"name",name},
        {"description",description},
        {"default",def},
    });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_int_parameter(std::string const& id, std::string const& name, int min, int def, int max, std::string const& description)
{
    add_parameter(id, "bpy.props.IntProperty", {
        {"name",name},
        {"description",description},
        {"default",def},
        {"min",min},
        {"max",max}
    });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_float_parameter(std::string const& id, std::string const& name, float min, float def, float max, std::string const& description)
{
    add_parameter(id, "bpy.props.FloatProperty", {
        {"name",name},
        {"description",description},
        {"default",def},
        {"min",min},
        {"max",max}
    });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_bool_parameter(std::string const& id, std::string const& name, bool def, std::string const& description)
{
    add_parameter(id, "bpy.props.BoolProperty", {
        {"name",name},
        {"description",description},
        {"default",def},
    });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_enum_parameter(std::string const& id, std::string const& name, std::string const& def, std::initializer_list<operator_enum_entry> const& values, std::string const& description)
{
    nlohmann::json items = {};
    for (operator_enum_entry const& entry : values)
    {
        items.push_back({ {
            "__tuple",{entry.m_id, entry.m_name, entry.m_description}
        } });
    }
    add_parameter(id, "bpy.props.EnumProperty", {
        {"name", name},
        {"description", description},
        {"default",def},
        {"items",items}
    });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_enum_parameter(std::string const& id, std::string const& name, std::initializer_list<operator_enum_entry> const& values, std::string const& description)
{
    add_enum_parameter(id, name, values.begin()->m_id, values, description);
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_dynamic_enum_parameter(std::string const& id, std::string const& name, std::string const& code, std::string const& description)
{
    add_parameter(id, "bpy.props.EnumProperty", {
        {"name", name},
        {"description", description},
        {"items", {{"__code",code}}}
    });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_parameter(std::string const& id, std::string const& type, nlohmann::json properties)
{
    if (properties.find("name") == properties.end())
    {
        properties["name"] = id;
    }
    m_parameters[id] = { type,properties };
    m_draw_instructions.push_back({ id, false });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::set_callback(std::function<void(nlohmann::json)> callback)
{
    operator_map[m_bl_id] = callback;
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_parent_class(std::string const& parent)
{
    m_parents.insert(parent);
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_option(std::string const& option)
{
    m_options.insert(option);
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_parent_class(std::initializer_list<std::string > const& parents)
{
    m_parents.insert(parents);
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_option(std::initializer_list<std::string > const& options)
{
    m_options.insert(options);
    return *this;
}

bxx::operator_builder& bxx::operator_builder::begin_column()
{
    m_draw_instructions.push_back({ "cur = self.layout.row()", true });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::end_column()
{
    m_draw_instructions.push_back({ "cur = self.layout", true });
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_label(std::string const& label)
{
    m_draw_instructions.push_back({ fmt::format("cur.label(text='{}')",label), true});
    return *this;
}

std::string bxx::operator_builder::get_class_name()
{
    std::string id = m_bl_id;
    std::replace(id.begin(), id.end(), '.', '_');
    return fmt::format("op_{}", id);
}

template <typename T>
static void stream_set(std::stringstream& ss, std::string const& wrap, T const& set)
{
    int i = 0;
    for (std::string const& value : set)
    {
        ss << wrap << value << wrap;
        if (i++ < set.size() - 1) ss << ",";
    }
}

template <typename T>
static void stream_keys(std::stringstream& ss, std::string const& wrap, T const& set)
{
    int i = 0;
    for (auto const& [key, _] : set)
    {
        ss << wrap << key << wrap;
        if (i++ < set.size() - 1) ss << ",";
    }
}

static std::string json_to_python(nlohmann::json const& value)
{
    std::stringstream stream;
    if (value.is_object() && value.contains("__tuple"))
    {
        auto arr = value["__tuple"];
        stream << "(";
        for (auto& entry : arr)
        {
            stream << json_to_python(entry) << ",";
        }
        stream << ")";
        return stream.str();
    } 
    else if (value.is_object() && value.contains("__code"))
    {
        auto str = value["__code"];
        if (str.is_string())
        {
            stream << str.get<std::string>();
        }
    }
    else if (value.is_object())
    {
        stream << "{";
        for (auto& [key, value] : value.items())
        {
            stream << "\"" << key << "\" : " << json_to_python(value)<<",";
        }
        stream << "}";
    }
    else if (value.is_array())
    {
        stream << "[";
        for (auto& entry : value)
        {
            stream << json_to_python(entry) << ",";
        }
        stream << "]";
    }
    else if(value.is_boolean())
    {
        if (value)
        {
            return "True";
        }
        else
        {
            return "False";
        }
    }
    else
    {
        stream << value.dump();
    }
    return stream.str();
}

bxx::operator_builder::~operator_builder()
{
    std::stringstream stream;
    stream << "class " << get_class_name() << "(";
    stream_set(stream, "", m_parents);
    stream 
        << "):\n"
        << "    bl_idname = '" << m_bl_id << "'\n"
        << "    bl_label = '" << m_name << "'\n"
        << "    bl_description = '" << m_description << "'\n"
        << "    bl_options = {"
        ;
    stream_set(stream, "'", m_options);
    stream << "}\n";
    if (m_parameters.size() > 0)
    {
        for (auto& [key, value] : m_parameters)
        {
            stream << "    " << key << ": " << value.m_type << "(\n";
            for (auto const& [p_key, p_value] : value.m_properties.items())
            {
                stream << "        " << p_key << " = " << json_to_python(p_value) << ",\n";
            }
            stream << "    )\n\n";
        }

        stream
            << "    @classmethod\n"
            << "    def poll(cls, context):\n"
            << "        return True\n"
            << "\n"
            << "    def invoke(self, context, event):\n"
            << "        return context.window_manager.invoke_props_dialog(self)"
            << "\n"
            << "    def draw(self, context):\n"
            ;
        for (auto const& value : m_draw_instructions)
        {
            if (value.m_is_special)
            {
                stream << "        " << value.m_code << "\n";
            }
            else
            {
                stream 
                    << "        "
                    << "cur.prop(self, '"
                    << value.m_code
                    << "', text = "
                    << m_parameters[value.m_code].m_properties["name"] 
                    << ")\n";
            }
        }
    }
    stream 
        << "    def execute(self, context):\n"
        << "        parameter_names = ["
        ;
    stream_keys(stream, "'", m_parameters);
    stream
        << "]\n"
        << "        fire_operator(self, '" << bxx::cur_script_name() << "', '" << m_bl_id << "', parameter_names)\n"
        << "        return {'FINISHED'}\n"
        << "\n\n"
        << "def show_" << get_class_name() << "(self, context):\n"
        << "    self.layout.operator( " << get_class_name() << ".bl_idname)\n"
        << "register_operator('" << bxx::cur_script_name() << "','" << m_bl_id << "'," << get_class_name() << ", show_" << get_class_name() << ")\n";
        ;
    exec(stream.str());
}

