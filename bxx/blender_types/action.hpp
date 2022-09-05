#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/objects/id.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_action_types.h>
#include <makesdna/DNA_anim_types.h>
#pragma warning(pop)

namespace bxx
{
    class fcurve;

    class action : public id<bl_action>
    {
    public:
        using id<bl_action>::id;
        fcurve find_fcurve(std::string const& name, int index = 0);
        fcurve add_fcurve(std::string const& group, std::string const& name, int index = 0);
        static action create(std::string const& name);
    };

    class keyframe : public blender_struct<bl_keyframe>
    {
    public:
        void set(float time, float value);
        void set_time(float time);
        void set_value(float value);
        float get_time();
        float get_value();
    };

    class fcurve : public blender_py_struct<bl_fcurve>
    {
    public:
        using blender_py_struct<bl_fcurve>::blender_py_struct;
        void add_points(int amount);
        int num_points() const;
        keyframe get_point(int index);
        std::string get_data_path() const;
        int get_index() const;
    };
}
