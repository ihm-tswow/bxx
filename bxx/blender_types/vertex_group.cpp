#include <bxx/blender_types/vertex_group.hpp>
#include <bxx/objects/python_list.hpp>

#include <magic_enum.hpp>

namespace bxx
{
    void vertex_group::add(
        std::vector<std::uint32_t> const& indices,
        float weight,
        VertexAssignMode mode
    ) {
        call("add"
            , python_list(indices)
            , weight
            , std::string(magic_enum::enum_name<VertexAssignMode>(mode))
        );
    }

    void vertex_group::remove(std::vector<std::uint32_t> const& indices)
    {
        call("remove", python_list(indices));
    }

    float vertex_group::weight(std::uint32_t index)
    {
        return call<float>("weight", index);
    }

    std::string vertex_group::get_name()
    {
        return get_raw_struct()->name;
    }
}