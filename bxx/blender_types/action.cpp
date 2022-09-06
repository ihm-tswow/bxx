#include <bxx/blender_types/action.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{

    fcurve fcurves::find(std::string const& name, int index)
    {
        return call("find", name, kwarg("index", index));
    }


    fcurve fcurves::add(std::string const& group, std::string const& name, int index)
    {
        return call("new", name, kwarg("index", index), kwarg("action_group", group));
    }

    fcurves action::fcurves()
    {
        return getattr<bxx::fcurves>("fcurves");
    }

    void keyframe::set(float time, float value)
    {
        set_time(time);
        set_value(value);
    }

    void keyframe::set_time(float time)
    {
        get_raw_struct()->vec[1][0] = time;
    }

    void keyframe::set_value(float value)
    {
        get_raw_struct()->vec[1][1] = value;
    }

    float keyframe::get_time()
    {
        return get_raw_struct()->vec[1][0];
    }

    float keyframe::get_value()
    {
        return get_raw_struct()->vec[1][1];
    }

    void fcurve::add_points(int amount)
    {
        getattr("keyframe_points").call("add", amount);
    }

    std::string fcurve::get_data_path() const
    {
        return get_raw_struct()->rna_path;
    }

    int fcurve::get_index() const
    {
        return get_raw_struct()->array_index;
    }

    blender_ptr_iterable<bl_fcurve, bl_keyframe, keyframe, details::keyframe_len, details::get_keyframe>
        fcurve::points()
    {
        return get_raw_struct();
    }

    namespace details
    {
        size_t keyframe_len(bl_fcurve* curve)
        {
            return curve->totvert;
        }

        bl_keyframe* get_keyframe(bl_fcurve* curve, size_t index)
        {
            return curve->bezt + index;
        }
    }
}

