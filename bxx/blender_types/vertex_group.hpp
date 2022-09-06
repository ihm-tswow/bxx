#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/objects/blender_struct.hpp>

#include <vector>

namespace bxx
{
    enum class VertexAssignMode
    {
        REPLACE,
        ADD,
        SUBTRACT
    };

    class vertex_group : public blender_py_struct<bl_vertex_group>
    {
    public:
        using blender_py_struct<bl_vertex_group>::blender_py_struct;

        PYFIELD(std::uint32_t, index)
        std::string get_name();
        void add(
            std::vector<std::uint32_t> const& indices,
            float weight,
            VertexAssignMode mode = VertexAssignMode::ADD
        );

        void remove(std::vector<std::uint32_t> const& indices);
        float weight(std::uint32_t index);
    };

    class vertex_groups : public blender_py_iterable<vertex_group>
    {
    public:
        using blender_py_iterable<vertex_group>::blender_py_iterable;
        vertex_group find(std::string const& name);
        vertex_group add(std::string const& name);
    };
}