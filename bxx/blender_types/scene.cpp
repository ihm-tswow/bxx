#include <bxx/blender_types/scene.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    std::string scene::get_type_path() const
    {
        return "bpy.data.scenes";
    }

    view_layer scene::add_view_layer(std::string const& name)
    {
        return view_layer(*this, eval_ptr<bl_view_layer>(
            "out = {}.view_layers.new(name='{}').as_pointer()", get_name_full(), name
        ));
    }

    view_layer scene::get_view_layer(std::string const& name)
    {
        return view_layer(*this, eval_ptr<bl_view_layer>(
            "out = {}.view_layers['{}'].as_pointer()", get_name_full(), name
        ));
    }

    scene scene::create(std::string const& name)
    {
        return scene(eval_ptr<bl_scene>(
            "out = bpy.data.scenes.new(name='{}').as_pointer()", name
        ));
    }

    scene scene::get(std::string const& name)
    {
        return scene(eval_ptr<bl_scene>(
            "out = bpy.data.scenes['{}'].as_pointer()", name
        ));
    }
}
