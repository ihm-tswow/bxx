#pragma once

#include "python_builder.hpp"
#include "value_builder.hpp"

#include <string>
#include <vector>
#include <numeric>

namespace bxx
{
    template <typename T>
    class class_header_builder
    {
    public:
        class_header_builder(std::string const& name)
            : m_class_name(name)
        {
        }

        virtual ~class_header_builder(){}

        template <typename V>
        T& set_class_variable(std::string const& name, V const& value)
        {
            m_variables.emplace(name, value);
            return *dynamic_cast<T*>(this);
        }

        template <typename T>
        map_builder& mod_class_variable(std::string const& key, std::function<void(T&)> callback)
        {
            auto itr = m_variables.find(key);
            if (itr == m_variables.end())
            {
                return set(key, callback);
            }
            else
            {
                callback(*itr->second.get<std::unique_ptr<T>>().get());
            }
        }

        std::string get_class_name()
        {
            return m_class_name;
        }

        T& add_parent_class(std::string const& name)
        {
            m_parents.push_back(name);
            return *dynamic_cast<T*>(this);
        }

        T& set_class_name(std::string const& name)
        {
            m_class_name = name;
            return *dynamic_cast<T*>(this);
        }

        void write(python_builder& builder, std::function<void()> in_class_writer)
        {
            if (m_parents.size() > 0)
            {
                builder.begin_line("class {}({}):",m_class_name,
                    std::accumulate(m_parents.begin(),m_parents.end(),std::string(""),
                        [](std::string const& acc, std::string const& cur){
                            return acc.empty() ? cur : acc + "," + cur;
                        }));
            }
            else
            {
                builder.begin_line("class {}:",m_class_name);
            }
            auto block = builder.scoped_block();
            for (auto& [key, value] : m_variables)
            {
                auto line = builder.scoped_line("{} = ",key);
                value.write(builder);
            }
            in_class_writer();
            builder.end_line();
        }

    private:
        std::string m_class_name;
        std::vector<std::string> m_parents;
        std::map<std::string, builder_value> m_variables;
    };
}