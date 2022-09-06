#include <bxx/blender_types/armature.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    bone armature::add_bone(std::string const& name, mathutils::vec3 const& head, mathutils::vec3 const& tail, float roll)
    {
        bone b = getattr("edit_bones").call("new", name);
        b.set_head(head);
        b.set_tail(tail);
        b.set_roll(roll);
        return b;
    }

    std::string bone::get_name()
    {
        return get_raw_struct()->name;
    }

    mathutils::vec3 bone::get_head()
    {
        return *((mathutils::vec3*)&get_raw_struct()->head);
    }

    mathutils::vec3 bone::get_tail()
    {
        return *((mathutils::vec3*)&get_raw_struct()->tail);
    }

    void bone::set_head(mathutils::vec3 const& head)
    {
        *get_raw_struct()->head = *((float*)&head);
    }

    void bone::set_tail(mathutils::vec3 const& tail)
    {
        *get_raw_struct()->tail = *((float*)&tail);
    }

    void bone::set_roll(float roll)
    {
        get_raw_struct()->roll = roll;
    }

    float bone::get_roll()
    {
        return get_raw_struct()->roll;
    }
}
