#include <bxx/blender_types/constraint.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/armature.hpp>
#include <bxx/blender_types/action.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_constraint_types.h>
#pragma warning(pop)

namespace bxx
{
    PYFIELD_IMPL(constraint, bool, active)
    PYFIELD_IMPL(constraint, bool, enabled)
    PYFIELD_IMPL(constraint, float, error_location)
    PYFIELD_IMPL(constraint, float, error_rotation)
    PYFIELD_IMPL(constraint, float, influence)
    PYFIELD_IMPL(constraint, bool, is_override_data)
    PYFIELD_IMPL(constraint, bool, is_valid)
    PYFIELD_IMPL(constraint, bool, mute)
    PYFIELD_STRINGENUM_IMPL(constraint, owner_space, owner_space)
    PYFIELD_IMPL(constraint, bool, show_expanded)
    PYFIELD_IMPL(constraint, object, space_object)
    PYFIELD_IMPL(constraint, std::string, space_subtarget)
    PYFIELD_STRINGENUM_IMPL(constraint, target_space, target_space)
    PYFIELD_STRINGENUM_IMPL(constraint, constraint_type, type)

    // action_constraint
    PYFIELD_IMPL(action_constraint, action, action)
    PYFIELD_IMPL(action_constraint, float, eval_time)
    PYFIELD_IMPL(action_constraint, std::int32_t, frame_end)
    PYFIELD_IMPL(action_constraint, std::int32_t, frame_start)
    PYFIELD_IMPL(action_constraint, float, max)
    PYFIELD_IMPL(action_constraint, float, min)
    PYFIELD_STRINGENUM_IMPL(action_constraint, mix_mode, mix_mode)
    PYFIELD_IMPL(action_constraint, std::string, subtarget)
    PYFIELD_IMPL(action_constraint, object, target)
    PYFIELD_STRINGENUM_IMPL(action_constraint, transform_channel, transform_channel)
    PYFIELD_IMPL(action_constraint, bool, use_bone_object_action)
    PYFIELD_IMPL(action_constraint, bool, use_eval_time)

    // armature_constraint
    PYFIELD_IMPL(armature_constraint, bool, use_bone_envelopes)
    PYFIELD_IMPL(armature_constraint, bool, use_current_location)
    PYFIELD_IMPL(armature_constraint, bool, use_deform)

    // camera_solver_constraint
    PYFIELD_IMPL(camera_solver_constraint, python_object, clip)
    PYFIELD_IMPL(camera_solver_constraint, bool, use_active_clip)
}