#pragma once

#include <ostream>

// mathutils is for our own primitive math types
namespace mathutils
{
    template <int max>
    struct rgba
    {
        rgba(float _r, float _g, float _b, float _a = max)
            : r(_r), g(_g), b(_b), a(_a)
        {}
        float r;
        float g;
        float b;
        float a = max;
    };

    struct vec3
    {
        vec3(float x, float y, float z);
        float x;
        float y;
        float z;
    };

    struct quaternion
    {
        quaternion(float w, float x, float y, float z);
        float w;
        float x;
        float y;
        float z;
    };

    bool operator==(const mathutils::vec3& lhs, const mathutils::vec3& rhs);
    bool operator==(const mathutils::quaternion& lhs, const mathutils::quaternion& rhs);
    bool operator!=(const mathutils::vec3& lhs, const mathutils::vec3& rhs);
    bool operator!=(const mathutils::quaternion& lhs, const mathutils::quaternion& rhs);
    std::ostream& operator<<(std::ostream& os, const mathutils::vec3& dt);
    std::ostream& operator<<(std::ostream& os, const mathutils::quaternion& dt);

}

// bxx is for blender type references / functions
namespace bxx
{
    typedef float bl_quat[4];
    typedef float bl_vec[3];

    struct quaternion
    {
    public:
        quaternion(bl_quat* raw);
        mathutils::quaternion get();
        void set(float w, float x, float y, float z);
        void set_w(float w);
        void set_x(float x);
        void set_y(float y);
        void set_z(float z);
        float get_w();
        float get_x();
        float get_y();
        float get_z();
    private:
        bl_quat* m_raw;
    };

    struct vec3
    {
    public:
        vec3(bl_vec* raw);
        void set(float x, float y, float z);
        void set(mathutils::vec3 const& vec);
        void set_x(float x);
        void set_y(float y);
        void set_z(float z);
        float get_x();
        float get_y();
        float get_z();
        mathutils::vec3 get();
    private:
        bl_vec* m_raw;
    };
}