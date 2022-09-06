#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/objects/id.hpp>

namespace bxx
{
    namespace details
    {
        size_t keyframe_len(bl_fcurve* curve);
        bl_keyframe* get_keyframe(bl_fcurve* curve, size_t index);
    }

    class keyframe : public blender_struct<bl_keyframe>
    {
    public:
        using blender_struct<bl_keyframe>::blender_struct;
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

        blender_ptr_iterable<bl_fcurve, bl_keyframe, keyframe, details::keyframe_len, details::get_keyframe>
            points();

        void add_points(int amount);
        std::string get_data_path() const;
        int get_index() const;
    };

    class fcurves : public blender_py_iterable<fcurve>
    {
    public:
        using blender_py_iterable<fcurve>::blender_py_iterable;
        fcurve find(std::string const& name, int index = 0);
        fcurve add(std::string const& group, std::string const& name, int index = 0);
    };

    class action : public id<bl_action>
    {
    public:
        using id<bl_action>::id;
        fcurves fcurves();
    };

    class action_data : public blender_py_iterable<action>
    {
    public:
        using blender_py_iterable<action>::blender_py_iterable;
        action create(std::string const& name);
        action find(std::string const& name);
    };
}
