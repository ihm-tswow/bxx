#include "object.hpp"
#include "exec.hpp"
#include "mesh.hpp"
#include "armature.hpp"

#include "makesdna/DNA_object_types.h"

#include "fmt/core.h"

bxx::object bxx::object::create(std::string const& name, bxx::mesh mesh)
{
    return bxx::object(eval_ptr<bl_object>({
        fmt::format("out = bpy.data.objects.new('{}',{}).as_pointer()", name, mesh.get_full_name())
    }));
}

bxx::object bxx::object::create(std::string const& name, bxx::armature armature)
{
    return bxx::object(eval_ptr<bl_object>({
        fmt::format("out = bpy.data.objects.new('{}',{}).as_pointer()", name, armature.get_full_name())
    }));
}


bxx::object::object(bl_object* obj)
    : m_raw(obj)
{}

bl_object* bxx::object::get_raw()
{
    return m_raw;
}

std::string bxx::object::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::object::get_full_name() const
{
    return fmt::format("bpy.data.objects['{}']", get_name());
}

bxx::vec3 bxx::object::location()
{
    return bxx::vec3(&m_raw->loc);
}

bxx::vec3 bxx::object::rotation_euler()
{
    return bxx::vec3(&m_raw->rot);
}

bxx::quaternion bxx::object::rotation_quaternion()
{
    return bxx::quaternion(&m_raw->quat);
}

bxx::vec3 bxx::object::scale()
{
    return bxx::vec3(&m_raw->scale);
}

mathutils::quaternion bxx::quaternion::get()
{
    return { get_w(), get_x(), get_y(), get_z() };
}

void bxx::object::set_selected(bool selected)
{
    exec(fmt::format("{}.select_set({})",get_full_name(),selected ? "True" : "False"));
}

