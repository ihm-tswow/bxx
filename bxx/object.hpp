#pragma once

#include "id.hpp"
#include "mathutils.hpp"

typedef struct Object bl_object;

namespace bxx
{
    class mesh;
    class armature;

    class object : public id
    {
    public:
        object(bl_object* obj);
        bl_object* get_raw();
        std::string get_name() const override;
        std::string get_full_name() const override;
        bxx::vec3 location();
        bxx::vec3 rotation_euler();
        bxx::quaternion rotation_quaternion();
        bxx::vec3 scale();
        bxx::mesh mesh();
        bxx::armature armature();
        void set_selected(bool selected);
        static bxx::object create(std::string const& name, bxx::mesh mesh);
        static bxx::object create(std::string const& name, bxx::armature armature);
    private:
        bl_object* m_raw;
    };
}
