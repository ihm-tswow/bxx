#include <bxx/mathutils.hpp>
#include <bxx/objects/python_object.hpp>

namespace mathutils
{
    rgba::rgba(bxx::python_object_weak obj)
        : r(obj.get_item<float>(0)), g(obj.get_item<float>(1)), b(obj.get_item<float>(2)), a(obj.get_item<float>(3))
    {
    }

    vec3::vec3(bxx::python_object_weak obj)
        : x(obj.get_item<float>(0)), y(obj.get_item<float>(1)), z(obj.get_item<float>(2))
    {
    }

    vec2::vec2(bxx::python_object_weak obj)
        : x(obj.get_item<float>(0)), y(obj.get_item<float>(1))
    {
    }

    quaternion::quaternion(bxx::python_object_weak obj)
        : w(obj.get_item<float>(0)), x(obj.get_item<float>(1)), y(obj.get_item<float>(2)), z(obj.get_item<float>(2))
    {
    }

    mathutils::vec3::vec3(float _x, float _y, float _z)
        : x(_x), y(_y), z(_z)
    {}

    mathutils::vec2::vec2(float _x, float _y)
        : x(_x), y(_y)
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

    bool operator== (rgba const& a, rgba const& b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }

    bool operator!= (rgba const& a, rgba const& b)
    {
        return !(a == b);
    }

    std::ostream& operator<<(std::ostream& os, rgba const& rgba)
    {
        os << "{" << rgba.r << "," << rgba.g << "," << rgba.b << "," << rgba.a << "}";
        return os;
    }
}

namespace bxx
{
    quaternion::quaternion(bl_quat* raw)
        : m_raw(raw)
    {}

    void quaternion::set(float w, float x, float y, float z)
    {
        set_w(w);
        set_x(x);
        set_y(y);
        set_z(z);
    }

    float quaternion::get_w()
    {
        return (*m_raw)[0];
    }

    float quaternion::get_x()
    {
        return (*m_raw)[1];
    }

    float quaternion::get_y()
    {
        return (*m_raw)[2];
    }

    float quaternion::get_z()
    {
        return (*m_raw)[3];
    }

    void quaternion::set_w(float w)
    {
        (*m_raw)[0] = w;
    }

    void quaternion::set_x(float x)
    {
        (*m_raw)[1] = x;
    }

    void quaternion::set_y(float y)
    {
        (*m_raw)[2] = y;
    }

    void quaternion::set_z(float z)
    {
        (*m_raw)[3] = z;
    }

    vec3::vec3(bl_vec* raw)
        : m_raw(raw)
    {}

    void vec3::set(float x, float y, float z)
    {
        set_x(x);
        set_y(y);
        set_z(z);
    }

    void vec3::set(mathutils::vec3 const& vec)
    {
        set(vec.x, vec.y, vec.z);
    }

    void vec3::set_x(float x)
    {
        *(m_raw)[0] = x;
    }

    void vec3::set_y(float y)
    {
        *(m_raw)[1] = y;
    }

    void vec3::set_z(float z)
    {
        *(m_raw)[2] = z;
    }

    float vec3::get_x()
    {
        return *(m_raw)[0];
    }

    float vec3::get_y()
    {
        return *(m_raw)[1];
    }

    float vec3::get_z()
    {
        return *(m_raw)[2];
    }

    mathutils::vec3 vec3::get()
    {
        return { get_x(), get_y(), get_z() };
    }
}
