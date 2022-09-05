#include <bxx/blender_types/action.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    fcurve action::find_fcurve(std::string const& name, int index)
    {
        return getattr("fcurves").call("find", name, kwarg("index", index));
    }

    fcurve action::add_fcurve(std::string const& group, std::string const& name, int index)
    {
        return getattr("fcurves").call("new", name, kwarg("index", index), kwarg("action_group", group));
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

    int fcurve::num_points() const
    {
        return get_raw_struct()->totvert;
    }

    keyframe fcurve::get_point(int index)
    {
        return keyframe(get_raw_struct()->bezt + index);
    }

    std::string fcurve::get_data_path() const
    {
        return get_raw_struct()->rna_path;
    }

    int fcurve::get_index() const
    {
        return get_raw_struct()->array_index;
    }

    action action::create(std::string const& name)
    {
        return action(eval_pyobject("out = bpy.data.actions.new(name='{}')",name));
    }
}

