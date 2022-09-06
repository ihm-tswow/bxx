#pragma once

#include <bxx/objects/id.hpp>
#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>

namespace bxx
{
    class view_layer;
    class view_layers;

    class scene : public id<bl_scene>
    {
    public:
        using id<bl_scene>::id;
        view_layers view_layers();
    private:
        bl_scene *m_raw;
    };

    class scene_data : public blender_py_iterable<scene>
    {
    public:
        using blender_py_iterable<scene>::blender_py_iterable;
        scene create(std::string const& name);
        scene find(std::string const& name);
    };
}