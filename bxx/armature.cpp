#include "armature.hpp"
#include "exec.hpp"

#include "makesdna/DNA_armature_types.h"
#include "blenkernel/BKE_armature.h"

#include "fmt/core.h"

std::string bxx::armature::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::armature::get_full_name() const
{
    return fmt::format("bpy.data.armatures['{}']",get_name());
}

bl_bone* bxx::bone::get_raw()
{
    return m_bone;
}

std::string bxx::bone::get_name() const
{
    return m_bone->name;
}

std::string bxx::bone::get_full_name() const
{
    return fmt::format("{}.edit_bones['{}']", m_armature.get_full_name(), get_name());
}

mathutils::vec3 bxx::bone::get_head()
{
    return *((mathutils::vec3*)&m_bone->head);
}

mathutils::vec3 bxx::bone::get_tail()
{
    return *((mathutils::vec3*)&m_bone->tail);
}

bxx::bone::bone(bxx::armature const& armature, bl_bone* bone)
    : m_armature(armature)
    , m_bone(bone)
{}

bxx::armature bxx::armature::create(std::string const& name)
{
    return bxx::armature(eval_ptr<bl_armature>({
        fmt::format("out = bpy.data.armatures.new('{}').as_pointer()", name)
    }));
}

bxx::bone bxx::armature::add_bone(std::string const& name, mathutils::vec3 const& head, mathutils::vec3 const& tail, float roll)
{
    bxx::bone bone = bxx::bone(*this, eval_ptr<bl_bone>({
        fmt::format("out = {}.edit_bones.new('{}').as_pointer()", get_full_name(), name)
    }));
    bone.set_head(head);
    bone.set_tail(tail);
    bone.set_roll(roll);
    return bone;
}

void bxx::bone::set_head(mathutils::vec3 const& head)
{
    *m_bone->head = *((float*) &head);
}

void bxx::bone::set_tail(mathutils::vec3 const& tail)
{
    *m_bone->tail = *((float*) &tail);
}

void bxx::bone::set_roll(float roll)
{
    m_bone->roll = roll;
}

float bxx::bone::get_roll()
{
    return m_bone->roll;
}

bxx::armature::armature(bl_armature * raw)
    : m_raw(raw)
{}

bl_armature* bxx::armature::get_raw()
{
    return m_raw;
}
