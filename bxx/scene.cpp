#include "scene.hpp"
#include "view_layer.hpp"
#include "exec.hpp"

#include "makesdna/DNA_scene_types.h"
#include "fmt/core.h"

std::string bxx::scene::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::scene::get_full_name() const
{
    return fmt::format("bpy.data.scenes['{}']", get_name());
}

bxx::scene::scene(bl_scene* raw)
    : m_raw(raw)
{}

void bxx::scene::set_name(std::string const& name)
{
    exec({
        fmt::format("{}.name = '{}'", get_full_name(), name)
    });
}

bxx::view_layer bxx::scene::add_view_layer(std::string const& name)
{
    return bxx::view_layer(*this, eval_ptr<bl_view_layer>({
        fmt::format("out = {}.view_layers.new(name='{}').as_pointer()", get_full_name(), name)
    }));
}

bxx::view_layer bxx::scene::get_view_layer(std::string const& name)
{
    return bxx::view_layer(*this, eval_ptr<bl_view_layer>({
        fmt::format("out = {}.view_layers['{}'].as_pointer()", get_full_name(), name)
    }));
}

bxx::scene bxx::scene::create(std::string const& name)
{
    return bxx::scene(eval_ptr<bl_scene>({
        fmt::format("out = bpy.data.scenes.new(name='{}').as_pointer()", name)
    }));
}

bxx::scene bxx::scene::get(std::string const& name)
{
    return bxx::scene(eval_ptr<bl_scene>({
        fmt::format("out = bpy.data.scenes['{}'].as_pointer()", name)
    }));
}
