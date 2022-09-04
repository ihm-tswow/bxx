#include <bxx/blender_types/vertex_group.hpp>
#include <bxx/objects/python_list.hpp>

#include <magic_enum.hpp>

namespace bxx
{
    vertex_group::vertex_group(object parent, bl_vertex_group* group)
        : blender_py_struct<bl_vertex_group>(group)
        , m_parent(parent)
    {}

    void vertex_group::add(
        std::vector<std::uint32_t> const& indices,
        float weight,
        VertexAssignMode mode
    ) {
        get_pyobject().call("add"
            , python_list(indices)
            , weight
            , std::string(magic_enum::enum_name<VertexAssignMode>(mode))
        );
    }

    void vertex_group::remove(std::vector<std::uint32_t> const& indices)
    {
        get_pyobject().call("remove", python_list(indices));
    }

    float vertex_group::weight(std::uint32_t index)
    {
        return get_pyobject().call<float>("weight", index);
    }

    std::string vertex_group::get_name()
    {
        return get_raw_struct()->name;
    }

    python_object vertex_group::get_pyobject()
    {
        return eval_pyobject("out = {}.vertex_groups['{}']",m_parent.get_name_full(), get_name());
    }
}