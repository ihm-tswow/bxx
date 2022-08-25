#pragma once

#include "id.hpp"

namespace bxx
{
    class property_group : public id
    {
    public:
        property_group(std::string path);
        std::string get_name() const final;
        std::string get_full_name() const final;
    private:
        std::string m_path;
    };
}