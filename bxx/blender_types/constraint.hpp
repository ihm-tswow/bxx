#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/objects/blender_struct.hpp>

#include <string>
#include <type_traits>

namespace bxx
{
    enum class constraint_type {
        CAMERA_SOLVER,
        FOLLOW_TRACK,
        OBJECT_SOLVER,
        COPY_LOCATION,
        COPY_ROTATION,
        COPY_SCALE,
        COPY_TRANSFORMS,
        LIMIT_DISTANCE,
        LIMIT_LOCATION,
        LIMIT_ROTATION,
        LIMIT_SCALE,
        MAINTAIN_VOLUME,
        TRANSFORM,
        TRANSFORM_CACHE,
        CLAMP_TO,
        DAMPED_TRACK,
        IK,
        LOCKED_TRACK,
        SPLINE_IK,
        STRETCH_TO,
        TRACK_TO,
        ACTION,
        ARMATURE,
        CHILD_OF,
        FLOOR,
        FOLLOW_PATH,
        PIVOT,
        SHRINKWRAP
    };

    enum class owner_space
    {
        WORLD,
        CUSTOM,
        POSE,
        LOCAL_WITH_PARENT,
        LOCAL
    };

    enum class target_space
    {
        WORLD,
        CUSTOM,
        POSE,
        LOCAL_WITH_PARENT,
        LOCAL,
        LOCAL_OWNER_ORIENT,
    };

    class object;
    class action;

    class constraint : public blender_py_struct<bl_constraint>
    {
    public:
        using blender_py_struct<bl_constraint>::blender_py_struct;
        PYFIELD_DECL(bool, active)
        PYFIELD_DECL(bool, enabled)
        PYFIELD_DECL(float, error_location)
        PYFIELD_DECL(float, error_rotation)
        PYFIELD_DECL(float, influence)
        PYFIELD_DECL(bool, is_override_data)
        PYFIELD_DECL(bool, is_valid)
        PYFIELD_DECL(bool, mute)
        PYFIELD_STRINGENUM_DECL(owner_space, owner_space)
        PYFIELD_DECL(bool, show_expanded)
        PYFIELD_DECL(object, space_object)
        PYFIELD_DECL(std::string, space_subtarget)
        PYFIELD_STRINGENUM_DECL(target_space, target_space)
        PYFIELD_STRINGENUM_DECL(constraint_type, type)
    };

    enum class mix_mode {
        BEFORE_FULL,
        BEFORE,
        BEFORE_SPLIT,
        AFTER_FULL,
        AFTER
    };

    enum class transform_channel {
        LOCATION_X,
        LOCATION_Y,
        LOCATION_Z,
        ROTATION_X,
        ROTATION_Y,
        ROTATION_Z,
        SCALE_X,
        SCALE_Y,
        SCALE_Z
    };

    class action_constraint : public constraint
    {
    public:
        using constraint::constraint;
        PYFIELD_DECL(action, action)
        PYFIELD_DECL(float, eval_time)
        PYFIELD_DECL(std::int32_t, frame_end)
        PYFIELD_DECL(std::int32_t, frame_start)
        PYFIELD_DECL(float, max)
        PYFIELD_DECL(float, min)
        PYFIELD_STRINGENUM_DECL(mix_mode, mix_mode)
        PYFIELD_DECL(std::string, subtarget)
        PYFIELD_DECL(object, target)
        PYFIELD_STRINGENUM_DECL(transform_channel, transform_channel)
        PYFIELD_DECL(bool,use_bone_object_action)
        PYFIELD_DECL(bool,use_eval_time)
    };

    class armature_constraint : public constraint
    {
    public:
        using constraint::constraint;
        // todo: add targets handlers
        PYFIELD_DECL(bool,use_bone_envelopes)
        PYFIELD_DECL(bool,use_current_location)
        PYFIELD_DECL(bool,use_deform)
    };

    class camera_solver_constraint : public constraint
    {
    public:
        using constraint::constraint;
        PYFIELD_DECL(python_object, clip)
        PYFIELD_DECL(bool,use_active_clip)
    };

    class constraints : public blender_py_iterable<constraint>
    {
    public:
        using blender_py_iterable<constraint>::blender_py_iterable;
        template <typename T>
        T add(std::string const& name);

        template <typename T>
        T get(std::uint32_t index);
    };
}

#include <bxx/blender_types/constraint.ipp>