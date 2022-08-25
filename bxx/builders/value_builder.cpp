#include "value_builder.hpp"
#include "string_builder.hpp"
#include "python_builder.hpp"

namespace bxx
{
    python_code::python_code(std::string const& code)
        : m_code(code)
    {}

    builder_value::builder_value(bool value)
    {
        m_value.emplace<bool>(value);
    }

    builder_value::builder_value(char const* value)
    {
        m_value.emplace<std::string>(std::string(value));
    }


    builder_value::builder_value(std::string const& value)
    {
        m_value.emplace<std::string>(value);
    }

    builder_value::builder_value(python_code const& value)
    {
        m_value.emplace<python_code>(value);
    }

    builder_value::builder_value(std::int8_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::uint8_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::int16_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::uint16_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::int32_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::uint32_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::int64_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(std::uint64_t value)
    {
        m_value.emplace<double>(double(value));
    }

    builder_value::builder_value(float value)
    {
        m_value.emplace<double>(value);
    }

    builder_value::builder_value(double value)
    {
        m_value.emplace<double>(value);
    }

    builder_value::builder_value(std::function<void(map_builder&)> builder)
    {
        builder(*m_value.emplace <std::unique_ptr<map_builder>>(std::make_unique<map_builder>()).get());
    }

    builder_value::builder_value(std::function<void(set_builder&)> builder)
    {
        builder(*m_value.emplace <std::unique_ptr<set_builder>>(std::make_unique<set_builder>()).get());
    }

    builder_value::builder_value(std::function<void(list_builder&)> builder)
    {
        builder(*m_value.emplace <std::unique_ptr<list_builder>>(std::make_unique<list_builder>()).get());
    }

    bool builder_value::operator < (builder_value const& rhs) const
    {
        if (m_value.index() != rhs.m_value.index())
        {
            return m_value.index() < rhs.m_value.index();
        }
        bool lt;
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>)
                lt = arg < std::get<bool>(rhs.m_value);
            else if constexpr (std::is_same_v<T, double>)
                lt = arg < std::get<double>(rhs.m_value);
            else if constexpr (std::is_same_v<T, std::string>)
                lt = arg < std::get<std::string>(rhs.m_value);
            else if constexpr (std::is_same_v<T, python_code>)
                lt = arg.m_code < std::get<python_code>(rhs.m_value).m_code;
            else if constexpr (std::is_same_v<T, std::unique_ptr<map_builder>>)
                lt = arg.get() < std::get<std::unique_ptr<map_builder>>(rhs.m_value).get();
            else if constexpr (std::is_same_v<T, std::unique_ptr<set_builder>>)
                lt = arg.get() < std::get<std::unique_ptr<set_builder>>(rhs.m_value).get();
            else if constexpr (std::is_same_v<T, std::unique_ptr<list_builder>>)
                lt = arg.get() < std::get<std::unique_ptr<list_builder>>(rhs.m_value).get();
            else
                static_assert(false, "non-exhaustive visitor!");
            }, m_value);
        return lt;
    }


    void builder_value::write(python_builder& builder) const
    {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>)
                builder.write("{}",arg ? "True" : "False");
            else if constexpr (std::is_same_v<T, double>)
                builder.write("{}", arg);
            else if constexpr (std::is_same_v<T, std::string>)
                builder.write("\"{}\"",string_to_python_string(arg));
            else if constexpr (std::is_same_v<T, python_code>)
                builder.write("{}",arg.m_code);
            else if constexpr (std::is_same_v<T, std::unique_ptr<map_builder>>)
                arg->write(builder);
            else if constexpr (std::is_same_v<T, std::unique_ptr<set_builder>>)
                arg->write(builder);
            else if constexpr (std::is_same_v<T, std::unique_ptr<list_builder>>)
                arg->write(builder);
            else
                static_assert(false, "non-exhaustive visitor!");
            }, m_value);
    }

    set_builder::set_builder()
        : bracketed_builder(python_builder::round_brackets)
    {}

    map_builder::map_builder()
        : bracketed_builder(python_builder::squiggly_brackets)
    {}

    list_builder::list_builder()
        : bracketed_builder(python_builder::square_brackets)
    {}

    void map_builder::write(python_builder& builder) const
    {
        auto block = builder.scoped_block(get_bracket_type());

        size_t i = 0;
        for (auto const& [key, value] : m_entries)
        {
            auto line = builder.scoped_line("{} = ", key);
            value.write(builder);
            if (i < m_entries.size() - 1)
            {
                builder.write(",");
            }
            ++i;
        }
    }

    void set_builder::write(python_builder& builder) const
    {
        auto block = builder.scoped_block(get_bracket_type());

        size_t i = 0;
        for (auto const& value : m_entries)
        {
            auto line = builder.scoped_line();
            value.write(builder);
            if (i < m_entries.size() - 1)
            {
                builder.write(",");
            }
            ++i;
        }
    }

    void list_builder::write(python_builder& builder) const
    {
        auto block = builder.scoped_block(get_bracket_type());
        size_t i = 0;
        for (auto const& value : m_entries)
        {
            auto line = builder.scoped_line();
            value.write(builder);
            if (i < m_entries.size() - 1)
            {
                builder.write(",");
            }
            ++i;
        }
    }
}
