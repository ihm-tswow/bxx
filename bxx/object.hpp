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

        bxx::vec3 location();
        bxx::vec3 rotation_euler();
        bxx::quaternion rotation_quaternion();
        bxx::vec3 scale();
        static bxx::object create(std::string const& name, bxx::mesh mesh);
    private:
        bl_object* m_raw;
    };
}
