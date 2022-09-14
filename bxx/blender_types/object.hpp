#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/vertex_group.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/blender_types/constraint.hpp>
#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>

namespace bxx
{
    class mesh;
    class armature;
    class vertex_group;
    class vertex_groups;
    class object_children;

    enum class object_type
    {
        MESH,
        CURVE,
        SURFACE,
        META,
        FONT,
        ARMATURE,
        LATTICE,
        EMPTY,
        GPENCIL,
        CAMERA,
        LIGHT,
        SPEAKER,
        LIGHT_PROBE
    };

    class object : public id<bl_object>
    {
    public:
        PYFIELD(bool, selected)
        PYFIELD(object, parent)
        PYFIELD_STRINGENUM_DECL(object_type,type)

        using id<bl_object>::id;
        vec3 location();
        vec3 rotation_euler();
        quaternion rotation_quaternion();
        vec3 scale();
        mesh get_mesh();
        armature get_armature();
        constraints constraints();
        vertex_groups vertex_groups();
        object_children children();
    };

    class object_children : public blender_py_iterable<object>
    {
    public:
        using blender_py_iterable<object>::blender_py_iterable;
    };

    class object_data : public blender_py_iterable<object>
    {
    public:
        using blender_py_iterable<object>::blender_py_iterable;
        object create(std::string const& name, armature const& arma);
        object create(std::string const& name, mesh const& arma);
        object find(std::string const& name);
    };
}

#include <bxx/blender_types/object.ipp>
