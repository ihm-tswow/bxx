#include "operator_builder.hpp"
#include "exec.hpp"
#include "../script.hpp" // include dir not working for this one file for some bizarre reason
#include "python_builder.hpp"

#include "fmt/core.h"
#include <nlohmann/json.hpp>

#include <algorithm>
#include <sstream>
#include <map>

static std::map < std::string, std::function<void(nlohmann::json)>> operator_map;
std::function<void(nlohmann::json)> bxx::operators::get_callback(std::string const& id)
{
    auto itr = operator_map.find(id);
    return itr != operator_map.end() ? itr->second : nullptr;
}

static std::string id_to_class_name(std::string str)
{
    std::replace(str.begin(), str.end(), '.', '_');
    return fmt::format("op_{}", str);
}

bxx::operator_builder::operator_builder(std::string const& bl_id, bool registers)
    : class_header_builder(id_to_class_name(bl_id))
    , m_registers(registers)
{
    m_bl_id = bl_id;
    set_class_variable("bl_idname", "bxx."+bl_id);
    set_class_variable("bl_label", bl_id);
    set_class_variable("bl_description", "description");
    set_class_variable("bl_options", [](set_builder& set) { set
        .set_bracket_type(python_builder::squiggly_brackets)
        .insert("REGISTER")
        ;
    });
    add_parent_class("bpy.types.Operator");
}

bxx::operator_builder& bxx::operator_builder::set_description(std::string const& description)
{
    set_class_variable("bl_description", description);
    return *this;
}

bxx::operator_builder& bxx::operator_builder::set_callback(std::function<void(nlohmann::json)> callback)
{
    operator_map[m_bl_id] = callback;
    return *this;
}

bxx::operator_builder& bxx::operator_builder::add_option(std::string const& option)
{
    m_options.insert(option);
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

void bxx::operator_builder::write(bxx::python_builder& builder)
{
    class_header_builder::write(builder, [&]() {
        property_builder<operator_builder>::write(builder);
        builder.end_line();

        builder.write_line("@classmethod");
        builder.begin_line("def poll(cls, context):");
        {
            auto block = builder.scoped_block(python_builder::no_brackets, 1);
            builder.write_line("return True");
        }

        if (m_properties.size() > 0)
        {
            builder.begin_line("def invoke(self, context, event):");
            {
                auto block = builder.scoped_block(python_builder::no_brackets, 1);
                builder.write_line("return context.window_manager.invoke_props_dialog(self)");
            }

            builder.begin_line("def draw(self, context):");
            {
                auto block = builder.scoped_block(python_builder::no_brackets, 1);
                for_each_property([&](property_entry& entry, bool last) {
                    std::optional<std::string> const& draw = entry.get_draw_instruction();
                    if (draw.has_value())
                    {
                        builder.write_line("{}", draw.value());
                    }
                    else
                    {
                        std::string val = entry.get_option_or("name", entry.get_id());
                        builder.write_line("self.layout.prop(self,'{}', text = \"{}\");", entry.get_id(), val);
                    }
                });
            }
        }

        builder.begin_line("def execute(self, context):");
        {
            auto block = builder.scoped_block(python_builder::no_brackets,1);
            builder.begin_line("parameter_names = ");
            {
                auto block = builder.scoped_block(python_builder::square_brackets, 1);
                for_each_property([&](property_entry& entry, bool last) {
                    auto line = builder.scoped_line("'{}'", entry.get_id());
                    if (!last)
                    {
                        builder.write(",");
                    }
                    });
            }
            builder.write_line("fire_operator(self, {}, '{}', parameter_names)", get_script_index(), m_bl_id);
            builder.begin_line("return {{'FINISHED'}}");
        }

    });
    builder.begin_line("def show_{}(self,context):", get_class_name());
    {
        auto block = builder.scoped_block(python_builder::no_brackets, 1);
        builder.write_line("self.layout.operator({}.bl_idname)", get_class_name());
    }

    builder.write_line("register_operator({},'{}',{},show_{})", get_script_index(), m_bl_id, get_class_name(), get_class_name());
}


bxx::operator_builder::~operator_builder()
{
    if (m_registers)
    {
        bxx::python_builder builder;
        write(builder);
        exec(builder.get_code());
    }
}

