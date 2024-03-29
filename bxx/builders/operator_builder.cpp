#include <bxx/builders/operator_builder.hpp>
#include <bxx/builders/python_builder.hpp>
#include <bxx/objects/python_tuple.hpp>
#include <bxx/script.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <sstream>
#include <map>

namespace bxx
{
    static std::string id_to_class_name(std::string str)
    {
        std::replace(str.begin(), str.end(), '.', '_');
        return fmt::format("op_{}", str);
    }

    operator_builder::operator_builder(std::string const& bl_id, bool registers)
        : class_header_builder(id_to_class_name(bl_id))
        , m_registers(registers)
    {
        m_bl_id = "object." + bl_id;
        set_class_variable("bl_description", "description");
        set_class_variable("bl_options", [](set_builder& set) { set
            .set_bracket_type(python_builder::squiggly_brackets)
            .insert("REGISTER")
            ;
            });
        add_parent_class("bpy.types.Operator");
    }

    operator_builder& operator_builder::set_description(std::string const& description)
    {
        set_class_variable("bl_description", description);
        return *this;
    }

    operator_builder& operator_builder::set_invoke(std::function<python_object(python_object, python_object, python_object)> invoke)
    {
        m_invoke = invoke;
        return *this;
    }

    operator_builder& operator_builder::set_callback(std::function<void(python_object)> callback)
    {
        m_callback = callback;
        return *this;
    }


    operator_builder& operator_builder::set_draw(std::function<void(python_object)> callback)
    {
        m_draw = callback;
        return *this;
    }


    operator_builder& operator_builder::set_attribute(std::string const& option)
    {
        m_options.insert(option);
        return *this;
    }

    operator_builder& operator_builder::set_attribute(std::initializer_list<std::string > const& options)
    {
        m_options.insert(options);
        return *this;
    }

    operator_builder& operator_builder::begin_column()
    {
        m_draw_instructions.push_back({ "cur = self.layout.row()", true });
        return *this;
    }

    operator_builder& operator_builder::end_column()
    {
        m_draw_instructions.push_back({ "cur = self.layout", true });
        return *this;
    }

    operator_builder& operator_builder::add_label(std::string const& label)
    {
        m_draw_instructions.push_back({ fmt::format("cur.label(text='{}')",label), true });
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

    void operator_builder::write(python_builder& builder)
    {
        auto cb = m_callback;
        size_t event_index = lib_register_event([cb](python_tuple args) {
            cb(python_object(args.get<python_object>(0)));
            return python_object();
        });

        class_header_builder::write(builder, [&]() {
            property_builder<operator_builder>::write(builder);
            builder.end_line();

            builder.write_line("@classmethod");
            builder.begin_line("def poll(cls, context):");
            {
                auto block = builder.scoped_block(python_builder::no_brackets, 1);
                builder.write_line("return True");
            }

            if ((m_properties.size() > 0 || m_draw) && !m_disable_drawing)
            {
                builder.begin_line("def invoke(self, context, event):");
                {
                    auto block = builder.scoped_block(python_builder::no_brackets, 1);
                    if (m_invoke)
                    {
                        auto invoke = m_invoke;
                        size_t invoke_id = lib_register_event([invoke](python_tuple args) {
                            return invoke(args.get_item(0), args.get_item(1), args.get_item(2));
                        });
                        builder.write_line("return fire_event({},{},self,context,event)", get_script_index(), invoke_id);
                    }
                    else
                    {
                        builder.write_line("return context.window_manager.invoke_props_dialog(self)");
                    }
                }

                builder.begin_line("def draw(self, context):");
                auto block = builder.scoped_block(python_builder::no_brackets, 1);

                if (m_draw)
                {
                    auto draw = m_draw;
                    size_t draw_id = lib_register_event([draw](python_tuple tup) {
                        draw(tup.get<python_object>(0));
                        return python_object();
                        });
                    builder.write_line("fire_event({},{},self)", get_script_index(), draw_id);
                }
                else
                {
                    // auto-draw
                    for_each_property([&](property_entry_base_base& entry, bool last) {
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
                auto block = builder.scoped_block(python_builder::no_brackets, 1);
                builder.write_line("fire_event(int({}), int({}), self)", get_script_index(), event_index);
                builder.begin_line("return {{'FINISHED'}}");
            }

            });
        builder.begin_line("def show_{}(self,context):", get_class_name());
        {
            auto block = builder.scoped_block(python_builder::no_brackets, 1);
            builder.write_line("self.layout.operator({}.bl_idname)", get_class_name());
        }

        builder.write_line("register_operator({},{},show_{})", get_script_index(), get_class_name(), get_class_name());
    }

    operator_builder& operator_builder::set_disable_drawing(bool disable)
    {
        m_disable_drawing = disable;
        return *this;
    }


    operator_builder::~operator_builder()
    {
        if (m_registers)
        {
            python_builder builder;
            write(builder);
            exec({ builder.get_code() });
        }
    }
}
