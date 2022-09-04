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
    view_layer::view_layer(scene const& parent, bl_view_layer* layer)
        : blender_py_struct<bl_view_layer>(layer)
        , m_parent(parent)
    {}

    void view_layer::link_object(object const& obj)
    {
        exec("{}.active_layer_collection.collection.objects.link({})",m_parent.get_name_full(), obj.get_name_full());
    }

    python_object view_layer::get_pyobject()
    {
        // todo: fix
        return eval_pyobject("");
    }

    std::string view_layer::get_name() const
    {
        return get_raw_struct()->name;
    }

    std::string view_layer::get_full_name() const
    {
        return fmt::format("{}.view_layers['{}']", m_parent.get_name_full(), get_name());
    }

    void view_layer::update()
    {
        exec("{}.update()", get_full_name());
    }
}
