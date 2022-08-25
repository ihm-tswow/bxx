#pragma once

#include "property_builder.hpp"
#include "class_header_builder.hpp"

#include <nlohmann/json.hpp>

#include <string>
#include <functional>
#include <variant>
#include <map>
#include <set>

#define BXX_OPERATOR(x) void __register_##x(bxx::operator_builder & x)

namespace bxx
{
    class python_builder;
    struct operator_draw_instruction
    {
        std::string m_code;
        bool m_is_special;
    };

    class operator_builder
        : public bxx::property_builder<operator_builder>
        , public bxx::class_header_builder<operator_builder>
    {
    public:
        operator_builder(std::string const& bl_id, bool registers);
        operator_builder& set_description(std::string const& description);
        operator_builder& add_option(std::string const& option);
        operator_builder& add_option(std::initializer_list<std::string> const& option);
        operator_builder& set_callback(std::function<void(nlohmann::json)> callback);

        operator_builder& begin_column();
        operator_builder& end_column();
        operator_builder& add_label(std::string const& label);

        ~operator_builder();
        operator_builder(operator_builder const& oth) = delete;
        operator_builder() = delete;
        void write(python_builder& builder);
    private:
        std::vector<operator_draw_instruction> m_draw_instructions = { { "cur = self.layout", true } };
        std::set<std::string> m_parents = { "bpy.types.Operator" };
        std::set<std::string> m_options = { "REGISTER" };
        std::string m_bl_id;
        bool m_registers;
        friend class operators;
    };

    class operators
    {
    public:
        static std::function<void(nlohmann::json)> get_callback(std::string const& id);
    };
}
