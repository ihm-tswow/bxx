#include "mathutils.hpp"

bxx::vec3::vec3(float _x, float _y, float _z)
    : x(_x),y(_y),z(_z)
{}

bxx::quaternion::quaternion(float _w, float _x, float _y, float _z)
    : w(_w), x(_x), y(_y), z(_z)
{}

bool bxx::operator==(const bxx::vec3& lhs, const bxx::vec3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool bxx::operator==(const bxx::quaternion& lhs, const bxx::quaternion& rhs)
{
    return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool bxx::operator!=(const bxx::vec3& lhs, const bxx::vec3& rhs)
{
    return !(lhs == rhs);
}
bool bxx::operator!=(const bxx::quaternion& lhs, const bxx::quaternion& rhs)
{
    return !(lhs == rhs);
}

std::ostream& bxx::operator<<(std::ostream& os, const bxx::vec3& dt)
{
    os << "{" << dt.x << "," << dt.y << "," << dt.z << "}";
    return os;
}

std::ostream& bxx::operator<<(std::ostream& os, const bxx::quaternion& dt)
{
    os << "{" << dt.w << "," << dt.x << "," << dt.y << "," << dt.z << "}";
    return os;
}

