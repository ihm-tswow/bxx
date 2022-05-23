#include "mathutils.hpp"

mathutils::vec3::vec3(float _x, float _y, float _z)
    : x(_x),y(_y),z(_z)
{}

mathutils::quaternion::quaternion(float _w, float _x, float _y, float _z)
    : w(_w), x(_x), y(_y), z(_z)
{}

bool mathutils::operator==(const mathutils::vec3& lhs, const mathutils::vec3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool mathutils::operator==(const mathutils::quaternion& lhs, const mathutils::quaternion& rhs)
{
    return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool mathutils::operator!=(const mathutils::vec3& lhs, const mathutils::vec3& rhs)
{
    return !(lhs == rhs);
}
bool mathutils::operator!=(const mathutils::quaternion& lhs, const mathutils::quaternion& rhs)
{
    return !(lhs == rhs);
}

std::ostream& mathutils::operator<<(std::ostream& os, const mathutils::vec3& dt)
{
    os << "{" << dt.x << "," << dt.y << "," << dt.z << "}";
    return os;
}

std::ostream& mathutils::operator<<(std::ostream& os, const mathutils::quaternion& dt)
{
    os << "{" << dt.w << "," << dt.x << "," << dt.y << "," << dt.z << "}";
    return os;
}


bxx::quaternion::quaternion(bl_quat* raw)
    : m_raw(raw)
{}

void bxx::quaternion::set(float w, float x, float y, float z)
{
    set_w(w);
    set_x(x);
    set_y(y);
    set_z(z);
}

float bxx::quaternion::get_w()
{
    return (*m_raw)[0];
}

float bxx::quaternion::get_x()
{
    return (*m_raw)[1];
}

float bxx::quaternion::get_y()
{
    return (*m_raw)[2];
}

float bxx::quaternion::get_z()
{
    return (*m_raw)[3];
}

void bxx::quaternion::set_w(float w)
{
    (*m_raw)[0] = w;
}

void bxx::quaternion::set_x(float x)
{
    (*m_raw)[1] = x;
}

void bxx::quaternion::set_y(float y)
{
    (*m_raw)[2] = y;
}

void bxx::quaternion::set_z(float z)
{
    (*m_raw)[3] = z;
}

bxx::vec3::vec3(bl_vec* raw)
    : m_raw(raw)
{}

void bxx::vec3::set(float x, float y, float z)
{
    set_x(x);
    set_y(y);
    set_z(z);
}

void bxx::vec3::set(mathutils::vec3 const& vec)
{
    set(vec.x, vec.y, vec.z);
}

void bxx::vec3::set_x(float x)
{
    *(m_raw)[0] = x;
}

void bxx::vec3::set_y(float y)
{
    *(m_raw)[1] = y;
}

void bxx::vec3::set_z(float z)
{
    *(m_raw)[2] = z;
}

float bxx::vec3::get_x()
{
    return *(m_raw)[0];
}

float bxx::vec3::get_y()
{
    return *(m_raw)[1];
}

float bxx::vec3::get_z()
{
    return *(m_raw)[2];
}

mathutils::vec3 bxx::vec3::get()
{
    return { get_x(), get_y(), get_z() };
}
