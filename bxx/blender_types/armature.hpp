#pragma once

#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>
#include <bxx/blender_types/blender_types.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_armature_types.h>
#include <blenkernel/BKE_armature.h>
#pragma warning(pop)

namespace bxx
{
    class bone;
    class armature : public id<bl_armature>
    {
    public:
        using id<bl_armature>::id;
        std::string get_type_path() const override;
        bone add_bone(
            std::string const& name,
            mathutils::vec3 const& head = { 0,0,0 },
            mathutils::vec3 const& tail = { 1,0,0 },
            float roll = 0
        );
        static armature create(std::string const& name);
    };

    class bone : public blender_py_struct<bl_bone>
    {
    public:
        bone(armature const& armature, bl_bone* bone);
        void set_head(mathutils::vec3 const& head);
        void set_tail(mathutils::vec3 const& tail);
        // radians
        void set_roll(float roll);
        mathutils::vec3 get_head();
        mathutils::vec3 get_tail();
        // radians
        float get_roll();
        std::string get_name();
    protected:
        python_object get_pyobject() final;
    private:
        armature m_armature;
        bl_bone* m_bone;
    };
}