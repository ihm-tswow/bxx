#pragma once

#include <bxx/blender_types/blender_types.hpp>
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

    class object : public id<bl_object>
    {
    public:
        PYFIELD(bool, selected)
        using id<bl_object>::id;
        std::string get_type_path() const final;
        vec3 location();
        vec3 rotation_euler();
        quaternion rotation_quaternion();
        vec3 scale();
        mesh get_mesh();
        armature get_armature();
        vertex_group add_vertex_group(std::string const& name);
        vertex_group get_vertex_group(std::string const& name);

        template <typename T>
        T add_constraint(std::string const& name);

        template <typename T>
        T get_constraint(std::uint32_t index);

        size_t constraints_len();

        static object create(std::string const& name, bxx::mesh mesh);
        static object create(std::string const& name, bxx::armature armature);
    };
}

#include <bxx/blender_types/object.ipp>
