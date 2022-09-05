#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/armature.hpp>
#include <bxx/blender_types/vertex_group.hpp>
#include <bxx/misc.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    object object::create(std::string const& name, mesh ms)
    {
        return eval_pyobject("out = bpy.data.objects").call("new", name, ms);
    }

    object object::create(std::string const& name, armature arma)
    {
        return eval_pyobject("out = bpy.data.objects").call("new", name, arma);
    }

    vec3 object::location()
    {
        return vec3(&get_raw_struct()->loc);
    }

    vec3 object::rotation_euler()
    {
        return vec3(&get_raw_struct()->rot);
    }

    quaternion object::rotation_quaternion()
    {
        return quaternion(&get_raw_struct()->quat);
    }

    vec3 object::scale()
    {
        return vec3(&get_raw_struct()->scale);
    }

    mathutils::quaternion quaternion::get()
    {
        return { get_w(), get_x(), get_y(), get_z() };
    }

    mesh object::get_mesh()
    {
        return getattr("data");
    }

    armature object::get_armature()
    {
        return getattr("data");
    }

    size_t object::constraints_len()
    {
        return PyObject_Size(getattr("constraints"));
    }

    vertex_group object::add_vertex_group(std::string const& name)
    {
        return getattr("vertex_groups").call("new", kwarg("name", name));
    }

    vertex_group object::get_vertex_group(std::string const& name)
    {
        return getattr("vertex_groups").get_item(name);
    }
}
