#include <bxx/blender_types/scene.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    view_layer scene::add_view_layer(std::string const& name)
    {
        return getattr("view_layers").call("new", kwarg("name", name));
    }

    view_layer scene::get_view_layer(std::string const& name)
    {
        return getattr("view_layers").get_item(name);
    }
}
