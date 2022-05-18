#include "object.hpp"

#include "makesdna/DNA_object_types.h"

#include "fmt/core.h"

bl_object* bxx::object::get_raw()
{
    return m_raw;
}

std::string bxx::object::get_name()
{
    return m_raw->id.name + 2;
}

std::string bxx::object::get_full_name()
{
    return fmt::format("bpy.data.objects['{}']", get_name());
}

void bxx::object::set_location(float x, float y, float z)
{
    set_float_prop("location", x, y, z);
}

void bxx::object::set_rotation_euler(float x, float y, float z)
{
    set_string_prop("rotation_mode", "XYZ");
    set_float_prop("rotation_euler", x, y, z);
}

void bxx::object::set_rotation_euler(std::string const& fmt, float x, float y, float z)
{
    set_string_prop("rotation_mode", fmt);
    set_float_prop("rotation_euler", x, y, z);
}

void bxx::object::set_rotation_quaternion(float w, float x, float y, float z)
{
    set_string_prop("rotation_mode", "QUATERNION");
    set_float_prop("rotation_euler", x, y, z);
}

void bxx::object::set_scale(float x, float y, float z)
{
    set_float_prop("scale", x, y, z);
}