#pragma once

#include <bxx/blender_types/object.hpp>

#include <type_traits>

namespace bxx
{
    template <typename T>
    constraint_type get_constraint_type()
    {
        if constexpr (std::is_same<T, action_constraint>()) return constraint_type::ACTION;
        if constexpr (std::is_same<T, armature_constraint>()) return constraint_type::ARMATURE;
        if constexpr (std::is_same<T, camera_solver_constraint>()) return constraint_type::CAMERA_SOLVER;
        // todo: compile-time
        throw std::runtime_error("Wrong constraint template");
    }

    template <typename T>
    T object::add_constraint(std::string const& name)
    {
        constraint_type type = get_constraint_type<T>();
        std::string type_name = std::string(magic_enum::enum_name<constraint_type>(type));
        python_object constraints = getattr("constraints");
        python_object constraint_py = constraints.call("new", kwarg("type", type_name));
        constraint_py.setattr("name", name);
        return T(constraint_py);
    }

    template <typename T>
    T object::get_constraint(std::uint32_t index)
    {
        constraint_type type = get_constraint_type<T>();
        std::string type_name = std::string(magic_enum::enum_name<constraint_type>(type));
        python_object constraints = getattr("constraints");
        python_object constraint_py = constraints.get_item(index);
        return T(constraint_py);
    }
}