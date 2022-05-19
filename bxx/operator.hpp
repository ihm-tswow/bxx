#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <functional>
#include <variant>
#include <map>
#include <set>

#define BXX_OPERATOR(x) void __register_##x(bxx::operator_builder & x)

namespace bxx
{
    struct operator_parameter
    {
        std::string m_type;
        nlohmann::json m_properties;
    };

    struct operator_enum_entry
    {
        std::string m_id;
        std::string m_name = m_id;
        std::string m_description = "";
    };

    struct operator_draw_instruction
    {
        std::string m_code;
        bool m_is_special;
    };

    class operator_builder
    {
    public:
        operator_builder& set_name(std::string const& name);
        operator_builder& set_description(std::string const& description);

        operator_builder& add_string_parameter(std::string const& id, std::string const& name, std::string const& def, std::string const& description = "");
        operator_builder& add_int_parameter(std::string const& id, std::string const& name, int min, int def, int max, std::string const& description = "");
        operator_builder& add_float_parameter(std::string const& id, std::string const& name, float min, float def, float max, std::string const& description = "");
        operator_builder& add_bool_parameter(std::string const& id, std::string const& name, bool def, std::string const& description = "");
        operator_builder& add_enum_parameter(std::string const& id, std::string const& name, std::initializer_list<operator_enum_entry> const& values, std::string const& description = "");
        operator_builder& add_enum_parameter(std::string const& id, std::string const& name, std::string const& def, std::initializer_list<operator_enum_entry> const& values, std::string const& description = "");
        operator_builder& add_dynamic_enum_parameter(std::string const& id, std::string const& name, std::string const& code, std::string const& description = "");

        operator_builder& add_parameter(std::string const& id, std::string const& type, nlohmann::json properties);
        operator_builder& add_parent_class(std::string const& parent);
        operator_builder& add_option(std::string const& option);
        operator_builder& add_parent_class(std::initializer_list<std::string> const& parents);
        operator_builder& add_option(std::initializer_list<std::string> const& option);
        operator_builder& set_callback(std::function<void(nlohmann::json)> callback);

        operator_builder& begin_column();
        operator_builder& end_column();
        operator_builder& add_label(std::string const& label);

        std::string get_class_name();
        ~operator_builder();
        operator_builder(operator_builder const& oth) = delete;
        operator_builder() = delete;
    private:
        operator_builder(std::string const& bl_id);
        std::string m_bl_id;
        std::string m_name;
        std::string m_description = "";
        std::map<std::string, operator_parameter> m_parameters;
        std::vector<operator_draw_instruction> m_draw_instructions = { { "cur = self.layout", true } };
        std::set<std::string> m_parents = { "bpy.types.Operator" };
        std::set<std::string> m_options = { "REGISTER" };
        friend class operators;
    };

    class operators
    {
    public:
        static operator_builder create(std::string const& id);
        static std::function<void(nlohmann::json)> get_callback(std::string const& id);
    };
}
