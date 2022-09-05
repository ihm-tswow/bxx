#include <bxx/blender_types/collection.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    void bxx::collection::link_object(bxx::object const& obj)
    {
        getattr("objects").call("link", obj);
    }
}
