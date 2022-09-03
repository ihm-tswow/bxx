#include "collection.hpp"
#include "object.hpp"
#include "exec.hpp"

#include "fmt/core.h"

namespace bxx
{
    std::string collection::get_type_path() const
    {
        return "bpy.data.collections";
    }

    void bxx::collection::link_object(bxx::object const& obj)
    {
        exec(
            fmt::format("{}.objects.link({})", get_name_full(), obj.get_name_full())
        );
    }
}
