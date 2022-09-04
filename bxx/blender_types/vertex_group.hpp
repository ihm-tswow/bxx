#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/object.hpp>
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
        vertex_group(object parent, bl_vertex_group* group);

        PYFIELD(std::uint32_t, index)

        python_object get_pyobject();
        std::string get_name();
        void add(
            std::vector<std::uint32_t> const& indices,
            float weight,
            VertexAssignMode mode = VertexAssignMode::ADD
        );

        void remove(std::vector<std::uint32_t> const& indices);
        float weight(std::uint32_t index);
    private:
        object m_parent;
    };
}