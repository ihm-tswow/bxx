#include "property_group.hpp"

namespace bxx
{
    property_group::property_group(std::string path)
        : m_path(path)
    {}

    std::string property_group::get_name() const
    {
        // todo: what to even return here? property_groups don't necessarily have names
        return m_path;
    }

    std::string property_group::get_full_name() const
    {
        return m_path;
    }
}