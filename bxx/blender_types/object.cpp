#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/armature.hpp>
#include <bxx/misc.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    std::string object::get_type_path() const
    {
        return "bpy.data.objects";
    }

    object object::create(std::string const& name, mesh ms)
    {
        return eval_ptr<bl_object>(
            "out = bpy.data.objects.new('{}',{}).as_pointer()", name, ms.get_name_full()
        );
    }

    object object::create(std::string const& name, armature arma)
    {
        return eval_ptr<bl_object>(
            "out = bpy.data.objects.new('{}',{}).as_pointer()", name, arma.get_name_full()
        );
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
        return mesh(eval_ptr<bl_mesh>(
            "out = {}.data.as_pointer()", get_name_full()
        ));
    }

    armature object::get_armature()
    {
        return armature(eval_ptr<bl_armature>(
            "out = {}.data.as_pointer()", get_name_full()
        ));
    }
}
