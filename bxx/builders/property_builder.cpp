#include <bxx/builders/property_builder.hpp>

namespace bxx
{
    property_entry::property_entry(std::string const& id, std::string const& type)
        : m_id(id)
        , m_type(type)
    {}
}
