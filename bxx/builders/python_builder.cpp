#include "python_builder.hpp"
#include "exec.hpp"

#include <cassert>

namespace bxx
{
    scoped_block::~scoped_block()
    {
        m_builder->end_block(m_newlines_at_end);
    }

    scoped_block::scoped_block(python_builder* builder, std::uint32_t newlines_at_end)
        : m_builder(builder)
        , m_newlines_at_end(newlines_at_end)
    {}

    scoped_block python_builder::scoped_block(bracket_type const& brackets, std::uint32_t newlines_at_end)
    {
        begin_block(brackets);
        return bxx::scoped_block(this, newlines_at_end);
    }

    void python_builder::begin_block(bracket_type const& brackets)
    {
        m_code += brackets.m_open;
        m_code += '\n';
        m_indention_brackets.push_back(brackets.m_close);
    }

    scoped_line::~scoped_line()
    {
        m_builder->end_line();
    }

    scoped_line::scoped_line(python_builder* builder)
        : m_builder(builder)
    {}

    void python_builder::end_line()
    {
        m_code += '\n';
    }

    std::uint32_t python_builder::indention_level(std::int32_t offset)
    {
        if (-offset > m_indention_brackets.size())
        {
            return 0;
        }
        return (std::int32_t(m_indention_brackets.size()) + offset) * PYTHON_INDENTION;
    }

    void python_builder::end_block(std::uint32_t newlines)
    {
        assert(m_code.size() > 0);
        assert(m_indention_brackets.size() > 0);
        bool non_empty_close = strlen(m_indention_brackets[m_indention_brackets.size() - 1]) > 0;
        if (m_code[m_code.size()-1] == '\n' && non_empty_close)
        {
            m_code += std::string(indention_level(-1), ' ');
        }
        m_code += m_indention_brackets[m_indention_brackets.size() - 1];
        m_indention_brackets.pop_back();
        m_code += std::string(newlines, '\n');
    }

    void python_builder::exec()
    {
        ::exec({ m_code });
    }

    std::string python_builder::get_code()
    {
        return m_code;
    }
}
