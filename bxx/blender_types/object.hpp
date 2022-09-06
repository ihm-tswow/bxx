#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/vertex_group.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/blender_types/constraint.hpp>
#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_object_types.h>
#include <makesdna/DNA_mesh_types.h>
#include <makesdna/DNA_armature_types.h>
#pragma warning(pop)

namespace bxx
{
    class mesh;
    class armature;
    class vertex_group;
    class vertex_groups;
    class child_objects;

    class object : public id<bl_object>
    {
    public:
        PYFIELD(bool, selected)
        using id<bl_object>::id;
        vec3 location();
        vec3 rotation_euler();
        quaternion rotation_quaternion();
        vec3 scale();
        mesh get_mesh();
        armature get_armature();
        constraints constraints();
        vertex_groups vertex_groups();

        static object create(std::string const& name, bxx::mesh mesh);
        static object create(std::string const& name, bxx::armature armature);
    };
}

#include <bxx/blender_types/object.ipp>
