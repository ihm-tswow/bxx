#pragma once

#include "python_builder.hpp"

#include <stdint.h>
#include <variant>
#include <functional>
#include <string>
#include <map>
#include <set>

namespace bxx
{
    class map_builder;
    class set_builder;
    class list_builder;
    class builder_value;
    class python_builder;

    struct python_code
    {
        std::string m_code;
        python_code(std::string const& code);
    };

    using builder_value_type = std::variant
        <
        bool,
        double,
        std::string,
        python_code,
        // bleh
        std::unique_ptr<map_builder>,
        std::unique_ptr<set_builder>,
        std::unique_ptr<list_builder>
        >;

    class builder_value
    {
    public:
        // todo: there has to be a better way to do these overloads
        builder_value(bool value);
        builder_value(char const* value);
        builder_value(std::string const& value);
        builder_value(python_code const& value);
        builder_value(std::int8_t value);
        builder_value(std::uint8_t value);
        builder_value(std::int16_t value);
        builder_value(std::uint16_t value);
        builder_value(std::int32_t value);
        builder_value(std::uint32_t value);
        builder_value(std::int64_t value);
        builder_value(std::uint64_t value);
        builder_value(float value);
        builder_value(double value);
        builder_value(std::function<void(map_builder&)> builder);
        builder_value(std::function<void(set_builder&)> builder);
        builder_value(std::function<void(list_builder&)> builder);
        builder_value() = default;

        size_t index()
        {
            return m_value.index();
        }

        template <typename T>
        bool is()
        {
            return std::holds_alternative<T>(m_value);
        }

        template <typename T>
        T get()
        {
            return std::get<T>(m_value);
        }

        template <typename T>
        T const& get() const
        {
            return std::get<T>(m_value);
        }

        template <typename T>
        T& emplace(T const& value)
        {
            return m_value.emplace(value);
        }

        bool operator < (builder_value const& rhs) const;
        void write(python_builder & builder) const;
    private:
        builder_value_type m_value;
    };

    template <typename T>
    class bracketed_builder
    {
    public:
        bracketed_builder(bracket_type type)
            : m_bracket_type(type)
        {}
        virtual ~bracketed_builder() {}
        T& set_bracket_type(bracket_type type) {
            m_bracket_type = type;
            return *dynamic_cast<T*>(this);
        }

        bracket_type get_bracket_type() const {
            return m_bracket_type;
        }
    private:
        bracket_type m_bracket_type;
    };

    class set_builder : public bracketed_builder<set_builder>
    {
    public:
        set_builder();
        template <typename T>
        set_builder& insert(T const& value)
        {
            m_entries.emplace(value);
            return *this;
        }

        void write(python_builder & builder) const;
    private:
        std::set<builder_value> m_entries;
    };

    class list_builder : public bracketed_builder<list_builder>
    {
    public:
        list_builder();
        template <typename T>
        list_builder& add(T const& value)
        {
            m_entries.emplace_back(value);
            return *this;
        }

        template <typename T>
        map_builder& mod(size_t index, std::function<void(T&)> callback)
        {
            callback(m_entries[index].get<std::unique_ptr<T>>().get());
        }

        void write(python_builder& builder) const;
    private:
        std::vector<builder_value> m_entries;
    };

    class map_builder : public bracketed_builder<map_builder>
    {
    public:
        map_builder();
        template <typename T>
        map_builder& set(std::string const& key, T value)
        {
            m_entries.emplace(key, value);
            return *this;
        }

        template <typename T>
        map_builder& mod(std::string const& key, std::function<void(T&)> callback)
        {
            auto itr = m_entries.find(key);
            if (itr == m_entries.end())
            {
                return set(key, callback);
            }
            else
            {
                callback(*itr->second.get<std::unique_ptr<T>>().get());
            }
        }

        void write(python_builder & builder) const;
    private:
        std::map<std::string, builder_value> m_entries;
    };
}