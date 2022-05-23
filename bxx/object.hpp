#pragma once

#include "id.hpp"
#include "mathutils.hpp"

typedef struct Object bl_object;

namespace bxx
{
    class mesh;

    class object : public id
    {
    public:
        object(bl_object* obj);
        bl_object* get_raw();
        std::string get_name() override;
        std::string get_full_name() override;
        void set_location(float x, float y, float z);
        void set_rotation_euler(float x, float y, float z);
        void set_rotation_euler(std::string const& fmt, float x, float y, float z);
        void set_rotation_quaternion(float w, float x, float y, float z);
        void set_scale(float x, float y, float z);

        vec3 get_location();
        vec3 get_rotation_euler();
        quaternion get_rotation_quaternion();
        vec3 get_scale();

        static bxx::object create(std::string const& name, bxx::mesh mesh);
    private:
        bl_object* m_raw;
    };
}
