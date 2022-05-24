#pragma once

#include "id.hpp"
#include "mathutils.hpp"

typedef struct bArmature bl_armature;
typedef struct EditBone bl_bone;

namespace bxx
{
    class bone;
    class armature : public id
    {
    public:
        static bxx::armature create(std::string const& name);
        armature(bl_armature * raw);
        std::string get_name() const override;
        std::string get_full_name() const override;
        bxx::bone add_bone(
            std::string const& name,
            mathutils::vec3 const& head = { 0,0,0 },
            mathutils::vec3 const& tail = { 1,0,0 },
            float roll = 0
        );
        bl_armature* get_raw();
    private:
        bl_armature* m_raw;
    };

    class bone : public id
    {
    public:
        bone(bxx::armature const& armature, bl_bone* bone);
        std::string get_name() const override;
        std::string get_full_name() const override;
        void set_head(mathutils::vec3 const& head);
        void set_tail(mathutils::vec3 const& tail);
        // radians
        void set_roll(float roll);
        mathutils::vec3 get_head();
        mathutils::vec3 get_tail();
        // radians
        float get_roll();
        bl_bone* get_raw();
    private:
        bxx::armature m_armature;
        bl_bone* m_bone;
    };
}