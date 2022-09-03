#include "action.hpp"
#include "exec.hpp"

#include "fmt/core.h"

namespace bxx
{
    fcurve action::find_fcurve(std::string const& name, int index)
    {
        return fcurve(*this, eval_ptr<bl_fcurve>(fmt::format("out = {}.fcurves.find('{}',index = {})", get_name_full(), name, index)));
    }

    std::string action::get_type_path() const
    {
        return "bpy.data.actions";
    }

    fcurve action::add_fcurve(std::string const& group, std::string const& name, int index)
    {
        return fcurve(*this,eval_ptr<bl_fcurve>(fmt::format("out = {}.fcurves.new('{}', index = {}, action_group='{}').as_pointer()",get_name_full(),name,index, group)));
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

    fcurve::fcurve(action parent, bl_fcurve* curve)
        : blender_py_struct<bl_fcurve>(curve)
        , m_action(parent)
    {}

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

    python_object fcurve::get_pyobject()
    {
        return eval_pyobject(fmt::format("out = {}.fcurves.find('{}',index={})", m_action.get_name_full(), get_raw_struct()->rna_path, get_raw_struct()->array_index));
    }

    action action::create(std::string const& name)
    {
        return action(eval_ptr<bl_action>(fmt::format("out = bpy.data.actions.new(name='{}').as_pointer()",name)));
    }
}

