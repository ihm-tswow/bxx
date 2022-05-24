#include "view_layer.hpp"
#include "object.hpp"
#include "exec.hpp"
#include "collection.hpp"

#include "makesdna/DNA_layer_types.h"
#include "makesdna/DNA_scene_types.h"
#include "fmt/core.h"

bxx::view_layer::view_layer(bxx::scene const& scene, bl_view_layer* layer)
    : m_raw(layer)
    , m_parent(scene)
{}

void bxx::view_layer::link_object(bxx::object const& obj)
{
    exec({
        fmt::format("{}.active_layer_collection.collection.objects.link({})",get_full_name(), obj.get_full_name())
    });
}


std::string bxx::view_layer::get_name() const
{
    return m_raw->name;
}

std::string bxx::view_layer::get_full_name() const
{
    return fmt::format("{}.view_layers['{}']", m_parent.get_full_name(),get_name());
}

bl_view_layer* bxx::view_layer::get_raw()
{
    return m_raw;
}

void bxx::view_layer::update()
{
    exec(fmt::format("{}.update()", get_full_name()));
}

