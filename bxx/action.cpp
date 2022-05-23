#include "action.hpp"
#include "exec.hpp"

#include "makesdna/DNA_action_types.h"
#include "makesdna/DNA_anim_types.h"

#include "fmt/core.h"

bxx::fcurve bxx::action::find_fcurve(std::string const& name, int index)
{
    return bxx::fcurve(*this, eval_ptr<bl_fcurve>({
        fmt::format("{}.fcurves.find('{}',{}).as_pointer()",get_full_name(),name,index)
    }));
}

bxx::fcurve bxx::action::add_fcurve(std::string const& group, std::string const& name, int index)
{
    return fcurve(*this, eval_ptr<bl_fcurve>({
        fmt::format("out = {}.fcurves.new('{}', index = {}, action_group='{}').as_pointer()",get_full_name(),name,index, group)
    }));
}

std::string bxx::action::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::action::get_full_name() const
{
    return fmt::format("bpy.data.actions['{}']", get_name());
}

bl_action* bxx::action::get_raw()
{
    return m_raw;
}

bxx::keyframe::keyframe(bl_keyframe* raw)
    : m_raw(raw)
{}

void bxx::keyframe::set(float time, float value)
{
    set_time(time);
    set_value(value);
}

void bxx::keyframe::set_time(float time)
{
    m_raw->vec[1][0] = time;
}

void bxx::keyframe::set_value(float value)
{
    m_raw->vec[1][1] = value;
}

float bxx::keyframe::get_time()
{
    return m_raw->vec[1][0];
}

float bxx::keyframe::get_value()
{
    return m_raw->vec[1][1];
}

bl_keyframe* bxx::keyframe::get_raw()
{
    return m_raw;
}

bxx::fcurve::fcurve(bxx::action const& parent, bl_fcurve* raw)
    : m_parent(parent)
    , m_raw(raw)
{}

void bxx::fcurve::add_points(int amount)
{
    exec({
        fmt::format("{}.keyframe_points.add({})", get_full_name(), amount)
    });
}

int bxx::fcurve::num_points() const
{
    return m_raw->totvert;
}

bxx::keyframe bxx::fcurve::get_point(int index)
{
    return bxx::keyframe(m_raw->bezt + index);
}

bl_fcurve* bxx::fcurve::get_raw()
{
    return m_raw;
}

std::string bxx::fcurve::get_data_path() const
{
    return m_raw->rna_path;
}

std::string bxx::fcurve::get_name() const
{
    return fmt::format("{}[{}]", get_data_path(), get_index());
}

std::string bxx::fcurve::get_full_name() const
{
    return fmt::format("{}.fcurves.find('{}',index = {})", m_parent.get_full_name(), get_data_path(), get_index());
}

int bxx::fcurve::get_index() const
{
    return m_raw->array_index;
}

bxx::action::action(bl_action* raw)
    : m_raw(raw)
{}

bxx::action bxx::action::create(std::string const& name)
{
    return bxx::action(eval_ptr<bl_action>({
        fmt::format("out = bpy.data.actions.new(name='{}').as_pointer()",name)
    }));
}

