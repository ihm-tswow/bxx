#include <bxx/blender_types/view_layer.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/collection.hpp>
#include <common/exec.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_layer_types.h>
#include <makesdna/DNA_scene_types.h>
#pragma warning(pop)

#include <fmt/core.h>

namespace bxx
{
    void view_layer::link_object(object const& obj)
    {
        getattr("objects").call("link", obj);
    }

    std::string view_layer::get_name()
    {
        return get_raw_struct()->name;
    }

    void view_layer::update()
    {
        call("update");
    }

    view_layer view_layers::create(std::string const& name)
    {
        return call("new", kwarg("name", name));
    }

    view_layer view_layers::find(std::string const& name)
    {
        return get_item(name);
    }
}
