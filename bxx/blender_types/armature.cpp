#include "armature.hpp"
#include "exec.hpp"

#include "fmt/core.h"

namespace bxx
{
    std::string armature::get_type_path() const
    {
        return "bpy.data.armatures";
    }

    bone armature::add_bone(std::string const& name, mathutils::vec3 const& head, mathutils::vec3 const& tail, float roll)
    {
        bone b = bone(*this, eval_ptr<bl_bone>("out = {}.edit_bones.new('{}').as_pointer()", get_name_full(), name));
        b.set_head(head);
        b.set_tail(tail);
        b.set_roll(roll);
        return b;
    }

    std::string bone::get_name()
    {
        return get_raw_struct()->name;
    }

    python_object bone::get_pyobject()
    {
        return eval_pyobject("{}.edit_bones['{}']", m_armature.get_name_full(), get_name());
    }

    mathutils::vec3 bone::get_head()
    {
        return *((mathutils::vec3*)&m_bone->head);
    }

    mathutils::vec3 bone::get_tail()
    {
        return *((mathutils::vec3*)&m_bone->tail);
    }

    bone::bone(armature const& armature, bl_bone* bone)
        : blender_py_struct<bl_bone>(bone)
        , m_armature(armature)
    {}

    void bone::set_head(mathutils::vec3 const& head)
    {
        *m_bone->head = *((float*)&head);
    }

    void bone::set_tail(mathutils::vec3 const& tail)
    {
        *m_bone->tail = *((float*)&tail);
    }

    void bone::set_roll(float roll)
    {
        m_bone->roll = roll;
    }

    float bone::get_roll()
    {
        return m_bone->roll;
    }

    armature armature::create(std::string const& name)
    {
        return armature(eval_ptr<bl_armature>("out = bpy.data.armatures.new('{}').as_pointer()", name));
    }
}
