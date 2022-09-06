#pragma once

#include <bxx/builders/property_builder.hpp>
#include <bxx/builders/class_header_builder.hpp>
#include <bxx/objects/python_object.hpp>

#include <nlohmann/json.hpp>

#include <string>
#include <functional>
#include <variant>
#include <map>
#include <set>

#define BXX_OPERATOR(x) void op_register_##x(bxx::operator_builder & x)

namespace bxx
{
    class python_builder;
    struct operator_draw_instruction
    {
        std::string m_code;
        bool m_is_special;
    };

    class operator_builder
        : public property_builder<operator_builder>
        , public class_header_builder<operator_builder>
    {
    public:
        operator_builder(std::string const& bl_id, bool registers = true);
        operator_builder& set_description(std::string const& description);
        operator_builder& add_option(std::string const& option);
        operator_builder& add_option(std::initializer_list<std::string> const& option);
        operator_builder& set_callback(std::function<void(python_object)> callback);
        operator_builder& set_draw(std::function<void(python_object)> callback);
        operator_builder& set_disable_drawing(bool disable);
        operator_builder& begin_column();
        operator_builder& end_column();
        operator_builder& add_label(std::string const& label);

        ~operator_builder();
        operator_builder(operator_builder const& oth) = delete;
        operator_builder() = delete;
        void write(python_builder& builder);
    private:
        std::function<void(python_object)> m_callback;
        std::function<void(python_object)> m_draw;
        std::vector<operator_draw_instruction> m_draw_instructions = { { "cur = self.layout", true } };
        std::set<std::string> m_parents = { "bpy.types.Operator" };
        std::set<std::string> m_options = { "REGISTER" };
        std::string m_bl_id;
        bool m_registers;
        bool m_disable_drawing = false;
        friend class operators;
    };

    class operators
    {
    public:
        static std::function<void(python_object)> get_callback(std::string const& id);
    };
}
