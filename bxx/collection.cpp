#include "collection.hpp"
#include "object.hpp"
#include "exec.hpp"

#include "makesdna/DNA_collection_types.h"
#include "fmt/core.h"

bxx::collection::collection(bl_collection* raw)
    : m_raw(raw)
{}

std::string bxx::collection::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::collection::get_full_name() const
{
    return fmt::format("bpy.data.collections['{}']",get_name());
}

void bxx::collection::link_object(bxx::object const& obj)
{
    exec({
        fmt::format("{}.objects.link({})", get_full_name(), obj.get_full_name())
    });
}

bl_collection* bxx::collection::get_raw()
{
    return m_raw;
}
