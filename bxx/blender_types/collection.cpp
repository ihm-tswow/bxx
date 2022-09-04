#include <bxx/blender_types/collection.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    std::string collection::get_type_path() const
    {
        return "bpy.data.collections";
    }

    void bxx::collection::link_object(bxx::object const& obj)
    {
        exec("{}.objects.link({})", get_name_full(), obj.get_name_full());
    }
}
