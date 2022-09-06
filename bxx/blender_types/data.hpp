#pragma once

#include <bxx/objects/python_object.hpp>
#include <bxx/blender_types/iterables.hpp>

namespace bxx
{
    class action_data;
    class object_data;
    class armature_data;
    class mesh_data;
    class material_data;
    class scene_data;
    class image_data;
    class collection_data;

    class data : public python_object
    {
    public:
        using python_object::python_object;
        action_data actions();
        object_data objects();
        armature_data armatures();
        mesh_data meshes();
        material_data materials();
        scene_data scenes();
        image_data images();
        collection_data collections();
    };

    data get_data();
}