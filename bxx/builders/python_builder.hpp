#pragma once

#include <fmt/core.h>

#include <stdint.h>
#include <string>
#include <vector>

constexpr std::uint32_t PYTHON_INDENTION = 4;

namespace bxx
{
    struct bracket_type
    {
        char const* m_open;
        char const* m_close;
    };

    class python_builder;

    class scoped_block
    {
    public:
        ~scoped_block();
        scoped_block(python_builder* builder, std::uint32_t newlines_at_end = 0);
    private:
        std::uint32_t m_newlines_at_end;
        python_builder* m_builder;
    };

    class scoped_line
    {
    public:
        ~scoped_line();
        scoped_line(python_builder* builder);
    private:
        python_builder* m_builder;
    };

    class python_builder
    {
    public:
        static constexpr bracket_type round_brackets = bracket_type( "(", ")" );
        static constexpr bracket_type square_brackets = bracket_type( "[", "]" );
        static constexpr bracket_type squiggly_brackets = bracket_type( "{", "}" );
        static constexpr bracket_type no_brackets = bracket_type( "", "" );
        static constexpr bracket_type scope = bracket_type(":", "");

        scoped_block scoped_block(bracket_type const& brackets = no_brackets, std::uint32_t newline_at_end = 0);

        template <typename... Args>
        scoped_line scoped_line(fmt::format_string<Args...> s = "", Args&&... args)
        {
            begin_line(s, std::forward<Args>(args)...);
            return bxx::scoped_line(this);
        }

        void begin_block(bracket_type const& brackets = no_brackets);
        void end_block(std::uint32_t newlines = 0);

        template <typename ...Args>
        void write(fmt::format_string<Args...> s = "", Args&&... args)
        {
            m_code += fmt::format(s, std::forward<Args>(args)...);
        }

        template <typename ...Args>
        void write_line(fmt::format_string<Args...> s, Args&&... args)
        {
            begin_line(s, std::forward<Args>(args)...);
            end_line();
        }

        template <typename ...Args>
        void begin_line(fmt::format_string<Args...> s = "", Args&&... args)
        {
            m_code += std::string(indention_level(), ' ');
            write(s, std::forward<Args>(args)...);
        }
        void end_line();

        void exec();
        std::string get_code();

        std::uint32_t indention_level(std::int32_t offset = 0);
    private:
        std::vector<char const*> m_indention_brackets;
        std::string m_code;
    };
}