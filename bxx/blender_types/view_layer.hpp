#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/scene.hpp>
#include <bxx/objects/id.hpp>

namespace bxx
{
    class object;
    class scene;
    class collection;

    class view_layer : public blender_py_struct<bl_view_layer>
    {
    public:
        using blender_py_struct<bl_view_layer>::blender_py_struct;
        std::string get_name();
        void link_object(object const& obj);
        void update();
    };
}